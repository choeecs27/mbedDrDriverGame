/*
 * halADXL345.h
 *
 *  Created on: Oct 8, 2018
 *      Author: choeecs27
 */

#ifndef HALADXL345_HPP_
#define HALADXL345_HPP_

#define ADXL345_IRQ_OVERRUN     0x01
#define ADXL345_IRQ_WATERMARK   0x02
#define ADXL345_IRQ_FREE_FALL   0x04
#define ADXL345_IRQ_INACT       0x08
#define ADXL345_IRQ_ACTIVITY    0x10
#define ADXL345_IRQ_DOUBLE_TAP  0x20
#define ADXL345_IRQ_SINGLE_TAP  0x40
#define ADXL345_IRQ_DATA_READY  0x80

#define ADXL345_ADDR_INT_ENABLE 0x2E
#define ADXL345_ADDR_INT_MAP    0x2F

typedef struct
{
	int xOffset;
	int yOffset;
	int zOffset;
} sAccelerometerOffsetCalData;

class hal_ADXL345 {
public:
	~hal_ADXL345(){ delete pInst; pInst = 0; }

	void Init(void);
	void ReadXYZ(int16_t *data);
	void SetSingleTapInterrupt(bool enable, bool isPinInt2);
	void DoOffsetCalibration(int offset[3]);
	bool CheckOffsetValue(sAccelerometerOffsetCalData* pAcceCalData);
	void SetOffset(sAccelerometerOffsetCalData* pAcceCalData);
	void GetOffset(sAccelerometerOffsetCalData* pAcceCalData);

	unsigned char GetInterruptSource(void);

	static hal_ADXL345* GetInstance(void);

	static hal_ADXL345* pInst;
private:

	hal_ADXL345(){}

	void setOffsetRegs(int x, int y, int z);

	void initSPI(void);
	void writeReg(unsigned char addr, char value);
	char readReg(unsigned char addr);


};

#endif /* HALADXL345_HPP_ */
