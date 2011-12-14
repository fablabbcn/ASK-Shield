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
 *  Version:		0.2
 *  Design:		David Gascón
 *  Implementation:	David Cuartielles, Alberto Bielsa, Yuri Carmona
 */
 

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspXBee::WaspXBee()
{
    _uart = 0;
}

// Public Methods //////////////////////////////////////////////////////////////

void WaspXBee::begin()
{
  beginSerial(XBEE_RATE, _uart);
  pinMode(XBEE_PW,OUTPUT);
}

void WaspXBee::begin(uint8_t uart, uint16_t speed)
{
	_uart = uart;
	beginSerial(speed, _uart);
	pinMode(XBEE_PW,OUTPUT);
}

void WaspXBee::begin(uint16_t speed)
{
	beginSerial(speed, _uart);
	pinMode(XBEE_PW,OUTPUT);
}

void WaspXBee::close()
{
  closeSerial(_uart);
}

void WaspXBee::setMode(uint8_t mode)
{
  _pwrMode = mode;
  // set the GPS in the defined power mode
  switch (_pwrMode)
  {
	case XBEE_ON:
		if(!_uart) digitalWrite(XBEE_PW,HIGH);
		else
		{			
			pinMode(DIGITAL6,OUTPUT);
			digitalWrite(DIGITAL6,HIGH);
			// FIXME: This will disappear in the final version
			pinMode(GPRS_PW,OUTPUT);
			digitalWrite(GPRS_PW,HIGH); 
		}
		break;
	
	case XBEE_OFF:
		if(!_uart) digitalWrite(XBEE_PW,LOW);
		else
		{
			Utils.setMux(MUX_TO_LOW,MUX_TO_LOW);
			pinMode(DIGITAL6,OUTPUT);
			digitalWrite(DIGITAL6,LOW);
			// FIXME: This will disappear in the final version
			pinMode(GPRS_PW,OUTPUT);
			digitalWrite(GPRS_PW,LOW); 
		}
		break;
	
	default:
		break;
  }
}

uint8_t WaspXBee::available()
{
  return serialAvailable( _uart);
}

int WaspXBee::read()
{
  return serialRead(_uart);
}

void WaspXBee::flush()
{
  serialFlush(_uart);
}

void WaspXBee::print(char c)
{
  printByte(c,  _uart);
}

void WaspXBee::print(const char c[])
{
  printString(c,  _uart);
}

void WaspXBee::print(uint8_t b)
{
  printByte(b,  _uart);
}

void WaspXBee::print(int n)
{
  print((long) n);
}

void WaspXBee::print(unsigned int n)
{
  print((unsigned long) n);
}

void WaspXBee::print(long n)
{
  if (n < 0) {
    print('-');
    n = -n;
  }
  printNumber(n, 10);
}

void WaspXBee::print(unsigned long n)
{
  printNumber(n, 10);
}

void WaspXBee::print(double n)
{
	printFloat(n, 10);
}

void WaspXBee::print(long n, int base)
{
  if (base == 0)
    print((char) n);
  else if (base == 10)
    print(n);
  else
    printNumber(n, base);
}

void WaspXBee::println()
{
  print('\r');
  print('\n');  
}

void WaspXBee::println(char c)
{
  print(c);
  println();  
}

void WaspXBee::println(const char c[])
{
  print(c);
  println();
}

void WaspXBee::println(uint8_t b)
{
  print(b);
  println();
}

void WaspXBee::println(int n)
{
  print(n);
  println();
}

void WaspXBee::println(long n)
{
  print(n);
  println();  
}

void WaspXBee::println(unsigned long n)
{
  print(n);
  println();  
}

void WaspXBee::println(long n, int base)
{
  print(n, base);
  println();
}

void WaspXBee::println(double n)
{
	print(n);
	println();
}

// Private Methods /////////////////////////////////////////////////////////////

void WaspXBee::printNumber(unsigned long n, uint8_t base)
{
  printIntegerInBase(n, base,  _uart);
}

void WaspXBee::printFloat(double number, uint8_t digits) 
{
  // Handle negative numbers
  if (number < 0.0)
  {
     print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint; 
  }
}

// Preinstantiate Objects //////////////////////////////////////////////////////

WaspXBee XBee = WaspXBee();
WaspXBee XBee2 = WaspXBee();

