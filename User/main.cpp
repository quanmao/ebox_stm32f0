/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"



PWM pwm1(&PA6);//
void setup()
{
	ebox_init();
	
  uart1.begin(115200);
	uart1.printf("\r\n test");
	PA5.mode(OUTPUT_PP);
	pwm1.begin(480000, 500);//??? PWM ??
	uart1.printf("CLOCK: %d  \r\n",pwm1.TIM_Clock);
  uart1.printf("period: %d -- prescaler: %d \r\n",pwm1.period,pwm1.prescaler);
//	delay_ms(5000);
//	pwm1.set_oc_polarity(0);//?? PWM ?????
//	delay_ms(5000);
	//pwm1.set_frq(100);
}
uint16_t y;
int main(void)
{
	setup();
	while (1)
	{
		PA5.toggle();
		delay_ms(5000);
		delay_ms(5000);
		delay_ms(5000);
		delay_ms(5000);
		uart1.printf("\r\n test66666666");
//    for(y = 0;y<=1000;y++)
//    {
//      pwm1.set_duty(y);
//      delay_ms(50);
//    }

	}
}





