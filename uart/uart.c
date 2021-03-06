/*
 * uart.c
 *
 *  Created on: 28 июля 2018 г.
 *      Author: vasilek
 */

#include "stm32f4xx_hal.h"

#ifdef __STM32F4xx_HAL_UART_H
#include <string.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "uart.h"

extern uint8_t GL_PROJECT_NAME[];

static UART_HandleTypeDef *GL_UART;

//static HAL_UART_StateTypeDef GL_UART_STATE = HAL_UART_STATE_RESET;
static uint8_t GL_TEMP_UART_BUFFER[K_MAX_STRING] = {0};
static uint32_t GL_TEMP_UART_BUFFER_OFFSET = 0;

void uart_Init(UART_HandleTypeDef *par_uart)
{
  HAL_StatusTypeDef loc_status;

  GL_UART = par_uart;
  HAL_UART_Transmit(GL_UART, (uint8_t*)"\n\r", strnlen("\n\r",K_MAX_STRING),0xFFFF);
  circular_buffer_init();
  loc_status = HAL_UART_Receive_IT(GL_UART, &GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET], sizeof(GL_TEMP_UART_BUFFER[0]));

  return;
}

void uart_Printf(const uint8_t *par_string)
{
  /*HAL_StatusTypeDef loc_status;*/
  HAL_UART_Transmit(GL_UART, (uint8_t*)par_string, strnlen((char*)par_string,K_MAX_STRING),0xFFFF);

  return;
}

void uart_PrintfBuildVersion()
{
  /*HAL_StatusTypeDef loc_status;*/
  uint8_t loc_buf[K_MAX_STRING];

  snprintf((char*)loc_buf,sizeof(loc_buf),"Project:%s Date:%s Time:%s\n\r",GL_PROJECT_NAME,__DATE__,__TIME__);
  uart_Printf(loc_buf);

  return;
}


static void uart_SetBinString(unsigned int par_value, char *par_string)
{
  int loc_i;
  unsigned int loc_value = par_value;

  par_string[sizeof(unsigned int)*8] = 0;

  for(loc_i = sizeof(unsigned int)*8 - 1;loc_i>=0;loc_i--)
  {
    par_string[loc_i] = 0x30 + (loc_value & 0x1);
    loc_value = loc_value >> 1;
  }

  return;
}



void uart_PrintfInteger(int par_value, const char *par_base)
{
  char loc_buff[40];

  if((strcmp(par_base,"dec")==0)||(strcmp(par_base,"DEC")==0))
  {
    sprintf(loc_buff,"%d\n\r",par_value);
  }
  else if((strcmp(par_base,"hex")==0)||(strcmp(par_base,"HEX")==0))
  {
    sprintf(loc_buff,"0x%08x\n\r",par_value);
  }
  else if((strcmp(par_base,"oct")==0)||(strcmp(par_base,"OCT")==0))
  {
    sprintf(loc_buff,"%o\n\r",par_value);
  }
  else if((strcmp(par_base,"bin")==0)||(strcmp(par_base,"BIN")==0))
  {
    uart_SetBinString(par_value,loc_buff);
    strcat(loc_buff,"\n\r");
  }
  else
  {
    /*nothing to do*/
  }

  HAL_UART_Transmit(GL_UART, (uint8_t*)loc_buff, strnlen(loc_buff,K_MAX_STRING),0xFFFF);

  return;
}


void uart_Scanf(uint8_t *par_buffer, uint32_t *par_size)
{
  if((par_buffer != 0) && (par_size != 0))
  {
    circular_buffer_get(par_buffer, par_size);
  }
  else
  {
    /*nothing to do*/
  }

  return;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *par_uart)
{
  HAL_StatusTypeDef loc_status;

  if (par_uart == GL_UART)
  {
    if((GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET] == '\r') || GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET] == '\n')
    {
      /*@@SHOUD BE TAKEN OUT FROM INTERRUPT*/
      circular_buffer_add(GL_TEMP_UART_BUFFER,strnlen(GL_TEMP_UART_BUFFER,K_MAX_STRING) - 1); /*-1 - remove carriage return*/
      memset(GL_TEMP_UART_BUFFER,0,GL_TEMP_UART_BUFFER_OFFSET + 1); /*+1 - carriage return or '\n'*/
      GL_TEMP_UART_BUFFER_OFFSET = 0;
    }
    else
    {
      GL_TEMP_UART_BUFFER_OFFSET = (GL_TEMP_UART_BUFFER_OFFSET + 1) % K_MAX_STRING;
    }
    loc_status = HAL_UART_Receive_IT(GL_UART, &GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET], sizeof(GL_TEMP_UART_BUFFER[0]));
  }
  else
  {
    /*nothing to do*/
  }

  return;
}

void uart_ReplaceUnprintableCharacters(uint8_t *par_string, uint32_t par_legnth, uint8_t par_character)
{
  uint32_t loc_count = 0;

  for(loc_count = 0; loc_count < par_legnth; loc_count++)
  {
    if((par_string[loc_count] < 0x20) || (par_string[loc_count] > 0x7E))
    {
      par_string[loc_count] = par_character;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return;
}


#endif /* __STM32F4xx_HAL_UART_H */



