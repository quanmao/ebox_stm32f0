/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  LQM
  * @version V1.0.0
  * @brief   usart1,2,3
  * @log  
  * @date    2016.6.28   移植到基于HAL固件库STM32F0平台
  ******************************************************************************
  */
#ifndef __EBOX_UART_H
#define __EBOX_UART_H
#include "ebox_common.h"

//用户配置//////////////
#define UART_MAX_SEND_BUF 128

class Usart
{
public:
    Usart(USART_TypeDef *USARTx, Gpio *tx, Gpio *rx);

    void    begin(uint32_t baud_rate);
    void    begin(uint32_t baud_rate,uint8_t use_dma);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit);
    void    attach_rx_interrupt(void (*callback_fun)(void));
    void    attach_tx_interrupt(void (*callback_fun)(void));

    int 	put_char(uint16_t ch);
    void 	put_string(const char *str);
    void    printf_length(const char *str, uint16_t length);
    void    printf(const char *fmt, ...); //需要注意缓冲区溢出
    void    wait_busy();

    uint16_t            receive();   

private:
	// 端口参数
    Gpio                *tx;
    Gpio                *rx;
    uint8_t             gpio_af_usart;
    // USART参数
    USART_TypeDef       *USARTx;
    void                (*rcc_usart_clock_cmd)(uint32_t Periphs);
    uint32_t            usart_rcc;
    IRQn_Type           usart_irq;
    void                usart_config(uint32_t baud_rate);
    // DMA参数
    uint8_t             use_dma;
    void                (*rcc_dma_clock_cmd)(uint32_t Periphs);
    uint32_t            dma_rcc;
    IRQn_Type           dma_irq;
    DMA_TypeDef         *dma;
    uint32_t            dma_channel;
    void                dma_config();
    uint16_t            dma_send_string(const char *str, uint16_t length);
	// 其他
    char                send_buf[UART_MAX_SEND_BUF];
    void                put_string(const char *str, uint16_t length);
    void                set_busy();
    void                interrupt(FunctionalState enable);
};


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
     
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
