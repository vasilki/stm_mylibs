/*
 * timers.c
 *
 *  Created on: 27 июля 2018 г.
 *      Author: vasilek
 */

#include "stm32f4xx_hal.h"

static uint32_t gl_time = 0;
static uint32_t gl_time_ms = 0;

static void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim);

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



void tim_InitTimer(TIM_HandleTypeDef *par_htim)
{
  tim_StartTimer(par_htim);
  
  return;
}


void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim)
{
  gl_time_ms += 1;

  return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *par_htim)
{
  tim_UpdateTimeFromStartMS(par_htim);

  return;
}

uint32_t tim_GetTimeFromStartMS(void)
{
  return gl_time_ms;
}

uint32_t tim_GetTimeFromStartSEC(void)
{
  return tim_GetTimeFromStartMS() / 1000;
}
