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
 *  Version:		0.1
 *  Design:		David Gascón
 *  Implementation:	David Cuartielles, Alberto Bielsa
 */
 

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspUSB::WaspUSB()
{
    _uart = 0;
}

// Public Methods //////////////////////////////////////////////////////////////

void WaspUSB::begin()
{
  beginSerial(USB_RATE, _uart);
}

void WaspUSB::close()
{
  closeSerial(_uart);
}

uint8_t WaspUSB::available()
{
  return serialAvailable( _uart);
}

int WaspUSB::read()
{
  return serialRead( _uart);
}

void WaspUSB::flush()
{
  serialFlush( _uart);
}

void WaspUSB::print(char c)
{
  printByte(c,  _uart);
}

void WaspUSB::print(const char c[])
{
  printString(c,  _uart);
}

void WaspUSB::print(uint8_t b)
{
  printByte(b,  _uart);
}

void WaspUSB::print(int n)
{
  print((long) n);
}

void WaspUSB::print(unsigned int n)
{
  print((unsigned long) n);
}

void WaspUSB::print(long n)
{
  if (n < 0) {
    print('-');
    n = -n;
  }
  printNumber(n, 10);
}

void WaspUSB::print(unsigned long n)
{
  printNumber(n, 10);
}

void WaspUSB::print(long n, int base)
{
  if (base == 0)
    print((char) n);
  else if (base == 10)
    print(n);
  else
    printNumber(n, base);
}

void WaspUSB::print(double n)
{
  printFloat(n, 10);
}

void WaspUSB::print(uint64_t n)
{
	printInteger(n,0);
}

void WaspUSB::println()
{
  print('\r');
  print('\n');  
}

void WaspUSB::println(char c)
{
  print(c);
  println();  
}

void WaspUSB::println(const char c[])
{
  print(c);
  println();
}

void WaspUSB::println(uint8_t b)
{
  print(b);
  println();
}

void WaspUSB::println(int n)
{
  print(n);
  println();
}

void WaspUSB::println(long n)
{
  print(n);
  println();  
}

void WaspUSB::println(unsigned long n)
{
  print(n);
  println();  
}

void WaspUSB::println(long n, int base)
{
  print(n, base);
  println();
}

void WaspUSB::println(double n)
{
  print(n);
  println();
}

void WaspUSB::println(uint64_t n)
{
	printInteger(n,0);
	println();
}

// Private Methods /////////////////////////////////////////////////////////////

void WaspUSB::printNumber(unsigned long n, uint8_t base)
{
  printIntegerInBase(n, base,  _uart);
}

void WaspUSB::printFloat(double number, uint8_t digits) 
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

WaspUSB USB = WaspUSB();

