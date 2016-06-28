/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  LQM
  * @version V1.0.0
  * @brief   usart1,2,3
  * @log  
  * @date    2016.6.28   ��ֲ������HAL�̼���STM32F0ƽ̨
  ******************************************************************************
  */
#include "ebox_uart.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"


callback_fun_type usart_callback_table[5][2];//֧�ִ��ڵ�rx�ж�
#define uart_tx_length   100
uint8_t uart_tx_buf[uart_tx_length];
volatile uint8_t busy[5];

/**
 *@name     USART::USART(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin)
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
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
 *@brief    ��������
 *@param    USARTx:  USART1,2,3��UART4,5
 *          baud_rate:  ������
 *@retval   None
*/
void Usart::begin(uint32_t baud_rate)
{
    begin(baud_rate,1);
}

/**
 *@name     USART::begin(uint32_t baud_rate,uint8_t use_dma)
 *@brief    ��������
 *@param    USARTx:  USART1,2,3��UART4,5
 *          baud_rate:  ������
 *		    use_dma�� �Ƿ�ʹ��DMA
 *@retval   None
*/
void Usart::begin(uint32_t baud_rate,uint8_t use_dma)
{
   this->use_dma = use_dma;
//    // ���ø����ڲ���
   switch((uint32_t)USARTx)
   {
        case (uint32_t)USART1_BASE:
		   // GPIO Parameter
           gpio_af_usart       = GPIO_AF1_USART1; 
		   // USART Parameter��ʱ�ӣ��ж�
           rcc_usart_clock_cmd = LL_APB1_GRP2_EnableClock;
           usart_rcc           = LL_APB1_GRP2_PERIPH_USART1;
           usart_irq           = USART1_IRQn;
		   // DMA Parameter
           rcc_dma_clock_cmd   = LL_AHB1_GRP1_EnableClock;        
           dma_rcc             = LL_AHB1_GRP1_PERIPH_DMA1;
           dma                 = DMA1;
           dma_channel         = LL_DMA_CHANNEL_3;
           dma_irq             = DMA1_Channel4_5_6_7_IRQn;
       break;
       
       case (uint32_t)USART2_BASE:
		   // GPIO Parameter
           gpio_af_usart       = GPIO_AF1_USART2;
		   // USART Parameter��ʱ�ӣ��ж�
           rcc_usart_clock_cmd = LL_APB1_GRP1_EnableClock;
           usart_rcc           = LL_APB1_GRP1_PERIPH_USART2;
           usart_irq           = USART2_IRQn;
		   // DMA Parameter
           rcc_dma_clock_cmd   = LL_AHB1_GRP1_EnableClock;        
           dma_rcc             = LL_AHB1_GRP1_PERIPH_DMA1;
           dma                 = DMA1;
           dma_channel         = LL_DMA_CHANNEL_4;
           dma_irq             = DMA1_Channel4_5_6_7_IRQn;
       break;
//       
//       case (uint32_t)USART3_BASE:
//           /* gpio parament
//           */
//           gpio_af_usart       = GPIO_AF_USART3;
//       
//           /* usart parament
//           */
//           rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
//           usart_rcc           = RCC_APB1Periph_USART3;
//           usart_irq           = USART3_IRQn;
//           /* dma parament
//           */
//           rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
//           dma_rcc             = RCC_AHB1Periph_DMA1;
//           dma                 = DMA1;
//           dma_channel         = DMA_Channel_4;
//           dma_irq             = DMA1_Stream3_IRQn;
//           dma_stream          = DMA1_Stream3;
//       break;   
//   
//       case (uint32_t)UART4_BASE:
//           /* gpio parament
//           */
//           gpio_af_usart       = GPIO_AF_UART4;
//       
//           /* usart parament
//           */
//           rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
//           usart_rcc           = RCC_APB1Periph_UART4;
//           usart_irq           = UART4_IRQn;
//           /* dma parament
//           */
//           rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
//           dma_rcc             = RCC_AHB1Periph_DMA1;
//           dma                 = DMA1;
//           dma_channel         = DMA_Channel_4;
//           dma_irq             = DMA1_Stream4_IRQn;
//           dma_stream          = DMA1_Stream4;
//       break;   
//   
//       case (uint32_t)UART5_BASE:
//           /* gpio parament
//           */
//           gpio_af_usart       = GPIO_AF_UART5;
//       
//           /* usart parament
//           */
//           rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
//           usart_rcc           = RCC_APB1Periph_UART5;
//           usart_irq           = UART5_IRQn;
//           /* dma parament
//           */
//           rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
//           dma_rcc             = RCC_AHB1Periph_DMA1;
//           dma                 = DMA1;
//           dma_channel         = DMA_Channel_4;
//           dma_irq             = DMA1_Stream7_IRQn;
//           dma_stream          = DMA1_Stream7;
//       break;   
   
   }               
    usart_config(baud_rate);
    if(this->use_dma == 1)
    {
        dma_config();
    }
}

/**
 *@name     USART::usart_config(uint32_t baud_rate)
 *@brief    ���ô���
 *@param    USARTx:  USART1,2,3��UART4,5
 *          baud_rate:  ������
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
	    Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance

	    In this example, Peripheral Clock is expected to be equal to 48000000 Hz => equal to SystemCoreClock
	*/
	LL_USART_SetBaudRate(USARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, baud_rate);
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
	// �����жϻ������ʱ�����ж�
	NVIC_SetPriority(usart_irq, 0);
	NVIC_EnableIRQ(usart_irq);
	LL_USART_EnableIT_TC(USARTx);
}
/**
 *@name     void USART::attach_rx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڽ����ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Usart::attach_rx_interrupt(void (*callback_fun)(void))
{
	/* (1) ע���жϻص����� */
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
//    case (uint32_t)UART4_BASE:
//        usart_callback_table[3][0] = callback_fun;
//        break;
//    case (uint32_t)UART5_BASE:
//        usart_callback_table[4][0] = callback_fun;
//        break;
	}
	/* (2) ENABLE USART RXNE interrupts */
	LL_USART_EnableIT_RXNE(USARTx);
}

/**
 *@name     void USART::attach_tx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڷ�������ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Usart::attach_tx_interrupt(void (*callback_fun)(void))
{
	/* (1) ע���жϻص����� */
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
//    case (uint32_t)UART4_BASE:
//        usart_callback_table[3][1] = callback_fun;
//        break;
//    case (uint32_t)UART5_BASE:
//        usart_callback_table[4][1] = callback_fun;
//        break;
	}
}
/**
 *@name     uint16_t USART::receive()
 *@brief    ���ڽ������ݣ��˺���ֻ�����û������ж��е���
 *@param    NONE
 *@retval   ���������յ�������
*/
uint16_t Usart::receive()
{
    return (uint16_t)(USARTx->RDR & (uint16_t)0x01FF);
}
/**
 *@name     uint16_t USART::dma_send_string(const char *str,uint16_t length)
 *@brief    ����DMA��ʽ�����ַ���������������
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �������ݵĳ���
*/
uint16_t Usart::dma_send_string(const char *str, uint16_t length)
{
//�˺����д��Ż���ȥ����һ������
// DMA������ģʽ������Ҫ�ȹر�ͨ�����ٴ򿪲���д������
//	LL_DMA_DisableChannel(dma, dma_channel);
	LL_DMA_SetDataLength(dma,dma_channel,length);
	LL_DMA_EnableChannel(dma,dma_channel);
	return length;
}

/**
 *@name     int USART::put_char(uint16_t ch)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    ch��    Ҫ���͵��ַ�
 *@retval   �ѷ��͵�����
*/
int Usart::put_char(uint16_t ch)
{
	while (LL_USART_IsActiveFlag_TXE(USARTx)== RESET);
	LL_USART_TransmitData8(USARTx,ch);
    return ch;
}
/**
 *@name     void USART::put_string(const char *str,uint16_t length)
 *@brief    ���ڷ�ʽ�����ַ���������������
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��   Ҫ���͵��ַ���,ֱ������'\0'�����û��'\0'������󻺳�������
 *@retval   �ѷ��͵�����
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
*/
void Usart::printf(const char *fmt, ...)
{
	uint16_t i = 0;
	uint16_t length = 0;

	wait_busy();
	set_busy();
	va_list va_params;
	va_start(va_params, fmt);
	vsprintf(send_buf, fmt, va_params); //�����ڴ�����ķ���
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
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
			if (usart_callback_table[0][0] != 0)
				usart_callback_table[0][0]();
		}
//        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//        {
//            if(usart_callback_table[0][0] != 0)
//                usart_callback_table[0][0]();
//            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//        }
//        if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
//        {
//            busy[0] = 0;
//            if(usart_callback_table[0][1] != 0)
//                usart_callback_table[0][1]();
//            USART_ClearITPendingBit(USART1, USART_IT_TC);
//        }
	}
	void USART2_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
		{
			// ���ý����жϻص�����
			if (usart_callback_table[1][0] != 0)
				usart_callback_table[1][0]();
			// ����ص�������û�ж�ȡ���ݣ��򽫵�ǰ����������׼����һ�ν���
			if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
			{
				LL_USART_RequestRxDataFlush(USART2);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART2) == SET)
		{
			// ���æ��־�����÷��ͽ����ص�����
			busy[1] = 0;
			if (usart_callback_table[1][1] != 0)
				usart_callback_table[1][1]();
			// ������ͽ����жϱ�־
			LL_USART_ClearFlag_TC(USART2);
			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
		}
	}
}
