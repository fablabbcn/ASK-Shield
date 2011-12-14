/*! \file WaspUSB.h
    \brief Library for managing the USB interface
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
  
    Version:		0.1

    Design:		David Gascón

    Implementation:	David Cuartielles, Alberto Bielsa

 */
 
/*! \def WaspUSB_h
    \brief The library flag
    
 */
#ifndef WaspUSB_h
#define WaspUSB_h


/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <inttypes.h>


/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def DEC
    \brief The base to print numbers. Decimal Base in this case
    
 */
/*! \def HEX
    \brief The base to print numbers. Hexadecimal Base in this case
    
 */
/*! \def OCT
    \brief The base to print numbers. Octal Base in this case
    
 */
/*! \def BIN
    \brief The base to print numbers. Binary Base in this case
    
 */
/*! \def BYTE
    \brief The base to print numbers. Byte Base in this case
    
 */
#define DEC 	10
#define HEX 	16
#define OCT 	8
#define BIN 	2
#define BYTE 	0

/*! \def USB_RATE
    \brief USB Baud Rate
    
 */
#define USB_RATE	38400


/******************************************************************************
 * Class
 ******************************************************************************/
 
 //! WaspUSB Class
/*!
	WaspUSB Class defines all the variables and functions used for managing the USB interface
 */
class WaspUSB
{
  private:
	
	//! Variable : specifies the UART where the USB is connected
  	/*!    
	 */
    	uint8_t _uart;
	
	//! It prints a number in the specified base
  	/*!
	\param unsigned long n : the number to print
	\param uint8_t base : the base for printing the number
	\return void
	 */
    	void printNumber(unsigned long n, uint8_t base);
	
	//! It prints a 'float' number
  	/*!
	\param double number : the number to print
	\param uint8_t digits : the number of non-integer part digits
	\return void
	 */
    	void printFloat(double number, uint8_t digits);
  public:
	  
	//! class constructor
  	/*!
		It initializes some variables
	\param void
	\return void
	*/
    	WaspUSB();
	
	//! It opens UART to be able to communicate with the FTDI conversor
  	/*!
		It gets the baud rate from 'USB_RATE'
	\param void
	\return void
	 */
    	void begin();
	
	//! It closes the previously opened UART
  	/*!
	\param void
	\return void
	 */
    	void close();
	
	//! It checks if there is available data waiting to be read
  	/*!
	\param void
	\return '1' if there is available data, '0' otherwise
	 */
	uint8_t available();
	
	//! It reads a byte from the UART
  	/*!
	\param void
	\return the read byte or '-1' if no data is available
	 */
	int read();
	
	//! It clears the UART buffer
  	/*!
	\param void
	\return void
	 */
	void flush();
	
	//! It prints a character
  	/*!
	\param char c : the character to print
	\return void
	 */
	void print(char c);
	
	//! It prints a string
  	/*!
	\param const char[] c : the string to print
	\return void
	 */
	void print(const char[]);
	
	//! It prints an unsigned 8-bit integer
  	/*!
	\param uint8_t b : the number to print
	\return void
	 */
	void print(uint8_t b);
	
	//! It prints an integer
  	/*!
	\param int n : the number to print
	\return void
	 */
	void print(int n);
	
	//! It prints an unsigned integer
  	/*!
	\param unsigned int n : the number to print
	\return void
	 */
	void print(unsigned int n);
	
	//! It prints a long integer
  	/*!
	\param long n : the number to print
	\return void
	 */
	void print(long n);
	
	//! It prints an unsigned long integer
  	/*!
	\param unsigned long n : the number to print
	\return void
	 */
	void print(unsigned long n);
	
	//! It prints a long number in the specified base
  	/*!
	\param long n : the number to print
	\param int base : the base for printing the number
	\return void
	 */
	void print(long n, int base);
	
	//! It prints a double number
  	/*!
	\param double n : the number to print
	\return void
	 */
	void print(double n);
	
	//! It prints a 64-bit number
  	/*!
	\param uint64_t n : the number to print
	\return void
	 */
	void print(uint64_t n);
	
	//! It prints an EOL and a carriage return
  	/*!
	\param void
	\return void
	 */
	void println();
	
	//! It prints a character adding an EOL and a carriage return
  	/*!
	\param char c : the character to print
	\return void
	 */
	void println(char c);
	
	//! It prints a string adding an EOL and a carriage return
  	/*!
	\param const char[] c : the string to print
	\return void
	 */
	void println(const char[]);
	
	//! It prints an unsigned 8-bit integer adding an EOL and a carriage return
  	/*!
	\param uint8_t b : the number to print
	\return void
	 */
	void println(uint8_t b);
	
	//! It prints an integer adding an EOL and a carriage return
  	/*!
	\param int n : the number to print
	\return void
	 */
	void println(int n);
	
	//! It prints a long integer adding an EOL and a carriage return
  	/*!
	\param long n : the number to print
	\return void
	 */
	void println(long n);
	
	//! It prints an unsigned long integer adding an EOL and a carriage return
  	/*!
	\param unsigned long n : the number to print
	\return void
	 */
	void println(unsigned long n);
	
	//! It prints a long number in the specified base adding an EOL and a carriage return
  	/*!
	\param long n : the number to print
	\param int base : the base for printing the number
	\return void
	 */
	void println(long n, int base);
	
	//! It prints a double number adding an EOL and a carriage return
  	/*!
	\param double n : the number to print
	\return void
	 */
	void println(double n);
	
	//! It prints a 64-bit number adding an EOL and a carriage return
  	/*!
	\param uint64_t n : the number to print
	\return void
	 */
	void println(uint64_t n);
};

extern WaspUSB USB;

#endif

