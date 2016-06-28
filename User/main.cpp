/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "stm32f0xx_hal.h"

#include "ebox.h"

void rx_event()
{
char c;
c = uart2.receive();
uart2.put_char(c);
}

void tx_event()
{
PA5.toggle();
}


void setup()
{
	ebox_init();
	PA5.mode(OUTPUT_PP);
	uart2.begin(115200);
	uart2.printf("test66666666");
//	uart2.attach_rx_interrupt(rx_event);
//	uart2.attach_tx_interrupt(tx_event);

}
int main(void)
{
	setup();
	//random_seed(10);
	while (1)
	{
		uart2.printf("test66666666");
		delay_ms(500);
	}
}




