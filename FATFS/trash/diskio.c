#include "stm32f4xx.h" 
#include "ffconf.h" 
#include "diskio.h" 

#include "SDIO.h" 
//#include "nand_if.h" 
//#include "fsmc_nand.h" 

SD_CardInfo SDCardInfo2; 

static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */
SD_Error status  = SD_OK;
#define USB		2
#define SECTOR_SIZE 		512U /* Block Size in Bytes */

/*-------------------------------------------------------------------------- 

   Public Functions 

---------------------------------------------------------------------------*/ 


/*-----------------------------------------------------------------------*/ 
/* Initialize Disk Drive                                                 */ 
/*-----------------------------------------------------------------------*/ 
//#pragma optimize=none
DSTATUS disk_initialize (
                         BYTE drv				/* Physical drive nmuber (0..) */
                           )
{ 
  switch (drv) 
  {
    case MMC:
    {     
      NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
      // Initialize SD Card
       status = SD_Init();
      
      if (status == SD_OK) Stat &= ~STA_NOINIT;
    }
  }
  
  return Stat;
  
}


/*-----------------------------------------------------------------------*/ 
/* Get Disk Status                                                       */ 
/*-----------------------------------------------------------------------*/ 

DSTATUS disk_status ( 
BYTE drv	 /* Physical drive number (0) */ 
) 
{ 
return Stat; 
} 


/*-----------------------------------------------------------------------*/ 
/* Read Sector(s)                                                        */ 
/*-----------------------------------------------------------------------*/ 

DRESULT disk_read ( 
BYTE drv,	 /* Physical drive number (0) */ 
BYTE *buff,	 /* Pointer to the data buffer to store read data */ 
DWORD sector,	 /* Start sector number (LBA) */ 
BYTE count	 /* Sector count (1..255) */ 
) 
{
  
SD_Error res = SD_OK;
  
if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }
    //if(!SD_DET())
   // {
    //    return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
   // }

	if(count==1)
    {
		res = SD_ReadBlock(buff, sector << 9, SECTOR_SIZE);        
	}
	else
    {
		res =  SD_ReadMultiBlocks(buff, sector << 9, SECTOR_SIZE, count);        
	} 
	
	if(res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    } 
} 


/*-----------------------------------------------------------------------*/ 
/* Write Sector(s)                                                       */ 
/*-----------------------------------------------------------------------*/ 

DRESULT disk_write ( 
BYTE drv,	 /* Physical drive number (0) */ 
const BYTE *buff,	/* Pointer to the data to be written */ 
DWORD sector,	 /* Start sector number (LBA) */ 
BYTE count	 /* Sector count (1..255) */ 
) 
{ 
SD_Error res = SD_OK;
  
if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }
    //if(!SD_DET())
   // {
    //    return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
   // }

	if(count==1)
    {
		res = SD_WriteBlock((uint8_t*)buff, sector << 9, SECTOR_SIZE);        
	}
	else
    {
		res =  SD_WriteMultiBlocks((uint8_t*)buff, sector << 9, SECTOR_SIZE, count);        
	} 
	
	if(res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    } 
} 


/*-----------------------------------------------------------------------*/ 
/* Get current time                                                      */ 
/*-----------------------------------------------------------------------*/ 

DWORD get_fattime () 
{ 
return	((2006UL-1980) << 25)	      // Year = 2006 
| (2UL << 21)	      // Month = Feb 
| (9UL << 16)	      // Day = 9 
| (22U << 11)	      // Hour = 22 
| (30U << 5)	      // Min = 30 
| (0U >> 1)	      // Sec = 0 
; 
} 



/*-----------------------------------------------------------------------*/ 
/* Miscellaneous Functions                                               */ 
/*-----------------------------------------------------------------------*/ 
 	
DRESULT disk_ioctl (
                    BYTE drv,		/* Physical drive nmuber (0..) */
                    BYTE ctrl,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
  switch (drv) 
  {
    case MMC:
    {      
      switch (ctrl)
      {
        case CTRL_SYNC:
          // no synchronization to do since not buffering in this module
          return RES_OK;
        case GET_SECTOR_SIZE:
          *(WORD*)buff = 512;
          return RES_OK;
        case GET_SECTOR_COUNT:
          *(DWORD*)buff = SDCardInfo2.CardCapacity / 512;
          return RES_OK;
        case GET_BLOCK_SIZE:
          *(DWORD*)buff = 512;
          return RES_OK;
      }
    }
  }
  return RES_PARERR;
}
