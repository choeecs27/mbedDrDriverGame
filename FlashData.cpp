/*
 * FlashData.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "IAP.h"
#include "FlashData.hpp"

extern Serial pc;

IAP     iap;

FlashData* FlashData::pInst = 0;

FlashData* FlashData::GetInstance(void)
{
	if (pInst == 0)
		pInst = new FlashData();
	return pInst;
}

void FlashData::Init(void)
{
	pc.printf( "\r\n\r\n=== IAP: Flash memory commands ===\r\n" );
	pc.printf(         "*** NXP Device-ID = 0x%08X ***\r\n\r\n", iap.read_ID() );
	DEVICE_ID sn = iap.read_serial();
	pc.printf("Device SN: 0x%08X 0x%08X 0x%08X 0x%08X\r\n\r\n",sn.DIN_0, sn.DIN_1, sn.DIN_2, sn.DIN_3);
}

int FlashData::WriteAccelerometerOffsetCalData(sAccelerometerOffsetCalData* calData)
{
	sFlashData sFlashDataForWrite = {};

	if ( isDataEmpty() == false )
	{
		//
		// Read existing data on flash, so when we write cal data, it won't erase the rest
		// of data.  In this application we do not have other data on flash though.
		//
		pData = (sFlashData*) DATA_POINTER;
		memcpy(&sFlashDataForWrite, (void *) pData, sizeof(sFlashData));
	}
	//
	// Copy calData to FlashData Structure
	//
	memcpy(&(sFlashDataForWrite.acceCalData), (void *) calData, sizeof(sAccelerometerOffsetCalData));

	//
	// Write to Flash
	//
	return writeData(&sFlashDataForWrite);
}

sAccelerometerOffsetCalData* FlashData::GetAccelerometerOffsetCalData(void)
{
	sAccelerometerOffsetCalData* pCalData;

	if ( isDataEmpty() == false ) {
		pData = (sFlashData*) DATA_POINTER;
		pCalData = &(pData->acceCalData);
	} else {
		pData = 0;
		pCalData = 0;
	}
	return pCalData;
}

int FlashData::eraseData(void)
{
	//
	// Erase Target Sector if NOT BLANK
	//
	int rc = 0;
	if ( isDataEmpty() == false ) {
		pc.printf("TARGET SECTOR is NOT BLANK!  Erasing...\r\n");
		iap.prepare( TARGET_SECTOR, TARGET_SECTOR );	// Always must prepare sector before erasing or writing
		rc   = iap.erase( TARGET_SECTOR, TARGET_SECTOR );
		pc.printf( "erase result       = 0x%08X\r\n", rc );
	}
	return rc;
}

int FlashData::isDataEmpty(void)
{
	return iap.blank_check( TARGET_SECTOR, TARGET_SECTOR ) != SECTOR_NOT_BLANK;
}

int FlashData::writeData(sFlashData* pFlashData)
{
	// copy struct data into RAM mem buffer
	memset(&mem[0], 0, sizeof(mem));	// Set all elements of mem array to 0

	char *p = (char *) pFlashData;

	for(unsigned int i = 0; i< sizeof(sFlashData);++i)
		mem[i] = *(p+i);

	// erase existing content
	eraseData();

	// copy RAM to Flash
	iap.prepare( TARGET_SECTOR, TARGET_SECTOR );
	int rc   = iap.write( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
	pc.printf( "copied: SRAM(0x%08X)->Flash(0x%08X) for %d bytes. (result=0x%08X)\r\n", mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE, rc );

	// compare
	rc   = iap.compare( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
	pc.printf( "compare result     = \"%s\"\r\n", rc ? "FAILED - Sector was probably not Blank before writing" : "OK" );

	return rc;
}
