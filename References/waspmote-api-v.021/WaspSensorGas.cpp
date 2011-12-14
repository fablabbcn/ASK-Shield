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
 *  Version:		0.9
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, Manuel Calahorra
 */
 

#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspSensorGas::WaspSensorGas()
{
	pinMode(DIGITAL7,OUTPUT);
	pinMode(DIGITAL6,OUTPUT);
	pinMode(DIGITAL5,OUTPUT);
	pinMode(DIGITAL2,OUTPUT);
	pinMode(DIGITAL4,OUTPUT);
	pinMode(DIGITAL3,OUTPUT);
	pinMode(DIGITAL1,OUTPUT);	
	pinMode(SENS_PW_3V3,OUTPUT);
	pinMode(SENS_PW_5V,OUTPUT);
	
	digitalWrite(DIGITAL7,LOW);
	digitalWrite(DIGITAL6,LOW);
	digitalWrite(DIGITAL5,LOW);
	digitalWrite(DIGITAL2,LOW);
	digitalWrite(DIGITAL4,LOW);
	digitalWrite(DIGITAL3,LOW);
	digitalWrite(DIGITAL1,LOW);
	digitalWrite(SENS_PW_3V3,LOW);
	digitalWrite(SENS_PW_5V,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorGas::setBoardMode(uint8_t mode)
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

void	WaspSensorGas::configureSensor(uint16_t sensor, uint8_t gain)
{
	configureSensor(sensor,gain,0);
}

void	WaspSensorGas::configureSensor(uint16_t sensor, uint8_t gain, float resistor)
{
	switch( sensor )
	{
		case	SENS_CO2	:	configureAmplifier(SENS_AMPLI1,gain);
						break;
		case	SENS_O2		:	configureAmplifier(SENS_AMPLI1,gain);
						break;
		case	SENS_SOCKET2A	:	configureResistor(SENS_R1,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI2,gain);
						break;
		case	SENS_SOCKET2B	:	configureResistor(SENS_R1,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI2,gain);
						break;
		case	SENS_SOCKET3A	:	configureResistor(SENS_R2,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI3,gain);
						break;
		case	SENS_SOCKET3B	:	configureResistor(SENS_R2,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI3,gain);
						break;
		case	SENS_SOCKET3C	:	configureResistor(SENS_R2,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI3,gain);
						break;
		case	SENS_SOCKET4A	:	configureResistor(SENS_R3,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI4,gain);
						break;
		case	SENS_SOCKET4B	:	configureResistor(SENS_R3,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI4,gain);
						break;
		case	SENS_SOCKET4C	:	configureResistor(SENS_R3,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI4,gain);
						break;
	}
}

void	WaspSensorGas::setSensorMode(uint8_t mode, uint16_t sensor)
{
	if( mode==SENS_ON )
	{
		switch( sensor )
		{
			case	SENS_PRESSURE	:	digitalWrite(DIGITAL8,HIGH);
							break;
			case	SENS_CO2	:	digitalWrite(DIGITAL2,HIGH);
							break;
			case	SENS_O2		:	digitalWrite(DIGITAL2,LOW);
							break;
			case	SENS_SOCKET2A	:	digitalWrite(DIGITAL6,LOW);
							digitalWrite(DIGITAL4,HIGH);
							break;
			case	SENS_SOCKET2B:		digitalWrite(DIGITAL4,LOW);
							digitalWrite(DIGITAL6,HIGH);
							break;
			case	SENS_SOCKET3A	:	digitalWrite(DIGITAL1,HIGH);
							digitalWrite(DIGITAL7,HIGH);
							break;
			case	SENS_SOCKET4A	:	digitalWrite(DIGITAL3,HIGH);
							digitalWrite(DIGITAL5,HIGH);
							break;
		}
	}
	
	if( mode==SENS_OFF )
	{
		switch( sensor )
		{
			case	SENS_PRESSURE	:	digitalWrite(DIGITAL8,LOW);
							break;
			case	SENS_CO2	:	digitalWrite(DIGITAL2,LOW);
							break;
			case	SENS_SOCKET2A	:	digitalWrite(DIGITAL4,LOW);
							break;
			case	SENS_SOCKET2B:		digitalWrite(DIGITAL6,LOW);
							break;
			case	SENS_SOCKET3A	:	digitalWrite(DIGITAL1,LOW);
							digitalWrite(DIGITAL7,LOW);
							break;
			case	SENS_SOCKET4A	:	digitalWrite(DIGITAL3,LOW);
							digitalWrite(DIGITAL5,LOW);
							break;
		}
	}
}

float	WaspSensorGas::readValue(uint16_t sensor)
{
	uint16_t aux=0;
	switch( sensor )
	{
		case	SENS_TEMPERATURE	:	aux=analogRead(ANALOG1);
							break;
		case	SENS_HUMIDITY		:	aux=analogRead(ANALOG4);
							break;
		case	SENS_PRESSURE		:	aux=analogRead(ANALOG5);
							break;
		case	SENS_CO2		:	aux=analogRead(ANALOG3);
							break;
		case	SENS_O2			:	aux=analogRead(ANALOG3);
							break;
		case	SENS_SOCKET2A		:	aux=analogRead(ANALOG2);
							break;
		case	SENS_SOCKET2B		:	aux=analogRead(ANALOG2);
							break;
		case	SENS_SOCKET3A		:	aux=analogRead(ANALOG7);
							break;
		case	SENS_SOCKET3B		:	aux=pulse(SENS_SOCKET3B);
							break;
		case	SENS_SOCKET3C		:	aux=pulse(SENS_SOCKET3C);
							break;
		case	SENS_SOCKET4A		:	aux=analogRead(ANALOG6);
							break;
		case	SENS_SOCKET4B		:	aux=pulse(SENS_SOCKET4B);
							break;
		case	SENS_SOCKET4C		:	aux=pulse(SENS_SOCKET4C);
							break;
	}
	return	(aux*3.3)/1023;
}

// Private Methods //////////////////////////////////////////////////////////////

void WaspSensorGas::configureResistor(uint8_t ampli, float resistor)
{
	switch( ampli )
	{
		case	SENS_R1	:	setResistor(B0101100,resistor);
					break;
		case	SENS_R2	:	setResistor(B0101110,resistor);
					break;
		case	SENS_R3	:	setResistor(B0101010,resistor);
					break;
	}
}

void WaspSensorGas::setResistor(uint8_t address, float value)
{
	float auxiliar = 0;
	uint8_t resist=0;
	
	auxiliar = 128*value;
	auxiliar = auxiliar/100;
	resist = (uint8_t) 128-auxiliar;

	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.send(B00000000);
	Wire.send(resist);
	Wire.endTransmission();
	delay(DELAY_TIME);
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

void WaspSensorGas::configureAmplifier(uint8_t ampli, uint8_t gain)
{
	switch( ampli )
	{
		case	SENS_AMPLI1	:	setAmplifier(B0101000,gain);
						break;
		case	SENS_AMPLI2	:	setAmplifier(B0101100,gain);
						break;
		case	SENS_AMPLI3	:	setAmplifier(B0101110,gain);
						break;
		case	SENS_AMPLI4	:	setAmplifier(B0101010,gain);
						break;
	}
}

void WaspSensorGas::setAmplifier(uint8_t address, uint8_t value)
{
	uint8_t ampli=0;
	value--;

	ampli=(uint8_t) 128-(128/100)*value;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.send(B00010000);
	Wire.send(ampli);
	Wire.endTransmission();
	delay(DELAY_TIME);
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

uint16_t WaspSensorGas::pulse(uint16_t sensor)
{
	uint16_t aux=0;
	
	switch( sensor )
	{
		case	SENS_SOCKET3B	:	digitalWrite(DIGITAL7, HIGH); 
  						delay(14);
						digitalWrite(DIGITAL7, LOW);
						delay(980);
						digitalWrite(DIGITAL1, HIGH);
						delay(3); 
						aux = analogRead(ANALOG7);
						delay(2);
						digitalWrite(DIGITAL1, LOW);
						break;
		case	SENS_SOCKET3C	:	digitalWrite(DIGITAL7, HIGH); 
  						delay(2);
						digitalWrite(DIGITAL1, HIGH);
						delay(4);
						aux = analogRead(ANALOG7);
						delay(1);
						digitalWrite(DIGITAL1, LOW);
						delay(7); 
						digitalWrite(DIGITAL7, LOW);
						delay(236);
						break;
		case	SENS_SOCKET4B	:	digitalWrite(DIGITAL5, HIGH); 
  						delay(14);
						digitalWrite(DIGITAL5, LOW);
						delay(980);
						digitalWrite(DIGITAL3, HIGH);
						delay(3); 
						aux = analogRead(ANALOG6);
						delay(2);
						digitalWrite(DIGITAL3, LOW);
						break;
		case	SENS_SOCKET4C	:	digitalWrite(DIGITAL5, HIGH); 
  						delay(2);
						digitalWrite(DIGITAL3, HIGH);
						delay(4);
						aux = analogRead(ANALOG6);
						delay(1);
						digitalWrite(DIGITAL3, LOW);
						delay(7); 
						digitalWrite(DIGITAL5, LOW);
						delay(236);
						break;
	}
	return aux;
}

WaspSensorGas SensorGas=WaspSensorGas();