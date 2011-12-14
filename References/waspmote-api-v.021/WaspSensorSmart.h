/*! \file WaspSensorSmart.h
    \brief Library for managing the Smart Metering Sensor Board
    
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

    Implementation:	Alberto Bielsa, Manuel Calahorra

 */


 /*! \def WaspSensorSmart_h
    \brief The library flag
    
  */
#ifndef WaspSensorSmart_h
#define WaspSensorSmart_h

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/

/*! \def SENS_SMART_EFERGY
    \brief Sensor types. Efergy sensor
    
 */
/*! \def SENS_SMART_LCELLS
    \brief Sensor types. Load Cells sensor
    
 */
/*! \def SENS_SMART_LIQUID1
    \brief Sensor types. Liquid sensor 1
    
 */
/*! \def SENS_SMART_LIQUID2
    \brief Sensor types. Liquid sensor 2
    
 */
/*! \def SENS_SMART_LIQUID3
    \brief Sensor types. Liquid sensor 3
    
 */
/*! \def SENS_SMART_LDR
    \brief Sensor types. LDR sensor
    
 */
/*! \def SENS_SMART_TEMPERATURE
    \brief Sensor types. Temperature sensor
    
 */
/*! \def SENS_SMART_HUMIDITY
    \brief Sensor types. Humidity sensor
    
 */
/*! \def SENS_SMART_4mA20_1
    \brief Sensor types. 4-20mA sensor 1
    
 */
/*! \def SENS_SMART_4mA20_2
    \brief Sensor types. 4-20mA sensor 2
    
 */
/*! \def SENS_SMART_3V3_1
    \brief Sensor types. 3,3V sensor 1
    
 */
/*! \def SENS_SMART_3V3_2
    \brief Sensor types. 3,3V sensor 2
    
 */
/*! \def SENS_SMART_5V_1
    \brief Sensor types. 5V sensor 1
    
 */
/*! \def SENS_SMART_5V_2
    \brief Sensor types. 5V sensor 2
    
 */
/*! \def SENS_SMART_24V_CONVERTER
    \brief Sensor types. 24V Converter
    
 */
#define	SENS_SMART_4mA20_2		1
#define	SENS_SMART_LDR			2
#define	SENS_SMART_LIQUID1		4
#define	SENS_SMART_FV_CONVERTER		8
#define	SENS_SMART_LCELLS		16
#define	SENS_SMART_EFERGY		32
#define	SENS_SMART_LIQUID2		64
#define	SENS_SMART_LIQUID3		128
#define	SENS_SMART_TEMPERATURE		256
#define	SENS_SMART_HUMIDITY		512
#define	SENS_SMART_4mA20_1		1024
#define	SENS_SMART_3V3_1		2048
#define	SENS_SMART_3V3_2		4096
#define	SENS_SMART_5V_1			8192
#define	SENS_SMART_5V_2			16384
#define	SENS_SMART_24V_CONVERTER	0


/*! \def SENS_SMART_LCELL_IN_USE
    \brief Specifies the load cell in use (0:5V -- 1:10V)
    
 */
#define	SENS_SMART_LCELL_IN_USE		1

/******************************************************************************
 * Class
 ******************************************************************************/
 
 //! WaspSensorSmart Class
/*!
	WaspSensorSmart Class defines all the variables and functions used for managing the Smart Metering Sensor Board
 */
class WaspSensorSmart
{
	private:
    
	//! It sets the Digipot0 threshold
  	/*!
	\param uint8_t address : digipo0 address
	\param uint8_t value : threshold
	\return void
   	*/
	void setDigipot0(uint8_t address, float value);
	
	//! It sets the Digipot1 threshold
  	/*!
	\param uint8_t address : digipo1 address
	\param uint8_t value : threshold
	\return void
	 */
	void setDigipot1(uint8_t address, float value);
	
	//! It converts the data read from the ADC to a voltage level
  	/*!
	\param byte data_input : the data read from the ADC
	\return the voltage level
	 */
	float conversion(byte data_input[3]);

	//! It reads from the ADC
  	/*!
	\return the value read from the ADC
	 */
	float readValue_int();
	
	//! It configures the ADC to read the desired channel
  	/*!
	\param uint16_t sensor : the sensor to read from
	\return void
	 */
	void configureI2C(uint16_t sensor);

	public:

	//! Variable : specifies the sensor that has generated the interruption
  	/*!
	It specifies the socket that has generated the interruption, activating the corresponding flag position
   	*/
	uint8_t	intFlag;

	//! class constructor
  	/*!
		It initializes the different digital pins
	\param void
	\return void
	 */
	WaspSensorSmart();

	//! It sets ON/OFF the 3V3/5V switches
  	/*!
	\param uint8_t mode : SENS_ON or SENS_OFF
	\return void
	 */
	void 	setBoardMode(uint8_t mode);
	
	
	//! It sets ON/OFF the different sensor switches
  	/*!
	\param uint8_t mode : SENS_ON or SENS_OFF
	\param uint16_t sensor : the sensor to set ON/OFF
	\return void
	 */
	void setSensorMode(uint8_t mode, uint16_t sensor);
	
	//! It sets threshold configuring digipots
  	/*!
	\param uint16_t sensor : specifies the socket to set the threshold to ('SENS_SMART_X')
	\param float threshold : the threshold to set (0-3.3V)
	\return void
	 */
	void	setThreshold(uint16_t sensor, float threshold);
	
	//! It sets the resistor value
  	/*!
	\param float value : the value to set the resistor to
	\return void
	 */
	void setLoadResistor(float value);
	
	//! It sets the value of the gain
  	/*!
	\param uint8_t value : the value to set the amplifier to
	\return void
	 */
	void setAmplifierGain(uint16_t value);
	
	
	//! It reads the value measured by the sensor
  	/*!
	\param uint16_t sensor : specifies the socket to set the threshold to ('SENS_SMART_X')
	\return the value read 
	 */
	float 	readValue(uint16_t sensor);
	
	//! It attaches the interruption
  	/*!
	\param void
	\return void
	 */
	void	attachInt();
	
	//! It detaches the interruption
  	/*!
	\param void
	\return void
	 */
	void	detachInt();
	
	//! It loads the parallel shift register to check the sensor that has generated the interruption
  	/*!
	\param void
	\return the flag to check the sensor that has generated the interruption
	 */
	uint8_t	loadInt();
};

extern WaspSensorSmart SensorSmart;

#endif

