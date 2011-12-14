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

WaspSensorSmart::WaspSensorSmart()
{
	pinMode(DIGITAL1,OUTPUT);
	pinMode(DIGITAL2,OUTPUT);
	pinMode(DIGITAL3,OUTPUT);
	pinMode(DIGITAL4,OUTPUT);
	pinMode(DIGITAL5,OUTPUT);
	pinMode(DIGITAL6,OUTPUT);
	pinMode(DIGITAL7,OUTPUT);
	pinMode(DIGITAL8,OUTPUT);
	pinMode(14,OUTPUT);
	pinMode(15,OUTPUT);
	pinMode(16,OUTPUT);
	pinMode(17,OUTPUT);
	pinMode(18,INPUT);
	pinMode(19,OUTPUT);
	pinMode(20,OUTPUT);
	pinMode(SENS_PW_3V3,OUTPUT);
	pinMode(SENS_PW_5V,OUTPUT);

	digitalWrite(DIGITAL1,HIGH);
	digitalWrite(DIGITAL2,LOW);
	digitalWrite(DIGITAL3,LOW);
	digitalWrite(DIGITAL4,LOW);
	digitalWrite(DIGITAL5,HIGH);
	digitalWrite(DIGITAL6,LOW);
	digitalWrite(DIGITAL7,LOW);
	digitalWrite(DIGITAL8,HIGH);
	digitalWrite(14,LOW);
	digitalWrite(15,HIGH);
	digitalWrite(16,HIGH);
	digitalWrite(17,LOW);
	digitalWrite(19,LOW);
	digitalWrite(20,LOW);
	digitalWrite(SENS_PW_3V3,LOW);
	digitalWrite(SENS_PW_5V,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorSmart::setBoardMode(uint8_t mode)
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


void WaspSensorSmart::setLoadResistor(float value)
{
	float auxiliar = 0;
	uint8_t resist=0;
	
	auxiliar = 128*value;
	auxiliar = auxiliar/10000;
	resist = (uint8_t) 128-auxiliar;

	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(B0101110);
	Wire.send(B00010000);
	Wire.send(resist);
	Wire.endTransmission();
	delay(DELAY_TIME);
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}


void WaspSensorSmart::setAmplifierGain(uint16_t value)
{
	uint8_t ampli=0;
	float auxiliar = 0.0;
	float auxiliar2= 0.0;
	
	auxiliar = (float) (10000)/(value-1);
	auxiliar = auxiliar*128;
	auxiliar = auxiliar/10000;
	ampli=128-(uint8_t)auxiliar;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(B0101110);
	Wire.send(B00000000);
	Wire.send(ampli);
	Wire.endTransmission();
	delay(DELAY_TIME);
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}


/* setThreshold(sensor, threshold) - set threshold configuring digipots
 *
 * It sets threshold configuring digipots
 */
void	WaspSensorSmart::setThreshold(uint16_t sensor, float threshold) 
{
	switch( sensor )
	{
		case	SENS_SMART_LCELLS	:	setDigipot1(B0101000,threshold);
							break;
		case	SENS_SMART_EFERGY	:	setDigipot0(B0101000,threshold);
							break;
		case	SENS_SMART_LDR		:	setDigipot0(B0101100,threshold);
							break;
		case	SENS_SMART_4mA20_2	:	setDigipot1(B0101100,threshold);
							break;
		case	SENS_SMART_FV_CONVERTER	:	setDigipot0(B0101010,threshold);
							break;
	}
}


void	WaspSensorSmart::setSensorMode(uint8_t mode, uint16_t sensor)
{
	if( mode==SENS_ON )
	{
		switch( sensor )
		{
			case	SENS_SMART_4mA20_2	:	digitalWrite(DIGITAL8,LOW);
								digitalWrite(DIGITAL1,LOW);
								break;
			case	SENS_SMART_LDR		:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_LIQUID1	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_5V_2		:	digitalWrite(DIGITAL4,HIGH);
								break;
			case	SENS_SMART_LCELLS	:	digitalWrite(14,HIGH);
								if(SENS_SMART_LCELL_IN_USE==0) digitalWrite(16,LOW);
								if(SENS_SMART_LCELL_IN_USE==1) digitalWrite(15,LOW);
								digitalWrite(DIGITAL1,LOW);
								break;
			case	SENS_SMART_EFERGY	:	digitalWrite(14,HIGH);
								break;
			case	SENS_SMART_LIQUID2	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_LIQUID3	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_TEMPERATURE	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_HUMIDITY	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_SMART_4mA20_1	:	digitalWrite(DIGITAL5,LOW);
								digitalWrite(DIGITAL1,LOW);
								break;
			case	SENS_SMART_3V3_1	:	digitalWrite(DIGITAL3,HIGH);
								break;
			case	SENS_SMART_3V3_2	:	digitalWrite(DIGITAL6,HIGH);
								break;
			case	SENS_SMART_5V_1		:	digitalWrite(DIGITAL7,HIGH);
								break;
			case	SENS_SMART_24V_CONVERTER:	digitalWrite(DIGITAL1,LOW);
								break;					
		}
	}
	
	if( mode==SENS_OFF )
	{
		switch( sensor )
		{
			case	SENS_SMART_4mA20_2	:	digitalWrite(DIGITAL8,HIGH);
								digitalWrite(DIGITAL1,HIGH);
								break;
			case	SENS_SMART_LDR		:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_LIQUID1	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_5V_2		:	digitalWrite(DIGITAL4,LOW);
								break;
			case	SENS_SMART_LCELLS	:	digitalWrite(14,LOW);
								if(SENS_SMART_LCELL_IN_USE==0) digitalWrite(16,HIGH);
								if(SENS_SMART_LCELL_IN_USE==1) digitalWrite(15,HIGH);
								digitalWrite(DIGITAL1,HIGH);
								break;
			case	SENS_SMART_EFERGY	:	digitalWrite(14,LOW);
								break;
			case	SENS_SMART_LIQUID2	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_LIQUID3	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_TEMPERATURE	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_HUMIDITY	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_SMART_4mA20_1	:	digitalWrite(DIGITAL5,HIGH);
								digitalWrite(DIGITAL1,HIGH);
								break;
			case	SENS_SMART_3V3_1	:	digitalWrite(DIGITAL3,LOW);
								break;
			case	SENS_SMART_3V3_2	:	digitalWrite(DIGITAL6,LOW);
								break;
			case	SENS_SMART_5V_1		:	digitalWrite(DIGITAL7,LOW);
								break;
			case	SENS_SMART_24V_CONVERTER:	digitalWrite(DIGITAL1,HIGH);
								break;	
		}
	}
}


float	WaspSensorSmart::readValue(uint16_t sensor)
{
	float aux=0;
	
	configureI2C(sensor);
	aux=readValue_int();
	if( sensor==SENS_SMART_5V_2 || sensor==SENS_SMART_HUMIDITY || sensor==SENS_SMART_5V_1 ) aux*=2;
	delay(500);
	
	return aux;
}


/* attacInt() - attach interruption
 *
 * It attaches sensors board interruption, setting three-state
 */
void	WaspSensorSmart::attachInt() 
{
	digitalWrite(SENS_INT_SMART_ENABLE,HIGH); 
	enableInterrupts(SENS_INT);
}


/* detachInt() - detaches interruptions
 *
 * It detaches sensor board interruption, unsetting three-state
 */
void	WaspSensorSmart::detachInt() 
{
	digitalWrite(SENS_INT_SMART_ENABLE,LOW); 
	disableInterrupts(SENS_INT);
}


/* loadInt() - loads parallel register to check the sensor that generates the interruption
 *
 * It loads parallel register to check the sensor that generates the interruption.
 * 
 * It updates 'intFlag' to show the sensor that has generated the interruption
 */
uint8_t	WaspSensorSmart::loadInt() 
{
	intFlag=0;
	
	delay(1);
	digitalWrite(SENS_INT_SMART_ENABLE, LOW);  
	delay(10);
	digitalWrite(SENS_INT_SMART_CLK_INH, LOW);
	delay(1);

	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 1;
	delay(2);

	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 2;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 4;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 8;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 16;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 32;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 64;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_SMART_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_SMART_DO)) intFlag |= 128;
	delay(1);
	digitalWrite(SENS_INT_SMART_CLK_REG, LOW);
	delay(2);
	
	delay(200);
	digitalWrite(SENS_INT_SMART_ENABLE, HIGH);
	digitalWrite(SENS_INT_SMART_CLK_INH, HIGH);
	
	return intFlag;
}

// Private Methods //////////////////////////////////////////////////////////////

void WaspSensorSmart::setDigipot1(uint8_t address, float value)
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

void WaspSensorSmart::setDigipot0(uint8_t address, float value)
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


void WaspSensorSmart::configureI2C(uint16_t sensor)
{
	if( !Wire.I2C_ON ) Wire.begin();
	Wire.beginTransmission(B0010100);
	switch( sensor )
	{
		case	SENS_SMART_4mA20_2	:	Wire.send(B10111111);
							break;
		case	SENS_SMART_LDR		:	Wire.send(B10111011);
							break;
		case	SENS_SMART_LIQUID1	:	Wire.send(B10110001);
							break;
		case	SENS_SMART_5V_2		:	Wire.send(B10111110);
							break;
		case	SENS_SMART_LCELLS	:	Wire.send(B10111000);
							break;
		case	SENS_SMART_EFERGY	:	Wire.send(B10110000);
							break;
		case	SENS_SMART_LIQUID2	:	Wire.send(B10111001);
							break;
		case	SENS_SMART_LIQUID3	:	Wire.send(B10110010);
							break;
		case	SENS_SMART_TEMPERATURE	:	Wire.send(B10110011);
							break;
		case	SENS_SMART_HUMIDITY	:	Wire.send(B10111010);
							break;
		case	SENS_SMART_4mA20_1	:	Wire.send(B10110110);
							break;
		case	SENS_SMART_3V3_1	:	Wire.send(B10111101);
							break;
		case	SENS_SMART_3V3_2	:	Wire.send(B10110111);
							break;
		case	SENS_SMART_5V_1		:	Wire.send(B10110100);
							break;
		case	SENS_SMART_FV_CONVERTER :	Wire.send(B10110101);
							break;
							
	}
	Wire.endTransmission(); 
}


float WaspSensorSmart::readValue_int()
{
	byte valor[3];
	
	delay(400);
	
	Wire.requestFrom(B0010100, 3);

	int i = 0;
	while(Wire.available() && i<3){
		valor[i] = Wire.receive();
		i++;
	}
	delay(100);
	if( Wire.I2C_ON && !ACC.isON && RTC.isON!=1){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
	return conversion(valor);
}


float WaspSensorSmart::conversion(byte data_input[3])
{
	long val = 0;
	float val_def = 0;
	int signo = 0;
	int overflow = 0;

	signo = int(data_input[0] >> 7);
	overflow = int((data_input[0] >> 6)&B00000001);

	val = long((data_input[2] >> 6)&B00000011) + long(data_input[1])*4 + long((data_input[0])&B00111111)*1024;

	if (signo == 1)
	{
		if (overflow == 1)
		{
     //OVERFLOW ALTO
			val_def = 2.25;
		}
		else
		{
			val_def = val*2.25;
			val_def = val_def/65535;
		}
	}
	else
	{
		if (overflow == 0)
		{
     //OVERFLOW BAJO
			val_def = -2.25;
		}
		else
		{
			val_def = -(65535 - val)*2.25;
			val_def = val_def/65535;
		}
	}

	return(val_def);
}



WaspSensorSmart SensorSmart=WaspSensorSmart();
