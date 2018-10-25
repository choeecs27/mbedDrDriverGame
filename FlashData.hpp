/*
 * FlashData.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: choeecs27
 */

#ifndef FLASHDATA_HPP_
#define FLASHDATA_HPP_

#include "IAP.h"
#include "ScoreBoard.hpp"

class FlashData {
public:
	~FlashData() {delete pInst; pInst = 0;}

	void Init(void);

	int WriteScoreBoardData(sScoreBoard* pSB);
	sScoreBoard* GetScoreBoardData(void);

	static FlashData* GetInstance(void);
	static FlashData* pInst;

private:
	typedef struct
	{
		sScoreBoard scoreBoardData;
	} sFlashData;

    static const int MEM_SIZE = 256;		// memory buffer size must be either 256, 512, 1024 or 4096 when copying to flash
	static const int TARGET_SECTOR = 29;    //  use sector 29 as target sector if it is on LPC1768
	static const int DATA_POINTER = FLASH_SECTOR_29;

	char mem[MEM_SIZE];    //  RAM memory buffer to use when copying data to flash
	sFlashData *pData;

	FlashData() {}

	int eraseData(void);
	int isDataEmpty(void);
	int writeData(sFlashData* pFlashData);


};

#endif /* FLASHDATA_HPP_ */
