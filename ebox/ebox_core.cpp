#include "ebox_common.h"


extern "C" {
#include "ebox_core.h"

	//system_clock_t system_clock;
	__IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
	//static void get_system_clock(system_clock_t *clock);

	void ebox_init(void)
	{
		//get_system_clock(&system_clock); 
		// SystemCoreClockUpdate();
		// SysTick_Config(SystemCoreClock/1000);
		//SysTick_Config(system_clock.core/1000);//  每隔 (nhz/168,000,000)s产生一次中断
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 待确认
		HAL_Init();  // 配置SysTick
//		SystemClock_Config();
		
	}

	uint64_t micros(void)
	{
		if ((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK())) //如果此时屏蔽了所有中断且发生了systick溢出，需要对millis_secend进行补偿
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
		while (micros() < end);
	}
	void delay_us(uint64_t us)
	{
		uint64_t end = micros() + us;
		while (micros() < end);
	}


	callback_fun_type systick_cb_table[1] = {0};
	__IO uint16_t systick_user_event_per_sec;//真实的值
	__IO uint16_t _systick_user_event_per_sec;//用于被millis_second取余数

	void set_systick_user_event_per_sec(uint16_t frq)
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
		if ((millis_seconds % _systick_user_event_per_sec) == 0)
		{
			if (systick_cb_table[0] != 0)
			{
				systick_cb_table[0]();
			}
		}

	}


//	static void get_system_clock(system_clock_t *clock)
//	{
//		__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//		uint32_t tmp = 0, pllmull = 0, pllsource = 0, predivfactor = 0;

//		/* Get SYSCLK source -------------------------------------------------------*/
//		tmp = RCC->CFGR & RCC_CFGR_SWS;

//		switch (tmp)
//		{
//		case RCC_CFGR_SWS_HSI:  /* HSI used as system clock */
//			clock->core = HSI_VALUE;
//			break;
//		case RCC_CFGR_SWS_HSE:  /* HSE used as system clock */
//			clock->core = HSE_VALUE;
//			break;
//		case RCC_CFGR_SWS_PLL:  /* PLL used as system clock */
//			/* Get PLL clock source and multiplication factor ----------------------*/
//			pllmull = RCC->CFGR & RCC_CFGR_PLLMUL;
//			pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
//			pllmull = ( pllmull >> 18) + 2;
//			predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;

//			if (pllsource == RCC_CFGR_PLLSRC_HSE_PREDIV)
//			{
//				/* HSE used as PLL clock source : SystemCoreClock = HSE/PREDIV * PLLMUL */
//				clock->core = (HSE_VALUE/predivfactor) * pllmull;
//			}
//#if defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) || defined(STM32F098xx)
//			else if (pllsource == RCC_CFGR_PLLSRC_HSI48_PREDIV)
//			{
//				/* HSI48 used as PLL clock source : SystemCoreClock = HSI48/PREDIV * PLLMUL */
//				clock->core = (HSI48_VALUE/predivfactor) * pllmull;
//			}
//#endif /* STM32F042x6 || STM32F048xx || STM32F072xB || STM32F078xx || STM32F091xC || STM32F098xx */
//			else
//			{
//#if defined(STM32F042x6) || defined(STM32F048xx)  || defined(STM32F070x6) \
// || defined(STM32F078xx) || defined(STM32F071xB)  || defined(STM32F072xB) \
// || defined(STM32F070xB) || defined(STM32F091xC) || defined(STM32F098xx)  || defined(STM32F030xC)
//				/* HSI used as PLL clock source : SystemCoreClock = HSI/PREDIV * PLLMUL */
//				clock->core = (HSI_VALUE/predivfactor) * pllmull;
//#else
//				/* HSI used as PLL clock source : SystemCoreClock = HSI/2 * PLLMUL */
//				clock->core = (HSI_VALUE >> 1) * pllmull;
//#endif /* STM32F042x6 || STM32F048xx || STM32F070x6 ||
//				STM32F071xB || STM32F072xB || STM32F078xx || STM32F070xB ||
//				STM32F091xC || STM32F098xx || STM32F030xC */
//			}
//			break;
//		default: /* HSI used as system clock */
//			clock->core = HSI_VALUE;
//			break;
//		}
//		/* Compute HCLK clock frequency ----------------*/
//		/* Get HCLK prescaler */
//		tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
//		/* HCLK clock frequency */
//		clock->core >>= tmp;

//		// 待确认
//		clock->hclk = clock->core;
//		clock->pclk2 = clock->core/2;
//		clock->pclk1 = clock->core/4;

//	}

}
