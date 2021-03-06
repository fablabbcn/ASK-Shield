/*! \file WaspSensorEvent.h
    \brief Library for managing the Prototyping Sensor Board
    
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
 
/*! \def WaspSensorPrototyping_h
    \brief The library flag
    
 */
#ifndef WaspSensorPrototyping_h
#define WaspSensorPrototyping_h


/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <inttypes.h>


/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def SENS_RELAY_ON
    \brief Sensor Board Relay Power Mode. ON in this case
    
 */
/*! \def SENS_RELAY_OFF
    \brief Sensor Board Relay Power Mode. OFF in this case
    
 */
#define	SENS_RELAY_ON	0
#define	SENS_RELAY_OFF	1

/*! \def RELAY_PIN
    \brief Digital pin used for managing the relay
    
 */
#define	RELAY_PIN	DIGITAL1

/******************************************************************************
 * Class
 ******************************************************************************/
 
//! WaspSensorPrototyping Class
/*!
	WaspSensorPrototyping Class defines all the variables and functions used for managing the Prototyping Sensor Board
 */
class WaspSensorPrototyping
{
	private:
	
	public:
		
		//! class constructor
  		/*!
			It initializes the different digital pins
		\param void
		\return void
	 	*/
		WaspSensorPrototyping();
		
		//! It sets board power mode, setting ON/OFF 3v3 and 5V switches
  		/*!
		\param uint8_t mode : SENS_ON or SENS_OFF
		\return void
	 	*/
		void 	setBoardMode(uint8_t mode);
		
		//! It sets relay power mode, setting ON/OFF its switch
  		/*!
		\param uint8_t mode : SENS_ON or SENS_OFF
		\return void
		 */
		void 	setRelayMode(uint8_t mode);
		
		//! It reads the Analogic-to-Digital Converter
  		/*!
		\param void
		\return the value returned by the conversor
		 */
		float	readADC();
		
		//! It sets the load resistor value
  		/*!
		\param float resistor : the value to set the load resistor to (range [0-100] KOhms)
		\return void
		 */
		void 	setLoadResistor(float resistor);
		
		//! It sets the amplifier gain
  		/*!
		\param float gain : the gain to set the amplifier to (range [1-11])
		\return void
		 */
		void	setAmplifierGain(float gain);
		
		//! It reads an analog sensor and returns its measured value in Volts
  		/*!
		\param uint8_t pin : specifies the analog pin where the sensor is connected
		\return the measured value in Volts (range [0-3.3] Volts)
		 */
		float	readAnalogSensor(uint8_t pin);
};

extern WaspSensorPrototyping SensorProto;

#endif
