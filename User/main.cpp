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
void rx_even(void)
{
	uint8_t c;
	c = uart2.receive();
	uart2.put_char(c);
}

void setup()
{
    ebox_init();
	  PA5.mode(OUTPUT_PP);
	  uart2.begin(115200);
	  uart2.printf("test66666666"); 
	  uart2.attach_rx_interrupt(rx_even);
    	
}
int main(void)
{
    setup();
    //random_seed(10);
    while(1)
    {
			uart2.printf("test66666666");
			PA5.toggle();
			delay_ms(500);
//        PA15.toggle();
//        for(int i = 0; i < 10;i++)
//        {
//            wbuf[i] = i;
//        }
//        flash.write_sector(ADDR_FLASH_SECTOR_4,wbuf,10);
//        flash.read(ADDR_FLASH_SECTOR_4,rbuf,10);

//        delay_ms(100);
    }
}




