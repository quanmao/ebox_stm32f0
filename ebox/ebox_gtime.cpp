/*
file   : gtimer.cpp
author : shentq
version: V1.1
date   : 2015/7/5
date   : 2016/6/30 LQM移植到STM32F0平台
Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "ebox_gtimer.h"

//#define TIM_NUM 7

extern callback_fun_type timx_cb_table[7][5];

//////////////////////////////////////


TIM::TIM(TIM_TypeDef *TIMx)
{
    _TIMx = TIMx;
}

/**
 *@name     void TIM::begin(uint32_t frq)
 *@brief    根据需要的频率计算预分频以及重载值参数
 *@param    frq:  频率
 *@retval   None
*/
void TIM::begin(uint32_t frq)
{
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;
    if(frq >= 1000000)frq = 1000000;
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        //_period = 84000000 / _prescaler / frq;
		    _period = __LL_TIM_CALC_ARR(SystemCoreClock, _prescaler, frq);
        if((0xffff >= _period))break;
    }	

    base_init(_period, _prescaler);
}

void TIM::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}

/**
 *@name     void TIM::attach_interrupt(void(*callback)(void))
 *@brief    绑定TIM中断所调用的用户程序
 *@param    callback_fun:  用户函数
 *@retval   None
*/
void TIM::attach_interrupt(void(*callback)(void))
{
    switch((uint32_t)_TIMx)
    {
    case (uint32_t)TIM2_BASE:
        timx_cb_table[1][0] = callback;
        break;
    case (uint32_t)TIM3_BASE:
        timx_cb_table[2][0] = callback;
        break;
//    case (uint32_t)TIM4_BASE:
//        timx_cb_table[3][0] = callback;
//        break;
//    case (uint32_t)TIM5_BASE:
//        timx_cb_table[4][0] = callback;
//        break;
//    case (uint32_t)TIM6_BASE:
//        timx_cb_table[5][0] = callback;
//        break;
//    case (uint32_t)TIM7_BASE:
//        timx_cb_table[6][0] = callback;
//        break;
    }
}

void TIM::interrupt(FunctionalState enable)
{
    //必须加，否则开启中断会立即产生一次中断
	LL_TIM_ClearFlag_UPDATE(_TIMx);
	if (enable == DISABLE)
	{
		LL_TIM_DisableIT_UPDATE(_TIMx);
	}else{
		LL_TIM_EnableIT_UPDATE(_TIMx);
	}
	
}

void TIM::start(void)
{
	/* Enable counter */
	LL_TIM_EnableCounter(_TIMx);
}

void TIM::stop(void)
{
	/* DISABLE counter */
	LL_TIM_DisableCounter(_TIMx);
}
void TIM::base_init(uint16_t period, uint16_t prescaler)
{
  /* (1)Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  /* (1)select clock Source */
  LL_TIM_SetClockSource(_TIMx,LL_TIM_CLOCKSOURCE_INTERNAL);
  /* (2)Set counter mode ,可跳过，Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(_TIMx, LL_TIM_COUNTERMODE_UP);
  /* (3)Set Clock Division */
  LL_TIM_SetClockDivision(_TIMx,LL_TIM_CLOCKDIVISION_DIV1);	
  /* (4)Set the pre-scaler value to have TIMx counter */
  LL_TIM_SetPrescaler(_TIMx, prescaler - 1);  
  /* (5)Set the auto-reload value to have an initial update event */
  //  InitialAutoreload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(_TIMx), 10);
  LL_TIM_SetAutoReload(_TIMx, period);
  /* (6)Enable the update interrupt */
  LL_TIM_EnableIT_UPDATE(_TIMx);
  
  /* Configure the NVIC to handle TIM2 update interrupt */
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn); 
  
  /* Enable counter */
  //LL_TIM_EnableCounter(TIM2);
  
  /* Force update generation */
  //LL_TIM_GenerateEvent_UPDATE(TIM2);
//    NVIC_InitTypeDef NVIC_InitStructure;

//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_DeInit(_TIMx);
//    switch((uint32_t)_TIMx)
//    {
//    //		case (uint32_t)TIM1:
//    //			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//    //			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
//    //			break;
//    case (uint32_t)TIM2_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
//        break;
//    case (uint32_t)TIM3_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//
//        break;
//    case (uint32_t)TIM4_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//
//        break;
//#if defined (STM32F10X_HD)
//    case (uint32_t)TIM5_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//
//        break;
//    case (uint32_t)TIM6_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//
//        break;
//    case (uint32_t)TIM7_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//
//        break;
//#endif
//    }
}

void TIM::set_reload(uint16_t auto_reload)
{    
	LL_TIM_SetAutoReload(_TIMx,auto_reload);	
}

void TIM::clear_count(void)
{
    _TIMx->CNT = 0;
}




