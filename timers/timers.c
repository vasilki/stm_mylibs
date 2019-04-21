/*
 * timers.c
 *
 *  Created on: 27 июля 2018 г.
 *      Author: vasilek
 */

#include "stm32f4xx_hal.h"

static uint32_t gl_time = 0;
static uint32_t gl_time_ms = 0;
static uint32_t gl_time_sec = 0;
//static uint32_t gl_freq = 0;

static void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim);
static void tim_UpdateTimeFromStartSEC(TIM_HandleTypeDef *par_htim);

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;

void tim_UpdatePeriod(void)
{
  gl_time++;
  return;
}

unsigned int tim_GetPeriod(void)
{
  return gl_time;
}

void tim_StartTimer(TIM_HandleTypeDef *par_htim)
{
 // HAL_TIM_Base_Start(par_htim);
  HAL_TIM_Base_Start_IT(par_htim);
  return;
}



void tim_InitTimer()
{
  /*division by two because APB2 bus*/
//  gl_freq = HAL_RCC_GetSysClockFreq() / 2;
  tim_StartTimer(&htim9);
  tim_StartTimer(&htim10);
  
  return;
}


void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim)
{
  gl_time_ms += 1;

  return;
}

void tim_UpdateTimeFromStartSEC(TIM_HandleTypeDef *par_htim)
{
  gl_time_sec += 1; 
    
  return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM9)
  {
    tim_UpdateTimeFromStartMS(htim);

  }
  else
  {
    /*nothing to do - it is not our interrupt*/
  }

  if(htim->Instance == TIM10)
  {
    tim_UpdateTimeFromStartSEC(htim);
  }
  else
  {
    /*nothing to do - it is not our interrupt*/
  }

  return;
}

uint32_t tim_GetTimeFromStartMS(void)
{
  return gl_time_ms;
}

uint32_t tim_GetTimeFromStartSEC(void)
{
  return gl_time_sec;
}
