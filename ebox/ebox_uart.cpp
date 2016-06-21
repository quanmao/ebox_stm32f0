#include "ebox_uart.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"


callback_fun_type usart_callback_table[5][2];//֧�ִ��ڵ�rx�ж�
#define uart_tx_length   100
uint8_t uart_tx_buf[uart_tx_length];
uint8_t busy[5];
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
void Usart::begin(uint32_t baud_rate)
{
    begin(baud_rate,1);
}
void Usart::begin(uint32_t baud_rate,uint8_t use_dma)
{
    
    this->use_dma = use_dma;
    
    switch((uint32_t)USARTx)
    {
        case (uint32_t)USART1_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART1;
        
            /* usart parament
            */
            rcc_usart_clock_cmd = RCC_APB2PeriphClockCmd;
            usart_rcc           = RCC_APB2Periph_USART1;
            usart_irq           = USART1_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA2;
            dma_irq             = DMA2_Stream7_IRQn;
            dma                 = DMA2;
            dma_channel         = DMA_Channel_4;
            dma_stream          = DMA2_Stream7;
        break;
        
        case (uint32_t)USART2_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART2;
        
            /* usart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_USART2;
            usart_irq           = USART2_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream6_IRQn;
            dma_stream          = DMA1_Stream6;
        break;
        
        case (uint32_t)USART3_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART3;
        
            /* usart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_USART3;
            usart_irq           = USART3_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream3_IRQn;
            dma_stream          = DMA1_Stream3;
        break;   
    
        case (uint32_t)UART4_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_UART4;
        
            /* usart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_UART4;
            usart_irq           = UART4_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream4_IRQn;
            dma_stream          = DMA1_Stream4;
        break;   
    
        case (uint32_t)UART5_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_UART5;
        
            /* usart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_UART5;
            usart_irq           = UART5_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream7_IRQn;
            dma_stream          = DMA1_Stream7;
        break;   
    
    }               
    usart_config(baud_rate);
    if(this->use_dma == 1)
    {
        dma_config();
    }
}
void Usart::usart_config(uint32_t baud_rate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
    
    tx->mode(AF_PP,gpio_af_usart);
    rx->mode(AF_PP,gpio_af_usart);
    
    rcc_usart_clock_cmd(usart_rcc,ENABLE);
  	USART_InitStructure.USART_BaudRate = baud_rate;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USARTx, &USART_InitStructure);
    
    USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);  
   	USART_Cmd(USARTx, ENABLE);
  	USART_ClearFlag(USARTx, USART_FLAG_TC);  
 
    interrupt(ENABLE);
}
void Usart::dma_config()
{
	NVIC_InitTypeDef NVIC_InitStructure ;
	DMA_InitTypeDef DMA_InitStructure;

    //���ڷ�DMA����  
    //����DMAʱ��
    rcc_dma_clock_cmd(dma_rcc, ENABLE);//////////////////////
    //DMA�����ж�����
    NVIC_InitStructure.NVIC_IRQChannel = dma_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //DMAͨ������
    DMA_DeInit(dma_stream);/////////////////////////////
    DMA_InitStructure.DMA_Channel = dma_channel; /////////////////////
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USARTx->DR);/////////////////
    //�ڴ��ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)this->send_buf;//////////////////////////
    //dma���䷽��
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = uart_tx_length;///////////////////////////////
    //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����DMA���ڴ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //���������ֳ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //�ڴ������ֳ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    //����DMA�Ĵ���ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //����DMA�����ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
    //ָ����FIFO��ֵˮƽ
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        
    //ָ����Burstת�������ڴ洫�� 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       
    //ָ����Burstת��������Χת�� */  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 

    //����DMA1��ͨ��         
    DMA_Init(dma_stream, &DMA_InitStructure);  //////////////////////////////////////////
    //ʹ���ж�
    DMA_ITConfig(dma_stream,DMA_IT_TC,ENABLE); /////////////////////
}

void Usart::interrupt(FunctionalState enable)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    USART_ITConfig(USARTx, USART_IT_RXNE, enable);
    USART_ITConfig(USARTx, USART_IT_TC, enable);
    USART_ClearITPendingBit(USARTx, USART_IT_TC);

    NVIC_InitStructure.NVIC_IRQChannel = usart_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
/**
 *@name     void USART::attach_rx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڽ����ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Usart::attach_rx_interrupt(void (*callback_fun)(void))
{
    switch((uint32_t)USARTx)
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
    case (uint32_t)UART4_BASE:
        usart_callback_table[3][0] = callback_fun;
        break;
    case (uint32_t)UART5_BASE:
        usart_callback_table[4][0] = callback_fun;
        break;
    }
}

/**
 *@name     void USART::attach_tx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڷ�������ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Usart::attach_tx_interrupt(void (*callback_fun)(void))
{
    switch((uint32_t)USARTx)
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
    case (uint32_t)UART4_BASE:
        usart_callback_table[3][1] = callback_fun;
        break;
    case (uint32_t)UART5_BASE:
        usart_callback_table[4][1] = callback_fun;
        break;
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
    return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
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
	DMA_SetCurrDataCounter(dma_stream,length);
 	DMA_Cmd(dma_stream,ENABLE);
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
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
    USART_SendData(USARTx, ch);
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
    if(this->use_dma == 1)
    {
        dma_send_string(str, length);
    }
    else
    {
        while(length--)
        {
            while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
            USART_SendData(USARTx, *str++);
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

    while(send_buf[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
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
    switch((uint32_t)USARTx)
    {
    case (uint32_t)USART1_BASE:
        while(busy[0] == 1);
        break;
    case (uint32_t)USART2_BASE:
        while(busy[1] == 1);
        break;
    case (uint32_t)USART3_BASE:
        while(busy[2] == 1);
        break;
    case (uint32_t)UART4_BASE:
        while(busy[3] == 1);
        break;
    case (uint32_t)UART5_BASE:
        while(busy[4] == 1);
        break;
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
    switch((uint32_t)USARTx)
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
    case (uint32_t)UART4_BASE:
        busy[3] = 1;
        break;
    case (uint32_t)UART5_BASE:
        busy[4] = 1;
        break;
    }
}

extern "C" {

    void USART1_IRQHandler(void)
    {
        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        {
            if(usart_callback_table[0][0] != 0)
                usart_callback_table[0][0]();
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
        {
            busy[0] = 0;
            if(usart_callback_table[0][1] != 0)
                usart_callback_table[0][1]();
            USART_ClearITPendingBit(USART1, USART_IT_TC);
        }
    }
    void USART2_IRQHandler(void)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
        {
            if(usart_callback_table[1][0] != 0)
                usart_callback_table[1][0]();
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
        {
            busy[1] = 0;
            if(usart_callback_table[1][1] != 0)
                usart_callback_table[1][1]();
            USART_ClearITPendingBit(USART2, USART_IT_TC);
        }
    }
    void USART3_IRQHandler(void)
    {
        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
        {
            if(usart_callback_table[2][0] != 0)
                usart_callback_table[2][0]();
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART3, USART_IT_TC) == SET)
        {
            busy[2] = 0;
            if(usart_callback_table[2][1] != 0)
                usart_callback_table[2][1]();
            USART_ClearITPendingBit(USART3, USART_IT_TC);
        }
    }
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            if(usart_callback_table[3][0] != 0)
                usart_callback_table[3][0]();
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TC) == SET)
        {
            busy[3] = 0;
            if(usart_callback_table[3][1] != 0)
                usart_callback_table[3][1]();
            USART_ClearITPendingBit(UART4, USART_IT_TC);
        }
    }
    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
            if(usart_callback_table[4][0] != 0)
                usart_callback_table[4][0]();
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TC) == SET)
        {
            busy[4] = 0;
            if(usart_callback_table[4][1] != 0)
                usart_callback_table[4][1]();
            USART_ClearITPendingBit(UART5, USART_IT_TC);
        }
    }




}