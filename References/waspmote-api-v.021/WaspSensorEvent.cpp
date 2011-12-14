/*
 *  Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:		0.8
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
 

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspSensorEvent::WaspSensorEvent()
{
	pinMode(DIGITAL0,OUTPUT);
	pinMode(DIGITAL1,INPUT);
	pinMode(DIGITAL2,OUTPUT);
	pinMode(DIGITAL3,OUTPUT);
	pinMode(DIGITAL4,OUTPUT);
	pinMode(DIGITAL5,INPUT);
	pinMode(DIGITAL6,OUTPUT);
	pinMode(DIGITAL7,OUTPUT);
	pinMode(DIGITAL8,OUTPUT);
	pinMode(DIGITAL9,OUTPUT);
	pinMode(SENS_PW_3V3,OUTPUT);
		
	digitalWrite(DIGITAL0,LOW);
	digitalWrite(DIGITAL2,LOW);
	digitalWrite(DIGITAL3,LOW);
	digitalWrite(DIGITAL4,LOW);
	digitalWrite(DIGITAL6,LOW);
	digitalWrite(DIGITAL7,LOW);
	digitalWrite(DIGITAL8,LOW);
	digitalWrite(DIGITAL9,LOW);
	digitalWrite(SENS_PW_3V3,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorEvent::setBoardMode(uint8_t mode)
{
	switch( mode )
	{
		case	SENS_ON :	digitalWrite(SENS_PW_3V3,HIGH);
					// Sets RTC on to enable I2C
					if(!RTC.isON) RTC.setMode(RTC_ON, RTC_I2C_MODE);
					break;
		case	SENS_OFF:	digitalWrite(SENS_PW_3V3,LOW);
					break;
	}
}


/* setThreshold(sensor, threshold) - set threshold configuring digipots
 *
 * It sets threshold configuring digipots
 */
void	WaspSensorEvent::setThreshold(uint8_t sensor, float threshold) 
{
	switch( sensor )
	{
		case	SENS_SOCKET1	:	setDigipot1(B0101000,threshold);
						break;
		case	SENS_SOCKET2	:	setDigipot0(B0101000,threshold);
						break;
		case	SENS_SOCKET3	:	setDigipot0(B0101100,threshold);
						break;
		case	SENS_SOCKET4	:	setDigipot1(B0101100,threshold);
						break;
		case	SENS_SOCKET5	:	setDigipot0(B0101010,threshold);
						break;
		case	SENS_SOCKET6	:	setDigipot1(B0101010,threshold);
						break;
	}
}


float	WaspSensorEvent::readValue(uint8_t sensor)
{
	uint16_t aux=0;
	switch( sensor )
	{
		case	SENS_SOCKET1	:	aux=analogRead(ANALOG1);
						break;
		case	SENS_SOCKET2	:	aux=analogRead(ANALOG2);
						break;
		case	SENS_SOCKET3	:	aux=analogRead(ANALOG4);
						break;
		case	SENS_SOCKET4	:	aux=analogRead(ANALOG3);
						break;
		case	SENS_SOCKET5	:	aux=analogRead(ANALOG6);
						break;
		case	SENS_SOCKET6	:	aux=analogRead(ANALOG7);
						break;
		case	SENS_SOCKET7	:	aux=digitalRead(DIGITAL5);
						return aux;
						break;
		case	SENS_SOCKET8	:	aux=analogRead(ANALOG5);
						break;
	}
	return	(aux*3.3)/1023;
}


/* attacInt() - attach interruption
 *
 * It attaches sensors board interruption, setting three-state
 */
void	WaspSensorEvent::attachInt() 
{
	digitalWrite(SENS_INT_ENABLE,HIGH); 
	enableInterrupts(SENS_INT);
}


/* detachInt() - detaches interruptions
 *
 * It detaches sensor board interruption, unsetting three-state
 */
void	WaspSensorEvent::detachInt() 
{
	digitalWrite(SENS_INT_ENABLE,LOW); 
	disableInterrupts(SENS_INT);
}


/* loadInt() - loads parallel register to check the sensor that generates the interruption
 *
 * It loads parallel register to check the sensor that generates the interruption.
 * 
 * It updates 'intFlag' to show the sensor that has generated the interruption
 */
uint8_t	WaspSensorEvent::loadInt() 
{
	uint8_t a=0;
	uint8_t aux=0;
	
	intFlag=0;
	
	delay(1);
	digitalWrite(SENS_INT_ENABLE, LOW);  
	delay(10);
	digitalWrite(SENS_INT_CLK_INH, LOW);
	delay(1);

	if(digitalRead(SENS_INT_DO)) intFlag |= 1;
	delay(2);

	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 2;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 4;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 8;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 16;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 32;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 64;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_DO)) intFlag |= 128;
	delay(1);
	digitalWrite(SENS_INT_CLK_REG, LOW);
	delay(2);
	
	delay(200);
	digitalWrite(SENS_INT_ENABLE, HIGH);
	digitalWrite(SENS_INT_CLK_INH, HIGH);
	
	return intFlag;
}

// Private Methods //////////////////////////////////////////////////////////////

void WaspSensorEvent::setDigipot1(uint8_t address, float value)
{

	float thres=0.0;
	uint8_t threshold=0;
		
	thres=(3.3-value);
	thres *=128;
	thres /=3.3;
	threshold = (uint8_t) thres;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.send(B00010000);
	Wire.send(threshold);
	Wire.endTransmission();
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

void WaspSensorEvent::setDigipot0(uint8_t address, float value)
{
	float thres=0.0;
	uint8_t threshold=0;
		
	thres=(3.3-value);
	thres *=128;
	thres /=3.3;
	threshold = (uint8_t) thres;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.send(B00000000);
	Wire.send(threshold);
	Wire.endTransmission();
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

WaspSensorEvent SensorEvent=WaspSensorEvent();