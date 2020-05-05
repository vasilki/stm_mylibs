/*
 * timers.c
 *
 *  Created on: 27 июля 2018 г.
 *      Author: vasilek
 */

#include "stm32f4xx_hal.h"

#if defined(__STM32F4xx_HAL_TIM_H) || defined(STM32F4xx_HAL_TIM_H)

typedef struct
{
  TIM_HandleTypeDef *htim;
  uint32_t time_ms;
  uint32_t time_sec;
  uint32_t periodnumber;
}T_TIMERS;

static T_TIMERS GL_TIMERS;

static void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim);

void tim_UpdatePeriod(void)
{
  GL_TIMERS.periodnumber++;

  return;
}

unsigned int tim_GetPeriod(void)
{
  return GL_TIMERS.periodnumber;
}

void tim_StartTimer(TIM_HandleTypeDef *par_htim)
{
 // HAL_TIM_Base_Start(par_htim);
  HAL_TIM_Base_Start_IT(par_htim);
  return;
}



void tim_InitTimer(TIM_HandleTypeDef *par_htim)
{
  GL_TIMERS.htim = par_htim;
  GL_TIMERS.periodnumber = 0;
  GL_TIMERS.time_ms = 0;
  GL_TIMERS.time_sec = 0;

  tim_StartTimer(par_htim);
  
  return;
}


void tim_UpdateTimeFromStartMS(TIM_HandleTypeDef *par_htim)
{
  GL_TIMERS.time_ms += 1;
  GL_TIMERS.time_sec = GL_TIMERS.time_ms / 1000;

  return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *par_htim)
{
  if(GL_TIMERS.htim->Instance == par_htim->Instance)
  {
    tim_UpdateTimeFromStartMS(par_htim);
  }
  else
  {
    /*nothing to do - it is not our interrupt*/
  }

  return;
}

uint32_t tim_GetTimeFromStartMS(void)
{
  return GL_TIMERS.time_ms;
}

uint32_t tim_GetTimeFromStartSEC(void)
{
  return GL_TIMERS.time_sec;
}

#endif /* defined(__STM32F4xx_HAL_TIM_H) || defined(STM32F4xx_HAL_TIM_H) */


