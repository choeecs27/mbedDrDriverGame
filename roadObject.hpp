/*
 * roadObject.hpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#ifndef ROADOBJECT_HPP_
#define ROADOBJECT_HPP_

enum objType{
	OBSTACLE,
	ROAD_CAR,
	MR_DRIVER,
	OBJ_TYPE_COUNT,
};

class roadObject
{
public:
	roadObject() {}
	virtual ~roadObject() {}

	virtual bool UpdatePosition(float extSpeed, unsigned int currentTime);
	virtual bool DoIBlockDrDriver(unsigned int vPos, unsigned int hPos);
	virtual unsigned int GetObjType(void);
	virtual void PrintOnLcd(char line[2][17]);

	unsigned int GetHPosition(void) {return hPosition;}
	unsigned int GetVPosition(void) {return vPosition;}

protected:

	unsigned int timeAppear;
	unsigned int vPosition;
	unsigned int hPosition;
	unsigned int width;
	float speed;
};

class obstacle: public roadObject
{
public:
	obstacle(int vPosition, unsigned int timeAppear);

	unsigned int GetObjType(void){return OBSTACLE;}
	bool UpdatePosition(float extSpeed, unsigned int currentTime);
	bool DoIBlockDrDriver(unsigned int vPos, unsigned int hPos);
	void PrintOnLcd(char line[2][17]);

private:

};

class roadCar: public roadObject
{
public:
	roadCar(int vPosition, float speed, unsigned int timeAppear);

	unsigned int GetObjType(void){return ROAD_CAR;}
	bool UpdatePosition(float extSpeed, unsigned int currentTime);
	bool DoIBlockDrDriver(unsigned int vPos, unsigned int hPos);
	void PrintOnLcd(char line[2][17]);
	float GetSpeed(void) {return speed;}

private:
};
#endif /* ROADOBJECT_HPP_ */
