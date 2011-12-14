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
 *  Version:		0.13
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
  

#ifndef __WPROGRAM_H__
//  #include <WProgram.h>
  #include "WaspClasses.h"
#endif

// Constructors ////////////////////////////////////////////////////////////////

WaspGPRS::WaspGPRS()
{
	_baudRate=XBEE_RATE;
	_pwrMode=GPRS_ON;
	_uart=1;
	not_ready=1;
}


// Private Methods /////////////////////////////////////////////////////////////

/* setPattern(pattern) - sets pattern to use in sending and receiving messages
 *
 * This function sets pattern to use in sending and receiving messages
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setPattern(const char* pattern)
{
	char* command = (char*) calloc(25,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_PATTERN);
	
	sprintf(command,"%s%c%s%c",AT_GPRS_PATTERN,aux,pattern,aux);
	
	answer=sendATCommand(command,AT_GPRS_PATTERN_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_PATTERN;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_PATTERN;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_PATTERN) return 0;
	return 1;
}

/* setConnectionTimer() - sets GPRS connection time out
 *
 * This function sets GPRS connection time out
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setConnectionTimer()
{
	char* command = (char*) calloc(25,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_TIMER);
	
	sprintf(command,"%s0,120,2,120",AT_GPRS_CNX_TIMER);
	
	answer=sendATCommand(command,AT_GPRS_CNX_TIMER_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_TIMER;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_TIMER;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_TIMER) return 0;
	return 1;
}


/* checkGPRS() - checks if GPRS connection is OK
 *
 * This function checks if GPRS connection is OK
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
uint8_t WaspGPRS::checkGPRS()
{
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_CHECK);
		
	answer=sendATCommand(AT_GPRS_CHECK,AT_GPRS_CHECK_R);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CHECK;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_CHECK;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_CHECK) return 0;
	return 1;
}


/* setConnectionProfile() - sets GPRS connection profile
 *
 * This function sets GPRS connection profile
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
uint8_t WaspGPRS::setConnectionProfile()
{
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_PROFILE);
		
	answer=sendATCommand(AT_GPRS_CNX_PROFILE,AT_GPRS_CNX_PROFILE_R);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_PROFILE;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_PROFILE;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_PROFILE) return 0;
	return 1;
}

/* setFlowControl() - sets GPRS flow control
 *
 * This function sets GPRS flow control
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setFlowControl()
{
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_SMTP);
		
	answer=sendATCommand(AT_GPRS_K3,AT_GPRS_K3_R);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMTP) return 0;
	return 1;
}


/* writeData(data,start) - writes data to send
 *
 * This function writes data to send
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::writeData(const char* data)
{
	char* command = (char*) calloc(GPRS_MAX_DATA+15,sizeof(char));
	if( command==NULL ) return -1;
	
	sprintf(command,"%s%c%c%s",data,'\r','\n',GPRS_PATTERN);
	printString(command,PORT_USED);
	free(command);
	return 1;
}


/* parse_GSM(data) - parses GSM string and specifies if it is a call or an sms
 *
 * This function writes data to send
 *
 * Returns '0' on GSM incoming call, '1' on GSM incoming SMS, '2' if error
*/
uint8_t WaspGPRS::parse_GSM(const char* data)
{
	uint8_t a=0;
	char cmp[5];
	
	while( (data[a]!='+') && (a<=45) ){
		a++;
	}
	a++;
	for(int b=0;b<4;b++)
	{
		cmp[b]=data[a];
		a++;
	}
	cmp[4]='\0';
	if(!strcmp(cmp,"CLIP")) return 0;
	if(!strcmp(cmp,"CMTI")) return 1;
	return 2;
}

/* setEmailParams(smtp_server, port, to) - sets the email parameters
 *
 * This function sets the email parameters
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setEmailParams(const char* smtp_server, uint16_t port, const char* to)
{
	char* command = (char*) calloc(100,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	sprintf(command,"%s%c%s%c,%u,%c%s%c",AT_SMTP_PARAM,aux,smtp_server,aux,port,aux,to,aux);
	answer=sendATCommand(command,AT_SMTP_PARAM_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMTP) return 0;
	return 1;
}

/* setEmailPwd(user, pwd) - sets the email user and password
 *
 * This function sets the email user and password
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setEmailPwd(const char* user, const char* pwd)
{
	char* command = (char*) calloc(100,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	sprintf(command,"%s%c%s%c,%c%s%c",AT_SMTP_PWD,aux,user,aux,aux,pwd,aux);
	answer=sendATCommand(command,AT_SMTP_PWD_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMTP) return 0;
	return 1;
}

/* setEmailDestination(destination) - sets the email destination
 *
 * This function sets the email destination
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setEmailDestination(const char* destination)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	sprintf(command,"%s%c%s%c",AT_SMTP_TO,aux,destination,aux);
	answer=sendATCommand(command,AT_SMTP_TO_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMTP) return 0;
	return 1;
}

/* setEmailSubject(subject) - sets the email subject
 *
 * This function sets the email subject
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::setEmailSubject(const char* subject)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	sprintf(command,"%s%c%s%c",AT_SMTP_SUBJECT,aux,subject,aux);
	answer=sendATCommand(command,AT_SMTP_SUBJECT_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMTP;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMTP) return 0;
	return 1;
}


// AT Comands ///////////////////////////////////////////////////////////////////

byte WaspGPRS::sendATCommand(const char* ATcommand, const char* expectedAnswer) {
    return sendATCommand(ATcommand, expectedAnswer, 0);
}
  
byte WaspGPRS::sendATCommand(const char* ATcommand, const char* expectedAnswer, int sendOnce) {
  // command style: +CMGS=0736584317
  // this means, we gotta add the "AT"

    sprintf(theCommand, "AT%s", ATcommand);
    return sendCommand(theCommand, "\r\n", expectedAnswer, sendOnce);
}

byte WaspGPRS::sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer) {
    return sendCommand(theText, endOfCommand, expectedAnswer, 0);
}

byte WaspGPRS::sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce) {
    sprintf(theEnd, "%c", endOfCommand);
    return sendCommand(theText, theEnd, expectedAnswer, MAX_TIMEOUT, sendOnce);
}

byte WaspGPRS::sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer, int sendOnce) {
    sprintf(theEnd, "%c", endOfCommand);
    return sendCommand(theText, theEnd, expectedAnswer, sendOnce);
}

byte WaspGPRS::sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer) {
    return sendCommand(theText, (int) endOfCommand, expectedAnswer, 0);
}

byte WaspGPRS::sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT) {
    return sendCommand(theText, (int) endOfCommand, expectedAnswer, MAX_TIMEOUT, 0);
}

byte WaspGPRS::sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce) {
    return sendCommand(theText, (int) endOfCommand, expectedAnswer, MAX_TIMEOUT, sendOnce);
}

byte WaspGPRS::sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer) {
    return sendCommand(theText, endOfCommand, expectedAnswer, DEFAULT_TIMEOUT, 0);
}

byte WaspGPRS::sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer, int sendOnce) {
    return sendCommand(theText, endOfCommand, expectedAnswer, DEFAULT_TIMEOUT, sendOnce);
}

byte WaspGPRS::sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce) {
    int timeout = 0;

    int length=sprintf(theCommand, "%s%s", theText,endOfCommand);
    

  // try sending the command
  // wait for serial response
    timeout = 0;
    serialFlush(PORT_USED);
    while(!serialAvailable(PORT_USED) && timeout < MAX_TIMEOUT) {
        if (!sendOnce || !timeout) {
            printString(theCommand,PORT_USED);
        }
        delay(DELAY_ON_SEND);
        timeout++;
    };

    int answer= waitForData( expectedAnswer, MAX_TIMEOUT, timeout, 0);
    
    return answer;
}


byte WaspGPRS::waitForData(const char* expectedAnswer, int MAX_TIMEOUT, int timeout, int seconds) {

	for (int i = 0; i < 100; i++) theCommand[i] = ' ';

	int theLength = 0;
	int it=0;
	bool theSame=false;	
	uint8_t first=1;
	uint8_t match=0;
    
	while( expectedAnswer[theLength]!='\0' ) theLength++;
		
	// if there is a heating time, then wait to see if you got
	// any data from the serial port
	while (seconds >0) {
		delay(1000);
		seconds--;
	}
		
	while(timeout < MAX_TIMEOUT) {
		while(!serialAvailable(PORT_USED) && timeout < MAX_TIMEOUT) {
			timeout++;
			delay(1000);
		}
		
		while( serialAvailable(PORT_USED) && !match )
		{
			if( first )
			{
				for(it=0;it<theLength;it++)
				{
					theCommand[it]=serialRead(PORT_USED);
					delay(20);
				}
				first=0;
			}
			it=0;
			
			if( serialAvailable(PORT_USED) )
			{
				theSame=true;
				for(it=0; it<theLength ; it++)
				{
					if(theCommand[it]!=expectedAnswer[it]){
						theSame= false;
						break;
					}
				}
				if( theSame ){
					match=1;
					return 1;
				}
				else
				{
					for(it=0; it<theLength-1 ; it++)
					{
						theCommand[it]=theCommand[it+1];
					}
					theCommand[it]=serialRead(PORT_USED);
					delay(20);
				}
			}
		}
	}
	if( match ) return 1;
	else return 0;
}

uint16_t WaspGPRS::waitForData(const char* data, const char* expectedAnswer)
{
	uint16_t i=0;
	for (i = 0; i < 100; i++) theCommand[i] = ' ';
	int theLength = 0;
	int it=0;
	bool theSame=false;
	uint8_t first=1;
	uint8_t match=0;
	i=0;
    
	while( expectedAnswer[theLength]!='\0' ) theLength++;
	
	while( !match && data[i]!='\0' )
	{
		if( first )
		{
			for(it=0;it<theLength;it++)
			{
				theCommand[it]=data[i];
				i++;

			}
			first=0;
		}
		it=0;
		theSame=true;
		for(it=0; it<theLength ; it++)
		{
			if(theCommand[it]!=expectedAnswer[it]){
				theSame= false;
				break;
			}
		}
		if( theSame ) match=1;
		else
		{
			for(it=0; it<theLength-1 ; it++)
			{
				theCommand[it]=theCommand[it+1];
			}
			theCommand[it]=data[i];
			i++;
		}
	}
	
	if( !match ) i=0;
	return i;
}

int8_t WaspGPRS::sendDataFTP(const char* file, const char* path, uint8_t id)
{
	char* command = (char*) calloc(100,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	long previous=0;
	uint8_t answer=0;
	uint8_t end=0;
	uint32_t i,j=0;
			
	sprintf(command,"AT%s%c,,%c%c,%c%s%c,0%c%c", AT_FTP_SEND, id, aux, aux, aux, file, aux, '\r', '\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("CONNECT",20,0,0);
	if(answer!=1) return 0;
		
	serialFlush(PORT_USED);
		
	Utils.strExplode(path,'/');
	while( path[i]!='\0' )
	{
		if( path[i]== '/' ) j++;
		i++;
	}
	i=0;
	
	SD.ON();
	
	while( j>0 )
	{
		if(!SD.cd(Utils.arguments[i])){
			SD.OFF();
			return 0;
		}
		i++;
		j--;
	}
	i=0;
	j=0;
	while( !end )
	{
		printString(SD.cat(file,250*i,250),PORT_USED);
		while( SD.buffer[j]!='\0' ) j++;
		if( j<249 ) end=1;
		i++;
		j=0;
	}
	printString(GPRS_PATTERN,PORT_USED);
		
	SD.OFF();
	
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("OK",20,0,0);
	if(answer!=1) return 0;
	
	return 1;	
}


int8_t WaspGPRS::readDataFTP(const char* file, const char* path, uint8_t id)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	long previous=0;
	uint8_t answer=0;
	int i,j=0;
	char* aux2;
	uint16_t length=0;

	sprintf(command,"AT%s%c,,%c%c,%c%s%c,0%c%c", AT_FTP_RECV, id, aux, aux, aux, file, aux, '\r', '\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("CONNECT",30,0,0);
	if(answer!=1)	return 0;
	
	delay(20);
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}	
	aux2 = (char*) calloc(MAX_SIZE_FTP,sizeof(char));
	if( aux2==NULL ) return 0;
		
	serialRead(PORT_USED);
	serialRead(PORT_USED);
	previous=millis();
	i=0;
	while( millis()-previous<5000 )
	{
		while( serialAvailable(PORT_USED) && i<MAX_SIZE_FTP )
		{
			aux2[i]=serialRead(PORT_USED);
			i++;
			previous=millis();
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	
	}
	if( i>=MAX_SIZE_FTP ) aux2[i-1]='\0';
	else aux2[i]='\0';

	length=waitForData(aux2,GPRS_PATTERN);
	if( !length ) length=i;
	else
	{
		j=Utils.sizeOf(GPRS_PATTERN);
		length-=j;
	}
	
	i=0;
	j=0;
	
	Utils.strExplode(path,'/');
	while( path[i]!='\0' )
	{
		if( path[i]== '/' ) j++;
		i++;
	}
	i=0;
	
	SD.ON();
	
	while( j>0 )
	{
		if(!SD.cd(Utils.arguments[i])){
			SD.OFF();
			free(aux2);
			aux2=NULL; 
			return 0;
		}
		i++;
		j--;
	}
	SD.create(Utils.arguments[i]);
	
	if(!SD.append(Utils.arguments[i],aux2,length)){
		SD.OFF();
		free(aux2);
		aux2=NULL; 
		return 0;
	}
	
	SD.ls();
		
	SD.OFF();
	
	free(aux2);
	aux2=NULL;
	return 1;	
}

/* getIfReady() - gets if GPRS module is ready or not
 *
 * This function gets if GPRS module is ready or not
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns nothing. It changes the value of 'not_ready'
*/
void	WaspGPRS::getIfReady()
{
	uint8_t answer=0;
	
	printString(AT_COMMAND,PORT_USED);
	printByte('\r',PORT_USED);
	printByte('\n',PORT_USED);
	delay(10);
	answer=waitForData("OK",2,0,0);
	if(answer==1) not_ready=0;
	else not_ready=1;
}


// Public Methods //////////////////////////////////////////////////////////////

/* ON(void) - opens UART1 and powers the GPRS module
 *
 * It opens UART1 and powers the GPRS module
 *
 * Returns nothing
*/
void WaspGPRS::ON()
{
	disableInterrupts(HAI_INT);
	disableInterrupts(LAI_INT);
	if( !RTC.isON ) RTC.setMode(RTC_ON, RTC_I2C_MODE);
	begin();
	setMode(GPRS_ON);
}


/* begin(void) - initialize GPRS module
 *
 * This function powers up the GPRS module and open Serial Port at velocity selected by the user
 * By default, it will be at 38400bps
 *
 * Returns nothing
*/
void WaspGPRS::begin()
{
	_baudRate=GPRS_RATE;
	Utils.setMuxGPRS();
	beginSerial(_baudRate,_uart);
}


/* close(void) - closes UART used by GPRS module
 *
 * This function closes UART used by GPRS module
 *
 * Returns nothing
*/
void WaspGPRS::close()
{
	closeSerial(_uart);
	Utils.setMux(MUX_TO_LOW,MUX_TO_LOW);
}

/* OFF(void) - closes UART1 and powers off the GPRS module
 *
 * This function closes UART1 and powers off the GPRS module
 *
 * Returns nothing
*/
void WaspGPRS::OFF()
{
        setMode(GPRS_OFF);
	close();
	if( RTC.isON==2 ){
		PWR.closeI2C();
		RTC.setMode(RTC_OFF, RTC_I2C_MODE);
	}
}


/* setMode(uint8_t) - Sets GPRS Power Mode
 *
 * This function selects the active power mode among: ON, SLEEP/HIBERNATE and OFF
 * It does not close the serial port, only sends the proper command to GPRS module
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
uint8_t WaspGPRS::setMode(uint8_t pwrMode)
{
	_pwrMode=pwrMode;
	flag &= ~(GPRS_ERROR_POWER);
	uint8_t answer=0;
    
	switch(_pwrMode)
	{
		case    GPRS_ON  	:	pinMode(DIGITAL6,OUTPUT);
                                                digitalWrite(DIGITAL6,HIGH);
                                                pinMode(GPRS_PW, OUTPUT);
						digitalWrite(GPRS_PW, HIGH);
						delay(2000);
						digitalWrite(GPRS_PW, LOW);
						delay(500);
						answer=1;
						break;

		case    GPRS_HIBERNATE 	:	answer=sendATCommand(POWER_HIBERNATE,POWER_R);
						switch(answer)
						{
							case	0	:	flag |= GPRS_ERROR_POWER;
										break;
							case	2	:	flag |= GPRS_ERROR_POWER;
										break;
						}
						break;

		case	GPRS_SLEEP	:	answer=sendATCommand(POWER_SLEEP,POWER_R);
						switch(answer)
						{
							case	0	:	flag |= GPRS_ERROR_POWER;
										break;
							case	2	:	flag |= GPRS_ERROR_POWER;
										break;
						}
						break;

                case    GPRS_OFF  	:	pinMode(DIGITAL6,OUTPUT);
                                                digitalWrite(DIGITAL6,LOW);
						delay(500);
						answer=1;
						break;

	}
	return answer;
}


/* getMode(void) - Gets GPRS Power Mode
 *
 * This function gets the actual GPRS Power Mode. Possible values are ON, SLEEP/HIBERNATE and OFF
 *
 * Returns the power mode
*/
uint8_t	WaspGPRS::getMode()
{
	return	_pwrMode;
}


/* check(void) - Checks if GPRS is connected to the network
 *
 * This function checks if GPRS module is connected to the network. If not, it has no sense working with GPRS.
 *
 * It sends a command to GPRS module DEFAULT_TIMEOUT times. If GPRS module does not connect within these tries, function
 * exits.
 *
 * Returns '1' when connected and '0' if not
*/
int8_t WaspGPRS::check()
{
	uint8_t timeout=DEFAULT_TIMEOUT;
	char* command = (char*) calloc(10,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t answer=0;
	
	while(timeout)
	{
		sprintf(command,"%s%c%c","AT+CREG?",'\r','\n');
		printString(command,PORT_USED);
		answer=waitForData("+CREG: 0,1",1,0,0);
		switch(answer){
			case	0:	break;
			case	1:	connected = 1;
					free(command);
                                        return 1;
					break;
			case 	2:	break;
		}
		printString(command,PORT_USED);
		answer=waitForData("+CREG: 0,5",1,0,0);
		switch(answer){
			case	0:	break;
                        case	1:	connected = 1;
					free(command);
                                        return 1;
			                break;
			case 	2:	break;
		}
		delay(100);
		timeout--;
	}
        connected = 0;
	free(command);
	return 0;	
}


/* setPIN(const char* pin) - sets PIN to the SIM
 *
 * This function sets the specified PIN to the SIM
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t	WaspGPRS::setPIN(const char* pin)
{
	char* command = (char*) calloc(20,sizeof(char));
	if( command==NULL ) return -1;
	flag &= ~(GPRS_ERROR_PIN);
	char aux='"';
	uint8_t answer=0;

	sprintf(command,"%s%c%s%c",AT_PIN,aux,pin,aux);
	answer=sendATCommand(command,AT_PIN_R,SEND_ONCE);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_PIN;
					break;
		case	2	:	flag |= GPRS_ERROR_PIN;
					break;
	}
	if(flag & GPRS_ERROR_PIN) return 0;
	return 1;
}


/* setInfoIncomingCall() - set the info shown when an incoming call is received
 *
 * This function sets the info shown when an incoming call is received
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
 */
uint8_t	WaspGPRS::setInfoIncomingCall()
{
	flag &= ~(GPRS_ERROR_CALLINFO);
	uint8_t answer=0;

	answer=sendATCommand(AT_ID_INCALL,AT_ID_INCALL_R,SEND_ONCE);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CALLINFO;
					break;
		case	2	:	flag |= GPRS_ERROR_CALLINFO;
					break;
	}
	if(flag & GPRS_ERROR_CALLINFO) return 0;
	return 1;
}


/* setInfoIncomingSMS() - set the info shown when an incoming SMS is received
 *
 * This function sets the info shown when an incoming SMS is received
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
 */
uint8_t WaspGPRS::setInfoIncomingSMS()
{
	flag &= ~(GPRS_ERROR_SMSINFO);
	uint8_t answer=0;

	answer=sendATCommand(AT_SMS_INFO,AT_SMS_INFO_R,SEND_ONCE);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMSINFO;
					break;
		case	2	:	flag |= GPRS_ERROR_CALLINFO;
					break;
	}
	if(flag & GPRS_ERROR_SMSINFO) return 0;
	return 1;
}


/* setTextModeSMS() - set the text mode to the sms
 *
 * This function sets the text mode to the sms
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
 */
uint8_t WaspGPRS::setTextModeSMS()
{
	flag &= ~(GPRS_ERROR_SMS);
	uint8_t answer=0;

	answer=sendATCommand(AT_SMS_MODE,AT_SMS_MODE_R);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
	}
	if(flag & GPRS_ERROR_SMS) return 0;
	return 1;
}


/* makeCall(const char*) - makes a call to the seleted telephone number
 *
 * This function makes a call to the given telephone number.
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::makeCall(const char* tlfNumber)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	flag &= ~(GPRS_ERROR_CALL);
	uint8_t answer=0;

	sprintf(command,"%s%s;",AT_CALL,tlfNumber);
	answer=sendATCommand(command,AT_CALL_R,SEND_ONCE);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CALL;
					break;
		case	2	:	flag |= GPRS_ERROR_CALL;
					break;
	}
	if(flag & GPRS_ERROR_CALL) return 0;
	return 1;
	
}


/* makelostCall(const char* , uint8_t) - makes a lost call to the seleted telephone number
 *
 * This function makes a call to the given telephone number and its duration is specified by the other input.
 * After 'timeCall' seconds, hang the call up.
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::makeLostCall(const char* tlfNumber, uint8_t timeCall)
{
	char* command = (char*) calloc(25,sizeof(char));
	if( command==NULL ) return -1;
	flag &= ~(GPRS_ERROR_CALL);
	uint8_t answer=0;

	sprintf(command,"%s%s;",AT_CALL,tlfNumber);
	answer=sendATCommand(command,AT_CALL_R,SEND_ONCE);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CALL;
					break;
		case	1	:	for(int a=0; a<timeCall ; a++) delay(1000);
					hangUp();
					break;
		case	2	:	flag |= GPRS_ERROR_CALL;
					break;
	}
	if(flag & GPRS_ERROR_CALL) return 0;
	return 1;
}


/* hangUp(void) - hangs the call up
 *
 * This function hangs all the active calls up.
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
uint8_t WaspGPRS::hangUp()
{
	uint8_t answer=0;
	flag &= ~(GPRS_ERROR_HANG);
	
	answer=sendATCommand(AT_HANG,AT_HANG_R,SEND_ONCE);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_HANG;
					break;
		case	2	:	flag |= GPRS_ERROR_HANG;
					break;
	}
	if(flag & GPRS_ERROR_HANG) return 0;
	return 1;
}


/* sendSMS(const char*, const char*) - sends an SMS to the specified number
 *
 * This function sends an SMS to the specified number.
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::sendSMS(const char* smsText, const char* tlfNumber)
{
	flag &= ~(GPRS_ERROR_SMS);
	flag &= ~(GPRS_ERROR_SMS);
	char* command = (char*) calloc(25,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;

	sprintf(command,"%s%c%s%c",AT_SMS,aux,tlfNumber,aux);
	
	//setTextModeSMS();
	answer=sendATCommand(command,AT_SMS_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
	}
	
	answer=sendCommand((char*)smsText,0x1A,AT_SMS_TEXT_R,20,SEND_ONCE);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_SMS;
					return 0;
					break;
	}
	
	if(flag & GPRS_ERROR_SMS) return 0;
	return 1;
}


/* readCall() - set in 'tlfIN' variable the tlf number of the incoming call
 *
 * This function sets in 'tlfIN' variable the tlf number of the incoming call
 *
 * This function should be executed only inside 'manageIncomingData' function.
 *
 * 'data' must contains the tlfn number, something like that: aaaa"+34666999888"aaa
 *
 * Returns '1' on success and '0' if error
*/
uint8_t	WaspGPRS::readCall(const char* data)
{
	uint8_t a=0;
	uint8_t b=0;

        tlfIN[0]='\0';
	while( (data[a]!='"') && (a<45) )
	{
		a++;
	}
	a++;
	if(a>=45) return 0;
	while( (data[a]!='"') && (a<45)  && (b<15) )
	{
		tlfIN[b]=data[a];
		a++;
		b++;
	}
        if( b>=15 ) tlfIN[b-1]='\0';
        else tlfIN[b]='\0';
	return 1;
}


/* readSMS() - set in 'tlfIN' and 'sms' variables the tlf number and text of the incoming SMS
 *
 * This function sets in 'tlfIN' and 'sms' variables the tlf number and text of the incoming SMS
 *
 * This function should be executed only inside 'manageIncomingData' function.
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::readSMS(const char* data)
{
	uint8_t a=0,b=0,c=0;
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t* byteIN = (uint8_t*) calloc(200,sizeof(uint8_t));
	if( byteIN==NULL ) return -1;
	long previous=0;
	uint8_t counter=0;
	
        tlfIN[0]='\0';
        sms[0]='\0';
        sms_index[0]='\0';
	while( (data[a]!=',') && (a<45) ) a++;
	a++;
	if(a>=45){
		free(command);
		free(byteIN); 
		return 0;
	}
	while( data[a]!=13 )
	{
		sms_index[b]=data[a];
		a++;
		b++;
	}
	sms_index[b]='\0';
	sprintf(command,"AT%s%s\r\n",AT_SMS_READ,sms_index);
	printString(command,PORT_USED);
	while(!serialAvailable(PORT_USED));
	previous=millis();
	a=0;
	while( (millis()-previous) < 2000 )
	{
		while( serialAvailable(PORT_USED) && (millis()-previous) < 2000 )
		{
			byteIN[a]=serialRead(PORT_USED);
			a++;
	   		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem	
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	a=0;
	while( counter <3 )
	{
		while( (byteIN[a]!='"') && (a<200) )
		{
			a++;
		}
		a++;
		counter++;
	}
	if(a>=200){
		free(command);
		free(byteIN); 
		return 0;
	}
	counter=0;
	while( (byteIN[a]!='"') && (a<200) && (c<15) )
	{
		tlfIN[c]=byteIN[a];
		a++;
		c++;
	}
        if( c>=15 ) tlfIN[c-1]='\0';
        else tlfIN[c]='\0';
	while( counter < 5 )
	{
		while( (byteIN[a]!='"') && (a<200) )
		{
			a++;
		}
		a++;
		counter++;
	}
	if(a>=200){
		free(command);
		free(byteIN); 
		return 0;
	}
	a++;
	a++;
	b=0;
	while( (byteIN[a]!=13) && (a<200) && (b<100) )
	{
		sms[b]=byteIN[a];
		b++;
		a++;
	}
	if(a>=200){
		free(command);
		free(byteIN); 
		return 0;
	}
        if( b>=100 )sms[b-1]='\0';
        else sms[b]='\0';
	free(command);
	free(byteIN); 
	return 1;
}


/* manageIncomingData() - manage incoming data from serial port, executing proper functions to store received data
 *
 * This function manages incoming data from serial port, executing proper functions to store received data
 *
 * Returns '1' on success and '0' if error
*/
int8_t	WaspGPRS::manageIncomingGSMData()
{
	char* byteIN = (char*) calloc(100,sizeof(char));
	if( byteIN==NULL ) return -1;
	uint8_t a=0;
	long previous=0;
	uint8_t answer=0;
	
	while(a<100){
		byteIN[a]=0;
		a++;
	}
	a=0;
	
	serialFlush(PORT_USED);
	previous=millis();
	while(!serialAvailable(PORT_USED) && (millis()-previous)<20000)
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	previous=millis();
	while( (millis()-previous) < 2000 )
	{
		while( serialAvailable(PORT_USED) && (millis()-previous) < 2000 )
		{
			byteIN[a]=serialRead(PORT_USED);
			a++;
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	answer=parse_GSM(byteIN);
	switch( answer ){
		case	0:	readCall(byteIN);
				break;
		case	1:	readSMS(byteIN);
				break;
	}
	free(byteIN);
	if(answer==2) return 0;
	return 1;
}


/* configureGPRS() - configures GPRS connection with login, password and some other parameters
 *
 * This function creates a GPRS connection with the carrier server to get access to the internet
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::configureGPRS()
{
	char* command = (char*) calloc(100,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	
	flag &= ~(GPRS_ERROR_CONF);

	sprintf(command,"%s0,%c%s%c,%c%s%c,%c%s%c,%c%s%c,%c%s%c",AT_GPRS_CONN_CFG,aux,
		AT_GPRS,aux,aux,AT_GPRS_APN,aux,aux,AT_GPRS_LOGIN,aux,aux,AT_GPRS_PASSW,aux,aux,AT_GPRS_IP,aux);
	
	answer=sendATCommand(command,AT_GPRS_CONN_CFG_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CONF;
					return 0;
					break;
		case	2	:	flag |= GPRS_ERROR_CONF;
					return 0;
					break;
	}
	
	if(!setConnectionTimer()) return 0;
	
	if(!setConnectionProfile()) return 0;
	
	if(!checkGPRS()) return 0;
	
	if(!setPattern(GPRS_PATTERN)) return 0;
	
	if(flag & GPRS_ERROR_CONF) return 0;
	return 1;
}


/* createSocket(ip, port) - creates a TCP/IP connection to the specified IP and PORT
 *
 * This function creates a TCP/IP connection to the specified IP and PORT
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * It stores in 'socket_ID' the TCP session ID assigned to the last call to create a socket
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::createSocket(const char* ip,const char* port, uint8_t mode)
{
	char* command = (char*) calloc(40,sizeof(char));
	if( command==NULL ) return -1;
	char aux='"';
	uint8_t answer=0;
	long previous=0;
	uint8_t counter=0;
        uint8_t i=0;

	
	flag &= ~(GPRS_ERROR_SOCKET);
	
	switch(mode){
		case GPRS_CLIENT:
			sprintf(command,"%s0,%u,%c%s%c,%s%c%c",AT_GPRS_TCP_CFG,mode,aux,ip,aux,port,'\r','\n');
			break;
		case GPRS_SERVER:
			sprintf(command,"%s0,%u,%s%c%c",AT_GPRS_TCP_CFG,mode,port,'\r','\n');
			break;
	}

	
	while( counter<3 )
	{
		serialFlush(PORT_USED);
		printString(command,PORT_USED);
		while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
		{
		   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		delay(10);
		answer=waitForData("+KTCPCFG:",20,0,0);
		if(answer==1) break;
		counter++;
	}
	
	free(command);
	if(answer!=1) return 0;
	serialRead(PORT_USED);
	delay(30);
        i=0;
        while(i<4){
            socket_ID[i]=0;
            i++;
        }
        i=0;
        while( socket_ID[i]!='\r' && i<4 && serialAvailable(PORT_USED) ){
            socket_ID[i]=serialRead(PORT_USED);
            i++;
        }
	delay(50);
	answer=0;
	counter=0;
	while( counter<3 )
	{
		serialFlush(PORT_USED);
		printString(AT_GPRS_TCP_CNX,PORT_USED);
                i=0;
                while( socket_ID[i]!='\r' ){
		  printByte(socket_ID[i],PORT_USED);
                  i++;
                }
		printByte('\r',PORT_USED);
		printByte('\n',PORT_USED);
		while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
		{
		   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		delay(10);
		answer=waitForData("OK",20,0,10);
		if(answer==1) break;
		counter++;
		
	}
	if(answer!=1) return 0;
	return 1;
}

/* readURL(url) - access to the specified URL and stores the info read in 'data_URL' variable
 *
 * This function access to the specified URL and stores the info read in 'data_URL' variable
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * It stores in 'socket_ID' the TCP session ID assigned to the last call to create a socket
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::readURL(const char* url)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t answer=0;
	
	if(!configureGPRS()) return 0;
	
	if(!createSocket(url,"80",GPRS_CLIENT)) return 0;
	
	serialFlush(PORT_USED);
	sprintf(command,"%s%c%c","GET / HTTP/1.0",'\r','\n');
	if(!sendData(command,socket_ID)){
		free(command);
		closeSocket(socket_ID);
		return 0;
	}
	free(command);
	waitForData("+KTCP_DATA:",20,0,0);
	if(readData(socket_ID,GPRS_DATA_LENGTH)<0){
		closeSocket(socket_ID);
		return 0;
	}
	
	while(!closeSocket(socket_ID)) closeSocket(socket_ID);
	while(!deleteSocket(socket_ID)) deleteSocket(socket_ID);
	
	return 1;
}


/* sendData(data,socket) - sends 'data' to the specified 'socket'
 *
 * This function sends 'data' to the specified 'socket'
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * It gets from 'socket_ID' the TCP session ID assigned to the last call of creating a socket
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::sendData(const char* data, uint8_t* socket)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t answer=0;
	long previous=0;
	uint8_t counter=0;
        uint8_t i=0;

	
	while(data[counter]!='\0') counter++;
	counter+=2;
        while(socket[i]!='\r') i++;
        counter+=i-1;
	serialFlush(PORT_USED);
        switch(i)
        {
            case 1: sprintf(command,"%s%c,%u%c%c",AT_GPRS_TCP_SND,socket[0],counter,'\r','\n');
                    break;
            case 2: sprintf(command,"%s%c%c,%u%c%c",AT_GPRS_TCP_SND,socket[0],socket[1],counter,'\r','\n');
                    break;
	    case 3: sprintf(command,"%s%c%c%c,%u%c%c",AT_GPRS_TCP_SND,socket[0],socket[1],socket[2],counter,'\r','\n');
                    break;
        }
       	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("CONNECT",20,0,0);
	if(answer!=1) return 0;
	answer=0;
	counter=0;
	while( (counter<3) && (answer!=1) )
	{
		serialFlush(PORT_USED);
		delay(20);
		writeData(data);
		previous=millis();
		while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
		{
		   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		delay(10);
		answer=waitForData("OK",20,0,0);
		counter++;
	}
	if(answer!=1) return 0;
	return 1;
}


/* readData(socket,data_length) - reads data of 'data_length' size from socket ID 'socket'
 *
 * This function reads data of 'data_length' size from socket ID 'socket'
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * It stores in 'socket_ID' the TCP session ID assigned to the last call to create a socket
 *
 * Returns '1' on success, '-1' if error and '0' if more data is available
*/
int8_t WaspGPRS::readData(uint8_t* socket, const char* data_length)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t answer=0;
	long previous=0;
	uint8_t a=0;
	uint8_t aux[10];
        uint8_t i=0;

        while(socket[i]!='\r') i++;
        
        switch(i)
        {
            case 1: sprintf(command,"%s%c,%s%c%c",AT_GPRS_TCP_RCV,socket[0],data_length,'\r','\n');
            break;
            case 2: sprintf(command,"%s%c%c,%s%c%c",AT_GPRS_TCP_RCV,socket[0],socket[1],data_length,'\r','\n');
            break;
            case 3: sprintf(command,"%s%c%c%c,%s%c%c",AT_GPRS_TCP_RCV,socket[0],socket[1],socket[2],data_length,'\r','\n');
            break;
        }
	
	serialFlush(PORT_USED);
	delay(50);
	printString(command,PORT_USED);
	free(command);
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("CONNECT",20,0,0);
	if(answer!=1) return -1;
	serialRead(PORT_USED);
	serialRead(PORT_USED);
        while( a<GPRS_DATA_LENGTH_U && serialAvailable(PORT_USED) )
        {
		aux[0]=serialRead(PORT_USED);
		if(aux[0]=='E'){
			aux[1]=serialRead(PORT_USED);
			if(aux[1]=='N'){
				aux[2]=serialRead(PORT_USED);
				if(aux[2]=='D'){
					aux[3]=serialRead(PORT_USED);
					if(aux[3]=='M'){
						aux[4]=serialRead(PORT_USED);
						if(aux[4]=='E'){
							aux[5]=serialRead(PORT_USED);
							if(aux[5]=='S'){
								answer=1;
								break;
							}
							else{
								data_URL[a]=aux[0];
								data_URL[a+1]=aux[1];
								data_URL[a+2]=aux[2];
								data_URL[a+3]=aux[3];
								data_URL[a+4]=aux[4];
								data_URL[a+5]=aux[5];
								a+=6;
							}
						}
						else{
							data_URL[a]=aux[0];
							data_URL[a+1]=aux[1];
							data_URL[a+2]=aux[2];
							data_URL[a+3]=aux[3];
							data_URL[a+4]=aux[4];
							a+=5;
						}
					}
					else{
						data_URL[a]=aux[0];
						data_URL[a+1]=aux[1];
						data_URL[a+2]=aux[2];
						data_URL[a+3]=aux[3];
						a+=4;
					}
				}
				else{
					data_URL[a]=aux[0];
					data_URL[a+1]=aux[1];
					data_URL[a+2]=aux[2];
					a+=3;
				}
			}
			else{
				data_URL[a]=aux[0];
				data_URL[a+1]=aux[1];
				a+=2;
			}	
		}
		else{
			data_URL[a]=aux[0];
			a++;
		}
	}
	
		
	data_read+=a;
	if(answer && a<(GPRS_DATA_LENGTH_U-10)) return 1;
	if(answer && a>=(GPRS_DATA_LENGTH_U-10)) return 0;
	return -1;
}


/* closeSocket(socket) - closes the socket specified by 'socket'
 *
 * This function closes the socket specified by 'socket'
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::closeSocket(uint8_t* socket)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
        uint8_t i=0;
        
/*        setInfoIncomingCall();*/
        flag &= ~(GPRS_ERROR_CLOSE);
        while(socket[i]!='\r') i++;
        switch(i)
        {
            case 1: sprintf(command,"%s%c,1",AT_GPRS_TCP_CLOSE,socket[0]);
                    break;
            case 2: sprintf(command,"%s%c%c,1",AT_GPRS_TCP_CLOSE,socket[0],socket[1]);
                    break;
            case 3: sprintf(command,"%s%c%c%c,1",AT_GPRS_TCP_CLOSE,socket[0],socket[1],socket[2]);
                    break;
        }
	

	answer=sendATCommand(command,AT_GPRS_TCP_CLOSE_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CLOSE;
					break;
		case	2	:	flag |= GPRS_ERROR_CLOSE;
					break;
	}
	if(flag & GPRS_ERROR_CLOSE) return 0;
	return 1;
}


/* deleteSocket(socket) - deletes the socket specified by 'socket'
 *
 * This function deletes the socket specified by 'socket'
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::deleteSocket(uint8_t* socket)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
        uint8_t i=0;
        
	setInfoIncomingCall();
	flag &= ~(GPRS_ERROR_DELETE);
        while(socket[i]!='\r') i++;
        switch(i)
        {
            case 1: sprintf(command,"%s%c",AT_GPRS_TCP_DEL,socket[0]);
                    break;
            case 2: sprintf(command,"%s%c%c",AT_GPRS_TCP_DEL,socket[0],socket[1]);
                    break;
            case 3: sprintf(command,"%s%c%c%c",AT_GPRS_TCP_DEL,socket[0],socket[1],socket[2]);
                    break;
        }
		
	answer=sendATCommand(command,AT_GPRS_TCP_DEL_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_DELETE;
					break;
		case	2	:	flag |= GPRS_ERROR_DELETE;
					break;
	}
	if(flag & GPRS_ERROR_DELETE) return 0;
	return 1;
}


/* getCellInfo() - gets the information from the cell where the module is connected
 *
 * This function gets the information from the cell where the module is connected
 *
 * It stores in 'RSSI' and 'cellID' variables the information from the cell
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::getCellInfo()
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t* byteIN = (uint8_t*) calloc(200,sizeof(uint8_t));
	if( byteIN==NULL ) return -1;
	long previous=millis();
	uint8_t counter=0;
	uint8_t a,b,c=0;
	
	serialFlush(PORT_USED);
	sprintf(command,"AT%s\r\n",AT_GPRS_CELLID);
	printString(command,PORT_USED);
	free(command);
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	previous=millis();
	a=0;
	while( (millis()-previous) < 2000 )
	{
		while( serialAvailable(PORT_USED) && (millis()-previous) < 2000 && (a<200) )
		{
			byteIN[a]=serialRead(PORT_USED);
			a++;
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	a=0;
	while( counter < 5 )
	{
		while( (byteIN[a]!=',') && (a<200) )
		{
			a++;
		}
		a++;
		counter++;
	}
	if(a>=200){
		free(byteIN); 
		return 0;
	}
	counter=0;
	while( (byteIN[a]!=',') && (a<200) )
	{
		cellID[c]=byteIN[a];
		a++;
		c++;
	}
	a++;
	while( (byteIN[a]!=',') && (a<200) )
	{
		RSSI[b]=byteIN[a];
		delay(10);
		b++;
		a++;
	}
	free(byteIN);
	return 1;
}


/* sendCommand(ATcommand, ATcommand_R) - sends any command to GPRS module
 *
 * This function sends any command to GPRS module
 *
 * It stores in 'answer_command' variable the answer returned by the GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::sendCommand(const char* ATcommand)
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t timeout=0;
	uint8_t i=0;
	
	sprintf(command, "AT%s%c%c", ATcommand,'\r','\n');

	serialFlush(PORT_USED);
	while(!serialAvailable(PORT_USED)) {
		printString(command,PORT_USED);
		delay(DELAY_ON_SEND);
	}
	free(command);
	while( timeout < 5 )
	{
		while(!serialAvailable(PORT_USED) && timeout < 5) {
			timeout++;
			delay(1000);
		}
		while(serialAvailable(PORT_USED) && timeout < 5){
			answer_command[i] = serialRead(PORT_USED);
			delay(20);
			i++;
			timeout=0;
			if(i>=199) timeout=5;
		}
	}
	answer_command[i]='\0';
	if( i<5 ) return 0;
	return 1;
}

/* sendMail() - sends an email
 *
 * This function sends an email
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::sendMail(const char* from, const char* to, const char* subject, const char* body, const char* user, const char* passw, const char* smtp_server, uint16_t port)
{
	uint8_t counter=0;
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
	
	if(!setFlowControl()) return 0;
		
	if(!configureGPRS()) return 0;
		
	if(!setEmailParams(smtp_server, port, from)) return 0;
		
	if(!setEmailPwd(user, passw)) return 0;
		
	if(!setEmailDestination(to)) return 0;
		
	if(!setEmailSubject(subject)) return 0;
		
	while( body[counter]!='\0' ) counter++;
	counter+=2;
	
	serialFlush(PORT_USED);
	sprintf(command,"AT%s1,%u%c%c",AT_SMTP_SEND,counter,'\r','\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("CONNECT",20,0,0);
	if(answer!=1) return 0;
	
	printString(body,PORT_USED);
	printByte('\r',PORT_USED);
	printByte('\n',PORT_USED);
	
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("OK",20,0,0);
	if(answer!=1) return 0;
	return 1;
}

/* readMail() - reads an email
 *
 * This function reads an email
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::readMail(const char* user, const char* passw, const char* pop3_server, uint16_t port)
{
	char* command = (char*) calloc(70,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
	char aux='"';
	uint16_t index=0;
	uint16_t i=0;
	uint8_t end=0;
	char* aux2;
	uint8_t j=0;
	
	if(!setFlowControl()) return 0;
		
	if(!configureGPRS()) return 0;
	
	// Connect to POP3 Server
	sprintf(command,"AT%s%c%s%c,%u,%c%s%c,%c%s%c%c%c",AT_POP3_PARAM,aux,pop3_server,aux,port,aux,user,aux,aux,passw,aux,'\r','\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("OK",20,0,0);
	if(answer!=1) return 0;
	
	
	// Get the index
	printString(AT_POP3_LIST,PORT_USED);
	printByte('\r',PORT_USED);
	printByte('\n',PORT_USED);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("+KPOPLIST:",20,0,0);
	if(answer!=1) return 0;
	i=0;
	while( serialRead(PORT_USED)!=':' );
	serialRead(PORT_USED);
	index=serialRead(PORT_USED);
	serialFlush(PORT_USED);
		
	// Read the email
	printString(AT_POP3_READ,PORT_USED);
	printByte(index,PORT_USED);
	printByte('\r',PORT_USED);
	printByte('\n',PORT_USED);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
	   if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	
	aux2 = (char*) calloc(MAX_SIZE_POP3,sizeof(char));
	if( aux2==NULL ) return -1;
	
	previous=millis();
	i=0;
	while( millis()-previous<5000 )
	{
		while( serialAvailable(PORT_USED) && i<MAX_SIZE_POP3 )
		{
			aux2[i]=serialRead(PORT_USED);
			i++;
			previous=millis();
		}
	}
	if( i>=MAX_SIZE_POP3 ) aux2[i-1]='\0';
	else aux2[i]='\0';
	
	
	i=0;
	j=0;
	i=waitForData(aux2,"Return-path: <");
	
	emailAddress[j]=aux2[i];
	while( emailAddress[j]!='>' && i!=0 )
	{
		i++;
		j++;
		emailAddress[j]=aux2[i];
		if(j>=30) break;
	}
	emailAddress[j]='\0';
	
	
	i=0;
	j=0;
	i=waitForData(aux2,"ubject: ");
	
	subject[j]=aux2[i];
	while( subject[j]!='\r' && i!=0 )
	{
		i++;
		j++;
		subject[j]=aux2[i];
		if(j>=30) break;
	}
	subject[j]='\0';
	i=0;
	j=0;
	
	i=waitForData(aux2,"*/*");
	while( j<=GPRS_MAX_DATA && !end && i!=0 )
	{
		body[j]=aux2[i];
		if( body[j]=='*' )
		{
			j++;
			i++;
			body[j]=aux2[i];
			if( body[j]=='/' )
			{
				j++;
				i++;
				body[j]=aux2[i];
				if( body[j]=='*' )
				{
					j++;
					i++;
					body[j]='\0';
					end=1;
				}
			}
		}
		j++;
		i++;
	}
	body[j-1]='\0';
	i=0;
	
	free(aux2);
	aux2=NULL;
	return 1;
}


/* uploadFile() - uploads a file to a FTP server
 *
 * This function uploads a file to a FTP server
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::uploadFile(const char* file, const char* path, const char* user, const char* passw, const char* ftp_server, uint8_t ftp_port)
{
	char* command = (char*) calloc(100,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
	char aux='"';
	uint8_t id=0;
	
	if(!setFlowControl()) return 0;
			
	if(!configureGPRS()) return 0;
		
	// Connect to FTP Server
	sprintf(command,"AT%s,%c%s%c,%c%s%c,%c%s%c,%u,1%c%c",AT_FTP_PARAM,aux,ftp_server,aux,aux,user,aux,aux,passw,aux,ftp_port,'\r','\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("+KFTPCFG: ",20,0,0);
	if(answer!=1) return 0;
		
	id=serialRead(PORT_USED);
	
	if( !sendDataFTP(file, path,id) ) return 0;
	
	return 1;
}


/* uploadFile() - uploads a file to a FTP server
 *
 * This function uploads a file to a FTP server
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::downloadFile(const char* file, const char* path, const char* user, const char* passw, const char* ftp_server, uint8_t ftp_port)
{
	char* command = (char*) calloc(70,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
	char aux='"';
	uint8_t id=0;
	
	if(!setFlowControl()) return 0;
			
	if(!configureGPRS()) return 0;
		
	// Connect to FTP Server
	sprintf(command,"AT%s,%c%s%c,%c%s%c,%c%s%c,%u,1%c%c",AT_FTP_PARAM,aux,ftp_server,aux,aux,user,aux,aux,passw,aux,ftp_port,'\r','\n');
	printString(command,PORT_USED);
	free(command);
	previous=millis();
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<10000) )
	{
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	delay(10);
	answer=waitForData("+KFTPCFG: ",20,0,0);
	if(answer!=1) return 0;
		
	id=serialRead(PORT_USED);
	
	if( !readDataFTP(file, path, id) ) return 0;
	
	return 1;
}


/* closeFTP() - closes the FTP
 *
 * This function closes the FTP
 *
 * It modifies 'flag' if expected answer is not received after sending a command to GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::closeFTP()
{
	char* command = (char*) calloc(30,sizeof(char));
	if( command==NULL ) return -1;
	long previous=0;
	uint8_t answer=0;
        
        setInfoIncomingCall();
        flag &= ~(GPRS_ERROR_CLOSE);
	sprintf(command,"%s0",AT_FTP_CLOSE);	

	answer=sendATCommand(command,AT_FTP_CLOSE_R);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_CLOSE;
					break;
		case	2	:	flag |= GPRS_ERROR_CLOSE;
					break;
	}
	if(flag & GPRS_ERROR_CLOSE) return 0;
	return 1;
}


/* reset() - resets the GPRS module
 *
 * This function resets the GPRS module
 *
 * Returns '1' on success and '0' if error
*/
int8_t	WaspGPRS::reset()
{
	char* command = (char*) calloc(20,sizeof(char));
	if( command==NULL ) return -1;
	flag &= ~(GPRS_ERROR_PIN);
	uint8_t answer=0;

	sprintf(command,"%s\r\n",RESET_GPRS);
	answer=sendATCommand(command,RESET_GPRS_R,SEND_ONCE);
	free(command);
	switch(answer)
	{
		case	0	:	flag |= GPRS_ERROR_PIN;
					break;
		case	2	:	flag |= GPRS_ERROR_PIN;
					break;
	}
	if(flag & GPRS_ERROR_PIN) return 0;
	return 1;
}


/* getIMEI() - gets the IMEI from the SIM card
 *
 * This function gets the IMEI from the SIM card. It stores the IMEI into 'IMEI' variable.
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::getIMEI()
{
	char* command = (char*) calloc(15,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t* byteIN = (uint8_t*) calloc(20,sizeof(uint8_t));
	if( byteIN==NULL ) return -1;
	long previous=millis();
	uint8_t a,b=0;
	
	serialFlush(PORT_USED);
	sprintf(command,"%s\r\n",AT_GPRS_IMEI);
	printString(command,PORT_USED);
	free(command);
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) )
	{
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	previous=millis();
	a=0;
	while( (millis()-previous) < 2000 )
	{
		while( serialAvailable(PORT_USED) && (millis()-previous) < 2000 && (a<20) )
		{
			byteIN[a]=serialRead(PORT_USED);
			a++;
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	a=0;

		while( (byteIN[a]!='\r') && (byteIN[a]!='\n') && (a<20) )
		{
			a++;
		}
		if(a>=20){
			free(byteIN);
			return 0;
		}
		a++;

	if(a>=20){
			free(byteIN);
			return 0;
		}

	b=0;
	while( (byteIN[a]!='\r') && (a<20) )
	{
		IMEI[b]=byteIN[a];
		a++;
		b++;
	}
	IMEI[b]='\0';
	if(b<=10){
		free(byteIN);
		return 0;
	}
	free(byteIN);
	return 1;
}


/* getIMSI() - gets the IMSI from the SIM card
 *
 * This function gets the IMSI from the SIM card. It stores the IMSI into 'IMSI' variable.
 *
 * Returns '1' on success and '0' if error
*/
int8_t WaspGPRS::getIMSI()
{
	char* command = (char*) calloc(15,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t* byteIN = (uint8_t*) calloc(20,sizeof(uint8_t));
	if( byteIN==NULL ) return -1;
	long previous=millis();
	uint8_t a,b=0;
	
	serialFlush(PORT_USED);
	sprintf(command,"%s\r\n",AT_GPRS_IMSI);
	printString(command,PORT_USED);
	free(command);
	while( (!serialAvailable(PORT_USED)) && ((millis()-previous)<3000) );
	previous=millis();
	a=0;
	while( (millis()-previous) < 2000 )
	{
		while( serialAvailable(PORT_USED) && (millis()-previous) < 2000 && (a<20) )
		{
			byteIN[a]=serialRead(PORT_USED);
			a++;
			if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
	}
	a=0;

		while( (byteIN[a]!='\r') && (byteIN[a]!='\n') && (a<20) )
		{
			a++;
		}
		if(a>=20){
			free(byteIN);
			return 0;
		}
		a++;

	if(a>=20){
			free(byteIN);
			return 0;
		}

	b=0;
	while( (byteIN[a]!='\r') && (a<20) )
	{
		IMSI[b]=byteIN[a];
		a++;
		b++;
	}
	IMSI[b]='\0';
	if(b<=10){
		free(byteIN);
		return 0;
	}
	free(byteIN);
	return 1;
}



// Preinstantiate Objects //////////////////////////////////////////////////////

WaspGPRS GPRS = WaspGPRS();

