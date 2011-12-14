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
 *  Version:		1.1
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, David Cuartielles, Marcos Yarza
 */
 

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif


// Constructors ////////////////////////////////////////////////////////////////

WaspRTC::WaspRTC()
{
  // nothing to do when constructing
}

// Public Methods //////////////////////////////////////////////////////////////

/*
 * ON (void) - It opens I2C bus and powers the RTC
 *
 *  It opens I2C bus and powers the RTC
 */
void WaspRTC::ON(void)
{
	begin();
}


/*
 * OFF (void) - It closes I2C bus and powers off the RTC
 *
 *  It closes I2C bus and powers off the RTC
 */
void WaspRTC::OFF(void)
{
	close();
	setMode(RTC_OFF, RTC_NORMAL_MODE);
}


/* begin() - inits I2C bus and used pins
 *
 * It enables internal pull-up resistor for the RTC interrupt pin, so as this pin is set to HIGH when init
 * It inits I2C bus for communicating with RTC
 * It reads from RTC time,date and alarms, setting the corresponding variables
 *
 * Returns nothing
 */ 
void WaspRTC::begin()
{
  // Powers RTC UP
  setMode(RTC_ON, RTC_NORMAL_MODE);
  // Inits I2C bus
  if( !Wire.I2C_ON ) Wire.begin();

  // initialize the variables used to store the data
  // from the RTC
  clearAlarmFlag();
  resetVars();
  readRTC(RTC_ALARM2_ADDRESS);
}


/* close() - closes I2C bus and used pins
 *
 * It enables internal pull-up resistor for the RTC interrupt pin, so as this pin is set to HIGH when init
 * It inits I2C bus for communicating with RTC
 * It reads from RTC time,date and alarms, setting the corresponding variables
 *
 * Returns nothing
 */
void WaspRTC::close()
{
	if( Wire.I2C_ON && !ACC.isON) PWR.closeI2C();
}


/* setMode(mode) - set power mode
 *
 * It sets the power mode on the RTC. It has two possibilities: ON or OFF
 */
void WaspRTC::setMode(uint8_t mode, uint8_t I2C_mode)
{
	_pwrMode=mode;
	pinMode(RTC_PW,OUTPUT);
	switch(_pwrMode)
	{
		case RTC_ON 	: digitalWrite(RTC_PW,HIGH);
				  if( I2C_mode==RTC_I2C_MODE ) isON = 2;
				  else if( I2C_mode==RTC_NORMAL_MODE ) isON = 1;
			      	  break;
		case RTC_OFF 	: digitalWrite(RTC_PW,LOW);
				  isON = 0;
				  break;
	}
}


/* getMode() - gets power mode
 *
 * It gets the power mode of the RTC.
 */
 uint8_t WaspRTC::getMode()
{
	return _pwrMode;
}

/* resetVars() - resets variables to zero
 *
 * It resets all the used variables to default value
 */ 
void WaspRTC::resetVars()
{
    	year = 0;
    	month = 0;
    	day = 0;
    	hour = 0;
    	minute = 0;
    	second = 0;
    	date = 0;
	second_alarm1 = 0;
	minute_alarm1 = 0;
	hour_alarm1 = 0;
	day_alarm1 = 0;
	minute_alarm2 = 0;
	hour_alarm2 = 0;
	day_alarm2 = 0;
	alarm1Mode = 0;
	alarm2Mode = 0;
	temp=0;
	tempNegative=false;
}


/* getRTCarray() - gets the last array taken from the RTC
 *
 * It gets the 'registersRTC' variable which contains the last values given by the RTC
 *
 * It returns a string containing this array
 */
char* WaspRTC::getRTCarray() 
{
  char aux[60];
  for(uint8_t i = 0; i < RTC_DATA_SIZE; i++) {
    sprintf(aux, "%u", registersRTC[i]);
  }
  return aux;
}


/* getTimestamp() - returns a string containing variables related with time and date
 *
 * It returns a string containing variables related with time and date. These values are the last taken from RTC
 */
char* WaspRTC::getTimestamp() 
{
  free(timeStamp);
  timeStamp=NULL;
  timeStamp=(char*)calloc(31,sizeof(char));
  switch (day) {
	case 1:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_1, year, month, date, hour, minute, second);
			break;
	case 2:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_2, year, month, date, hour, minute, second);
			break;
	case 3:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_3, year, month, date, hour, minute, second);
			break;
	case 4:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_4, year, month, date, hour, minute, second);
			break;
	case 5:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_5, year, month, date, hour, minute, second);
			break;
	case 6:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_6, year, month, date, hour, minute, second);
			break;
	case 7:		sprintf (timeStamp, "%s, %02d/%02d/%02d - %02d:%02d.%02d", DAY_7, year, month, date, hour, minute, second);
			break;
  }
  return timeStamp;
}


/* readRTC(endAddress) - reads from RTC the specified addresses
 *
 * It reads from RTC the specified addresses by 'endAddress'. This input specifies the last address wants to be read.
 *
 * 'endAddress' may be any address, but it is recommended to use one of the following:
 *
 * --> RTC_DATE_ADDRESS: to read time and date
 * --> RTC_ALARM1_ADDRESS: to read time, date and alarm1
 * --> RTC_ALARM2_ADDRESS: to read time, date, alarm1 and alarm2
 *
 * It stores in corresponding variables the read values up to alarm2 values and stores then in 'registersRTC' array too.
 */
void WaspRTC::readRTC(uint8_t endAddress) 
{
  uint16_t timecount = 0;
  // ADDRESSING FROM MEMORY POSITION ZERO
  Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x68)
  // the address specified in the datasheet is 208 (0xD0)
  // but i2c adressing uses the high 7 bits so it's 104    
  Wire.send(RTC_START_ADDRESS);  // start from address zero
  Wire.endTransmission();
  
  // START READING
  Wire.requestFrom(RTC_ADDRESS, RTC_DATA_SIZE); // transmit to device #104 (0x68)
  // the address specified in the datasheet is 208 (0xD0)
  // but i2c adressing uses the high 7 bits so it's 104    

  while(timecount <= endAddress)    // slave may send less than requested
  { 
    if (Wire.available()) {
      uint8_t c = Wire.receive(); // receive a byte as character
      registersRTC[timecount] = c;
      switch (timecount) {
      case 0:
        second = BCD2byte(c>>4, c&B00001111);
        break;
      case 1:
        minute = BCD2byte(c>>4, c&B00001111);
        break;
      case 2:
        hour = BCD2byte(c>>4, c&B00001111);
        break;
      case 3:
        day = c;
        break;
      case 4:
        date = BCD2byte(c>>4, c&B00001111);
        break;
      case 5:
        month = BCD2byte(c>>4, c&B00001111);
        break;
      case 6:
        year = BCD2byte(c>>4, c&B00001111);
        break;
      case 7:
        second_alarm1 = BCD2byte((c>>4)&B00000111, c&B00001111);
        break;
      case 8:
        minute_alarm1 = BCD2byte((c>>4)&B00000111, c&B00001111);
        break;
      case 9:
        hour_alarm1 = BCD2byte((c>>4)&B00000011, c&B00001111);
        break;
      case 10:
        day_alarm1 = BCD2byte((c>>4)&B00000011, c&B00001111);
        break;
      case 11:
        minute_alarm2 = BCD2byte((c>>4)&B00000111, c&B00001111);
        break;
      case 12:
        hour_alarm2 = BCD2byte((c>>4)&B00000011, c&B00001111);
        break;
      case 13:
        day_alarm2 = BCD2byte((c>>4)&B00000011, c&B00001111);
        break;
      }
      timecount++;
    }
  }

  timecount = 0;
}


/* writeRTC() - writes the stored variables to the RTC
 *
 * It writes the stored variables corresponding to time and date to the RTC. It loads the variables into 'registersRTC'
 * array and then, this array is sent to the RTC
 */
void WaspRTC::writeRTC() 
{
 int timecount = 0;
 Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x4A)
 // the address specified in the datasheet is 208 (0xD0)
 // but i2c adressing uses the high 7 bits so it's 104
 Wire.send(RTC_START_ADDRESS);  // start from address zero

 registersRTC[RTC_SECONDS_ADDRESS] = byte2BCD(second);
 registersRTC[RTC_MINUTES_ADDRESS] = byte2BCD(minute);
 registersRTC[RTC_HOURS_ADDRESS] = byte2BCD(hour);
 registersRTC[RTC_DAYS_ADDRESS] = day;
 registersRTC[RTC_DATE_ADDRESS] = byte2BCD(date);
 registersRTC[RTC_MONTH_ADDRESS] = byte2BCD(month);
 registersRTC[RTC_YEAR_ADDRESS] = byte2BCD(year);

 for(timecount = 0; timecount <= 0x06; timecount++) {
   Wire.send(registersRTC[timecount]);
 }

 Wire.endTransmission();
}


/* writeRTCalarm1() - writes stored alarm1 to the RTC
 *
 * It writes stored time in corresponding alarm1 variables to the RTC. It loads these values to 'registersRTC' array
 * and then is sent to the RTC
 */
void WaspRTC::writeRTCalarm1() 
{
 byte timecount = 0;
 Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x4A)
 // the address specified in the datasheet is 208 (0xD0)
 // but i2c adressing uses the high 7 bits so it's 104
 Wire.send(RTC_ALM1_START_ADDRESS);  // start from address zero

 registersRTC[RTC_ALM1_SECONDS_ADDRESS] = 0x7F & byte2BCD(second_alarm1);
 registersRTC[RTC_ALM1_MINUTES_ADDRESS] = 0x7F & byte2BCD(minute_alarm1);
 registersRTC[RTC_ALM1_HOURS_ADDRESS] = 0x7F & byte2BCD(hour_alarm1);
 registersRTC[RTC_ALM1_DAYS_ADDRESS] = 0x3F & byte2BCD(day_alarm1);

 for(timecount = 7; timecount <= 0x0A; timecount++) {
   Wire.send(registersRTC[timecount]);
 }

 Wire.endTransmission();
}


/* writeRTCalarm2() - writes stored alarm2 to the RTC
 *
 * It writes stored time in corresponding alarm2 variables to the RTC. It loads these values to 'registersRTC' array
 * and then is sent to the RTC
 */
void WaspRTC::writeRTCalarm2() 
{
 byte timecount = 0;
 Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x4A)
 // the address specified in the datasheet is 208 (0xD0)
 // but i2c adressing uses the high 7 bits so it's 104
 Wire.send(RTC_ALM2_START_ADDRESS);  // start from address 0x0B

 registersRTC[RTC_ALM2_MINUTES_ADDRESS] = 0x7F & byte2BCD(minute_alarm2);
 registersRTC[RTC_ALM2_HOURS_ADDRESS] = 0x7F & byte2BCD(hour_alarm2);
 registersRTC[RTC_ALM2_DAYS_ADDRESS] = 0x3F & byte2BCD(day_alarm2);


 for(timecount = 0X0B; timecount <= 0x0D; timecount++) {
   Wire.send(registersRTC[timecount]);
 }

 Wire.endTransmission();
}


/* configureAlarmMode(alarmNum, alarmMode) - configures alarmMode and enables interrupt
 *
 * It configures the mode for the alarm and enables interrupt. After calling this function, the alarm is set in the RTC.
 * Prior to this, time and date should be set.
 *
 * 'alarNum' specifies if the selected alarm is 1 or 2
 * 'alarMode' specifies the selected mode. The different modes are:
 *
 * -->  RTC_ALM1_MODE1	0	// Day,hours,minutes and seconds match
 * -->	RTC_ALM1_MODE2	1	// Date,hours,minutes and seconds match
 * -->	RTC_ALM1_MODE3	2	// Hours,minutes and seconds match
 * -->	RTC_ALM1_MODE4	3	// Minutes and seconds match
 * -->	RTC_ALM1_MODE5	4	// Seconds match
 * -->	RTC_ALM1_MODE6	5	// Once per second 
 * 
 * -->	RTC_ALM2_MODE1	0	// Day,hours and minutes match
 * -->	RTC_ALM2_MODE2	1	// Date,hours and minutes match
 * -->	RTC_ALM2_MODE3	2	// Hours and minutes
 * -->	RTC_ALM2_MODE4	3	// Minutes match
 * -->	RTC_ALM2_MODE5	4	// Once per minute
 */
void WaspRTC::configureAlarmMode (uint8_t alarmNum, uint8_t alarmMode)
{
 if (alarmNum == 1){

   registersRTC[RTC_CONTROL_ADDRESS] &= B11111101; // activate the INT/SQW output on alarm match
   registersRTC[RTC_CONTROL_ADDRESS] |= B00000101;
   writeRTCregister(RTC_CONTROL_ADDRESS);
   registersRTC[RTC_STATUS_ADDRESS] &= B11111100;  // reset the alarm flags
   writeRTCregister(RTC_STATUS_ADDRESS);

   switch (alarmMode){
     case 0: // when day, hours, minutes and seconds match
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] &= B01111111; // set A1M1 bit to 0
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] &= B01111111; // set A1M2 bit to 0
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] &= B01111111; // set A1M3 bit to 0
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] &= B01111111; // set A1M4 bit to 0
       registersRTC[RTC_ALM1_DAYS_ADDRESS] |= B01000000; // set DY/DT bit to 1
       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);

       break;

       case 1: // when date, hours, minutes and seconds match
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] &= B01111111; // set A1M1 bit to 0
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] &= B01111111; // set A1M2 bit to 0
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] &= B01111111; // set A1M3 bit to 0
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] &= B00111111; // set A1M4 bit to 0 and DY/DT to 0

       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);

       break;
       case 2: // when hours, minutes and seconds match
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] &= B01111111; // set A1M1 bit to 0
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] &= B01111111; // set A1M2 bit to 0
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] &= B01111111; // set A1M3 bit to 0
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] |= B10000000; // set A1M4 bit to 1
       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);

       break;
       case 3: // when minutes and seconds match
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] &= B01111111; // set A1M1 bit to 0
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] &= B01111111; // set A1M2 bit to 0
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] |= B10000000; // set A1M3 bit to 1
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] |= B10000000; // set A1M4 bit to 1
       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);
       break;
       case 4: // when seconds match
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] &= B01111111; // set A1M1 bit to 0
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] |= B10000000; // set A1M2 bit to 1
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] |= B10000000; // set A1M3 bit to 1
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] |= B10000000; // set A1M4 bit to 1
       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);
       break;
     case 5: // once per second
       registersRTC[RTC_ALM1_SECONDS_ADDRESS] |= B10000000; // set A1M1 bit to 1
       writeRTCregister(RTC_ALM1_SECONDS_ADDRESS);

       registersRTC[RTC_ALM1_MINUTES_ADDRESS] |= B10000000; // set A1M2 bit to 1
       writeRTCregister(RTC_ALM1_MINUTES_ADDRESS);

       registersRTC[RTC_ALM1_HOURS_ADDRESS] |= B10000000; // set A1M3 bit to 1
       writeRTCregister(RTC_ALM1_HOURS_ADDRESS);

       registersRTC[RTC_ALM1_DAYS_ADDRESS] |= B10000000; // set A1M4 bit to 1
       writeRTCregister(RTC_ALM1_DAYS_ADDRESS);
       break;
     case 6: // alarm1 OFF
       registersRTC[RTC_CONTROL_ADDRESS] &= B11111000; // de-activate the INT/SQW output on alarm match
       writeRTCregister(RTC_CONTROL_ADDRESS);
	detachInt();
       break;
   }

 }

 if (alarmNum == 2){

   registersRTC[RTC_CONTROL_ADDRESS] &= B11111110; // activate the INT/SQW output on alarm match
   registersRTC[RTC_CONTROL_ADDRESS] |= B00000110;
   writeRTCregister(RTC_CONTROL_ADDRESS);
   registersRTC[RTC_STATUS_ADDRESS] &= B11111100;  // reset the alarm flags
   writeRTCregister(RTC_STATUS_ADDRESS);

   switch (alarmMode){
     case 0: // when day, hours and minutes match
       registersRTC[RTC_ALM2_MINUTES_ADDRESS] &= B01111111; // set A2M2 bit to 0
       writeRTCregister(RTC_ALM2_MINUTES_ADDRESS);

       registersRTC[RTC_ALM2_HOURS_ADDRESS] &= B01111111; // set A2M3 bit to 0
       writeRTCregister(RTC_ALM2_HOURS_ADDRESS);

       registersRTC[RTC_ALM2_DAYS_ADDRESS] &= B01111111; // set A2M4 bit to 0
       registersRTC[RTC_ALM2_DAYS_ADDRESS] |= B01000000; // set DY/DT bit to 1
       writeRTCregister(RTC_ALM2_DAYS_ADDRESS);

       break;

     case 1: // when date,hours and minutes match
       registersRTC[RTC_ALM2_MINUTES_ADDRESS] &= B01111111; // set A2M2 bit to 0
       writeRTCregister(RTC_ALM2_MINUTES_ADDRESS);

       registersRTC[RTC_ALM2_HOURS_ADDRESS] &= B01111111; // set A2M3 bit to 0
       writeRTCregister(RTC_ALM2_HOURS_ADDRESS);

       registersRTC[RTC_ALM2_DAYS_ADDRESS] &= B00111111; // set A2M4 bit to 0 and DY/DT to 0
       writeRTCregister(RTC_ALM2_DAYS_ADDRESS);

       break;
     case 2: // when hours and minutes match
       registersRTC[RTC_ALM2_MINUTES_ADDRESS] &= B01111111; // set A2M2 bit to 0
       writeRTCregister(RTC_ALM2_MINUTES_ADDRESS);

       registersRTC[RTC_ALM2_HOURS_ADDRESS] &= B01111111; // set A2M3 bit to 0
       writeRTCregister(RTC_ALM2_HOURS_ADDRESS);

       registersRTC[RTC_ALM2_DAYS_ADDRESS] |= B10000000; // set A2M4 bit to 1
       writeRTCregister(RTC_ALM2_DAYS_ADDRESS);

       break;
     case 3: // when minutes match
       registersRTC[RTC_ALM2_MINUTES_ADDRESS] &= B01111111; // set A2M2 bit to 0
       writeRTCregister(RTC_ALM2_MINUTES_ADDRESS);

       registersRTC[RTC_ALM2_HOURS_ADDRESS] |= B10000000; // set A2M3 bit to 1
       writeRTCregister(RTC_ALM2_HOURS_ADDRESS);

       registersRTC[RTC_ALM2_DAYS_ADDRESS] |= B10000000; // set A2M4 bit to 1
       writeRTCregister(RTC_ALM2_DAYS_ADDRESS);

       break;
     case 4: // Once per minute
      registersRTC[RTC_ALM2_MINUTES_ADDRESS] |= B10000000; // set A2M2 bit to 1
       writeRTCregister(RTC_ALM2_MINUTES_ADDRESS);

       registersRTC[RTC_ALM2_HOURS_ADDRESS] |= B10000000; // set A2M3 bit to 1
       writeRTCregister(RTC_ALM2_HOURS_ADDRESS);

       registersRTC[RTC_ALM2_DAYS_ADDRESS] |= B10000000; // set A2M4 bit to 1
       writeRTCregister(RTC_ALM2_DAYS_ADDRESS);

       break;
     case 5: // alarm 2 OFF
       registersRTC[RTC_CONTROL_ADDRESS] &= B11111000; // de-activate the INT/SQW output on alarm match
       writeRTCregister(RTC_CONTROL_ADDRESS);
	detachInt();
       break;
   }

 }

	attachInt();
}


/* writeRTCregister(theaddress) - writes the specified register into the RTC
 *
 * It writes the specified register into the RTC. It takes the data from 'registersRTC' array.
 *
 * - FIXME: modify it to write to EEPROM
 */
void WaspRTC::writeRTCregister(uint8_t theAddress) 
{
  // ADDRESSING FROM MEMORY POSITION RECEIVED AS PARAMETER
  Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x68)
  // the address specified in the datasheet is 208 (0xD0)
  // but i2c adressing uses the high 7 bits so it's 104    
  Wire.send(theAddress);  // start from address theAddress

  // START SENDING
  Wire.send(registersRTC[theAddress]);
  Wire.endTransmission();
}


/* readRTCregister(theaddress) - reads from RTC the specified register
 *
 * It reads from RTC the specified register and stores into 'registersRTC' array in corresponding position
 *
 * - FIXME: modify it to read from EEPROM
 */
void WaspRTC::readRTCregister(uint8_t theAddress) 
{
  // ADDRESSING FROM MEMORY POSITION RECEIVED AS PARAMETER
  Wire.beginTransmission(RTC_ADDRESS); // transmit to device #104 (0x68)
  // the address specified in the datasheet is 208 (0xD0)
  // but i2c adressing uses the high 7 bits so it's 104    
  Wire.send(theAddress);  // start from address theAddress
  Wire.endTransmission();
  
  // START READING
  Wire.requestFrom(RTC_ADDRESS, 0x01); // transmit to device #104 (0x68)
  // the address specified in the datasheet is 208 (0xD0)
  // but i2c adressing uses the high 7 bits so it's 104    
  while(!Wire.available()) {};
  registersRTC[theAddress] = Wire.receive();
  Wire.endTransmission();
}


/* setTime(time) - sets time and date in the RTC
 *
 * It sets time and date in the RTC.
 *
 * After setting the variables, function 'writeRTC' is called to write to RTC the values
 *
 * 'time' must be set in a specify format: YY:MM:DD:dow:hh:mm:ss
 */
void WaspRTC::setTime(const char* time)
{
	uint8_t aux=0, aux2=0;
	
	aux=(uint8_t) time[0] - 48;
	aux2=(uint8_t) time[1] - 48;
	year = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[3] - 48;
	aux2=(uint8_t) time[4] - 48;
	month = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[6] - 48;
	aux2=(uint8_t) time[7] - 48;
	date = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[9] - 48;
	aux2=(uint8_t) time[10] - 48;
	day = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[12] - 48;
	aux2=(uint8_t) time[13] - 48;
	hour = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[15] - 48;
	aux2=(uint8_t) time[16] - 48;
	minute = RTC.BCD2byte(aux, aux2);
	aux=(uint8_t) time[18] - 48;
	aux2=(uint8_t) time[19] - 48;
	second = RTC.BCD2byte(aux, aux2);
	writeRTC();
}

/* setTime(_year,_month,_date,day_week,_hour,_minute,_second) - sets time and date in the RTC
 *
 * It sets time and date in the RTC.
 *
 * After setting the variables, function 'writeRTC' is called to write to RTC the values
 *
 * Each input corresponds to the relayed part of time and date.
 */
void WaspRTC::setTime(uint8_t _year, uint8_t _month, uint8_t _date, uint8_t day_week, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
	uint8_t aux=0, aux2=0;
	if(_year<10)
	{
		aux=0;
		aux2=_year;
		year = RTC.BCD2byte(aux, aux2);
	}
	else if(_year>=10)
	{
		aux2=_year%10;
		aux=_year/10;
		year = RTC.BCD2byte(aux, aux2);
	}
	if(_month<10)
	{
		aux=0;
		aux2=_month;
		month = RTC.BCD2byte(aux, aux2);
	}
	else if(_month>=10)
	{
		aux2=_month%10;
		aux=_month/10;
		month = RTC.BCD2byte(aux, aux2);
	}
	if(_date<10)
	{
		aux=0;
		aux2=_date;
		date = RTC.BCD2byte(aux, aux2);
	}
	else if(_date>=10)
	{
		aux2=_date%10;
		aux=_date/10;
		date = RTC.BCD2byte(aux, aux2);
	}
	if(day_week<10)
	{
		aux=0;
		aux2=day_week;
		day = RTC.BCD2byte(aux, aux2);
	}
	else if(day_week>=10)
	{
		aux2=day_week%10;
		aux=day_week/10;
		day = RTC.BCD2byte(aux, aux2);
	}
	if(_hour<10)
	{
		aux=0;
		aux2=_hour;
		hour = RTC.BCD2byte(aux, aux2);
	}
	else if(_hour>=10)
	{
		aux2=_hour%10;
		aux=_hour/10;
		hour = RTC.BCD2byte(aux, aux2);
	}
	if(_minute<10)
	{
		aux=0;
		aux2=_minute;
		minute = RTC.BCD2byte(aux, aux2);
	}
	else if(_minute>=10)
	{
		aux2=_minute%10;
		aux=_minute/10;
		minute = RTC.BCD2byte(aux, aux2);
	}
	if(_second<10)
	{
		aux=0;
		aux2=_second;
		second = RTC.BCD2byte(aux, aux2);
	}
	else if(_second>=10)
	{
		aux2=_second%10;
		aux=_second/10;
		second = RTC.BCD2byte(aux, aux2);
	}
	writeRTC();
}


/* getTime() - gets time and date
 *
 * It gets time and date, storing them in 'registersRTC' array.
 *
 * It returns a string containing time and data in the following format: "YY:MM:DD:dw:hh:mm:ss"
 */
char* WaspRTC::getTime()
{
	readRTC(RTC_DATE_ADDRESS_2);
	return getTimestamp();
}


/* setTimeFromGPS() - sets time and date from the GPS to the RTC. GPS has to be initialized first and got the time/date
 *
 * It sets time and date from the GPS to the RTC. GPS has to be initialized first and got the time/date
 *
 * It returns nothing
 */
void WaspRTC::setTimeFromGPS()
{
	int day, month, year, hour, minute, second = 0;
  
	day = (GPS.dateGPS[0]-'0')*10 + (GPS.dateGPS[1]-'0');
  
	month = (GPS.dateGPS[2]-'0')*10 + (GPS.dateGPS[3]-'0');
  
	year = (GPS.dateGPS[4]-'0')*10 + (GPS.dateGPS[5]-'0');
  
	hour = (GPS.timeGPS[0]-'0')*10 + (GPS.timeGPS[1]-'0');
  
	minute = (GPS.timeGPS[2]-'0')*10 + (GPS.timeGPS[3]-'0');
  
	second = (GPS.timeGPS[4]-'0')*10 + (GPS.timeGPS[5]-'0');
  
	RTC.setTime(year, month, day, 1, hour, minute, second);
}


/* getTemperature() - gets temperature
 *
 * It gets temperature from RTC. It reads associated registers to temperature and stores the temperature in a variable
 * called 'temp'. If temperature is negative, a variable called 'tempNegative' is set to TRUE.
 *
 * It returns temperature value.
 */
uint8_t	WaspRTC::getTemperature()
{
  readRTCregister(RTC_MSB_TEMP_ADDRESS);
  readRTCregister(RTC_LSB_TEMP_ADDRESS);
  temp = (registersRTC[RTC_MSB_TEMP_ADDRESS]&B01111111) + (registersRTC[RTC_LSB_TEMP_ADDRESS]>>6);
  
  if (registersRTC[RTC_MSB_TEMP_ADDRESS]>>7 == 1)	
  {
	  tempNegative=true;
	  ~(temp);
	  temp++;
  }
  else tempNegative=false;
   
	return temp;
}


/* setAlarm1(time,offset,mode) - sets Alarm1 to the specified time, offset and mode
 *
 * It sets Alarm1 to the specified time, offset and mode.
 *
 * 'time' --> has the following format : "dd:hh:mm:ss"
 * 'offset' --> possible values are:
 *	RTC_OFFSET--> 'time' is added to the actual time read from RTC
 *	RTC_ABSOLUTE--> 'time' is set as the time for Alarm1
 * 'mode' --> specifies the mode for Alarm1. Possible values are the same as specified in 'configureAlarmMode'
 *
 * This function specifies the time for alarm, sets alarm in RTC and enables interrupt.
 */
void WaspRTC::setAlarm1(const char* time, uint8_t offset, uint8_t mode)
{
	uint8_t aux=0, aux2=0;
	aux=(uint8_t) time[0] - 48;
	aux2=(uint8_t) time[1] - 48;
	day_alarm1 = BCD2byte(aux, aux2);
	aux=(uint8_t) time[3] - 48;
	aux2=(uint8_t) time[4] - 48;
	hour_alarm1 = BCD2byte(aux, aux2);
	aux=(uint8_t) time[6] - 48;
	aux2=(uint8_t) time[7] - 48;
	minute_alarm1 = BCD2byte(aux, aux2);
	aux=(uint8_t) time[9] - 48;
	aux2=(uint8_t) time[10] - 48;
	second_alarm1 = BCD2byte(aux, aux2);
	
	if(offset==RTC_OFFSET) // add the date to the actual date
	{
		readRTC(RTC_DATE_ADDRESS_2);
		second_alarm1+=second;
		if(second_alarm1>=60)
		{
			second_alarm1-=60;
			minute_alarm1++;
		}
		minute_alarm1+=minute;
		if(minute_alarm1>=60)
		{
			minute_alarm1-=60;
			hour_alarm1++;
		}
		hour_alarm1+=hour;
		if(hour_alarm1>=24)
		{
			hour_alarm1-=24;
			day_alarm1++;
		}
		if(mode==RTC_ALM1_MODE1)
		{
			day_alarm1+=day;
			if( day_alarm1>7 ) day_alarm1-=7;
		}
		else
		{	
			day_alarm1+=date;
			if( month==1 || month==3 || month==5 || month==7 || month==8 || month==12 )
			{
				if( day_alarm1>31 ) day_alarm1-=31;
			}
			if( month==4 || month==6 || month==9 || month==10 || month==11 )
			{
				if( day_alarm1>30 ) day_alarm1-=30;
			}
			if( month==2 )
			{
				if( day_alarm1>28 ) day_alarm1-=28;
			}			
		}
	}
	
	RTC.writeRTCalarm1();
	RTC.configureAlarmMode(1,mode);
}

/* setAlarm1(day_date,_hour,_minute,_second,offset,mode) - sets Alarm1 to the specified time, offset and mode
 *
 * It sets Alarm1 to the specified time, offset and mode.
 *
 * 'day_date,_hour,,_minute,_second' --> specifies the time for Alarm1
 * 'offset' --> possible values are:
 *	RTC_OFFSET--> 'time' is added to the actual time read from RTC
 *	RTC_ABSOLUTE--> 'time' is set as the time for Alarm1
 * 'mode' --> specifies the mode for Alarm1. Possible values are the same as specified in 'configureAlarmMode'
 *
 * This function specifies the time for alarm, sets alarm in RTC and enables interrupt.
 */
void WaspRTC::setAlarm1(uint8_t day_date, uint8_t _hour, uint8_t _minute, uint8_t _second, uint8_t offset, uint8_t mode)
{
	uint8_t aux=0, aux2=0;

	if(day_date<10)
	{
		aux=0;
		aux2=day_date;
		day_alarm1 = BCD2byte(aux, aux2);
	}
	else if(day_date>=10)
	{
		aux2=day_date%10;
		aux=day_date/10;
		day_alarm1 = BCD2byte(aux, aux2);
	}
	if(_hour<10)
	{
		aux=0;
		aux2=_hour;
		hour_alarm1 = BCD2byte(aux, aux2);
	}
	else if(_hour>=10)
	{
		aux2=_hour%10;
		aux=_hour/10;
		hour_alarm1 = BCD2byte(aux, aux2);
	}
	if(_minute<10)
	{
		aux=0;
		aux2=_minute;
		minute_alarm1 = BCD2byte(aux, aux2);
	}
	else if(_minute>=10)
	{
		aux2=_minute%10;
		aux=_minute/10;
		minute_alarm1 = BCD2byte(aux, aux2);
	}
	if(_second<10)
	{
		aux=0;
		aux2=_second;
		second_alarm1 = BCD2byte(aux, aux2);
	}
	else if(_second>=10)
	{
		aux2=_second%10;
		aux=_second/10;
		second_alarm1 = BCD2byte(aux, aux2);
	}
	
	if(offset==RTC_OFFSET) // add the date to the actual date
	{
		readRTC(RTC_DATE_ADDRESS_2);
		second_alarm1+=second;
		if(second_alarm1>=60)
		{
			second_alarm1-=60;
			minute_alarm1++;
		}
		minute_alarm1+=minute;
		if(minute_alarm1>=60)
		{
			minute_alarm1-=60;
			hour_alarm1++;
		}
		hour_alarm1+=hour;
		if(hour_alarm1>=24)
		{
			hour_alarm1-=24;
			day_alarm1++;
		}
		if(mode==RTC_ALM1_MODE1)
		{
			day_alarm1+=day;
			if( day_alarm1>7 ) day_alarm1-=7;
		}
		else
		{	
			day_alarm1+=date;
			if( month==1 || month==3 || month==5 || month==7 || month==8 || month==12 )
			{
				if( day_alarm1>31 ) day_alarm1-=31;
			}
			if( month==4 || month==6 || month==9 || month==10 || month==11 )
			{
				if( day_alarm1>30 ) day_alarm1-=30;
			}
			if( month==2 )
			{
				if( day_alarm1>28 ) day_alarm1-=28;
			}			
		}
	}
	
	RTC.writeRTCalarm1();
	RTC.configureAlarmMode(1,mode);
}


/* getAlarm1() - gets Alarm1 time
 *
 * It gets Alarm1 time from RTC. 
 *
 * It returns a string containing this time and date for Alarm1
 */
char* WaspRTC::getAlarm1()
{
	char aux[40];
	
	readRTC(RTC_ALARM1_ADDRESS);
	sprintf(aux, "%02d, %02d:%02d:%02d - %02d:%02d:%02d", date, year, month, day_alarm1, hour_alarm1, minute_alarm1, second_alarm1);
	return aux;
}

/* setAlarm2(time,offset,mode) - sets Alarm2 to the specified time, offset and mode
 *
 * It sets Alarm2 to the specified time, offset and mode.
 *
 * 'time' --> has the following format : "dd:hh:mm"
 * 'offset' --> possible values are:
 *	RTC_OFFSET--> 'time' is added to the actual time read from RTC
 *	RTC_ABSOLUTE--> 'time' is set as the time for Alarm2
 * 'mode' --> specifies the mode for Alarm2. Possible values are the same as specified in 'configureAlarmMode'
 *
 * This function specifies the time for alarm, sets alarm in RTC and enables interrupt.
 */
void WaspRTC::setAlarm2(const char* time, uint8_t offset, uint8_t mode)
{
	uint8_t aux=0, aux2=0;
	
	aux=(uint8_t) time[0] - 48;
	aux2=(uint8_t) time[1] - 48;
	day_alarm2 = BCD2byte(aux, aux2);
	aux=(uint8_t) time[3] - 48;
	aux2=(uint8_t) time[4] - 48;
	hour_alarm2 = BCD2byte(aux, aux2);
	aux=(uint8_t) time[6] - 48;
	aux2=(uint8_t) time[7] - 48;
	minute_alarm2 = BCD2byte(aux, aux2);
	
	if(offset==RTC_OFFSET) // add the date to the actual date
	{
		readRTC(RTC_DATE_ADDRESS_2);
		minute_alarm2+=minute;
		if(minute_alarm2>=60)
		{
			minute_alarm2-=60;
			hour_alarm2++;
		}
		hour_alarm2+=hour;
		if(hour_alarm2>=24)
		{
			hour_alarm2-=24;
			day_alarm2++;
		}
		if(mode==RTC_ALM2_MODE1)
		{
			day_alarm2+=day;
			if( day_alarm2>7 ) day_alarm2-=7;
		}
		else
		{	
			day_alarm2+=date;
			if( month==1 || month==3 || month==5 || month==7 || month==8 || month==12 )
			{
				if( day_alarm2>31 ) day_alarm2-=31;
			}
			if( month==4 || month==6 || month==9 || month==10 || month==11 )
			{
				if( day_alarm2>30 ) day_alarm2-=30;
			}
			if( month==2 )
			{
				if( day_alarm2>28 ) day_alarm2-=28;
			}			
		}
	}
	
	RTC.writeRTCalarm2();
	RTC.configureAlarmMode(2,mode);
}

/* setAlarm2(day_date,_hour,_minute,offset,mode) - sets Alarm2 to the specified time, offset and mode
 *
 * It sets Alarm2 to the specified time, offset and mode.
 *
 * 'day_date,_hour,_minute' --> specifies the time for Alarm2
 * 'offset' --> possible values are:
 *	RTC_OFFSET--> 'time' is added to the actual time read from RTC
 *	RTC_ABSOLUTE--> 'time' is set as the time for Alarm2
 * 'mode' --> specifies the mode for Alarm2. Possible values are the same as specified in 'configureAlarmMode'
 *
 * This function specifies the time for alarm, sets alarm in RTC and enables interrupt.
 */
void WaspRTC::setAlarm2(uint8_t day_date, uint8_t _hour, uint8_t _minute, uint8_t offset, uint8_t mode)
{
	uint8_t aux=0, aux2=0;
	if(day_date<10)
	{
		aux=0;
		aux2=day_date;
		day_alarm2 = BCD2byte(aux, aux2);
	}
	else if(day_date>=10)
	{
		aux2=day_date%10;
		aux=day_date/10;
		day_alarm2 = BCD2byte(aux, aux2);
	}
	if(_hour<10)
	{
		aux=0;
		aux2=_hour;
		hour_alarm2 = BCD2byte(aux, aux2);
	}
	else if(_hour>=10)
	{
		aux2=_hour%10;
		aux=_hour/10;
		hour_alarm2 = BCD2byte(aux, aux2);
	}
	if(_minute<10)
	{
		aux=0;
		aux2=_minute;
		minute_alarm2 = BCD2byte(aux, aux2);
	}
	else if(_minute>=10)
	{
		aux2=_minute%10;
		aux=_minute/10;
		minute_alarm2 = BCD2byte(aux, aux2);
	}
	
	if(offset==RTC_OFFSET) // add the date to the actual date
	{
		readRTC(RTC_DATE_ADDRESS_2);
		minute_alarm2+=minute;
		if(minute_alarm2>=60)
		{
			minute_alarm2-=60;
			hour_alarm2++;
		}
		hour_alarm2+=hour;
		if(hour_alarm2>=24)
		{
			hour_alarm2-=24;
			day_alarm2++;
		}
		if(mode==RTC_ALM2_MODE1)
		{
			day_alarm2+=day;
			if( day_alarm2>7 ) day_alarm2-=7;
		}
		else
		{	
			day_alarm2+=date;
			if( month==1 || month==3 || month==5 || month==7 || month==8 || month==12 )
			{
				if( day_alarm2>31 ) day_alarm2-=31;
			}
			if( month==4 || month==6 || month==9 || month==10 || month==11 )
			{
				if( day_alarm2>30 ) day_alarm2-=30;
			}
			if( month==2 )
			{
				if( day_alarm2>28 ) day_alarm2-=28;
			}			
		}
	}
	
	RTC.writeRTCalarm2();
	RTC.configureAlarmMode(2,mode);
}


/* getAlarm1() - gets Alarm1 time
 *
 * It gets Alarm1 time from RTC. 
 *
 * It returns a string containing this time and date for Alarm1
 */
char* WaspRTC::getAlarm2()
{
	char aux[35];
	
	readRTC(RTC_ALARM2_ADDRESS);
	sprintf (aux, "%02d, %02d:%02d:%02d - %02d:%02d", date, year, month, day_alarm2, hour_alarm2, minute_alarm2);
	return aux;
}


/* clearAlarmFlag() - clear alarm flags in RTC
 *
 * It clears alarm flags (A1F and A2F) in the RTC.
 * If these flags are not cleared after an interrupt is captured, no more interrupts could be captured.
 */
void WaspRTC::clearAlarmFlag()
{
	RTC.registersRTC[RTC_STATUS_ADDRESS] &= B11111100;  // reset the alarm flags in RT
	RTC.writeRTCregister(RTC_STATUS_ADDRESS);
}


/* BCD2byte ( number ) - converts a BCD number to an integer
 * 
 */
uint8_t WaspRTC::BCD2byte(uint8_t number) 
{
  return (number>>4)*10 | (number & 0x0F);
}

/* BCD2byte ( high, low ) - converts a BCD number to an integer
 */
uint8_t WaspRTC::BCD2byte(uint8_t high, uint8_t low) 
{
  return high*10 + low;
}

/* byte2BCD ( number ) - converts an integer number to a BCD number
 */
uint8_t WaspRTC::byte2BCD(uint8_t theNumber) 
{
  return (theNumber%10 | ((theNumber-theNumber%10)/10)<<4);  // note that binary operations have preference on the others
}


/*******************************************************************************
 * HANDLING HARDWARE INTERRUPTS
 *******************************************************************************/

/*
 * attachInterrupt(void) - configure the specific hardware interrupt for the RTC
 *
 * the default interrupt functions are defined inside WInterrupts.c
 */
void WaspRTC::attachInt(void)
{
	enableInterrupts(RTC_INT);
}

/*
 * detachInterrupt(void) - unset the specific hardware interrupt for the RTC
 *
 * It also clear Alarm Flags so as to provide RTC the correct parameters for future alarms
 */
void WaspRTC::detachInt(void)
{
  disableInterrupts(RTC_INT);
  clearAlarmFlag();
}


// Private Methods /////////////////////////////////////////////////////////////

// Preinstantiate Objects //////////////////////////////////////////////////////

WaspRTC RTC = WaspRTC();

