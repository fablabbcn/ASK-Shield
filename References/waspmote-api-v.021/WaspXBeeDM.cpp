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

void	WaspXBeeDM::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used)
{
	init(protocol_used, frequency, model_used, 0);
}

void WaspXBeeDM::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used)
{
	protocol=protocol_used;
	freq=frequency;
	model=model_used;
	uart=uart_used;
	
	data_length=0;
	it=0;
	start=0;
	finish=0;
	add_type=0;
	mode=0;
	frag_length=0;
	TIME1=0;
	
	totalFragmentsReceived=0;
	pendingPackets=0;
	pos=0;
	discoveryOptions=0x00;
	
	scanTime[0]=SCAN_TIME_DIGIMESH_H;
	scanTime[1]=SCAN_TIME_DIGIMESH_L;
	encryptMode=ENCRYPT_MODE_DIGIMESH;
	timeRSSI=TIME_RSSI_DIGIMESH;
	
	if( protocol!=XBEE_900 )
	{	
		awakeTime[0]=AWAKE_TIME_DIGIMESH_H;
		awakeTime[1]=AWAKE_TIME_DIGIMESH_M;
		awakeTime[2]=AWAKE_TIME_DIGIMESH_L;
		sleepTime[0]=SLEEP_TIME_DIGIMESH_H;
		sleepTime[1]=SLEEP_TIME_DIGIMESH_M;
		sleepTime[2]=SLEEP_TIME_DIGIMESH_L;
		
		sleepOptions=SLEEP_OPTIONS_DIGIMESH;
		powerLevel=POWER_LEVEL_DIGIMESH;
			
		networkHops=7;
		netDelaySlots=3;
		netRouteRequest=3;
		meshNetRetries=1;
	}
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
uint8_t WaspXBeeDM::getRFerrors()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_RF_errors_DM);
    error=gen_send(get_RF_errors_DM);
    
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
uint8_t WaspXBeeDM::getGoodPackets()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_good_pack_DM);
    error=gen_send(get_good_pack_DM);
    
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
uint8_t WaspXBeeDM::getChannelRSSI(uint8_t channel)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_channel_RSSI_DM,channel);
    gen_checksum(get_channel_RSSI_DM);
    error=gen_send(get_channel_RSSI_DM);
    
    if(!error)
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
uint8_t WaspXBeeDM::getTransmisionErrors()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_trans_errors_DM);
    error=gen_send(get_trans_errors_DM);
    
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
 Function: Set the maximum number of hops expected to be seen in a network route
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NH command
 Parameters:
   nhops: maximum number of hops (1-0xFF)
*/
uint8_t WaspXBeeDM::setNetworkHops(uint8_t nhops)
{
    int8_t error=2;
    
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(set_network_hops_DM,nhops);
	    gen_checksum(set_network_hops_DM);
	    error=gen_send(set_network_hops_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(!error)
    {
        networkHops=nhops;
    }
    return error;
}

/*
 Function: Read the maximum number of hops expected to be seen in a network route
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "networkHops" variable max number of hops
*/
uint8_t WaspXBeeDM::getNetworkHops()
{
    int8_t error=2;
        
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(get_network_hops_DM);
	    error=gen_send(get_network_hops_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        networkHops=data[0];
        delay(20);
    } 
    return error;
}

/*
 Function: Set the maximum random number of network delay slots before rebroadcasting a network packet
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NN command
 Parameters:
   dslots: maximum number of delay slots (0-0x0A)
*/
uint8_t WaspXBeeDM::setNetworkDelaySlots(uint8_t dslots)
{
    int8_t error=2; 
    
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(set_network_delay_DM,dslots);
	    gen_checksum(set_network_delay_DM);
	    error=gen_send(set_network_delay_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        netDelaySlots=dslots;
    }
    return error;
}

/*
 Function: Read the maximum random number of network delay slots before rebroadcasting a network packet
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "netDelaySlots" variable max random number of net delay slots
*/
uint8_t WaspXBeeDM::getNetworkDelaySlots()
{
    int8_t error=2;
        
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(get_network_delay_DM);
	    error=gen_send(get_network_delay_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        netDelaySlots=data[0];
        delay(20);
    } 
    return error;
}

/*
 Function:  Set the maximum number of route discovery retries allowed to find a path to the destination node
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NQ command
 Parameters:
   route: maximum number of route discovery retries (0-0x0A)
*/
uint8_t WaspXBeeDM::setNetworkRouteRequests(uint8_t route)
{
    int8_t error=2;
     
    
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(set_network_route_DM,route);
	    gen_checksum(set_network_route_DM);
	    error=gen_send(set_network_route_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        netRouteRequest=route;
    }
    return error;
}

/*
 Function: Read the maximum number of route discovery retries allowed to find a path to the destination node
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "netRouteRequest" variable max number of route discovery retries
*/
uint8_t WaspXBeeDM::getNetworkRouteRequests()
{
    int8_t error=2;
        
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(get_network_route_DM);
	    error=gen_send(get_network_route_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        netRouteRequest=data[0];
        delay(20);
    } 
    return error;
}

/*
 Function: Set the maximum number of network packet delivery attempts
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the MR command
 Parameters:
   mesh: maximum number of attempts (0-7)
*/
uint8_t WaspXBeeDM::setMeshNetworkRetries(uint8_t mesh)
{
    int8_t error=2; 
    
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(set_network_retries_DM,mesh);
	    gen_checksum(set_network_retries_DM);
	    error=gen_send(set_network_retries_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        meshNetRetries=mesh;
    }
    return error;
}

/*
 Function: Read the maximum number of network packet delivery attempts
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "meshNetRetries" variable max number of net packet delivery attempts
*/
uint8_t WaspXBeeDM::getMeshNetworkRetries()
{
    int8_t error=2;
        
    if( protocol!=XBEE_900 )
    {
	    error_AT=2;
	    gen_data(get_network_retries_DM);
	    error=gen_send(get_network_retries_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    if(error==0)
    {
        meshNetRetries=data[0];
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
 Values: Executes the TP command. Stores in global "temperature" variable the readed temp in Celsius
*/
uint8_t WaspXBeeDM::getTemperature()
{
    int8_t error=2;
        
    if( (protocol==XBEE_900) || ( (protocol==DIGIMESH) && (freq==FREQ900M) ) )
    {
	    error_AT=2;
	    gen_data(get_temperature_DM);
	    error=gen_send(get_temperature_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
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
uint8_t WaspXBeeDM::getSupplyVoltage()
{
    int8_t error=2;
        
    if( (protocol==XBEE_900) || ( (protocol==DIGIMESH) && (freq==FREQ900M) ) )
    {
	    error_AT=2;
	    gen_data(get_supply_Volt_DM);
	    error=gen_send(get_supply_Volt_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
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
 Function: Restore module parameters to compiled defaults
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the R1 command.
*/
uint8_t WaspXBeeDM::restoreCompiled()
{
    int8_t error=2;
        
    if( (protocol==XBEE_900) || ( (protocol==DIGIMESH) && (freq==FREQ900M) ) )
    {
	    error_AT=2;
	    gen_data(restore_compiled_DM);
	    error=gen_send(restore_compiled_DM);
    }
    else {
	    error=-1;
	    error_AT=-1;
    }
    return error;
}

WaspXBeeDM	xbeeDM = WaspXBeeDM();
