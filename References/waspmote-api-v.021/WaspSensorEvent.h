/*! \file WaspSensorEvent.h
    \brief Library for managing the Event Sensor Board
    
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


 /*! \def WaspSensorEvent_h
    \brief The library flag
    
  */
#ifndef WaspSensorEvent_h
#define WaspSensorEvent_h

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def SENS_SOCKET4
    \brief Sensor sockets. Socket number 4 in this case
    
*/
/*! \def SENS_SOCKET3
    \brief Sensor sockets. Socket number 3 in this case
    
 */
/*! \def SENS_SOCKET6
    \brief Sensor sockets. Socket number 6 in this case
    
 */
/*! \def SENS_SOCKET5
    \brief Sensor sockets. Socket number 5 in this case
    
 */
/*! \def SENS_SOCKET1
    \brief Sensor sockets. Socket number 1 in this case
    
 */
/*! \def SENS_SOCKET2
    \brief Sensor sockets. Socket number 2 in this case
    
 */
/*! \def SENS_SOCKET7
    \brief Sensor sockets. Socket number 7 in this case
    
 */
/*! \def SENS_SOCKET8
    \brief Sensor sockets. Socket number 8 in this case
    
 */
#define	SENS_SOCKET4		1
#define	SENS_SOCKET3		2
#define	SENS_SOCKET6		4
#define	SENS_SOCKET5		8
#define	SENS_SOCKET1		16
#define	SENS_SOCKET2		32
#define	SENS_SOCKET7		64
#define	SENS_SOCKET8		128

/******************************************************************************
 * Class
 ******************************************************************************/
 
 //! WaspSensorEvent Class
/*!
	WaspSensorEvent Class defines all the variables and functions used for managing the Event Sensor Board
 */
class WaspSensorEvent
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

	public:

	//! Variable : specifies the sensor that has generated the interruption
  	/*!
	It specifies the socket that has generated the interruption, activating the corresponding flag position using the 'SENS_SOCKETX' definitions (X range [1-8])
	\sa SENS_SOCKET4, SENS_SOCKET3, SENS_SOCKET6, SENS_SOCKET5, SENS_SOCKET1, SENS_SOCKET2, SENS_SOCKET7, SENS_SOCKET8
   	*/
	uint8_t	intFlag;

	//! class constructor
  	/*!
		It initializes the different digital pins
	\param void
	\return void
	 */
	WaspSensorEvent();

	//! It sets ON/OFF the 3V3/5V switches
  	/*!
	\param uint8_t mode : SENS_ON or SENS_OFF
	\return void
	 */
	void 	setBoardMode(uint8_t mode);
	
	//! It sets threshold configuring digipots
  	/*!
	\param uint8_t sensor : specifies the socket to set the threshold to ('SENS_SOCKETX')
	\param float threshold : the threshold to set (0-3.3V)
	\return void
	 */
	void	setThreshold(uint8_t sensor, float threshold);
	
	//! It reads the value measured by the sensor
  	/*!
	\param uint8_t sensor : specifies the socket ('SENS_SOCKETX')
	\return the value read 
	 */
	float 	readValue(uint8_t sensor);
	
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

extern WaspSensorEvent SensorEvent;

#endif

