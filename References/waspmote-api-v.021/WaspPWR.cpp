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
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
 
#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Variables ////////////////////////////////////////////////////////////////

// these two variables have been moved to the general system
uint8_t intIPRA = 0;
uint8_t intIPRB = 0;

// Constructors ////////////////////////////////////////////////////////////////

WaspPWR::WaspPWR()
{
  // nothing to do when constructing
}

// Private Methods /////////////////////////////////////////////////////////////

/* setIPF ( peripheral )
 * - sets a certain internal peripheral on 
 * - to control the pwr on the different internal peripherals it is
 *   convenient to read MCU's manual on pgs. 56/57
 * FIXME: missing all the Timers and UART to reduce consumption
 */
void WaspPWR::setIPF(uint8_t peripheral) 
{
	setIPF_(peripheral);
	intIPRA = IPRA;
}

/* resetIPR ( peripheral )
 * - resets a certain internal peripheral to off 
 * - to control the pwr on the different internal peripherals it is
 *   convenient to read MCU's manual on pgs. 56/57
 * FIXME: missing all the Timers and UART to reduce consumption
 */
void WaspPWR::resetIPF(uint8_t peripheral) 
{
	resetIPF_(peripheral);
	intIPRA = IPRA;
}

/* uint8_t getIPR ( )
 * - answers with the whole IPR 
 */
uint8_t WaspPWR::getIPF() 
{
	return intIPRA;
}


// Public Methods //////////////////////////////////////////////////////////////



/* setSensorPower( type, mode) - set ON/OFF 3V3 or 5V switches
 *
 * It sets ON/OFF 3V3 or 5V switches
 */
void 	WaspPWR::setSensorPower(uint8_t type, uint8_t mode)
{
	pinMode(SENS_PW_3V3,OUTPUT);
	pinMode(SENS_PW_5V,OUTPUT);
	
	switch( type )
	{
		case SENS_3V3: 	if(mode==SENS_ON) digitalWrite(SENS_PW_3V3,HIGH);
		else if(mode==SENS_OFF) digitalWrite(SENS_PW_3V3,LOW);
		break;
		case SENS_5V:	if(mode==SENS_ON) digitalWrite(SENS_PW_5V,HIGH);
		else if(mode==SENS_OFF) digitalWrite(SENS_PW_5V,LOW);
		break;
	}
}


/* setWatchdog( mode, timer) - enables or disables watchdog interruption
 *
 * It enables or disables watchdog interruption.
 *
 * 'mode' --> if mode=WTD_ON, it enables watchdog interruption. If mode=WTD_OFF, it disables watchdog interruption.
 * 'timer' --> it specifies the time before the watchdog activates the interruption. Possible values are:
 * 	WTD_16MS	0
 *	WTD_32MS	1
 *	WTD_64MS	2
 *	WTD_128MS	3
 *	WTD_250MS	4
 *	WTD_500MS	5
 *	WTD_1S		6
 *	WTD_2S		7
 *	WTD_4S		8
 *	WTD_8S		9
 * 
 * It returns nothing
 */
void 	WaspPWR::setWatchdog(uint8_t mode, uint8_t timer)
{
	if(mode==WTD_ON)
	{
		enableInterrupts(WTD_INT);
		setup_watchdog(timer); // set watchdog interrupt to wake up from Sleep Power Down Mode
	}
	
	if(mode==WTD_OFF)
	{
		disableInterrupts(WTD_INT);
		off_watchdog();
	}
}


/* switchesOFF() - switches off the Waspmote switches specified
 *
 * It switches off all Waspmote switches
 */
void	WaspPWR::switchesOFF(uint8_t option)
{
	cbi(ADCSRA,ADEN);		// switch Analog to Digital Converter OFF
	pinMode(SERID_PW,OUTPUT);
	digitalWrite(SERID_PW,LOW);
	pinMode(MEM_PW,OUTPUT);
	digitalWrite(MEM_PW,LOW);
		
	if( option & SENS_OFF )
	{
		pinMode(SENS_PW_3V3,OUTPUT);
		digitalWrite(SENS_PW_3V3,LOW);	
		pinMode(SENS_PW_5V,OUTPUT);
		digitalWrite(SENS_PW_5V,LOW);
	}
	
	if( option & UART0_OFF )
	{
		XBee.setMode(XBEE_OFF);
	}
	
	if( option & UART1_OFF )
	{
		closeSerial(1);
		pinMode(MUX_PW, OUTPUT);
		digitalWrite(MUX_PW, LOW);
		pinMode(GPS_PW, OUTPUT);
		digitalWrite(GPS_PW, LOW);
	}
	
	if( option & RTC_OFF )
	{
		if( !(ACC.AccEventMode & ACC_FREE_FALL)  && !(ACC.AccEventMode & ACC_DIRECTION) )
		{
			pinMode(RTC_PW,OUTPUT);
			digitalWrite(RTC_PW,LOW);
		}
	}
	
	if( option & BAT_OFF )
	{
		pinMode(BAT_MONITOR_PW,OUTPUT);
		digitalWrite(BAT_MONITOR_PW,LOW);
	}	
}


/* switchesON() - switches on all Waspmote switches
 *
 * It switches on all Waspmote switches
 */
void	WaspPWR::switchesON(uint8_t option)
{
	sbi(ADCSRA,ADEN);        		// switch Analog to Digital Converter ON
}


/* clearInts() - clears all captured interrupts to allow new interrupts on that modules
 *
 * It clears all captured interrupts to allow new interrupts on that modules
 */
void 	WaspPWR::clearInts()
{
	if( intFlag & ACC_INT )
	{
		ACC.setFF();
	}
	if( intFlag & BAT_INT )
	{
	}
	if( intFlag & RTC_INT ) // hay que mirar si está inicializado el I2C
	{
		RTC.clearAlarmFlag();
	}
	if( intFlag & UART1_INT )
	{
		enableInterrupts(UART1_INT);
	}
	if( intFlag & WTD_INT )
	{
	}
	if( intFlag & SENS_INT )
	{
	}
}

/* sleep() - sets the microcontroller to the lowest consumption sleep mode
 *
 * It sets the microcontroller to the lowest consumption sleep mode. Before setting this state, some interruption
 * should be enabled to be able to wake up the microcontroller from this state.
 *
 * It switches off all the switches on the Waspmote board.
 *
 * It returns nothing.
 */
void	WaspPWR::sleep(uint8_t option)
{
	switchesOFF(option);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	sleep_disable();
	switchesON(option);
}


/* sleep(timer) - sets the microcontroller to the lowest consumption sleep mode
 *
 * It sets the microcontroller to the lowest consumption sleep mode. It enables watchdog interruption to be able to
 * wake up the microcontroller after 'timer' time.
 *
 * 'timer' --> it specifies the time before the watchdog activates the interruption. Possible values are:
 * 	WTD_16MS	0
 *	WTD_32MS	1
 *	WTD_64MS	2
 *	WTD_128MS	3
 *	WTD_250MS	4
 *	WTD_500MS	5
 *	WTD_1S		6
 *	WTD_2S		7
 *	WTD_4S		8
 *	WTD_8S		9
 * 
 * It switches off all the switches on the Waspmote board.
 *
 * It returns nothing.
 */
void	WaspPWR::sleep(uint8_t	timer, uint8_t option)
{
	switchesOFF(option);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	
	setWatchdog(WTD_ON,timer);
	sleep_mode();
	sleep_disable();
	switchesON(option);
	
}


/* deepSleep(time2wake, offset, mode) - sets the microcontroller to the lowest consumption sleep mode
 *
 * It sets the microcontroller to the lowest consumption sleep mode. It enables RTC interruption to be able to
 * wake up the microcontroller when the RTC alarm is launched.
 *
 * 'time2wake' --> it specifies the time at which the RTC alarm will activate. It must follow the next format:
 *	"DD:HH:MM:SS"
 * 'offset' --> it specifies if 'time2wake' is added to the actual time or if this time is set as the alarm
 * 'mode' --> it specifies the mode for RTC alarm
 *
 * It uses Alarm1 on the RTC due to this Alarm has more precision than Alarm2
 * 
 * It switches off all the switches on the Waspmote board.
 *
 * It returns nothing.
 */
void	WaspPWR::deepSleep(const char*	time2wake, uint8_t offset, uint8_t mode, uint8_t option)
{
	// Set RTC alarme to wake up from Sleep Power Down Mode
	RTC.setAlarm1(time2wake,offset,mode);
	RTC.close();
	switchesOFF(option);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	sleep_disable();
	switchesON(option);
	RTC.ON();
	RTC.clearAlarmFlag();
	if( option & RTC_OFF ) RTC.OFF();
}


/* hibernate(time2wake, offset, mode) - switches off the general switch and enables RTC interruption
 *
 * It switches off the general switch and enables RTC interruption. It enables RTC interruption to be able to
 * switch on the general switch.
 *
 * When this function is called, Waspmote has no power and when RTC alarm is activated, the power will return to
 * the board. This will cause the Waspmote inits again, restarting the code from the init.
 *
 * 'time2wake' --> it specifies the time at which the RTC alarm will activate. It must follow the next format:
 *	"DD:HH:MM:SS"
 * 'offset' --> it specifies if 'time2wake' is added to the actual time or if this time is set as the alarm
 * 'mode' --> it specifies the mode for RTC alarm
 *
 * It uses Alarm1 on the RTC due to this Alarm has more precision than Alarm2
 * 
 * It switches off all the switches on the Waspmote board.
 *
 * It returns nothing.
 */
void WaspPWR::hibernate(const char* time2wake, uint8_t offset, uint8_t mode)
{     
   pinMode(XBEE_PW,OUTPUT);
   digitalWrite(XBEE_PW, LOW);

   closeSerial(0);

   while(digitalRead(GPS_PW))
   {              
       digitalWrite(GPS_PW,LOW);
   }
   RTC.ON();
   // Set RTC alarme to wake up from Sleep Power Down Mode
   RTC.setAlarm1(time2wake,offset,mode);
   RTC.close();
   RTC.setMode(RTC_OFF, RTC_NORMAL_MODE);
   Utils.writeEEPROM(HIB_ADDR,HIB_VALUE);
   pinMode(RTC_SLEEP,OUTPUT);
   digitalWrite(RTC_SLEEP,HIGH);
   delay(18);
   digitalWrite(RTC_SLEEP,LOW);
   // To avoid executing any other function after calling hibernate
   while(1);
}


/* getBatteryLevel() - gets % of remaining battery
 *
 * It gets the % of remaining battery.
 *
 * It gives a value of 1024 for +3V3
 * A resistor bridge is put to down max +4V2 battery level around +2V07 on 100% battery charge
 * Minimum value for good battery is +1V5, so with resistor bridge is set to +0V75
 * Values (in this case) are from 204 to 567
 */
uint8_t    WaspPWR::getBatteryLevel()
{
   float aux=0;
   uint8_t resul=0;
   pinMode(BAT_MONITOR_PW,OUTPUT);
   digitalWrite(BAT_MONITOR_PW,HIGH);
   aux=analogRead(0);
         if (aux < 512)
   {
       aux = 0;
       resul=(uint8_t) aux;
       digitalWrite(BAT_MONITOR_PW,LOW);
       return resul;
   }
   if (aux > 651)
   {
       aux = 100;
       resul=(uint8_t) aux;
       digitalWrite(BAT_MONITOR_PW,LOW);
       return resul;
   }
   if ((aux <= 651)&&(aux > 543))
   {          aux = (aux * (90.0/108.0)) - 442.0;
       resul=(uint8_t) aux;
       digitalWrite(BAT_MONITOR_PW,LOW);
       return resul;
   }
   else
   {
       aux = ((10.0/(543.0-511.0)) * aux) - 160.0;
       resul=(uint8_t) aux;
       digitalWrite(BAT_MONITOR_PW,LOW);
       return resul;
   }   
} 





/* getBatteryVolts() - gets Volts of remaining battery
 *
 * This function gets the Volts of remaining battery.
 *
 * Returns the voltage of battery. 
 */
float WaspPWR::getBatteryVolts()
{
   float aux_volts=0;
   pinMode(BAT_MONITOR_PW,OUTPUT);
   digitalWrite(BAT_MONITOR_PW,HIGH);
   aux_volts=analogRead(0);
   digitalWrite(BAT_MONITOR_PW,LOW);
   return aux_volts*3.3*2.0/1023.0;
} 




/* closeI2C() - closes I2C, setting SDA and SCL to '0'
 *
 * This function closes I2C, setting SDA and SCL to '0'
 *
 * Returns nothing
 */
void	WaspPWR::closeI2C()
{
	pinMode(I2C_SDA,OUTPUT);
	pinMode(I2C_SCL,OUTPUT);
	Wire.close();
}

// inits the value of the digipot used in the battery detector
void    WaspPWR::setLowBatteryThreshold(float threshold)
{
	uint8_t dig=0;
	uint8_t rpot=200;
	dig = uint8_t (rpot-(((threshold-1.15)*470/1.15)-806))*255/rpot;
	if( !Wire.I2C_ON ) Wire.begin();
	delay(200);
	Wire.beginTransmission(0x2d);    // Address
	Wire.send(0x11);                 // Write command
	Wire.send(dig);                 // Data
	Wire.endTransmission();
	if( Wire.I2C_ON ) closeI2C();
} 

// checks if Hibernate has generated the reset
void	WaspPWR::ifHibernate()
{
	if( digitalRead(RTC_INT_PIN_MON) && (Utils.readEEPROM(HIB_ADDR)==HIB_VALUE) )
	{
		intFlag |= HIB_INT;
	}
	Utils.writeEEPROM(HIB_ADDR,0);
	if( !(intFlag & HIB_INT) )
	{
		pinMode(RST_RTC, OUTPUT);
		digitalWrite(RST_RTC, LOW);
		delay(10);
		digitalWrite(RST_RTC, HIGH);
	}
	if( !(intFlag & HIB_INT) )
	{
		Utils.setLED(LED0,LED_ON);
		delay(1000);
		Utils.setLED(LED0,LED_OFF);
	}
}

// reboots waspmote
void    WaspPWR::reboot()
{
    __asm__("jmp 0x1E000");
}

// Private Methods /////////////////////////////////////////////////////////////

// Preinstantiate Objects //////////////////////////////////////////////////////

WaspPWR PWR = WaspPWR();

