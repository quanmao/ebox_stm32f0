/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"

FLASHCLASS flash;//??????flash????

uint8_t wbuf[10];
uint8_t rbuf[10];

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("eraes \r\n");

    PA15.mode(OUTPUT_PP);
    PA15.toggle();
        delay_ms(100);
    PA15.toggle();
        delay_ms(100);
    PA15.toggle();
        delay_ms(100);
    
}
int main(void)
{
    setup();

    //random_seed(10);
    while(1)
    {
        PA15.toggle();
        for(int i = 0; i < 10;i++)
        {
            wbuf[i] = i;
        }
        flash.write_sector(ADDR_FLASH_SECTOR_4,wbuf,10);
        flash.read(ADDR_FLASH_SECTOR_4,rbuf,10);

        delay_ms(100);
    }
}




