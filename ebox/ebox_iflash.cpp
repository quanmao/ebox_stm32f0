/*
file   : iflash.cpp
author : shentq
version: V1.1
date   : 2015/7/5
date   : 2016/6/30 LQM移植到STM32F0平台
Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "ebox_iflash.h"



/**
 *@name     int erase_sector(FLASH_SECTOR_ADDR_t start_addr)
 *@brief    F0系列Flash实际是按页排列的，所以这里实际是檫除1页
 *@param    start_addr：  要檫除的页地址
 *@retval   NONE
*/
int FLASHCLASS::erase_sector(FLASH_SECTOR_ADDR_t start_addr)
{
	/*Variable used for Erase procedure*/
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = start_addr;
	EraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		/* Error occurred while sector erase.
		   User can add here some code to deal with this error  */
		while (1)
		{
		}
	}
	HAL_FLASH_Lock();
}

int FLASHCLASS::write_sector(FLASH_SECTOR_ADDR_t start_addr, uint16_t *buf, uint32_t num_to_write)
{

    HAL_FLASH_Unlock();
       /* Clear pending flags (if any) */  
    // FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  // FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    
    // if (FLASH_EraseSector(addr_to_sector(start_addr), VoltageRange_3) != FLASH_COMPLETE)
    // { 
      // /* Error occurred while sector erase. 
         // User can add here some code to deal with this error  */
      // while (1)
      // {
      // }
    // }
    write_without_check(start_addr,buf,num_to_write);    
    HAL_FLASH_Lock();
    return num_to_write;
}
int FLASHCLASS::read(uint32_t Address, uint16_t *buf, int32_t num_to_read)
{
    int i = 0;
    while(i < num_to_read )
    {
        *(buf + i) = *(__IO uint16_t*) Address;
        i+=1;
        Address = Address + 2;
    }
    return i;
}


uint16_t FLASHCLASS::addr_to_sector(uint32_t Address)
{
  
  // if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  // {
    // return FLASH_Sector_0;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  // {
    // return FLASH_Sector_1;

  // }
  // else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  // {
    // return FLASH_Sector_2;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  // {
    // return FLASH_Sector_3;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  // {
    // return FLASH_Sector_4;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  // {
    // return FLASH_Sector_5;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  // {
    // return FLASH_Sector_6;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  // {
    // return FLASH_Sector_7;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  // {
    // return FLASH_Sector_8;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  // {
    // return FLASH_Sector_9;
  // }
  // else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  // {
    // return FLASH_Sector_10;
  // }
}

/**
 *@name     uint16_t write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
 *@brief    F0系列Flash实际是按页排列的，所以这里实际是檫除1页
 *@param    iAddress  要檫除的页地址
	        *buf 	  指向要写入的数据缓冲区
			iNumByteToWrite 要写入多少数据
 *@retval   NONE
*/
uint16_t FLASHCLASS::write_without_check(uint32_t iAddress, uint16_t *buf, uint16_t iNumByteToWrite)
{
	uint16_t i;
	volatile HAL_StatusTypeDef FLASHStatus = HAL_OK;
	i = 0;

	while ((i < iNumByteToWrite) && (FLASHStatus == HAL_OK))
	{
		FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,iAddress, *buf);
		i = i + 1;
		iAddress = iAddress + 2;
		buf = buf + 1;
	}
	return iNumByteToWrite;
}
