#include "dwt_stm32_delay.h"


/**
 * @brief  Initializes DWT_Clock_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 *         1: clock cycle counter not started
 *         0: clock cycle counter works
 */
uint32_t DWT_Delay_Init(void)
{
  static unsigned char loc_B_WasInitialized = 0;

  if(loc_B_WasInitialized == 0)
  {
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;

    /* 3 NO OPERATION instructions */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");

    /* Check if clock cycle counter has started */
    if(DWT->CYCCNT)
    {
      loc_B_WasInitialized = 0; /*clock cycle counter started*/
    }
    else
    {
      loc_B_WasInitialized = 1; /*clock cycle counter not started*/
    }
  }
  else
  {
    /*nothing to do*/
  }

  return loc_B_WasInitialized;
}

/* Use DWT_Delay_Init (); and DWT_Delay_us (microseconds) in the main */
