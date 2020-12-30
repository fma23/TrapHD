/*
 * MX25L4006E.c
 *
 *  Created on: Dec. 14, 2020
 *      Author: Farid Mabrouk
 */


//#include <spiFlash/MX25L4006E.h>
#include "MX25L4006E.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"



lpspi_master_config_t masterConfig;


/*******************************************************************************
* Function Name  : SPI1 Init
* Description    : Initialize SPI1 peripheral
* Return         : Nothing
*******************************************************************************/
void SPI1_Init(void)
{
    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);  // these are the default setting for the flash for 32bits per frame
    LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig,  LPSPI_MASTER_CLK_FREQ);
}

/******************************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification into FlashID buffer
* Return         : true for read ID success or false for read ID fail
*******************************************************************************************/
bool SPI_FLASH_ReadID(uint8_t FlashID[])
{
	status_t status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
	uint8_t BuffSize=4;                    /* transmitting 4 bytes at a time */

	uint8_t TxBuffer[4]={0,0,0,FLASH_CMD_RDID};
	uint8_t RxData[4]={0};

	/* First 4 bytes to transmit are for read command, where MSB bit
	   on MOSI is set to 1 */
	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=(uint8_t*)&RxData;       /* rxData is NUll since we are not expecting any rely yet */

	Data2TX.dataSize=BuffSize;
	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
	  PRINTF("SPI transfer fails!");
	  return false;
	}

	FlashID[0]= RxData[2];   /*Manufacture ID */
	FlashID[1]= RxData[1];   /*Memory Type */
	FlashID[2]= RxData[0];   /*Memory capacity */


	return true;
}


/************************************************************************************************
* Function Name  : commandWriteEnable
* Description    : sets Write Enable Latch (WEL) bit
* Return         : true upon success to enable write bit or false upon failure to set write bit
*************************************************************************************************/
bool commandWriteEnable(void)
{
	    status_t status;
		lpspi_transfer_t Data2TX= {0,0,0,0};
		uint8_t BuffSize=1;                    /* transmitting 1 byte */

		uint8_t TxBuffer[1]={0};

		TxBuffer[0]=FLASH_CMD_WREN;

		Data2TX.txData=(uint8_t*)&TxBuffer;
		Data2TX.rxData=NULL;                     /* rxData is NUll since we are not expecting any rely yet */
		Data2TX.dataSize=BuffSize;


		/* change default setup to one byte for spi MOSI */
		LPSPI_MasterGetDefaultConfig(&masterConfig);
		masterConfig.bitsPerFrame = 8;                 //sending only 8 bits
	    LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

		status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
		if(status!=0)
		{
			PRINTF("Command write enable failed!");
			return false;
		}

        //set the bits per frame to 32 bits: default setup
		LPSPI_MasterGetDefaultConfig(&masterConfig);
		masterConfig.bitsPerFrame = 32;    //sending 32 bits per frame
		LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig,LPSPI_MASTER_CLK_FREQ);

		return true;
}

/*******************************************************************************
* Function Name  : checkStatus
* Description    : check flash status bits flags
* Return         : true or false
*******************************************************************************/
bool checkStatus(uint8_t Flag)
{
	bool status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
	uint8_t BuffSize=4;                    /* transmitting 4 bytes at a time */

	uint8_t TxBuffer[4]={0x00,0x00,0x00,FLASH_CMD_RDSR};
	uint8_t RxData[4]={0};

	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=(uint8_t*)&RxData;

	Data2TX.dataSize=BuffSize;
	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
		PRINTF("failed to read Flash status! \n");
		return false;
	}

	return (RxData[1]&Flag);
}

/*******************************************************************************
* Function Name  : commandPageProgram
* Description    : command page program
* Return         : true or false
*******************************************************************************/
bool commandPageProgram(uint32_t address, uint8_t Wrdatabuf[], uint8_t FlashDataRd[], int size, int timeout)
{
	status_t status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
    uint8_t TxBuffer[260]={0};
    uint8_t RxBuffer[260]={0};
    uint8_t RdTxBuffer[4]={0};
    uint32_t i=0;


	if (address > MX25L4006E_ADDRESS_MAX)
	{
		PRINTF("commandPageProgram error: invalid address! \n");
		return false;
	}

	if ((address & 0xFF) + size > 256)
	{
		PRINTF("commandPageProgram error: write operation will exceed one page! address is: %d", address);
		return false;
	}

	while (!checkStatus(STATUS_FLAG_WEL))
	{
		//Send WREN command which sets WEL bit: Write Enable Latch
		commandWriteEnable();

	    //Wait millisecond
	    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		if (timeout-- == 0)
	    {
		  PRINTF("commandPageProgram error: erase failed! timeout! \n");
		  return false;
		}
	}

	//set the bits per frame to 32 bits
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame =(size*8)+32; //number of bytes to writes + 4 command bytes
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig,LPSPI_MASTER_CLK_FREQ);

	TxBuffer[3]= FLASH_CMD_PP;
	TxBuffer[2]= (address>>16);
	TxBuffer[1]= (address>>8);
	TxBuffer[0]= address;

	for( int i=0; i<size; i++)
	{
	  TxBuffer[4+i]=Wrdatabuf[i];
	}

	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=NULL;
    Data2TX.dataSize=size+4;

	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
	   PRINTF( "commandPageProgram error: Flash SPI command write fails!\n");
	   return false;
	}

	//Set the bits frame to 32
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame =32;
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

	/* wait till write in process bit (WIP is cleared) */
	while (checkStatus(STATUS_FLAG_WIP))
	{
	   //Wait one millisecond
	   SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

	   if (timeout-- == 0)
	   {
		  PRINTF("commandPageProgram error:WIP flag timeout! \n");
		  return false;
	   }
	}

	 //read back the data
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame = size*8+32;
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

	RdTxBuffer[3]= FLASH_CMD_READ;
	RdTxBuffer[2]= (address>>16);
	RdTxBuffer[1]= (address>>8);
	RdTxBuffer[0]= address;

	Data2TX.txData=(uint8_t*)&RdTxBuffer;
	Data2TX.rxData=(uint8_t*)&RxBuffer;
    Data2TX.dataSize=size+4;

    status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
    if(status!=0)
    {
      PRINTF( "read page verification failed!\n");
      return false;
    }

    for(i=0; i<size; i++)
    {
     FlashDataRd[i]=RxBuffer[i+4];
    }

    if(memcmp(Wrdatabuf, FlashDataRd,size))
    {
       PRINTF("flash written data does not match data that was just read !\r\n ");
       return false;
    }
    else
    {
       PRINTF("flash write page success. \r\n");
    }

	//done
	//Set the bits frame to 32
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame =  32;
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

    return true;
}



/*******************************************************************************
* Function Name  : ReadflashPage
* Description    : Read flash Page
* Return         : true or false
*******************************************************************************/
bool ReadMemory(uint32_t address, uint8_t Rdatabuf[], int size, int timeout)
{
	status_t status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
    uint8_t TxBuffer[4]={0};
    uint8_t RxBuffer[260]={0};


	if (address > MX25L4006E_ADDRESS_MAX)
	{
		PRINTF("error: invalid address!");
		return false;
	}

	while (checkStatus(STATUS_FLAG_WIP))
	{
		/* do nothing */
		if (timeout-- == 0)
		{
			PRINTF("checking status failed! timeout!");
			return false;
		}

		//Delay 1 millisecond
		SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
	}

	//Set the bits frame to 256
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame =  (size*8)+32;    //256 bytes of data+4 bytes for commands bytes for command
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

	TxBuffer[0] = address;
	TxBuffer[1] = (address >> 8);
	TxBuffer[2] = (address >>16);
	TxBuffer[3] = FLASH_CMD_READ;

	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=(uint8_t*)&RxBuffer;
	Data2TX.dataSize=size+4;       //total number of RX bytes+4 command bytes

	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
	  PRINTF( "Flash SPI transfer fails!");
	  return false;
	}

	memcpy(Rdatabuf, &RxBuffer[4], size);

	//Set the bits frame to 32
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame = 32;                           //sending 32 bits per frame
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

	return true;
}


/*******************************************************************************
* Function Name  : commandSectorErase
* Description    : command to erase a sector
* Return         : true or false
*******************************************************************************/
bool commandSectorErase(uint32_t address, int timeout)
{
	status_t status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
	uint8_t BuffSize=4;                    /* transmitting 4 bytes at a time */
	uint8_t TxBuffer[4]={0};

	if (address > MX25L4006E_ADDRESS_MAX)
	{
		PRINTF("invalid address!");
		return false;
	}

	while (checkStatus(STATUS_FLAG_WIP))
	{
		/* do nothing */
		if(timeout-- == 0)
		{
			PRINTF("erase failed! timeout!");
			return false;
		}

		//Delay 1 millisecond
	    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
	}

	if (!commandWriteEnable())
	{
		PRINTF("command write enable failed during erase command!");
		return false;
	}

	TxBuffer[0]= address;
	TxBuffer[1]= (address >> 8);
	TxBuffer[2]= (address >> 16);
	TxBuffer[3]=FLASH_CMD_SE;

	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=NULL;

	Data2TX.dataSize=BuffSize;
	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
	  PRINTF( "Flash SPI erase command  transfer fails!");
	  return false;
	}

	return true;

}
/*******************************************************************************
* Function Name  : MX25L4006E_EraseSector
* Description    : Erase a Flash sector
* Return         : true or false
*******************************************************************************/
bool MX25L4006E_EraseSector(uint32_t sectorNumber, int timeout, bool waitUntil)
{
	if (sectorNumber >= MX25L4006E_SECTOR_COUNT)
	{
		PRINTF("invalid sector number!");
		return false;
	}

	if (!commandSectorErase(MX25L4006E_SECTOR_SIZE * sectorNumber, timeout))
	{
		PRINTF("erase sector%d failed!", sectorNumber);
		return false;
	}

	if (waitUntil)
	{
		while (checkStatus(STATUS_FLAG_WIP))
		{
			if (timeout-- == 0)
			{
				PRINTF("erase sector timeout!");
				return false;
			}

			//Delay 1 millisecond
		    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
		}
	}

	return true;
}

/*******************************************************************************
* Function Name  : MX25L4006E_Read
* Description    : Read MX25L4006E memory
* Return         : true or false
*******************************************************************************/
bool MX25L4006E_Read(uint32_t address, uint8_t buf[], int size, int timeout)
{
	return ReadMemory(address, buf, size, timeout);
}

/*******************************************************************************
* Function Name  : MX25L4006E_write
* Description    : write page for MX25L4006E memory
* Return         : true or false
*******************************************************************************/
bool MX25L4006E_Write(uint32_t address, uint8_t Wrbuf[], uint8_t Rdbuf[], int size, int timeout)
{
	return commandPageProgram(address, Wrbuf, Rdbuf, size, timeout);
}


/*******************************************************************************
* Function Name  : testSPItransfer
* Description    : test spi1 transfer
* Return         : true or false
*******************************************************************************/
bool testSPItransfer(void)
{
	status_t status;
	lpspi_transfer_t Data2TX= {0,0,0,0};
	uint8_t BuffSize=10;                    /* transmitting 4 bytes at a time */

	uint8_t TxBuffer[10]={0};
	uint8_t RxData[10]={0};

	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.bitsPerFrame = 80;                     //256 bytes of data+4 bytes for commands bytes for command
	LPSPI_MasterInit(LPSPI1_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

	for(uint8_t i=0; i<10; i++)
	{
		TxBuffer[i]=i;
	}

	Data2TX.txData=(uint8_t*)&TxBuffer;
	Data2TX.rxData=(uint8_t*)&RxData;

	Data2TX.dataSize=BuffSize;
	status= LPSPI_MasterTransferBlocking(LPSPI1_MASTER_BASEADDR,&Data2TX);
	if(status!=0)
	{
		PRINTF("SPI Transfer fails! \n");
		return false;
	}

	return true;
}

/*******************************************************************************
* Function Name  : TestMX25L4006E
* Description    : test spi flash: read, write, erase, read device ID
* Return         : true or false
*******************************************************************************/
bool TestMX25L4006E (void)
{
   bool status=false;
   uint8_t SPIFlash_ID[3]={0};

   uint8_t FlashDataWr[256]={0};
   uint8_t FlashDataRd[256]={0};
   uint32_t size;

   int i=0;

   /*** testing the LPSPI flash ****/
   status= SPI_FLASH_ReadID(SPIFlash_ID);
   if(status==false)
   {
       PRINTF( "Failed to read Flash ID! \r\n");
       return false;
   }

   status=MX25L4006E_EraseSector(0, 300, true);  /*erase a sector */
   if(status==false)
     {
         PRINTF( "Failed to erase sector \r\n");
         return false;
     }

   for(i=0;i<260;i++)      /* Initialize flash page buffer */
   {
     FlashDataWr[i]=i+8;    //writing dummy data
   }

   /*write page */
   size=256;                  //number of bytes to write
   status= MX25L4006E_Write(0,FlashDataWr,FlashDataRd,size,500);
   if(status==false)
   	{
   	  PRINTF( "Flash SPI transfer fails! \r\n");
   	  return false;
   	}


   /* read memory */
   size=256;  //number of bytes to read
   status= MX25L4006E_Read(0,FlashDataRd,size,500);
   if(status==false)
   {
	   PRINTF("Memory read fails\r\n");
   }

   PRINTF("Data read from the SPI Flash is:\r\n");
   for(i=0; i<size;i++)
   {
     PRINTF(" %d \n",FlashDataRd[i]);
   }

   return true;
}
