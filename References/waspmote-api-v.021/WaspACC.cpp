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
 *  Implementation:	David Cuartielles, Alberto Bielsa
 */
  

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif

// Variables ///////////////////////////////////////////////////////////////////

// Constructors ////////////////////////////////////////////////////////////////

WaspACC::WaspACC()
{
    // nothing to do
}

// Public Methods //////////////////////////////////////////////////////////////

/*******************************************************************************
 * QUICK BOOTING AND CONFIGURATION OF THE SENSOR
 *******************************************************************************/

/*
 * ON (void) - It opens I2C bus and powers the accelerometer
 *
 *  It opens I2C bus and powers the accelerometer
 */
void WaspACC::ON(void)
{
	begin();
	setMode(ACC_ON);
}


/*
 * begin (void) - initialize the accelerometer
 *
 * It initializes communication with I2C bus
 * it makes no tests and checks nothing from the communication, therefore
 * returns nothing and modifies no flags
 */
void WaspACC::begin(void)
{
	// Sets RTC on to enable I2C
	if( !RTC.isON) RTC.setMode(RTC_ON, RTC_I2C_MODE);
	
  accMode = ACC_ON;	    // by default the accelerometer will be in continuous mode
                            // this should be changed once Wasp is released, by default
                            // the system should be in standby, and with the communication 
                            // disabled

  if( !Wire.I2C_ON ) Wire.begin(); // join i2c bus (address optional for master)
}

/*
 * boot (void) - reboot the accelerometer
 *
 * takes for granted the accelerometer is on and forces the sensor to reboot
 * by writing to the control register #2 into the right pin
 * it makes no tests and checks nothing from the communication, therefore
 * returns nothing and modifies no flags
 */
void WaspACC::boot(void)
{
  // send reboot command
  writeRegister(ctrlReg2, B00010000);

  delay(1);  // make a short pause after rebooting

  writeRegister(ctrlReg2, B00000000);

}

/*
 * close (void) - closes I2C bus
 *
 * It closes I2C bus. Accelerometer uses I2C to communicate with microcontroller.
 *
 * It is used to keep the battery to the maximum
 */
void WaspACC::close(void)
{
	setMode(ACC_HIBERNATE);	
	if( Wire.I2C_ON && RTC.isON!=1){
		 PWR.closeI2C();
		 RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}

/*******************************************************************************
 * CHECK ACCELEROMETER'S STATUS
 *******************************************************************************/

/*
 * getStatus (void) - gets the accelerometer status
 *
 * returns a byte containing the status of the accelerometer as read from
 * the proper register. It won't activate any flags by itself, but activates
 * ACC_COMM_ERROR in case there was an error writing to the register
 */
uint8_t WaspACC::getStatus(void)
{
  return (uint8_t) readRegister(statusReg);
}

/*
 * getWhoAmI (void) - checks if there is an accelerometer on board
 *
 * sends the accelerometer dummy register on 0x0F that should always answer 0x3A
 *
 * This function can be used to determine if the accelerometer is on the board
 * but also to check if it is still working properly. Any answer other than
 * 0x3A means there is an error
 *
 * since it calls readRegister, getWhoAmI won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::check(void)
{
  return (uint8_t) readRegister(whoAmIReg);
}

/*******************************************************************************
 * HANDLE THE CONFIGURATION REGISTERS, BOTH READ AND WRITE
 *******************************************************************************/

/*
 * getCTRL1 (void) - checks accelerometer's control register 1
 *
 * returns the contents of control register 1
 *
 * since it calls readRegister, getCTRL1 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::getCTRL1(void)
{
  return (uint8_t) readRegister(ctrlReg1);
}

/*
 * setCTRL1 (void) - sets accelerometer's control register 1
 *
 * sets the contents of control register 1, returns 1 if error
 *
 * since it calls writeRegister, setCTRL1 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setCTRL1(uint8_t val)
{
  return (uint8_t) writeRegister(ctrlReg1, val);
}

/*
 * getCTRL2 (void) - checks accelerometer's control register 2
 *
 * returns the contents of control register 2
 *
 * since it calls readRegister, getCTRL2 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::getCTRL2(void)
{
  return (uint8_t) readRegister(ctrlReg2);
}

/*
 * setCTRL2 (void) - sets accelerometer's control register 2
 *
 * sets the contents of control register 2, returns 1 if error
 *
 * since it calls writeRegister, setCTRL2 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setCTRL2(uint8_t val)
{
  return (uint8_t) writeRegister(ctrlReg2, val);
}

/*
 * getCTRL3 (void) - checks accelerometer's control register 3
 *
 * returns the contents of control register 3
 *
 * since it calls readRegister, getCTRL3 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::getCTRL3(void)
{
  return (uint8_t) readRegister(ctrlReg3);
}

/*
 * setCTRL3 (void) - sets accelerometer's control register 3
 *
 * sets the contents of control register 3, returns 1 if error
 *
 * since it calls writeRegister, setCTRL3 won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setCTRL3(uint8_t val)
{
  return (uint8_t) writeRegister(ctrlReg3, val);
}

/*
 * getADCmode (void) - gets the accelerometer's ADC mode
 *
 * returns '0' if success, '-1' if error
 *
 * since it calls readRegister, getADCmode won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::getADCmode(void)
{
  return readRegister(ctrlReg2) && B01;
}

/*
 * setADCmode (bit) - sets the accelerometer's ADC mode
 *
 * writes, inside the CTRL2 whether the ADC should operate in 12 or 16bits. Values
 * are zero (0) for 12 bits right aligned (MSB represents the sign) and one (1) 
 * for 16 bits left aligned
 *
 * since it calls writeRegister, setADCmode won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setADCmode(uint8_t val)
{
  uint8_t aux = readRegister(ctrlReg2);
  aux &= B11111110;
  aux |= val;
  return writeRegister(ctrlReg2, aux);
}


/*
 * setSamplingRate (rate) - sets the accelerometer's sampling rate
 *
 * writes, inside the CTRL1 the samplin rate desired
 *
 * since it calls writeRegister, setADCmode won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setSamplingRate(uint8_t rate)
{
	uint8_t aux = readRegister(ctrlReg1);
	aux &= B11001111;
	
	switch( rate )
	{
		case ACC_RATE_40 	:	aux |= rate;
						break;
		case ACC_RATE_160	:	aux |= B00010000;
						break;
		case ACC_RATE_640 	:	aux |= B00100000;
						break;
		case ACC_RATE_2560 	:	aux |= B00110000;
						break;
	}
	return writeRegister(ctrlReg1, aux);
}


/*******************************************************************************
 * GET ACCELERATION ON THE THREE AXIS
 *******************************************************************************/

/*
 * getX (void) - checks accelerometer's acceleration on OX 
 *
 * returns the combined contents of data registers outXhigh and outXlow as an
 * integer according to ADC's configuration
 *
 * since it calls readRegister, getX won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
int16_t WaspACC::getX(void)
{
  int16_t aux = ((int8_t)readRegister(outXhigh)*256) + (uint8_t)readRegister(outXlow);  
  return aux;
}

/*
 * getY (void) - checks accelerometer's acceleration on OY 
 *
 * returns the combined contents of data registers outYhigh and outYlow as an
 * integer according to ADC's configuration
 *
 * since it calls readRegister, getY won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
int16_t WaspACC::getY(void)
{
  int16_t aux = ((int8_t)readRegister(outYhigh)*256) + (uint8_t)readRegister(outYlow);
  return aux;
}

/*
 * getZ (void) - checks accelerometer's acceleration on OZ
 *
 * returns the combined contents of data registers outZhigh and outZlow as an
 * integer according to ADC's configuration
 *
 * since it calls readRegister, getZ won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
int16_t WaspACC::getZ(void)
{
  int16_t aux = ((int8_t)readRegister(outZhigh)*256) + (uint8_t)readRegister(outZlow);
  return aux;
}

/*******************************************************************************
 * HANDLE ACCELEROMETER'S WORK MODES                                           *
 *******************************************************************************/

/*
 * getMode (void) - checks accelerometer's work mode
 *
 * returns the value for the accelerometer's work mode, the possibilites are:
 * - ACC_INT: wake up the processor on preprogammed interrupt (free fall or deviation) 
 * - ACC_CONTINUOUS: read/send at a constant pace upon request
 * - ACC_CUSTOM: user defined work mode
 *
 * It doesn't call any other functions and therefore it won't activate any flags
 */
uint8_t WaspACC::getMode()
{
  return accMode;
}

/*
 * setMode (uint8_t) - sets accelerometer's work mode
 *
 * configures the accelerometer to a new work mode, the possibilites are:
 * - ACC_HIBERNATE: go on the lowest power mode possible. This mode has no interrupt
 *              calls and therefore won't disturb the processor if anything happened
 * - ACC_ON: read/send at a constant pace upon request
 * - ACC_CUSTOM: user defined work mode, should be programmed in here
 *
 * since it calls writeRegister, setAccMode won't activate any flags by itself, 
 * but the other functions will activate ACC_COMM_ERROR in case there 
 * was an error communicating to the register
 */
uint8_t WaspACC::setMode(uint8_t mode)
{
  accMode = mode;

  if (mode == ACC_HIBERNATE)
  {
	writeRegister(ctrlReg2,B00000000); // de-activate RDY 
	writeRegister(ctrlReg1,B00000111); // turn power off 
	isON = 0;
  }

  if (mode == ACC_ON)
  {
	writeRegister(ctrlReg2, B00000000);
        writeRegister(ctrlReg1, B01000111); // turn power ON
	writeRegister(ctrlReg3, B00001000);
	isON = 1;
  }

  if (mode == ACC_CUSTOM)
  {
	// here the custom mode has to be defined by expert users
  }

  // go out with no error, leaves room to implement an error function
  return 0;
}

/*******************************************************************************
 * SPECIFIC WASP FUNCTIONS TO HANDLE EVENTS
 *******************************************************************************/

/*
 * setFF (void) - sets the Free Fall interrupt
 */
uint8_t WaspACC::setFF(void)
{
  // clear the FF interrupt if active
  unsetFF();
   
  // configure the different registers to 
  // handle acceleration detection on the X, Y, or Z axis
  ACC.writeRegister(ctrlReg1, 0xC7);
  ACC.writeRegister(ctrlReg2, 0x08); 
  ACC.writeRegister(ctrlReg3, 0x08);
  
 // here we have to double-check if we
  // are in 12 or 16bit data modes. The 12
  // bit data mode is giving away the 4LSBs
  // therefore the register needs to be written
  // according to that. 0x3C00 is just below 1g
  // but the acc will give 0x03C0, beware!
  writeRegister(FF_WU_THS_L, FF_WU_THS_L_val);  // threshold L
  writeRegister(FF_WU_THS_H, FF_WU_THS_H_val);  // threshold H
  writeRegister(FF_WU_DURATION, FF_WU_DURATION_val);  // event duration
  // readRegister(HP_FILTER_RESET);   // config the acc. to get current acceleration as initial val
  writeRegister(FF_WU_CFG, FF_WU_CFG_val);  // config accelerometer to wake up 

  // assign the ACC_FREE_FALL mode
  AccEventMode |= ACC_FREE_FALL;

  // attach the hardware interrupt to the pin
  attachInt();

  return flag;
}

/*
 * unsetFF (void) - clears and unsets the Free Fall interrupt
 */
uint8_t WaspACC::unsetFF(void)
{

  writeRegister(FF_WU_CFG, 0x00);  
  readRegister(FF_WU_ACK);  // clear the interrupt flag

  // detach the event if there are no flags active
  AccEventMode &= ~(ACC_FREE_FALL);
  if (!AccEventMode) detachInt();
  return flag;
}

/*
 * setDD (uint8_t) - sets the Direction Change interrupt
 */
uint8_t WaspACC::setDD(uint8_t directions)
{
  // make sure it gets configured for whatever direction
  // people choose like e.g. XHIE or ZLIE
  directions |= (IEND | LIR);

  // clear the DD interrupt if active
  unsetDD();
   
  // configure the different registers to 
  // handle acceleration detection on the X, Y, or Z axis
  writeRegister(ctrlReg1, 0xC7);
  writeRegister(ctrlReg2, 0x08); 
  writeRegister(ctrlReg3, 0x4B);
  
  // here we have to double-check if we
  // are in 12 or 16bit data modes. The 12
  // bit data mode is giving away the 4LSBs
  // therefore the register needs to be written
  // according to that. 0x3C00 is just below 1g
  // but the acc will give 0x03C0, beware!
  writeRegister(DD_THSI_L, DD_THSI_L_val);  // threshold L internal
  writeRegister(DD_THSI_H, DD_THSI_H_val);  // threshold H internal
  writeRegister(DD_THSE_L, DD_THSE_L_val);  // threshold L external
  writeRegister(DD_THSE_H, DD_THSE_H_val);  // threshold H external
  readRegister(HP_FILTER_RESET);   // config the acc. to get current acceleration as initial val
  writeRegister(DD_CFG, directions);  // config accelerometer to wake up 

  // assign the ACC_FREE_FALL mode
  AccEventMode |= ACC_DIRECTION;

  // attach the hardware interrupt to the pin
  attachInt();

  return flag;
}

/*
 * unsetDD (void) - clears and unsets the Direction Change interrupt
 */
uint8_t WaspACC::unsetDD(void)
{

  writeRegister(DD_CFG, 0x00);
  readRegister(DD_ACK);  // clear the interrupt flag

  // detach the event if there are no flags active
  AccEventMode &= ~(ACC_DIRECTION);
  if (!AccEventMode) detachInt();
  return flag;
}

/*
 * getAccEvent (void) - returns the event type on the accelerometer
 *
 * will be zero if there are no values set, bigger otherwise
 *
 * It doesn't call any other functions and therefore it won't activate any flags
 */
uint8_t WaspACC::getAccEvent(void)
{
  return AccEventMode;
}

/*
 * setAccEvent (mode) - sets the event type on the accelerometer
 *
 * It will set up the type of event, they are non exclusive and could be:
 *
 * - ACC_NONE
 * - ACC_FREE_FALL
 * - ACC_DIRECTION
 * - ACC_THRESHOLD
 */
uint8_t WaspACC::setAccEvent(uint8_t mode)
{
  AccEventMode = mode;

  if (mode & ACC_DIRECTION)
  {
  }

  if (mode == ACC_FREE_FALL)
  {
  }

  if (mode == ACC_NONE)
  {
  }

  // go out with no error, leaves room to implement an error function
  return 0;
}

/*******************************************************************************
 * HANDLING HARDWARE INTERRUPTS
 *******************************************************************************/

/*
 * attachInterrupt(void) - configure the specific hardware interrupt for the acc
 *
 * the default interrupt functions are defined inside WInterrupts.c
 */
void WaspACC::attachInt(void)
{
	enableInterrupts(ACC_INT);
}

/*
 * detachInterrupt(void) - unset the specific hardware interrupt for the acc
 */
void WaspACC::detachInt(void)
{
  disableInterrupts(ACC_INT);
}


/*
 * clearAlarmFlag() - clears the Accelerometer's alarm flag
 */
void WaspACC::clearAlarmFlag(void)
{
	writeRegister(FF_WU_CFG, 0x00);  
	readRegister(FF_WU_ACK);  // clear the interrupt flag
	
	writeRegister(DD_CFG, 0x00);
	readRegister(DD_ACK);  // clear the interrupt flag
}

/*
void attachInterrupt();
void detachInterrupt();
*/

/*******************************************************************************
 * LOW LEVEL READ AND WRITE REGISTERS
 *******************************************************************************/

// reads a register from the accelerometer
// returns its value or -1 if error
int16_t WaspACC::readRegister(uint8_t regNum) 
{
  // reset the flag
  flag &= ~(ACC_ERROR_READING);

  uint8_t aux = 0;
  Wire.beginTransmission(i2cID);
  Wire.send(regNum);
  Wire.endTransmission();

  Wire.requestFrom(i2cID, 1);
  if(Wire.available())
  {
    aux = Wire.receive();
    return aux;
  }

  // error, activate the reading flag
  flag |= ACC_ERROR_READING;

  return -1;
}

// writes a byte to a register in the accelerometer
// returns 0 or -1 if error
int16_t WaspACC::writeRegister(uint8_t address, uint8_t val)
{
  // reset the flag
  flag &= ~(ACC_ERROR_WRITING);

  Wire.beginTransmission(i2cID);
  Wire.send(address); 
  Wire.send(val);
  Wire.endTransmission();

  if (readRegister(address) != val)
  {
    // error, activate the reading flag
    flag |= ACC_ERROR_WRITING;

    return -1;
  }

  // exit with no error
  return 0;
}

// Private Methods /////////////////////////////////////////////////////////////

// Preinstantiate Objects //////////////////////////////////////////////////////

WaspACC ACC = WaspACC();

