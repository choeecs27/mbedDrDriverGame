/*********************************************************************************************************/
//
// Final project - Dr. Driver
//
//
//  TextLDC Library by Simon Ford  https://os.mbed.com/users/simon/code/TextLCD/docs/tip/classTextLCD.html
//
/*********************************************************************************************************/

#include "mbed.h"
#include "TextLCD.h"
#include "DrDriverGameMain.hpp"
#include "halShockInterrupt.hpp"
#include "halADXL345.hpp"
#include "FlashData.hpp"
//
// Below defines system HAL resources
//
// Timer, TextLCD,
// Custom character programming
// Interrupt for input
//    (WIP) Final solution is to use accelerometer with Active high (when interrupt occurred) according to spec
// (WIP) SPI 3-x accelerometer
// (WIP) DigitalOut for flashing LCD backlight
// (WIP) LED4 for TURBO
// (WIP) 3 DigitalOut for Traffic light
// (WIP) LED1, LED2 for lives
//
//TextLCD::TextLCD(PinName rs, PinName e, PinName d4, PinName d5,
//                 PinName d6, PinName d7, LCDType type)
//
TextLCD lcd(p20, p19, p21, p22, p23, p24); // rs, e, d4-d7
Timer gameTimer;
Timer shockIntTimer;
Serial pc(USBTX,USBRX);

//
// shock interrupt to use pull down mode 2
//
InterruptIn shockIntIn(p10);

//DigitalIn steer(p26);



int main() 
{
	//
	// System init
	//
	pc.baud(115200);
	pc.printf("Dr. Driver!...\r\n\r\n");

	ShockIntInit();

	hal_ADXL345* acce = hal_ADXL345::GetInstance();

	acce->Init();

	pc.printf("\r\n\r\nADXL345 in operation...\r\n");

	acce->SetSingleTapInterrupt(true, false);

	DrDriverGameMain game;
	while(1)
	{
		pc.printf("Let the game starting!\r\n");
		game.MainStateMachine();
	}
}
