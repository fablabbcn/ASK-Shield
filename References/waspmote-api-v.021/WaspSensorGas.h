/*! \file WaspSensorEvent.h
    \brief Library for managing the Gas Sensor Board
    
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
  
    Version:		0.9

    Design:		David Gascón

    Implementation:	Alberto Bielsa, Manuel Calahorra

*/

 /*! \def WaspSensorGas_h
    \brief The library flag
    
  */
#ifndef WaspSensorGas_h
#define WaspSensorGas_h

/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def SENS_ON
    \brief Sensor Board Power Modes. ON in this case
    
 */
/*! \def SENS_OFF
    \brief Sensor Board Power Modes. OFF in this case
    
 */
#define	SENS_ON		0
#define	SENS_OFF	1


/*! \def SENS_3V3
    \brief Sensor Board Types. 3V3 switch in this case
    
 */
/*! \def SENS_5V
    \brief Sensor Board Types. 5V switch in this case
    
 */
#define	SENS_3V3	0
#define	SENS_5V		1


/*! \def SENS_TEMPERATURE
    \brief Sensor types. Temperature sensor in this case
    
 */
/*! \def SENS_HUMIDITY
    \brief Sensor types. Humidity sensor in this case
    
 */
/*! \def SENS_PRESSURE
    \brief Sensor types. Pressure sensor in this case
    
 */
/*! \def SENS_CO2
    \brief Sensor types. CO2 sensor in this case
    
 */
/*! \def SENS_O2
    \brief Sensor types. O2 sensor in this case
    
 */
/*! \def SENS_SOCKET2A
    \brief Sensor types. Socket2A sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET2B
    \brief Sensor types. Socket2A sensor in this case (NO2, O3 or VOC sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET3A
    \brief Sensor types. Socket3A sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET3B
    \brief Sensor types. Socket3B sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET3C
    \brief Sensor types. Socket3C sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET4A
    \brief Sensor types. Socket4A sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET4B
    \brief Sensor types. Socket4B sensor in this case (many sensors can be connected to this socket)
    
 */
/*! \def SENS_SOCKET4C
    \brief Sensor types. Socket4C sensor in this case (many sensors can be connected to this socket)
    
 */
#define	SENS_TEMPERATURE	0
#define	SENS_HUMIDITY		1
#define	SENS_PRESSURE		2
#define	SENS_CO2		4
#define	SENS_O2			8
#define	SENS_SOCKET2A		16
#define	SENS_SOCKET2B		32
#define	SENS_SOCKET3A		64
#define	SENS_SOCKET3B		128
#define	SENS_SOCKET3C		256
#define	SENS_SOCKET4A		512
#define	SENS_SOCKET4B		1024
#define	SENS_SOCKET4C		2048


/*! \def SENS_AMPLI1
    \brief Sensor amplifiers and resistors. Amplifier1 in this case
    
 */
/*! \def SENS_AMPLI2
    \brief Sensor amplifiers and resistors. Amplifier2 in this case
    
 */
/*! \def SENS_AMPLI3
    \brief Sensor amplifiers and resistors. Amplifier3 in this case
    
 */
/*! \def SENS_AMPLI4
    \brief Sensor amplifiers and resistors. Amplifier4 in this case
    
 */
/*! \def SENS_R1
    \brief Sensor amplifiers and resistors. Resistor1 in this case
    
 */
/*! \def SENS_R2
    \brief Sensor amplifiers and resistors. Resistor2 in this case
    
 */
/*! \def SENS_R3
    \brief Sensor amplifiers and resistors. Resistor3 in this case
    
 */
#define	SENS_AMPLI1	0
#define	SENS_AMPLI2	1
#define	SENS_AMPLI3	2
#define	SENS_AMPLI4	4
#define	SENS_R1		8
#define	SENS_R2		16
#define	SENS_R3		32

#define DELAY_TIME	500

/******************************************************************************
 * Class
 ******************************************************************************/
 
//! WaspSensorGas Class
/*!
	WaspSensorGas Class defines all the variables and functions used for managing the Gas Sensor Board
 */
class WaspSensorGas
{
	private:
    	
	//! It configures the sensor resistors
  	/*!
	\param uint8_t ampli : the amplifier to configure
	\param float resistor : the value to set the resistor to
	\return void
	 */
	void configureResistor(uint8_t ampli, float resistor);
	
	//! It sets the resistor value
  	/*!
	\param uint8_t address : the resistor address
	\param float value : the value to set the resistor to
	\return void
	 */
	void setResistor(uint8_t address, float value);
	
	//! It configures the corresponding amplifier	
  	/*!
	\param uint8_t ampli : the amplifier to set
	\param uint8_t gain : the gain to set
	\return void
	 */
	void configureAmplifier(uint8_t ampli, uint8_t gain);
	
	//! It sets the value of the gain
  	/*!
	\param uint8_t address : the amplifier address
	\param uint8_t value : the value to set the amplifier to
	\return void
	 */
	void setAmplifier(uint8_t address, uint8_t value);
	
	//! It sends a pulse to read the sensor value
  	/*!
	\param uint16_t sensor : the sensor to read the value from
	\return the value returned by the sensor
	 */
	uint16_t pulse(uint16_t sensor);
	
	public:
	
	//! class constructor
  	/*!
		It initializes the different digital pins
	\param void
	\return void
	 */
	WaspSensorGas();
	
	//! It sets board power mode, setting ON/OFF 3v3 and 5V switches
  	/*!
	\param uint8_t mode : SENS_ON or SENS_OFF
	\return void
	 */
	void setBoardMode(uint8_t mode);
	
	//! It configures the amplifier gain in the different sensors
  	/*!
	\param uint16_t sensor : the sensor to configure 
	\param uint8_t gain : the gain to set (range [1-101])
	\return void
	 */
	void configureSensor(uint16_t sensor, uint8_t gain);
	
	//! It configures the amplifier gain and load resistor in the different sensors
  	/*!
	\param uint16_t sensor : the sensor to configure
	\param uint8_t gain : the gain to set (range [1-101])
	\param float resistor : the load resistor to set (range [0-100] in KOhms)
	\return void
	 */
	void configureSensor(uint16_t sensor, uint8_t gain, float resistor);
	
	//! It sets ON/OFF the different sensor switches
  	/*!
	\param uint8_t mode : SENS_ON or SENS_OFF
	\param uint16_t sensor : the sensor to set ON/OFF
	\return void
	 */
	void setSensorMode(uint8_t mode, uint16_t sensor);
	
	//! It reads the value measured by the sensor
  	/*!
	\param uint16_t sensor : the sensor to read the value from
	\return the value measured by the sensor (range [0-3.3] Volts)
	 */
	float readValue(uint16_t sensor);
};

extern WaspSensorGas SensorGas;

#endif

