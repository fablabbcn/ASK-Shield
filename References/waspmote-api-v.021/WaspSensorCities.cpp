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
//  #include <WProgram.h>
  #include <WaspClasses.h>
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspSensorCities::WaspSensorCities()
{
	pinMode(DIGITAL1,OUTPUT);
	pinMode(DIGITAL2,OUTPUT);
	pinMode(DIGITAL3,INPUT);
	pinMode(DIGITAL4,OUTPUT);
	pinMode(DIGITAL5,OUTPUT);
	pinMode(DIGITAL6,OUTPUT);
	pinMode(DIGITAL7,OUTPUT);
	pinMode(DIGITAL8,OUTPUT);
	pinMode(14,INPUT);
	pinMode(15,OUTPUT);
	pinMode(16,INPUT);
	pinMode(17,INPUT);
	pinMode(18,INPUT);
	pinMode(19,INPUT);
	pinMode(20,INPUT);
	pinMode(SENS_PW_3V3,OUTPUT);
	pinMode(SENS_PW_5V,OUTPUT);

	digitalWrite(DIGITAL1,LOW);
	digitalWrite(DIGITAL2,LOW);
	digitalWrite(DIGITAL4,LOW);
	digitalWrite(DIGITAL5,LOW);
	digitalWrite(DIGITAL6,LOW);
	digitalWrite(DIGITAL7,LOW);
	digitalWrite(DIGITAL8,LOW);
	digitalWrite(15,LOW);
	digitalWrite(SENS_PW_3V3,LOW);
	digitalWrite(SENS_PW_5V,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorCities::setBoardMode(uint8_t mode)
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


void WaspSensorCities::setAudioGain(uint8_t value1, float value2)
{
	uint8_t ampli=0;
	float auxiliar = 0.0;

	gain1 = value1;

	gain2 = value2;

	if(value1 > 101)
		value1 = 101;
	else if(value1 < 1)
		value1 = 1;

	value1--;
	ampli=(uint8_t) 128-(128/100)*value1;

	if( !Wire.I2C_ON ) Wire.begin();
	delay(100);
	Wire.beginTransmission(B0101110);
	Wire.send(B00000000);
	Wire.send(ampli);
	Wire.endTransmission();
	delay(DELAY_TIME);

	if(value2 > 5.0)
		value2 = 5.0;
	else if(value2 < 1.056)
		value2 = 1.056;

	auxiliar = 5.6 / (value2-1.0);

	ampli=(uint8_t) 128-(128/100)*auxiliar;

	Wire.beginTransmission(B0101110);
	Wire.send(B00010000);
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
void	WaspSensorCities::setThreshold(uint16_t sensor, float threshold) 
{
	switch( sensor )
	{
		case	SENS_CITIES_DUST	:	setDigipot0(B0101010,threshold);
							break;
		case	SENS_CITIES_LD		:	setDigipot1(B0101000,threshold);
							break;
		case	SENS_CITIES_AUDIO	:	setDigipot1(B0101010,threshold);
							break;
		case	SENS_CITIES_HUMIDITY	:	setDigipot0(B0101111,threshold);
							break;
		case	SENS_CITIES_TEMPERATURE	:	setDigipot1(B0101100,threshold);
							break;
		case	SENS_CITIES_ULTRASOUND	:	setDigipot1(B0101100,threshold);
							break;
		case	SENS_CITIES_LDR		:	setDigipot0(B0101100,threshold);
							break;
		case	SENS_CITIES_CD		:	setDigipot0(B0101100,threshold);
							break;
		case	SENS_CITIES_CP		:	setDigipot0(B0101100,threshold);
							break;
		default:;
	}
}


void	WaspSensorCities::setSensorMode(uint8_t mode, uint16_t sensor)
{
	if( mode==SENS_ON )
	{
		switch( sensor )
		{
			case	SENS_CITIES_DUST	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_LD		:	digitalWrite(DIGITAL1,HIGH);
								break;
			case	SENS_CITIES_AUDIO	:	digitalWrite(DIGITAL6,HIGH);
								break;
			case	SENS_CITIES_HUMIDITY	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_TEMPERATURE	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_ULTRASOUND	:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_LDR		:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_CD		:	digitalWrite(DIGITAL2,HIGH);
								break;
			case	SENS_CITIES_CP		:	digitalWrite(DIGITAL2,HIGH);
								break;
			default:;
		}
	}
	
	if( mode==SENS_OFF )
	{
		switch( sensor )
		{
			case	SENS_CITIES_DUST	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_LD		:	digitalWrite(DIGITAL1,LOW);
								break;
			case	SENS_CITIES_AUDIO	:	digitalWrite(DIGITAL6,LOW);
								break;
			case	SENS_CITIES_HUMIDITY	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_TEMPERATURE	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_ULTRASOUND	:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_LDR		:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_CD		:	digitalWrite(DIGITAL2,LOW);
								break;
			case	SENS_CITIES_CP		:	digitalWrite(DIGITAL2,LOW);
								break;
			default:;
		}
	}
}


float	WaspSensorCities::readValue(uint16_t sensor)
{
	int aux=0;
	float value = 0.0;
	
	switch( sensor )
	{
		case	SENS_CITIES_DUST	:	aux = analogRead(ANALOG1);
							value = dust_conversion(aux);
							break;
		case	SENS_CITIES_LD		:	aux = analogRead(ANALOG7);
							value = ld_conversion(aux);
							break;
		case	SENS_CITIES_AUDIO	:	aux = analogRead(ANALOG6);
							value = audio_conversion(aux);
							break;
		case	SENS_CITIES_HUMIDITY	:	aux = analogRead(ANALOG3);
							value = humidity_conversion(aux);
							break;
		case	SENS_CITIES_TEMPERATURE	:	aux = analogRead(ANALOG4);
							value = temperature_conversion(aux);
							break;
		case	SENS_CITIES_ULTRASOUND	:	aux = analogRead(ANALOG4);
							value = ultrasound_conversion(aux);
							break;
		case	SENS_CITIES_LDR		:	aux = analogRead(ANALOG5);
							value = ldr_conversion(aux);
							break;
		case	SENS_CITIES_CD		:	aux = analogRead(ANALOG5);
							value = aux;
							break;
		case	SENS_CITIES_CP		:	aux = analogRead(ANALOG5);
							value = aux;
							break;
		default:;
	}
	
	return value;
}


/* attacInt() - attach interruption
 *
 * It attaches sensors board interruption, setting three-state
 */
void	WaspSensorCities::attachInt() 
{
	digitalWrite(SENS_INT_CITIES_ENABLE,HIGH); 
	enableInterrupts(SENS_INT);
}


/* detachInt() - detaches interruptions
 *
 * It detaches sensor board interruption, unsetting three-state
 */
void	WaspSensorCities::detachInt() 
{
	digitalWrite(SENS_INT_CITIES_ENABLE,LOW); 
	disableInterrupts(SENS_INT);
}


/* loadInt() - loads parallel register to check the sensor that generates the interruption
 *
 * It loads parallel register to check the sensor that generates the interruption.
 * 
 * It updates 'intFlag' to show the sensor that has generated the interruption
 */
uint8_t	WaspSensorCities::loadInt() 
{
	uint8_t a=0;
	uint8_t aux=0;
	
	intFlag=0;
	
	delay(1);
	digitalWrite(SENS_INT_CITIES_ENABLE, LOW);
	delay(10);
	digitalWrite(SENS_INT_CITIES_CLK_INH, LOW);
	delay(1);

	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 1;
	delay(2);

	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 2;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 4;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 8;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 16;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 32;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);
      
	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 64;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);

	digitalWrite(SENS_INT_CITIES_CLK_REG, HIGH);
	delay(1);
	if(digitalRead(SENS_INT_CITIES_DO)) intFlag |= 128;
	delay(1);
	digitalWrite(SENS_INT_CITIES_CLK_REG, LOW);
	delay(2);
	
	delay(200);
	digitalWrite(SENS_INT_CITIES_ENABLE, HIGH);
	digitalWrite(SENS_INT_CITIES_CLK_INH, HIGH);
	
	return intFlag;
}

// Private Methods //////////////////////////////////////////////////////////////

void WaspSensorCities::setDigipot1(uint8_t address, float value)
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

void WaspSensorCities::setDigipot0(uint8_t address, float value)
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


 float WaspSensorCities::dust_conversion(int readValue)
{
	float dust = 0;
   
  	dust = ((float) readValue/310-0.0)/5;
   
	return(dust);
}

 float WaspSensorCities::ld_conversion(int readValue)
{
	float ld = 0;
   
	ld = float(readValue) * 11 / (1023);
      
	return(ld);
}

 float WaspSensorCities::audio_conversion(int readValue)
{
	float audio = 0;
   
	audio = float(readValue-19) * 3300 / 1023;

	audio = audio/((float)gain1 * (float)gain2);

	audio = audio / 44.457;

	audio = 20 * log10(audio/0.00002);
   
	return(audio);
}

 float WaspSensorCities::ldr_conversion(int readValue)
{
	float ldr = 0;
   
	ldr = float(readValue) * 3.3 / 1023;
      
	return(ldr);
}

 float WaspSensorCities::temperature_conversion(int readValue)
{
	float temperature = 0;
   
	temperature = float(readValue) * 3300 / 1023;
   
	temperature = (temperature - 500) / 10;
   
	return(temperature);   
}

 float WaspSensorCities::humidity_conversion(int readValue)
{
	float humidity = 0;
   
	humidity = float(readValue) * 5000 / 1023;
   
	humidity = (humidity - 800) / 31;
   
	return(humidity);
   
}

 float WaspSensorCities::ultrasound_conversion(int readValue)
{
	float distance = 0;
   
	distance = float(readValue) * 3300 / 1023;

	if(SENS_ULTRASOUND_TYPE)
	{
		distance = distance / 250;
	}else
	{
		distance = distance / 320;
	}
      
	return(distance);
   
}


WaspSensorCities SensorCities=WaspSensorCities();