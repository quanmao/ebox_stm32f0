/*
file   : common.h
author : shentq
version: V1.1
date   : 2016/4/6
author : cat_li
version: V1.1
date   : 2016/6/22 ��ֲ��stm32F0ƽ̨

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_CORE_H
#define __EBOX_CORE_H
extern "C" {
//#include "stm32f4xx.h"
#include "stm32f0xx_hal.h"
    
    
typedef void (*callback_fun_type)(void);

#define interrupts() 		__enable_irq()//���������ж�
#define no_interrupts() 	__disable_irq()//��ֹ�����ж�
    
//typedef struct
//{
//    uint32_t pll_vco;
//    uint32_t core;
//    uint32_t hclk;
//    uint32_t pclk2;
//    uint32_t pclk1;
//    
//}system_clock_t;    
    
    void ebox_init(void);
    uint64_t micros(void);
    uint64_t millis( void );
    void delay_ms(uint64_t ms);
    void delay_us(uint64_t us);
    void set_systick_user_event_per_sec(uint16_t frq);
    void attach_systick_user_event(void (*callback_fun)(void));
}
#endif
