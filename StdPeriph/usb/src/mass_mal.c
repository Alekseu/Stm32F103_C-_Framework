/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : mass_mal.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"
#include "../../../StdPeriph/cmsis_boot/stm32f10x.h"
#include "../../../StdPeriph/stm_lib/inc/stm32f10x_flash.h"
#include "../inc/mass_mal.h"

#include "../../../Device/sdCard/inc/diskio.h"
#include "../../../Device/sdCard/inc/ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
DRESULT res;
volatile FATFS	fs;
unsigned char data[FLASH_PAGE_SIZE];

char init=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{

  switch (lun)
  {
    case 0:
			//FLASH_Unlock();

    	//res = disk_initialize(0);

      break;
    case 1:
	  return MAL_FAIL;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

void Internal_Flash_Write(unsigned char* data, unsigned int address, unsigned int count) {

	unsigned int i;

	uint32_t HalfWord;
	count = count/2;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(address);
	while(count--)
	  {
	    HalfWord = *(data++);
	    HalfWord|=*(data++)<<8;
	    FLASH_ProgramHalfWord(address,HalfWord);
	    address+=2;
	  }
	FLASH_Lock();
}

/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{

  uint16_t i;
  uint8_t*  buff = (uint8_t*)Writebuff;

  switch (lun)
  {
    case 0:

//    	//Internal_Flash_Write((unsigned char*)buff,FLASH_DISK_START_ADDRESS + Memory_Offset,Transfer_Length);
//
//	   for(i = 0; i < Transfer_Length; i += FLASH_PAGE_SIZE)
//	   {
//		   FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//	    if (FLASH_WaitForLastOperation(WAIT_TIMEOUT) != FLASH_TIMEOUT)
//		  {
//		     FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//		  }
//		  FLASH_ErasePage(FLASH_DISK_START_ADDRESS + Memory_Offset + i);
//	   }
//
//	   for(i = 0; i < Transfer_Length; i += 4)
//	   {
//		   FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//		   if(FLASH_WaitForLastOperation(WAIT_TIMEOUT) != FLASH_TIMEOUT)
//		   {
//		      FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//		   }
//		   FLASH_ProgramWord(FLASH_DISK_START_ADDRESS + Memory_Offset + i , Writebuff[i >> 2]);
//	   }
    	memcpy(data,buff, FLASH_PAGE_SIZE);
    	res = disk_write(fs.drv,data, Memory_Offset, Transfer_Length/512);

    	if(res==RES_ERROR)
    	{
    		return MAL_FAIL;
    	}

      break;
    case 1:
      break;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

char* FLASH_Read(uint32_t address)
{

    return (char*)address;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{
  uint16_t i;
 uint8_t*  buff = (uint8_t*)Readbuff;

  if(!init)
  {
	  init=1;
	f_mount(0, &fs);
  	disk_initialize(fs.drv);
  	disk_read(fs.drv,data,0,1);
  }


  switch (lun)
  {
    case 0:

//    	memcpy(buff, FLASH_Read(FLASH_DISK_START_ADDRESS + Memory_Offset), Transfer_Length);
//    	for(i = 0; i < Transfer_Length; i += 4)
//    	      {
//    	        Readbuff[i >> 2] = ((vu32*)(FLASH_DISK_START_ADDRESS + Memory_Offset))[i >> 2];
//    	      }
    	res =	disk_read(fs.drv,data,Memory_Offset,Transfer_Length/512);

    	if(res==RES_ERROR)
    	{
    		return MAL_FAIL;
    	}

    	memcpy(buff, data, FLASH_PAGE_SIZE);

      break;
    case 1:
      break;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
  if (lun == 0)
  {
	  Mass_Block_Count[0] = FLASH_DISK_SIZE / FLASH_PAGE_SIZE;
      Mass_Block_Size[0] =  FLASH_PAGE_SIZE;
      Mass_Memory_Size[0] = FLASH_DISK_SIZE;
      return MAL_OK;
  }

  return MAL_FAIL;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
