/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "st7735_port.h"
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WAVE_VCENTER 40
#define WAVE_HCENTER 32
#define WAVE_HEIGHT 160
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
/* USER CODE BEGIN PV */
ST7735_Object_t lcd;
ST7735_IO_t lcd_io;

uint8_t waveform[128];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DrawLine(ST7735_Object_t *lcd, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = x1-x0;
	int dy = y1-y0;

	int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

	if(steps == 0)
	{
		ST7735_LCD_Driver.SetPixel(lcd, x0, y0, color);
		return;
	}

	for(int i = 0; i<=steps; i++)
	{
		int x = x0 + (dx*i) / steps;
		int y = y0 + (dy*i) / steps;

		ST7735_LCD_Driver.SetPixel(lcd, x, y, color);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  lcd_io.Init = ST7735_Port_Init;
  lcd_io.DeInit = ST7735_Port_DeInit;
  lcd_io.WriteReg = ST7735_Port_WriteReg;
  lcd_io.ReadReg = ST7735_Port_ReadReg;
  lcd_io.SendData = ST7735_Port_SendData;
  lcd_io.RecvData = ST7735_Port_RecvData;
  lcd_io.GetTick = ST7735_Port_GetTick;

  ST7735_RegisterBusIO(&lcd, &lcd_io);

  int32_t ret;

  ret = ST7735_Init(&lcd,ST7735_FORMAT_DEFAULT, ST7735_ORIENTATION_PORTRAIT);

  if(ret != ST7735_OK)
  {
	  Error_Handler();
  }
  ret = ST7735_LCD_Driver.FillRect(&lcd, 0, 0, ST7735_WIDTH,ST7735_HEIGHT,0x0000);

  if(ret != ST7735_OK)
      {
    	  Error_Handler();
      }
  /*Testing screen RGB*/
//  ret = ST7735_LCD_Driver.FillRect(&lcd, 0, 0, 128, 50, 0x001F);
//  ret = ST7735_LCD_Driver.FillRect(&lcd, 0, 50, 128, 50, 0x07E0);
//  ret = ST7735_LCD_Driver.FillRect(&lcd, 0, 100, 128, 60, 0xF800);

  for(uint32_t x=0; x<128; x+=16)
  {
	  ret = ST7735_LCD_Driver.DrawVLine(&lcd, x, 0, 160, 0x39E7);
  }

  if(ret != ST7735_OK)
      {
    	  Error_Handler();
      }

  for(uint32_t y=0; y<160; y+=16)
    {
  	  ret = ST7735_LCD_Driver.DrawHLine(&lcd, 0, y, 128, 0x39E7);
    }

  if(ret != ST7735_OK)
	{
	  	  Error_Handler();
	}

  uint32_t sample_count = sizeof(waveform) / sizeof(waveform[0]);

  for(uint32_t i=0; i<128; i++)
  {
	  waveform[i] = (uint8_t)(80.0f + 50.0f*sinf(i * 2.0f * M_PI / 64.0f));
  }

  uint8_t wave_min = waveform[0];
  uint8_t wave_max = waveform[0];

  for(uint32_t i = 1; i< sample_count; i++)
  {
	  if(waveform[i] < wave_min)
		  wave_min = waveform[i];

	  if(waveform[i]>wave_max)
		  wave_max = waveform[i];
  }

  uint32_t y_step = (WAVE_HEIGHT -1) / (sample_count - 1);
  uint8_t wave_range = wave_max-wave_min;

  if(wave_range == 0)
  {
	  wave_range = 1;
  }

  for(uint32_t i=0; i< (sample_count-1); i++)
  {
	  uint32_t x0 = ((uint32_t)(waveform[i] - wave_min) * 127) / wave_range;
	  uint32_t x1 = ((uint32_t)(waveform[i+1] - wave_min) * 127) / wave_range;

	  DrawLine(&lcd, x0, i*y_step, x1, (i+1)*y_step, 0x07E0);
  }


  if(ret != ST7735_OK)
    {
  	  Error_Handler();
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LPUART1_TX_Pin LPUART1_RX_Pin */
  GPIO_InitStruct.Pin = LPUART1_TX_Pin|LPUART1_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF12_LPUART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_RESET_Pin */
  GPIO_InitStruct.Pin = TFT_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_CS_Pin */
  GPIO_InitStruct.Pin = TFT_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_CS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
