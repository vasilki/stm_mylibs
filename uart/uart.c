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

#define K_MAX_STRING 200
extern uint8_t GL_PROJECT_NAME[];

static UART_HandleTypeDef *GL_UART;

void uart_Init(UART_HandleTypeDef *par_uart)
{
  GL_UART = par_uart;
  HAL_UART_Transmit(GL_UART, (uint8_t*)"\n\r", strnlen("\n\r",K_MAX_STRING),0xFFFF);

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
  uint16_t loc_size = 0;
  HAL_StatusTypeDef loc_status;
  HAL_UART_StateTypeDef loc_state;

  if((GL_UART != 0) && (par_buffer != 0) && (par_size != 0))
  {
    loc_state = HAL_UART_STATE_BUSY_TX_RX;
    while ((loc_state == HAL_UART_STATE_BUSY_RX) || (loc_state == HAL_UART_STATE_BUSY_TX_RX))
    {
      loc_state = HAL_UART_GetState(GL_UART);
    }

    loc_status = HAL_UART_Receive_IT(GL_UART, par_buffer, *par_size);
    if(loc_status != HAL_OK)
    {
      *par_size = 0;
    }
    else
    {
      *par_size = loc_status;
      /*nothing to do*/
    }
  }
  else
  {
    /*nothing to do*/
  }

  return;
}

static HAL_UART_StateTypeDef GL_UART_STATE = HAL_UART_STATE_RESET;
#define K_CIRCULAR_BUFF_LENGTH 10
#define K_SEGMENT_BUFF_SIZE 20
static uint8_t GL_UART_BUFF[K_CIRCULAR_BUFF_LENGTH*K_SEGMENT_BUFF_SIZE];
static uint32_t GL_UART_BUFF_SIZE;

typedef struct
{
  uint8_t buffer[K_CIRCULAR_BUFF_LENGTH*K_SEGMENT_BUFF_SIZE];
  uint32_t data_size[K_CIRCULAR_BUFF_LENGTH];
  uint8_t *offset;
  uint32_t segment_size;
  uint32_t circular_length;
  uint32_t get_index;
  uint32_t add_offset;
}T_CIRCULAR_BUFFER;

static T_CIRCULAR_BUFFER GL_BUFFER;

static void uart_circbuff_init()
{
  GL_BUFFER.segment_size = K_SEGMENT_BUFF_SIZE;
  GL_BUFFER.circular_length = K_CIRCULAR_BUFF_LENGTH;
  memset(GL_BUFFER.buffer, 0, sizeof(GL_BUFFER.buffer));
  memset(GL_BUFFER.data_size, 0, sizeof(GL_BUFFER.data_size));
  GL_BUFFER.offset = &GL_BUFFER.buffer[0];
  GL_BUFFER.get_index = 0;
  GL_BUFFER.add_offset = 0;

  return;
}

static void uart_circbuff_add(uint8_t *par_buff, uint32_t par_size)
{
  uint32_t loc_size = 0;

  if((par_buff != 0) && (par_size > 0))
  {
    if(par_size > GL_BUFFER.segment_size)
    {
      loc_size = GL_BUFFER.segment_size;
    }
    else
    {
      loc_size = par_size;
    }

    memcpy(GL_BUFFER.offset,par_buff,loc_size);
    GL_BUFFER.offset += GL_BUFFER.segment_size;
    *GL_BUFFER.data_size = loc_size;
    GL_BUFFER.data_size++;


    if(GL_BUFFER.offset > &GL_BUFFER.buffer[sizeof(GL_BUFFER.buffer)])
    {
      GL_BUFFER.offset = &GL_BUFFER.buffer[0];
      GL_BUFFER.data_size = &GL_BUFFER.data_size[0];
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

static void uart_circbuff_get(uint8_t *par_buff, uint32_t *par_size)
{
  if((par_buff != 0) && (par_size != 0))
  {
    if(GL_BUFFER.data_size[GL_BUFFER.get_index] != 0)
    {
      memcpy(par_buff, &GL_BUFFER.buffer[GL_BUFFER.get_index * GL_BUFFER.segment_size],GL_BUFFER.data_size[GL_BUFFER.get_index]);
      *par_size = GL_BUFFER.data_size[GL_BUFFER.get_index];
      GL_BUFFER.data_size[GL_BUFFER.get_index] = 0;
      GL_BUFFER.get_index = (GL_BUFFER.get_index + 1) % GL_BUFFER.circular_length;
    }
    else
    {
      *par_size = 0;
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

  if ((par_uart != 0) && (GL_UART_STATE == HAL_UART_STATE_READY))
  {
    GL_UART_STATE = HAL_UART_STATE_READY;
    loc_status = HAL_UART_Receive_IT(GL_UART, GL_UART_BUFF, GL_UART_BUFF_SIZE);
  }
  else
  {

  }

  return;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *par_uart)
{

  return;
}

#endif /* __STM32F4xx_HAL_UART_H */



