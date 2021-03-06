/*
file   : gtimer.h
author : shentq
version: V1.1
date   : 2016/4/6
date   : 2016/6/30 LQM移植到STM32F0平台
Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_GTIMER_H
#define __EBOX_GTIMER_H

#include "ebox_common.h"

/*
	1.支持tim2,3
	2.提供一个1-1Mhz的定时中断
*/

class TIM
{
public:
    TIM(TIM_TypeDef *TIMx);
    void begin(uint32_t frq);
    void attach_interrupt(void(*callback)(void));
    void interrupt(FunctionalState enable);
    void start(void);
    void stop(void);
    void reset_frq(uint32_t frq);
private:
    void base_init(uint16_t period, uint16_t prescaler);
    void set_reload(uint16_t auto_reload);
    void clear_count(void);
    TIM_TypeDef *_TIMx;

};
#endif
