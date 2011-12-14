/*! \file WaspSensorCities.h
    \brief Library for managing the Smart Cities Sensor Board
    
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


 /*! \def WaspSensorCities_h
    \brief The library flag
    
  */
#ifndef WaspSensorCities_h
#define WaspSensorCities_h

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/

/*! \def SENS_CITIES_DUST
    \brief Sensor types. Dust sensor
    
 */
/*! \def SENS_CITIES_LD
    \brief Sensor types. Linear displacement sensor
    
 */
/*! \def SENS_CITIES_AUDIO
    \brief Sensor types. Microphone
    
 */
/*! \def SENS_CITIES_HUMIDITY
    \brief Sensor types. Humidity sensor
    
 */
/*! \def SENS_CITIES_TEMPERATURE
    \brief Sensor types. Temperature sensor
    
 */
/*! \def SENS_CITIES_LDR
    \brief Sensor types. LDR sensor
    
 */
/*! \def SENS_CITIES_ULTRASOUND
    \brief Sensor types. Ultrasonic sensor
    
 */
/*! \def SENS_CITIES_CD
    \brief Sensor types. Crack detection sensor
    
 */
/*! \def SENS_CITIES_CP
    \brief Sensor types. Crack propagation sensor
    
 */

#define	SENS_CITIES_HUMIDITY		1
#define	SENS_CITIES_DUST		8
#define	SENS_CITIES_AUDIO		16
#define	SENS_CITIES_LDR			32
#define	SENS_CITIES_LD			64
#define	SENS_CITIES_TEMPERATURE		128
#define SENS_CITIES_ULTRASOUND		256
#define SENS_CITIES_CD			512
#define SENS_CITIES_CP			1024

/*! \def SENS_ULSTRASOUND_TYPE
    \brief Specifies the ultrasound sensor in use (0:XL -- 1:LV)
    
 */
#define SENS_ULTRASOUND_TYPE		1



/******************************************************************************
 * Class
 ******************************************************************************/
 
 //! WaspSensorCities Class
/*!
	WaspSensorCities Class defines all the variables and functions used for managing the Smart Cities Sensor Board
 */
class WaspSensorCities
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

	//! It converts the analog read value into dust density ()
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float dust_conversion(int readValue);

	//! It converts the analog read value into displacement (mm)
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float ld_conversion(int readValue);

	//! It converts the analog read value into sound pressure ()
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float audio_conversion(int readValue);

	//! It converts the analog read value into luminosity ()
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float ldr_conversion(int readValue);

	//! It converts the analog read value into humidity (%RH)
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float humidity_conversion(int readValue);
	
	//! It converts the analog read value into temperature (ºC)
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float temperature_conversion(int readValue);

	//! It converts the analog read value into distance (m)
  	/*!
	\param int readValue : the data to convert
	\return the value converted
	 */	
	float ultrasound_conversion(int readValue);



	public:

	//! Variable : specifies the sensor that has generated the interruption
  	/*!
	It specifies the socket that has generated the interruption, activating the corresponding flag position
   	*/
	uint8_t	intFlag;

	//! Variable : specifies the gain of the input amplifier
  	/*!
	It specifies the gain of the input amplifier of the microphone adaption stage
   	*/
	uint8_t	gain1;

	//! Variable : specifies the gain of the output amplifier
  	/*!
	It specifies the gain of the output amplifier of the microphone adaption stage
   	*/
	float	gain2;

	//! class constructor
  	/*!
		It initializes the different digital pins
	\param void
	\return void
	 */
	WaspSensorCities();

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
	
	//! It sets the value of the gain of the amplifiers in the microphone adaption stage
  	/*!
	\param uint8_t value : the value to set the input amplifier to
	\param float value : the value to set the output amplifier to
	\return void
	 */
	void setAudioGain(uint8_t value1, float value2);
	
	
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

extern WaspSensorCities SensorCities;

#endif

