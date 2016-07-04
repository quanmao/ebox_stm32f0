/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  LQM
  * @version V1.0.0
  * @brief   usart1,2,3
  * @log  
  * @date    2016.6.28   移植到基于HAL固件库STM32F0平台
  ******************************************************************************
  */
#include "ebox_uart.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"



#define uart_tx_length   100
uint8_t uart_tx_buf[uart_tx_length];
volatile uint8_t busy[5];
callback_fun_type usart_callback_table[5][2];//支持串口的rx中断

/**
 *@name     USART::USART(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin)
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Usart::Usart(USART_TypeDef *USARTx, Gpio *tx, Gpio *rx)
{
    this->USARTx = USARTx;
    this->tx = tx;
    this->rx = rx;
}

/**
 *@name     USART::begin(uint32_t baud_rate)
 *@brief    启动串口
 *@param    USARTx:  USART1,2,3和UART4,5
 *          baud_rate:  波特率
 *@retval   None
*/
void Usart::begin(uint32_t baud_rate)
{
    begin(baud_rate,1);
}

/**
 *@name     USART::begin(uint32_t baud_rate,uint8_t use_dma)
 *@brief    启动串口
 *@param    USARTx:  USART1,2,3和UART4,5
 *          baud_rate:  波特率
 *		    use_dma： 是否使用DMA
 *@retval   None
*/
void Usart::begin(uint32_t baud_rate,uint8_t use_dma)
{
   this->use_dma = use_dma;
//    // 配置各串口参数
   switch((uint32_t)USARTx)
   {
        case (uint32_t)USART1_BASE:
		   // GPIO Parameter
           gpio_af_usart       = GPIO_AF1_USART1; 
		   // USART Parameter：时钟，中断
           rcc_usart_clock_cmd = LL_APB1_GRP2_EnableClock;
           usart_rcc           = LL_APB1_GRP2_PERIPH_USART1;
		       usart_rcc_source	   = LL_RCC_USART1_CLKSOURCE;
           usart_irq           = USART1_IRQn;
		   // DMA Parameter
           rcc_dma_clock_cmd   = LL_AHB1_GRP1_EnableClock;        
           dma_rcc             = LL_AHB1_GRP1_PERIPH_DMA1;
           dma                 = DMA1;
           dma_channel         = LL_DMA_CHANNEL_2;
       break;
       
       case (uint32_t)USART2_BASE:
		   // GPIO Parameter
           gpio_af_usart       = GPIO_AF1_USART2;
		   // USART Parameter：时钟，中断
           rcc_usart_clock_cmd = LL_APB1_GRP1_EnableClock;
           usart_rcc           = LL_APB1_GRP1_PERIPH_USART2;
		   usart_rcc_source	   = LL_RCC_USART2_CLKSOURCE;
           usart_irq           = USART2_IRQn;
		   // DMA Parameter
           rcc_dma_clock_cmd   = LL_AHB1_GRP1_EnableClock;        
           dma_rcc             = LL_AHB1_GRP1_PERIPH_DMA1;
           dma                 = DMA1;
           dma_channel         = LL_DMA_CHANNEL_4;
       break;
   
   }               
    usart_config(baud_rate);
    if(this->use_dma == 1)
    {
        dma_config();
    }
}

/**
 *@name     USART::usart_config(uint32_t baud_rate)
 *@brief    配置串口
 *@param    USARTx:  USART1,2,3和UART4,5
 *          baud_rate:  波特率
 *@retval   None
*/
void Usart::usart_config(uint32_t baud_rate)
{
	/* (1) Enable GPIO clock and configures the USART pins **********************/
	tx->mode(AF_PP_PU,gpio_af_usart);
	rx->mode(AF_PP_PU,gpio_af_usart);
	/* (2) Enable USART2 peripheral clock and clock source ****************/
	rcc_usart_clock_cmd(usart_rcc);
	//LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	/* Set clock source */
//	LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
	/* (3) Configure USART2 functional parameters ********************************/
	/* TX/RX direction */
	LL_USART_SetTransferDirection(USARTx, LL_USART_DIRECTION_TX_RX);
	/* 8 data bit, 1 start bit, 1 stop bit, no parity */
	LL_USART_ConfigCharacter(USARTx, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	/* Set Baudrate to 115200 using APB frequency set to 48000000 Hz */
	/* Frequency available for USART peripheral can also be calculated through LL RCC macro */
	/* Ex :
	    Periphclk = LL_RCC_GetUSARTClockFreq(Instance);depending on USART/UART instance
	    In this example, Peripheral Clock is expected to be equal to 48000000 Hz => equal to SystemCoreClock
	*/
	LL_USART_SetBaudRate(USARTx, LL_RCC_GetUSARTClockFreq(usart_rcc_source), LL_USART_OVERSAMPLING_16, baud_rate);
	/* (4) Enable USARTx **********************************************************/
	LL_USART_Enable(USARTx);

	/* Polling USART initialisation */
	while ((!(LL_USART_IsActiveFlag_TEACK(USARTx))) || (!(LL_USART_IsActiveFlag_REACK(USARTx))))
	{
	}
	interrupt(ENABLE);
}

void Usart::dma_config()
{
  /* (1) Enable the clock of DMA1 */
  rcc_dma_clock_cmd(dma_rcc);
  /* (2) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(dma, dma_channel, 
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(dma, dma_channel,
                         (uint32_t)this->send_buf,/*(uint32_t)aTxBuffer,*/
                         LL_USART_DMA_GetRegAddr(USARTx, LL_USART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(dma, dma_channel));
  LL_DMA_SetDataLength(dma, dma_channel,uart_tx_length/*ubNbDataToTransmit*/);
  
  /* Enable DMA TX Request */
  LL_USART_EnableDMAReq_TX(USARTx);
}

void Usart::interrupt(FunctionalState enable)
{
	// 附加中断会掉函数时，打开中断
	NVIC_SetPriority(usart_irq, 0);
	NVIC_EnableIRQ(usart_irq);
	LL_USART_EnableIT_TC(USARTx);
}
/**
 *@name     void USART::attach_rx_interrupt(void (*callback_fun)(void))
 *@brief    绑定串口接收中断所调用的用户程序
 *@param    callback_fun:  用户函数
 *@retval   None
*/
void Usart::attach_rx_interrupt(void (*callback_fun)(void))
{
	/* (1) 注册中断回调函数 */
	switch ((uint32_t)USARTx)
	{
	case (uint32_t)USART1_BASE:
		usart_callback_table[0][0] = callback_fun;
		break;
	case (uint32_t)USART2_BASE:
		usart_callback_table[1][0] = callback_fun;
		break;
	case (uint32_t)USART3_BASE:
		usart_callback_table[2][0] = callback_fun;
		break;
}
	/* (2) ENABLE USART RXNE interrupts */
	LL_USART_EnableIT_RXNE(USARTx);
}

/**
 *@name     void USART::attach_tx_interrupt(void (*callback_fun)(void))
 *@brief    绑定串口发送完成中断所调用的用户程序
 *@param    callback_fun:  用户函数
 *@retval   None
*/
void Usart::attach_tx_interrupt(void (*callback_fun)(void))
{
	/* (1) 注册中断回调函数 */
	switch ((uint32_t)USARTx)
	{
	case (uint32_t)USART1_BASE:
		usart_callback_table[0][1] = callback_fun;
		break;
	case (uint32_t)USART2_BASE:
		usart_callback_table[1][1] = callback_fun;
		break;
	case (uint32_t)USART3_BASE:
		usart_callback_table[2][1] = callback_fun;
		break;
	}
}
/**
 *@name     uint16_t USART::receive()
 *@brief    串口接收数据，此函数只能在用户接收中断中调用
 *@param    NONE
 *@retval   串口所接收到的数据
*/
uint16_t Usart::receive()
{
    return (uint16_t)(USARTx->RDR & (uint16_t)0x01FF);
}
/**
 *@name     uint16_t USART::dma_send_string(const char *str,uint16_t length)
 *@brief    串口DMA方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   发送数据的长度
*/
uint16_t Usart::dma_send_string(const char *str, uint16_t length)
{
//此函数有待优化，去掉第一个参数
// DMA非连续模式，必须要先关闭通道，再打开才能写入数据
//	LL_DMA_DisableChannel(dma, dma_channel);
	LL_DMA_SetDataLength(dma,dma_channel,length);
	LL_DMA_EnableChannel(dma,dma_channel);
	return length;
}

/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    ch：    要发送的字符
 *@retval   已发送的数据
*/
int Usart::put_char(uint16_t ch)
{
	while (LL_USART_IsActiveFlag_TXE(USARTx)== RESET);
	LL_USART_TransmitData8(USARTx,ch);
    return ch;
}
/**
 *@name     void USART::put_string(const char *str,uint16_t length)
 *@brief    串口方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   NONE
*/
void Usart::put_string(const char *str, uint16_t length)
{
	if (this->use_dma == 1)
	{
		dma_send_string(str, length);
	}
	else
	{
		while (length--)
		{
			while (LL_USART_IsActiveFlag_TXE(USARTx)== RESET);
			LL_USART_TransmitData8(USARTx,*str++);
		}
	}
}
/**
 *@name     void USART::put_string(const char *str)
 *@brief    串口方式发送一个字节
 *@param    str：   要发送的字符串,直到遇到'\0'，如果没有'\0'，则按最大缓冲区计算
 *@retval   已发送的数据
*/
void Usart::put_string(const char *str)
{
    uint16_t i = 0;
    uint16_t length = 0;

    wait_busy();
    set_busy();
    while(str[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
        {
            length = UART_MAX_SEND_BUF;
            break;
        }
    };
    put_string(str, length);

}
/////////////////////////////////////////////////
/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Usart::printf_length(const char *str, uint16_t length)
{
    wait_busy();
    set_busy();
    if(length >= UART_MAX_SEND_BUF)
        length = UART_MAX_SEND_BUF;
    for(int i = 0; i < length; i++)
        send_buf[i] = *str++;
    put_string(send_buf, length);
}

/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Usart::printf(const char *fmt, ...)
{
	uint16_t i = 0;
	uint16_t length = 0;

	wait_busy();
	set_busy();
	va_list va_params;
	va_start(va_params, fmt);
	vsprintf(send_buf, fmt, va_params); //存在内存溢出的风险
	va_end(va_params);

	while (send_buf[i++] != '\0')
	{
		length++;
		if (length >= UART_MAX_SEND_BUF)
		{
			length = UART_MAX_SEND_BUF;
			break;
		}
	};
	put_string(send_buf, length);
}


/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Usart::wait_busy()
{
	switch ((uint32_t)USARTx)
	{
	case (uint32_t)USART1_BASE:
		while (busy[0] == 1);
		break;
	case (uint32_t)USART2_BASE:
		while (busy[1] == 1);
		break;
	case (uint32_t)USART3_BASE:
		while (busy[2] == 1);
		break;
//    case (uint32_t)UART4_BASE:
//        while(busy[3] == 1);
//        break;
//    case (uint32_t)UART5_BASE:
//        while(busy[4] == 1);
//        break;
	}
}

/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Usart::set_busy()
{
	switch ((uint32_t)USARTx)
	{
	case (uint32_t)USART1_BASE:
		busy[0] = 1;
		break;
	case (uint32_t)USART2_BASE:
		busy[1] = 1;
		break;
	case (uint32_t)USART3_BASE:
		busy[2] = 1;
		break;
//    case (uint32_t)UART4_BASE:
//        busy[3] = 1;
//        break;
//    case (uint32_t)UART5_BASE:
//        busy[4] = 1;
//        break;
	}
}

extern "C" {
	void USART1_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
		{
			// 调用接收中断回调函数
			if (usart_callback_table[0][0] != 0)
				usart_callback_table[0][0]();
			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
			if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
			{
				LL_USART_RequestRxDataFlush(USART1);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART1) == SET)
		{
			// 清除忙标志，调用发送结束回调函数
			busy[0] = 0;
			if (usart_callback_table[0][1] != 0)
				usart_callback_table[0][1]();
			// 清除发送结束中断标志
			LL_USART_ClearFlag_TC(USART1);
			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
		}
	}

	void USART2_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
		{
			// 调用接收中断回调函数
			if (usart_callback_table[1][0] != 0)
				usart_callback_table[1][0]();
			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
			if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
			{
				LL_USART_RequestRxDataFlush(USART2);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART2) == SET)
		{
			// 清除忙标志，调用发送结束回调函数
			busy[1] = 0;
			if (usart_callback_table[1][1] != 0)
				usart_callback_table[1][1]();
			// 清除发送结束中断标志
			LL_USART_ClearFlag_TC(USART2);
			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
		}
	}
}
