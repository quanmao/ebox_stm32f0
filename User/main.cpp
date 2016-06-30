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

FLASHCLASS flash;//创建一个内部 flash 读写对象
uint16_t wbuf[10] = {0,1,2,3,4,5,6,7,8,2};
uint16_t rbuf[10];
void setup()
{
	ebox_init();
	uart1.begin(115200);
	uart1.printf("test66666666");
}
int main(void)
{
	setup();
//random_seed(10);
	while (1)
	{
//for(int i = 0; i <10; i++)//使用 random 函数给写缓冲区赋值
//{
//wbuf[i] = random(100);
//}
		flash.erase_sector(ADDR_FLASH_PAGE_15);
		flash.write_sector(ADDR_FLASH_PAGE_15,(uint16_t*)wbuf,9);//将写缓冲区的内容写入内部 flash
		uart1.printf("write data\r\n");
		for (int i = 0; i <10; i++)
		{
			uart1.printf("%02d ",wbuf[i]);
		}
		uart1.printf("\r\n");
		uart1.printf("read data\r\n");
		flash.read(ADDR_FLASH_PAGE_15,(uint16_t*)rbuf,10);//将内部 flash 中的内容读取到读缓冲区
		for (int i = 0; i <10; i++)
		{
			uart1.printf("%02d ",rbuf[i]);
		}
		uart1.printf("\r\n============================\r\n");
		delay_ms(5000);
	}
}



