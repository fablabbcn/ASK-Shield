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
 *  Version:		0.3
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, Yuri Carmona
 */


#ifndef __WPROGRAM_H__
	#include "WaspClasses.h"
#endif

void	WaspXBee868::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used)
{
	init(protocol_used, frequency, model_used, 0);
}

void	WaspXBee868::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used)
{
	protocol=protocol_used;
	freq=frequency;
	model=model_used;
	uart=uart_used;
	
	totalFragmentsReceived=0;
	pendingPackets=0;
	pos=0;
	discoveryOptions=0x00;
	
	scanTime[0]=0x00;
	scanTime[1]=0x82;
	encryptMode=0;
	powerLevel=4;
	timeRSSI=0x20;

	ACKerrors[0]=0x00;
	ACKerrors[1]=0x00;
	errorsRF[0]=0x00;
	errorsRF[1]=0x00;
	goodPackets[0]=0x00;
	goodPackets[1]=0x00;
	transmisionErrors[0]=0x00;
	transmisionErrors[1]=0x00;
	maxPayloadBytes[0]=0x01;
	maxPayloadBytes[1]=0x00;
	multipleBroadcast=0x03;
	macRetries=0x0A;
	resetReason=0x00;
	
	data_length=0;
	it=0;
	start=0;
	finish=0;
	add_type=0;
	mode=0;
	frag_length=0;
	TIME1=0;	
	nextIndex1=0;
	frameNext=0;
	replacementPolicy=XBEE_OUT;
	indexNotModified=1;
	error_AT=2;
	error_RX=2;
	error_TX=2;
	clearFinishArray();
	clearCommand();
}

/*
 Function: Read the number of times the RF receiver detected a CRC or length error
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "errorsRF" variable number of times CRC or length error
*/
uint8_t WaspXBee868::getRFerrors()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_RF_errors_868);
    error=gen_send(get_RF_errors_868);
    
    if(!error)
    {
        errorsRF[0]=data[0];
        delay(20);
        errorsRF[1]=data[1];
        delay(20);       
    }
    return error;
}

/*
 Function: Read the number of good frames with valid MAC headers that are received on the RF interface
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "goodPackets" variable the number of good frames received
*/
uint8_t WaspXBee868::getGoodPackets()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_good_pack_868);
    error=gen_send(get_good_pack_868);
    
    if(!error)
    {
        goodPackets[0]=data[0];
        delay(20);
        goodPackets[1]=data[1];
        delay(20);       
    } 
    return error;
}

/*
 Function: Reads the DBM level of the designated channel
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "channelRSSI" variable the DBM level of the designated channel
 Parameters:
   channel --> The channel to get the DBM value
*/
uint8_t WaspXBee868::getChannelRSSI(uint8_t channel)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_channel_RSSI_868,channel);
    gen_checksum(get_channel_RSSI_868);
    error=gen_send(get_channel_RSSI_868);
    
    if(error==0)
    {
        channelRSSI=data[1];
        delay(20);
    } 
    return error;
}

/*
 Function:  Read the number of MAC frames that exhaust MAC retries without ever receiving a MAC acknowledgement message from the adjacent node
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "transmisionErrors" variable the number of MAC frames that exhaust
	 MAC retries
*/
uint8_t WaspXBee868::getTransmisionErrors()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_trans_errors_868);
    error=gen_send(get_trans_errors_868);
    
    if(!error)
    {
        transmisionErrors[0]=data[0];
        delay(20);
        transmisionErrors[1]=data[1];
        delay(20);       
    } 
    return error;
}

/*
 Function: Read module temperature in Celsius. Negatives temperatures can be returned
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the TP command. Stores in global "temperature" variable the read temp in Celsius
*/
uint8_t WaspXBee868::getTemperature()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_temperature_868);
    error=gen_send(get_temperature_868);
    
    if(error==0)
    {
            temperature[0]=data[0];
            delay(20);
            temperature[1]=data[1];    
            delay(20);
    }
    return error;
}

/*
 Function: Reads the voltage on the Vcc pin
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the %V command. Stores in global "supplyVoltage" variable the VCC
*/
uint8_t WaspXBee868::getSupplyVoltage()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_supply_Volt_868);
    error=gen_send(get_supply_Volt_868);
    
    if(error==0)
    {
            supplyVoltage[0]=data[0];
            delay(20);
            supplyVoltage[1]=data[1];    
            delay(20);
    }
    return error;
}

/*
 Function: Get the current device type
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the DD command. Stores in global "deviceType" variable the current device type
*/
uint8_t WaspXBee868::getDeviceType()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_device_type_868);
    error=gen_send(get_device_type_868);
    
    if(error==0)
    {
        for(it=0;it<4;it++)
        {
            deviceType[it]=data[it];
            delay(20);
        }
    }
    return error;
}

/*
 Function: Get the bytes that can be used in the payload
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the NP command. Stores in global "maxPayloadBytes" variable the max Payload
*/
uint8_t WaspXBee868::getPayloadBytes()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_payload_bytes_868);
    error=gen_send(get_payload_bytes_868);
    
    if(error==0)
    {
        maxPayloadBytes[0]=data[0];
        maxPayloadBytes[1]=data[1];
    }
    return error;
} 

/*
 Function: Specifies the number of additional broadcast retransmissions
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the MT command
 Parameters:
   mtrans: number of additional broadcast retransmissions (0x00-0x0F)
*/
uint8_t WaspXBee868::setMultipleBroadcast(uint8_t mtrans)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(set_mult_broadcast_868,mtrans);
    gen_checksum(set_mult_broadcast_868);
    error=gen_send(set_mult_broadcast_868);
    
    if(error==0)
    {
        multipleBroadcast=mtrans;
    }
    return error;
}

/*
 Function: Gets the number of additional broadcast retransmissions
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the MT command
*/
uint8_t WaspXBee868::getMultipleBroadcast()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_mult_broadcast_868);
    error=gen_send(get_mult_broadcast_868);
    
    if(error==0)
    {
        multipleBroadcast=data[0];
    }
    return error;
}

/*
 Function: Specifies the number of retries than can be sent for a given unicast RF packet
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the RR command
 Parameters:
   macretries: number of retries than can be sent for a given unicast RF packet (0x00-0x0F)
*/
uint8_t WaspXBee868::setMacRetries(uint8_t macretries)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(set_retries_868,macretries);
    gen_checksum(set_retries_868);
    error=gen_send(set_retries_868);
    
    if(error==0)
    {
        macRetries=macretries;
    }
    return error;
}

/*
 Function: Gets the number of retries than can be sent for a given unicast RF packet
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the RR command
*/
uint8_t WaspXBee868::getMacRetries()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_retries_868);
    error=gen_send(get_retries_868);
    
    if(error==0)
    {
        macRetries=data[0];
    }
    return error;
}

/*
 Function: Get the % it has been used of available Duty Cicle
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the DC command
*/
uint8_t WaspXBee868::getDutyCicle()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_duty_cicle_868);
    error=gen_send(get_duty_cicle_868);
    
    if(error==0)
    {
        dutyCicle=data[0];
    }
    return error;
}

/*
 Function: Tells the reason for the last module reset
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the R# command
*/
uint8_t WaspXBee868::getResetReason()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_reset_reason_868);
    error=gen_send(get_reset_reason_868);
    
    if(error==0)
    {
        resetReason=data[0];
    }
    return error;
}

/*
 Function: Specifies the number of failed ACK retries
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the TA command
*/
uint8_t WaspXBee868::getACKerrors()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_ACK_errors_868);
    error=gen_send(get_ACK_errors_868);
    
    if(error==0)
    {
        ACKerrors[0]=data[0];
        ACKerrors[1]=data[1];
    }
    return error;
}

WaspXBee868	xbee868 = WaspXBee868();
