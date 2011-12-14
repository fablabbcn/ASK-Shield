/*
 *  Copyright (C) 2010 Libelium Comunicaciones Distribuidas S.L.
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
 *  Implementation:	Alberto Bielsa, Manuel Calahorra, Yuri Carmona
 */


#ifndef __WPROGRAM_H__
#include "WaspClasses.h"
#endif

// Private Methods /////////////////////////////////////////////////////////////

// returns '0' if the string is not found
uint16_t WaspBT::waitForData(uint8_t* data, const char* expectedAnswer)
{
	uint16_t i=0;
	for (i = 0; i < 100; i++) received[i] = ' ';

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
				received[it]=data[i];
				i++;
			}
			first=0;
		}
		it=0;
		theSame=true;
		for(it=0; it<theLength ; it++)
		{
			if(received[it]!=expectedAnswer[it]){
				theSame= false;
				break;
			}
		}
		if( theSame ) match=1;
		else
		{
			for(it=0; it<theLength-1 ; it++)
			{
				received[it]=received[it+1];
			}
			received[it]=data[i];
			i++;
		}
	}
	
	if( !match ) i=0;
	return i;
}


uint8_t WaspBT::parse_data()
{
	return parse_data("");
}


// Returns '0' on match with answer, '1' on mismatch with answer, '2' no char received with answer, '3' no error without answer, '4' no char received without answer
uint8_t WaspBT::parse_data(char* answer)
{
	bool withAnswer = false;
	bool match = false;
	uint8_t pin_en = 0;
	uint8_t lengthAnswer = 0;
	long previous=millis();
	long previous2=millis();
	int16_t interval=50;
	int16_t intervalMAX=40000;
	uint16_t MAX_BT_DATA=450;
	uint8_t* memory = (uint8_t*) calloc(MAX_BT_DATA,sizeof(uint8_t));
	if( memory==NULL ) return -1;
	
	// Checking if we are waiting for a specific answer
	i=0;
	while(answer[i]!='\0') i++;
	lengthAnswer=i;
	if( lengthAnswer ) withAnswer = true;
	
	if( !(strcmp(answer,"+RDDSCNF=0")) ) interval=30000;
	if( !(strcmp(answer,"+RSDSCNF=0")) ){
		 interval=10000;
		 pin_en=1;
	}
	if( !(strcmp(answer,"ROK")) ) interval=5000;
	if( !(strcmp(answer,"+RCCRCNF")) ){
		interval=10000; 
		pin_en=2;
	}

	// Read data from BT meanwhile data is available
	i=0;
	previous2=millis();
	previous=millis();
	
	while( ((millis()-previous)<interval) && ((millis()-previous2)<intervalMAX) && i<MAX_BT_DATA && !match)
	{
		if(serialAvailable(_uart))
		{
			memory[i]=serialRead(_uart);
			i++;
			previous=millis();
		}
		if( withAnswer ){
			if( waitForData(memory,answer) ) match=true;
		}		
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		if( millis()-previous2 < 0 ) previous2=millis(); //avoid millis overflow problem
	}
	//USB.println();
	
			
	// If we are here, the answer has not been found or there wasn't any answer to find
	if( waitForData(memory,"+RPCI") ){
		if(!pin_request(memory,pin_en)) match = true;
	}
	if( waitForData(memory,"+RCOI") && (auto_accept==BT_MANUAL_ACC) ) accept_connection(memory);
	if( waitForData(memory,"+RDAI") ){
		 if(!data_request(memory)) match = true;
	}
	if( waitForData(memory,"+RDII") ) ending_connection(memory);
	if( waitForData(memory,"+RDDSCNF=0") ) parse_brothers(memory,i);
	if( waitForData(memory,"+RSDSCNF=0") ) parse_device(memory);
	if( waitForData(memory,"+RCCRCNF") ) get_MTU(memory);
		
	if( match ){
		free(memory);
		memory=NULL;
		return 0;
	}
	
	if( withAnswer )
	{
		if( i>0 ){
			free(memory);
			memory=NULL;
			return 1;
		}
		else{
			free(memory);
			memory=NULL;
			return 2;
		}
	}
	else
	{
		if( i>0 ){
			free(memory);
			memory=NULL;
			return 3;
		}
		else{
			free(memory);
			memory=NULL;
			return 4;
		}
	}
	
}


uint8_t WaspBT::pin_request(uint8_t* data, uint8_t pin_en)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t length=0;
		
	i=0;
	while(bt_pin[i]!='\0') i++;
	length=i;	
	
	if(length<10) sprintf(command,"%s0%d,%s",BT_AT_PIN_REQUEST,length,bt_pin);
	else if(length>=10) sprintf(command,"%s%d,%s",BT_AT_PIN_REQUEST,length,bt_pin);
	
	printData(command);
	
	free(command);
	command=NULL;
	
	if(pin_en==1) return parse_data("+RSDSCNF=0");
	if(pin_en==2) return parse_data("+RCCRCNF");
	else if(!pin_en) return parse_data("+RSLE");
}


uint8_t WaspBT::accept_connection(uint8_t* data)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	
	sprintf(command,"%s1",BT_AT_ACCEPT_CONN);
	
	printData(command);
	
	free(command);
	command=NULL;
	
	return parse_data("OK");
}


uint8_t WaspBT::data_request(uint8_t* data)
{
	uint16_t i=0;
	uint16_t j=0;
	
	j=waitForData(data,"+RDAI");
	
	while(data[j]!=',') j++;
	j++;
	
	while(data[j]!='\r' && i<BT_MAX_DATA){
		data_received[i]=data[j];
		i++;
		j++;
	}

	if(i<BT_MAX_DATA) data_received[i]='\0';
	else data_received[BT_MAX_DATA-1]='\0';
	
	return 0;
}


void WaspBT::ending_connection(uint8_t* data)
{
	active_connection=0;
}


void WaspBT::parse_brothers(uint8_t* data, uint16_t number_of_data)
{
	uint16_t i=0;
	uint16_t j=0;
	uint8_t k=0;
	uint8_t end_line=0;
	
	i=0;
	while(i<number_of_data){
		if(data[i]=='\n') end_line++;
		i++;
	}

	devices_found=end_line-1;
	if(devices_found>BT_MAX_DEVICES) devices_found=BT_MAX_DEVICES;
	
	i=0;
	while( i<(end_line-1) && i<BT_MAX_DEVICES )
	{
		while(data[j]!='=' && j<number_of_data) j++;
		j++;
		for(k=0;k<12;k++)
		{
			discovered_devices[i].mac_address[k]=data[j];
			j++;
		}
		j++;
		k=0;
		while(data[j]!=',' && k<18)
		{
			discovered_devices[i].name[k]=data[j];
			j++;
			k++;
		}
		discovered_devices[i].name[k]='\0';
		while(data[j]!=',' && j<number_of_data) j++;
		j++;
		k=0;
		while(data[j]!='\r' && k<6)
		{
			discovered_devices[i].CoD[k]=data[j];
			j++;
			k++;
		}
		discovered_devices[i].CoD[k]='\0';
		i++;
		k=0;
	}
}


void WaspBT::parse_device(uint8_t* data)
{
	uint16_t j=0;
	uint8_t k=0;
		
	if( waitForData(data,"+RSDSRES=") ){
		while(data[j]!='=') j++;
		j++;
		while(data[j]!=',' && k<16)
		{
			device_service_name[k]=data[j];
			j++;
			k++;
		}
		j++;
		device_service_name[k]='\0';
		device_service_channel[0]=data[j];
		device_service_channel[1]=data[j+1];
	}
}


void WaspBT::get_MTU(uint8_t* data)
{
	uint8_t i=0;
	uint16_t j=0;
	
	while(data[j]!='=') j++;
	j++;
	
	connection_mtu[0]=serialRead(_uart);
	connection_mtu[1]=serialRead(_uart);
	connection_mtu[2]=serialRead(_uart);		
}


void WaspBT::printData(const char* data)
{	
	printString(data,_uart);
	printByte('\r',_uart);
	printByte('\n',_uart);
}


// Constructors ////////////////////////////////////////////////////////////////

WaspBT::WaspBT()
{
	i=0;
	while(BT_PIN[i]!='\0'){
		bt_pin[i]=BT_PIN[i];
		i++;
	}
	
	auto_accept=BT_AUTO_ACC;
	
	active_connection=0;
	
	devices_found=0;
}

// Public Methods //////////////////////////////////////////////////////////////

void WaspBT::ON()
{
	ON(UART0);
}

void WaspBT::ON(uint8_t uart)
{
	uint8_t answer=0;
	
	// Store the selected uart
	_uart = uart;
	
	// Open the selected UART
	beginSerial(XBEE_RATE, _uart);
	
	// Switch power supply on
	if( !_uart )
	{	// UART0
		pinMode(XBEE_PW,OUTPUT);
		digitalWrite(XBEE_PW,HIGH);
	}
	else
	{	// UART1
		Utils.setMuxGPRS();
		pinMode(DIGITAL6,OUTPUT);
		digitalWrite(DIGITAL6,HIGH);		
		pinMode(GPRS_PW,OUTPUT);
		digitalWrite(GPRS_PW,HIGH); 
	}
	
	// Waiting for the answer 'ROK'
	answer=parse_data("ROK");
	
	if(answer) flag |= BT_ERROR_ON;
}


void WaspBT::OFF()
{
	closeSerial(_uart);
	
	if( !_uart )
	{
		pinMode(XBEE_PW,OUTPUT);
		digitalWrite(XBEE_PW,LOW);
	}
	else
	{
		Utils.setMux(MUX_TO_LOW,MUX_TO_LOW);
		pinMode(DIGITAL6,OUTPUT);
		digitalWrite(DIGITAL6,LOW);		
		pinMode(GPRS_PW,OUTPUT);
		digitalWrite(GPRS_PW,LOW); 
	}
	
}


// Returns '0' when the functions are right, '>0' otherwise
uint8_t WaspBT::init()
{
	uint8_t error = 0;
	
	first=1;
	
	if(setSecurity(BT_SECURITY_PREDEFINED,bt_pin)) error++;
		
	if(setServiceParameters(BT_SERVICE_NAME,BT_SERVICE_CHANNEL,BT_SERVICE_COD)) error++;
	
	if(setPublicName(BT_PUBLIC_NAME)) error++;
	
	if(setAutoAccept(BT_AUTO_ACC)) error++;
	
	if(setDiscoveryOptions(BT_DISCOVERABLE)) error++;
		
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::setSecurity(uint8_t mode, const char* pin)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t length=0;
	uint8_t error=0;
	
	i=0;
	while(pin[i]!='\0'){
		bt_pin[i]=pin[i];
		i++;
	}
	length=i;	
	
	if(!first) reset();
	
	if(length<10) sprintf(command,"%s%d,1,1,0%d,%s",BT_AT_SECURITY,mode,length,pin);
	else if(length>=10) sprintf(command,"%s%d,1,1,%d,%s",BT_AT_SECURITY,mode,length,pin);
	
	printData(command);
	
	first=0;
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_SECURITY;
	free(command);
	command=NULL;
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::setPublicName(const char* name)
{	
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t length=0;
	uint8_t error=0;
	
	i=0;
	while(name[i]!='\0') i++;
	length=i;
	
	if(length<10) sprintf(command,"%s0%d,%s",BT_AT_PUBLIC_NAME,length,name);
	else if(length>=10) sprintf(command,"%s%d,%s",BT_AT_PUBLIC_NAME,length,name);
	
	printData(command);
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_NAME;
	free(command);
	command=NULL;
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::setAutoAccept(uint8_t mode)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;
	
	auto_accept=mode;	

	sprintf(command,"%s%d",BT_AT_AUTO_ACCEPT,mode);
	
	printData(command);
		
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_ACCEPT;
	free(command);
	command=NULL;
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::setDiscoveryOptions(uint8_t option)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;

	sprintf(command,"%s%d",BT_AT_DISCOV_OPT,option);
	
	printData(command);
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_DISC_OPT;
	free(command);
	command=NULL;
	return error;
}


uint8_t WaspBT::readData()
{
	return parse_data();
}


uint8_t WaspBT::setServiceParameters(const char* name, uint8_t channel, const char* cod)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t length=0;
	uint8_t error=0;
	
	i=0;
	while(name[i]!='\0') i++;
	length=i;	
	
	if(length<10 && channel<10) sprintf(command,"%s1101,0%d,%s,0%d,%s",BT_AT_SERVICE_PAR,length,name,channel,cod);
	else if(length>=10 && channel<10) sprintf(command,"%s1101,%d,%s,0%d,%s",BT_AT_SERVICE_PAR,length,name,channel,cod);
	else if(length<10 && channel>=10) sprintf(command,"%s1101,0%d,%s,%d,%s",BT_AT_SERVICE_PAR,length,name,channel,cod);
	else if(length>=10 && channel>=10) sprintf(command,"%s1101,%d,%s,%d,%s",BT_AT_SERVICE_PAR,length,name,channel,cod);
		
	printData(command);
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_SERVICE;
	free(command);
	command=NULL;
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::reset()
{	
	uint8_t error=0;
	
	printData(BT_AT_RESET);
	
	error=parse_data("ROK");
	if(error) flag |= BT_ERROR_RESET;
	return error;
}


// Returns '0' if no error, '>0' otherwise
uint8_t WaspBT::scanNetwork()
{	
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;
	
	//Initialize structure
	devices_found=0;
	for(int i=0; i<BT_MAX_DEVICES; i++)
	{
	   discovered_devices[i].name[0]='\0';
	   for(int m=0;m<12;m++)
    	   {
      		BT.discovered_devices[i].mac_address[m]='\0';
    	   }
	   discovered_devices[i].channel[0]='\0';
	   discovered_devices[i].CoD[0]='\0';
	   discovered_devices[i].service_name[0]='\0';
	}
	
	sprintf(command,"%s%d",BT_AT_DISCOVER,0);
	
	printData(command);
	free(command);
	command=NULL;
	if(parse_data("OK")) return 1;

	error=parse_data("+RDDSCNF=0");
	if(error) flag |= BT_ERROR_SCAN;
	
	return error;
}


uint8_t WaspBT::discoverDevice(const char* mac, const char* profile)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;
	
	sprintf(command,"%s%s,%s",BT_AT_DISCOVER_DEV,mac,profile);
	
	printData(command);
	free(command);
	command=NULL;
	if(parse_data("OK")) return 1;

	error=parse_data("+RSDSCNF=0");
	if(error) flag |= BT_ERROR_DISC;
	
	return error;
}


uint8_t WaspBT::createConnection(const char* mac,const char* channel)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;
	
	sprintf(command,"%s%s,%s",BT_AT_CONNECT,mac,channel);
	
	printData(command);
	
	if(parse_data("OK")){
		flag |= BT_ERROR_CONNECTING; 
		free(command);
		command=NULL;
		return 1;
	}

	error=parse_data("+RCCRCNF");
	if(error) flag |= BT_ERROR_CONNECTING;
	else active_connection=1;
	
	free(command);
	command=NULL;
	return error;
}


uint8_t WaspBT::getOwnMac()
{	
	printData(BT_AT_OWN_MAC);
	
	if(!parse_data("+RRBDRES")){
		i=0;
		while(i<12){
			own_mac[i]=serialRead(_uart);
			i++;
		}		
		return 0;
	}
	flag |= BT_ERROR_MAC;
	return 1;
}


uint8_t WaspBT::removeTrustedDevice(const char* mac)
{
	char* command = (char*) calloc(50,sizeof(char));
	if( command==NULL ) return -1;
	uint8_t error=0;
	
	sprintf(command,"%s%s",BT_AT_DELETE_TD,mac);
	
	printData(command);
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_REMOVING_TD;
	free(command);
	command=NULL;
	return error;
}


uint8_t WaspBT::sendData(const char* data)
{
	uint16_t length=0;
	uint8_t error=0;
	
	i=0;
	while(data[i]!='\0') i++;
	length=i;	
	
	char* command = (char*) calloc(20+length,sizeof(char));
	if( command==NULL ) return -1;

	if(length<10) sprintf(command,"%s00%d,%s",BT_AT_SEND_DATA,length,data);
	else if(length<100 && length>10) sprintf(command,"%s0%d,%s",BT_AT_SEND_DATA,length,data);
	else sprintf(command,"%s%d,%s",BT_AT_SEND_DATA,length,data);
		
	printData(command);
	
	free(command);
	command=NULL;
	error=parse_data("OK");
	delay(300);
	if(error) flag |= BT_ERROR_SENDING_DATA;
	return error;
}


uint8_t WaspBT::removeConnection()
{	
	printData(BT_AT_DISCONNECT);
	
	if(!parse_data("OK"))
	{
		getOwnMac();
		return 0;
	}
	flag |= BT_ERROR_REMOVING_CONNECTION;
	return 1;
}

uint8_t WaspBT::createStreamConnection()
{
	uint8_t error=0;

	printData(BT_AT_STREAM_CONN);
	
	error=parse_data("OK");
	if(error) flag |= BT_ERROR_STREAM;
	return error;
}	


WaspBT BT=WaspBT();
