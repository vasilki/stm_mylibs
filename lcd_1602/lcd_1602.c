/*
 * lcd_1602.c
 *
 *  Created on: 30 июля 2018 г.
 *      Author: vasilek
 *      Based on article https://goo.gl/dDGv4P with a lot of modifications for STM32 F401RE and HAL
 */

#include "lcd_1602.h"
#include <string.h>
#include "dwt_stm32_delay.h"
#include "uart.h"

/*Static variables defining pins*/

static uint32_t GL_PIN_RS;           // GPIO_Pin_0          // PB0
static uint32_t GL_PIN_EN;           // GPIO_Pin_1          // PB1
static uint32_t GL_PIN_D7;           // GPIO_Pin_7          // PB7
static uint32_t GL_PIN_D6;           // GPIO_Pin_6          // PB6
static uint32_t GL_PIN_D5;           // GPIO_Pin_5          // PB5
static uint32_t GL_PIN_D4;           // GPIO_Pin_4          // PB4

#define      GL_PIN_MASK  ((GL_PIN_RS | GL_PIN_EN | GL_PIN_D7 | GL_PIN_D6 | GL_PIN_D5 | GL_PIN_D4))
#define K_DELAY_FOR_STM32_F401RE            (10000000)

#define K_DELAY_MORE_THAN_39uS 50
#define K_DELAY_MORE_THAN_1530uS 2000
#define K_DELAY_MORE_THAN_100uS 150
#define K_DELAY_MORE_THAN_4100uS 4500
#define K_DELAY_MORE_THAN_30ms 31000

static GPIO_TypeDef *GL_LINE;


/*Delay function wich you should find out the coefficients of a*/
/*
void delay(int a)
{
    int i = 0;
    int f = 0;
    a=a*20;
    while(f < a)
    {
        while(i<60)
            {i++;}
        f++;
    }
}
*/

/*Delay using SysClockFreq (It was taken from LIB MT_WH1602)*/
/*
void delay(uint32_t par_us)
{
  extern uint32_t HAL_RCC_GetSysClockFreq();

  uint32_t loc_i = HAL_RCC_GetSysClockFreq();
  loc_i= (loc_i/K_DELAY_FOR_STM32_F401RE)*par_us;
  for (; loc_i != 0; loc_i--);

  return;
}
*/


/*delay in microseconds using DWT timer*/

void delay(uint32_t par_us)
{
  DWT_Delay_us(par_us);
  
  return;
}

/*Set RS pin*/
static void lcd_SetRS()
{
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_RS,GPIO_PIN_SET);
  
  return;
}

/*Reset RS pin*/
static void lcd_ResetRS()
{
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_RS,GPIO_PIN_RESET);
  
  return;
}

/*Set EN pin*/
static void lcd_SetEN()
{
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_EN,GPIO_PIN_SET);
  return;
}

/*Reset EN pin*/
static void lcd_ResetEN()
{
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_EN,GPIO_PIN_RESET);
  return;
}

/*Reset all used pins*/
static void lcd_ResetPins()
{
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_EN,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_RS,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_D7,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_D6,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_D5,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GL_LINE,GL_PIN_D4,GPIO_PIN_RESET);
  
  return;
}

/*Set most significant 4 bits for 4-bit bus communication*/
static void lcd_SetMostSignificant4Bits(char par_4bits)
{
  
  if(((par_4bits >> 7) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D7,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D7,GPIO_PIN_RESET);
  }
  
  if(((par_4bits >> 6) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D6,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D6,GPIO_PIN_RESET);
  }

  if(((par_4bits >> 5) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D5,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D5,GPIO_PIN_RESET);
  }
  
  if(((par_4bits >> 4) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D4,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D4,GPIO_PIN_RESET);
  }
  
  return;
}


/*Set less significant 4 bits for 4-bit bus communication*/
static void lcd_SetLessSignificant4Bits(char par_4bits)
{
  
  if(((par_4bits >> 3) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D7,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D7,GPIO_PIN_RESET);
  }
  
  if(((par_4bits >> 2) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D6,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D6,GPIO_PIN_RESET);
  }

  if(((par_4bits >> 1) & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D5,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D5,GPIO_PIN_RESET);
  }
  
  if((par_4bits & 0x1) == 1)
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D4,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GL_LINE,GL_PIN_D4,GPIO_PIN_RESET);
  }
  
  return;
}


/*Strobbing of EN pin*/
static void lcd_PulseLCD()
{
  lcd_ResetEN();
  delay(2);
  lcd_SetEN();
  delay(2);
  lcd_ResetEN();
  delay(2);
  
  return;
}

/*Send a byte to LCD*/
static void lcd_SendByte(char par_ByteToSend, char par_B_IsData)
{
  /*Set most significant 4 bits for 4-bit bus communication*/
  
  lcd_ResetPins();
  lcd_SetMostSignificant4Bits(par_ByteToSend);
  if (par_B_IsData == 1)
  {
    lcd_SetRS();
  }
  else
  {
    lcd_ResetRS();
  }

  lcd_PulseLCD();

  /*Set less significant 4 bits for 4-bit bus communication*/
  lcd_ResetPins();
  lcd_SetLessSignificant4Bits(par_ByteToSend);
  if (par_B_IsData == 1)
  {
    lcd_SetRS();
  }
  else
  {
    lcd_ResetRS();

  }
  
  lcd_PulseLCD();
  
  return;
}

/*Set cursor position (0,0) - is first ROW, first column*/
void lcd_SetCursor(char par_Row, char par_Col)
{
  char loc_address;
  
  if (par_Row == 0)
  {
    loc_address = 0;
  }
  else
  {
    loc_address = 0x40;
  }
  loc_address |= par_Col;
  loc_address |= (char)0x80;
  lcd_SendByte(loc_address, 0);
  delay(K_DELAY_MORE_THAN_39uS);

  return;
}

/*Cursor off*/
void lcd_CursorOFF()
{
  lcd_SendByte(0xC, 0);
  delay(K_DELAY_MORE_THAN_39uS);
  
  return;
}

/*Set freezing cursor*/
void lcd_SetCursorFreeze()
{
  lcd_SendByte(0xE, 0); 
  delay(K_DELAY_MORE_THAN_39uS);
  
  return;
}

/*Set blinking cursor*/
void lcd_SetCursorBlink()
{
  lcd_SendByte(0xF, 0); 
  delay(K_DELAY_MORE_THAN_39uS);
  
  return;
}


/*Clear LCD Screen*/
void lcd_ClearLCDScreen()
{
  lcd_SendByte(0x01, 0);
  delay(K_DELAY_MORE_THAN_1530uS);
  lcd_SendByte(0x02, 0);
  delay(K_DELAY_MORE_THAN_1530uS);

  return;
}


/*LCD initialization*/
void lcd_Init(T_LCD_GPIO_Parameters par_parameters /*GPIOx*/)
{
  DWT_Delay_Init();

  GL_LINE = par_parameters.pLine;
  GL_PIN_RS = par_parameters.RS;
  GL_PIN_EN = par_parameters.EN;
  GL_PIN_D4 = par_parameters.D4;
  GL_PIN_D5 = par_parameters.D5;
  GL_PIN_D6 = par_parameters.D6;
  GL_PIN_D7 = par_parameters.D7;

  lcd_ResetPins();
  /*Init delay more than 30 ms)*/
  delay(K_DELAY_MORE_THAN_30ms);

  /*Function set 1*/    
  lcd_SendByte(0x30,0);
  /*delay more than 4.1ms*/
  delay(K_DELAY_MORE_THAN_4100uS);    
  
  /*Function set 2*/    
  lcd_SendByte(0x30,0);
  delay(K_DELAY_MORE_THAN_100uS);    
  /*Function set 3*/ 
  lcd_SendByte(0x30,0);    
  delay(K_DELAY_MORE_THAN_39uS);    
  lcd_SendByte(0x20,0);    
  delay(K_DELAY_MORE_THAN_39uS);    
  lcd_SendByte(0x20,0); 
  delay(K_DELAY_MORE_THAN_39uS);    
 
  /*Set 4pins-bus, two rows, font2 (5x7)*/
  lcd_SendByte(0x28,0);
  delay(K_DELAY_MORE_THAN_39uS);   
  /*Display ON/OFF control*/
  lcd_SendByte(0xF,0);
  delay(K_DELAY_MORE_THAN_39uS);  
  /*Display clear*/
  lcd_SendByte(0x1,0);
  /*delay more than 1.53ms*/
  delay(K_DELAY_MORE_THAN_1530uS);      
  /*Entry mode set*/
  lcd_SendByte(0x60,0);
  delay(K_DELAY_MORE_THAN_39uS);
    

  return;
}

/*print string*/
void lcd_PrintStr(char *par_string)
{
  char *c;
  
  c = par_string;
  
  while ((c != 0) && (*c != 0))
  {
    lcd_SendByte(*c, 1);
    delay(K_DELAY_MORE_THAN_39uS);
    c++;
  }
  
  return;
}

