/*
 * st7735_port.c
 *
 *  Created on: 25-Sept-2026
 *      Author: Admin
 */
#include "st7735_port.h"

extern SPI_HandleTypeDef hspi1;

static void ST7735_Port_Reset(void)
{
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);

	HAL_Delay(20);

	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);

	HAL_Delay(120);
}

int32_t ST7735_Port_Init(void)
{
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);

	ST7735_Port_Reset();

	return(0);
}

int32_t ST7735_Port_DeInit(void)
{
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
	return(0);
}

int32_t ST7735_Port_WriteReg(uint8_t reg, uint8_t *data, uint32_t length)
{
	HAL_StatusTypeDef status;
	/* Command mode */
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);

	/* Select display */
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);

	/* Send command */
	status = HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);

	/* Data mode */
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

	if(status == HAL_OK && length>0)
	{
		status = HAL_SPI_Transmit(&hspi1, data, length, HAL_MAX_DELAY);
	}

	/* DeSelect display */
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
	return (status == HAL_OK) ? 0:-1;
}

int32_t ST7735_Port_ReadReg(uint8_t reg, uint8_t *data)
{
	(void)reg;
	(void)data;
	return(-1);
}

int32_t ST7735_Port_SendData(uint8_t *data, uint32_t length)
{
	HAL_StatusTypeDef status;

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(&hspi1, data, length, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
	if(status == HAL_OK)
	{
		return 0;
	}

	return(-1);
}

int32_t ST7735_Port_RecvData(uint8_t *data, uint32_t length)
{
	(void)data;
	(void)length;

	return(-1);
}

int32_t ST7735_Port_GetTick(void)
{
	return(int32_t)HAL_GetTick();
}


