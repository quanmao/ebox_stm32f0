/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"

// void rx_event()
// {
// char c;
// c = uart2.receive();
// uart2.put_char(c);
// }

// void tx_event()
// {
// PA5.toggle();
// }


// void setup()
// {
	// ebox_init();
	// PA5.mode(OUTPUT_PP);
	// uart2.begin(115200);
	// uart2.printf("test66666666");
	// uart2.attach_rx_interrupt(rx_event);
	// uart2.attach_tx_interrupt(tx_event);

// }
// int main(void)
// {
	// setup();
	//random_seed(10);
	// while (1)
	// {
		//uart2.printf("test66666666");
		// delay_ms(500);
	// }
// }

TIM timer2(TIM2);//???????????,?? TIM2?
uint32_t count;
void t2_event()//?????????
{
	count++;
	if (count == 1000)
	{
		count = 0;
		PA5.toggle();
		uart1.printf("\r\ntimer2 is triggered 1000 times !");
	}
}
void setup()
{
	ebox_init();
	uart1.begin(115200);
	timer2.begin(2000);//?????????:1KHz
	timer2.attach_interrupt(t2_event);//??????
	timer2.interrupt(ENABLE);//????
	timer2.start();//?????
	PA5.mode(OUTPUT_PP);
	uart1.printf("\r\ntimer2 test !");
}

int main(void)
{
	setup();
	while (1)
	{
	}
}



