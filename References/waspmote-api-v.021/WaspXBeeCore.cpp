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
 *  Version:		0.14
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, Yuri Carmona
 */
 

#ifndef __WPROGRAM_H__
#include "WaspClasses.h"
#endif


/*
Function: Initializes all the global variables that will be used later
Returns: Nothing
Parameters:
  protocol_used: Protocol the XBee is using
  frequency: Frequency the XBee is running on
  model_used: Model of XBee used
*/
void	WaspXBeeCore::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used)
{
	init(protocol_used, frequency, model_used, 0);
}


/*
Function: Initializes all the global variables that will be used later
Returns: Nothing
Parameters:
  protocol_used: Protocol the XBee is using
  frequency: Frequency the XBee is running on
  model_used: Model of XBee used
  uart_used: UART in use
*/
void	WaspXBeeCore::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used)
{
    protocol=protocol_used;
    freq=frequency;
    model=model_used;
    uart=uart_used;

    totalFragmentsReceived=0;
    pendingPackets=0;
    pos=0;
    discoveryOptions=0x00;
    if(protocol==XBEE_802_15_4)
    {
        awakeTime[0]=AWAKE_TIME_802_15_4_H;
        awakeTime[1]=AWAKE_TIME_802_15_4_L;
        sleepTime[0]=SLEEP_TIME_802_15_4_H;
        sleepTime[1]=SLEEP_TIME_802_15_4_L;
        scanTime[0]=SCAN_TIME_802_15_4;
        scanChannels[0]=SCAN_CHANNELS_802_15_4_H;
        scanChannels[1]=SCAN_CHANNELS_802_15_4_L;
        encryptMode=ENCRYPT_MODE_802_15_4;
        powerLevel=POWER_LEVEL_802_15_4;
        timeRSSI=TIME_RSSI_802_15_4;
        sleepOptions=SLEEP_OPTIONS_802_15_4;
    }
    if(protocol==ZIGBEE)
    {
        awakeTime[0]=AWAKE_TIME_ZIGBEE_H;
        awakeTime[1]=AWAKE_TIME_ZIGBEE_L;
        sleepTime[0]=SLEEP_TIME_ZIGBEE_H;
        sleepTime[1]=SLEEP_TIME_ZIGBEE_L;
        scanTime[0]=SCAN_TIME_ZIGBEE;
        scanChannels[0]=SCAN_CHANNELS_ZIGBEE_H;
        scanChannels[1]=SCAN_CHANNELS_ZIGBEE_L;
        timeEnergyChannel=TIME_ENERGY_CHANNEL_ZIGBEE;
        encryptMode=ENCRYPT_MODE_ZIGBEE;
        powerLevel=POWER_LEVEL_ZIGBEE;
        timeRSSI=TIME_RSSI_ZIGBEE;
        sleepOptions=SLEEP_OPTIONS_ZIGBEE;
    }
    if(protocol==DIGIMESH)
    {
        awakeTime[0]=AWAKE_TIME_DIGIMESH_H;
        awakeTime[1]=AWAKE_TIME_DIGIMESH_M;
        awakeTime[2]=AWAKE_TIME_DIGIMESH_L;
        sleepTime[0]=SLEEP_TIME_DIGIMESH_H;
        sleepTime[1]=SLEEP_TIME_DIGIMESH_M;
        sleepTime[2]=SLEEP_TIME_DIGIMESH_L;
        scanTime[0]=SCAN_TIME_DIGIMESH_H;
        scanTime[1]=SCAN_TIME_DIGIMESH_L;
        encryptMode=ENCRYPT_MODE_DIGIMESH;
        powerLevel=POWER_LEVEL_DIGIMESH;
        timeRSSI=TIME_RSSI_DIGIMESH;
        sleepOptions=SLEEP_OPTIONS_DIGIMESH;
    }
	
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
    apsEncryption=0;
    sd_on=0;
    firm_info.already_init=0;
    firm_info.multi_type=3;
}

/*
 Function: Get the 32 lower bits of my MAC address
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: When it is executed stores the returned value by SL command in the global 
         "sourceMacLow[4]" variable
*/
uint8_t WaspXBeeCore::getOwnMacLow()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_own_mac_low);
    error=gen_send(get_own_mac_low);
    
    if(error==0)
    {
        for(it=0;it<4;it++)
        {
            sourceMacLow[it]=data[it];
        }
    }
    return error; 
}

/*
 Function: Get the 32 higher bits of my MAC address
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: When it is executed stores the returned value by SH in the global 
         "sourceMacHigh[4]" variable
*/
uint8_t WaspXBeeCore::getOwnMacHigh()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_own_mac_high);
    error=gen_send(get_own_mac_high);
    
    if(error==0)
    {
        for(it=0;it<4;it++)
        {
            sourceMacHigh[it]=data[it];
        }
    }
    return error;
}

/*
 Function: Get the 64 bits of my MAC address
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes functions getOwnMacLow() and getOwnMacHigh()
*/
 uint8_t WaspXBeeCore::getOwnMac()
{
    int8_t error=2;
    error=getOwnMacLow();
    if(error==0)
    {
        error=getOwnMacHigh();  
    }
    return error;
}

/*
 Function: Set the 16b network address
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command in this protocol
 Parameters: 
   NA_H : Higher byte of Network Address (0x00-0xFF)
   NA_L : Lower byte of Network Address (0x00-0xFF)
   Values: Stores in global "sourceNA[2]" variable the 16b address set by the user
 */
uint8_t WaspXBeeCore::setOwnNetAddress(uint8_t NA_H, uint8_t NA_L)
{
    int8_t error=2;

    if(protocol==XBEE_802_15_4)
    {
        error_AT=2;
        gen_data(set_own_net_address,NA_H,NA_L);
        gen_checksum(set_own_net_address);
        error=gen_send(set_own_net_address);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    
    if(!error)
    {
        sourceNA[0]=NA_H;
        sourceNA[1]=NA_L;
    }
    return error;
}

/*
 Function: Get the 16b network address
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command in this protocol  
 Values: Stores in global "sourceNA[2]" variable the returned 16b network address
 by MY command
*/
uint8_t WaspXBeeCore::getOwnNetAddress()
{
    int8_t error=2;
     

    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) ) 
    {
        error_AT=2;
        gen_data(get_own_net_address);
        error=gen_send(get_own_net_address);
    } 
    else
    {
        error_AT=-1;
        error=-1;
    }
  
    if(!error)
    {
        sourceNA[0]=data[0];
        sourceNA[1]=data[1];
    }
    return error;
}

/*
 Function: Set Baudrate to use
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command in this protocol
 Parameters: 
   baud_rate: integer that contains the baudrate
   Values: Stores in global "baudrate" variable the baudrate
 */
uint8_t WaspXBeeCore::setBaudrate(uint8_t baud_rate)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(set_baudrate,baud_rate);
    gen_checksum(set_baudrate);
    error=gen_send(set_baudrate);
    
    if(!error)
    {
        baudrate=baud_rate;
    }
    return error;
}

/*
 Function: Set API values
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command in this protocol
 Parameters: 
   api_value: integer that contains the api value
   Values: Stores in global "apiValue" variable the baudrate
 */
uint8_t WaspXBeeCore::setAPI(uint8_t api_value)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(set_api_mode,api_value);
    gen_checksum(set_api_mode);
    error=gen_send(set_api_mode);
    
    if(!error)
    {
        apiValue=api_value;
    }
    return error;
}

/*
 Function: Set API options. Enable ZIgBee Application Layer Addressing
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command in this protocol
 Parameters: 
   api_options: integer that contains the baudrate
 */
uint8_t WaspXBeeCore::setAPIoptions(uint8_t api_options)
{
    int8_t error=2;
        
    if( (protocol!=XBEE_802_15_4) )
    {
        error_AT=2;
        gen_data(set_api_options,api_options);
        gen_checksum(set_api_options);
        error=gen_send(set_api_options);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    return error;
}

/*
 Function: Set the network identifier
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Parameters: 
   PANID: Array of integers than contains the 16b or 64b PAN ID
 Values: Stores in global "PAN_ID" variable the recent set PAN ID value
 */
uint8_t WaspXBeeCore::setPAN(uint8_t* PANID)
{
    int8_t error=2;
        
    if( (protocol==XBEE_802_15_4) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) ) 
    {
        error_AT=2;
        gen_data(set_pan,PANID);
        gen_checksum(set_pan);
        error=gen_send(set_pan);
    }
    
    if(protocol==ZIGBEE) 
    {	
        error_AT=2;
        gen_data(set_pan_zb,PANID);
        gen_checksum(set_pan_zb);
        error=gen_send(set_pan_zb);
    }

    if(!error)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) ) 
        {
            for(it=0;it<2;it++)
            {
                PAN_ID[it]=PANID[it];
            }
        }
        if(protocol==ZIGBEE) 
        {
            for(it=0;it<8;it++)
            {
                PAN_ID[it]=PANID[it];
            }
        }
    } 
    return error;
}

/*
 Function: Get Network ID
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "error" variable any error happened while execution
	 Stores in global "PAN_ID" variable the 16b or 64b network PAN ID
*/
uint8_t WaspXBeeCore::getPAN()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_pan);
    if( protocol==ZIGBEE ) error=gen_send(get_pan);
    else error=gen_send(get_pan);
    
    if(!error)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) ) 
        {
            for(it=0;it<2;it++)
            {
                PAN_ID[it]=data[it];
                delay(20);
            }
        }
        if(protocol==ZIGBEE) 
        {
            for(it=0;it<8;it++)
            {
                PAN_ID[it]=data[it];
                delay(20);
            }
        }
    } 
    return error;
}

/*
 Function: Set the module to the sleep mode specified.
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores the returned value by SM command in the global "sleepMode" variable
 Parameters:
   sleep: Defines the sleep mode to use by the XBee (0-5)
*/
uint8_t WaspXBeeCore::setSleepMode(uint8_t sleep)
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(set_sleep_mode_xbee,sleep);
    gen_checksum(set_sleep_mode_xbee);
    error=gen_send(set_sleep_mode_xbee);
    
    if(!error)
    {
        sleepMode=sleep;
    }
    return error;
}

/*
 Function: Get the XBee mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores the XBee mode in the global "sleepMode" variable
*/
uint8_t WaspXBeeCore::getSleepMode()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_sleep_mode_xbee);
    error=gen_send(get_sleep_mode_xbee);
    
    if(error==0)
    {
        sleepMode=data[0];
    } 
    return error;
}

/*
 Function: Set the time the module has to be idle before start sleeping
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the ST parameter in XBee module
	 Stores in global "awakeTime" the value of this time
 Parameters: 
   awake: Array of integers that specifies the time to be awake before sleep
 */
uint8_t WaspXBeeCore::setAwakeTime(uint8_t* awake)
{
    int8_t error=2;
        
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_868) )
    {
        error_AT=2;
        gen_data(set_awake_time,awake);
        gen_checksum(set_awake_time);
        error=gen_send(set_awake_time);
    }
    
    if( (protocol==DIGIMESH) || (protocol==XBEE_900) )
    {
        error_AT=2;
        gen_data(set_awake_time_DM,awake);
        gen_checksum(set_awake_time_DM);
        error=gen_send(set_awake_time_DM);
    }
    
    if(!error)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_868) )
        {
            awakeTime[0]=awake[0];
            awakeTime[1]=awake[1];
        }
        if( (protocol==DIGIMESH) || (protocol==XBEE_900) )
        {
            awakeTime[0]=awake[0];
            awakeTime[1]=awake[1];
            awakeTime[2]=awake[2];
        }
    }
    return error;
}

/*
 Function: Set the cyclic sleeping time of the node
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the SP parameter in the XBee module
	 Stores in global "sleepTime" the value of this time
 Parameters:
   sleep: Array of Integers that specifies the amount of time the module spends sleeping
*/
uint8_t WaspXBeeCore::setSleepTime(uint8_t* sleep)
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_868) )
    {
        error_AT=2;
        gen_data(set_sleep_time,sleep);
        gen_checksum(set_sleep_time);
        error=gen_send(set_sleep_time);
    }
    
    if( (protocol==DIGIMESH) || (protocol==XBEE_900) )
    {
        error_AT=2;
        gen_data(set_sleep_time_DM,sleep);
        gen_checksum(set_sleep_time_DM);
        error=gen_send(set_sleep_time_DM);
    }
    
    if(!error)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_868) )
        {
            sleepTime[0]=sleep[0];
            sleepTime[1]=sleep[1];
        }
        if( (protocol==DIGIMESH) || (protocol==XBEE_900) )
        {
            sleepTime[0]=sleep[0];
            sleepTime[1]=sleep[1];
            sleepTime[2]=sleep[2];
        }
    }
    return error;
}

/*
 Function: Set the channel frequency where the module is going to work 
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Stores the selected channel in the global "channel" variable
 Parameters:
   _channel: Channel used to transmit (0x0B-0x1A)
*/
uint8_t WaspXBeeCore::setChannel(uint8_t _channel)
{
    int8_t error=2;
     

    if( (protocol==XBEE_802_15_4) || (protocol==DIGIMESH) || (protocol==XBEE_900) )
    {
        error_AT=2;
        gen_data(set_channel,_channel);
        gen_checksum(set_channel);
        error=gen_send(set_channel);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(!error)
    {
        channel=_channel;
    }

    return error;
}

/*
 Function: Get the actual frequency channel 
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores the frequency channel in the global "channel" variable
*/
uint8_t WaspXBeeCore::getChannel()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_channel);
    error=gen_send(get_channel);
    
    if(!error)
    {
        channel=data[0];
    }
    return error;
}

/*
 Function: Set the Node Indentifier
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NI to the selected in the function
         The NI must be a 20 character max string
         Stores the given NI in the global "nodeID" variable
 Parameters: 
   node: string that specifies the node indentifier
*/
uint8_t WaspXBeeCore::setNodeIdentifier(const char* node)
{
    uint8_t* NI = (uint8_t*) calloc(30,sizeof(uint8_t)); //{0x7E, 0x00, 0x00, 0x08, 0x52, 0x4E, 0x49, 0x02};
    if( NI==NULL ) return 2;
    NI[0]=0x7E;
    NI[1]=0x00;
    NI[3]=0x08;
    NI[4]=0x52;
    NI[5]=0x4E;
    NI[6]=0x49;
    int8_t error=2;
    uint8_t* ByteIN = (uint8_t*) calloc(20,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    
    uint8_t counter=0;
    uint8_t checksum=0; 


    it=0;
    error_AT=2;
    while( (node[it]!='\0') )
    {
        NI[it+7]=uint8_t(node[it]);
        it++;
    }
    NI[2]=4+it;
    for(it=3;it<(7+(NI[2]-4));it++)
    {
        checksum=checksum+NI[it];
    }
    while( (checksum>255))
    {
        checksum=checksum-256;
    }
    checksum=255-checksum;
    NI[7+NI[2]-4]=checksum;
    while(counter<(8+NI[2]-4))
    {
	if( uart==UART0 ) XBee.print(NI[counter], BYTE); 
	else if( uart==UART1 ) XBee2.print(NI[counter], BYTE); 
        counter++;
    }
    counter=0;
    clearCommand();
    command[5]=0x4E;
    command[6]=0x49;
    error=parse_message(command);

    if(error==0)
    {
        for(it=0;it<NI[2]-4;it++)
        {
            nodeID[it]=node[it];
        }
    }
    free(NI);
    free(ByteIN);
    NI=NULL;
    ByteIN=NULL;
    return error;
}

/*
 Function: Get the Node Identifier
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores the NI in the global "nodeID" variable
*/
uint8_t WaspXBeeCore::getNodeIdentifier()
{
    int8_t error=2; 

    error_AT=2;
    gen_data(get_NI);
    error=gen_send(get_NI);
    
    if(!error)
    {
        for(it=0;it<data_length;it++)
        {
            nodeID[it]=char(data[it]);
        }
    }
    return error;
}

/*
 Function: Scans for brothers in the same channel and same PAN ID
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores given info (SH,SL,MY,RSSI,NI) in global array "scannedBrothers" variable
         Stores in global "totalScannedBrothers" the number of found brothers
*/
uint8_t WaspXBeeCore::scanNetwork()
{
    uint8_t error=2;
	
    error_AT=2;
    totalScannedBrothers=0;
    gen_data(scan_network);
    error=gen_send(scan_network);

    return error;
}

/*
 Function: Scans for brothers in the same channel and same PAN ID
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores given info (SH,SL,MY,RSSI,NI) in global array "scannedBrothers" variable
         Stores in global "totalScannedBrothers" the number of founded brothers
 Parameters:
    node: 20-byte max string containing NI of the node to search
*/
uint8_t WaspXBeeCore::scanNetwork(const char* node)
{
    uint8_t* ND = (uint8_t*) calloc(30,sizeof(uint8_t)); //{0x7E, 0x00, 0x04, 0x08, 0x52, 0x4E, 0x44, 0x13};
    if( ND==NULL ) return 2;
    ND[0]=0x7E;
    ND[1]=0x00;
    ND[3]=0x08;
    ND[4]=0x52;
    ND[5]=0x4E;
    ND[6]=0x44;
    int8_t error=2;
    uint8_t* ByteIN = (uint8_t*) calloc(20,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    
    uint8_t counter=0;
    uint16_t checksum=0;
    uint16_t interval=WAIT_TIME2;
    
    error_AT=2;
    totalScannedBrothers=0;
    if( (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        interval=14000;
    }
    it=0;
    while( (node[it]!='\0') )
    {
        ND[it+7]=uint8_t(node[it]);
        it++;
    }
    ND[2]=4+it;
    for(it=3;it<(7+(ND[2]-4));it++)
    {
        checksum=checksum+ND[it];
    }
    while( (checksum>255))
    {
        checksum=checksum-256;
    }
    checksum=255-checksum;
    ND[7+ND[2]-4]=checksum;
    while(counter<(8+ND[2]-4))
    {
	if( uart==UART0 ) XBee.print(ND[counter], BYTE); 
	else if( uart==UART1 ) XBee2.print(ND[counter], BYTE); 
        counter++;
    }
    counter=0;
    clearCommand();
    command[5]=ND[5];
    command[6]=ND[6];
    error=parse_message(command);
    
    free(ND);
    free(ByteIN);
    ND=NULL;
    ByteIN=NULL;
    return error;
}


/*
 Function: Defines the amount of time the scanNetwork() function is scanning
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Changes the NT command
	 Stores in global "scanTime" variable the recent set time
 Parameters:
   time: amount of time ND is scanning for brothers (0x01-0xFC)
*/
uint8_t WaspXBeeCore::setScanningTime(uint8_t* time)
{
    int8_t error=2;
     
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_900) )
    {
        error_AT=2;
        gen_data(set_scanning_time,time);
        gen_checksum(set_scanning_time);
        error=gen_send(set_scanning_time);
    }
    
    if( (protocol==DIGIMESH) || (protocol==XBEE_868) )
    {
        error_AT=2;
        gen_data(set_scanning_time_DM,time);
        gen_checksum(set_scanning_time_DM);
        error=gen_send(set_scanning_time_DM);
    }
        
    
    if(!error)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) || (protocol==XBEE_900) )
        {
            scanTime[0]=time[0];
        }
        if( (protocol==DIGIMESH) || (protocol==XBEE_868) )
        {
            scanTime[0]=time[0];
            scanTime[1]=time[1];
        }
    }
    return error;
}

/*
 Function: Get the Scanning Time
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "error" variable any error happened while execution
	 Stores in global "scanTime" the value of scanning time
*/
uint8_t WaspXBeeCore::getScanningTime()
{
    int8_t error=2;
     
    error_AT=2;
    gen_data(get_scanning_time);
    if( (protocol==DIGIMESH) || (protocol==XBEE_868) || (protocol==ZIGBEE) || (protocol==XBEE_900) ) error=gen_send(get_scanning_time);
    else error=gen_send(get_scanning_time);
    
    if(!error)
    {
        if( (protocol==XBEE_802_15_4) )
        {
            scanTime[0]=data[0];
        }
        if( (protocol==ZIGBEE) || (protocol==XBEE_900) )
        {
            scanTime[0]=data[1]; 
        }
        if( (protocol==DIGIMESH) || (protocol==XBEE_868) )
        {
            scanTime[0]=data[0];
            scanTime[1]=data[1];
        }
    }
    return error;
}

/*
 Function: Set the options value for the network discovery command
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NO command
 Parameters:
   options: chosen option (0x00-0x03)
*/
uint8_t WaspXBeeCore::setDiscoveryOptions(uint8_t options)
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(set_discov_options,options);
        gen_checksum(set_discov_options);
        error=gen_send(set_discov_options);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(!error)
    {
        discoveryOptions=options;
    }
    return error;
}

/*
 Function: Get the options value for the network discovery command
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the NO command. Stores in global "discoveryOptions" variable the options
*/
uint8_t WaspXBeeCore::getDiscoveryOptions()
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(get_discov_options);
        error=gen_send(get_discov_options);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(error==0)
    {
        discoveryOptions=data[0];
    }
    return error;
}

/*
 Function: Performs a quick search. 
	   802.15.4 : It keeps in DL the MY of the looked up NI brother
	   ZIGBEE : Stores in global "paquete" naD,macDH,macDL from the searched device
	   DIGIMESH: Stores in global "paquete" macDH,macDL from the searched device
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes DN command. 
 Parameters: 
   node: string that specifies the NI that identifies the searched brother
   length: length of that NI (0-20)
*/
uint8_t WaspXBeeCore::nodeSearch(const char* node, struct packetXBee* paq)
{
    uint8_t* DN = (uint8_t*) calloc(30,sizeof(uint8_t)); //{0x7E, 0x00, 0x00, 0x08, 0x52, 0x44, 0x4E, 0xE3};
    if( DN==NULL ) return 2;
    DN[0]=0x7E;
    DN[1]=0x00;
    DN[3]=0x08;
    DN[4]=0x52;
    DN[5]=0x44;
    DN[6]=0x4E;
    uint8_t num_DN;
    int8_t error=2;
    uint8_t* ByteIN = (uint8_t*) calloc(25,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    
    uint8_t counter=0;
    uint8_t checksum=0; 
    uint16_t interval=2000;


    error_AT=2;
    if(protocol==DIGIMESH)
    {
        interval=14000;
    }    
    it=0;
    while( (node[it]!='\0') )
    {
        DN[it+7]=uint8_t(node[it]);
        it++;
    }
    DN[2]=4+it;
    for(it=3;it<(7+(DN[2]-4));it++)
    {
        checksum=checksum+DN[it];
    }
    while( (checksum>255))
    {
        checksum=checksum-256;
    }
    checksum=255-checksum;
    DN[7+DN[2]-4]=checksum;
    while(counter<(8+DN[2]-4))
    {
	if( uart==UART0 ) XBee.print(DN[counter], BYTE); 
	else if( uart==UART1 ) XBee2.print(DN[counter], BYTE); 
        counter++;
    }
    
    counter=0;
    clearCommand();
    command[5]=0x44;
    command[6]=0x4E;
    error=parse_message(command);
    
    if(error==0)
    {
        if( (protocol==ZIGBEE) || (protocol==XBEE_900) || (protocol==XBEE_868) )
        {
            for(it=0;it<2;it++)
            {
                paq->naD[it]=data[it];
            }
            for(it=0;it<4;it++)
            {
                paq->macDH[it]=data[it+2];
            }
            for(it=0;it<4;it++)
            {
                paq->macDL[it]=data[it+6];
            }
        }
        if(protocol==DIGIMESH)
        {
            for(it=0;it<4;it++)
            {
                paq->macDH[it]=data[it];
            }
            for(it=0;it<4;it++)
            {
                paq->macDL[it]=data[it+4];
            }
        }
    }
    free(DN);
    free(ByteIN);
    DN=NULL;
    ByteIN=NULL;
    return error;
}

/*
 Function: Write the current parameters to a non volatil memory
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the WR command
*/
uint8_t WaspXBeeCore::writeValues()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(write_values);
    error=gen_send(write_values);
    
    return error;
}

/*
 Function: Specifies the list of channels to scan when performing an energy scan 
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Change the SC command. Stores in global "scanChannels" variable the list of channels
 Parameters: 
   channel_H: higher byte of list of channels (0x00-0xFF)
   channel_L: lower byte of list of channels (0x00-0xFF
*/
uint8_t WaspXBeeCore::setScanningChannels(uint8_t channel_H, uint8_t channel_L)
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(set_scanning_channel,channel_H,channel_L);
        gen_checksum(set_scanning_channel);
        error=gen_send(set_scanning_channel);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(error==0)
    {
        scanChannels[0]=channel_H;
        scanChannels[1]=channel_L;
    }
    return error;
}

/*
 Function: Get the list of channels to scan when performing an energy scan
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Stores in global "scanChannels" variable the scanning channel list
*/
uint8_t WaspXBeeCore::getScanningChannels()
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(get_scanning_channel);
        error=gen_send(get_scanning_channel);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(error==0)
    {
        for(it=0;it<2;it++)
        {
            scanChannels[it]=data[it];
        }
    }
    return error;
}

/*
 Function: It sets the time the energy scan will be performed
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Change the ED command. Stores in global "energyChannel" variable the energy in each channel
 Parameters:
   duration: amount of time that the energy scan will be performed (0-6)
*/
uint8_t WaspXBeeCore::setDurationEnergyChannels(uint8_t duration)
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) )
    {
        error_AT=2;
        gen_data(set_duration_energy,duration);
        gen_checksum(set_duration_energy);
        error=gen_send(set_duration_energy);
    }
    else if( (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(set_duration_energy_ZB,duration);
        gen_checksum(set_duration_energy_ZB);
        error=gen_send(set_duration_energy_ZB);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
	
    if(error==0)
    {
        if(protocol==XBEE_802_15_4)
        {
            for(it=0;it<data_length;it++)
            {
                energyChannel[it]=data[it];
            }
        }
        if(protocol==ZIGBEE)
        {
            timeEnergyChannel=data[0];
        }
    }
    return error;
}

/*
 Function: It gets the time the energy scan will be performed
 Returns: Integer that determines if there has been any error 
   error=2  --> The command has not been executed
   error=1  --> There has been an error while executing the command
   error=0  --> The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Change the SD command. Stores in global "timeEnergyChannel" variable the time the energy 
	 scan will be performed
*/
uint8_t WaspXBeeCore::getDurationEnergyChannels()
{
    int8_t error=2;
    
    if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(get_duration_energy);
        error=gen_send(get_duration_energy);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(!error)
    {
        timeEnergyChannel=data[0];
    }
    return error;
}

/*
 Function: Sets the encryption key to be used in the 128b AES algorithm
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the KY command. Stores in global "linkKey" variable the key has been set
 Parameters:
   key: 16 byte array of chars that specifies the 128b AES key
*/
uint8_t WaspXBeeCore::setLinkKey(const char* key)
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(set_link_key,key);
    gen_checksum(set_link_key);
    error=gen_send(set_link_key);
    
    if(!error)
    {
        for(it=0;it<16;it++)
        {
            linkKey[it]=char(key[it]);
        }
    }
    return error;
}

/*
 Function: Sets the encryption mode on/off
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the EE command. Stores in global "encryptMode" variable the encryption mode
 Parameters:
   mode: on/off the encryption mode (1/0)
*/
uint8_t WaspXBeeCore::encryptionMode(uint8_t mode)
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(set_encryption,mode);
    gen_checksum(set_encryption);
    error=gen_send(set_encryption);
    if(!error)
    {
        encryptMode=mode;
    }
    return error;
}

/*
 Function: Select the power level at which the RF module transmits
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the PL command. Stores in global "powerLevel" the power level at which RF tx
 Parameters:
   value: power level of transmission (0-4)
*/
uint8_t WaspXBeeCore::setPowerLevel(uint8_t value)
{
    int8_t error=2;
    
    if(protocol!=XBEE_900)
    {
        error_AT=2;
        gen_data(set_power_level,value);
        gen_checksum(set_power_level);
        error=gen_send(set_power_level);
    }
    if(!error)
    {
        powerLevel=value;
    }
    return error;
}

/*
 Function: Get the Received Signal Strength Indicator of the last received packet
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "valueRSSI" variable the RSSI value of last received packet
*/
uint8_t WaspXBeeCore::getRSSI()
{
    int8_t error=2;
    uint8_t* ByteIN = (uint8_t*) calloc(40,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    uint8_t i=0;

    if( (protocol == XBEE_802_15_4 ) || (protocol==ZIGBEE) )
    {
        error_AT=2;
        gen_data(get_RSSI);
        error=gen_send(get_RSSI);
    }
    else if( (protocol== DIGIMESH) || (protocol==XBEE_868) || (protocol==XBEE_900) )
    {
	if( uart==UART0 )
	{
		delay(2000);
		XBee.print("+++");
		delay(2000);
		XBee.flush();
		XBee.println("atdb");
		delay(1000);
		error_AT=2;
		while(XBee.available()>0)
		{
			ByteIN[i]=XBee.read();
			error=0;
			i++;
			error_AT=0;
		}
		i=0;
		XBee.println("atcn");
		delay(1000);
		valueRSSI[0]=Utils.str2hex(ByteIN);
	}
	else if( uart==UART1 )
	{
		delay(2000);
		XBee2.print("+++");
		delay(2000);
		XBee2.flush();
		XBee2.println("atdb");
		delay(1000);
		error_AT=2;
		while(XBee2.available()>0)
		{
			ByteIN[i]=XBee2.read();
			error=0;
			i++;
			error_AT=0;
		}
		i=0;
		XBee2.println("atcn");
		delay(1000);
		valueRSSI[0]=Utils.str2hex(ByteIN);
	}
    }
    if(error==0)
    {
        if( (protocol==XBEE_802_15_4) || (protocol==ZIGBEE) )
        {
            valueRSSI[0]=data[0];
        }
    }
    free(ByteIN);
    ByteIN=NULL;
    return error;
}

/*
 Function: Get the Harware Version
 Returns: Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "hardVersion" variable the Hardware Version
*/
uint8_t WaspXBeeCore::getHardVersion()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(get_hard_version);
    error=gen_send(get_hard_version);
    if(!error)
    {
        hardVersion[0]=data[0];
        hardVersion[1]=data[1];
    } 
    return error;
}

/*
 Function: Get the version of the firmware
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "softVersion" variable the firmware version
*/
uint8_t WaspXBeeCore::getSoftVersion()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(get_soft_version);
    error=gen_send(get_soft_version);
    if(error==0)
    {
        softVersion[0]=data[0];
        softVersion[1]=data[1];
    } 
    return error;
}


/*
 Function: Set the RSSI time
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the RP command. Stores in global "timeRSSI" variable the RSSI time
 Parameters:
   time: amount of time to do the pwm (0x00-0xFF)
*/
uint8_t WaspXBeeCore::setRSSItime(uint8_t time)
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(set_RSSI_time,time);
    gen_checksum(set_RSSI_time);
    error=gen_send(set_RSSI_time);
    if(!error)
    {
        timeRSSI=time;
    }
    return error;
}

/*
 Function: Get the RSSI time
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Stores in global "timeRSSI" variable the RSSI time
*/
uint8_t WaspXBeeCore::getRSSItime()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(get_RSSI_time);
    error=gen_send(get_RSSI_time);
    if(!error)
    {
        timeRSSI=data[0];
    } 
    return error;
}

/*
 Function:  Immediately applies new settings without exiting command mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the AC command
*/
uint8_t WaspXBeeCore::applyChanges()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(apply_changes);
    error=gen_send(apply_changes);
    return error;
}


/*
 Function: Reset the XBee Firmware
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the FR command  
*/
uint8_t WaspXBeeCore::reset()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(reset_xbee);
    error=gen_send(reset_xbee);
    return error;
}

/*
 Function: Set the parameteres to the factory defaults
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the RE command  
*/
uint8_t WaspXBeeCore::resetDefaults()
{
    int8_t error=2;
    
    error_AT=2;
    gen_data(reset_defaults_xbee);
    error=gen_send(reset_defaults_xbee);
    return error;
}

/*
 Function: Configure options for sleep
 Returns: Integer that determines if there has been any error 
   error=2 -->  The command has not been executed
   error=1 -->  There has been an error while executing the command
   error=0 -->  The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Change the SO command. Stores in global "sleepOptions" variable the options
 Parameters:
   soption: options for sleep (0x00-0xFF)
*/
uint8_t WaspXBeeCore::setSleepOptions(uint8_t soption)
{
    int8_t error=2;
        
    if( (protocol==ZIGBEE) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        error_AT=2;
        gen_data(set_sleep_options_xbee,soption);
        gen_checksum(set_sleep_options_xbee);
        error=gen_send(set_sleep_options_xbee);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(!error)
    {
        sleepOptions=soption;
    }
    return error;
}

/*
 Function: Reads the options for sleep
 Returns: Integer that determines if there has been any error 
   error=2 -->  The command has not been executed
   error=1 -->  There has been an error while executing the command
   error=0 -->  The command has been executed with no errors
   error=-1 --> Forbidden command for this protocol
 Values: Executes the SO command. Stores in global "sleepOptions" variable the options
*/
uint8_t WaspXBeeCore::getSleepOptions()
{
    int8_t error=2;
        
    if( (protocol==ZIGBEE) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        error_AT=2;
        gen_data(get_sleep_options_xbee);
        error=gen_send(get_sleep_options_xbee);
    }
    else
    {
        error_AT=-1;
        error=-1;
    }
    if(!error)
    {
        sleepOptions=data[0]; 
    }
    return error;
}

/*
 Function: Transparent function. The user introduces an AT command within a string and the function executes it without knowing its meaning
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Parameters:
  atcommand : String to specify the AT command to execute
*/
uint8_t WaspXBeeCore::sendCommandAT(const char* atcommand)
{
    uint8_t* AT = (uint8_t*) calloc(30,sizeof(uint8_t));// {0x7E, 0x00, 0x00, 0x08, 0x52, 0x00, 0x00, 0x00};
    if( AT==NULL ) return 2;
    AT[0]=0x7E;
    AT[1]=0x00;
    AT[3]=0x08;
    AT[4]=0x52;
    int8_t error=2;
    uint8_t it2=0;
    
    uint8_t* ByteIN = (uint8_t*) calloc(120,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    uint8_t counter=0;
    uint8_t checksum=0; 
    uint16_t length=0;
    
    it=0;
    error_AT=2;
    while( atcommand[it2]!='#' )
    {
        if( it>=2 )
        {
            if( atcommand[it2+1]!='#' )
            {
                AT[it+5]=Utils.converter(atcommand[2*(it-1)],atcommand[2*(it-1)+1]);
                it2+=2;
            }
            else
            {
                switch( atcommand[it2] )
                {
                    case '0':	AT[it+5]=0;
                    break;
                    case '1':	AT[it+5]=1;
                    break;
                    case '2':	AT[it+5]=2;
                    break;
                    case '3':	AT[it+5]=3;
                    break;
                    case '4':	AT[it+5]=4;
                    break;
                    case '5':	AT[it+5]=5;
                    break;
                    case '6':	AT[it+5]=6;
                    break;
                    case '7':	AT[it+5]=7;
                    break;
                    case '8':	AT[it+5]=8;
                    break;
                    case '9':	AT[it+5]=9;
                    break;
                    case 'A':	AT[it+5]='A';
                    break;
                    case 'B':	AT[it+5]='B';
                    break;
                    case 'C':	AT[it+5]='C';
                    break;
                    case 'D':	AT[it+5]='D';
                    break;
                    case 'E':	AT[it+5]='E';
                    break;
                    case 'F':	AT[it+5]='F';
                    break;
                }
                it2++;
            }
        }
        else
        {
            AT[it+5]=atcommand[it];
            it2++;
        }
        it++;
    } 
    length=it;
    
    AT[2]=2+length;
    for(it=3;it<(5+length);it++)
    {
        checksum=checksum+AT[it];
    }
    while( (checksum>255))
    {
        checksum=checksum-256;
    }
    checksum=255-checksum;
    AT[5+length]=checksum;
    while(counter<(6+length))
    {
	if( uart==UART0 ) XBee.print(AT[counter], BYTE); 
	else if( uart==UART1 ) XBee2.print(AT[counter], BYTE); 
        counter++;
    }
    counter=0;
    clearCommand();
    command[5]=AT[5];
    command[6]=AT[6];
    data_length=0;
    error=parse_message(command);
    if(error==0)
    {
        if(data_length>0)
        {
            for(it=0;it<data_length;it++)
            {
                commandAT[it]=data[it];
                delay(20);
            }
        }
        else
        {
            commandAT[0]=0x4F;
            commandAT[1]=0x4B;
        }
    }
    
    free(AT);
    AT=NULL;
    free(ByteIN);
    ByteIN=NULL;
    return error;
}

/*
 Function: Connect XBee, activating switch in Waspmote
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::ON()
{
    uint8_t error=2;
    if( uart==UART0 )
    {
	    XBee.begin(uart,XBEE_RATE);
	    XBee.setMode(XBEE_ON);
    }
    else if( uart==UART1 )
    {
	    XBee2.begin(uart,XBEE_RATE);
	    XBee2.setMode(XBEE_ON);
    }
    if( protocol== ZIGBEE || protocol==XBEE_868 ) delay(500);
    else delay(50);
    error=0;
    XBee_ON=1;
    return error;
}


/*
 Function: disconnects XBee, switching it off and closing the UART
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::OFF()
{
    uint8_t error=2;
    if( uart==UART0 )
    {
	XBee.close();
	XBee.setMode(XBEE_OFF);
    }
    else if( uart==UART1 )
    {
	XBee2.close();
	XBee2.setMode(XBEE_OFF);
    }
    error=0;
    XBee_ON=0;
    return error;
}


/*
 Function: Set XBee to sleep, asserting PIN 9
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::sleep()
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
uint8_t WaspXBeeCore::wake()
{
    uint8_t error=2; 
    if( uart==UART0 )
    {
	pinMode(XBEE_SLEEP, OUTPUT);
	digitalWrite(XBEE_SLEEP,LOW); 
	XBee.begin(uart,XBEE_RATE);
    }
    else if( uart==UART1 )
    {
	pinMode(DIGITAL7, OUTPUT);
    	digitalWrite(DIGITAL7,LOW); 
	XBee2.begin(uart,XBEE_RATE);
    }
    delay(50);
    error=0;
    return error;
}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Parameters: 
   packet :	A struct of packetXBee type
*/
uint8_t WaspXBeeCore::sendXBeePriv(struct packetXBee* packet)
{
    uint8_t* TX = (uint8_t*) calloc(120,sizeof(uint8_t));
    if( TX==NULL ) return 2;
    uint8_t counter=0;
    uint8_t checksum=0; 
    long previous=0;
    uint16_t aux=0;
    uint8_t protegido=0;
    uint8_t tipo=0;
    uint8_t estado=1;
    int8_t error=2;
    uint8_t* ByteIN = (uint8_t*) calloc(20,sizeof(uint8_t));
    if( ByteIN==NULL ) return 2;
    uint8_t old_netAddress[2];
    uint8_t net_Address_changed = 0;
    uint8_t a=5;

    clearCommand();

    error_TX=2;
    for(it=0;it<120;it++)
    {
        TX[it]=0;
    }
    TX[0]=0x7E;
    TX[1]=0x00;
    TX[4]=packet->packetID; // frame ID
    it=0;
    error_AT=2;
    if(protocol==XBEE_802_15_4)
    {
        if(packet->mode==BROADCAST)
        {
            tipo=15;
            TX[3]=0x00;
	    
            previous=millis();
            error_AT=2;
            while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
            {
                estado=getOwnNetAddress();
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
            }
			
            old_netAddress[0]=sourceNA[0];
            old_netAddress[1]=sourceNA[1];
	    
            previous=millis();
            error_AT=2;
            while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
            {
                estado=setOwnNetAddress(0xFF,0xFF);
                net_Address_changed = 1;
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
            }
            error=2;
            while(a<13) // destination address
            {
                for(it=0;it<4;it++)
                {
                    TX[a]=0x00;
                    a++; 
                }
                for(it=0;it<2;it++)
                {
                    TX[a]=0x00;
                    a++;
                }
                for(it=0;it<2;it++)
                {
                    TX[a]=0xFF;
                    a++;
                }
            }
            TX[13]=0x00;
            TX[14]=packet->packetID;
            TX[15]=packet->numFragment;
            it=0;
            gen_frame(packet,TX,16);
            TX[2]=11+packet->frag_length; // fragment length
            for(it=3;it<(TX[2]+3);it++) // calculating checksum
            {
                checksum=checksum+TX[it];
            }
            while((checksum>255))
            {
                checksum=checksum-256;
            }
            checksum=255-checksum;
            TX[packet->frag_length+14]=checksum; // setting checksum
        }   
        if(packet->mode==UNICAST)
        {
            if(packet->address_type==_64B)
            {
                tipo=15;
                TX[3]=0x00;
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDH[it];
                        a++; 
                    }
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDL[it];
                        a++;
                    }
                }
			
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=getOwnNetAddress();
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
			
                old_netAddress[0]=sourceNA[0];
                old_netAddress[1]=sourceNA[1];
			
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=setOwnNetAddress(0xFF,0xFF);
                    net_Address_changed = 1;
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
                error=2;
                TX[13]=0x00;
                TX[14]=packet->packetID;
                TX[15]=packet->numFragment;
                it=0;
                gen_frame(packet,TX,16);
                TX[2]=11+packet->frag_length; // fragment length
                for(it=3;it<(TX[2]+3);it++) // calculating checksum
                {
                    checksum=checksum+TX[it];
                }
                while((checksum>255))
                {
                    checksum=checksum-256;
                }
                checksum=255-checksum;
                TX[packet->frag_length+14]=checksum; // setting checksum
            }
            if(packet->address_type==_16B)
            {
                tipo=9;
                TX[3]=0x01;
                TX[5]=packet->naD[0];
                TX[6]=packet->naD[1];
                TX[7]=0x00;
                TX[8]=packet->packetID;
                TX[9]=packet->numFragment;
                it=0;
                gen_frame(packet,TX,10);
                TX[2]=5+packet->frag_length; // fragment length
                for(it=3;it<(TX[2]+3);it++) // calculating checksum
                {
                    checksum=checksum+TX[it];
                }
                while((checksum>255))
                {
                    checksum=checksum-256;
                }
                checksum=255-checksum;
                TX[packet->frag_length+8]=checksum; // setting checksum
            }
        }
        if(packet->mode==SYNC)
        {
            tipo=15;
            TX[3]=0x00;
            if(packet->opt==1) // Broadcast
            {
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=getOwnNetAddress();
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
			
                old_netAddress[0]=sourceNA[0];
                old_netAddress[1]=sourceNA[1];
		
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=setOwnNetAddress(0xFF,0xFF);
                    if( (error_AT==1) || (error_AT==2) )
                    {
                        TIME1=millis();
                        delay(100);
                    }
                    net_Address_changed = 1;
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
                error=2;
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=0x00;
                        a++; 
                    }
                    for(it=0;it<2;it++)
                    {
                        TX[a]=0x00;
                        a++;
                    }
                    for(it=0;it<2;it++)
                    {
                        TX[a]=0xFF;
                        a++;
                    }
                }
            }
            else if(packet->opt==0) // UNICAST 64B
            {
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDH[it];
                        a++; 
                    }
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDL[it];
                        a++;
                    }
                }
	    
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=getOwnNetAddress();
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
			
                old_netAddress[0]=sourceNA[0];
                old_netAddress[1]=sourceNA[1];
	    
                previous=millis();
                error_AT=2;
                while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
                {
                    estado=setOwnNetAddress(0xFF,0xFF);
                    net_Address_changed = 1;
		    if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
                }
            }
            TX[13]=0x00;
            TX[14]=packet->packetID;
            TX[15]=packet->numFragment;
            it=0;
            gen_frame(packet,TX,16);
            TX[2]=11+packet->frag_length; // fragment length
            for(it=3;it<(TX[2]+3);it++) // calculating checksum
            {
                checksum=checksum+TX[it];
            }
            while((checksum>255))
            {
                checksum=checksum-256;
            }
            checksum=255-checksum;
            TX[packet->frag_length+14]=checksum; // setting checksum
        }
    // AP = 2
        gen_frame_ap2(packet,TX,protegido,tipo);
    // Frame OK
        counter=0;

        while(counter<(packet->frag_length+tipo+protegido))
        {
	    if( uart==UART0 ) XBee.print(TX[counter], BYTE); 
	    else if( uart==UART1 )
	    {
		Utils.setMuxGPRS();
		XBee2.print(TX[counter], BYTE); 
            }
            counter++;
        }
        counter=0;
    
        command[0]=0xFF;
        error=parse_message(command);
        packet->deliv_status=delivery_status;
    
        if( net_Address_changed )
        {
            error_AT=2;
            previous=millis();
            while( ((error_AT==1) || (error_AT==2)) && (millis()-previous<5000) )
            {
                estado=setOwnNetAddress(old_netAddress[0],old_netAddress[1]);
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
            }
        }
    
    }
    
    if( (protocol==ZIGBEE) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        if( (packet->mode==BROADCAST) || (packet->mode==UNICAST) )
        {
            if(protocol==XBEE_868)
            {
                if(packet->frag_length>241)
                {
                    TX[1]=0x01;
                    aux=packet->frag_length+14;
                    aux=aux-256;
                    if(aux>9)
                    {
                        switch(aux)
                        {
                            case 10: TX[2]=0x0A;
                            break;
                            case 11: TX[2]=0x0B;
                            break;
                            case 12: TX[2]=0x0C;
                            break;
                            case 13: TX[2]=0x0D;
                            break;
                            case 14: TX[2]=0x0E;
                            break;
                        }
                    }
                    else
                    {
                        TX[2]=aux;
                    }
                }
                else
                {
                    TX[1]=0x00;
                    TX[2]=14+packet->frag_length;
                }
            }
            else
            {
                TX[2]=14+packet->frag_length; // fragment length
            }
            aux=0;
            TX[3]=0x10; // frame ID
            tipo=18;	   
            if(packet->mode==BROADCAST)
            {
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=0x00;
                        a++; 
                    }
                    for(it=0;it<2;it++)
                    {
                        TX[a]=0x00;
                        a++;
                    }
                    for(it=0;it<2;it++)
                    {
                        TX[a]=0xFF;
                        a++;
                    }
                }
                TX[13]=0xFF;
                TX[14]=0xFE;
            }
            if(packet->mode==UNICAST)
            {
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDH[it];
                        a++; 
                    }
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDL[it];
                        a++;
                    }
                }
                if (packet->MY_known==1) // destination network address
                {
                    for(it=0;it<2;it++) 
                    {
                        TX[it+13]=packet->naD[it];
                    }
                }
                else
                {
                    TX[13]=0xFF;
                    TX[14]=0xFE;
                }
            }
            if(protocol==XBEE_868)
            {
                TX[13]=0xFF;
                TX[14]=0xFE;
            }
            TX[15]=hops;
            if( apsEncryption) TX[16]=0x20;
            else TX[16]=packet->opt;
            TX[17]=packet->packetID;
            TX[18]=packet->numFragment;
            it=0;
            gen_frame(packet,TX,19);
            for(it=3;it<(TX[2]+3);it++) // calculating checksum
            {
                checksum=checksum+TX[it];
            }
            while((checksum>255))
            {
                checksum=checksum-256;
            }
            checksum=255-checksum;
            TX[packet->frag_length+17]=checksum; // setting checksum
        }
        else // Cluster Type
        {
            if( (protocol==ZIGBEE) || (protocol==XBEE_868) )
            {
                if(protocol==XBEE_868)
                {
                    if(packet->frag_length>235)
                    {
                        TX[1]=0x01;
                        aux=packet->frag_length+20;
                        aux=aux-256;
                        if(aux>9)
                        {
                            switch(aux)
                            {
                                case 10: TX[2]=0x0A;
                                break;
                                case 11: TX[2]=0x0B;
                                break;
                                case 12: TX[2]=0x0C;
                                break;
                                case 13: TX[2]=0x0D;
                                break;
                                case 14: TX[2]=0x0E;
                                break;
                                case 15: TX[2]=0x0F;
                                break;
                                case 16: TX[2]=0x10;
                                break;
                                case 17: TX[2]=0x11;
                                break;
                                case 18: TX[2]=0x12;
                                break;
                                case 19: TX[2]=0x13;
                                break;
                                case 20: TX[2]=0x14;
                                break;
                            }
                        }
                        else
                        {
                            TX[2]=aux;
                        }
                    }
                    else
                    {
                        TX[1]=0x00;
                        TX[2]=20+packet->frag_length;
                    }
                }
                else
                {
                    TX[2]=20+packet->frag_length; // fragment length
                }
                TX[3]=0x11; // frame ID
                tipo=24;
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDH[it];
                        a++; 
                    }
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDL[it];
                        a++;
                    }
                }
                if (packet->MY_known==1) // destination network address
                {
                    for(it=0;it<2;it++) 
                    {
                        TX[it+13]=packet->naD[it];
                    }
                }
                else
                {
                    TX[13]=0xFF;
                    TX[14]=0xFE;
                }
                TX[15]=packet->SD;
                TX[16]=packet->DE;
                TX[17]=packet->CID[0];
                TX[18]=packet->CID[1];     
                TX[19]=packet->PID[0];
                TX[20]=packet->PID[1];     
                TX[21]=hops;
                if( apsEncryption) TX[22]=0x20;
                else TX[22]=packet->opt;
                TX[23]=packet->packetID;
                TX[24]=packet->numFragment;
                it=0;
                gen_frame(packet,TX,25);
                for(it=3;it<(TX[2]+3);it++) // calculating checksum
                {
                    checksum=checksum+TX[it];
                }
                while((checksum>255))
                {
                    checksum=checksum-256;
                }
                checksum=255-checksum;
                TX[packet->frag_length+23]=checksum; // setting checksum
            }
            else // DIGIMESH or XBEE_900
            {
                TX[2]=20+packet->frag_length; // fragment length
                TX[3]=0x11; // frame ID
                tipo=24;
                while(a<13) // destination address
                {
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDH[it];
                        a++; 
                    }
                    for(it=0;it<4;it++)
                    {
                        TX[a]=packet->macDL[it];
                        a++;
                    }
                }
                if (packet->MY_known==1) // destination network address
                {
                    for(it=0;it<2;it++) 
                    {
                        TX[it+13]=packet->naD[it];
                    }
                }
                else
                {
                    TX[13]=0xFF;
                    TX[14]=0xFE;
                }
                TX[15]=packet->SD;
                TX[16]=packet->DE;
                TX[17]=0x00;
                TX[18]=packet->CID[0];     
                TX[19]=packet->PID[0];
                TX[20]=packet->PID[1];     
                TX[21]=hops;
                TX[22]=packet->opt;
                TX[23]=packet->packetID;
                TX[24]=packet->numFragment;
                it=0;
                gen_frame(packet,TX,25);
                for(it=3;it<(TX[2]+3);it++) // calculating checksum
                {
                    checksum=checksum+TX[it];
                }
                while((checksum>255))
                {
                    checksum=checksum-256;
                }
                checksum=255-checksum;
                TX[packet->frag_length+23]=checksum; // setting checksum
            }
        }
        
    // AP = 2
        gen_frame_ap2(packet,TX,protegido,tipo);
    // Frame OK
        while(counter<(packet->frag_length+tipo+protegido))
        {
	    if( uart==UART0 ) XBee.print(TX[counter], BYTE); 
	    else if( uart==UART1 ) 
	    {
		Utils.setMuxGPRS();
		XBee2.print(TX[counter], BYTE);
	    }
            counter++;
        }
        counter=0;    
        command[0]=0xFE;
        error=parse_message(command);
        packet->deliv_status=delivery_status;
        packet->discov_status=discovery_status;
        packet->true_naD[0]=true_naD[0];
        packet->true_naD[1]=true_naD[1];
        packet->retries=retries_sending;
    }
    free(TX);
    free(ByteIN);
    TX=NULL;
    ByteIN=NULL;
    return error;
}

/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Parameters: 
   packet : A struct of packetXBee type
*/
uint8_t WaspXBeeCore::sendXBee(struct packetXBee* packet)
{
    uint8_t estadoSend=0;
    uint8_t maxPayload=0;
    uint8_t numPackets=0;
    uint8_t maxPackets=0;
    uint16_t lastPacket=0;
    uint16_t aux3=0;
    int8_t error=2;
    uint8_t firstPacket=1;
    uint16_t counter1=0;
    uint8_t type=0;
    uint8_t header=0;

  
    it=0;

  //FIXME Add max payloads for Cluster type
    if(protocol==XBEE_802_15_4)
    {
        if(encryptMode==0)
        {
            maxPayload=100;
        }
        else
        {
            if(packet->mode==BROADCAST)
            {
                maxPayload=95;
            }
            else
            {
                if(packet->address_type==_16B)
                {
                    maxPayload=98;
                }
                else
                {
                    maxPayload=94;
                }
            }
        }
    }
    if(protocol==ZIGBEE)
    {
        if(encryptMode==0)
        {
            if(packet->mode==BROADCAST)
            {
                maxPayload=92;
            }
            else
            {
                maxPayload=84;
            }
        }
        else
        {
            if(packet->mode==BROADCAST)
            {
                if(apsEncryption) maxPayload=70;
                else maxPayload=74;
            }
            else
            {
                if(apsEncryption) maxPayload=62;
                else maxPayload=66;
            }
        }
    }
    if( (protocol==DIGIMESH) )
    {
        maxPayload=73;
    }
    if( (protocol==XBEE_900) )
    {
        if(encryptMode) maxPayload=80;
        else maxPayload=100;
    }
    if(protocol==XBEE_868)
    {
        maxPayload=100;
    }

    switch(packet->typeSourceID)
    {
        case MY_TYPE: 	type=2;
        break;
        case MAC_TYPE: 	type=8;
        break;
        case NI_TYPE: 	while(packet->niO[it]!='#'){
            counter1++;
            it++;
        }
        type=counter1+1;
        break;
        default:	break;
    }
    header=3+firstPacket+type;
    aux3=packet->data_length;
    if((aux3+header)<=maxPayload)
    {
        lastPacket=aux3+header;
        numPackets=1;
    }
    else
    {
        while((aux3+header)>maxPayload)
        {
            numPackets++;
            aux3=aux3-maxPayload+header;
            firstPacket=0;
            header=3+firstPacket+type;
            if((aux3+header)<=maxPayload)
            {
                lastPacket=aux3+header;
                numPackets++;
            }
        }
    }
    maxPackets=numPackets;
  
  
    while(estadoSend!=1)
    {
        while(numPackets>0)
        {
            packet->numFragment=numPackets;
            if(numPackets==1) // last fragment
            {
                packet->frag_length=lastPacket;
            }
            else
            {
                packet->frag_length=maxPayload;
            }
            if(numPackets==maxPackets)
            {
                start=0;
                firstPacket=1;
                header=3+firstPacket+type;
                packet->endFragment=1;
            }
            else
            {
                start=finish+1;
                firstPacket=0;
                header=3+firstPacket+type;
                packet->endFragment=0;
            }
            if(numPackets==1)
            {
                finish=packet->data_length-1;
            }
            else
            {
                finish=start+packet->frag_length-header-1;
            }
            frag_length=packet->frag_length;

            error=sendXBeePriv(packet);
            if(error==0)
            {
                numPackets--;
                if(numPackets==0)
                {
                    estadoSend=1;
                }
                else delay(50);
            }
            else
            {
                numPackets=0;
                estadoSend=1;
            }
        }
    }
    return error;

}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::send(uint8_t* address, uint8_t* data)
{	
	char macDest[17];
	uint8_t length=0;
	
	Utils.hex2str(address, macDest);
	
	while( !((data[length]==0xAA) && (data[length+1]==0xAA)) ) length++;
		
	return send(macDest,(char*) data,0,length);
}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::send(char* address, uint8_t* data)
{	
	uint8_t length=0;

	while( !((data[length]==0xAA) && (data[length+1]==0xAA)) ) length++;
		
	return send(address,(char*) data,0,length);
}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::send(uint8_t* address, char* data)
{	
	char macDest[17];
	Utils.hex2str(address, macDest);
		
	return send(macDest,data,1,0);
}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::send(char* address, char* data)
{
	return send(address,data,1,0);
}


/*
 Function: Send a packet from one XBee to another XBee in API mode
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::send(char* address, char* data, uint8_t type, uint8_t dataMax)
{
	uint8_t maxPayload = 0;
	uint8_t i=0;
	uint8_t j=0;
	char aux[2];
	uint8_t error=2;
	uint8_t destination[8];
	uint8_t maxData = 0;
	uint8_t checksum = 0;
	uint8_t tipo = 0;
			
	switch( protocol )
	{
		case XBEE_802_15_4 :	if( !encryptMode ) maxPayload = 100;
					else
					{
						if( !strcmp(address,"000000000000FFFF") ) maxPayload = 95;
						else maxPayload = 94;
					}
					tipo = 15;
					break;
		case ZIGBEE :		if( !encryptMode )
					{
						if( !strcmp(address,"000000000000FFFF") ) maxPayload = 92;
						else maxPayload = 84;
					}
					else
					{
						if( !strcmp(address,"000000000000FFFF") ) maxPayload = 74;
						else maxPayload = 66;
					}
					tipo = 18;
					break;
		case DIGIMESH :		tipo = 18;
					maxPayload = 73;
					break;
		case XBEE_900 :		tipo = 18;
					if(encryptMode) maxPayload=80;
					else maxPayload=100;
					break;
		case XBEE_868 :		tipo = 18;
					maxPayload = 100;
					break;
	}

	if( type==1 )
	{
		while( data[i]!='\0' )
		{
			maxData++;
			i++;
		}
		i=0;
	}
	else if( type==0 ) maxData=dataMax;
	
	
	if( maxData > maxPayload )
	{
		error_TX = 2;
		return -1;
	}
	
	while(j<8)
	{
		aux[i-j*2]=address[i];
		aux[(i-j*2)+1]=address[i+1];
		destination[j]=Utils.str2hex(aux);
		i+=2;
		j++;
	}
	
	uint8_t* command = (uint8_t*) calloc(130,sizeof(uint8_t));
	if(command==NULL){
		error_TX = 2;
		return -1;
	}
	
	switch( protocol )
	{
		case XBEE_802_15_4 :	command[0] = 0x7E;
					command[1] = 0x00;
					command[2] = maxData+11;
					command[3] = 0x00;
					command[4] = 0x01;
					for(it=0;it<8;it++) command[it+5]=destination[it];
					command[13]=0x00;
					for(it=0;it<maxData;it++) command[it+14]=data[it];
					for(it=3;it<(maxData+14);it++)
					{
						checksum=checksum+command[it];
					}
					while( (checksum>255))
					{
						checksum=checksum-256;
					}
					checksum=255-checksum;
					command[14+maxData]=checksum;
					break;

		case DIGIMESH	:	

		case XBEE_900	:
			
		case XBEE_868	:
			
		case ZIGBEE	:	command[0] = 0x7E;
					command[1] = 0x00;
					command[2] = maxData+14;
					command[3] = 0x10;
					command[4] = 0x01;
					for(it=0;it<8;it++) command[it+5]=destination[it];
					command[13]=0xFF;
					command[14]=0xFE;
					command[15]=0x00;
					command[16]=0x00;
					for(it=0;it<maxData;it++) command[it+17]=data[it];
					for(it=3;it<(maxData+17);it++)
					{
						checksum=checksum+command[it];
					}
					while( (checksum>255))
					{
						checksum=checksum-256;
					}
					checksum=255-checksum;
					command[17+maxData]=checksum;
					break;
	}
		
	it=0;
	while(it<(maxData+tipo))
	{
		if( uart==UART0 ) XBee.print(command[it], BYTE); 
		else if( uart==UART1 ) XBee2.print(command[it], BYTE); 
		it++;
	}
	
    		
	if( protocol==XBEE_802_15_4 ) command[0]=0xFF;
	else if( protocol==ZIGBEE || protocol==DIGIMESH || protocol==XBEE_900 || protocol==XBEE_868) command[0]=0xFE;
		
	error=parse_message(command);
		
	free(command);
	command=NULL;
	
	return error;
}



/*
 Function: Treats any data from the XBee UART
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::treatData()
{
	int8_t error=2;
	uint8_t pos_aux=0;
	uint8_t pos_old=0;
	uint8_t pos_disor=0;
	uint8_t pos_max=0;
	uint8_t first_stop=1;
		
	command[0]=0xEE;
	error=parse_message(command);
		
	pos_max=pos;
	pos_old=pos;
	pos_aux=pos;
	if(pos_aux>1) pos=1;
	
	if( !error_RX )
	{
		while(pos_aux>0)
		{
			switch(packet_finished[pos-1]->packetID)
			{
				case 0xFA:	error=new_firmware_received();
						if(!error)
						{
							while(pos_max>0)
							{
								free(packet_finished[pos_max-1]);
								packet_finished[pos_max-1]=NULL;
								pos_max--;
								pos_old--;
							}
							pos_aux=1;
						}
						break;
				case 0xFB:	new_firmware_packets();
						if( !firm_info.paq_disordered )
						{
							free(packet_finished[pos-1]);
							packet_finished[pos-1]=NULL;
							pos_old--;
						}
						else if(firm_info.paq_disordered==1){
							pos_disor=pos;
						}
						break;
				case 0xFC:	new_firmware_end();
						while(pos_max>0)
						{
							free(packet_finished[pos_max-1]);
							packet_finished[pos_max-1]=NULL;
							pos_max--;
							pos_old--;
						}
						pos_aux=1;
						break;
				case 0xFD:	upload_firmware();
						pos_old--;
						break;
				case 0xFE:	request_ID();
						free(packet_finished[pos-1]);
						packet_finished[pos-1]=NULL;
						pos_old--;
						break;
				case 0xFF:	request_bootlist();
						free(packet_finished[pos-1]);
						packet_finished[pos-1]=NULL;
						pos_old--;
						break;
				case 0xF8:	delete_firmware();
						free(packet_finished[pos-1]);
						packet_finished[pos-1]=NULL;
						pos_old--;
						break;
				case 0xF9:	free(packet_finished[pos-1]);
						packet_finished[pos-1]=NULL;
						pos_old--;
						break;
				default:	if( programming_ON )
						{
							free(packet_finished[pos-1]);
							packet_finished[pos-1]=NULL;
							pos_old--;
						}
						break;
			}
			// Handle disordered packets
			if(!firm_info.paq_disordered && !pos_disor)
			{
				pos_aux--;
				pos++;
			}
			else if(firm_info.paq_disordered==1)
			{
				pos++;
				pos_aux--;
			}
			else if(pos_disor==1)
			{
				if( pos_old <= 1)
				{
					pos=1;
					pos_aux=1;
					pos_disor=0;
				}
				else if( first_stop )
				{
					pos=pos_max;
					first_stop=0;
				}
				else pos--;
				
			}
		}
	} 
	else
	{
		if( uart==UART0 ) XBee.flush();
		else if( uart==UART1 ) XBee2.flush();
	}
	
	pos=pos_old;
	
	return error;
}


/*
 Function: Free the outcoming data buffer in the XBee
*/
uint8_t WaspXBeeCore::freeXBee()
{
    uint8_t temp=0;
    uint8_t error=2;
    if( uart==UART0 )
    {
	while(XBee.available()>0)
	{
	   temp=XBee.read();
	   error=0;
	}
    }
    else if( uart==UART1 )
    {
	while(XBee2.available()>0)
	{
	   temp=XBee2.read();
	   error=0;
	}
    }
    return error;
}


/*
 Function: Synchronizes two nodes in a PAN
*/
uint8_t WaspXBeeCore::synchronization(struct packetXBee* paq)
{
    uint8_t estado=2;
    int i=0;
    char* number = (char*) alloca(10);
    long previous2=0;
    int syncro=0;
    TIME1=millis();
    long TIME3=3000;
    long TIME2=0;
    long TIME4=0;
    long TIME5=0;
    i=Utils.long2array(TIME3,number);
    i=0;
    paq->data[i]=char(35);
    paq->data[i+1]=char(35);
    while( (number[i]) != 35 )
    {
        paq->data[i+2]=number[i];
        i++;
    }
    paq->data[i+2]=char(35);
    paq->data_length=i+3;
    estado=sendXBee(paq);
    TIME1=millis()-TIME1;
    delay(TIME3-TIME1);
    digitalWrite(XBEE_SLEEP,HIGH); // Dormimos el XBee
    free(number);
    return estado;
}

/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setDestinationParams(packetXBee* paq, uint8_t* address, const char* data, uint8_t type, uint8_t off_type)
{
	char macDest[17];
	Utils.hex2str(address, macDest);	
	return setDestinationParams(paq,macDest,data,type,off_type);
}

/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setDestinationParams(packetXBee* paq, uint8_t* address, int data, uint8_t type, uint8_t off_type)
{
	char macDest[17];
	Utils.hex2str(address, macDest);
	return setDestinationParams(paq,macDest,data,type,off_type);
}

/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setDestinationParams(packetXBee* paq, const char* address, const char* data, uint8_t type, uint8_t off_type)
{
    uint8_t* destination = (uint8_t*) calloc(8,sizeof(uint8_t));
    if( destination==NULL ) return -1;
    uint8_t i=0;
    uint8_t j=0;
    char aux[2];
	
    if( off_type==DATA_ABSOLUTE )
    {
        if( type==MAC_TYPE )
        {
            while(j<8)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                destination[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            for(uint8_t a=0;a<4;a++)
            {
                paq->macDH[a]=destination[a];
            }
            for(uint8_t b=0;b<4;b++)
            {
                paq->macDL[b]=destination[b+4];
            }
            paq->address_type=_64B;
        }
        if( type==MY_TYPE )
        {
            while(j<2)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                destination[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            paq->naD[0]=destination[0];
            paq->naD[1]=destination[1];
            paq->address_type=_16B;
        }
        data_ind=0;
    }
    while( *data!='\0' )
    {
        paq->data[data_ind]=*data++;
        data_ind++;
        if( data_ind>=MAX_DATA ) break;
    }
    paq->data_length=data_ind;
    free(destination);
    return 1;
}


/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setDestinationParams(packetXBee* paq, const char* address, int data, uint8_t type, uint8_t off_type)
{
    uint8_t* destination = (uint8_t*) calloc(8,sizeof(uint8_t));
    if( destination==NULL ) return -1;
    uint8_t i=0;
    uint8_t j=0;
    char aux[2];
    char numb[10];
	
    if( off_type==DATA_ABSOLUTE )
    {
        if( type==MAC_TYPE )
        {
            while(j<8)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                destination[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            for(uint8_t a=0;a<4;a++)
            {
                paq->macDH[a]=destination[a];
            }
            for(uint8_t a=0;a<4;a++)
            {
                paq->macDL[a]=destination[a+4];
            }
            paq->address_type=_64B;
        }
        if( type==MY_TYPE )
        {
            while(j<2)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                destination[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            paq->naD[0]=destination[0];
            paq->naD[1]=destination[1];
            paq->address_type=_16B;
        }
        data_ind=0;
    }
    i=0;
    Utils.long2array(data,numb);
    while( numb[i]!='\0' )
    {
        paq->data[data_ind]=numb[i]++;
        data_ind++;
        i++;
        if( data_ind>=MAX_DATA ) break;
    }
    paq->data_length=data_ind;
    free(destination);
    return 1;
}

/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setOriginParams(packetXBee* paq, uint8_t type)
{
	return setOriginParams(paq,"",type);
}

/* FUnction: Sets to 'paq' the destination address and data
*/
int8_t WaspXBeeCore::setOriginParams(packetXBee* paq, const char* address, uint8_t type)
{
    uint8_t* origin = (uint8_t*) calloc(8,sizeof(uint8_t));
    if( origin==NULL ) return -1;
    uint8_t i=0;
    uint8_t j=0;
    char aux[2];
	
    if( type==MAC_TYPE )
    {
        if(Utils.sizeOf(address)<5)
        {
            getOwnMac();
            for(uint8_t a=0;a<4;a++)
            {
                paq->macOH[a]=sourceMacHigh[a];
            }
            for(uint8_t b=0;b<4;b++)
            {
                paq->macOL[b]=sourceMacLow[b];
            }
        }
        else
        {
            while(j<8)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                origin[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            for(uint8_t a=0;a<4;a++)
            {
                paq->macOH[a]=origin[a];
            }
            for(uint8_t b=0;b<4;b++)
            {
                paq->macOL[b]=origin[b+4];
            }
        }
        paq->typeSourceID=MAC_TYPE;
    }
    if( type==MY_TYPE )
    {
        if(Utils.sizeOf(address)<2)
        {
            getOwnNetAddress();
            for(uint8_t a=0;a<2;a++)
            {
                paq->naO[a]=sourceNA[a];
            }
        }
        else
        {
            while(j<2)
            {
                aux[i-j*2]=address[i];
                aux[(i-j*2)+1]=address[i+1];
                origin[j]=Utils.str2hex(aux);
                i+=2;
                j++;
            }
            paq->naO[0]=origin[0];
            paq->naO[1]=origin[1];
        }
        paq->typeSourceID=MY_TYPE;
    }
    i=0;
    if( type==NI_TYPE )
    {
        while( *address!='\0' )
        {
            paq->niO[i]=*address++;
            i++;
        }
        paq->niO[i]='#';
        paq->typeSourceID=NI_TYPE;
    }
    free(origin);
    return 1;
}


/*
 Function: Treats and parses the read bytes wich are a message sent by a remote XBee
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
   error=-1 --> No more memory available
 Values: Stores in global "packet_finished" array the received message  
*/
int8_t WaspXBeeCore::readXBee(uint8_t* data)
{
    uint16_t aux=0;
    uint16_t aux2=0;
    uint8_t aux3=0;
    int8_t error=2;
    uint16_t cont3=0;
    uint8_t header=0;
    uint16_t temp=0;
    int16_t temp2=0;
    uint16_t temp3=0;
    uint8_t samePacket=0;
    uint8_t comp=0;
    uint8_t index1=0;
    uint8_t index2=0;
    long time=0;
    uint8_t finishIndex=0;
  
    it=0;	
    if( indexNotModified )
    {
        for(it=0;it<MAX_FINISH_PACKETS;it++)
        {
            if( pendingFragments[it]==NULL ) break;
        }
        nextIndex1=it;
    }
	
    it=0;
#if DEBUG
    XBee.println("");
    XBee.println("-----------------------------------------------------");
    XBee.print("Pending Packets: ");
    XBee.println(pendingPackets,DEC);
#endif
	
    temp=0;
    if( protocol!=XBEE_802_15_4 && data_length<12 ) return 1;
    if( protocol==XBEE_802_15_4 && add_type==_16B && data_length<5 ) return 1;
    if( protocol==XBEE_802_15_4 && add_type==_64B && data_length<11 ) return 1;
	
    while(temp<MAX_FINISH_PACKETS) // miramos si ya tenemos fragmentos de este paquete global
    {
#if DEBUG
        XBee.println("Posible fragmento de paquete existente");
#endif
        it=0;
        temp2=0;
        temp3=0;
        if( pendingFragments[temp]->time > 0 )
        {
            if(protocol==XBEE_802_15_4)
            {
                if(add_type==_16B)
                {
                    if( (data[4]) == pendingFragments[temp]->packetID )
                    {
                        if((data[6])==35)
                        {
                            temp2=1;
                        }
                        else
                        {
                            temp2=0;
                        }
                        temp3=data[6+temp2];
                        it=0;
                        switch(temp3)
                        {
                            case MY_TYPE:	if(pendingFragments[temp]->naO[0]==data[7+temp2])
                            {
                                if(pendingFragments[temp]->naO[1]==data[8+temp2])
                                {
                                    samePacket=1;
                                }
                            }
                            break;
                            case 1:	for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOH[it]!=data[7+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOL[it]!=data[11+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                            case 2:	while(pendingFragments[temp]->niO[it]!=35)
                            {
                                if(pendingFragments[temp]->niO[it]!=data[7+it+temp2])
                                {
                                    comp=1;
                                    break;
                                }
                                it++;
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                        }
                        if(samePacket==1)  // Fragmento del mismo paquete
                        {
                            index1=temp;
                            temp=MAX_FINISH_PACKETS;
                            if(temp2==1)
                            {
                                pendingFragments[index1]->totalFragments=data[5];
                            }
                            pendingFragments[index1]->recFragments++;
                            pendingFragments[index1]->RSSI=pendingFragments[index1]->RSSI+data[2];
                            index2=data[5]-1;
                            if( index2>= MAX_FRAG_PACKETS ){
                                freeIndexMatrix(index1);
                                return -1;
                            }
                        }
                    }
                }
                else if(add_type==_64B)
                {
                    if((data[10])==pendingFragments[temp]->packetID)
                    {
                        if((data[12])==35)
                        {
                            temp2=1;
                        }
                        else
                        {
                            temp2=0;
                        }
                        temp3=data[12+temp2];
                        it=0;
                        switch(temp3)
                        {
                            case MY_TYPE:	if(pendingFragments[temp]->naO[0]==data[13+temp2])
                            {
                                if(pendingFragments[temp]->naO[1]==data[14+temp2])
                                {
                                    samePacket=1;
                                }
                            }
                            break;
                            case 1:	for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOH[it]!=data[13+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOL[it]!=data[17+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                            case 2:	while(pendingFragments[temp]->niO[it]!=35)
                            {
                                if(pendingFragments[temp]->niO[it]!=data[13+it+temp2])
                                {
                                    comp=1;
                                    break;
                                }
                                it++;
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                        }
                        if(samePacket==1)  // Fragmento del mismo paquete
                        {
#if DEBUG
                            XBee.println("Fragmento de paquete existente");
#endif
                            index1=temp;
                            temp=MAX_FINISH_PACKETS;
                            if(temp2==1)
                            {
                                pendingFragments[index1]->totalFragments=data[11];
                            }
                            pendingFragments[index1]->recFragments++;
                            pendingFragments[index1]->RSSI=pendingFragments[index1]->RSSI+data[8];
                            index2=data[11]-1;
                            if( index2>= MAX_FRAG_PACKETS ){
                                freeIndexMatrix(index1);
                                return -1;
                            }
#if DEBUG
                            XBee.print("Index1: ");
                            XBee.println(index1,DEC);
                            XBee.print("Index2: ");
                            XBee.println(index2,DEC);
#endif
                        }
                    }
                }
            }
            else if( (protocol==DIGIMESH) || (protocol==ZIGBEE) || (protocol==XBEE_900) || (protocol==XBEE_868) )
            {
                if(mode==UNICAST)
                {
                    if((data[11])==pendingFragments[temp]->packetID)
                    {
                        if((data[13])==35)
                        {
                            temp2=1;
                        }
                        else
                        {
                            temp2=0;
                        }
                        temp3=data[13+temp2];
                        it=0;
                        switch(temp3)
                        {
                            case MY_TYPE: if(pendingFragments[temp]->naO[0]==data[14+temp2])
                            {
                                if(pendingFragments[temp]->naO[1]==data[15+temp2])
                                {
                                    samePacket=1;
                                }
                            }
                            break;
                            case 1: for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOH[it]!=data[14+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOL[it]!=data[18+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                            case 2: while(pendingFragments[temp]->niO[it]!=35)
                            {
                                if(pendingFragments[temp]->niO[it]!=data[14+it+temp2])
                                {
                                    comp=1;
                                    break;
                                }
                                it++;
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                        }
                        if(samePacket==1)  // Fragmento del mismo paquete
                        {
                            index1=temp;
#if DEBUG
                            XBee.println("Fragmento de paquete existente");
#endif
                            temp=MAX_FINISH_PACKETS;
                            if(temp2==1)
                            {
                                pendingFragments[index1]->totalFragments=data[12];
                            }
                            pendingFragments[index1]->recFragments++;
                            index2=data[12]-1;
#if DEBUG
                            XBee.print("Index1: ");
                            XBee.println(index1,DEC);
                            XBee.print("Index2: ");
                            XBee.println(index2,DEC);
#endif
                            if( index2>= MAX_FRAG_PACKETS ){
                                freeIndexMatrix(index1);
                                return -1;
                            }
                        }
                    }
                }
                else if(mode==CLUSTER)
                {
                    if((data[17])==pendingFragments[temp]->packetID)
                    {
                        if((data[19])==35)
                        {
                            temp2=1;
                        }
                        else
                        {
                            temp2=0;
                        }
                        temp3=data[19+temp2];
                        it=0;
                        switch(temp3)
                        {
                            case MY_TYPE: if(pendingFragments[temp]->naO[0]==data[20+temp2])
                            {
                                if(pendingFragments[temp]->naO[1]==data[21+temp2])
                                {
                                    samePacket=1;
                                }
                            }
                            break;
                            case 1: for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOH[it]!=data[20+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            for(it=0;it<4;it++)
                            {
                                if(pendingFragments[temp]->macOL[it]!=data[24+temp2+it])
                                {
                                    comp=1;
                                    break;
                                }
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                            case 2: while(pendingFragments[temp]->niO[it]!=35)
                            {
                                if(pendingFragments[temp]->niO[it]!=data[20+it+temp2])
                                {
                                    comp=1;
                                    break;
                                }
                                it++;
                            }
                            if(comp==0)
                            {
                                samePacket=1;
                            }
                            break;
                        }
                        if(samePacket==1)  // Fragmento del mismo paquete
                        {
                            index1=temp;
                            temp=MAX_FINISH_PACKETS;
                            if(temp2==1)
                            {
                                pendingFragments[index1]->totalFragments=data[18];
                            }
                            pendingFragments[index1]->recFragments++;
                            index2=data[18]-1;
                            if( index2>= MAX_FRAG_PACKETS ){
                                freeIndexMatrix(index1);
                                return -1;
                            }
                        }
                    }
                }
            }
        }
        temp++;
    }
    it=0;
    if(samePacket==0) // Fragmento de un paquete nuevo
    {
#if DEBUG
        XBee.println("Fragmento de paquete nuevo");
#endif
        index1=nextIndex1;
        indexNotModified=1;
        if( pendingPackets>= MAX_FINISH_PACKETS ){
            freeIndex();
            index1=nextIndex1;
        }
        pendingPackets++;
        pendingFragments[index1] = (index*) calloc(1,sizeof(index));
        if(pendingFragments[index1]==NULL){
            freeAll();
            return -1;
        }
        if(protocol==XBEE_802_15_4)
        {
            if(add_type==_16B)
            {
#if DEBUG
                XBee.println("Entro en 16B");
                XBee.println(pendingFragments[index1]->packetID,HEX);
                XBee.println(pendingFragments[index1]->time,DEC);
#endif
                pendingFragments[index1]->address_typeS=add_type;
                pendingFragments[index1]->packetID=data[4];
                pendingFragments[index1]->time=millis();
                for(it=0;it<2;it++)
                {
                    pendingFragments[index1]->naS[it]=data[it];
#if DEBUG
                    XBee.print(pendingFragments[index1]->naS[it],HEX);
#endif
                }
                if((data[6])==35)
                {
                    temp2=1;
                }
                else
                {
                    temp2=0;
                }
                pendingFragments[index1]->typeSourceID=data[6+temp2];
                it=0;
                switch(pendingFragments[index1]->typeSourceID)
                {
                    case MY_TYPE:	pendingFragments[index1]->naO[0]=data[7+temp2];
                    pendingFragments[index1]->naO[1]=data[8+temp2];
                    break;
                    case MAC_TYPE:	for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOH[it]=data[7+it+temp2];
                    }
                    for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOL[it]=data[11+it+temp2];
                    }
                    break;
                    case NI_TYPE:	while(data[7+temp2+it]!=35)
                    {
                        pendingFragments[index1]->niO[it]=char(data[7+it+temp2]);
                        it++;
                        if(it>20)
                        {
                            break;
                        }
                    }
                    pendingFragments[index1]->niO[it]=char(35);
                    break;
                }
                if(temp2==1)
                {
                    pendingFragments[index1]->totalFragments=data[5];
                }
                pendingFragments[index1]->recFragments=1;
                index2=data[5]-1;
                if( index2>= MAX_FRAG_PACKETS ){
                    freeIndexMatrix(index1);
                    return -1;
                }
                pendingFragments[index1]->RSSI=data[2];
                pendingFragments[index1]->opt=data[3];
                if( (pendingFragments[index1]->opt==0x01) || (pendingFragments[index1]->opt==0x02) )
                {
                    pendingFragments[index1]->mode=BROADCAST;
                }
                else
                {
                    pendingFragments[index1]->mode=UNICAST;
                }
            }
            else if(add_type==_64B)
            {
                pendingFragments[index1]->address_typeS=add_type;
                pendingFragments[index1]->packetID=data[10];
                pendingFragments[index1]->time=millis();
#if DEBUG
                XBee.println("Entro en 64B");
                XBee.println(pendingFragments[index1]->packetID,HEX);
                XBee.println(pendingFragments[index1]->time,DEC);
#endif
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSH[it]=data[it];
#if DEBUG
                    XBee.print(pendingFragments[index1]->macSH[it],HEX);
#endif
                }
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSL[it]=data[it+4];
#if DEBUG
                    XBee.print(pendingFragments[index1]->macSL[it],HEX);
#endif
                }
                if((data[12])==35)
                {
                    temp2=1;
                }
                else
                {
                    temp2=0;
                }
                pendingFragments[index1]->typeSourceID=data[12+temp2];
#if DEBUG
                XBee.println("");
                XBee.println(pendingFragments[index1]->typeSourceID,DEC);
#endif
                it=0;
                switch(pendingFragments[index1]->typeSourceID)
                {
                    case MY_TYPE:	pendingFragments[index1]->naO[0]=data[13+temp2];
                    pendingFragments[index1]->naO[1]=data[14+temp2];
#if DEBUG
                    XBee.println(pendingFragments[index1]->naO[0],HEX);
                    XBee.println(pendingFragments[index1]->naO[1],HEX);
#endif
                    break;
                    case MAC_TYPE:	for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOH[it]=data[13+it+temp2];
                    }
                    for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOL[it]=data[17+it+temp2];
                    }
                    break;
                    case NI_TYPE:	while(data[13+temp2+it]!=35)
                    {
                        pendingFragments[index1]->niO[it]=char(data[13+it+temp2]);
#if DEBUG
                        XBee.print(pendingFragments[index1]->niO[it],BYTE);
#endif
                        it++;
                        if(it>20)
                        {
                            break;
                        }
                    }
                    pendingFragments[index1]->niO[it]=char(35);
                    break;
                }
                pendingFragments[index1]->RSSI=data[8];
                pendingFragments[index1]->opt=data[9];
                if( (pendingFragments[index1]->opt==0x01) || (pendingFragments[index1]->opt==0x02) )
                {
                    pendingFragments[index1]->mode=BROADCAST;
                }
                else
                {
                    pendingFragments[index1]->mode=UNICAST;
                }
                if(temp2==1)
                {
                    pendingFragments[index1]->totalFragments=data[11];
                }
                pendingFragments[index1]->recFragments=1;
                index2=data[11]-1;
                if( index2>= MAX_FRAG_PACKETS ){
                    freeIndexMatrix(index1);
                    return -1;
                }
#if DEBUG
                XBee.print("Index1: ");
                XBee.println(index1,DEC);
                XBee.print("Index2: ");
                XBee.println(index2,DEC);
#endif
            }
        }
        else if( (protocol==DIGIMESH) || (protocol==ZIGBEE) || (protocol==XBEE_900) || (protocol==XBEE_868) )
        {
            if(mode==UNICAST)
            {
                pendingFragments[index1]->packetID=data[11];
                pendingFragments[index1]->time=millis();
                if((data[13])==35)
                {
                    temp2=1;
                }
                else
                {
                    temp2=0;
                }
                pendingFragments[index1]->typeSourceID=data[13+temp2];
                it=0;
                switch(pendingFragments[index1]->typeSourceID)
                {
                    case MY_TYPE: pendingFragments[index1]->naO[0]=data[14+temp2];
                    pendingFragments[index1]->naO[1]=data[15+temp2];
                    aux3=2;
                    break;
                    case MAC_TYPE: for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOH[it]=data[14+it+temp2];
                    }
                    for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOL[it]=data[18+it+temp2];
                    }
                    aux3=8;
                    break;
                    case NI_TYPE: while(data[14+temp2+it]!=35)
                    {
                        pendingFragments[index1]->niO[it]=char(data[14+it+temp2]);
                        it++;
                        aux3++;
                        if(it>20)
                        {
                            break;
                        }
                    }
                    pendingFragments[index1]->niO[it]=char(35);
                    break;
                }
                if(temp2==1)
                {
                    pendingFragments[index1]->totalFragments=data[12];
                }
                pendingFragments[index1]->recFragments=1;
                index2=data[12]-1;
                if( index2>= MAX_FRAG_PACKETS ){
                    freeIndexMatrix(index1);
                    return -1;
                }
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSH[it]=data[it];
                }
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSL[it]=data[it+4];
                }
                for(it=0;it<2;it++)
                {
                    pendingFragments[index1]->naS[it]=data[it+8];
                }
                pendingFragments[index1]->mode=UNICAST;
                pendingFragments[index1]->opt=data[10];
                if(pendingFragments[index1]->opt==0x02)
                {
                    pendingFragments[index1]->mode=BROADCAST;
                }
#if DEBUG
                XBee.print("Index1: ");
                XBee.println(index1,DEC);
                XBee.print("Index2: ");
                XBee.println(index2,DEC);
#endif
            }
            else if(mode==CLUSTER)
            {
                pendingFragments[index1]->packetID=data[17];
                pendingFragments[index1]->time=millis();
                if((data[19])==35)
                {
                    temp2=1;
                }
                else
                {
                    temp2=0;
                }
                pendingFragments[index1]->typeSourceID=data[19+temp2];
                it=0;
                switch(pendingFragments[index1]->typeSourceID)
                {
                    case MY_TYPE: pendingFragments[index1]->naO[0]=data[20+temp2];
                    pendingFragments[index1]->naO[1]=data[21+temp2];
                    aux3=2;
                    break;
                    case MAC_TYPE: for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOH[it]=data[20+it+temp2];
                    }
                    for(it=0;it<4;it++)
                    {
                        pendingFragments[index1]->macOL[it]=data[24+it+temp2];
                    }
                    aux3=8;
                    break;
                    case NI_TYPE: while(data[20+temp2+it]!=35)
                    {
                        pendingFragments[index1]->niO[it]=char(data[20+it+temp2]);
                        it++;
                        aux3++;
                        if(it>20)
                        {
                            break;
                        }
                    }
                    pendingFragments[index1]->niO[it]=char(35);
                    break;
                }
                if(temp2==1)
                {
                    pendingFragments[index1]->totalFragments=data[18];
                }
                pendingFragments[index1]->recFragments=1;
                index2=data[18]-1;
                if( index2>= MAX_FRAG_PACKETS ){
                    freeIndexMatrix(index1);
                    return -1;
                }
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSH[it]=data[it];
                }
                for(it=0;it<4;it++)
                {
                    pendingFragments[index1]->macSL[it]=data[it+4];
                }
                for(it=0;it<2;it++)
                {
                    pendingFragments[index1]->naS[it]=data[it+8];
                }
                pendingFragments[index1]->mode=CLUSTER;
                pendingFragments[index1]->SD=data[10];
                pendingFragments[index1]->DE=data[11];
                for(it=0;it<2;it++)
                {
                    pendingFragments[index1]->CID[it]=data[it+12];
                }
                for(it=0;it<2;it++)
                {
                    pendingFragments[index1]->PID[it]=data[it+14];
                }
                pendingFragments[index1]->opt=data[16];
                if(pendingFragments[index1]->opt==0x02)
                {
                    pendingFragments[index1]->mode=BROADCAST;
                }
            }
        }
    }
    samePacket=0;	
    packet_fragments[index1][index2] = (matrix*) calloc(1,sizeof(matrix));
    if(packet_fragments[index1][index2]==NULL){
        freeAll();
        return -1;
    }
    if(protocol==XBEE_802_15_4)
    {
        if(add_type==_16B)
        {
            packet_fragments[index1][index2]->numFragment=data[5];
            if(data[6]==35)
            {
                packet_fragments[index1][index2]->endFragment=1;
            }
            else
            {
                packet_fragments[index1][index2]->endFragment=0;
            }
            aux2=packet_fragments[index1][index2]->endFragment;
            it=0;
            cont3=0;
            switch(pendingFragments[index1]->typeSourceID)
            {
                case MY_TYPE:	cont3=2;
                break;
                case MAC_TYPE:	cont3=8;
                break;
                case NI_TYPE:	while(data[7+aux2+it]!=35)
                {
                    it++;
                    cont3++;
                }
                cont3++;
                break;
            }
            header=4+3+aux2+cont3;
            if( header>data_length ) data_length=header;
            packet_fragments[index1][index2]->frag_length=data_length-header;
#if DEBUG
            XBee.println("");
            XBee.print("data_length: ");
            XBee.println(data_length,DEC);
            XBee.print("header: ");
            XBee.println(header,DEC);
            XBee.print("aux2: ");
            XBee.println(aux2,DEC);
            XBee.print("cont3: ");
            XBee.println(cont3,DEC);
            XBee.println(packet_fragments[index1][index2]->frag_length,DEC);
#endif
            for(it=0;it<packet_fragments[index1][index2]->frag_length;it++)
            {     
                packet_fragments[index1][index2]->data[it]=char(data[it+header]);
#if DEBUG
                XBee.print(packet_fragments[index1][index2]->data[it],BYTE);
#endif
            }
        }
        if(add_type==_64B)
        {
            packet_fragments[index1][index2]->numFragment=data[11];
            if(data[12]==35)
            {
                packet_fragments[index1][index2]->endFragment=1;
            }
            else
            {
                packet_fragments[index1][index2]->endFragment=0;
            }
            aux2=packet_fragments[index1][index2]->endFragment;
            it=0;
            cont3=0;
            switch(pendingFragments[index1]->typeSourceID)
            {
                case MY_TYPE:	cont3=2;
                break;
                case MAC_TYPE:	cont3=8;
                break;
                case NI_TYPE:	while(data[13+aux2+it]!=35)
                {
                    it++;
                    cont3++;
                }
                cont3++;
                break;
            }
            header=10+3+aux2+cont3;
            if( header>data_length ) data_length=header;
            packet_fragments[index1][index2]->frag_length=data_length-header;
#if DEBUG
            XBee.println("");
            XBee.print("data_length: ");
            XBee.println(data_length,DEC);
            XBee.print("header: ");
            XBee.println(header,DEC);
            XBee.print("aux2: ");
            XBee.println(aux2,DEC);
            XBee.print("cont3: ");
            XBee.println(cont3,DEC);
            XBee.println(packet_fragments[index1][index2]->frag_length,DEC);
#endif
            for(it=0;it<packet_fragments[index1][index2]->frag_length;it++)
            {     
                packet_fragments[index1][index2]->data[it]=char(data[it+header]);
#if DEBUG
                XBee.print(packet_fragments[index1][index2]->data[it],BYTE);
#endif
            }
        }
    }
	
    if( (protocol==ZIGBEE) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        if(mode==UNICAST)
        {
            packet_fragments[index1][index2]->numFragment=data[12];
            if(data[13]==35)
            {
                packet_fragments[index1][index2]->endFragment=1;
            }
            else
            {
                packet_fragments[index1][index2]->endFragment=0;
            }
            aux2=packet_fragments[index1][index2]->endFragment;
            it=0;
            cont3=0;
            switch(pendingFragments[index1]->typeSourceID)
            {
                case MY_TYPE:	cont3=2;
                break;
                case MAC_TYPE:	cont3=8;
                break;
                case NI_TYPE:	while(data[14+aux2+it]!=35)
                {
                    it++;
                    cont3++;
                }
                cont3++;
                break;
            }
            header=11+3+aux2+cont3;
            if( header>data_length ) data_length=header;
            packet_fragments[index1][index2]->frag_length=data_length-header;
#if DEBUG
            XBee.println("");
            XBee.print("data_length: ");
            XBee.println(data_length,DEC);
            XBee.print("header: ");
            XBee.println(header,DEC);
            XBee.print("aux2: ");
            XBee.println(aux2,DEC);
            XBee.print("cont3: ");
            XBee.println(cont3,DEC);
            XBee.println(packet_fragments[index1][index2]->frag_length,DEC);
#endif
            for(it=0;it<packet_fragments[index1][index2]->frag_length;it++)
            {     
                packet_fragments[index1][index2]->data[it]=char(data[it+header]);
            }
        }
        else if(mode==CLUSTER)
        {
            packet_fragments[index1][index2]->numFragment=data[18];
            if(data[19]==35)
            {
                packet_fragments[index1][index2]->endFragment=1;
            }
            else
            {
                packet_fragments[index1][index2]->endFragment=0;
            }
            aux2=packet_fragments[index1][index2]->endFragment;
            it=0;
            cont3=0;
            switch(pendingFragments[index1]->typeSourceID)
            {
                case MY_TYPE:	cont3=2;
                break;
                case MAC_TYPE:	cont3=8;
                break;
                case NI_TYPE:	while(data[23+aux2+it]!=35)
                {
                    it++;
                    cont3++;
                }
                cont3++;
                break;
            }
            header=17+3+aux2+cont3;
            if( header>data_length ) data_length=header;
            packet_fragments[index1][index2]->frag_length=data_length-header;
            for(it=0;it<packet_fragments[index1][index2]->frag_length;it++)
            {     
                packet_fragments[index1][index2]->data[it]=char(data[it+header]);
            }
        }
    }
    totalFragmentsReceived++;
	
    if(totalFragmentsReceived==MAX_FRAG_PACKETS)
    {
        totalFragmentsReceived=0;
    }
	
    if(pendingFragments[index1]->recFragments==pendingFragments[index1]->totalFragments)
    {
        pendingFragments[index1]->complete=1;
    }

    temp=0;
    temp2=0;
    temp3=0;
    aux=0;
    while(temp<MAX_FINISH_PACKETS)
    {
        it=0;
        time=millis();
        if( (pendingFragments[temp]!=NULL) )
        {
            if ( (pendingFragments[temp]->time>0) )
            {
                if( ((time-pendingFragments[temp]->time)<=TIMEOUT) )
                {
                    if ((pendingFragments[temp]->time>0) ) // se mira el contenido
                    {
                        if(pendingFragments[temp]->complete==1)
                        {
                            nextIndex1=temp;
                            indexNotModified=0;
                            pos++;
                            finishIndex=getFinishIndex();
                            if( pos>=MAX_FINISH_PACKETS ){
                                switch( replacementPolicy )
                                {
                                    case	XBEE_FIFO:	finishIndex=getIndexFIFO();
                                    break;
                                    case	XBEE_LIFO:	finishIndex=getIndexLIFO();
                                    break;
                                    case	XBEE_OUT:	freeIndexMatrix(temp);
                                    return -1;
                                    break;
                                }
                            }
                            packet_finished[finishIndex] = (packetXBee*) calloc(1,sizeof(packetXBee));
                            if(packet_finished[finishIndex]==NULL){
                                freeIndexMatrix(temp);
                                return -1;
                            }
                            else pendingPackets--;
                            packet_finished[finishIndex]->time=pendingFragments[temp]->time;
                            packet_finished[finishIndex]->packetID=pendingFragments[temp]->packetID;
                            packet_finished[finishIndex]->address_typeS=pendingFragments[temp]->address_typeS;
                            packet_finished[finishIndex]->mode=pendingFragments[temp]->mode;
                            if(protocol==XBEE_802_15_4)
                            {
                                if(packet_finished[finishIndex]->address_typeS==_64B)
                                {
                                    for(it=0;it<4;it++)
                                    {
                                        packet_finished[finishIndex]->macSH[it]=pendingFragments[temp]->macSH[it];
                                    }
                                    for(it=0;it<4;it++)
                                    {
                                        packet_finished[finishIndex]->macSL[it]=pendingFragments[temp]->macSL[it];
                                    }
                                }
                                else if(packet_finished[finishIndex]->address_typeS==_16B)
                                {
                                    packet_finished[finishIndex]->naS[0]=pendingFragments[temp]->naS[0];
                                    packet_finished[finishIndex]->naS[1]=pendingFragments[temp]->naS[1];
                                }
                            }
                            else
                            {
                                for(it=0;it<4;it++)
                                {
                                    packet_finished[finishIndex]->macSH[it]=pendingFragments[temp]->macSH[it];
                                }
                                for(it=0;it<4;it++)
                                {
                                    packet_finished[finishIndex]->macSL[it]=pendingFragments[temp]->macSL[it];
                                }
                                packet_finished[finishIndex]->naS[0]=pendingFragments[temp]->naS[0];
                                packet_finished[finishIndex]->naS[1]=pendingFragments[temp]->naS[1];
                            }
                            aux=(pendingFragments[temp]->RSSI)/(pendingFragments[temp]->totalFragments);
                            packet_finished[finishIndex]->RSSI=aux;
                            packet_finished[finishIndex]->typeSourceID=pendingFragments[temp]->typeSourceID;
                            switch(packet_finished[finishIndex]->typeSourceID)
                            {
                                case MY_TYPE:	packet_finished[finishIndex]->naO[0]=pendingFragments[temp]->naO[0];
                                packet_finished[finishIndex]->naO[1]=pendingFragments[temp]->naO[1];
                                break;
                                case MAC_TYPE:	for(it=0;it<4;it++)
                                {
                                    packet_finished[finishIndex]->macOH[it]=pendingFragments[temp]->macOH[it];
                                }
                                for(it=0;it<4;it++)
                                {
                                    packet_finished[finishIndex]->macOL[it]=pendingFragments[temp]->macOL[it];
                                }
                                break;
                                case NI_TYPE:	it=0;
                                do
                                {
                                    packet_finished[finishIndex]->niO[it]=pendingFragments[temp]->niO[it];
                                    it++;
                                } while(pendingFragments[temp]->niO[it]!=35);
                                packet_finished[finishIndex]->niO[it]=char(35);
                                break;
                            }
                            temp2=(pendingFragments[temp]->totalFragments)-1;
                            aux=0;
                            temp3=0;
                            while(temp2>=0)
                            {
                                for(it=0;it<packet_fragments[temp][temp2]->frag_length;it++)
                                {
                                    packet_finished[finishIndex]->data[it+temp3]=packet_fragments[temp][temp2]->data[it];
                                    aux++;
                                }
                                temp3=packet_fragments[temp][temp2]->frag_length+temp3;
                                temp2--;
                            }
                            packet_finished[finishIndex]->data_length=aux;
                            if(mode==CLUSTER)
                            {
                                packet_finished[finishIndex]->SD=pendingFragments[temp]->SD;
                                packet_finished[finishIndex]->DE=pendingFragments[temp]->DE;
                                packet_finished[finishIndex]->CID[0]=pendingFragments[temp]->CID[0];
                                packet_finished[finishIndex]->CID[1]=pendingFragments[temp]->CID[1];
                                packet_finished[finishIndex]->PID[0]=pendingFragments[temp]->PID[0];
                                packet_finished[finishIndex]->PID[1]=pendingFragments[temp]->PID[1];
                            }
						// Vaciamos el index
                            for(it=0;it<MAX_FRAG_PACKETS;it++)
                            {
                                free(packet_fragments[temp][it]);
                                packet_fragments[temp][it]=NULL;
                            }
						
                            pendingFragments[temp]->time=0;
                            free(pendingFragments[temp]);
                            pendingFragments[temp]=NULL;
                        }
                        else if(pendingFragments[temp]->totalFragments==pendingFragments[temp]->recFragments)
                        {
                            for(it=0;it<MAX_FRAG_PACKETS;it++)
                            {
							
                                free(packet_fragments[temp][it]);
                                packet_fragments[temp][it]=NULL;
                            }
						
                            pendingFragments[temp]->time=0;
                            free(pendingFragments[temp]);
                            pendingFragments[temp]=NULL;
                            pendingPackets--;
                            nextIndex1=temp;
                            indexNotModified=0;
                        }
                    }
                }
                else // se borra la fila de la matriz y el correspondiente al vector indexador
                {
                    for(it=0;it<MAX_FRAG_PACKETS;it++)
                    {
						
                        free(packet_fragments[temp][it]);
                        packet_fragments[temp][it]=NULL;
                    }
                    pendingFragments[temp]->time=0;
                    free(pendingFragments[temp]);
                    pendingFragments[temp]=NULL;
                    pendingPackets--;
                    nextIndex1=temp;
                    indexNotModified=0;
                }
            }
        }
        temp++;
    }  
    temp=0; 
    return 0;
}


/*
 Function: Generates the API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 	param : The param to set
 Returns: Nothing
 Values: Stores in 'command' variable the API frame to send to the XBee module
*/
void WaspXBeeCore::gen_data(const char* data, uint8_t param)
{
    uint8_t inc=0;
    uint8_t inc2=0;
	
    clearCommand();
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    while(inc2<inc){
        command[inc2]=Utils.converter(data[2*inc2],data[2*inc2+1]);
        inc2++;
    }
	
    command[inc-2]=param;
}


/*
 Function: Generates the API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 Returns: Nothing
 Values: Stores in 'command' variable the API frame to send to the XBee module
*/
void WaspXBeeCore::gen_data(const char* data)
{
    uint8_t inc=0;
    uint8_t inc2=0;
	
    clearCommand();
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    while(inc2<inc){
        command[inc2]=Utils.converter(data[2*inc2],data[2*inc2+1]);
        inc2++;
    }
}


/*
 Function: Generates the API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 	param1 : The param to set
 	param2 : The param to set
 Returns: Nothing
 Values: Stores in 'command' variable the API frame to send to the XBee module
*/
void WaspXBeeCore::gen_data(const char* data, uint8_t param1, uint8_t param2)
{
    uint8_t inc=0;
    uint8_t inc2=0;
	
    clearCommand();
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    while(inc2<inc){
        command[inc2]=Utils.converter(data[2*inc2],data[2*inc2+1]);
        inc2++;
    }
	
    command[inc-3]=param1;
    command[inc-2]=param2;
}


/*
 Function: Generates the API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 	param : The param to set
 Returns: Nothing
 Values: Stores in 'command' variable the API frame to send to the XBee module
*/
void WaspXBeeCore::gen_data(const char* data, uint8_t* param)
{
    uint8_t inc=0;
    uint8_t inc2=0;
		
    clearCommand();
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    while(inc2<inc){
        command[inc2]=Utils.converter(data[2*inc2],data[2*inc2+1]);
        inc2++;
    }
	
    if(inc==24) 
    {
        for(it=0;it<16;it++)
        {
            command[inc-17+it]=param[it];
        }
    }
    else if(inc==16) 
    {
        for(it=0;it<8;it++)
        {
            command[inc-9+it]=param[it];
        }
    }
    else if(inc==11)
    {
        for(it=0;it<3;it++)
        {
            command[inc-4+it]=param[it];
        }
    }
    else if(inc==10)
    {
        for(it=0;it<2;it++)
        {
            command[inc-3+it]=param[it];
        }
    }
    else command[inc-2]=param[0];
}


/*
 Function: Generates the API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 	param : The param to set
 Returns: Nothing
 Values: Stores in 'command' variable the API frame to send to the XBee module
*/
void WaspXBeeCore::gen_data(const char* data, const char* param)
{
    gen_data(data,(uint8_t*) param);
}


/*
 Function: Generates the checksum API frame to send to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 Returns: Nothing
 Values: Stores in 'command' variable the checksum API frame to send to the XBee module
*/
uint8_t WaspXBeeCore::gen_checksum(const char* data)
{
    uint8_t inc=0;
    uint8_t checksum=0;
	
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    for(it=3;it<inc;it++)
    {
        checksum=checksum+command[it];
    }
    while( (checksum>255))
    {
        checksum=checksum-256;
    }
    checksum=255-checksum;
    command[inc-1]=checksum;
	
    return checksum;
}


/*
 Function: Sends the API frame stored in 'command' variable to the XBee module
 Parameters:
 	data : The string that contains part of the API frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::gen_send(const char* data)
{
    uint8_t inc=0;
    uint8_t inc2=0;
    int8_t error_int=2;
	
    it=0;
    while(data[it] != '\0') {
        inc++;
        it++;
    }
    inc/=2;
	
    while(inc2<inc)
    {
	if( uart==UART0 ) XBee.print(command[inc2], BYTE); 
	else if( uart==UART1 )
	{
		Utils.setMuxGPRS();
		XBee2.print(command[inc2], BYTE); 
	}
        inc2++;
    }
    inc2=0;

    error_int=parse_message(command);

    return error_int;
}


/*
 Function: Generates the API frame when a TX is done
 Parameters:
 	_packet : the packetXBee structure where the data to send is stored
 	TX_array : the array where the API frame is stored
 	start_pos : starting position in API frame
 Returns: Nothing
*/
void WaspXBeeCore::gen_frame(struct packetXBee* _packet, uint8_t* TX_array, uint8_t start_pos)
{
    uint16_t counter1=0;
	
    if(_packet->endFragment==1)
    {
        TX_array[start_pos]=0x23;
        switch(_packet->typeSourceID)
        {
            case MY_TYPE:  	TX_array[start_pos+1]=_packet->typeSourceID;
            TX_array[start_pos+2]=_packet->naO[0];
            TX_array[start_pos+3]=_packet->naO[1];
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+4]=uint8_t(_packet->data[it+start]);
            }
            break;
            case MAC_TYPE: 	TX_array[start_pos+1]=_packet->typeSourceID;
            for(it=0;it<4;it++)
            {
                TX_array[start_pos+2+it]=_packet->macOH[it];
            }
            for(it=0;it<4;it++)
            {
                TX_array[start_pos+6+it]=_packet->macOL[it];
            }
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+10]=uint8_t(_packet->data[it+start]);
            }
            break;
            case NI_TYPE:   TX_array[start_pos+1]=_packet->typeSourceID;
            while(_packet->niO[it]!='#'){
                counter1++;
                it++;
            }
            counter1++;
            for(it=0;it<counter1;it++)
            {
                TX_array[start_pos+2+it]=uint8_t(_packet->niO[it]);
            }
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+2+counter1]=uint8_t(_packet->data[it+start]);
            }
            break;
            default:    	break;
        }
    }
    else
    {
        switch(_packet->typeSourceID)
        {
            case MY_TYPE: 	TX_array[start_pos]=_packet->typeSourceID;
            TX_array[start_pos+1]=_packet->naO[0];
            TX_array[start_pos+2]=_packet->naO[1];
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+3]=uint8_t(_packet->data[it+start]);
            }
            break;
            case MAC_TYPE:  TX_array[start_pos]=_packet->typeSourceID;
            for(it=0;it<4;it++)
            {
                TX_array[start_pos+1+it]=_packet->macOH[it];
            }
            for(it=0;it<4;it++)
            {
                TX_array[start_pos+5+it]=_packet->macOL[it];
            }
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+9]=uint8_t(_packet->data[it+start]);
            }
            break;
            case NI_TYPE:   TX_array[start_pos]=_packet->typeSourceID;
            while(_packet->niO[it]!='#'){
                counter1++;
                it++;
            }
            counter1++;
            for(it=0;it<counter1;it++)
            {
                TX_array[start_pos+1+it]=uint8_t(_packet->niO[it]);
            }
            for(it=0;it<(finish-start+1);it++) // data
            {
                TX_array[it+start_pos+1+counter1]=uint8_t(_packet->data[it+start]);
            }
            break;
            default:   	break;
        }
    }
}


/*
 Function: Generates the eschaped API frame when a TX is done
 Parameters:
 	_packet : the packetXBee structure where the data to send is stored
 	TX_array : the array where the API frame is stored
 	protect : specifies the number of chars that had been eschaped
 	type : specifies the type of send
 Returns: Nothing
*/
void WaspXBeeCore::gen_frame_ap2(struct packetXBee* _packet, uint8_t* TX_array, uint8_t &protect, uint8_t type)
{
    uint8_t a=1;
    uint8_t final=0;
    uint8_t unico=0;
    uint16_t aux=0;
    uint16_t aux2=0;
	
    while(a<(_packet->frag_length+type+protect))
    {
        if( (TX_array[a]==17) && (unico==0) )
        {
            TX_array[a]=49;
            protect++;
            aux=TX_array[a];
            TX_array[a]=125;
            uint16_t l=a-1;
            while(final==0)
            {
                aux2=TX_array[l+2];
                TX_array[l+2]=aux;
                if( ((l+3)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                aux=TX_array[l+3];
                TX_array[l+3]=aux2;
                if( ((l+4)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                l++;
                l++;
            }
            final=0;
            unico=1;
        }
        if( (TX_array[a]==19) && (unico==0) )
        {
            TX_array[a]=51;
            protect++;
            aux=TX_array[a];
            TX_array[a]=125;
            uint16_t l=a-1;
            while(final==0)
            {
                aux2=TX_array[l+2];
                TX_array[l+2]=aux;
                if( ((l+3)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                aux=TX_array[l+3];
                TX_array[l+3]=aux2;
                if( ((l+4)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                l++;
                l++;
            }
            final=0;  
            unico=1;      
        }
        if( (TX_array[a]==126) && (unico==0) )
        {
            TX_array[a]=94;
            protect++;
            aux=TX_array[a];
            TX_array[a]=125;
            uint16_t l=a-1;
            while(final==0)
            {
                aux2=TX_array[l+2];
                TX_array[l+2]=aux;
                if( ((l+3)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                aux=TX_array[l+3];
                TX_array[l+3]=aux2;
                if( ((l+4)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                l++;
                l++;
            }
            final=0;
            unico=1;      
        }
        if( (TX_array[a]==125) && (unico==0) )
        {
            TX_array[a]=93;
            protect++;
            aux=TX_array[a];
            TX_array[a]=125;
            uint16_t l=a-1;
            while(final==0)
            {
                aux2=TX_array[l+2];
                TX_array[l+2]=aux;
                if( ((l+3)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                aux=TX_array[l+3];
                TX_array[l+3]=aux2;
                if( ((l+4)>=(_packet->frag_length+type+protect)) )
                {
                    final=1;
                    break;
                }
                l++;
                l++;
            }
            final=0;  
            unico=1;      
        }
        a++;
        unico=0;
    }
}


/*
 Function: Parses the answer received by the XBee module, calling the appropriate function
 Parameters:
 	frame : an array that contains the API frame that is expected to receive answer from if it is an AT command
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::parse_message(uint8_t* frame)
{
    uint8_t* memory = (uint8_t*) calloc(MAX_PARSE,sizeof(uint8_t));
    if( memory==NULL )
    {
	if( uart==UART0 ) XBee.flush();
	else if( uart==UART1 ) XBee2.flush();			
	return -1;
    }
    uint16_t i=0;
    long previous=millis();
    long previous2=millis();
    uint8_t num_mes=0;
    uint8_t num_esc=0;
    uint16_t num_data=0;
    uint16_t length_mes=0;
    uint16_t length_prev=0;
    int8_t error=2;
    long interval=50;
    long intervalMAX=40000;
    uint8_t good_frame=0;
    uint8_t maxFrame=30;
	
    // If a frame was truncated before, we set the first byte
    if( frameNext ){
        frameNext=0;
        memory[0]=0x7E;
        i=1;
        num_mes=1;
    }
	
    // If it is a TX we have a different behaviour
    if( frame[0]==0xFF ){
        error_TX=txStatusResponse(memory);
        free(memory);
        memory=NULL;
        return error_TX; 
    }
    else if( frame[0]==0xFE ){
        error_TX=txZBStatusResponse(memory);
        free(memory);
        memory=NULL;
        return error_TX; 
    }
	
    // If a RX we reduce the interval
    if( frame[0]==0xEE ){
        interval=5;
        maxFrame=109;
    }
	
    // Check if a ED is performed
    if( frame[5]==0x45 && frame[6]==0x44 && protocol==XBEE_802_15_4 ) interval=3000;
	
    // Check if a DN is performed
    if( frame[5]==0x44 && frame[6]==0x4E ) interval=1000;
		
    // Check if a ND is performed
    if( frame[5]==0x4E && frame[6]==0x44 ){
        interval=20000;
        if(protocol==DIGIMESH) interval=40000;
        else if( (protocol==XBEE_900) || (protocol==XBEE_868) )
        {
            interval=14000;
        }
    }
	
    // Read data from XBee meanwhile data is available
    previous2=millis();
    previous=millis();

    while( ((millis()-previous)<interval) && ((millis()-previous2)<intervalMAX) && i<MAX_PARSE && !frameNext )
    {	
	if( uart==UART0 )
	{
	   if(XBee.available())
	   {
		memory[i]=XBee.read();
		i++;
		if(memory[i-1]==0x7E)
		{
			if( (MAX_PARSE-i) < maxFrame ) frameNext=1;
			else num_mes++;
		}
		previous=millis();
	   }
	}
	else if( uart==UART1 )
	{
	   if(XBee2.available())
	   {
		memory[i]=XBee2.read();	
		i++;
		if(memory[i-1]==0x7E)
		{
			if( (MAX_PARSE-i) < maxFrame ) frameNext=1;
			else num_mes++;
		}
		previous=millis();
	   }
	}
	if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
        if( millis()-previous2 < 0 ) previous2=millis(); //avoid millis overflow problem
    }
		
    num_data=i;
    i=1;
	
	// If some corrupted frame has appeared we jump it
    if( memory[0]!=0x7E ) num_mes++;
	
	// Parse the received messages from the XBee
    while( num_mes>0 )
    {
        while( memory[i]!=0x7E && i<num_data ) i++;
        length_mes=i-length_prev;
		
		// If some char has been eschaped, it must be converted before parsing it
        for( it=0;it<length_mes;it++ )
        {
            if( memory[it+length_prev]==0x7D ) num_esc++;
        }
        if( num_esc ) des_esc(memory,length_mes,i-length_mes);
		
        switch( memory[(i-length_mes)+3] )
        {
            case 0x88 :	error=atCommandResponse(memory,frame,length_mes-num_esc+length_prev,i-length_mes);
            error_AT=error;
            break;
            case 0x8A :	error=modemStatusResponse(memory,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x80 :	error=rxData(memory,length_mes-num_esc+length_prev,i-length_mes);
            error_RX=error;
            break;
            case 0x81 :	error=rxData(memory,length_mes-num_esc+length_prev,i-length_mes);
            error_RX=error;
            break;
            case 0x90 :	error=rxData(memory,length_mes-num_esc+length_prev,i-length_mes);
            error_RX=error;
            break;
            case 0x91 :	error=rxData(memory,length_mes-num_esc+length_prev,i-length_mes);
            error_RX=error;
            break;
            default   :	break;
        }
		
        num_mes--;
        length_prev=i;
        i++;
        num_esc=0;
        if(!error) good_frame++;
    }
		
    free(memory);
    memory=NULL;

    if(good_frame) return 0;
    else return error;
}


/*
 Function: Generates the correct API frame from an eschaped one
 Parameters:
 	data_in : The string that contains the eschaped API frame
 	end : the end of the frame
 	start : the start of the frame
 Returns: Nothing
*/
void WaspXBeeCore::des_esc(uint8_t* data_in, uint16_t end, uint16_t start)
{
    uint16_t i=0;
    uint16_t aux=0;
		
    while( i<end )
    {
        while( data_in[start+i]!=0x7D && i<end ) i++;
        if( i<end )
        {
            aux=i+1;
            switch( data_in[start+i+1] )
            {
                case 0x31 : 	data_in[start+i]=0x11;
                break;
                case 0x33 : 	data_in[start+i]=0x13;
                break;
                case 0x5E : 	data_in[start+i]=0x7E;
                break;
                case 0x5D : 	data_in[start+i]=0x7D;
                break;
            }
            i++;
            end--;
            while( i<(end) ){
                data_in[start+i]=data_in[start+i+1];
                i++;
            }
            i=aux;
        }
    }
}


/*
 Function: Parses the AT command answer received by the XBee module
 Parameters:
 	data_in : the answer received by the module
 	frame : an array that contains the API frame that is expected to receive answer from if it is an AT command
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::atCommandResponse(uint8_t* data_in, uint8_t* frame, uint16_t end, uint16_t start)
{	
	// Check the checksum
    if(checkChecksum(data_in,end,start)) return 1;
		
	// Check the AT Command Response is from the command expected
    if( data_in[start+5]!=frame[5] || data_in[start+6]!=frame[6] ) return 1;
		
	// Check if there is data in the AT Command Response frame
    if( (end-start)==9 ){
        if( data_in[start+7]==0 ) return 0;
        else return 1;
    }
		
    if( data_in[start+7]!=0 ) return 1;
	// Store the data in the response frame
    for(it=0;it<(end-start-9);it++)
    {
        data[it]=data_in[8+it+start];
    }
	
	// Check if a ND is performed
    data_length=end-start-9;
    if( frame[5]==0x4E && frame[6]==0x44 ){
        if( data_length>1 ) totalScannedBrothers++;
        treatScan();
    }
    return 0;
}


/*
 Function: Parses the Modem Status message received by the XBee module
 Parameters:
 	data_in : the answer received by the module
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::modemStatusResponse(uint8_t* data_in, uint16_t end, uint16_t start)
{		
	// Check the checksum
    if(checkChecksum(data_in,end,start)) return 1;	
		
    modem_status=data_in[start+4];
    return 0;
}


/*
 Function: Parses the TX Status message received by the XBee module
 Parameters:
 	data_in : the answer received by the module
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::txStatusResponse(uint8_t* ByteIN)
{
    long previous=millis();
    uint16_t numberBytes=7;
    uint8_t end=0;
    uint16_t counter3=0;
    uint8_t undesired=0;
    uint8_t status=0;
    uint16_t num_TX=0;
    uint8_t num_esc=0;
    int16_t interval=2000;
    uint8_t num_mes=0;
    uint16_t i=1;
    uint16_t length_mes=0;
    uint16_t length_prev=0;
    uint8_t maxFrame=110;
	
    error_TX=2;
		
    if( frameNext )
    {
        ByteIN[0]=0x7E;
        counter3=1;
        num_mes=1;
        frameNext=0;
    }
	
    while( end==0 && !frameNext )
    {
	if( uart==UART0 )
	{
           	if(XBee.available()>0)
           	{
           		ByteIN[counter3]=XBee.read();
           		counter3++;
           		previous=millis();
           		if(ByteIN[counter3-1]==0x7E)
			{
           			if( (MAX_PARSE-counter3) < maxFrame ) frameNext=1;
                		else num_mes++;
            		}
            		if( (counter3==1) && (ByteIN[counter3-1]!=0x7E) ) counter3=0;	
            		if( counter3>=MAX_PARSE ) end=1;
            		if( (counter3==4+status*6+undesired) && (undesired!=1) ) //FIXME
            		{
                		if( (ByteIN[counter3-1]!= 0x89) && (ByteIN[counter3-1]!=0x8A) )
				{
	                    		undesired=1;
                    			numberBytes+=3;
                		}
            		}
            		if( undesired==1 ) numberBytes++;
	            	if( (ByteIN[counter3-1]==0x7D) && (!undesired) )
        	    	{
        	        	numberBytes++;
        	    	}
        	    	if( (ByteIN[counter3-1]==0x8A) && (counter3==(4+status*6)) )
        	    	{
        	        	numberBytes+=6;
        	       		status++;
        	    	}
        	    	if( (ByteIN[counter3-1]==0x7E) && (undesired==1) )
        	    	{
        	        	numberBytes--;
        	        	undesired=numberBytes-7;
        	    	}
        	    	if(counter3==numberBytes)
        	    	{
        	       		end=1;
        		}
        	}
        	if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
        	if( (millis()-previous) > interval )
        	{
            		end=1;
            		XBee.flush();
        	}
	}
	else if( uart==UART1 )
	{
		if(XBee2.available()>0)
		{
			ByteIN[counter3]=XBee2.read();
			counter3++;
			previous=millis();
			if(ByteIN[counter3-1]==0x7E){
				if( (MAX_PARSE-counter3) < maxFrame ) frameNext=1;
				else num_mes++;
			}
			if( (counter3==1) && (ByteIN[counter3-1]!=0x7E) ) counter3=0;	
			if( counter3>=MAX_PARSE ) end=1;
			if( (counter3==4+status*6+undesired) && (undesired!=1) ) //FIXME
			{
				if( (ByteIN[counter3-1]!= 0x89) && (ByteIN[counter3-1]!=0x8A) ){
					undesired=1;
					numberBytes+=3;
				}
			}
			if( undesired==1 ) numberBytes++;
			if( (ByteIN[counter3-1]==0x7D) && (!undesired) )
			{
				numberBytes++;
			}
			if( (ByteIN[counter3-1]==0x8A) && (counter3==(4+status*6)) )
			{
				numberBytes+=6;
				status++;
			}
			if( (ByteIN[counter3-1]==0x7E) && (undesired==1) )
			{
				numberBytes--;
				undesired=numberBytes-7;
			}
			if(counter3==numberBytes)
			{
				end=1;
			}
		}
		if( millis()-previous < 0 ) previous=millis();//avoid millis overflow problem
		if( (millis()-previous) > interval )
		{
			end=1;
			XBee2.flush();
		}
	} 
    }
    num_TX=counter3;
    counter3=0;
	
    // If some corrupted frame has appeared we jump it
    if( ByteIN[0]!=0x7E ) num_mes++;
	
    // Parse the received messages from the XBee
    while( num_mes>0 )
    {
        while( ByteIN[i]!=0x7E && i<num_TX ) i++;
        length_mes=i-length_prev;
		
	// If some char has been eschaped, it must be converted before parsing it
        for( it=0;it<length_mes;it++)
        {
            if( ByteIN[it+length_prev]==0x7D ) num_esc++;
        }
        if( num_esc ) des_esc(ByteIN,length_mes,i-length_mes);
		
        switch( ByteIN[(i-length_mes)+3] )
        {
            case 0x8A :	modemStatusResponse(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x80 :	error_RX=rxData(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x81 :	error_RX=rxData(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x89 :	delivery_status=ByteIN[i-length_mes+5];
            if( delivery_status==0 ) error_TX=0;
            else error_TX=1;
            break;
            default   :	break;
        }
		
        num_mes--;
        length_prev=i;
        i++;
        num_esc=0;
    }
	
    return error_TX;
}

/*
 Function: Parses the ZB TX Status message received by the XBee module
 Parameters:
 	data_in : the answer received by the module
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::txZBStatusResponse(uint8_t* ByteIN)
{	
    long previous=millis();
    uint16_t numberBytes=11;
    uint8_t end=0;
    uint16_t counter3=0;
    uint8_t undesired=0;
    uint8_t status=0;
    uint16_t num_TX=0;
    uint8_t num_esc=0;
    int16_t interval=2000;
    uint8_t num_mes=0;
    uint16_t i=1;
    uint16_t length_mes=0;
    uint16_t length_prev=0;
    uint8_t maxFrame=110;
	
    error_TX=2;
		
    if( frameNext )
    {
        ByteIN[0]=0x7E;
        counter3=1;
        num_mes=1;
        frameNext=0;
    }
	
    while( end==0 && !frameNext )
    {		
	if( uart==UART0 )
	{
		if(XBee.available()>0)
       		{
       			ByteIN[counter3]=XBee.read();
            		counter3++;
            		previous=millis();
            		if(ByteIN[counter3-1]==0x7E)
			{
                		if( (MAX_PARSE-counter3) < maxFrame ) frameNext=1;
                		else num_mes++;
            		}
            		if( (counter3==1) && (ByteIN[counter3-1]!=0x7E) ) counter3=0;	
           		if( counter3>=MAX_PARSE ) end=1;
           		if( (counter3==4+status*6+undesired) && (undesired!=1) ) //FIXME
            		{
                		if( (ByteIN[counter3-1]!= 0x8B) && (ByteIN[counter3-1]!=0x8A) )
				{
                    			undesired=1;
                    			numberBytes+=3;
                		}
            		}
            		if( undesired==1 ) numberBytes++;
            		if( (ByteIN[counter3-1]==0x7D) && (!undesired) )
            		{
                		numberBytes++;
            		}
            		if( (ByteIN[counter3-1]==0x8A) && (counter3==(4+status*6)) )
            		{
                		numberBytes+=6;
                		status++;
            		}
            		if( (ByteIN[counter3-1]==0x7E) && (undesired==1) )
            		{
                		numberBytes--;
                		undesired=numberBytes-7;
            		}
            		if(counter3==numberBytes)
            		{
                		end=1;
           		}
       		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
        	if( (millis()-previous) > interval )
       		{
            		end=1;
            		XBee.flush();
        	} 
    	}
	else if( uart==UART1 )
	{
		if(XBee2.available()>0)
		{
			ByteIN[counter3]=XBee2.read();
			counter3++;
			previous=millis();
			if(ByteIN[counter3-1]==0x7E)
			{
				if( (MAX_PARSE-counter3) < maxFrame ) frameNext=1;
				else num_mes++;
			}
			if( (counter3==1) && (ByteIN[counter3-1]!=0x7E) ) counter3=0;	
			if( counter3>=MAX_PARSE ) end=1;
			if( (counter3==4+status*6+undesired) && (undesired!=1) ) //FIXME
			{
				if( (ByteIN[counter3-1]!= 0x8B) && (ByteIN[counter3-1]!=0x8A) )
				{
					undesired=1;
					numberBytes+=3;
				}
			}
			if( undesired==1 ) numberBytes++;
			if( (ByteIN[counter3-1]==0x7D) && (!undesired) )
			{
				numberBytes++;
			}
			if( (ByteIN[counter3-1]==0x8A) && (counter3==(4+status*6)) )
			{
				numberBytes+=6;
				status++;
			}
			if( (ByteIN[counter3-1]==0x7E) && (undesired==1) )
			{
				numberBytes--;
				undesired=numberBytes-7;
			}
			if(counter3==numberBytes)
			{
				end=1;
			}
		}
		if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
		if( (millis()-previous) > interval )
		{
			end=1;
			XBee2.flush();
		} 
	}
    }
    num_TX=counter3;
    counter3=0;
	
    // If some corrupted frame has appeared we jump it
    if( ByteIN[0]!=0x7E ) num_mes++;
	
    // Parse the received messages from the XBee
    while( num_mes>0 )
    {
        while( ByteIN[i]!=0x7E && i<num_TX ) i++;
        length_mes=i-length_prev;
		
	// If some char has been eschaped, it must be converted before parsing it
        for( it=0;it<length_mes;it++)
        {
            if( ByteIN[it+length_prev]==0x7D ) num_esc++;
        }
        if( num_esc ) des_esc(ByteIN,length_mes,i-length_mes);
		
        switch( ByteIN[(i-length_mes)+3] )
        {
            case 0x8A :	modemStatusResponse(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x90 :	error_RX=rxData(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x91 :	error_RX=rxData(ByteIN,length_mes-num_esc+length_prev,i-length_mes);
            break;
            case 0x8B :	true_naD[0]=ByteIN[i-length_mes+5];
            true_naD[1]=ByteIN[i-length_mes+6];
            retries_sending=ByteIN[i-length_mes+7];
            discovery_status=ByteIN[i-length_mes+9];
            delivery_status=ByteIN[i-length_mes+8];
            if( delivery_status==0 ) error_TX=0;
            else error_TX=1;
            break;
            default   :	break;
        }
		
        num_mes--;
        length_prev=i;
        i++;
        num_esc=0;
    }

    return error_TX;
}

/*
 Function: Parses the RX Data message received by the XBee module
 Parameters:
 	data_in : the answer received by the module
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
int8_t WaspXBeeCore::rxData(uint8_t* data_in, uint16_t end, uint16_t start)
{
    uint8_t* byteIN = (uint8_t*) calloc(120,sizeof(uint8_t));
    if(byteIN==NULL) return -1;
    int8_t error=2;
		
	// Check the checksum
    if(checkChecksum(data_in,end,start)){
        free(byteIN);
        byteIN=NULL;
        return 1;
    }	
	
	// Copy the data
    data_length=0;
    for(it=4+start;it<end-1;it++)
    {
        byteIN[it-4-start]=data_in[it];
        data_length++;
    }
		
    switch( data_in[start+3] )
    {
        case 0x80 :	add_type=_64B;
        break;
        case 0x81 :	add_type=_16B;
        break;
        case 0x90 :	mode=UNICAST;
        break;
        case 0x91 :	mode=CLUSTER;
        break;
    }

    error=readXBee(byteIN);
    free(byteIN);
    byteIN=NULL;
	
    return error;
}

/*
 Function: Parses the ND message received by the XBee module
 Values: Stores in 'scannedBrothers' variable the data extracted from the answer
*/
void WaspXBeeCore::treatScan()
{
    uint8_t cont2=0;
    uint8_t length_NI=data_length-19;
		
    if(protocol==XBEE_802_15_4)
    {
        cont2=totalScannedBrothers-1;
        for(it=0;it<2;it++)
        {
            scannedBrothers[cont2].MY[it]=data[it];
        }
        for(it=0;it<4;it++)
        {
            scannedBrothers[cont2].SH[it]=data[it+2];
        }
        for(it=0;it<4;it++)
        {
            scannedBrothers[cont2].SL[it]=data[it+6];
        }
        scannedBrothers[cont2].RSSI=data[10];
        if (data_length>12)
        {
            for(it=0;it<(data_length-12);it++)
            {
                scannedBrothers[cont2].NI[it]=char(data[it+11]);
            }
        }
    }
    if( (protocol==ZIGBEE) || (protocol==DIGIMESH) || (protocol==XBEE_900) || (protocol==XBEE_868) )
    {
        cont2=totalScannedBrothers-1;
        for(it=0;it<2;it++)
        {
            scannedBrothers[cont2].MY[it]=data[it];
        }
        for(it=0;it<4;it++)
        {
            scannedBrothers[cont2].SH[it]=data[it+2];
        }
        for(it=0;it<4;it++)
        {
            scannedBrothers[cont2].SL[it]=data[it+6];
        }
        for(it=0;it<length_NI;it++)
        {
            scannedBrothers[cont2].NI[it]=char(data[it+10]);
        }
        for(it=0;it<2;it++)
        {
            scannedBrothers[cont2].PMY[it]=data[it+length_NI+11];
        }
        scannedBrothers[cont2].DT=data[length_NI+13];
        scannedBrothers[cont2].ST=data[length_NI+14];
        for(it=0;it<2;it++)
        {
            scannedBrothers[cont2].PID[it]=data[it+length_NI+15];
        }
        for(it=0;it<2;it++)
        {
            scannedBrothers[cont2].MID[it]=data[it+length_NI+17];
        }
    }
}

/*
 Function: Checks the checksum is good
 Parameters:
 	data_in : the answer received by the module
 	end : the end of the frame
 	start : the start of the frame
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeCore::checkChecksum(uint8_t* data_in, uint16_t end, uint16_t start)
{	
    uint16_t checksum=0;	
		
    for(it=3+start;it<end;it++)
    {
        checksum=checksum+data_in[it];
    }
    if( (checksum==255) ) return 0;
    checksum%=256;
    if( checksum!=255 ) return 1;
    return 0;
}

/*
 Function: Clears the variable 'command'
*/
void WaspXBeeCore::clearCommand()
{
    for(it=0;it<30;it++)
    {
        command[it]=0;
    }
}


/*
 Function: It frees a position in index array
*/
void WaspXBeeCore::freeIndex()
{
    uint16_t counter1=0;
	
    nextIndex1=0;
    while( counter1<MAX_FINISH_PACKETS )
    {
        for(it=counter1;it<(MAX_FINISH_PACKETS-1);it++)
        {
            if( pendingFragments[counter1]->time < pendingFragments[it+1]->time ) nextIndex1++;
            else break;
        }
        if( nextIndex1==(MAX_FINISH_PACKETS-1) ){
            nextIndex1=counter1;
            counter1=MAX_FINISH_PACKETS;
        }
        else nextIndex1=counter1+1;
        counter1++;
    }
    for(it=0;it<MAX_FRAG_PACKETS;it++)
    {
        free(packet_fragments[nextIndex1][it]);
        packet_fragments[nextIndex1][it]=NULL;
    }
    pendingFragments[nextIndex1]->time=0;
    free(pendingFragments[nextIndex1]);
    pendingFragments[nextIndex1]=NULL;
    pendingPackets--;
    indexNotModified=0;
}

/*
 Function: It frees index array and matrix
*/
void WaspXBeeCore::freeAll()
{	
    uint8_t temp=0;
	
    for(temp=0;temp<MAX_FINISH_PACKETS;temp++)
    {
        for(it=0;it<MAX_FRAG_PACKETS;it++)
        {
							
            free(packet_fragments[temp][it]);
            packet_fragments[temp][it]=NULL;
        }
        pendingFragments[temp]->time=0;
        free(pendingFragments[temp]);
        pendingFragments[temp]=NULL;
    }
    pendingPackets=0;
    nextIndex1=0;
    indexNotModified=0;
}

/*
 Function: It gets the next index where store the finished packet
*/
uint8_t WaspXBeeCore::getFinishIndex()
{
    for(it=0;it<MAX_FINISH_PACKETS;it++)
    {
        if( packet_finished[it]==NULL ) break;
    }
    return it;
}

/*
 Function: It clears the finished packets array
*/
void WaspXBeeCore::clearFinishArray()
{
    for(it=0;it<MAX_FINISH_PACKETS;it++)
    {
        free(packet_finished[it]);
        packet_finished[it]=NULL;
    }
}

/*
 Function: It gets the index in 'packet_finished' where store the new packet, according to a FIFO policy
*/
uint8_t WaspXBeeCore::getIndexFIFO()
{
    uint8_t position=0;
    uint16_t counter1=0;
	
    while( counter1<MAX_FINISH_PACKETS )
    {
        for(it=counter1;it<(MAX_FINISH_PACKETS-1);it++)
        {
            if( packet_finished[counter1]->time < packet_finished[it+1]->time ) position++;
            else break;
        }
        if( position==(MAX_FINISH_PACKETS-1) ){
            position=counter1;
            counter1=MAX_FINISH_PACKETS;
        }
        else position=counter1+1;
        counter1++;
    }
    free(packet_finished[position]);
    packet_finished[position]=NULL;
    return position;
}

/*
 Function: It gets the index in 'packet_finished' where store the new packet, according to a LIFO policy
*/
uint8_t WaspXBeeCore::getIndexLIFO()
{
    uint8_t position=0;
    uint16_t counter1=0;
	
    while( counter1<MAX_FINISH_PACKETS )
    {
        for(it=counter1;it<(MAX_FINISH_PACKETS-1);it++)
        {
            if( packet_finished[counter1]->time > packet_finished[it+1]->time ) position++;
            else break;
        }
        if( position==(MAX_FINISH_PACKETS-1) ){
            position=counter1;
            counter1=MAX_FINISH_PACKETS;
        }
        else position=counter1+1;
        counter1++;
    }
    free(packet_finished[position]);
    packet_finished[position]=NULL;
    return position;
}

/*
 Function: It frees the index array and the matrix row corresponding to the position is sent as an input parameter
*/
void WaspXBeeCore::freeIndexMatrix(uint8_t position)
{		
    for(it=0;it<MAX_FRAG_PACKETS;it++)
    {
        free(packet_fragments[position][it]);
        packet_fragments[position][it]=NULL;
    }
    pendingFragments[position]->time=0;
    free(pendingFragments[position]);
    pendingFragments[position]=NULL;
    pendingPackets--;
}

/*
 Function: It receives the first packet of a new firmware
 Returns: Integer that determines if there has been any error 
   error=1 --> There has been an error while executing the function
   error=0 --> The function has been executed with no errors
*/
uint8_t WaspXBeeCore::new_firmware_received()
{
	char aux_array[15];
	bool startSequence = true;
	char* asteriscos;
	uint8_t channel_to_set = 0;
	bool error_sd = false;
	
	
	// Check 'KEY_ACCESS'
	for (it = 0; it < 8;it++){
             if(packet_finished[pos-1]->data[it] != Utils.readEEPROM(it+107)){
		     startSequence = false;  
		     break;
	     }
        }
	
	firm_info.multi_type=3;
	
	if( startSequence && !firm_info.already_init )
	{
		asteriscos = (char*) calloc(449,sizeof(char));
		if( asteriscos == NULL ){
			return 1;
		}

		// Set OTA Flag and set last time a OTA packet was received
		programming_ON=1;		
		firm_info.time_arrived = millis();

		for(it=0;it<32;it++)
		{
			firm_info.ID[it]=packet_finished[pos-1]->data[it+8];
		}
		firm_info.ID[it]='\0';
		for(it=0;it<12;it++)
		{
			firm_info.DATE[it]=packet_finished[pos-1]->data[it+40];
		}
		firm_info.DATE[it]='\0';
		
		// 802.15.4 Multicast or DigiMesh direct-access
		if( packet_finished[pos-1]->data_length==53 && (protocol==XBEE_802_15_4 || protocol==DIGIMESH) )
		{
			channel_to_set = packet_finished[pos-1]->data[52];
			getChannel();
			firm_info.channel=channel;
			if( !error_AT ) setChannel(channel_to_set);
			writeValues();
			firm_info.multi_type=0;
		}
		
		// DigiMesh or ZigBee Multicast
		if( packet_finished[pos-1]->data_length==60 && (protocol==ZIGBEE || protocol==DIGIMESH) )
		{
			// Copy 'Auth key'
			for (it = 0; it < 8;it++) firm_info.authkey[it] = Utils.readEEPROM(it+107);
			
			// Set new 'Auth key'
			for (it = 0; it < 8;it++) Utils.writeEEPROM(it+107, packet_finished[pos-1]->data[it+52]);
			
			firm_info.multi_type=1;
		}
		
		// 868 or 900 Multicast
		if( packet_finished[pos-1]->data_length==84 && (protocol==XBEE_868 || protocol==XBEE_900) )
		{
			// Copy 'Encryption key'
			for (it = 0; it < 16;it++) firm_info.encryptionkey[it] = packet_finished[pos-1]->data[it+52];
			
			char auxkey[16];
			for (it = 0; it < 16;it++) auxkey[it] = packet_finished[pos-1]->data[it+68];
			
			setLinkKey(auxkey);
			writeValues();
			
			firm_info.multi_type=2;
		}
		
		
		for(it=0;it<4;it++)
		{
			firm_info.mac_programming[it]=packet_finished[pos-1]->macSH[it];
		}
		for(it=0;it<4;it++)
		{
			firm_info.mac_programming[it+4]=packet_finished[pos-1]->macSL[it];
		}
		
		for(it=0; it<7; it++)
		{
			if( firm_info.ID[it]=='*' ) break;
			firm_info.name_file[it]=firm_info.ID[it];
		}
		firm_info.name_file[it]='\0';
		
		firm_info.packets_received=0;
		firm_info.data_count_packet = 0;
		firm_info.data_count_packet_ant = 0;
		firm_info.paq_disordered = 0;
		firm_info.already_init = 1;
		
		file2.close();
	
		file1.close();
	
		root.close();
		
		sd_on=0;
				
		if( !sd_on )
		{
			sd_on=1;
		
			// initializa SPI
			if(!card.init(SPI_FULL_SPEED)){
				sd_on=0;
				error_sd=true;
			}
	
			// initialize a FAT volume
			volume.init(&card);
	
			// open the root directory
			if(!root.openRoot(&volume)){
				sd_on=0;
				error_sd=true;
			}
		}
		
		// Create the first sector
		for (it = 0;it<448;it++){
			asteriscos[it]='*';
		}
		asteriscos[448] = '\0';
		
		if( !error_sd )
		{
			if( !file1.open(&root, firm_info.name_file, O_WRITE | O_CREAT | O_EXCL | O_SYNC | O_APPEND) )
			{
				file1.remove(&root,firm_info.name_file);
				if(!file1.open(&root, firm_info.name_file, O_WRITE | O_CREAT | O_EXCL | O_SYNC | O_APPEND)) error_sd=true;
			}
			
			if( !error_sd )
			{
				file1.write(START_SECTOR);
				file1.write(firm_info.ID);
				file1.write(asteriscos);
			}
		}
		
		if( error_sd )
		{
			programming_ON=0;
			free(asteriscos);
			asteriscos=NULL;
			setMulticastConf();
			return 1;
		}
		
		free(asteriscos);
		asteriscos=NULL;
		
	}
	// Error mismatch --> Delete the packet
	else
	{
		if(!programming_ON)
		{
			programming_ON=0;
			setMulticastConf();
			return 1;
		}
	}
	return 0;
}


/*
 Function: It receives the data packets of a new firmware
 Returns: Nothing
*/
void WaspXBeeCore::new_firmware_packets()
{
	uint8_t data_bin[92];
	uint16_t sd_index=0;
	bool true_mac = true;
	bool error_sd = false;
	
	it=0;
	
	
	if( packet_finished[pos-1]->data[it]=='$' && programming_ON )
	{
		for(it=0;it<4;it++)
		{
			if( packet_finished[pos-1]->macSH[it] != firm_info.mac_programming[it] ){
				true_mac=false;
				break;
			}
		}
		for(it=0;it<4;it++)
		{
			if( packet_finished[pos-1]->macSL[it] != firm_info.mac_programming[it+4] ){
				true_mac=false;
				break;
			}
		}
		
		if( true_mac )
		{
			firm_info.data_count_packet = packet_finished[pos-1]->data[1];
			
			if (	(firm_info.data_count_packet == 0 && firm_info.packets_received==0) ||
               	  		(firm_info.data_count_packet - firm_info.data_count_packet_ant) == 1 ||
               			(firm_info.data_count_packet == 0)&&(firm_info.data_count_packet_ant == 255) )
                	{
				// To enable ASCII mode uncomment this part
//                  		for(sd_index=0;sd_index<(packet_finished[pos-1]->data_length-2)/2;sd_index++){
//                    			data_bin[sd_index]=Utils.converter(packet_finished[pos-1]->data[2*sd_index+2],packet_finished[pos-1]->data[2*sd_index+2+1]);
//                			}
				for(sd_index=0;sd_index<(packet_finished[pos-1]->data_length-2);sd_index++){
                   			data_bin[sd_index]=packet_finished[pos-1]->data[sd_index+2];
               			}
				
				file1.write(data_bin,sd_index);
								
				firm_info.already_init = 0;

				// Set new OTA previous packet arrival time 
				firm_info.time_arrived=millis();
				
				if(error_sd)
				{
					programming_ON=0;
					file1.remove(&root,firm_info.name_file);
					firm_info.packets_received=0;
					firm_info.paq_disordered=0;
					setMulticastConf();
				}
				else
				{
					firm_info.packets_received++;
					firm_info.data_count_packet_ant = firm_info.data_count_packet;
					firm_info.paq_disordered=0;
					sd_index=0;
				}
			}
			else if( !(firm_info.data_count_packet == firm_info.data_count_packet_ant) )
			{
				if( (firm_info.data_count_packet - firm_info.data_count_packet_ant) == 2 )
				{
					// re-order one packet lost
					firm_info.paq_disordered=1;
				}
				else
				{
					programming_ON=0;
					file1.remove(&root,firm_info.name_file);
					firm_info.packets_received=0;
					firm_info.paq_disordered=0;
					setMulticastConf();
					
					OFF();
					delay(10000);
					ON();
					delay(5000);
					if( uart==UART0 ) XBee.flush();
					else if( uart==UART1 ) XBee2.flush();
					
				}
			}
		}
		else
		{
			programming_ON=0;
			setMulticastConf();
		}
	}
	else
	{
		if(programming_ON)
		{
			programming_ON=0;
			setMulticastConf();
		}
	}
}


/*
 Function: It receives the last packet of a new firmware
*/
void WaspXBeeCore::new_firmware_end()
{
	char aux_array[5];
	uint8_t data_bin[92];
	uint16_t sd_index=0;
	bool true_mac = true;
	char num_packets_char[5];
	uint16_t num_packets=0;
	packetXBee* paq_sent;
	uint8_t destination[8];
	bool send_ok = true;
	bool error_sd = false;
	
		
	for(it=0; it<3; it++)
	{
		aux_array[it]=packet_finished[pos-1]->data[it];
	}
	aux_array[it]='\0';
	
	if( !strcmp(aux_array,"###") && programming_ON )
	{
		for(it=0;it<4;it++)
		{
			if( packet_finished[pos-1]->macSH[it] != firm_info.mac_programming[it] ){
				true_mac=false;
				break;
			}
		}
		for(it=0;it<4;it++)
		{
			if( packet_finished[pos-1]->macSL[it] != firm_info.mac_programming[it+4] ){
				true_mac=false;
				break;
			}
		}
		
		if( true_mac )
		{
			for(it=0;it<(packet_finished[pos-1]->data_length-3);it++){
				num_packets_char[it]=packet_finished[pos-1]->data[it+3];
			}
			num_packets_char[it]='\0';
			num_packets = Utils.array2long(num_packets_char);
			
			if( num_packets!=firm_info.packets_received ){
				send_ok = false;
			}
			else send_ok = true;
			
			if( send_ok )
			{
				file1.close();
				delay(10);
				
				if(!file1.open(&root, firm_info.name_file, O_READ)){
					send_ok = false;
				}
			}
		}
		else{
			send_ok = false;
		}
	}
	else{
		 send_ok = false;
	}
	
	if( send_ok )
	{
		programming_ON=0;
		firm_info.packets_received=0;

		if(!file2.open(&root, BOOT_LIST, O_WRITE | O_CREAT | O_EXCL | O_SYNC | O_APPEND) )
		{
			if(!file2.open(&root, BOOT_LIST, O_WRITE | O_SYNC | O_APPEND)) error_sd=true;
		}
				
		file2.write(firm_info.ID);
		file2.write(firm_info.DATE);
		file2.write('\r');
		file2.write('\n');
		
		if( !error_sd )
		{
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xFC; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE); 
			it=0;
			while(it<4) 
			{ 
				destination[it]=packet_finished[pos-1]->macSH[it]; 
				it++; 
			} 
			while(it<8) 
			{ 
				destination[it]=packet_finished[pos-1]->macSL[it-4]; 
				it++; 
			} 
	
			setDestinationParams(paq_sent, destination, NEW_FIRMWARE_MESSAGE_OK, MAC_TYPE, DATA_ABSOLUTE);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}  
			free(paq_sent); 
			paq_sent=NULL;
		}
		else
		{
			file1.remove(&root,firm_info.name_file);
			programming_ON=0;
			firm_info.packets_received=0;
				
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xFC; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE); 
			it=0;
			while(it<4) 
			{ 
				destination[it]=packet_finished[pos-1]->macSH[it]; 
				it++; 
			} 
			while(it<8) 
			{ 
				destination[it]=packet_finished[pos-1]->macSL[it-4]; 
				it++; 
			} 

			setDestinationParams(paq_sent, destination, NEW_FIRMWARE_MESSAGE_ERROR, MAC_TYPE, DATA_ABSOLUTE);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}  
			free(paq_sent); 
			paq_sent=NULL;
		}
		
		setMulticastConf();
	}
	else
	{
		file1.remove(&root,firm_info.name_file);
		programming_ON=0;
		firm_info.packets_received=0;
				
		paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
		paq_sent->mode=UNICAST; 
		paq_sent->MY_known=0; 
		paq_sent->packetID=0xFC; 
		paq_sent->opt=0; 
		hops=0; 
		setOriginParams(paq_sent, "5678", MY_TYPE); 
		it=0;
		while(it<4) 
		{ 
			destination[it]=packet_finished[pos-1]->macSH[it]; 
			it++; 
		} 
		while(it<8) 
		{ 
			destination[it]=packet_finished[pos-1]->macSL[it-4]; 
			it++; 
		} 

		setDestinationParams(paq_sent, destination, NEW_FIRMWARE_MESSAGE_ERROR, MAC_TYPE, DATA_ABSOLUTE);
		srand(millis());
		delay( (rand()%delay_end + delay_start) );
		// Try to send the answer for several times
		for(int k=0; k<MAX_OTA_RETRIES; k++)
		{		
		   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
		   else delay(rand()%delay_end + delay_start);
		}  
		free(paq_sent); 
		paq_sent=NULL;
		
		setMulticastConf();
	}

	file2.close();
	
	file1.close();
	
	root.close();
		
	sd_on=0;
}


/*
 Function: It uploads the new firmware
*/
void WaspXBeeCore::upload_firmware()
{
	// Buscar en boot_list el ID que nos mandan
	// Si no existe responder ERROR
	// Si existe responder OK
	// Resetear al bootloader
	uint16_t num_lines = 0;
	bool id_exist = true;
	uint16_t offset = 0;
	packetXBee* paq_sent;
	uint8_t destination[8];
	long previous=0;
	uint8_t buf_sd[46];
	bool end_file=false;
	uint8_t num_bytes = 0;
	bool reset = false;
	bool startSequence = true;
	bool error_sd = false;
	uint8_t reintentos=0;
	
	// Check 'KEY_ACCESS'
	for (it = 0; it < 8;it++){
		if(packet_finished[pos-1]->data[it] != Utils.readEEPROM(it+107)){
			startSequence = false;  
			break;
		}
	}
	
	if( startSequence )
	{
		if( !sd_on )
		{
			sd_on=1;
		
		// initializa SPI
			if(!card.init(SPI_FULL_SPEED)){
				sd_on=0;
				error_sd=true;
			}
	
		// initialize a FAT volume
			volume.init(&card);
	
		// open the root directory
			if(!root.openRoot(&volume)){
				sd_on=0;
				error_sd=true;
			}
			
			if(file2.open(&root, BOOT_LIST, O_READ))
			{
				sd_on=1;
				error_sd=false;
				file2.close();
			}
		}
	
		if( !error_sd )
		{
			if(file2.open(&root, BOOT_LIST, O_READ))
			{
				for(it=0;it<32;it++)
				{
					firm_info.ID[it]=packet_finished[pos-1]->data[it+8];
				}
				firm_info.ID[it]='\0';
			
				previous=millis();
				/*	while( num_lines>0 && (millis()-previous<5000) )*/
				while( millis()-previous<5000 && !end_file)
				{		
					if( (num_bytes=file2.read(buf_sd,sizeof(buf_sd))) == 0) end_file=true;
			
					for(it=0;it<32;it++)
					{
						if(buf_sd[it]!=firm_info.ID[it]){
							id_exist=false;
							break;
						}
					}
					if(!id_exist && !end_file ) id_exist=true;
					else if(id_exist) break;
				}
			}
			else
			{
				id_exist=false;
			}
			
			if(id_exist)
			{
				for(it=0; it<7; it++)
				{
					if( firm_info.ID[it]=='*' ) break;
					firm_info.name_file[it]=firm_info.ID[it];
				}
				firm_info.name_file[it]='\0';
			
				if(!file1.open(&root, firm_info.name_file, O_READ)){
					id_exist=false;
				}
			}
			
			
		}
		else
		{
			id_exist=false;
		}
		
		if( id_exist)
		{
			for(it=0;it<32;it++){
				Utils.writeEEPROM(it+2,firm_info.ID[it]);
			}
		
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xFD; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE); 
			it=0;
			while(it<4) 
			{ 
				destination[it]=packet_finished[pos-1]->macSH[it]; 
				it++; 
			} 
			while(it<8) 
			{ 
				destination[it]=packet_finished[pos-1]->macSL[it-4]; 
				it++; 
			} 

			setDestinationParams(paq_sent, destination, UPLOAD_FIRWARE_MESSAGE_OK, MAC_TYPE, DATA_ABSOLUTE);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}  
			free(paq_sent); 
			paq_sent=NULL;
		
			file2.close();
				
			file1.close();
				
			root.close();
		
			sd_on=0;
			
			free(packet_finished[pos-1]);
			packet_finished[pos-1]=NULL;
			
			// Save the transmitter MAC to answer later
			for(it=0;it<8;it++) Utils.writeEEPROM(99+it,destination[it]);
		
			previous=millis();
			while( !reset && millis()-previous<5000 )
			{
				Utils.writeEEPROM(0x01,0x01);
				if( Utils.readEEPROM(0x01)!=0x01 ) Utils.writeEEPROM(0x01,0x01);
				else reset=true;
				delay(10);
				if( millis()-previous < 0 ) previous=millis(); //avoid millis overflow problem
			}
			PWR.reboot();
		}
		else
		{
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xFD; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE); 
			it=0;
			while(it<4) 
			{ 
				destination[it]=packet_finished[pos-1]->macSH[it]; 
				it++; 
			} 
			while(it<8) 
			{ 
				destination[it]=packet_finished[pos-1]->macSL[it-4]; 
				it++; 
			} 

			setDestinationParams(paq_sent, destination, UPLOAD_FIRWARE_MESSAGE_ERROR, MAC_TYPE, DATA_ABSOLUTE);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}  
			free(paq_sent); 
			paq_sent=NULL;
		
			file2.close();
				
			file1.close();
				
			root.close();
		
			sd_on = 0;	
			
			free(packet_finished[pos-1]);
			packet_finished[pos-1]=NULL;
		}
	}	
}


/*
 Function: It answers the ID requested
*/
void WaspXBeeCore::request_ID()
{
	char PID_aux[33];
	packetXBee* paq_sent;
	uint8_t destination[8];
	char ID_aux[17];
	bool startSequence = true;
			
	
	// Check 'KEY_ACCESS'
	for (it = 0; it < 8;it++){
		if(packet_finished[pos-1]->data[it] != Utils.readEEPROM(it+107)){
			startSequence = false;  
			break;
		}
	}
	
	if( startSequence )
	{
		for(it=0;it<32;it++)
		{
			PID_aux[it]=Utils.readEEPROM(it+34);
		}
		PID_aux[32]='\0';
		
		for(it=0;it<16;it++)
		{
			ID_aux[it]=Utils.readEEPROM(it+147);
		}
		ID_aux[16]='\0';
	
		paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
		paq_sent->mode=UNICAST; 
		paq_sent->MY_known=0; 
		paq_sent->packetID=0xFE; 
		paq_sent->opt=0; 
		hops=0; 
		setOriginParams(paq_sent, "5678", MY_TYPE);
	
		it=0;
		while(it<4) 
		{ 
			destination[it]=packet_finished[pos-1]->macSH[it]; 
			it++; 
		} 
		while(it<8) 
		{ 
			destination[it]=packet_finished[pos-1]->macSL[it-4]; 
			it++; 
		} 
		
		setDestinationParams(paq_sent, destination, PID_aux, MAC_TYPE, DATA_ABSOLUTE);
		setDestinationParams(paq_sent, destination, ID_aux, MAC_TYPE, DATA_OFFSET);
		srand(millis());
		delay( (rand()%delay_end + delay_start) );
		// Try to send the answer for several times
		for(int k=0; k<MAX_OTA_RETRIES; k++)
		{		
		   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
		   else delay(rand()%delay_end + delay_start);
		}  
		free(paq_sent); 
		paq_sent=NULL;
	}
	else
	{
	}
}


/*
 Function: It answers the boot list file
*/
void WaspXBeeCore::request_bootlist()
{
	// Buscar en boot_list el ID que nos mandan
	// Si no existe responder ERROR
	// Si existe responder OK
	// Resetear al bootloader
	uint16_t num_lines = 0;
	bool id_exist = true;
	uint16_t offset = 0;
	packetXBee* paq_sent;
	uint8_t destination[8];
	long previous=0;
	uint8_t buf_sd[46];
	char buf_sd_aux[47];
	bool end_file=false;
	uint8_t num_bytes = 0;
	bool reset = false;
	bool startSequence = true;
	uint8_t errors_tx = 0;
			
	
	// Check 'KEY_ACCESS'
	for (it = 0; it < 8;it++){
		if(packet_finished[pos-1]->data[it] != Utils.readEEPROM(it+107)){
			startSequence = false;  
			break;
		}
	}
	
	if( startSequence )
	{
		if( !sd_on )
		{
			sd_on=1;
		
		// initializa SPI
			if(!card.init(SPI_FULL_SPEED)){
				sd_on=0;
			}
	
		// initialize a FAT volume
			volume.init(&card);
	
		// open the root directory
			if(!root.openRoot(&volume)){
				sd_on=0;
			}
		}
	
		if( sd_on )
		{
			if(file2.open(&root, BOOT_LIST, O_READ))
			{
				previous=millis();
				while( millis()-previous<30000 && !end_file)
				{
					if( (num_bytes=file2.read(buf_sd,sizeof(buf_sd))) <= 3) end_file=true;
					
					if( !end_file )
					{
						for(it=0;it<(sizeof(buf_sd));it++)
						{
							buf_sd_aux[it]=buf_sd[it];
						}
						buf_sd_aux[it]='\0';
				
						paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
						paq_sent->mode=UNICAST; 
						paq_sent->MY_known=0; 
						paq_sent->packetID=0xFF; 
						paq_sent->opt=0; 
						hops=0; 
						setOriginParams(paq_sent, "5678", MY_TYPE);
	
						it=0;
						while(it<4) 
						{ 
							destination[it]=packet_finished[pos-1]->macSH[it]; 
							it++; 
						} 
						while(it<8) 
						{ 
							destination[it]=packet_finished[pos-1]->macSL[it-4]; 
							it++; 
						} 
		
						setDestinationParams(paq_sent, destination, buf_sd_aux, MAC_TYPE, DATA_ABSOLUTE);
						srand(millis());
						delay( (rand()%delay_end + delay_start) );
						// Try to send the answer for several times
					        for(int k=0; k<MAX_OTA_RETRIES; k++)
						{		
						   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
						   else delay(rand()%delay_end + delay_start);
						}  
						if( error_TX ) errors_tx++;
						free(paq_sent); 
						paq_sent=NULL;
					}
				}
			
				file2.close();
				
				file1.close();
				
				root.close();
		
				sd_on=0;
				
				if( errors_tx )
				{
					paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
					paq_sent->mode=UNICAST; 
					paq_sent->MY_known=0; 
					paq_sent->packetID=0xFF; 
					paq_sent->opt=0; 
					hops=0; 
					setOriginParams(paq_sent, "5678", MY_TYPE);
	
					it=0;
					while(it<4) 
					{ 
						destination[it]=packet_finished[pos-1]->macSH[it]; 
						it++; 
					} 
					while(it<8) 
					{ 
						destination[it]=packet_finished[pos-1]->macSL[it-4]; 
						it++; 
					} 
		
					setDestinationParams(paq_sent, destination, REQUEST_BOOTLIST_MESSAGE, MAC_TYPE, DATA_ABSOLUTE);
					setDestinationParams(paq_sent, destination, "ER", MAC_TYPE, DATA_OFFSET);
					srand(millis());
					delay( (rand()%delay_end + delay_start) );
					// Try to send the answer for several times
		        		for(int k=0; k<MAX_OTA_RETRIES; k++)
					{		
					   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
					   else delay(rand()%delay_end + delay_start);
					}  
					free(paq_sent); 
					paq_sent=NULL;
				}
				else
				{
					paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
					paq_sent->mode=UNICAST; 
					paq_sent->MY_known=0; 
					paq_sent->packetID=0xFF; 
					paq_sent->opt=0; 
					hops=0; 
					setOriginParams(paq_sent, "5678", MY_TYPE);
	
					it=0;
					while(it<4) 
					{ 
						destination[it]=packet_finished[pos-1]->macSH[it]; 
						it++; 
					} 
					while(it<8) 
					{ 
						destination[it]=packet_finished[pos-1]->macSL[it-4]; 
						it++; 
					} 
		
					setDestinationParams(paq_sent, destination, REQUEST_BOOTLIST_MESSAGE, MAC_TYPE, DATA_ABSOLUTE);
					setDestinationParams(paq_sent, destination, "OK", MAC_TYPE, DATA_OFFSET);
					srand(millis());
					delay( (rand()%delay_end + delay_start) );
					// Try to send the answer for several times
		        		for(int k=0; k<MAX_OTA_RETRIES; k++)
					{		
					   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
					   else delay(rand()%delay_end + delay_start);
					}  
					free(paq_sent); 
					paq_sent=NULL;
				}
			}
			else
			{
			}
		}
		else
		{
		}
	}
	else
	{
	}
}


void WaspXBeeCore::checkNewProgram(){
	
	uint8_t current_ID[32];
	char MID[17];
	uint8_t m = 0;
	bool reprogrammingOK = true;
	uint8_t byte_aux[32];
	packetXBee* paq_sent;
	uint8_t destination[8];
    
	pinMode(SPI_SCK_PIN, INPUT);
	for(it=0;it<32;it++){
		current_ID[it]= Utils.readEEPROM(it+34);
		Utils.writeEEPROM(it+66,current_ID[it]);
	}
	
	for(it=0;it<16;it++)
	{
		MID[it]=Utils.readEEPROM(it+147);
	}
	MID[16]='\0';
	
	if( Utils.readEEPROM(0x01)==0x01 )
	{
		// Checking if programID and currentID are the same --> the program has been changed properly
		for(it = 0;it<32;it++){
			byte_aux[it] = eeprom_read_byte((unsigned char *) it+2);
		}
	
		for(it = 0;it<32;it++){
			if (byte_aux[it] != eeprom_read_byte((unsigned char *) it+34)){
				reprogrammingOK = false;
			}
		}
		Utils.writeEEPROM(0x01,0x00);
		
		// If both IDs are equal a confirmation message is sent to the trasmitter
		if (reprogrammingOK){
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xF9; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE);
	
			for(it=0;it<8;it++) destination[it]=Utils.readEEPROM(99+it);
		
			setDestinationParams(paq_sent, destination, MID, MAC_TYPE, DATA_ABSOLUTE);
			setDestinationParams(paq_sent, destination, ANSWER_START_WITH_FIRMWARE_OK, MAC_TYPE, DATA_OFFSET);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}  
			free(paq_sent); 
			paq_sent=NULL;
		}
		// If the IDs are different an error message is sent to the transmitter
		else
		{
			paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
			paq_sent->mode=UNICAST; 
			paq_sent->MY_known=0; 
			paq_sent->packetID=0xF9; 
			paq_sent->opt=0; 
			hops=0; 
			setOriginParams(paq_sent, "5678", MY_TYPE);
		
			for(it=0;it<8;it++) destination[it]=Utils.readEEPROM(99+it);
		
			setDestinationParams(paq_sent, destination, MID, MAC_TYPE, DATA_ABSOLUTE);
			setDestinationParams(paq_sent, destination, ANSWER_START_WITH_FIRMWARE_ERR, MAC_TYPE, DATA_OFFSET);
			srand(millis());
			delay( (rand()%delay_end + delay_start) );
			// Try to send the answer for several times
		        for(int k=0; k<MAX_OTA_RETRIES; k++)
			{		
			   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
			   else delay(rand()%delay_end + delay_start);
			}   
			free(paq_sent); 
			paq_sent=NULL;
		}
	}
	else
	{
		paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
		paq_sent->mode=BROADCAST; 
		paq_sent->MY_known=0; 
		paq_sent->packetID=0xF9; 
		paq_sent->opt=0; 
		hops=0; 
		setOriginParams(paq_sent, "5678", MY_TYPE);
				
		setDestinationParams(paq_sent, destination, MID, MAC_TYPE, DATA_ABSOLUTE);
		setDestinationParams(paq_sent, destination, RESET_MESSAGE, MAC_TYPE, DATA_OFFSET);
		srand(millis());
		delay( (rand()%delay_end + delay_start) );
		// Try to send the answer for several times
		for(int k=0; k<MAX_OTA_RETRIES; k++)
		{		
		   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
		   else delay(rand()%delay_end + delay_start);
		}  
		free(paq_sent); 
		paq_sent=NULL;
	}
}

void WaspXBeeCore::delete_firmware()
{
	// Buscar en boot_list el ID que nos mandan y lo borra. También borra el fichero con ese nombre en la SD
	// Si no existe responder ERROR
	// Si existe responder OK
	packetXBee* paq_sent;
	uint8_t destination[8];
	long previous=0;
	char buf_sd[46];
	char buf_sd_aux[47];
	bool end_file=false;
	uint8_t num_bytes = 0;
	bool startSequence = true;
	char file_to_delete[8];
	bool error=false;
	char* file_aux = "FILEAUX";
	bool match_id = true;
		
	// Check 'KEY_ACCESS'
	for (it = 0; it < 8;it++){
		if(packet_finished[pos-1]->data[it] != Utils.readEEPROM(it+107)){
			startSequence = false;  
			break;
		}
	}
	
	if( startSequence )
	{
		if( !sd_on )
		{
			sd_on=1;
		
		// initialize SPI
			if(!card.init(SPI_FULL_SPEED)){
				sd_on=0;
			}
	
		// initialize a FAT volume
			volume.init(&card);
	
		// open the root directory
			if(!root.openRoot(&volume)){
				sd_on=0;
			}
		}
	
		if( sd_on )
		{
			// Store the file to delete
			for(it=0;it<7;it++){
				if(packet_finished[pos-1]->data[it+8]=='*') break;
				file_to_delete[it]=packet_finished[pos-1]->data[it+8];
			}
			file_to_delete[7]='\0';
			
			// Open boot list
			if(file2.open(&root, BOOT_LIST, O_READ))
			{
				// Delete firmware file
				if(!file1.remove(&root, file_to_delete)) error=true;
				file1.close();
				
				// Create auxiliary file
				if(!file1.open(&root, file_aux, O_WRITE | O_CREAT | O_EXCL | O_SYNC | O_APPEND)) error=true;
				
				// Algorithm to copy boot_list but the line we want to delete
				previous=millis();
				while( millis()-previous<60000 && !end_file)
				{
					if( (num_bytes=file2.read(buf_sd,sizeof(buf_sd))) <= 3) end_file=true;
						
					if( !end_file )
					{
						for(it=0;it<(sizeof(buf_sd));it++)
						{
							buf_sd_aux[it]=buf_sd[it];
						}
						buf_sd_aux[it]='\0';
						
						for(it=0;it<7;it++)
						{
							if(buf_sd_aux[it]!=file_to_delete[it])
							{
								match_id=false;
								break;
							}
						}
						if(!match_id)
						{
							file1.write(buf_sd_aux);
							match_id = true;
						}
					}
				}
				end_file=false;
				file1.close();
				file2.close();
				
				// Delete previous boot_list
				if(!file2.remove(&root,BOOT_LIST)) error=true;
				
				// Create a new boot_list file and copy the content of auxiliary file
				if(!file2.open(&root, BOOT_LIST, O_WRITE | O_CREAT | O_EXCL | O_SYNC | O_APPEND) ) error=true;
				
				if(file1.open(&root, file_aux, O_READ))
				{
					previous=millis();
					while( millis()-previous<60000 && !end_file)
					{
						if( (num_bytes=file1.read(buf_sd,sizeof(buf_sd))) <= 3) end_file=true;
							
						if( !end_file )
						{
							for(it=0;it<(sizeof(buf_sd));it++)
							{
								buf_sd_aux[it]=buf_sd[it];
							}
							buf_sd_aux[it]='\0';
							
							file2.write(buf_sd_aux);
						}
					}
					file1.close();
					file1.remove(&root,file_aux);
				}
				else error=true;
			}
			else error=true;
		}
		else error=true;
	}
	else error=true;
	
	if(!error)
	{
		paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
		paq_sent->mode=UNICAST; 
		paq_sent->MY_known=0; 
		paq_sent->packetID=0xF8; 
		paq_sent->opt=0; 
		hops=0; 
		setOriginParams(paq_sent, "5678", MY_TYPE);
	
		it=0;
		while(it<4) 
		{ 
			destination[it]=packet_finished[pos-1]->macSH[it]; 
			it++; 
		} 
		while(it<8) 
		{ 
			destination[it]=packet_finished[pos-1]->macSL[it-4]; 
			it++; 
		} 
		
		setDestinationParams(paq_sent, destination, DELETE_MESSAGE_OK, MAC_TYPE, DATA_ABSOLUTE);
		srand(millis());
		delay( (rand()%delay_end + delay_start) );
		// Try to send the answer for several times
		for(int k=0; k<MAX_OTA_RETRIES; k++)
		{		
		   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
		   else delay(rand()%delay_end + delay_start);
		}  
		free(paq_sent); 
		paq_sent=NULL;
	}
	else
	{
		paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
		paq_sent->mode=UNICAST; 
		paq_sent->MY_known=0; 
		paq_sent->packetID=0xF8; 
		paq_sent->opt=0; 
		hops=0; 
		setOriginParams(paq_sent, "5678", MY_TYPE);
	
		it=0;
		while(it<4) 
		{ 
			destination[it]=packet_finished[pos-1]->macSH[it]; 
			it++; 
		} 
		while(it<8) 
		{ 
			destination[it]=packet_finished[pos-1]->macSL[it-4]; 
			it++; 
		} 
		
		setDestinationParams(paq_sent, destination, DELETE_MESSAGE_ERROR, MAC_TYPE, DATA_ABSOLUTE);
		srand(millis());
		delay( (rand()%delay_end + delay_start) );
		// Try to send the answer for several times
	        for(int k=0; k<MAX_OTA_RETRIES; k++)
		{		
		   if(!sendXBee(paq_sent)) k=MAX_OTA_RETRIES;
		   else delay(rand()%delay_end + delay_start);
		}  
		free(paq_sent); 
		paq_sent=NULL;
	}

	file2.close();

	file1.close();

	root.close();

	sd_on=0;	
}

void WaspXBeeCore::setMulticastConf()
{
	switch( firm_info.multi_type )
	{
		case 0:		setChannel(firm_info.channel);
				writeValues();
				break;
			
		case 1: 	// Set previous 'Auth key'
				for (it = 0; it < 8;it++) Utils.writeEEPROM(it+107, firm_info.authkey[it]);
				break;
			
		case 2: 	setLinkKey(firm_info.encryptionkey);
				writeValues();
				break;
	}
}


/*
 Function: Checks if timeout is up while sending program packets
 Returns: Integer that determines if there has been any error 
   1 --> Timeout is up
   0 --> The function has been executed with no errors   
*/
uint8_t WaspXBeeCore::checkOtapTimeout()
{
	long total_time;

	if( programming_ON )
   	{
	   // Check millis crossing through zero
	   if( (millis()-firm_info.time_arrived)<0 ) total_time=millis();//we don't count time till zero
	   else total_time=millis()-firm_info.time_arrived;

	   if( OTA_TIMEOUT < total_time )
	   {
		// Reach Timeout 
		programming_ON=0;
		file1.remove(&root,firm_info.name_file);
		firm_info.packets_received=0;
		firm_info.paq_disordered=0;
		setMulticastConf();
		
		OFF();
		delay(10000);
		ON();
		delay(5000);
		if( uart==UART0 ) XBee.flush();
		else if( uart==UART1 ) XBee2.flush();
		return 1;
	   }
   	}
	return 0;
}



uint8_t WaspXBeeCore::available()
{
  	return serialAvailable(uart);
}

void WaspXBeeCore::flush()
{
  serialFlush(uart);
}

