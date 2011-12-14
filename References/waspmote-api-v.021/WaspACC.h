/*! \file WaspACC.h
    \brief Library for managing the accelerometer LIS3LV02DL
    
    Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
    Version:		0.8

    Design:		David Gascón

    Implementation:	David Cuartielles, Alberto Bielsa

*/
  
 
/*! \def WaspACC_h
    \brief The library flag
    
 */
#ifndef WaspACC_h
#define WaspACC_h

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/

/*! \def i2cID
    \brief sensor's address on the I2C port
 */
#define i2cID 0x1D

/*! \def cbi
    \brief Function definition to set a register bit to '0'
    
    'sfr' is the register. 'bit' is the register bit to change
 */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

/*! \def sbi
    \brief Function definition to set a register bit to '1'
    
    'sfr' is the register. 'bit' is the register bit to change
 */
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/*! \def outXlow
    \brief X axis acceleration register lower bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outXlow 	0x28

/*! \def outXhigh
    \brief X axis acceleration register higher bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outXhigh 	0x29

/*! \def outYlow
    \brief Y axis acceleration register lower bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outYlow 	0x2A

/*! \def outYhigh
    \brief Y axis acceleration register higher bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outYhigh 	0x2B

/*! \def outZlow
    \brief Z axis acceleration register lower bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outZlow 	0x2C

/*! \def outZhigh
    \brief Z axis acceleration register higher bits address
    
    Data is stored either as signed 12-bit or unsigned 16-bit numbers
 */
#define outZhigh 	0x2D

/*! \def ctrlReg1
    \brief Control Register 1 Address
    
    This register contains configuration parameters as powering accelerometer or enabling the different axis
 */
#define ctrlReg1 	0x20

/*! \def ctrlReg2
    \brief Control Register 2 Address
    
    This register contains configuration parameters as enabling interruptions or data type
 */
#define ctrlReg2 	0x21

/*! \def ctrlReg3
    \brief Control Register 3 Address 
    
    This register contains configuration parameters related with high-pass filter
 */
#define ctrlReg3 	0x22

/*! \def HP_FILTER_RESET
    \brief High Pass Filter Register Address
    
    Dummy register. Reading at this address zeroes instantaneously the content of the internal high pass-filter. Read data is not significant.
 */
#define HP_FILTER_RESET 0x23

/*! \def statusReg
    \brief Status Register Address
    
    This register contains '0' if accelerometer is stopped and '1' if it is running
 */
#define statusReg 	0x27

/*! \def whoAmIReg
    \brief Who Am I Register Address
    
    This register will contain '0x3A' if accelerometer is properly working. Other value means a bad accelerometer behavior
 */
#define whoAmIReg 	0x0F


/*! \def FF_WU_CFG
    \brief Free Fall Interrupt Configuration Register
    
    This register specifies the axis enabled to generate an interruption
 */
#define FF_WU_CFG 	0x30

/*! \def FF_WU_SRC
    \brief Free Fall Interrupt Source Register
    
    This register specifies the axis in which the interruption was generated
 */
#define FF_WU_SRC 	0x31

/*! \def FF_WU_ACK
    \brief Free Fall Interrupt ACK Register
    
    Dummy register. If LIR bit in FF_WU_CFG register is set to ‘1’, a reading at this address allows the FF_WU_SRC register refresh. Read data is not significant.
 */
#define FF_WU_ACK 	0x32

/*! \def FF_WU_THS_L
    \brief Free Fall Interrupt Threshold LSB
    
    This register specifies the threshold less significant bits
 */
#define FF_WU_THS_L 	0x34

/*! \def FF_WU_THS_H
    \brief Free Fall Interrupt Threshold MSB
    
    This register specifies the threshold most significant bits
 */
#define FF_WU_THS_H 	0x35

/*! \def FF_WU_DURATION
    \brief Free Fall Interrupt Duration
    
    This register sets the minimum duration of the free-fall/wake-up event to be recognized.
 */
#define FF_WU_DURATION 	0x36

/*! \def DD_CFG
    \brief Direction Change Interrupt Configuration Register
    
    This register specifies the axis enabled to generate an interruption
 */
#define DD_CFG		0x38

/*! \def DD_SRC
    \brief Direction Change Interrupt Source Register
    
    This register specifies the axis in which the interruption was generated
 */
#define DD_SRC		0x39


/*! \def DD_ACK
    \brief Direction Change Interrupt ACK Register
    
     Dummy register. If LIR bit in DD_CFG register is set to ‘1’, a reading at this address allows the DD_SRC register refresh. Read data is not significant.
 */
#define DD_ACK		0x3A

/*! \def DD_THSI_L
    \brief Direction Change Interrupt Internal Threshold LSB
    
    This register specifies the internal threshold less significant bits
 */
#define DD_THSI_L	0x3C

/*! \def DD_THSI_H
    \brief Direction Change Interrupt Internal Threshold MSB
    
    This register specifies the internal threshold most significant bits
 */
#define DD_THSI_H	0x3D

/*! \def DD_THSE_L
    \brief Direction Change Interrupt External Threshold LSB
    
    This register specifies the external threshold less significant bits
 */
#define DD_THSE_L	0x3E

/*! \def DD_THSE_H
    \brief Direction Change Interrupt External Threshold MSB
    
    This register specifies the external threshold most significant bits
 */
#define DD_THSE_H	0x3F

/*! \def AOI
    \brief AND/OR Combination of interrupt events
    
    This bit is contained in FF_WU_CFG. '1' is AND combination, '0' is OR combination
 */
#define AOI   128

/*! \def IEND
    \brief Direction Change Interrupt Enable
    
    This bit is contained in DD_CFG. '1' enables interruptions, '0' disables interruptions
 */
#define IEND  128

/*! \def LIR
    \brief Latch Interrupt
    
    This bit is contained in DD_CFG and FF_WU_CFG. '1' interrupt request latched, '0' interrupt request not latched
 */
#define LIR   64

/*! \def IA
    \brief Interrupt Active
    
    This bit is contained in DD_SRC and FF_WU_SRC. '1' indicates interrupts have been generated, '0' indicates no interruptions have been generated
 */
#define IA    64

/*! \def ZHIE
    \brief Z High event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables Z High event interruption
 */
#define ZHIE  32

/*! \def ZLIE
    \brief Z Low event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables Z Low event interruption
 */
#define ZLIE  16

/*! \def YHIE
    \brief Y High event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables Y High event interruption
 */
#define YHIE  8

/*! \def YLIE
    \brief Y Low event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables Y Low event interruption
 */
#define YLIE  4

/*! \def XHIE
    \brief X High event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables X High event interruption
 */
#define XHIE  2

/*! \def XLIE
    \brief X Low event interruption
    
    This bit is contained in FF_WU_CFG. '1' enables X Low event interruption
 */
#define XLIE  1


/*! \def FF_WU_CFG_val
    \brief Free Fall Configuration
    
    Free Fall Interruption Configuration, setting interrupt latched and Z,Y,X High event
 */
#define FF_WU_CFG_val       LIR | ZHIE | YHIE | XHIE

/*! \def FF_WU_THS_L_val
    \brief Free Fall Threshold LSB
    
    Free Fall Threshold LSB. '0x3C00' is a good experimental value for 1g. This value should be changed in different applications
 */
#define FF_WU_THS_L_val     0x00

/*! \def FF_WU_THS_H_val
    \brief Free Fall Threshold MSB
    
    Free Fall Threshold MSB. '0x3C00' is a good experimental value for 1g. This value should be changed in different applications
 */
#define FF_WU_THS_H_val     0x4B

/*! \def FF_WU_DURATION_val
    \brief Free Fall Duration
    
    Free Fall Duration. Empirically tested to be a good duration
 */
#define FF_WU_DURATION_val  0x05

/*! \def DD_CFG_val
    \brief Direction Change Configuration
    
    Direction Change Interruption Configuration, setting interrupt enabled,interrupt latched and Y and X High and Low events
 */
#define DD_CFG_val          IEND | LIR | YHIE | YLIE | XHIE | XLIE

/*! \def DD_THSI_L_val
    \brief Direction Change Internal Threshold LSB
    
    Direction Change Internal Threshold LSB.
 */
#define DD_THSI_L_val       0x00

/*! \def DD_THSI_H_val
    \brief Direction Change Internal Threshold MSB
    
    Direction Change Internal Threshold MSB.
 */
#define DD_THSI_H_val       0x30

/*! \def DD_THSE_L_val
    \brief Direction Change External Threshold LSB
    
    Direction Change External Threshold LSB.
 */
#define DD_THSE_L_val       0x00

/*! \def DD_THSE_H_val
    \brief Direction Change External Threshold MSB
    
    Direction Change External Threshold MSB.
 */
#define DD_THSE_H_val       0x3C

/*! \def SIGNED_12_BITS
    \brief Accelerometer ADC modes. Signed 12 bits in this case
    
    This is the DAS bit in the CTRL2 register
 */
#define SIGNED_12_BITS 0

/*! \def UNSIGNED_16_BITS
    \brief Accelerometer ADC modes. Unsigned 16 bits in this case
    
    This is the DAS bit in the CTRL2 register
 */
#define UNSIGNED_16_BITS 1


/*! \def ACC_HIBERNATE
    \brief Accelerometer Operation modes. Hibernate in this case
 */
#define ACC_HIBERNATE 	1

/*! \def ACC_ON
    \brief Accelerometer Operation modes. ON in this case
 */
#define ACC_ON 		2

/*! \def ACC_CUSTOM
    \brief Accelerometer Operation modes. Custom mode in this case
 */
#define ACC_CUSTOM 	4

/*! \def ACC_NONE
    \brief Accelerometer event types. None in this case
    
    These events don't need to be excluding each other. It is possible to monitor continuous movement and free-fall situations at once
 */
#define ACC_NONE 0

/*! \def ACC_FREE_FALL
    \brief Accelerometer event types. Free Fall in this case
    
    These events don't need to be excluding each other. It is possible to monitor continuous movement and free-fall situations at once
 */
#define ACC_FREE_FALL 1

/*! \def ACC_DIRECTION
    \brief Accelerometer event types. Direction Change in this case
    
    These events don't need to be excluding each other. It is possible to monitor continuous movement and free-fall situations at once
 */
#define ACC_DIRECTION 2

/*! \def ACC_THRESHOLD
    \brief Accelerometer event types. Threshold in this case
    
    These events don't need to be excluding each other. It is possible to monitor continuous movement and free-fall situations at once
 */
#define ACC_THRESHOLD 4

/*! \def ACC_ERROR_READING
    \brief Flag values. Error reading register in this case
 */
#define ACC_ERROR_READING 1

/*! \def ACC_ERROR_READING
    \brief Flag values. Error writing register in this case
 */
#define ACC_ERROR_WRITING 2

/*! \def ACC_RATE_40
    \brief 40Hz Sampling Rate
 */
#define ACC_RATE_40	 0

/*! \def ACC_RATE_160
    \brief 160Hz Sampling Rate
 */
#define ACC_RATE_160 	1

/*! \def ACC_RATE_640
    \brief 640Hz Sampling Rate
 */
#define ACC_RATE_640 	2

/*! \def ACC_RATE_2560
    \brief 2560Hz Sampling Rate
 */
#define ACC_RATE_2560 	4

/******************************************************************************
 * Class
 ******************************************************************************/
 
 //! WaspACC Class
/*!
	WaspACC Class defines all the variables and functions used to manage LIS3LV02DL accelerometer
 */
class WaspACC
{
  private:

    //! Variable : Accelerometer Operation Mode
    /*!
	  Possible values are ACC_ON, ACC_HIBERNATE and ACC_CUSTOM
     */ 
    uint8_t accMode;

  public:

    //! class constructor
    /*!
	  It does nothing
	  \param void
	  \return void
     */ 
    WaspACC();
    
    //! Variable : Accelerometer Event Mode
    /*!
    Possible values are ACC_NONE, ACC_FREE_FALL, ACC_DIRECTION, ACC_THRESHOLD
     */ 
    uint8_t AccEventMode;
    
    //! Variable : status flag, used to see if there has been an error while reading/writing or if there has been a hardware interrupt
    /*!
	  Possible values are ACC_ERROR_READING or ACC_ERROR_WRITING
     */ 
    volatile uint8_t flag;
    
    //! Variable : interrupt flag, used to check which interrupt got active
    /*!
    	Its structure is:
                      (MSB)  7   6   5   4   3   2   1   0  (LSB)
                             XX  XX  XX  XX  XX  TH  DD  FF
                      TH - threshold, DD - direction change, FF - free fall
     */ 
    uint8_t flagInt;
    
    //! Variable : It stores if the ACC is ON(1) or OFF(0)
    /*!    
    */
    uint8_t isON;
    
    //! It opens I2C bus and powers the accelerometer
    /*!
    \param void
    \return void
    \sa boot(), close(), begin()
     */ 
    void ON();
    
    //! It opens I2C bus
    /*!
    \param void
    \return void
    \sa boot(), close()
     */ 
    void begin();
    
    //! It reboots the accelerometer
    /*!
    \param void
    \return void
    \sa init(), close()
     */ 
    void boot();
    
    //! It closes I2C bus
    /*!
    \param void
    \return void
    \sa init(), boot()
     */ 
    void close();
    
    //! It gets the accelerometer status
    /*!
    \param void
    \return a byte containing the accelerometer status as read from the proper register. It won't activate any flags by itself, but activates ACC_COMM_ERROR in case there was an error writing to the register
    \sa check()
     */ 
    uint8_t getStatus();
    
    //! It checks if accelerometer is working properly
    /*!
    \param void
    \return a byte containing the register value. If this value is different from '0x3A', the accelerometer is not working properly
    \sa getStatus()
     */
    uint8_t check();

    //! It gets the CTRL1 register
    /*!
    \param void
    \return a byte containing the register value.
    \sa setCTRL1(uint8_t val)
     */
    uint8_t getCTRL1();
    
    //! It sets the CTRL1 register
    /*!
    \param uint8_t val : value to set the CTRL1 register to
    \return '1' if error
    \sa getCTRL1()
     */
    uint8_t setCTRL1(uint8_t val);
    
    //! It gets the CTRL2 register
    /*!
    \param void
    \return a byte containing the register value.
    \sa setCTRL2(uint8_t val)
     */
    uint8_t getCTRL2();
    
    //! It sets the CTRL2 register
    /*!
    \param uint8_t val : value to set the CTRL2 register to
    \return '1' if error
    \sa getCTRL2()
     */
    uint8_t setCTRL2(uint8_t val);
    
    //! It gets the CTRL3 register
    /*!
    \param void
    \return a byte containing the register value.
    \sa setCTRL3(uint8_t val)
     */
    uint8_t getCTRL3();
    
    //! It sets the CTRL3 register
    /*!
    \param uint8_t val : value to set the CTRL3 register to
    \return '1' if error
    \sa getCTRL3()
     */
    uint8_t setCTRL3(uint8_t val);
    
    //! It reads a register from the accelerometer
    /*!
    \param uint8_t address : register address
    \return register value or '-1' if error
    \sa writeRegister(uint8_t address, uint8_t val)
     */
    int16_t readRegister(uint8_t address);
    
    //! It writes a register to the accelerometer
    /*!
    \param uint8_t address : register address
    \param uint8_t val : value to write
    \return '0' on success, '-1' if error
    \sa readRegister(uint8_t val)
     */
    int16_t writeRegister(uint8_t address, uint8_t val);
    
    //! It gets the accelerometer's ADC mode
    /*!
    \param void
    \return '0' for 12 bits right aligned (MSB represents the sign) and '1' for 16 bits left aligned
    \sa setADCmode(uint8_t val)
     */
    uint8_t getADCmode();
    
    //! It sets the accelerometer's ADC mode
    /*!
    \param uint8_t val : '0' for 12 bits right aligned (MSB represents the sign) and '1' for 16 bits left aligned
    \return '0' on success, '-1' if error
    \sa getADCmode()
     */
    uint8_t setADCmode(uint8_t val);
    
    //! It sets the accelerometer's sampling rate
    /*!
    \param uint8_t rate : ACC_RATE_40, ACC_RATE_160, ACC_RATE_640, ACC_RATE_2560
    \return '0' on success, '-1' if error
     */
    uint8_t setSamplingRate(uint8_t rate);
    
    //! It gets the acceleration on OX
    /*!
    \param void
    \return the combined contents of data registers outXhigh and outXlow as an integer according to ADC's configuration
    \sa getY(), getZ()
     */
    int16_t getX();
    
    //! It gets the acceleration on OY
    /*!
    \param void
    \return the combined contents of data registers outYhigh and outYlow as an integer according to ADC's configuration
    \sa getX(), getZ()
     */
    int16_t getY();
    
    //! It gets the acceleration on OZ
    /*!
    \param void
    \return the combined contents of data registers outZhigh and outZlow as an integer according to ADC's configuration
    \sa getX(), getY()
     */
    int16_t getZ();

    //! It sets the Free Fall interrupt using the parameters previously defined
    /*!
    \param void
    \return 'flag' variable
    \sa unsetFF()
     */
    uint8_t setFF(void);
    
    //! It unsets the Free Fall interrupt
    /*!
    \param void
    \return 'flag' variable
    \sa setFF()
     */
    uint8_t unsetFF(void);
    
    //! It sets the Direction Change interrupt using the input parameter
    /*!
    \param uint8_t directions : the axis where enable the interruptions
    \return 'flag' variable
    \sa setDD()
     */
    uint8_t setDD(uint8_t directions);
    
    //! It sets the Direction Change interrupt using the parameters previously defined
    /*!
    \param void
    \return 'flag' variable
    \sa setDD(uint8_t directions)
     */
    uint8_t setDD(void) { setDD(DD_CFG_val); return flag; }
    
    //! It unsets the Direction Change interrupt
    /*!
    \param void
    \return 'flag' variable
    \sa setFF()
     */
    uint8_t unsetDD(void);

    //! It gets the accelerometer's work mode
    /*!
    \param void
    \return 'accMode' variable
    \sa setMode(uint8_t val)
     */
    uint8_t getMode();
    
    //! It sets the accelerometer's work mode
    /*!
    \param uint8_t val : the accelerometer's work mode  (ACC_ON, ACC_HIBERNATE, ACC_CUSTOM)
    \return '0' if no error
    \sa getMode()
     */
    uint8_t setMode(uint8_t val);
    
    //! It gets the accelerometer's event type
    /*!
    \param void
    \return 'AccEventMode' variable 
    \sa setAccEvent(uint8_t val)
     */
    uint8_t getAccEvent();
    
    //! It sets the accelerometer's event type
    /*!
    \param uint8_t val : accelerometer's event type (ACC_NONE, ACC_FREE_FALL, ACC_DIRECTION, ACC_THRESHOLD)
    \return '0' if no error
    \sa getMode()
     */
    uint8_t setAccEvent(uint8_t val);
    
    //! It configures the specific hardware interrupt for the acc
    /*!
    \param void
    \return void
    \sa detachInt()
     */
    void attachInt(void);
    
    //! It unsets the specific hardware interrupt for the acc
    /*!
    \param void
    \return void
    \sa attachInt()
     */
    void detachInt(void);
    
    //! It clears the Accelerometer's alarm flag
    /*!
    \param void
    \return void
     */
    void clearAlarmFlag();
};

extern WaspACC ACC;

#endif

