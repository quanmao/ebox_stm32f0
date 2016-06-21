#include "ebox_common.h"


extern "C" {
#include "ebox_core.h"

    system_clock_t system_clock;
    __IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
     static void get_system_clock(system_clock_t *clock);
   
    void ebox_init(void)
    {
        get_system_clock(&system_clock);
        SysTick_Config(system_clock.core/1000);//  每隔 (nhz/168,000,000)s产生一次中断
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    }
    uint64_t micros(void)
    {
        if((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK())) //如果此时屏蔽了所有中断且发生了systick溢出，需要对millis_secend进行补偿
        {
            millis_seconds++;
        }
        return  millis_seconds * 1000 + (1000 - SysTick->VAL / 168);
    }
    uint64_t millis( void )
    {
        return micros() / 1000;
    }

    void delay_ms(uint64_t ms)
    {
        uint64_t end ;
        end = micros() + ms * 1000;
        while(micros() < end);
    }
    void delay_us(uint64_t us)
    {
        uint64_t end = micros() + us;
        while(micros() < end);
    }


    callback_fun_type systick_cb_table[1] = {0};
    __IO uint16_t systick_user_event_per_sec;//真实的值
    __IO uint16_t _systick_user_event_per_sec;//用于被millis_second取余数

    void set_systick_user_event_per_sec(u16 frq)
    {
        _systick_user_event_per_sec = 1000 / frq;
        systick_user_event_per_sec = frq;
    }

    void attach_systick_user_event(void (*callback_fun)(void))
    {
        systick_cb_table[0] = callback_fun;
    }
    void SysTick_Handler(void)//systick中断
    {
        millis_seconds++;
        if((millis_seconds % _systick_user_event_per_sec) == 0)
        {
            if(systick_cb_table[0] != 0)
            {
                systick_cb_table[0]();
            }
        }

    }

    
    static void get_system_clock(system_clock_t *clock)
    {
        __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
        uint32_t tmp = 0, pllp = 2, pllsource = 0, pllm = 2;
        #if defined(STM32F446xx)  
          uint32_t pllr = 2;
        #endif /* STM32F446xx */
          /* Get SYSCLK source -------------------------------------------------------*/
          tmp = RCC->CFGR & RCC_CFGR_SWS;

          switch (tmp)
          {
            case 0x00:  /* HSI used as system clock source */
              clock->core = HSI_VALUE;
              break;
            case 0x04:  /* HSE used as system clock source */
              clock->core = HSE_VALUE;
              break;
            case 0x08:  /* PLL P used as system clock source */
               /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
                 SYSCLK = PLL_VCO / PLL_P
                 */    
              pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
              pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
              
        #if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
              if (pllsource != 0)
              {
                /* HSE used as PLL clock source */
                clock->pll_vco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }
              else
              {
                /* HSI used as PLL clock source */
                clock->pll_vco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }
        #elif defined(STM32F410xx) || defined(STM32F411xE)
        #if defined(USE_HSE_BYPASS)
              if (pllsource != 0)
              {
                /* HSE used as PLL clock source */
                clock->pll_vco = (HSE_BYPASS_INPUT_FREQUENCY / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }  
        #else  
              if (pllsource == 0)
              {
                /* HSI used as PLL clock source */
                clock->pll_vco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }  
        #endif /* USE_HSE_BYPASS */  
        #endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F446xx || STM32F469_479xx */  
              pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
              clock->core = clock->pll_vco/pllp;      
              break;
        #if defined(STM32F446xx)      
              case 0x0C:  /* PLL R used as system clock source */
               /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
                 SYSCLK = PLL_VCO / PLL_R
                 */    
              pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
              pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
              if (pllsource != 0)
              {
                /* HSE used as PLL clock source */
                clock->pll_vco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }
              else
              {
                /* HSI used as PLL clock source */
                clock->pll_vco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      
              }
         
              pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >>28) + 1 ) *2;
              clock->core = pllvco/pllr;      
              break;
        #endif /* STM32F446xx */
            default:
              clock->core = HSI_VALUE;
              break;
          }
          /* Compute HCLK frequency --------------------------------------------------*/
          /* Get HCLK prescaler */
          tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
          /* HCLK frequency */
          clock->core >>= tmp;


            clock->hclk = clock->core;
            clock->pclk2 = clock->core/2;
            clock->pclk1 = clock->core/4;
          
    }








}