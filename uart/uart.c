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

#define K_MAX_STRING 200
extern uint8_t GL_PROJECT_NAME[];

static UART_HandleTypeDef *GL_UART;

//static HAL_UART_StateTypeDef GL_UART_STATE = HAL_UART_STATE_RESET;
static uint8_t GL_TEMP_UART_BUFFER[K_MAX_STRING];
static uint32_t GL_TEMP_UART_BUFFER_OFFSET = 0;

extern uint8_t GL_TEXT[200];

void uart_Init(UART_HandleTypeDef *par_uart)
{
  HAL_StatusTypeDef loc_status;

  GL_UART = par_uart;
  HAL_UART_Transmit(GL_UART, (uint8_t*)"\n\r", strnlen("\n\r",K_MAX_STRING),0xFFFF);
  circular_buffer_init();
  memset(GL_TEMP_UART_BUFFER,0,sizeof(GL_TEMP_UART_BUFFER));
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
    if(*par_size > 0)
    {
      snprintf(GL_TEXT,sizeof(GL_TEXT),"%s",par_buffer);
      uart_Printf(par_buffer);
    }
    else
    {
      /*nothing to do*/
    }
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
 // uint8_t loc_byte = 0;

  if (par_uart == GL_UART)
  {
    if((GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET] == '\r') || GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET] == '\n')
    {
      /*@@SHOUD BE TAKEN OUT FROM INTERRUPT*/
      circular_buffer_add(GL_TEMP_UART_BUFFER,strnlen((int8_t*)GL_TEMP_UART_BUFFER,K_MAX_STRING));
      GL_TEMP_UART_BUFFER_OFFSET = 0;
    }
    else
    {
      GL_TEMP_UART_BUFFER_OFFSET = (GL_TEMP_UART_BUFFER_OFFSET + 1) % K_MAX_STRING;
    }

    //GL_UART_STATE = HAL_UART_STATE_RESET;
    loc_status = HAL_UART_Receive_IT(GL_UART, &GL_TEMP_UART_BUFFER[GL_TEMP_UART_BUFFER_OFFSET], sizeof(GL_TEMP_UART_BUFFER[0]));
    //GL_UART_STATE = HAL_UART_STATE_READY;
  }
  else
  {
    /*nothing to do*/
  }

  return;
}

/*

void HAL_UART_ErrorCallback(UART_HandleTypeDef *par_uart)
{
  uint32_t loc_error;
  if (par_uart == GL_UART)
  {
    loc_error = HAL_UART_GetError(GL_UART);
    snprintf(GL_TEXT,sizeof(GL_TEXT),"ERROR:%d",loc_error);
  }
  else
  {
    //nothing to do
  }
  return;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *par_uart)
{

  return;
}
*/

#endif /* __STM32F4xx_HAL_UART_H */



