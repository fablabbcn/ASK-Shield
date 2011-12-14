/*! \file WaspBT.h
    \brief Library for managing the Bluetooth Board
    
    Copyright (C) 2010 Libelium Comunicaciones Distribuidas S.L.
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
  
    Version:		0.2
    Design:		David Gascón
    Implementation:	Alberto Bielsa, Manuel Calahorra, Yuri Carmona

 */

 /*! \def WaspBT_h
    \brief The library flag
    
  */
#ifndef WaspBT_h
#define WaspBT_h

/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <inttypes.h>

#ifndef __WASPBTCONSTANTS_H__
	#include "WaspBTConstants.h"
#endif

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
/*! \def BT_ERROR_SECURITY
    \brief BT Error Flag. Error while setting security parameters
 */
/*! \def BT_ERROR_ON
    \brief BT Error Flag. Error while powering the module
 */
/*! \def BT_ERROR_REMOVING_CONNECTION
    \brief BT Error Flag. Error while removing a connection
 */
/*! \def BT_ERROR_SENDING_DATA
    \brief BT Error Flag. Error while sending data
 */
/*! \def BT_ERROR_REMOVING_TD
    \brief BT Error Flag. Error while removing a trusted device
 */
/*! \def BT_ERROR_MAC
    \brief BT Error Flag. Error while getting our own mac
 */
/*! \def BT_ERROR_CONNECTING
    \brief BT Error Flag. Error while connecting to another device
 */
/*! \def BT_ERROR_DISC
    \brief BT Error Flag. Error while discovering a device
 */
/*! \def BT_ERROR_SCAN
    \brief BT Error Flag. Error while scanning the network
 */
/*! \def BT_ERROR_RESET
    \brief BT Error Flag. Error while resetting the device
 */
/*! \def BT_ERROR_SERVICE
    \brief BT Error Flag. Error while setting service parameters
 */
/*! \def BT_ERROR_DISC_OPT
    \brief BT Error Flag. Error while setting discovery options
 */
/*! \def BT_ERROR_ACCEPT
    \brief BT Error Flag. Error while setting auto accept incoming connections
 */
/*! \def BT_ERROR_NAME
    \brief BT Error Flag. Error while setting public name
 */
/*! \def BT_ERROR_STREAM
    \brief BT Error Flag. Error while setting stream connection
 */
#define	BT_ERROR_SECURITY		0
#define	BT_ERROR_ON			1
#define	BT_ERROR_REMOVING_CONNECTION	2
#define	BT_ERROR_SENDING_DATA		4
#define	BT_ERROR_REMOVING_TD		8
#define	BT_ERROR_MAC			16
#define	BT_ERROR_CONNECTING		32
#define	BT_ERROR_DISC			64
#define	BT_ERROR_SCAN			128
#define	BT_ERROR_RESET			256
#define	BT_ERROR_SERVICE		512
#define	BT_ERROR_DISC_OPT		1024
#define	BT_ERROR_ACCEPT			2048
#define	BT_ERROR_NAME			4096
#define	BT_ERROR_STREAM			8192


//! Structure : used for storing the discovered devices
/*!    
 */
typedef struct Device
{
	//! Variable : MAC address
    	/*!
	 */
	char mac_address[12];
	
	//! Variable : operating service channel
    	/*!
	 */
	char channel[2];
	
	//! Variable : service name
    	/*!
	 */
	char service_name[20];
	
	//! Variable : public name
    	/*!
	 */
	char name[19];
	
	//! Variable : class of device
    	/*!
	 */
	char CoD[7];
};


/******************************************************************************
 * Class
 ******************************************************************************/
 
//! WaspBT Class
/*!
	WaspBT Class defines all the variables and functions used for managing the Bluetooth Board
 */
class WaspBT
{
	private:
	
	//! Variable : integer used as counter
    	/*!
	 */
	uint16_t i;
	
	//! Variable : array for storing the data received from the module
    	/*!
	 */
	char received[100];
	
	//! Variable : integer to reset the BT module before setting security
    	/*!
	 */	
	uint8_t first;
	
	//! Variable : integer to store the uart opened
    	/*!
	 */	
	uint8_t _uart;
	
        //! It looks for a pattern in 'data'
    	/*!
	\param char* data : data where the 'expectedAnswer' has to be found
	\param char* expectedAnswer : string expected to be answered by the module
	\return '0' if pattern is not found, pattern position otherwise
	 */
	uint16_t waitForData(uint8_t* data, const char* expectedAnswer);
	
	//! It waits for response from the module and executes the proper function
    	/*!
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t parse_data();
	
	//! It waits for response from the module and executes the proper function
    	/*!
	\param char* answer : string expected to be answered by the module
	\return '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t parse_data(char* answer);
	
	//! It answers to the PIN request
    	/*!
	\param uint8_t* data : data that has been answered by the module
	\param uint8_t pin_en : specifies if pin is requested
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t pin_request(uint8_t* data,uint8_t pin_en);
	
	//! It accepts incoming connection
    	/*!
	\param uint8_t* data : data that has been answered by the module
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t accept_connection(uint8_t* data);

	//! It stores incoming data
    	/*!
	\param uint8_t* data : data that has been answered by the module
	\return '0'
	 */
	uint8_t data_request(uint8_t* data);

	//! It changes the value of 'active_connection' variable
    	/*!
	\param uint8_t* data : data that has been answered by the module
	 */
	void ending_connection(uint8_t* data);
		
	//! It stores the discovered devices in the appropriate array
    	/*!
	\param uint8_t* data : data that has been answered by the module
	\param uint16_t number_of_data : number of discovered devices
	 */
	void parse_brothers(uint8_t* data, uint16_t number_of_data);
	
	//! It stores the discovered device parameters
    	/*!
	\param uint8_t* data : data that has been answered by the module
	 */
	void parse_device(uint8_t* data);
	
	//! It stores the MTU of the active connection
    	/*!
	\param uint8_t* data : data that has been answered by the module
	 */
	void get_MTU(uint8_t* data);
	
	//! It sends the data through the UART
    	/*!
	\param uint8_t* data : data to be sent
	 */
	void printData(const char* data);



	public:
	
	//! Variable : status flag, used to see if there was an error while communicating with the module
    	/*!
	Possible values are : BT_ERROR_ON,
     	*/
	int   flag;
	
	//! class constructor
  	/*!
	It initializes some variables
	\param void
	\return void
	 */
	WaspBT();
	
	//! It opens the UART and powers the module
    	/*!
	 */
	void ON();	

	//! It opens the UART and powers the module
    	/*!
	\param uint8_t uart : UART to send the data (UART0 or UART1)
	 */
	void ON(uint8_t uart);
	
	//! It closes the UART and powers off the module
    	/*!
	 */
	void OFF();
	
	//! It sets some initial parameters such as security parameters, service parameters, public name, auto accept and discoverable
    	/*!
	\return '0' when the functions are right, '>0' otherwise
	 */
	uint8_t init();
	
	//! It sets security parameters
    	/*!
	\param uint8_t mode : security mode (BT_SECURITY_1, BT_SECURITY_NONE, BT_SECURITY_3)
	\param char* pin : the pin to stablish
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t setSecurity(uint8_t mode, const char* pin);
	
	//! It sets the public name
    	/*!
	\param char* name : the public name to stablish
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t setPublicName(const char* name);
	
	//! It sets auto accepting incoming connections
    	/*!
	\param uint8_t mode : BT_AUTO_ACC or BT_MANUAL_ACC
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t setAutoAccept(uint8_t mode);
	
	//! It sets the discovering options
    	/*!
	\param uint8_t option : the discovering options (BT_NO_DISC, BT_INQ_EN, BT_PAGE_EN, BT_DISCOVERABLE)
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t setDiscoveryOptions(uint8_t option);
	
	//! It waits for incoming data and stores it in the appropriate array
    	/*!
	\return '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t readData();
	
	//! It sets the service parameters
    	/*!
	\param char* name : the service name
	\param uint8_t channel : the service channel
	\param char* cod: Class of Device
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t setServiceParameters(const char* name, uint8_t channel, const char* cod);
	
	//! It resets the module
    	/*!
	\return '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t reset();
	
	//! It scans looking for devices. It stores in 'discovered_devices' struct the devices discovered
    	/*!
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t scanNetwork();
	
	//! It searchs for a specific device and stores the service name and service channel in the aprropriate variables
    	/*!
	\param char* mac : the MAC of the device to find
	\param char* profile : the profile (SPP='1101')
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t discoverDevice(const char* mac, const char* profile);
	
	//! It creates a connection with other module
    	/*!
	\param char* mac : the MAC of the device to connect to
	\param char* channel : the channel where the device is operating
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t createConnection(const char* mac,const char* channel);
	
	//! It gets the own MAC address. It stores the mac in 'own_mac' variable
    	/*!
	\return '0' on success, '1' if error
	 */
	uint8_t getOwnMac();
	
	//! It removes the trusted device specified
    	/*!
	\param char* mac : the MAC of the device to remove
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t removeTrustedDevice(const char* mac);
	
	//! It sends data to other module
    	/*!
	\param char* data : the data to send
	\return '-1' if memory error, '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t sendData(const char* data);
	
	//! It removes an active connection
    	/*!
	\return '0' on success, '1' if error
	 */
	uint8_t removeConnection();
	
	//! It creates a transparent communication between two devices
    	/*!
	\return '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
	 */
	uint8_t createStreamConnection();
	
	
	//! Variable : current pin
    	/*!
	 */
	char bt_pin[16];
	
	//! Variable : specifies if incoming connections are accepted automatically or not
    	/*!
	 */
	uint8_t auto_accept;
	
	//! Variable : specifies if there is an active connection or not
    	/*!
	 */
	uint8_t active_connection;
	
	//! Variable : stores the discovered devices
    	/*!
	 */
	Device discovered_devices[BT_MAX_DEVICES];
	
	//! Variable : stores the number of discovered devices
    	/*!
	 */
	uint8_t devices_found;
	
	//! Variable : stores the service name that a module is using. It is filled when the function 'searchDevice' is called
    	/*!
	 */
	char device_service_name[17];
	
	//! Variable : stores the service channel that a module is using. It is filled when the function 'searchDevice' is called
    	/*!
	 */
	char device_service_channel[2];
	
	//! Variable : stores the maximum MTU can be used in an active connection
    	/*!
	 */
	char connection_mtu[3];
	
	//! Variable : stores the MAC of the device
    	/*!
	 */
	char own_mac[12];
	
	//! Variable : stores the received data
    	/*!
	 */
	char data_received[BT_MAX_DATA];
};

extern WaspBT BT;

#endif

