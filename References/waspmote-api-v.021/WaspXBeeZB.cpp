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

void	WaspXBeeZB::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used)
{
	init(protocol_used, frequency, model_used, 0);
}

void	WaspXBeeZB::init(uint8_t protocol_used, uint8_t frequency, uint8_t model_used, uint8_t uart_used)
{
	protocol=protocol_used;
	freq=frequency;
	model=model_used;
	uart=uart_used;
	
	totalFragmentsReceived=0;
	pendingPackets=0;
	pos=0;
	discoveryOptions=0x00;
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

	parentNA[0]=0xFF;
	parentNA[1]=0xFE;
	deviceType[0]=0x00;
	deviceType[1]=0x03;
	deviceType[2]=0x00;
	deviceType[3]=0x00;
	for(it=0;it<8;it++)
	{
		extendedPAN[it]=0x00;
	}
	maxUnicastHops=0x1E;
	maxBroadcastHops=0x00;
	stackProfile=0;
	joinTime=0xFF;
	channelVerification=0x00;
	joinNotification=0x00;
	aggregateNotification=0xFF;
	encryptOptions=0;
	for(it=0;it<16;it++)
	{
		networkKey[it]=0x00;
	}
	powerMode=4;
	
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
}

/*
 Function: Reset the Network
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NR command
 Parameters: 
       reset --> Possible values are 0/1
         0: reset the node network interface
         1: reset the entirely network
*/
uint8_t WaspXBeeZB::resetNetwork(uint8_t reset)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(reset_network_ZB,reset);
    gen_checksum(reset_network_ZB);
    error=gen_send(reset_network_ZB);
    
    return error;
}

/*
 Function: Returns the 16b parent´s network address
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the MP command. Stores in global "parentNA" variable 
         the 16b parent address
*/
uint8_t WaspXBeeZB::getParentNetworkAddress()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_parent_NA_ZB);
    error=gen_send(get_parent_NA_ZB);
    
    if(error==0)
    {
        parentNA[0]=data[0];
        parentNA[1]=data[1];
    }
    return error;
}

/*
 Function: Returns the number of children which can still connect to the current device
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the NC command. Stores in global "remainingChildren" variable the number of
	 remaining children which can connect to this device
*/
uint8_t WaspXBeeZB::getRemainingChildren()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_rem_children_ZB);
    error=gen_send(get_rem_children_ZB);
    
    if(error==0)
    {
        remainingChildren=data[0];
    }
    return error;
}

/*
 Function: Set the current device type
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the DD command
 Parameters:
   type: current device type (0-0xFFFFFFFF)
*/
uint8_t WaspXBeeZB::setDeviceType(uint8_t* type)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_device_type_ZB,type);
    gen_checksum(set_device_type_ZB);
    error=gen_send(set_device_type_ZB);
    
    if(error==0)
    {
        for(it=0;it<4;it++)
        {
            deviceType[it]=type[it];
        }
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
uint8_t WaspXBeeZB::getDeviceType()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_device_type_ZB);
    error=gen_send(get_device_type_ZB);
    
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
uint8_t WaspXBeeZB::getPayloadBytes()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_payload_ZB);
    error=gen_send(get_payload_ZB);
    
    if(error==0)
    {
        maxPayloadBytes[0]=data[0];
        maxPayloadBytes[1]=data[1];
    }
    return error;
} 

/*
 Function: Get the Extended PAN
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the OP command. Stores in global "extendedPan" variable the 64b PAN ID
*/
uint8_t WaspXBeeZB::getExtendedPAN()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_ext_PAN_ZB);
    error=gen_send(get_ext_PAN_ZB);
    
    if(error==0)
    {
        for(it=0;it<8;it++)
        {
            extendedPAN[it]=data[it];
            delay(25);
        }
    }  
    return error;
}

/*
 Function: Get the Operating PAN ID
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the OI command. Stores in global "operatingPAN" variable the 64b PAN ID
*/
uint8_t WaspXBeeZB::getOperatingPAN()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_opt_PAN_ZB);
    error=gen_send(get_opt_PAN_ZB);
    
    if(error==0)
    {
        for(it=0;it<2;it++)
        {
            operatingPAN[it]=data[it];
            delay(25);
        }
    }  
    return error;
}

/*
 Function: Set the maximum hops limit
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NH command
 Parameters:
   hops: max number of hops that an unicast message can make (0x00-0xFF)
*/
uint8_t WaspXBeeZB::setMaxUnicastHops(uint8_t hops)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_max_uni_hops_ZB,hops);
    gen_checksum(set_max_uni_hops_ZB);
    error=gen_send(set_max_uni_hops_ZB);
    
    if(error==0)
    {
    maxUnicastHops=hops;
    }
    return error;
} 

/*
 Function: Get the maximum hop limits
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the NH command. Stores in global "error" variable 
         any error happened while execution
*/
uint8_t WaspXBeeZB::getMaxUnicastHops()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_max_uni_hops_ZB);
    error=gen_send(get_max_uni_hops_ZB);
    
    if(error==0)
    {
        maxUnicastHops=data[0];
    }
    return error;
}
  
/*
 Function: Set the maximum number of hops for each broadcast data transmission
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the BH command
 Parameters:
   bhops: max number of hops a bradcast message can make (0x00-0x20)
*/
uint8_t WaspXBeeZB::setMaxBroadcastHops(uint8_t bhops)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_max_brd_hops_ZB,bhops);
    gen_checksum(set_max_brd_hops_ZB);
    error=gen_send(set_max_brd_hops_ZB);
    
    if(error==0)
    {
        maxBroadcastHops=bhops;
    }
    return error;
}

/*
 Function: Get the maximum number of hops for each broadcast data transmission
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the BH command. Stores in global "maxBroadcastHops" variable the max number
	 of broadcast hops
*/
uint8_t WaspXBeeZB::getMaxBroadcastHops()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_max_brd_hops_ZB);
    error=gen_send(get_max_brd_hops_ZB);
    
    if(error==0)
    {
        maxBroadcastHops=data[0];
    }
    return error;
} 

/*
 Function: Set the ZigBee Stack profile
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the ZS command
 Parameters:
   profile: choosen stack profile (0-2)
*/
uint8_t WaspXBeeZB::setStackProfile(uint8_t profile)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_stack_profile_ZB,profile);
    gen_checksum(set_stack_profile_ZB);
    error=gen_send(set_stack_profile_ZB);
    
    if(error==0)
    {
        stackProfile=profile;
    }
    return error;
}

/*
 Function: Get the ZigBee Stack profile
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the ZS command. Stores in global "stackProfile" variable the stack profile
*/
uint8_t WaspXBeeZB::getStackProfile()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_stack_profile_ZB);
    error=gen_send(get_stack_profile_ZB);
    
    if(error==0)
    {
        stackProfile=data[0];
    }
    return error;
}

/*
 Function: Set the number of Sleep Periods to not assert XBee pin
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the SN command
 Parameters:
   periods: array which contains the number of sleep periods to sleep
*/
uint8_t WaspXBeeZB::setPeriodSleep(uint8_t* periods)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_period_sleep_ZB,periods);
    gen_checksum(set_period_sleep_ZB);
    error=gen_send(set_period_sleep_ZB);
    
    if(error==0)
    {
        periodSleep[0]=periods[0];
        periodSleep[1]=periods[1];
    }
    return error;
}


/*
 Function: Set the time that a Coordinator/Router allows nodes to join
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NJ command
 Parameters:
   time: time allowed for joining (0x00-0xFF)
*/
uint8_t WaspXBeeZB::setJoinTime(uint8_t time)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_join_time_ZB,time);
    gen_checksum(set_join_time_ZB);
    error=gen_send(set_join_time_ZB);
    
    if(error==0)
    {
        joinTime=time;
    }
    return error;
}

/*
 Function: Read the time that a Coordinator/Router allows nodes to join
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the NJ command. Stores in global "joinTime" variable the time allowing join
*/
uint8_t WaspXBeeZB::getJoinTime()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_join_time_ZB);
    error=gen_send(get_join_time_ZB);
    
    if(error==0)
    {
        joinTime=data[0];
    }
    return error;
}

/*
 Function: Set the channel verification parameter
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the JV command
 Parameters:
   verif: channel verification parameter (0-1)
*/
uint8_t WaspXBeeZB::setChannelVerification(uint8_t verif)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_channel_verif_ZB,verif);
    gen_checksum(set_channel_verif_ZB);
    error=gen_send(set_channel_verif_ZB);
    
    if(error==0)
    {
        channelVerification=verif;
    }
    return error;
}

/*
 Function: Read the channel verification parameter
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the JV command. Stores in global "channelVerification" variable 
         any error happened while execution
*/
uint8_t WaspXBeeZB::getChannelVerification()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_channel_verif_ZB);
    error=gen_send(get_channel_verif_ZB);
    
    if(error==0)
    {
        channelVerification=data[0];
    }
    return error;
}

/*
 Function: Set the join notification setting
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the JN command
 Parameters:
   notif: join notification setting (0-1)
*/
uint8_t WaspXBeeZB::setJoinNotification(uint8_t notif)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_join_notif_ZB,notif);
    gen_checksum(set_join_notif_ZB);
    error=gen_send(set_join_notif_ZB);
    
    if(error==0)
    {
        joinNotification=notif;
    }
    return error;
}

/*
 Function: Read the join notification setting
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the JN command. Stores in global "error" variable 
         any error happened while execution
*/
uint8_t WaspXBeeZB::getJoinNotification()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_join_notif_ZB);
    error=gen_send(get_join_notif_ZB);
    
    if(error==0)
    {
        joinNotification=data[0];
    }
    return error;
}

/*
 Function: Set time between consecutive aggregate route broadcast messages
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the AR command
 Parameters:
   anotif: time between consecutive aggregate route broadcast messages (0x00-0xFF)
*/
uint8_t WaspXBeeZB::setAggregateNotification(uint8_t anotif)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_aggreg_notif_ZB,anotif);
    gen_checksum(set_aggreg_notif_ZB);
    error=gen_send(set_aggreg_notif_ZB);
    
    if(error==0)
    {
        aggregateNotification=anotif;
    }
    return error;
}

/*
 Function: Get time between consecutive aggregate route broadcast messages
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the AR command. Stores in global "aggregateNotification" variable the time
*/
uint8_t WaspXBeeZB::getAggregateNotification()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_aggreg_notif_ZB);
    error=gen_send(get_aggreg_notif_ZB);
    
    if(error==0)
    {
        aggregateNotification=data[0];
    }
    return error;;
}

/*
 Function: Read information regarding last node join request
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the AI command. Stores in global "associationIndication" variable 
         any error happened while execution
*/
uint8_t WaspXBeeZB::getAssociationIndication()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_assoc_indic_ZB);
    error=gen_send(get_assoc_indic_ZB);
    
    if(error==0)
    {
        associationIndication=data[0];
    }
    return error;
}

/*
 Function: Configure options for encryption
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the EO command
 Parameters:
   eoptions: options for encryption (0x00-0xFF)
*/
uint8_t WaspXBeeZB::setEncryptionOptions(uint8_t eoptions)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_encryp_options_ZB,eoptions);
    gen_checksum(set_encryp_options_ZB);
    error=gen_send(set_encryp_options_ZB);
    
    if(error==0)
    {
        encryptOptions=eoptions;
    }
    return error;
}

/*
 Function: Read options for encryption
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the EO command. Stores in global "encrypOptions" variable 
         any error happened while execution
*/
uint8_t WaspXBeeZB::getEncryptionOptions()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_encryp_options_ZB);
    error=gen_send(get_encryp_options_ZB);
    
    if(error==0)
    {
        encryptOptions=data[0];
    }
    return error;
}

/*
 Function: Set the 128-bit AES encryption key
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the NK command
 Parameters:
   key: 16 position array that contains the 128b AES key (0-0xFFFFFFFFFFFFFFFF)
*/
uint8_t WaspXBeeZB::setNetworkKey(const char* key)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_netwk_key_ZB,key);
    gen_checksum(set_netwk_key_ZB);
    error=gen_send(set_netwk_key_ZB);
    
    if(error==0)
    {
	for(it=0;it<16;it++)
	{
		networkKey[it]=key[it];
	}
    }
    return error;
}

/*
 Function: Set the power mode of the device
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Change the PM command
 Parameters:
   power: power mode (0-1)
*/
uint8_t WaspXBeeZB::setPowerMode(uint8_t power)
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(set_power_mode_ZB,power);
    gen_checksum(set_power_mode_ZB);
    error=gen_send(set_power_mode_ZB);
    
    if(error==0)
    {
        powerMode=power;
    }
    return error;
}

/*
 Function: Read the power mode of the device
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
 Values: Executes the PM command. Stores in global "powerMode" variable the power mode 
*/
uint8_t WaspXBeeZB::getPowerMode()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_power_mode_ZB);
    error=gen_send(get_power_mode_ZB);
    
    if(error==0)
    {
        powerMode=data[0];
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
uint8_t WaspXBeeZB::getSupplyVoltage()
{
    int8_t error=2;
        
    error_AT=2;
    gen_data(get_supply_Volt_ZB);
    error=gen_send(get_supply_Volt_ZB);
    
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
 Function: Sets the Application Layer Encryption
 Returns: Integer that determines if there has been any error 
   error=2 --> The command has not been executed
   error=1 --> There has been an error while executing the command
   error=0 --> The command has been executed with no errors
*/
uint8_t WaspXBeeZB::setAPSencryption(uint8_t mode)
{
    int8_t error=0;
        
    if(mode==XBEE_ON) apsEncryption=1;
    else if(mode==XBEE_OFF) apsEncryption=0;
    
    error_AT=0;
    
    return error;
}


WaspXBeeZB	xbeeZB = WaspXBeeZB();
