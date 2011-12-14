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
 *  Version:		0.12
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, Yuri Carmona
 */
 

#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif

void	WaspXBeeXSC::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used)
{
	init(protocol_used, frequency, model_used, 0);
}

void	WaspXBeeXSC::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used)
{
	protocol=protocol_used;
	freq=frequency;
	model=model_used;
	uart=uart_used;
}

/*
 Function: Set the module into Command Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::setCommandMode()
{
  uint8_t error=0;

	delay(1500);
  	if( uart==UART0 ) XBee.print("+++");
  	else if( uart==UART1 ) XBee2.print("+++");
  	delay(1000);
	clean();
	error=0;
	return error;
}

/*
 Function: Exits the module from Command Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::exitCommandMode()
{
  	clean();
  	if( uart==UART0 ) XBee.println("atcn");
	else if( uart==UART1 ) XBee2.println("atcn"); 
  	delay(1000);
  	return check();
}

/*
 Function: Set the Module Vendor ID
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "vendorID" variable the module Vendor ID
*/
uint8_t WaspXBeeXSC::setVendorID(uint8_t VID_H, uint8_t VID_L)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atid");
  	XBee.print(VID_H,HEX);
  	XBee.println(VID_L,HEX);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atid");
  	XBee2.print(VID_H,HEX);
  	XBee2.println(VID_L,HEX);
  }
  delay(1000);
  if(!check())
  {
	vendorID[0]=VID_H;
	vendorID[1]=VID_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the Module Vendor ID
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "vendorID" variable the module Vendor ID
*/
uint8_t WaspXBeeXSC::getVendorID()
{
  uint8_t ByteIN[10];
  uint8_t error=2;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atid");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atid");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  vendorID[0]=converter(ByteIN[0],ByteIN[1]);
  vendorID[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}


/*
 Function: Set the Baud Rate used in the module
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "baudRate" variable the Baud Rate used in the module
*/
uint8_t WaspXBeeXSC::setBaudRate(uint8_t brate)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atbd");
	XBee.println(brate,DEC);
  }
  else if ( uart==UART1 )
  {
	XBee2.print("atbd");
	XBee2.println(brate,DEC);
  }
  delay(1000);
  if(!check())
  {
	baudRate=brate;
	return 0;
  }
  return 1;
}


/*
 Function: Get the Baud Rate used in the module
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "baudRate" variable the Baud Rate used in the module
*/
uint8_t WaspXBeeXSC::getBaudRate()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atbd");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atbd");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  baudRate=converter(ByteIN[0]);
  return error;
}

/*
 Function: Get the dBm level in the most recent packet
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "valueRSSI" variable the dBm level in the most recent packet
*/
uint8_t WaspXBeeXSC::getRSSI()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atdb");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atdb");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  valueRSSI=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the networking address of a module
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "destAddress" variable the networking address of the module
*/
uint8_t WaspXBeeXSC::setDestAddress(uint8_t destAD_H, uint8_t destAD_L)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("atdt");
  	XBee.print(destAD_H,HEX);
  	XBee.println(destAD_L,HEX);
  	delay(1000);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("atdt");
  	XBee2.print(destAD_H,HEX);
  	XBee2.println(destAD_L,HEX);
  	delay(1000);
  }
  
  if(!check())
  {
	destAddress[0]=destAD_H;
	destAddress[1]=destAD_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the networking address of a module
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "destAddress" variable the Baud Rate used in the module
*/
uint8_t WaspXBeeXSC::getDestAddress()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atdt");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
 	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atdt");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
 	}
  }
  destAddress[0]=converter(ByteIN[0],ByteIN[1]);
  destAddress[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the receive error count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "receiveErrorCount" variable the receive error count
*/
uint8_t WaspXBeeXSC::setReceiveErrorCount(uint8_t recerror_H, uint8_t recerror_L)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("ater");
  	XBee.print(recerror_H,HEX);
  	XBee.println(recerror_L,HEX);
  	delay(1000);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("ater");
  	XBee2.print(recerror_H,HEX);
  	XBee2.println(recerror_L,HEX);
  	delay(1000);
  }
  if(!check())
  {
	receiveErrorCount[0]=recerror_H;
	receiveErrorCount[1]=recerror_L;
	return 0;
  }
  return 1;
}

/*
 Function: Set the receive error count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "receiveErrorCount" variable the receive error count
*/
uint8_t WaspXBeeXSC::getReceiveErrorCount()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("ater");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("ater");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  receiveErrorCount[0]=converter(ByteIN[0],ByteIN[1]);
  receiveErrorCount[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Force a Wake-up initializer to be sent on the next transmit
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::forceWakeUP()
{
  clean();
  if( uart==UART0 ) XBee.println("atfh");
  else if( uart==UART1 ) XBee2.println("atfh");
  delay(1000);
  return check();
}

/*
 Function: Resets the module through the UART
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::forceReset()
{
  clean();
  if( uart==UART0 ) XBee.println("atfr");
  else if( uart==UART1 ) XBee2.println("atfr");
  delay(1000);
  return check();
}

/*
 Function: Set the receive good count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "receiveGoodCount" variable the receive good count
*/
uint8_t WaspXBeeXSC::setReceiveGoodCount(uint8_t recgood_H, uint8_t recgood_L)
{
  clean();
  if( uart==UART0 ) 
  {
	XBee.print("atgd");
  	XBee.print(recgood_H,HEX);
  	XBee.println(recgood_L,HEX);
  }
  else if( uart==UART1 ) 
  {
	XBee2.print("atgd");
  	XBee2.print(recgood_H,HEX);
  	XBee2.println(recgood_L,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	receiveGoodCount[0]=recgood_H;
	receiveGoodCount[1]=recgood_L;
	return 0;
  }
  return 1;
}

/*
 Function: Set the receive good count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "receiveGoodCount" variable the receive good count
*/
uint8_t WaspXBeeXSC::getReceiveGoodCount()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean(); 
  if( uart==UART0 ) 
  {
	XBee.println("atgd");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 ) 
  {
	XBee2.println("atgd");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }

  receiveGoodCount[0]=converter(ByteIN[0],ByteIN[1]);
  receiveGoodCount[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the Hopping Channel used in the communication
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "channel" variable the Hopping Channel used in the communication
*/
uint8_t WaspXBeeXSC::setHoppingChannel(uint8_t hchannel)
{
  clean();
  if( uart==UART0 ) 
  {
	XBee.print("athp");
	XBee.println(hchannel,DEC);
  }
  else if( uart==UART1 ) 
  {
	XBee2.print("athp");
	XBee2.println(hchannel,DEC);
  }
  delay(1000);
  
  if(!check())
  {
	channel=hchannel;
	return 0;
  }
  return 1;
}

/*
 Function: Get the Hopping Channel used in the communication
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "channel" variable the Hopping Channel used in the communication
*/
uint8_t WaspXBeeXSC::getHoppingChannel()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("athp");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("athp");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  } 
  channel=converter(ByteIN[0]);
  return error;
}

/*
 Function: Set the time before Wake-UP initializer
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeBeforeWakeUP" variable the time before Wake-UP initializer
*/
uint8_t WaspXBeeXSC::setTimeBeforeWakeUP(uint8_t timeHT_H, uint8_t timeHT_L)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("atht");
  	XBee.print(timeHT_H,HEX);
  	XBee.println(timeHT_L,HEX);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("atht");
  	XBee2.print(timeHT_H,HEX);
  	XBee2.println(timeHT_L,HEX);
  }

  delay(1000);
  
  if(!check())
  {
	timeBeforeWakeUP[0]=timeHT_H;
	timeBeforeWakeUP[1]=timeHT_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the time before Wake-UP initializer
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeBeforeWakeUP" variable the time before Wake-UP initializer
*/
uint8_t WaspXBeeXSC::getTimeBeforeWakeUP()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atht");
	delay(1000);
	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atht");
	delay(1000);
	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  timeBeforeWakeUP[0]=converter(ByteIN[0],ByteIN[1]);
  timeBeforeWakeUP[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the amount of time Wake-UP initializer is sent
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeWakeUpInit" variable the amount of time Wake-UP initializer is sent
*/
uint8_t WaspXBeeXSC::setTimeWakeUpInit(uint8_t timeLH)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("atlh");
  	XBee.println(timeLH,HEX);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("atlh");
  	XBee2.println(timeLH,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	timeWakeUpInit=timeLH;
	return 0;
  }
  return 1;
}

/*
 Function: Get the amount of time Wake-UP initializer is sent
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeWakeUpInit" variable the amount of time Wake-UP initializer is sent
*/
uint8_t WaspXBeeXSC::getTimeWakeUpInit()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atlh");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atlh");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  timeWakeUpInit=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the address mask
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "addressMask" variable the address mask
*/
uint8_t WaspXBeeXSC::setAddressMask(uint8_t mask_H, uint8_t mask_L)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("atmk");
  	XBee.print(mask_H,HEX);
  	XBee.println(mask_L,HEX);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("atmk");
  	XBee2.print(mask_H,HEX);
  	XBee2.println(mask_L,HEX);
  } 
  delay(1000);
  
  if(!check())
  {
	addressMask[0]=mask_H;
	addressMask[1]=mask_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the address mask
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "addressMask" variable the address mask
*/
uint8_t WaspXBeeXSC::getAddressMask()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atmk");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atmk");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  addressMask[0]=converter(ByteIN[0],ByteIN[1]);
  addressMask[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set Pin Wake-UP ON/OFF
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "pinWakeUP" variable Pin Wake-UP ON/OFF
*/
uint8_t WaspXBeeXSC::setPinWakeUP(uint8_t pin)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("atpw");
  	XBee.println(pin,DEC);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("atpw");
  	XBee2.println(pin,DEC);
  }

  delay(1000);
  
  if(!check())
  {
	pinWakeUP=pin;
	return 0;
  }
  return 1;
}

/*
 Function: Get Pin Wake-UP ON/OFF
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "pin" variable Pin Wake-UP ON/OFF
*/
uint8_t WaspXBeeXSC::getPinWakeUP()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atpw");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atpw");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  pinWakeUP=converter(ByteIN[0]);
  return error;
}

/*
 Function: Restore all configurable parameters to defaults values
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::restoreDefaults()
{
  clean();
  if( uart==UART0 ) XBee.println("atre");
  else if( uart==UART1 ) XBee2.println("atre");
  delay(1000);
  return check();
}

/*
 Function: Set Random delay slots
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "delaySlots" variable the Random delay slots
*/
uint8_t WaspXBeeXSC::setDelaySlots(uint8_t slot)
{
  clean();
  if( uart==UART0 )
  {
 	XBee.print("atrn");
  	XBee.println(slot,HEX);
  }
  else if( uart==UART1 )
  {
 	XBee2.print("atrn");
  	XBee2.println(slot,HEX);
  } 
  delay(1000);
  
  if(!check())
  {
	delaySlots=slot;
	return 0;
  }
  return 1;
}

/*
 Function: Get Random delay slots
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "delaySlots" variable the Random delay slots
*/
uint8_t WaspXBeeXSC::getDelaySlots()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atrn");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atrn");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  delaySlots=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the packetization timeout
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "packetTimeout" variable the packetization timeout
*/
uint8_t WaspXBeeXSC::setPacketTimeout(uint8_t pack_H, uint8_t pack_L)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atro");
	XBee.print(pack_H,HEX);
  	XBee.println(pack_L,HEX);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atro");
	XBee2.print(pack_H,HEX);
  	XBee2.println(pack_L,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	packetTimeout[0]=pack_H;
	packetTimeout[1]=pack_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the packetization timeout
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "packetTimeout" variable the packetization timeout
*/
uint8_t WaspXBeeXSC::getPacketTimeout()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atro");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atro");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  packetTimeout[0]=converter(ByteIN[0],ByteIN[1]);
  packetTimeout[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the time the output pin is active with the RSSI value
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeRSSI" variable the time the output pin is active with the RSSI value
*/
uint8_t WaspXBeeXSC::setRSSItime(uint8_t rssiTime)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atrp");
  	XBee.println(rssiTime,HEX);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atrp");
  	XBee2.println(rssiTime,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	timeRSSI=rssiTime;
	return 0;
  }
  return 1;
}

/*
 Function: Get the time the output pin is active with the RSSI value
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeRSSI" variable the time the output pin is active with the RSSI value
*/
uint8_t WaspXBeeXSC::getRSSItime()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) 
  {
	XBee.println("atrp");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 ) 
  {
	XBee2.println("atrp");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  timeRSSI=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the number of retries that can be sent for a given packet RF
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "retries" variable the number of retries that can be sent for a given packet RF
*/
uint8_t WaspXBeeXSC::setRetries(uint8_t retry)
{
  clean();
  if( uart==UART0 ) 
  {
  	XBee.print("atrr");
  	XBee.println(retry,HEX);
  }
  else if( uart==UART1 ) 
  {
  	XBee2.print("atrr");
  	XBee2.println(retry,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	retries=retry;
	return 0;
  }
  return 1;
}

/*
 Function: Get the number of retries that can be sent for a given packet RF
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "retries" variable the number of retries that can be sent for a given packet RF
*/
uint8_t WaspXBeeXSC::getRetries()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) 
  {
  	XBee.println("atrr");
 	 delay(1000);
 	 while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 ) 
  {
  	XBee2.println("atrr");
 	 delay(1000);
 	 while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  retries=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Get RSSI of the last packet received
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "RSSI" variable the RSSI of the last packet received
*/
uint8_t WaspXBeeXSC::getRSSIvalue()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) 
  {
	XBee.println("atrs");
	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
 	}
  }
  else if( uart==UART1 ) 
  {
	XBee2.println("atrs");
	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
 	}
  }
  RSSI=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the number of bits in the data packet
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "stopBits" variable the number of bits in the data packet
*/
uint8_t WaspXBeeXSC::setStopBits(uint8_t stop)
{
  clean();
  if( uart==UART0 ) 
  {
  	XBee.print("atsb");
  	XBee.println(stop,HEX);
  }
  else if( uart==UART1 ) 
  {
  	XBee2.print("atsb");
  	XBee2.println(stop,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	stopBits=stop;
	return 0;
  }
  return 1;
}

/*
 Function: Get the number of bits in the data packet
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "stopBits" variable the number of bits in the data packet
*/
uint8_t WaspXBeeXSC::getStopBits()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) 
  {
 	XBee.println("atsb");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 ) 
  {
 	XBee2.println("atsb");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  stopBits=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Get the 32b higher Source Mac 
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "sourceMacHigh" variable the 32b higher Source Mac 
*/
uint8_t WaspXBeeXSC::getSourceMacHigh()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) 
  {
	XBee.println("atsh");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 ) 
  {
	XBee2.println("atsh");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  if(i<5)
  {
	  ByteIN[3]=ByteIN[2];
	  ByteIN[2]=ByteIN[1];
	  ByteIN[1]=ByteIN[0];
	  ByteIN[0]=0;
  }
  it=0; 
  i=0;
  while(i<4)
  {
	sourceMacHigh[it]=converter(ByteIN[i],ByteIN[i+1]);
	i++;
	i++;
	it++;
  }
  return error;
}

/*
 Function: Get the 32b lowe Source Mac 
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "sourceMacLow" variable the 32b lower Source Mac 
*/
uint8_t WaspXBeeXSC::getSourceMacLow()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atsl");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atsl");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  if(i<5)
  {
	  ByteIN[3]=ByteIN[2];
	  ByteIN[2]=ByteIN[1];
	  ByteIN[1]=ByteIN[0];
	  ByteIN[0]=0;
  }
  i=0;
  it=0;
  while(i<4)
  {
	sourceMacLow[it]=converter(ByteIN[i],ByteIN[i+1]);
	i++;
	i++;
	it++;
  }
  return error;
}

/*
 Function: Set the Sleep Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "sleepMode" variable the Sleep Mode
*/
uint8_t WaspXBeeXSC::setSleepMode(uint8_t smode)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atsm");
  	XBee.println(smode,DEC);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atsm");
  	XBee2.println(smode,DEC);
  }
  delay(1000);
  
  if(!check())
  {
	sleepMode=smode;
	return 0;
  }
  return 1;
}

/*
 Function: Get the Sleep Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "sleepMode" variable the Sleep Mode
*/
uint8_t WaspXBeeXSC::getSleepMode()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
	XBee.println("atsm");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("atsm");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  sleepMode=converter(ByteIN[0]);
  return error;
}


/*
 Function: Set the amount of time the module stays inactive before entering into Sleep Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "awakeTime" variable the amount of time the module stays inactive before entering into Sleep Mode
*/
uint8_t WaspXBeeXSC::setAwakeTime(uint8_t awake_H, uint8_t awake_L)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atst");
  	XBee.print(awake_H,HEX);
  	XBee.println(awake_L,HEX);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atst");
  	XBee2.print(awake_H,HEX);
  	XBee2.println(awake_L,HEX);
  } 
  delay(1000);
  
  if(!check())
  {
	awakeTime[0]=awake_H;
	awakeTime[1]=awake_L;
	return 0;
  }
  return 1;
}


/*
 Function: Get the amount of time the module stays inactive before entering into Sleep Mode
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "awakeTime" variable the amount of time the module stays inactive before entering into Sleep Mode
*/
uint8_t WaspXBeeXSC::getAwakeTime()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atst");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atst");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  awakeTime[0]=converter(ByteIN[0],ByteIN[1]);
  awakeTime[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the Time before sending again Initializer
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeBeforeInit" variable the Time before sending again Initializer
*/
uint8_t WaspXBeeXSC::setTimeBeforeInit(uint8_t timeInit)
{
  clean();
  if( uart==UART0 )
  {
	XBee.print("atsy");
	XBee.println(timeInit,HEX);
  }
  else if( uart==UART1 )
  {
	XBee2.print("atsy");
	XBee2.println(timeInit,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	timeBeforeInit=timeInit;
	return 0;
  }
  return 1;
}

/*
 Function: Get the Time before sending again Initializer
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeBeforeInit" variable the Time before sending again Initializer
*/
uint8_t WaspXBeeXSC::getTimeBeforeInit()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atsy");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atsy");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  timeBeforeInit=converter(ByteIN[0],ByteIN[1]);
  return error;
}

/*
 Function: Set the trasmit error count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "transmitErrorCount" variable the transmit error count
*/
uint8_t WaspXBeeXSC::setTransmitErrorCount(uint8_t txerror_H, uint8_t txerror_L)
{
  clean();
  if( uart==UART0 )
  {
  	XBee.print("attr");
  	XBee.print(txerror_H,HEX);
  	XBee.println(txerror_L,HEX);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("attr");
  	XBee2.print(txerror_H,HEX);
  	XBee2.println(txerror_L,HEX);
  } 
  delay(1000);
  
  if(!check())
  {
	transmitErrorCount[0]=txerror_H;
	transmitErrorCount[1]=txerror_L;
	return 0;
  }
  return 1;
}

/*
 Function: Set the transmit error count
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "transmitErrorCount" variable the transmit error count
*/
uint8_t WaspXBeeXSC::getTransmitErrorCount()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();  
  if( uart==UART0 )
  {
	XBee.println("attr");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
	XBee2.println("attr");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  transmitErrorCount[0]=converter(ByteIN[0],ByteIN[1]);
  transmitErrorCount[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Set the max number of bytes a module can send before waiting some random time
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "transmitLimit" variable the max number of bytes a module can send before waiting some random time
*/
uint8_t WaspXBeeXSC::setTransmitLimit(uint8_t txlim_H, uint8_t txlim_L)
{
  clean();  
  if( uart==UART0 )
  {
  	XBee.print("attt");
 	XBee.print(txlim_H,HEX);
  	XBee.println(txlim_L,HEX);
  }
  else if( uart==UART1 )
  {
  	XBee2.print("attt");
 	XBee2.print(txlim_H,HEX);
  	XBee2.println(txlim_L,HEX);
  }
  delay(1000);
  
  if(!check())
  {
	transmitLimit[0]=txlim_H;
	transmitLimit[1]=txlim_L;
	return 0;
  }
  return 1;
}

/*
 Function: Get the max number of bytes a module can send before waiting some random time
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "transmitLimit" variable the max number of bytes a module can send before waiting some random time
*/
uint8_t WaspXBeeXSC::getTransmitLimit()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("attt");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("attt");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}
  }
  transmitLimit[0]=converter(ByteIN[0],ByteIN[1]);
  transmitLimit[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Get the Firmware Version of the module
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "firmwareVersion" variable the Firmware Version of the module
*/
uint8_t WaspXBeeXSC::getSoftVersion()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 )
  {
  	XBee.println("atvr");
  	delay(1000);
  	while(XBee.available() > 0) 
  	{
		ByteIN[i]=XBee.read();
		i++;
		error=0;
  	}
  }
  else if( uart==UART1 )
  {
  	XBee2.println("atvr");
  	delay(1000);
  	while(XBee2.available() > 0) 
  	{
		ByteIN[i]=XBee2.read();
		i++;
		error=0;
  	}  
  }
  firmwareVersion[0]=converter(ByteIN[0],ByteIN[1]);
  firmwareVersion[1]=converter(ByteIN[2],ByteIN[3]);
  return error;
}

/*
 Function: Write the values into a non-volatil memory
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::writeValues()
{
  uint8_t ByteIN[10];
  uint8_t error=1;
  uint8_t i=0;

  clean();
  if( uart==UART0 ) XBee.println("atwr");
  else if( uart==UART1 ) XBee2.println("atwr");
  delay(1000);
  return check();
}

/*
 Function: Send bytes to other modules in the same network
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::sendData(struct packetXSC* packet)
{
  uint8_t error=2;

	error=setCommandMode();
	error=getSourceMacHigh();
	error=getSourceMacLow();
	error=exitCommandMode();
	packet->sourceMacHigh[0]=sourceMacHigh[0];
	packet->sourceMacHigh[1]=sourceMacHigh[1];
	packet->sourceMacLow[0]=sourceMacLow[0];
	packet->sourceMacLow[1]=sourceMacLow[1];

	// Enviamos los datos
	if( uart==UART0 ) 
        {
		XBee.print(packet->sourceMacHigh[0],HEX);
		XBee.print(packet->sourceMacHigh[1],HEX);
		XBee.print(packet->sourceMacLow[0],HEX);
		XBee.print(packet->sourceMacLow[1],HEX);	
		XBee.print("-");
		XBee.print(packet->data_length,DEC);
		XBee.print("-");
		for(it=0;it<packet->data_length;it++)
		{
			XBee.print(packet->data[it],BYTE);
		}	
        }
        else if( uart==UART1 ) 
        {
		XBee2.print(packet->sourceMacHigh[0],HEX);
		XBee2.print(packet->sourceMacHigh[1],HEX);
		XBee2.print(packet->sourceMacLow[0],HEX);
		XBee2.print(packet->sourceMacLow[1],HEX);	
		XBee2.print("-");
		XBee2.print(packet->data_length,DEC);
		XBee2.print("-");
		for(it=0;it<packet->data_length;it++)
		{
			XBee2.print(packet->data[it],BYTE);
		}	
        }
	error=0;
	return error;
}

/*
 Function: Read incoming data via RF
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::readData(struct packetXSC* packet)
{
  uint8_t error=2;
  uint8_t end=0;  
  uint16_t interval=2000;
  long previous=millis();
  uint8_t counter3=0;
  uint8_t trama=0;
  uint8_t length=0;
  uint8_t dataIN[110];

  	for(int i=0;i<110;i++) dataIN[i]=0;
	previous=millis();
	while(end==0)
	{
		if( uart==UART0 ) 
        	{
		   if(XBee.available()>0)
		   {
			previous=millis();
			dataIN[counter3]=XBee.read();
			counter3++;
		   }
		
		   if( (millis()-previous) > interval )
		   {
			end=1;
			XBee.flush();
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		   } 
 		}
 		else if( uart==UART1 ) 
        	{
		   if(XBee2.available()>0)
		   {
			previous=millis();
			dataIN[counter3]=XBee2.read();
			counter3++;
		   }
		
		   if( (millis()-previous) > interval )
		   {
			end=1;
			XBee2.flush();
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		   } 
 		} 		
	}
	end=0;
	if(dataIN[0]==10) end=1;
	if( dataIN[8+end]!='-' ) return 1;
	packet->sourceMacHigh[0]=converter(dataIN[0+end],dataIN[1+end]);
	packet->sourceMacHigh[1]=converter(dataIN[2+end],dataIN[3+end]);
	packet->sourceMacLow[0]=converter(dataIN[4+end],dataIN[5+end]);
	packet->sourceMacLow[1]=converter(dataIN[6+end],dataIN[7+end]);
	it=9;
	while(dataIN[it]!='-')
	{
		length++;
		it++;
	}
	switch(length)
	{
		case 1: packet->data_length=converter(dataIN[9]);
			break;
		case 2:	packet->data_length=converter(dataIN[9],dataIN[10]);
			break;
		case 3:	packet->data_length=converter(dataIN[9],dataIN[10],dataIN[11]);
			break;
	}
	packet->data_length=Utils.dec2hex(packet->data_length);
	it=0;
	for(it=0;it<packet->data_length;it++)
	{
		packet->data[it]=char(dataIN[it+10+length]);
	}
	return 0;
}


/*
 Function: Transparent function. The user uint8_troduces an AT command within a string and the function executes it without knowing its meaning
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Parameters:
  atcommand : String to specify the AT command to execute
*/
uint8_t WaspXBeeXSC::sendCommandAT(char* atcommand)
{
	uint8_t error=1;
	uint8_t i=0;

	clean();
	if( uart==UART0 )
	{
		XBee.print("at");
		while( *atcommand!='#' )
		{
			XBee.print(*atcommand++,BYTE);
		}
		XBee.println("");
		delay(1000);
		while(XBee.available() > 0) 
		{
			commandAT[i]=XBee.read();
			i++;
			error=0;
		}
 	}
	else if( uart==UART1 )
	{
		XBee2.print("at");
		while( *atcommand!='#' )
		{
			XBee2.print(*atcommand++,BYTE);
		}
		XBee2.println("");
		delay(1000);
		while(XBee2.available() > 0) 
		{
			commandAT[i]=XBee2.read();
			i++;
			error=0;
		}
 	}
	return error;
}


/*
 Function: Connect XBee, activating switch in Waspmote
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::ON()
{
	uint8_t error=2;
	if( uart==UART0 )
	{
		XBee.begin();
		XBee.setMode(XBEE_ON);
	}
	else if( uart==UART1 )
	{
		XBee2.begin();
		XBee2.setMode(XBEE_ON);
	}
	delay(50);
	clean();
	error=0;
	return error;
}


/*
 Function: Set XBee to sleep, asserting PIN 9
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::sleep()
{
	uint8_t error=2;
	if( uart==UART0 )
	{
		pinMode(XBEE_SLEEP, OUTPUT);
		digitalWrite(XBEE_SLEEP,HIGH); 	
		XBee.close();
	}
	else if( uart==UART1 )
	{
		pinMode(DIGITAL7, OUTPUT);
		digitalWrite(DIGITAL7,HIGH); 	
		XBee2.close();
	}
	error=0;
	return error;
}


/*
 Function: Wake up XBee, de-asserting PIN 9
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeXSC::wake()
{
	uint8_t error=2;
	if( uart==UART0 )
	{
		pinMode(XBEE_SLEEP, OUTPUT);
		digitalWrite(XBEE_SLEEP,LOW); 
		XBee.begin();
	}
	else if( uart==UART1 )
	{
		pinMode(DIGITAL7, OUTPUT);
		digitalWrite(DIGITAL7,LOW); 
		XBee2.begin();
	}
	delay(50);
	clean();
	error=0;
	return error;
}


/*
 Function: Generates a decimal number from an ASCII character which was a number
 Returns: The generated number
*/
uint8_t WaspXBeeXSC::converter(uint8_t conv1)
{
  uint8_t aux=0;
  uint8_t resul=0;

	switch(conv1)
	{
		case 48: aux=0;
			 break;
		case 49: aux=1;
			 break;
		case 50: aux=2;
			 break;
		case 51: aux=3;
			 break;
		case 52: aux=4;
			 break;
		case 53: aux=5;
			 break;
		case 54: aux=6;
			 break;
		case 55: aux=7;
			 break;
		case 56: aux=8;
			 break;
		case 57: aux=9;
			 break;
	}
	resul=aux;
	return resul;
}


/*
 Function: Generates a decimal number from two ASCII characters which were numbers
 Returns: The generated number
*/
uint8_t WaspXBeeXSC::converter(uint8_t conv1, uint8_t conv2)
{
  uint8_t aux=0;
  uint8_t aux2=0;
  uint8_t resul=0;

	switch(conv1)
	{
		case 48: aux=0;
			 break;
		case 49: aux=1;
			 break;
		case 50: aux=2;
			 break;
		case 51: aux=3;
			 break;
		case 52: aux=4;
			 break;
		case 53: aux=5;
			 break;
		case 54: aux=6;
			 break;
		case 55: aux=7;
			 break;
		case 56: aux=8;
			 break;
		case 57: aux=9;
			 break;
		case 65: aux=10;
			 break;
		case 66: aux=11;
			 break;
		case 67: aux=12;
			 break;
		case 68: aux=13;
			 break;
		case 69: aux=14;
			 break;
		case 70: aux=15;
			 break;
	}
	switch(conv2)
	{
		case 48: aux2=0;
			 break;
		case 49: aux2=1;
			 break;
		case 50: aux2=2;
			 break;
		case 51: aux2=3;
			 break;
		case 52: aux2=4;
			 break;
		case 53: aux2=5;
			 break;
		case 54: aux2=6;
			 break;
		case 55: aux2=7;
			 break;
		case 56: aux2=8;
			 break;
		case 57: aux2=9;
			 break;
		case 65: aux2=10;
			 break;
		case 66: aux2=11;
			 break;
		case 67: aux2=12;
			 break;
		case 68: aux2=13;
			 break;
		case 69: aux2=14;
			 break;
		case 70: aux2=15;
			 break;
		default: aux2=100;
			 break;
	}
	if(aux2==100) // Only one character but we have treated two, so We have to fix it
	{
		resul=aux;
	}
	else
	{
		resul=(aux*16)+aux2;
	}
	return resul;
}

/*
 Function: Generates a decimal number from three ASCII characters which were numbers
 Returns: The generated number
*/
uint8_t WaspXBeeXSC::converter(uint8_t conv1, uint8_t conv2, uint8_t conv3)
{
  uint8_t aux=0;
  uint8_t aux2=0;
  uint8_t aux3=0;
  uint8_t resul=0;

	switch(conv1)
	{
		case 48: aux=0;
			 break;
		case 49: aux=1;
			 break;
		case 50: aux=2;
			 break;
		case 51: aux=3;
			 break;
		case 52: aux=4;
			 break;
		case 53: aux=5;
			 break;
		case 54: aux=6;
			 break;
		case 55: aux=7;
			 break;
		case 56: aux=8;
			 break;
		case 57: aux=9;
			 break;
	}
	switch(conv2)
	{
		case 48: aux2=0;
			 break;
		case 49: aux2=1;
			 break;
		case 50: aux2=2;
			 break;
		case 51: aux2=3;
			 break;
		case 52: aux2=4;
			 break;
		case 53: aux2=5;
			 break;
		case 54: aux2=6;
			 break;
		case 55: aux2=7;
			 break;
		case 56: aux2=8;
			 break;
		case 57: aux2=9;
			 break;
	}
	switch(conv3)
	{
		case 48: aux3=0;
			 break;
		case 49: aux3=1;
			 break;
		case 50: aux3=2;
			 break;
		case 51: aux3=3;
			 break;
		case 52: aux3=4;
			 break;
		case 53: aux3=5;
			 break;
		case 54: aux3=6;
			 break;
		case 55: aux3=7;
			 break;
		case 56: aux3=8;
			 break;
		case 57: aux3=9;
			 break;
	}
	resul=aux3*100+aux2*10+aux;
	return resul;
}

void WaspXBeeXSC::clean()
{
	uint8_t ByteIN=0;
	if( uart==UART0 )
	{
		while(XBee.available() > 0) 
		{
			ByteIN = XBee.read();
		}
	}
	else if( uart==UART1 )
	{
		while(XBee2.available() > 0) 
		{
			ByteIN = XBee2.read();
		}
	}
}

uint8_t WaspXBeeXSC::check()
{
	uint8_t ByteIN[10];
	uint8_t error=0;
	uint8_t i=0;
	uint8_t	counter=0;
	long previous=0;
	uint8_t end=0;
	long interval=1000;
	
	previous=millis();
	while(end==0)
	{
            if( uart==UART0 )
	    {
		if(XBee.available()>0)
		{
			ByteIN[i]=XBee.read();
			i++;
			previous=millis();
		}
		if( (millis()-previous) > interval )
		{
			end=1;
			XBee.flush();
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		} 
	    }
	    else if( uart==UART1 )
	    {
		if(XBee2.available()>0)
		{
			ByteIN[i]=XBee2.read();
			i++;
			previous=millis();
		}
		if( (millis()-previous) > interval )
		{
			end=1;
			XBee2.flush();
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		} 
	    }

	}
	counter=i;
	i=0;
	while(counter>i)
	{
		if(ByteIN[i]=='O')
		{
			if(ByteIN[i+1]=='K')
			{
				return 0;
			}
		}
		i++;		
	}
	return 1;
}

WaspXBeeXSC	xbeeXSC = WaspXBeeXSC();
