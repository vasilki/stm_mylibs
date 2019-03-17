/*
 * lcd_1602.h
 *
 *  Created on: 30 июля 2018 г.
 *      Author: vasilek
 */

#ifndef LCD_1602_H_
#define LCD_1602_H_

#include "stm32f401xe.h"

typedef struct
{
  GPIO_TypeDef *pLine;
  uint32_t RS;
  uint32_t EN;
  uint32_t D4;
  uint32_t D5;
  uint32_t D6;
  uint32_t D7;
}T_LCD_GPIO_Parameters;

void lcd_PrintStr(char *par_string);
void lcd_ClearLCDScreen();
void lcd_SetCursor(char Row, char Col);
void lcd_Init(T_LCD_GPIO_Parameters par_parameters /*GPIOx*/);

void lcd_CursorOFF();

void lcd_SetCursorFreeze();

void lcd_SetCursorBlink();

#endif /* LCD_1602_H_ */
