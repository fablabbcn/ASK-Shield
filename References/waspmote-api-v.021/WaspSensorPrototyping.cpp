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
 *  Implementation:	Alberto Bielsa, Manuel Calahorra
 */
 
#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspSensorPrototyping::WaspSensorPrototyping()
{
	pinMode(RELAY_PIN,OUTPUT);	
	pinMode(SENS_PW_3V3,OUTPUT);
	pinMode(SENS_PW_5V,OUTPUT);
	
	digitalWrite(RELAY_PIN,LOW);
	digitalWrite(SENS_PW_3V3,LOW);
	digitalWrite(SENS_PW_5V,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorPrototyping::setBoardMode(uint8_t mode)
{
	switch( mode )
	{
		case	SENS_ON :	digitalWrite(SENS_PW_3V3,HIGH);
					digitalWrite(SENS_PW_5V,HIGH);
					// Sets RTC on to enable I2C
					if(!RTC.isON) RTC.setMode(RTC_ON, RTC_I2C_MODE);
					break;
		case	SENS_OFF:	digitalWrite(SENS_PW_3V3,LOW);
					digitalWrite(SENS_PW_5V,LOW);
					break;
	}
}

void	WaspSensorPrototyping::setRelayMode(uint8_t mode)
{
	switch( mode )
	{
		case	SENS_RELAY_ON :		digitalWrite(RELAY_PIN,HIGH);
						break;
		case	SENS_RELAY_OFF:		digitalWrite(RELAY_PIN,LOW);
						break;
	}
}

float	WaspSensorPrototyping::readADC()
{
	uint8_t valor[2] = {0, 0};
	float val_def;
	uint16_t val;
	uint8_t i = 0;

	if( !Wire.I2C_ON ) Wire.begin();
	Wire.requestFrom(B0010100, 2);
	while(Wire.available()){
		valor[i] = Wire.receive();
		i++;
	}
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
	val = int(valor[0])*256 + int(valor[1]);
	val_def = (val - 32769)*9;
	val_def = val_def/65535;

	return val_def;
}

void	WaspSensorPrototyping::setLoadResistor(float resistor)
{
	uint8_t resist=0;
	
	resist=(uint8_t) 128-(128/100)*resistor;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(B0101000);
	Wire.send(B00000000);
	Wire.send(resist);
	Wire.endTransmission();
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

void	WaspSensorPrototyping::setAmplifierGain(float gain)
{
	uint8_t ampli=0;
	
	gain=(gain-1)*10;
	ampli=(uint8_t) 128-(128/100)*gain;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(B0101000);
	Wire.send(B00010000);
	Wire.send(ampli);
	Wire.endTransmission();
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

float WaspSensorPrototyping::readAnalogSensor(uint8_t pin)
{
	uint16_t read_val;
	float def_val;

	if ( (pin >= ANALOG1) && (pin <= ANALOG7) )
	{
		read_val = analogRead(pin);
		def_val = (read_val*3.3)/1023;
		return def_val;
	}
	else	return -1;
}


WaspSensorPrototyping SensorProto=WaspSensorPrototyping();