/*! \file WaspXBee.h
    \brief Library for managing the UART related with the XBee
    
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
  
    Version:		0.12

    Design:		David Gascón

    Implementation:	Alberto Bielsa, Yuri Carmona

 */
 
/*! \def WaspXBeeXSC_h
    \brief The library flag
    
 */
#ifndef WaspXBeeXSC_h
#define WaspXBeeXSC_h

/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#include "WConstants.h"
#include "WaspXBee.h"
#include "WaspUSB.h"

#ifndef __WASPXBEECONSTANTS_H__
	#include "WaspXBeeConstants.h"
#endif

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
//! Structure : used for storing the information and data to send/receive a packet using XBee XSC module
/*!    
*/
typedef struct packetXSC
{
	//! Structure Variable : Source MAC higher bytes
	/*!    
 	*/
	uint8_t sourceMacHigh[2];
	
	//! Structure Variable : Source MAC lower bytes
	/*!    
	 */
	uint8_t sourceMacLow[2];
	
	//! Structure Variable : Data to send or received
	/*!    
	 */
	char data[MAX_DATA];
	
	//! Structure Variable : Data sent/received length
	/*!    
	 */
	uint8_t data_length;
	
	//! Structure Variable : Last received packet RSSI
	/*!    
	 */
	uint8_t RSSI;
};

/******************************************************************************
 * Class
 ******************************************************************************/

  //! WaspXBeeXSC Class
/*!
	WaspXBee Class defines all the variables and functions used for managing the XBee XSC modules
 */
class WaspXBeeXSC
{
  public:
	  
	//! class constructor
  	/*!
	  It does nothing
	  \param void
	  \return void
	 */
	WaspXBeeXSC(){};
	
	//! It initializes the necessary variables
  	/*!
	\param uint8_t protocol_used : specifies the protocol used in the XBee module (XBEE_XSC in this case)
	\param uint8_t frequency : specifies the frequency used in the XBee module (FREQ900M in this case)
	\param uint8_t model_used : specifies the XBee model used (PRO)
	\return void
	 */
	void	init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used);
	
	//! It initializes the necessary variables
  	/*!
	It initalizes all the necessary variables
	\param uint8_t protocol_used : specifies the protocol used in the XBee module (depends on the XBee module)
	\param uint8_t frequency : specifies the frequency used in the XBee module (depends on the XBee module)
	\param uint8_t model_used : specifies the XBee model used (depends on the XBee module)
	\param uint8_t uart_used : specifies the UART where the data are sent to (0-1)
	\return void
	 */
	void	init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used);
	
	//! It sets the module into Command Mode
  	/*!
	\param void
	\return '0' on success, '1' otherwise
	 */
	uint8_t setCommandMode();
	
	//! It exits the module from Command Mode
  	/*!
	\param void
	\return '0' on success, '1' otherwise
	 */
	uint8_t exitCommandMode();
	
	//! It sets the Baud Rate used in the module
  	/*!
	\param uint8_t brate : the baud rate (range [0-5])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setBaudRate(uint8_t brate);
	
	//! It gets the Baud Rate used in the module
  	/*!
	It stores in global 'baudRate' variable the Baud Rate used in the module
	\return '0' on success, '1' otherwise
	 */
	uint8_t getBaudRate();
	
	//! It sets the module Vendor ID
  	/*!
	\param uint8_t VID_H : higher module Vendor ID byte (range [0x00-0x7F])
	\param uint8_t VID_L : lower module Vendor ID byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setVendorID(uint8_t VID_H, uint8_t VID_L);
	
	//! It gets the module Vendor ID
  	/*!
	It stores in global 'vendorID' variable the module Vendor ID
	\return '0' on success, '1' otherwise
	 */
	uint8_t getVendorID();
	
	//! It gets the dBm level in the most recent packet
  	/*!
	It stores in global 'valueRSSI' variable the dBm level in the most recent packet
	\return '0' on success, '1' otherwise
	 */
	uint8_t getRSSI();
	
	//! It sets the networking address of a module
  	/*!
	\param uint8_t destAD_H : higher destination address byte (range [0x00-0xFF])
	\param uint8_t destAD_L : lower destination address byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setDestAddress(uint8_t destAD_H, uint8_t destAD_L);
	
	//! It gets the networking address of a module
  	/*!
	It stores in global 'destAddress' variable the networking address of a module
	\return '0' on success, '1' otherwise
	 */
	uint8_t getDestAddress();
	
	//! It sets the receive error count
  	/*!
	\param uint8_t recerror_H : higher receiver error count byte (range [0x00-0xFF])
	\param uint8_t recerror_L : lower receiver error count byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setReceiveErrorCount(uint8_t recerror_H, uint8_t recerror_L);
	
	//! It gets the receive error count
  	/*!
	It stores in global 'receiveErrorCount' variable the receive error count
	\return '0' on success, '1' otherwise
	 */
	uint8_t getReceiveErrorCount();
	
	//! It forces a Wake-up initializer to be sent on the next transmit
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t forceWakeUP();
	
	//! It resets the module through the UART
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t forceReset();
	
	//! It sets the receive good count
  	/*!
	\param uint8_t recgood_H : higher receive good count byte (range [0x00-0xFF])
	\param uint8_t recgood_L : lower receive good count byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setReceiveGoodCount(uint8_t recgood_H, uint8_t recgood_L);
	
	//! It gets the receive good count
  	/*!
	It stores in global 'receiveGoodCount' variable the receive good count
	\return '0' on success, '1' otherwise
	 */
	uint8_t getReceiveGoodCount();
	
	//! It sets the Hopping Channel used in the transmission
  	/*!
	\param uint8_t hchannel : hopping channel used in the transmission (range [0-6])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setHoppingChannel(uint8_t hchannel);
	
	//! It gets the Hopping Channel used in the transmission
  	/*!
	It stores in global 'channel' variable the Hopping Channel used in the transmission
	\return '0' on success, '1' otherwise
	 */
	uint8_t getHoppingChannel();
	
	//! It sets the time before send a Wake-up initializer
  	/*!
	\param uint8_t timeHT_H : higher timer byte (range [0x00-0xFF])
	\param uint8_t timeHT_L : lower timer byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setTimeBeforeWakeUP(uint8_t timeHT_H, uint8_t timeHT_L);
	
	//! It gets the time before send a Wake-up initializer
  	/*!
	It stores in global 'timeBeforeWakeUP' variable the time before sending a Wake-up initializer
	\return '0' on success, '1' otherwise
	 */
	uint8_t getTimeBeforeWakeUP();
	
	//! It sets the amount of time the Wake-Up Init is sent
  	/*!
	\param uint8_t timeLH : the amount of time the Wake-Up Init is sent (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setTimeWakeUpInit(uint8_t timeLH);
	
	//! It gets the amount of time the Wake-Up Init is sent
  	/*!
	It stores in global 'timeWakeUpInit' variable the time before send a Wake-up initializer
	\return '0' on success, '1' otherwise
	 */
	uint8_t getTimeWakeUpInit();
	
	//! It sets the address mask
  	/*!
	\param uint8_t mask_H : higher address mask byte (range [0x00-0xFF])
	\param uint8_t mask_L : lower address mask byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setAddressMask(uint8_t mask_H, uint8_t mask_L);
	
	//! It gets the address mask
  	/*!
	It stores in global 'addressMask' variable the address mask
	\return '0' on success, '1' otherwise
	 */
	uint8_t getAddressMask();
	
	//! It sets the pin Wake-UP ON/OFF
  	/*!
	\param uint8_t pin : the pin to wake-up (range [0-1])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setPinWakeUP(uint8_t pin);
	
	//! It gets the pin Wake-UP ON/OFF
  	/*!
	It stores in global 'pin' variable the pin Wake-UP ON/OFF
	\return '0' on success, '1' otherwise
	 */
	uint8_t getPinWakeUP();
	
	//! It restores all configurable parameters to defaults values
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t restoreDefaults();
	
	//! It sets the random delay slots
  	/*!
	\param uint8_t slot : the random delay slots (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setDelaySlots(uint8_t slot);
	
	//! It gets the random delay slots
  	/*!
	It stores in global 'delaySlots' variable the random delay slots
	\return '0' on success, '1' otherwise
	 */
	uint8_t getDelaySlots();
	
	//! It sets the packetization timeout
  	/*!
	\param uint8_t pack_H : higher timeout byte (range [0x00-0xFF])
	\param uint8_t pack_L : lower timeout byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setPacketTimeout(uint8_t pack_H, uint8_t pack_L);
	
	//! It gets the packetization timeout
  	/*!
	It stores in global 'packetTimeout' variable the random delay slots
	\return '0' on success, '1' otherwise
	 */
	uint8_t getPacketTimeout();
	
	//! It sets the time the output pin is active with the RSSI value
  	/*!
	\param uint8_t rssiTime : the time the output pin is active with the RSSI value (range [0x00-0x7F])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setRSSItime(uint8_t rssiTime);
	
	//! It gets the time the output pin is active with the RSSI value
  	/*!
	It stores in global 'timeRSSI' variable the time the output pin is active with the RSSI value
	\return '0' on success, '1' otherwise
	 */
	uint8_t getRSSItime();
	
	//! It sets the number of retries that can be sent for a given packet RF
  	/*!
	\param uint8_t retry : the number of retries that can be sent for a given packet RF (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setRetries(uint8_t retry);
	
	//! It gets the number of retries that can be sent for a given packet RF
  	/*!
	It stores in global 'retries' variable the number of retries that can be sent for a given packet RF
	\return '0' on success, '1' otherwise
	 */
	uint8_t getRetries();
	
	//! It gets the last received packet RSSI
  	/*!
	It stores in global 'RSSI' variable the last received packet RSSI
	\return '0' on success, '1' otherwise
	 */
	uint8_t getRSSIvalue();
	
	//! It sets the number of stop bits in the data packets
  	/*!
	\param uint8_t stop : the number of stop bits in the data packets (range [0-1])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setStopBits(uint8_t stop);
	
	//! It gets the number of stop bits in the data packets
  	/*!
	It stores in global 'stopBits' variable the number of stop bits in the data packets
	\return '0' on success, '1' otherwise
	 */
	uint8_t getStopBits();
	
	//! It gets the 32b Higher Source Mac
  	/*!
	It stores in global 'sourceMacHigh' variable the 32b Higher Source Mac
	\return '0' on success, '1' otherwise
	 */
	uint8_t getSourceMacHigh();
	
	//! It gets the 32b Lower Source Mac
  	/*!
	It stores in global 'sourceMacLow' variable the 32b Lower Source Mac
	\return '0' on success, '1' otherwise
	 */
	uint8_t getSourceMacLow();
	
	//! It sets the Sleep Mode
  	/*!
	\param uint8_t smode : the Sleep Mode (range [0-8])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setSleepMode(uint8_t smode);
	
	//! It gets the Sleep Mode
  	/*!
	It stores in global 'sleepMode' variable the Sleep Mode
	\return '0' on success, '1' otherwise
	 */
	uint8_t getSleepMode();
	
	//! It sets the amount of time the module is awake
  	/*!
	\param uint8_t awake_H : higher awake time byte (range [0x00-0xFF])
	\param uint8_t awake_L : lower awake time byte (range [0x10-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setAwakeTime(uint8_t awake_H, uint8_t awake_L);
	
	//! It gets the amount of time the module is awake
  	/*!
	It stores in global 'awakeTime' variable the amount of time the module is awake
	\return '0' on success, '1' otherwise
	 */
	uint8_t getAwakeTime();
	
	//! It sets the time before sending again the initializer
  	/*!
	\param uint8_t timeInit : the time before sending again the initializer (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setTimeBeforeInit(uint8_t timeInit);
	
	//! It gets the time before sending again the initializer
  	/*!
	It stores in global 'timeBeforeInit' variable the time before sending again the initializer
	\return '0' on success, '1' otherwise
	 */
	uint8_t getTimeBeforeInit();
	
	//! It sets the transmit error count
  	/*!
	\param uint8_t txerror_H : higher transmit error count byte (range [0x00-0xFF])
	\param uint8_t txerror_L : lower transmit error count byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setTransmitErrorCount(uint8_t txerror_H, uint8_t txerror_L);
	
	//! It gets the transmit error count
  	/*!
	It stores in global 'transmitErrorCount' variable the transmit error count
	\return '0' on success, '1' otherwise
	 */
	uint8_t getTransmitErrorCount();
	
	//! It sets the max number of bytes a module can send before waiting
  	/*!
	\param uint8_t txlim_H : higher transmit limit byte (range [0x00-0xFF])
	\param uint8_t txlim_L : lower transmit limit byte (range [0x00-0xFF])
	\return '0' on success, '1' otherwise
	 */
	uint8_t setTransmitLimit(uint8_t txlim_H, uint8_t txlim_L);
	
	//! It gets the max number of bytes a module can send before waiting
  	/*!
	It stores in global 'transmitLimit' variable the max number of bytes a module can send before waiting
	\return '0' on success, '1' otherwise
	 */
	uint8_t getTransmitLimit();
	
	//! It gets the Firmware version 
  	/*!
	It stores in global 'firmwareVersion' variable the Firmware version 
	\return '0' on success, '1' otherwise
	 */
	uint8_t getSoftVersion();
	
	//! It writes the parameters changed into non-volatil memory
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t writeValues();
	
	//! It sends data to other modules
  	/*!
	It sends over the air its own MAC, data length and the data, so the receiver can identify the sender and store the data
	\param struct packetXSC* packet : a structure where the data to send is stored
	\return '0' on success, '1' otherwise
	 */
	uint8_t sendData(struct packetXSC* packet);
	
	//! It reads data from other modules
  	/*!
	It stores the sender MAC and data into the 'packet' structure
	\param struct packetXSC* packet : a structure where the received data and node information is stored
	\return '0' on success, '1' otherwise
	 */
	uint8_t readData(struct packetXSC* packet);
	
	//! It is a transparent function. The user introduces an AT command within a string.
  	/*!
	It stores in the global 'commandAT' variable the XBee answer to the command
	\param char *atcommand : the AT command to send to the XBee module
	\return '0' on success, '1' otherwise
	 */
	uint8_t sendCommandAT(char *atcommand);
	
	//! It connects XBee, activating switch and opens the UART
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t ON();
	
	//! It sets the XBee to sleep, asserting PIN 9
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t sleep();
	
	//! It wakes up the XBee, de-asserting PIN 9
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t wake();

	//! Variable : the module Vendor ID
  	/*!
	 */
	uint8_t vendorID[2];
	
	//! Variable : the Baud Rate used in the module
  	/*!
	 */
	uint8_t baudRate;
	
	//! Variable : the dBm level in the most recent packet
  	/*!
	 */
	uint8_t valueRSSI;
	
	//! Variable : the networking address of a module
  	/*!
	 */
	uint8_t destAddress[2];
	
	//! Variable : the receive error count
  	/*!
	 */
	uint8_t receiveErrorCount[2];
	
	//! Variable : the receive good count
  	/*!
	 */
	uint8_t receiveGoodCount[2];
	
	//! Variable : the Hopping Channel used in the transmission
  	/*!
	 */
	uint8_t channel;
	
	//! Variable : the time before send a Wake-up initializer
  	/*!
	 */
	uint8_t timeBeforeWakeUP[2];
	
	//! Variable : the amount of time the Wake-Up Init is sent
  	/*!
	 */
	uint8_t timeWakeUpInit;
	
	//! Variable : the address mask
  	/*!
	 */
	uint8_t addressMask[2];
	
	//! Variable : the pin Wake-UP ON/OFF
  	/*!
	 */
	uint8_t pinWakeUP;
	
	//! Variable : the random delay slots
  	/*!
	 */
	uint8_t delaySlots;
	
	//! Variable : the packetization timeout
  	/*!
	 */
	uint8_t packetTimeout[2];
	
	//! Variable : the time the output pin is active with the RSSI value
  	/*!
	 */
	uint8_t timeRSSI;
	
	//! Variable : the number of retries that can be sent for a given packet RF
  	/*!
	 */
	uint8_t retries;
	
	//! Variable : the last received packet RSSI
  	/*!
	 */
	uint8_t RSSI;
	
	//! Variable : the number of stop bits in the data packets
  	/*!
	 */
	uint8_t stopBits;
	
	//! Variable : the 32b Higher Source Mac
  	/*!
	 */
	uint8_t sourceMacHigh[2];
	
	//! Variable : the 32b Lower Source Mac
  	/*!
	 */
	uint8_t sourceMacLow[2];
	
	//! Variable : the Sleep Mode
  	/*!
	 */
	uint8_t sleepMode;
	
	//! Variable : the amount of time the module is awake
  	/*!
	 */
	uint8_t awakeTime[2];
	
	//! Variable : the time before sending again the initializer
  	/*!
	 */
	uint8_t timeBeforeInit;
	
	//! Variable : the transmit error count
  	/*!
	 */
	uint8_t transmitErrorCount[2];
	
	//! Variable : the max number of bytes a module can send before waiting
  	/*!
	 */
	uint8_t transmitLimit[2];
	
	//! Variable : the Firmware version 
  	/*!
	 */
	uint8_t firmwareVersion[2];
	
	//! Variable : the XBee answer to the command
  	/*!
	 */
	uint8_t commandAT[20];
	

  private:

	//! It generates a decimal number from an ASCII character which was a number
  	/*!
	  \param uint8_t conv1 : the ASCII number to convert
	\return the converted number
	 */
	uint8_t converter(uint8_t conv1);
	
	//! It generates a decimal number from two ASCII characters which were numbers
  	/*!
	\param uint8_t conv1 : the ASCII number first digit to convert
	\param uint8_t conv2 : the ASCII number second digit to convert
	\return the converted number
	 */
	uint8_t converter(uint8_t conv1, uint8_t conv2);
	
	//! It generates a decimal number from three ASCII characters which were numbers
  	/*!
	\param uint8_t conv1 : the ASCII number first digit to convert
	\param uint8_t conv2 : the ASCII number second digit to convert
	\param uint8_t conv3 : the ASCII number third digit to convert
	\return the converted number
	 */
	uint8_t converter(uint8_t conv1, uint8_t conv2, uint8_t conv3);
	
	//! It checks if the XBee module answer a 'OK' message
  	/*!
	\return '0' if OK returned, '1' otherwise
	 */
	void	clean();
	
	//! It cleans the unwanted bytes returned by XBee module
  	/*!
	\return '0' on success, '1' otherwise
	 */
	uint8_t check();

        //! Variable : counter used in the library
  	/*!
	 */
        uint8_t it;

	//! Variable : protocol used (XBEE_XSC in this case)
  	/*!
	 */
        uint8_t protocol;
	
	//! Variable : frequency used (FREQ900M in this case)
  	/*!
	 */
        uint8_t freq;
	
	//! Variable : model used (PRO in this case)
  	/*!
	 */
        uint8_t model;
	
	//! Variable : it stores the UART where the dara are sent to (UART0 or UART1)
	/*!    
	 */
	uint8_t uart;
};

extern WaspXBeeXSC xbeeXSC;

#endif
