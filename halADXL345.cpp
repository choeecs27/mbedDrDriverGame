/*
 * halADXL345.cpp
 *
 *  Created on: Oct 8, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "halADXL345.hpp"

SPI acc(p11,p12,p13);  // MOSI (SDA), MISO (SDO), SCL
DigitalOut cs(p14);
extern Serial pc(USBTX,USBRX);

hal_ADXL345* hal_ADXL345::pInst = 0;

hal_ADXL345* hal_ADXL345::GetInstance(void)
{
	if (pInst == 0)
		pInst = new hal_ADXL345();
	return pInst;
}

void hal_ADXL345::Init(void)
{
	initSPI();

	writeReg(0x31, 0x0B);
	//cs = 0;					// Select the ADXL345
	//acc.write(0x31);		// ADXL345 Data Format Register
	//acc.write(0x0B);		// format +/-16g, 0.003g/LSB
	//cs = 1;					// End of Transmission

	writeReg(0x2D, 0x08);
	//cs = 0;					// Start a new Tranmission
	//acc.write(0x2D);		// ADXL345 Power Control Register
	//acc.write(0x08);		// Select measure mode
	//cs = 1;					// End of Transmission

	unsigned char intSource = GetInterruptSource(); // Dummy read to clear existing interrupt
	setOffsetRegs(0, 0, 0); // Reset offset register
}

void hal_ADXL345::ReadXYZ(int16_t *data)
{
	char buffer[6];

	cs = 0;				// Start a transmission
	acc.write(0x80|0x40|0x32);	// RW bit high, MB bit high, plus address
	for(int i = 0;i <= 5; ++i)
	{
		buffer[i] = acc.write(0x00);	// Read back 6 data bytes
	}
	cs = 1; 			// End of Transmission

	// Interpret the raw data bytes into meaningful results
	data[0] = buffer[1]<<8 | buffer[0]; 	// Combine MSB with LSB
	data[1] = buffer[3]<<8 | buffer[2];
	data[2] = buffer[5]<<8 | buffer[4];
}

void hal_ADXL345::SetSingleTapInterrupt(bool enable, bool isPinInt2)
{
	unsigned char value = (unsigned char) readReg(0x2E);
	if (enable == true)
	{
		writeReg(0x1D, 0x50);  // 3g threshold
		writeReg(0x21, 15); //26 ms duration
		writeReg(0x22, 80); //26 ms latency
		//writeReg(0x23, 200); //26 ms window
		writeReg(0x2A, 0x07); //enable xyz axis
		writeReg(0x2E, value | ADXL345_IRQ_SINGLE_TAP);

		//
		// Set interrupt pin 1 if bool is false.  Otherwise set pin 2
		//
		value = (unsigned char) readReg(0x2F);
		if (isPinInt2 == true)  // pin 2
			writeReg(0x2F, value | ADXL345_IRQ_SINGLE_TAP);
		else // pin 1
			writeReg(0x2F, value & ~ADXL345_IRQ_SINGLE_TAP);
	}
	else
	{
		writeReg(0x2E, value & ~ADXL345_IRQ_SINGLE_TAP);
	}
}

unsigned char hal_ADXL345::GetInterruptSource(void)
{
	return (unsigned char) readReg(0x30);
}

void hal_ADXL345::DoOffsetCalibration(int offset[3])
{
	setOffsetRegs(0, 0, 0); // Reset offset register
	const int CAL_POINT =11;
	int xCal=0, yCal=0, zCal=0;
	int16_t data[3];

	for (int i=0;i<CAL_POINT;i++)
	{
		ReadXYZ(data);
		xCal += data[0];
		yCal += data[1];
		zCal += data[2]-256;
		pc.printf("x:  (%+4d), y:  (%+4d), z:  (%+4d)\r\n",data[0],data[1],data[2]);
		wait_ms(10);
	}
	xCal = ((xCal+(2* xCal > 0? 1:-1)) / (4*CAL_POINT)) * -1;
	yCal = ((yCal+(2* yCal > 0? 1:-1)) / (4*CAL_POINT)) * -1;
	zCal = ((zCal+(2* zCal > 0? 1:-1)) / (4*CAL_POINT)) * -1;

	setOffsetRegs(xCal, yCal, zCal);
	offset[0] = xCal;
	offset[1] = yCal;
	offset[2] = zCal;
}

bool hal_ADXL345::CheckOffsetValue(sAccelerometerOffsetCalData* pAcceCalData)
{
	return pAcceCalData->xOffset > -32768 && pAcceCalData->xOffset < 32767 &&
			pAcceCalData->yOffset > -32768 && pAcceCalData->yOffset < 32767 &&
			pAcceCalData->zOffset > -32768 && pAcceCalData->zOffset < 32767;
}

void hal_ADXL345::SetOffset(sAccelerometerOffsetCalData* pAcceCalData)
{
	setOffsetRegs(pAcceCalData->xOffset, pAcceCalData->yOffset, pAcceCalData->zOffset);
}

void hal_ADXL345::GetOffset(sAccelerometerOffsetCalData* pAcceCalData)
{
	pAcceCalData->xOffset = (int)(readReg(0x1E));
	pAcceCalData->yOffset = (int)(readReg(0x1F));
	pAcceCalData->zOffset = (int)(readReg(0x20));

	if(pAcceCalData->xOffset > 127)
		pAcceCalData->xOffset -= 256;
	if(pAcceCalData->yOffset > 127)
		pAcceCalData->yOffset -= 256;
	if(pAcceCalData->zOffset > 127)
		pAcceCalData->zOffset -= 256;
}

void hal_ADXL345::setOffsetRegs(int x, int y, int z)
{
	writeReg(0x1E, x);
	writeReg(0x1F, y);
	writeReg(0x20, z);
}

void hal_ADXL345::initSPI(void)
{
	// Initialize ADXL345
	cs = 1;					// ADXL345 not selected
	acc.format(8,3);		// 8 bit data, SPI Mode 3
	acc.frequency(2000000);	// 2 MHz SPI clock rate
}

void hal_ADXL345::writeReg(unsigned char addr, char value)
{
	cs = 0;					// Start a new Tranmission
	acc.write(addr);		// ADXL345 Register address
	acc.write(value);		// value
	cs = 1;					// End of Transmission
}

char hal_ADXL345::readReg(unsigned char addr)
{
	char value;
	cs = 0;					// Start a new Tranmission
	acc.write(0x80|addr);	// ADXL345 Register address
	value = acc.write(0x00);		// value
	cs = 1;					// End of Transmission
	return value;
}
