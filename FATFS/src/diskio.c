/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define ATA		0
#define MMC		1
#define USB		2



/*-----------------------------------------------------------------------*/
/* Inicializes a Drive                                                    */

DSTATUS disk_initialize (BYTE drv)    /* Physical drive nmuber (0..) */
{
	/* STD
  DSTATUS stat = STA_NOINIT;
  
  if(HCD_IsDeviceConnected(&USB_OTG_Core_dev))
  {  
    stat &= ~STA_NOINIT;
  }
  
  return stat;
	*/
	
	/* SD Init */
	SD_Error res = SD_OK;
	SD_NVIC_Configuration();
  res	= SD_Init(); 

  if(res == SD_OK)
  {
    res = (SD_Error)0x0;
  }
	else
	{
		while(1);
		//cprintf("#> SD - Not Connected;\n");
	}
  return ((DSTATUS)res);
  
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	/* STD
	DSTATUS stat;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_status();
		// translate the reslut code here

		return stat;

	case MMC :
		result = MMC_disk_status();
		// translate the reslut code here

		return stat;

	case USB :
		result = USB_disk_status();
		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
	*/
	
	if (drv) return STA_NOINIT;		/* Supports only single drive */
  return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	/* STD
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_read(buff, sector, count);
		// translate the reslut code here

		return res;

	case MMC :
		result = MMC_disk_read(buff, sector, count);
		// translate the reslut code here

		return res;

	case USB :
		result = USB_disk_read(buff, sector, count);
		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
	*/
	
  SD_Error status = SD_OK;
  
  SD_ReadMultiBlocks(buff, sector << 9, 512, count);
  
  /* Check if the Transfer is finished */
  status =  SD_WaitReadOperation();
  while(SD_GetStatus() != SD_TRANSFER_OK);
  
  if (status == SD_OK)
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

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	/* STD
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_write(buff, sector, count);
		// translate the reslut code here

		return res;

	case MMC :
		result = MMC_disk_write(buff, sector, count);
		// translate the reslut code here

		return res;

	case USB :
		result = USB_disk_write(buff, sector, count);
		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
	*/
	
 // if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12 )==Bit_SET)//любые записи на карту можно делать только если есть внешнее питание
 // {
    SD_Error status = SD_OK;
    //  while(SD_GetStatus() != SD_TRANSFER_OK  );
    
    SD_WriteMultiBlocks((BYTE *)buff, sector << 9, 512, count);
    
    /* Check if the Transfer is finished */
    status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);     
    
    if (status == SD_OK)
    {
      return RES_OK;
    }
    else
    {
      return RES_ERROR;
    }
 // }
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	/* STD
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		// pre-process here

		result = ATA_disk_ioctl(ctrl, buff);
		// post-process here

		return res;

	case MMC :
		// pre-process here

		result = MMC_disk_ioctl(ctrl, buff);
		// post-process here

		return res;

	case USB :
		// pre-process here

		result = USB_disk_ioctl(ctrl, buff);
		// post-process here

		return res;
	}
	return RES_PARERR;
	*/
	
  DRESULT res = RES_OK;
  switch (ctrl) {
    
    
  case GET_SECTOR_COUNT :	  // Get number of sectors on the disk (DWORD)
    *(DWORD*)buff = 131072;	// 4*1024*32 = 131072
    res = RES_OK;
    break;
    
  case GET_SECTOR_SIZE :	  // Get R/W sector size (WORD) 
    *(WORD*)buff = 512;
    res = RES_OK;
    break;
    
  case GET_BLOCK_SIZE :	    // Get erase block size in unit of sector (DWORD)
    *(DWORD*)buff = 32;
    res = RES_OK;
  }
  
  return res;
}

