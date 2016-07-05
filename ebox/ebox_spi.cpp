/*
file   : spi.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "ebox_spi.h"


SPI::SPI(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
	busy = 0;
	spi = SPIx;
	sck->mode(AF_PP);
	miso->mode(AF_PP);
	mosi->mode(AF_PP);

};

/**
 *@name     void SPI::begin(SPI_CONFIG_TYPE *spi_config)
 *@brief    SPI初始化
 *@param    spi_config:  spi配置
 *@retval   None
*/
void SPI::begin(SPI_CONFIG_TYPE *spi_config)
{
	if (spi == SPI1)
	{
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		/* Enable the peripheral clock of GPIOB */
		LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);
	}
	if (spi == SPI2)
	{
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		/* Enable the peripheral clock of GPIOB */
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
	}
//	if (spi == SPI3)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
//	}
	config(spi_config);
}

void SPI::config(SPI_CONFIG_TYPE *spi_config)
{
//    SPI_InitTypeDef SPI_InitStructure;

	current_dev_num = spi_config->dev_num;


//    SPI_Cmd(spi, DISABLE);

//    SPI_I2S_DeInit(spi);
//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
//    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式

//    if(spi_config->mode == SPI_MODE0)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    }
//    else if(spi_config->mode == SPI_MODE1)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//    }
//    else if(spi_config->mode == SPI_MODE2)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    }
//    else if(spi_config->mode == SPI_MODE3)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//    }
//    SPI_InitStructure.SPI_BaudRatePrescaler = spi_config->prescaler;
//    SPI_InitStructure.SPI_FirstBit = spi_config->bit_order;
//    SPI_Init(spi, &SPI_InitStructure);
//    SPI_Cmd(spi, ENABLE);

	/* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/
	/* Set priority for SPI1_IRQn */
//  NVIC_SetPriority(SPI1_IRQn, 0);
//  /* Enable SPI1_IRQn           */
//  NVIC_EnableIRQ(SPI1_IRQn);

	/* (3) Configure SPI1 functional parameters ********************************/
	/* Enable the peripheral clock of GPIOB */
//  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

	/* Configure SPI1 communication */
	LL_SPI_SetBaudRatePrescaler(SPI1, spi_config->prescaler);
	LL_SPI_SetTransferDirection(SPI1,LL_SPI_FULL_DUPLEX);

	// CPOL = Polarity = 时钟极性; CPHA = PHASE = 时钟相位;
	if (spi_config->mode == SPI_MODE0)
	{
		LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
		LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);
	}
	else if (spi_config->mode == SPI_MODE1)
	{
		LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
		LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
	}
	else if (spi_config->mode == SPI_MODE2)
	{
		LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_HIGH);
		LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);
	}
	else if (spi_config->mode == SPI_MODE3)
	{
		LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_HIGH);
		LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
	}

	/* Reset value is LL_SPI_MSB_FIRST */
	LL_SPI_SetTransferBitOrder(SPI1, spi_config->bit_order);
	LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
//  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);
	LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);

	/* Configure SPI1 transfer interrupts */
	/* Enable TXE   Interrupt */
//  LL_SPI_EnableIT_TXE(SPI1);
//  /* Enable SPI1 Error Interrupt */
//  LL_SPI_EnableIT_ERR(SPI1);

	/* Enable SPI1 */
	LL_SPI_Enable(SPI1);
}

uint8_t SPI::read_config(void)
{
	return current_dev_num;
}


int8_t SPI::write(uint8_t data)
{
	__IO uint8_t dummyByte;
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	spi->DR = data;
	while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	dummyByte = spi->DR;

	return 0;
}
int8_t SPI::write(uint8_t *data, uint16_t data_length)
{
	__IO uint8_t dummyByte;
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		spi->DR = *data++;
		while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
			;
		dummyByte = spi->DR;
	}
	return 0;
}
uint8_t SPI::read()
{
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	spi->DR = 0xff;
	while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	return(spi->DR);

}
int8_t SPI::read(uint8_t *recv_data)
{
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	spi->DR = 0xff;
	while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	*recv_data = spi->DR;

	return 0;
}

int8_t SPI::read(uint8_t *recv_data, uint16_t data_length)
{
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		spi->DR = 0xff;
		while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
			;
		*recv_data++ = spi->DR;
	}
	return 0;
}

int8_t SPI::take_spi_right(SPI_CONFIG_TYPE *spi_config)
{
	while ((busy == 1) && (spi_config->dev_num != read_config()))
		delay_ms(1);
	if (spi_config->dev_num == read_config())
	{
		busy = 1;
		return 0;
	}
	config(spi_config);
	busy = 1;
	return 0;
}
int8_t SPI::release_spi_right(void)
{
	busy = 0;
	return 0;
}



