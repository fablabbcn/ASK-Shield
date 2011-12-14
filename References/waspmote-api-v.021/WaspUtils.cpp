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
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
  

#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif

#include <inttypes.h>

/*
 * Constructor
 */
WaspUtils::WaspUtils (void)
{
}

/*
 * strtolong (str) - transform a string into a longint
 */
uint32_t WaspUtils::strtolong(const char* str)
{
    uint32_t l = 0;
    while(*str >= '0' && *str <= '9')
        l = l * 10 + (*str++ - '0');

    return l;
}

/*
 * sizeOf ( str ) - size of a string or array
 *
 * returns the size of a string or byte array, it makes a check
 * for end of string ('\0') and discounts 1 if needed
 */
int WaspUtils::sizeOf(const char* str)
{
  int cont = 0;
  while(*str++) cont++;
  if (*str == '\0')
    return cont--; // it will end with '\0', therefore we gotta take it out
  return cont;
}

/*
 * strCmp ( str1, str2, size ) - compare two strings
 *
 * returns 0 if str1 is equal to str2 for the first "size" characters, 
 * returns 1 otherwise
 */
uint8_t WaspUtils::strCmp(const char* str1, const char* str2, uint8_t size)
{
  uint8_t cmp = 0;
  for (int i = 0; i < size; i++)
    if (str1[i] != str2[i]) cmp = 1;
  return cmp;
}

/*
 * strCp ( str1, str2 ) - copy one string into another
 *
 * - str1: origin
 * - str2: target
 */
void WaspUtils::strCp(char* str1, char* str2)
{
  uint16_t size = sizeOf(str1);
  for (int i = 0; i < size; i++)
    str2[i] = str1[i];
}

/*
 * clearArguments (void) - empties the arguments[][] data matrix
 */
void WaspUtils::clearArguments(void)
{
  for (int index = 0; index < MAX_ARGS; index++)
    for (int count = 0; count < MAX_ARG_LENGTH; count++)
      arguments[index][count] = '\0';  // clear each cell in the arguments matrix
}

/*
 * strExplode (str, separator) - breaks a string into its arguments separated by "separators"
 */
void WaspUtils::strExplode(const char* str, char separator){
  int count = 0, index = 0, sIndex = 0;
  int theSize = Utils.sizeOf(str);
  clearArguments();
  while (theSize >= sIndex + 1){
     if (*str != separator) 
    {
      arguments[index][count] = *str;
      *str++;
      count++;
    } 
    else 
    {
      *str++; // jump over the separator
      count = 0;
      index++;
    }
    sIndex++;
  }
}




/*
 * parse_decimal (str) - get a number out of a string
 *
 * It gets a number out of a string. It gets to the 2nd decimal of the number.
 */
long WaspUtils::parse_decimal(char *str)
{
	bool isneg = *str == '-';
	if (isneg) *str++;
	unsigned long ret = 100UL * gpsatol(str);
	while (gpsisdigit(*str)) ++str;
	if (*str == '.')
	{
		if (gpsisdigit(str[1]))
		{
			ret += 10 * (str[1] - '0');
			if (gpsisdigit(str[2]))
				ret += str[2] - '0';
		}
	}
	return isneg ? -ret : ret;
}


/*
 * parse_degrees (str) - Gets the degree number out of a string
 *
 * It gets the degree number out of a string
 */
 double WaspUtils::parse_degrees(char *str)
{
	unsigned long left = gpsatol(str);
	unsigned long tenk_minutes = (left % 100UL) * 10000UL;
	while (gpsisdigit(*str)) ++str;
	if (*str == '.')
	{
		unsigned long mult = 1000;
		while (gpsisdigit(*++str))
		{
			tenk_minutes += mult * (*str - '0');
			mult /= 10;
		}
	}
	double aux=0.0;
	long aux2=0;
	aux=(double) tenk_minutes / 6;
	aux2=((left / 100) * 100000);
	aux = aux + aux2;
	return  aux;
}


/*
 * gpsatol (str) - get the integer part of a number out of a string
 *
 * It gets the integer part of a number out of a string
 */
long WaspUtils::gpsatol(char *str)
{
	long ret = 0;
	while (gpsisdigit(*str))
		ret = 10 * ret + *str++ - '0';
	return ret;
}

// It parses latitude or longitude, getting the number out of a string
double WaspUtils::parse_latitude(char *str)
{
	double aux=0;
	double aux2=0.0;
	
	aux=(double) parse_degrees(str);
	aux2=(double) aux/100000;
	aux2=aux2*M_PI/180;
	return aux2;
}


/* setLED(led, state) - set the specified LED to the specified state(ON or OFF)
 *
 * It sets the specified LED to the specified state(ON or OFF)
 */
void	WaspUtils::setLED(uint8_t led, uint8_t state)
{
	pinMode(led,OUTPUT);
	if(state==LED_ON)
	{
		digitalWrite(led,HIGH);
	}
	if(state==LED_OFF)
	{
		digitalWrite(led,LOW);
	}
}


/* getLED(led) - gets the state of the specified LED
 *
 * It gets the state of the specified LED
 */
uint8_t	WaspUtils::getLED(uint8_t led)
{
	return digitalRead(led);
}


/* blinkLEDs(time) - blinks LED, with the specified time of blinking
 *
 * It bliks LED0 and LED1, with the specified time of blinking
 */
void WaspUtils::blinkLEDs(uint16_t time) 
{
	setLED(LED0,LED_ON);
	setLED(LED1,LED_ON);
	delay(time);
	setLED(LED0,LED_OFF);
	setLED(LED1,LED_OFF);  
	delay(time);
}


/* map(x,in_min,in_max,out_min,out_max) - maps 'x' from the read range to the specified range
 *
 * It maps 'x' from the read range to the specified range
 *
 * 'in_min' and 'in_max' are the entry range read from the sensor
 * 'out_min' and 'out_max' are the desired output range
 */
long WaspUtils::map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/* setMux( MUX_LOW, MUX_HIGH) - sets MUX to the desired combination
 *
 * It sets MUX to the desired combination.
 *
 * --------------
 * MUX_LOW = 0 & MUX_HIGH = 1 ---> GPS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 1 ---> GPRS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 0 ---> AUX1 MODULE
 * MUX_LOW = 0 & MUX_HIGH = 0 ---> AUX2 MODULE
 */
void WaspUtils::setMux(uint8_t MUX_LOW, uint8_t MUX_HIGH)
{
	pinMode(MUX_PW, OUTPUT);
	pinMode(MUX0, OUTPUT);      
	pinMode(MUX1, OUTPUT);   
	digitalWrite(MUX_PW, HIGH);   
	digitalWrite(MUX0, MUX_LOW);
	digitalWrite(MUX1, MUX_HIGH);
}


/* setMuxGPS() - sets MUX to the desired combination (0,1)
 *
 * It sets MUX to the desired combination (0,1) to enable GPS module
 *
 * --------------
 * MUX_LOW = 0 & MUX_HIGH = 1 ---> GPS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 1 ---> GPRS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 0 ---> AUX1 MODULE
 * MUX_LOW = 0 & MUX_HIGH = 0 ---> AUX2 MODULE
 */
void WaspUtils::setMuxGPS()
{
	pinMode(MUX_PW, OUTPUT);
	pinMode(MUX0, OUTPUT);      
	pinMode(MUX1, OUTPUT);   
	digitalWrite(MUX_PW, HIGH);   
	digitalWrite(MUX0, LOW);
	digitalWrite(MUX1, HIGH);
}


/* setMuxGPRS() - sets MUX to the desired combination (1,1)
 *
 * It sets MUX to the desired combination (1,1) to enable GPRS module
 *
 * --------------
 * MUX_LOW = 0 & MUX_HIGH = 1 ---> GPS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 1 ---> GPRS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 0 ---> AUX1 MODULE
 * MUX_LOW = 0 & MUX_HIGH = 0 ---> AUX2 MODULE
 */
void WaspUtils::setMuxGPRS()
{
	pinMode(MUX_PW, OUTPUT);
	pinMode(MUX0, OUTPUT);      
	pinMode(MUX1, OUTPUT);   
	digitalWrite(MUX_PW, HIGH);   
	digitalWrite(MUX0, HIGH);
	digitalWrite(MUX1, HIGH);
}


/* setMuxAux1() - sets MUX to the desired combination (1,1)
 *
 * It sets MUX to the desired combination (1,1) to enable GPRS module
 *
 * --------------
 * MUX_LOW = 0 & MUX_HIGH = 1 ---> GPS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 1 ---> GPRS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 0 ---> AUX1 MODULE
 * MUX_LOW = 0 & MUX_HIGH = 0 ---> AUX2 MODULE
 */
void WaspUtils::setMuxAux1()
{
	pinMode(MUX_PW, OUTPUT);
	pinMode(MUX0, OUTPUT);      
	pinMode(MUX1, OUTPUT);   
	digitalWrite(MUX_PW, HIGH);   
	digitalWrite(MUX0, HIGH);
	digitalWrite(MUX1, LOW);
}


/* setMuxAux2() - sets MUX to the desired combination (1,1)
 *
 * It sets MUX to the desired combination (1,1) to enable GPRS module
 *
 * --------------
 * MUX_LOW = 0 & MUX_HIGH = 1 ---> GPS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 1 ---> GPRS MODULE
 * MUX_LOW = 1 & MUX_HIGH = 0 ---> AUX1 MODULE
 * MUX_LOW = 0 & MUX_HIGH = 0 ---> AUX2 MODULE
 */
void WaspUtils::setMuxAux2()
{
	pinMode(MUX_PW, OUTPUT);
	pinMode(MUX0, OUTPUT);      
	pinMode(MUX1, OUTPUT);   
	digitalWrite(MUX_PW, HIGH);   
	digitalWrite(MUX0, LOW);
	digitalWrite(MUX1, LOW);
}


/* readEEPROM(address) - reads from the EEPROM specified address
 *
 * It reads from the EEPROM specified address
 * 
 * EEPROM has 512 Bytes of memory
 */
uint8_t WaspUtils::readEEPROM(int address)
{
	return eeprom_read_byte((unsigned char *) address);
}


/* writeEEPROM(address,value) - writes the specified value into the specified address
 *
 * It writes the specified value into the specified address
 * 
 * EEPROM has 512 Bytes of memory
 */
void WaspUtils::writeEEPROM(int address, uint8_t value)
{
	eeprom_write_byte((unsigned char *) address, value);
}

uint8_t WaspUtils::dec2hex(uint8_t number)
{
	int aux=0, aux2=0;
	aux=number/16;
	aux2=number-(aux*16);
	return (aux*10+aux2);
}


void WaspUtils::hex2str(uint8_t* number, char* macDest)
{
	hex2str(number,macDest,8);
}


void WaspUtils::hex2str(uint8_t* number, char* macDest, uint8_t length)
{
	uint8_t aux_1=0;
	uint8_t aux_2=0;

	for(int i=0;i<length;i++){
		aux_1=number[i]/16;
		aux_2=number[i]%16;
		if (aux_1<10){
			macDest[2*i]=aux_1+'0';
		}
		else{
			macDest[2*i]=aux_1+('A'-10);
		}
		if (aux_2<10){
			macDest[2*i+1]=aux_2+'0';
		}
		else{
			macDest[2*i+1]=aux_2+('A'-10);
		}
	} 
	macDest[length*2]='\0';
}


/*
 Function: Converts a number stored in an array to a long
*/
long WaspUtils::array2long(char* num)
{
	int j=0;
	long resul=0;
	long aux=1;
	uint8_t counter=0;
  
	while( (num[counter]>='0') && (num[counter]<='9') ){
		counter++;
	}
	while( (num[j]>='0') && (num[j]<='9') ){
		for(int a=0;a<counter-1;a++)
		{
			aux=aux*10;
		}
		resul=resul+(num[j]-'0')*aux;
		counter--;
		j++;
		aux=1;
	}
	return resul;
}


/*
 Function: Coverts a long to a number stored in an array
*/
uint8_t WaspUtils::long2array(long num, char* numb)
{
	long aux=num;
	uint8_t i=0;
	
	if( num<0 )
	{
		num = ~(num);
		num+=1;
		numb[i]='-';
		i++;
	}
	aux=num;
	while(aux>=10)
	{
		aux=aux/10;
		i++;
	}
	numb[i+1]='\0';
	aux=num;
	while(aux>=10)
	{
		numb[i]=aux%10 + '0';
		aux=aux/10;
		i--;
	}
	numb[i]=aux + '0';
	return i;
}

/*
  Function: Converts a string to an hex number
  */
uint8_t WaspUtils::str2hex(char* str)
{
	int aux=0, aux2=0;
	
	
	if( (*str>='0') && (*str<='9') )
	{
		aux=*str++-'0';
	}
	else if( (*str>='A') && (*str<='F') )
	{
		aux=*str++-'A'+10;
	}
	if( (*str>='0') && (*str<='9') )
	{
		aux2=*str-'0';
	}
	else if( (*str>='A') && (*str<='F') )
	{
		aux2=*str-'A'+10;
	}
	return aux*16+aux2;
}


/*
  Function: Converts a string to an hex number
  */
uint8_t WaspUtils::str2hex(uint8_t* str)
{
	int aux=0, aux2=0;
	
	
	if( (*str>='0') && (*str<='9') )
	{
		aux=*str++-'0';
	}
	else if( (*str>='A') && (*str<='F') )
	{
		aux=*str++-'A'+10;
	}
	if( (*str>='0') && (*str<='9') )
	{
		aux2=*str-'0';
	}
	else if( (*str>='A') && (*str<='F') )
	{
		aux2=*str-'A'+10;
	}
	return aux*16+aux2;
}

/*
 Function: Generates a decimal number from two ASCII characters which were numbers
 Returns: The generated number
*/
uint8_t WaspUtils::converter(uint8_t conv1, uint8_t conv2)
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


void WaspUtils::float2String (float fl, char str[], int N) {

	boolean neg = false;
 
	if( fl<0 ){
		neg = true;
		fl*=-1;
	}
 
	float numeroFloat=fl; 
	int parteEntera[10];
	int cifra; 
	long numero=(long)numeroFloat;  
	int size=0;
  
	while(1){
		size=size+1;
		cifra=numero%10;
		numero=numero/10;
		parteEntera[size-1]=cifra; 
		if (numero==0){
			break;
		}
	}

	int indice=0;
	if( neg ){
		indice++;
		str[0]='-';
	}
	for (int i=size-1; i>=0; i--)
	{
		str[indice]=parteEntera[i]+'0'; 
		indice++;
	}

	str[indice]='.';
	indice++;

	numeroFloat=(numeroFloat-(int)numeroFloat);
	for (int i=1; i<=N ; i++)
	{
		numeroFloat=numeroFloat*10;
		cifra= (long)numeroFloat;          
		numeroFloat=numeroFloat-cifra;
		str[indice]=char(cifra)+48;
		indice++;
	}
	str[indice]='\0';
}


WaspUtils Utils = WaspUtils();
