/*
 * MX25L4006E.h
 *
 *  Created on: Dec. 15, 2020
 *      Author: Farid Mabrouk
 */

#ifndef MX25L4006E_H_
#define MX25L4006E_H_



//Byte organization
#define		MX25L4006E_PAGE_SIZE				256
#define 	MX25L4006E_PAGE_COUNT				2048

#define    	MX25L4006E_SECTOR_SIZE				4096UL
#define     MX25L4006E_SECTOR_COUNT				128

#define		MX25L4006E_BLOCK_SIZE				MX25L4006E_SECTOR_SIZE * 16
#define 	MX25L4006E_BLOCK_COUNT				8

#define    	MX25L4006E_ADDRESS_MAX 				(MX25L4006E_SECTOR_SIZE * MX25L4006E_SECTOR_COUNT)



//register commands
#define    	FLASH_CMD_WRSR      	0x01    // WRSR (Write Status register)
#define    	FLASH_CMD_RDSR      	0x05    // RDSR (Read Status register)


//Status register
#define		STATUS_FLAG_WIP			0x01 	// Write in progress bit
#define		STATUS_FLAG_WEL			0x02 	// Write enable latch bit
#define    	FLASH_CMD_RDID      	0x9F    // RDID (Read Identification)

//Program commands
#define    	FLASH_CMD_WREN     		0x06    // WREN (Write Enable)
#define    	FLASH_CMD_WRDI     		0x04    // WRDI (Write Disable)
#define    	FLASH_CMD_PP       		0x02    // PP (page program)

//READ commands
#define    	FLASH_CMD_READ        	0x03    // READ (1 x I/O)
#define    	FLASH_CMD_FASTREAD    	0x0B    // FAST READ (Fast read data)
#define    	FLASH_CMD_DREAD       	0x3B    // DREAD (1In/2 Out fast read)


//Erase commands
#define    	FLASH_CMD_SE       		0x20    // SE (Sector Erase)
#define    	FLASH_CMD_BE       		0xD8    // BE (Block Erase)
#define    	FLASH_CMD_CE       		0x60    // CE (Chip Erase) hex code: 60 or C7


//SPI defines
#define LPSPI1_MASTER_BASEADDR           (LPSPI1)
#define TRANSFER_BAUDRATE                500000U             /*! Transfer baudrate - 500k */
#define LPSPI_MASTER_PCS_FOR_INIT        (kLPSPI_Pcs0)
#define LPSPI_MASTER_PCS_FOR_TRANSFER    (kLPSPI_MasterPcs0)
#define LPSPI_MASTER_CLK_FREQ            (CLOCK_GetFreqFromObs(CCM_OBS_LPSPI1_CLK_ROOT))





/* functions */
void SPI1_Init(void);

bool SPI_FLASH_ReadID(uint8_t FlashID[]);
bool checkStatus(uint8_t Flag);
bool commandWriteEnable(void);
bool ReadMemory(uint32_t address, uint8_t Rdatabuf[], int size, int timeout);
bool commandPageProgram(uint32_t address, uint8_t Wrdatabuf[], uint8_t FlashDataRd[], int size, int timeout);
bool commandSectorErase(uint32_t address, int timeout);

bool MX25L4006E_Read(uint32_t address, uint8_t buf[], int size, int timeout);
bool MX25L4006E_Write(uint32_t address, uint8_t Wrbuf[], uint8_t Rdbuf[], int size, int timeout);
bool MX25L4006E_EraseSector(uint32_t sectorNumber, int timeout, bool waitUntil);


bool testSPItransfer(void);
bool TestMX25L4006E(void);


#endif /* MX25L4006E_H_ */
