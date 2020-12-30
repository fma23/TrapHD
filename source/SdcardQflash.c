/*
 * SdcardQflash.c
 *
 *  Created on: Dec. 29, 2020
 *      Author: Farid Mabrouk
 */


#include <FlashConfig.h>
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <string.h>
#include "fsl_sd.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "board.h"
#include "sdmmc_config.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "ImageData.h"





//#define IMGBUF_SIZE  7870


static status_t sdcardWaitCardInsert(void);
static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */
//static uint8_t s_nor_read_buffer[IMGBUF_SIZE];



//Functions
bool QSPIflashSaveImage(void);
bool TestSdCard(void);


//extern uint8_t g_bufferWrite[IMGBUF_SIZE];
//extern uint8_t g_bufferRead[IMGBUF_SIZE];








/*******************************************************************************
* Function Name  : sdcardWaitCardInsert
* Description    : this function detect an SD card once inserted
* Return         : true or false
*******************************************************************************/
status_t sdcardWaitCardInsert(void)
{
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }

    /* wait card insert */
    if (SD_PollingCardInsert(&g_sd, kSD_Inserted) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power off card */
        SD_SetCardPower(&g_sd, false);
        /* power on the card */
        SD_SetCardPower(&g_sd, true);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}



/*******************************************************************************
* Function Name  : TestSdCard
* Description    : this function test the sd card driver; it saves and a read a
*                  a saved jpeg image
* Return         : true or false
*******************************************************************************/
bool TestSdCard(void)
{
    FRESULT error;
    DIR directory;               /* Directory object */
    UINT bytesWritten;
    UINT bytesRead;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    BYTE work[FF_MAX_SS];


    //Configure SD card
    if (sdcardWaitCardInsert() != kStatus_Success)
    {
      PRINTF("\r\nCard not inserted.\r\n");
        return false;
    }
    else
    {
      PRINTF("\r\nCard detected.\r\n");
    }

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return false;
    }
    else
    {
      PRINTF("Mount volume succeeded.\r\n");
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return false;
    }
#endif

#if FF_USE_MKFS
    PRINTF("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
    if (f_mkfs(driverNumberBuffer, 0, work, sizeof work))
    {
        PRINTF("Make file system failed.\r\n");
        return false;
    }
#endif /* FF_USE_MKFS */

    PRINTF("\r\nCreate directory......\r\n");
    error = f_mkdir(_T("/img_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            return false;
        }
    }

    PRINTF("\r\nCreate a file in that directory......\r\n");
    error = f_open(&g_fileObject, _T("/img_1/jpg_5.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            return false;
        }
    }

    PRINTF("\r\nList the file in that directory......\r\n");
    if (f_opendir(&directory, "/img_1"))
    {
        PRINTF("Open directory failed.\r\n");
        return false;
    }

    g_bufferWrite[IMGBUF_SIZE - 2U] = '\r';
    g_bufferWrite[IMGBUF_SIZE - 1U] = '\n';

    PRINTF("\r\nWrite to above created file.\r\n");
    error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
    if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
    {
       PRINTF("Write file failed. \r\n");
       return false;
    }

    /* Move the file pointer */
    if (f_lseek(&g_fileObject, 0U))
    {
            PRINTF("Set file pointer position failed. \r\n");
            return false;
    }

    PRINTF("Read from above created file.\r\n");
    memset(g_bufferRead, 0U, sizeof(g_bufferRead));
    error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);

    if((error) || (bytesRead != sizeof(g_bufferRead)))
    {
       PRINTF("Read file failed. \r\n");
       return false;
    }

    PRINTF("Compare the read/write content......\r\n");
    if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
    {
       PRINTF("Compare read/write content isn't consistent.\r\n");
       return false;
    }
    PRINTF("The read/write content is consistent.\r\n");

    PRINTF("\r\nClosing file now.\r\n");
    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
        return false;
    }
    else
    {
      PRINTF("\r\nFile closed successfully.\r\n");
    }

    return true;
}

//
///*******************************************************************************
//* Function Name  : QSPIflashSaveImage
//* Description    : This function tests the read, write, erase for QSPI flash
//* Return         : true or false
//*******************************************************************************/
//bool QSPIflashSaveImage(void)
//{
//	status_t status;
//	uint8_t vendorID = 0;
//
//	PRINTF("\r\nTesting the Flash started!\r\n");
//
//    /* SPI Flash Initialization */
//    flexspi_nor_flash_init(FLASH_FLEXSPI);
//
//    /* Get vendor ID. */
//	status = flexspi_nor_get_vendor_id(FLASH_FLEXSPI, &vendorID);
//	if (status != kStatus_Success)
//	{
//	   return false;
//	}
//    PRINTF("Vendor ID: 0x%x\r\n", vendorID);
//
//
//   /* Enter quad mode. */
//   status = flexspi_nor_enable_quad_mode(FLASH_FLEXSPI);
//   if (status != kStatus_Success)
//   {
//       return false;
//   }
//
//   /* Erase sectors. */
//   PRINTF("Erasing Serial NOR over FlexSPI...\r\n");
//   status = flexspi_nor_flash_erase_sector(FLASH_FLEXSPI, FLASH_SECTOR * SECTOR_SIZE);
//   if (status != kStatus_Success)
//   {
//       PRINTF("Erase sector failure !\r\n");
//       return false;
//   }
//
//
//   //memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
//   memset(g_bufferWrite, 0xFFU, sizeof(g_bufferWrite));
//
//   DCACHE_InvalidateByRange(FLASH_FLEXSPI_AMBA_BASE + FLASH_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
//
//  // memcpy(s_nor_read_buffer, (void *)(FLASH_FLEXSPI_AMBA_BASE + FLASH_SECTOR * SECTOR_SIZE),sizeof(s_nor_read_buffer));
//   memcpy(g_bufferRead, (void *)(FLASH_FLEXSPI_AMBA_BASE + FLASH_SECTOR * SECTOR_SIZE),sizeof(s_nor_read_buffer));
//
//   //if (memcmp(g_bufferWrite, s_nor_read_buffer, sizeof(g_bufferWrite)))
//   if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
//   {
//       PRINTF("Erase data -  read out data value incorrect !\r\n ");
//       return false;
//   }
//   else
//   {
//       PRINTF("Erase data - successfully. \r\n");
//   }
//
//  // status = flexspi_nor_flash_page_program(FLASH_FLEXSPI, FLASH_SECTOR * SECTOR_SIZE, (void *)s_nor_program_buffer);
//   status = flexspi_nor_flash_page_program(FLASH_FLEXSPI, FLASH_SECTOR * SECTOR_SIZE, (void *)g_bufferWrite);
//
//   if (status != kStatus_Success)
//   {
//       PRINTF("Page program failure !\r\n");
//       return false;
//   }
//
//   DCACHE_InvalidateByRange(FLASH_FLEXSPI_AMBA_BASE + FLASH_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
//
//   memcpy(g_bufferRead, (void *)(FLASH_FLEXSPI_AMBA_BASE + FLASH_SECTOR * SECTOR_SIZE),sizeof(g_bufferRead));
//
//   //if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
//   if (memcmp(g_bufferRead, g_bufferWrite, sizeof(g_bufferWrite)) != 0)
//   {
//       PRINTF("Program data -  read out data value incorrect !\r\n ");
//       return false;
//   }
//   else
//   {
//       PRINTF("Program data - successfully. \r\n");
//   }
//
//    return true;
// }
//

