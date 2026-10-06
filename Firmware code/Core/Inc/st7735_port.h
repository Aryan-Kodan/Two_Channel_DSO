/*
 * st7735_port.h
 *
 *  Created on: 25-Sept-2026
 *      Author: Admin
 */

#ifndef INC_ST7735_PORT_H_
#define INC_ST7735_PORT_H_

#include "main.h"
#include "st7735.h"

int32_t ST7735_Port_Init(void);
int32_t ST7735_Port_DeInit(void);
int32_t ST7735_Port_WriteReg(uint8_t reg, uint8_t *data, uint32_t length);
int32_t ST7735_Port_ReadReg(uint8_t reg, uint8_t *data );
int32_t ST7735_Port_SendData(uint8_t *data, uint32_t length);
int32_t ST7735_Port_RecvData(uint8_t *data, uint32_t length);
int32_t ST7735_Port_GetTick(void);

#endif /* INC_ST7735_PORT_H_ */
