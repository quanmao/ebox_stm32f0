/*
file   : common.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_COMMON_H
#define __EBOX_COMMON_H

#include "ebox_core.h"                  

#define EBOX_VERSION "16.04.19"

#define EOK         0
#define EWAIT       1
#define EBUSY       2
#define ETIMEOUT    3
#define EPARA       4
#define E2LONG      5
#define EOTHER      128

////////宏定义/////////////////////////////////


#define true 0x1
#define false 0x0

#define HIGH 0x1
#define LOW  0x0


#define LSB_FIRST 0
#define MSB_FIRST 1

#define PI 3.1415926535898


typedef enum
{
    OUTPUT_PP    = 0x0,    
    OUTPUT_PP_PU    ,    
    OUTPUT_PP_PD    ,
    
    OUTPUT_OD       ,
    OUTPUT_OD_PU    ,
    OUTPUT_OD_PD    ,
    
    AIN             ,
    
    INPUT           ,
    INPUT_PD        ,
    INPUT_PU        ,
    
    AF_OD           ,
    AF_OD_PU        ,   
    AF_OD_PD        ,
    
    AF_PP           ,      
    AF_PP_PU        ,
    AF_PP_PD        ,
} PIN_MODE;
typedef enum
{
    PA0_ID = 0,     PA1_ID,     PA2_ID,     PA3_ID,     PA4_ID,     PA5_ID,     PA6_ID,     PA7_ID,
    PA8_ID,         PA9_ID,     PA10_ID,    PA11_ID,    PA12_ID,    PA13_ID,    PA14_ID,    PA15_ID,
    
    PB0_ID = 0,     PB1_ID,     PB2_ID,     PB3_ID,     PB4_ID,     PB5_ID,     PB6_ID,     PB7_ID,
    PB8_ID,         PB9_ID,     PB10_ID,    PB11_ID,    PB12_ID,    PB13_ID,    PB14_ID,    PB15_ID,
            
    PC0_ID = 0,     PC1_ID,     PC2_ID,     PC3_ID,     PC4_ID,     PC5_ID,     PC6_ID,     PC7_ID,
    PC8_ID,         PC9_ID,     PC10_ID,    PC11_ID,    PC12_ID,    PC13_ID,    PC14_ID,    PC15_ID,
            
    PD0_ID = 0,     PD1_ID,     PD2_ID,     PD3_ID,     PD4_ID,     PD5_ID,     PD6_ID,     PD7_ID,
    PD8_ID,         PD9_ID,     PD10_ID,    PD11_ID,    PD12_ID,    PD13_ID,    PD14_ID,    PD15_ID,
    
    PE0_ID = 0,     PE1_ID,     PE2_ID,     PE3_ID,     PE4_ID,     PE5_ID,     PE6_ID,     PE7_ID,
    PE8_ID,         PE9_ID,     PE10_ID,    PE11_ID,    PE12_ID,    PE13_ID,    PE14_ID,    PE15_ID,
    
    PF0_ID = 0,     PF1_ID,     PF2_ID,     PF3_ID,     PF4_ID,     PF5_ID,     PF6_ID,     PF7_ID,
    PF8_ID,         PF9_ID,     PF10_ID,    PF11_ID,    PF12_ID,    PF13_ID,    PF14_ID,    PF15_ID,
    
    PG0_ID = 0,     PG1_ID,     PG2_ID,     PG3_ID,     PG4_ID,     PG5_ID,     PG6_ID,     PG7_ID,
    PG8_ID,         PG9_ID,     PG10_ID,    PG11_ID,    PG12_ID,    PG13_ID,    PG14_ID,    PG15_ID,
}PIN_ID_t;

class Gpio
{
public:
    
    Gpio(GPIO_TypeDef *port, uint16_t pin);
    void mode(PIN_MODE mode);
    void mode(PIN_MODE mode,uint8_t af_configration);
    void set();
    void reset();
    void write(uint8_t val);
    void toggle();
    void read(uint8_t *val);
    uint8_t read(void);
    //操作符重载
    Gpio &operator= (int value)
    {
        write(value);
        return *this;
    }
    operator int()
    {
        return read();
    }
    PIN_ID_t pin_id;
    GPIO_TypeDef *port;
    uint16_t pin;
private:
    void af_config(uint8_t af_configration);

    
    uint32_t port_set_reg;
    uint32_t port_reset_reg;

};

class PARALLEL_GPIO
{
public:
    Gpio *bit[8];
public:
    void    all_mode(PIN_MODE mode);
    void    write(uint8_t data);
    void    write_low_4_4bit(uint8_t data);
    uint8_t read();
    uint8_t read_low_4_bit();
};
class SYSTEM
{
public:
    uint32_t chip_id[3];
    uint16_t flash_size;
    char version[8];

    void  get_chip_info();
    float get_cpu_temperature();
    
};
extern SYSTEM sys;

void        ADC1_init(void);

uint16_t	analog_read(Gpio *pin);
uint16_t	analog_read_voltage(Gpio *pin);

void        shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t		shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);

void        random_seed(uint16_t seed);
uint16_t    random();
uint16_t    random(uint16_t max);
uint16_t    random(uint16_t min, uint16_t max);



#endif
