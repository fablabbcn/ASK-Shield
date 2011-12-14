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
 *  Implementation:	Alberto Bielsa
 */
 

#ifndef __WASPXBEECONSTANTS_H__
#define __WASPXBEECONSTANTS_H__

//Different protocols used in the libraries
#define XBEE_802_15_4 	1
#define	ZIGBEE 		2
#define	DIGIMESH 	3
#define XBEE_900 	4
#define XBEE_868 	5
#define XBEE_XSC	6
#define SWARM		7

//Different frequencies
#define FREQ2_4G 	1
#define FREQ900M 	2
#define FREQ868M 	3

//Different models
#define NORMAL 		1
#define PRO 		2

//Different modes
#define UNICAST 	0
#define BROADCAST 	1
#define CLUSTER 	2
#define SYNC            3

//Different address types
#define _16B 		0
#define _64B 		1

//Different Max Sizes Used in Libraries
#define MAX_DATA		100
#define	DATA_MATRIX		100
#define	MAX_PARSE		300
#define	MAX_BROTHERS		5
#define	MAX_FRAG_PACKETS	5
#define MAX_FINISH_PACKETS	5
#define	TIMEOUT			7000
#define WAIT_TIME               2000
#define WAIT_TIME2              20000
#define WAIT_TIME_READ          5

// FIXME MAL ESTOS VALORES!!!
//Differents types
#define	MY_TYPE		0
#define	MAC_TYPE	1
#define	NI_TYPE		2

//UART Speed
#define	UART_4800	0
#define	UART_9600	1
#define	UART_19200	2
#define	UART_38400	3

//Data
#define	DATA_ABSOLUTE	0
#define	DATA_OFFSET	1

//Variable for debugging
#define	DEBUG       0
#define DEBUG2      0
#define MEMORY      0
#define DEBUG868    0

// Replacement Policy
#define	XBEE_LIFO	0
#define	XBEE_FIFO	1
#define	XBEE_OUT	2

/******************* 802.15.4 **************************/

//Awake Time
#define AWAKE_TIME_802_15_4_H		0x13
#define AWAKE_TIME_802_15_4_L		0x88

//Sleep Time
#define SLEEP_TIME_802_15_4_H		0x00
#define SLEEP_TIME_802_15_4_L		0x00

//Scan Time
#define SCAN_TIME_802_15_4		0x19

//Scan Channels
#define SCAN_CHANNELS_802_15_4_H	0x1F
#define SCAN_CHANNELS_802_15_4_L	0xFE

//Encryption Mode
#define ENCRYPT_MODE_802_15_4		0x00

//Power Level
#define POWER_LEVEL_802_15_4		0x04

//Time RSSI
#define TIME_RSSI_802_15_4		0x28

//Sleep Options
#define	SLEEP_OPTIONS_802_15_4		0x00


/******************* ZIGBEE ****************************/

//Awake Time
#define AWAKE_TIME_ZIGBEE_H		0x13
#define AWAKE_TIME_ZIGBEE_L		0x88

//Sleep Time
#define SLEEP_TIME_ZIGBEE_H		0x00
#define SLEEP_TIME_ZIGBEE_L		0x20

//Scan Time
#define	SCAN_TIME_ZIGBEE		0x3C

//Scan Channels
#define SCAN_CHANNELS_ZIGBEE_H		0x3F
#define SCAN_CHANNELS_ZIGBEE_L		0xFF

//Time Energy Channel
#define TIME_ENERGY_CHANNEL_ZIGBEE	0x03

//Encryption Mode
#define ENCRYPT_MODE_ZIGBEE		0x00

//Power Level
#define POWER_LEVEL_ZIGBEE		0x04

//Time RSSI
#define TIME_RSSI_ZIGBEE		0x28

//Sleep Options
#define	SLEEP_OPTIONS_ZIGBEE		0x00


/******************* DIGIMESH **************************/

//Awake Time
#define AWAKE_TIME_DIGIMESH_H		0x00
#define AWAKE_TIME_DIGIMESH_M		0x07
#define AWAKE_TIME_DIGIMESH_L		0xD0

//Sleep Time
#define SLEEP_TIME_DIGIMESH_H		0x00
#define SLEEP_TIME_DIGIMESH_M		0x00
#define SLEEP_TIME_DIGIMESH_L		0xC8

//Scan Time
#define SCAN_TIME_DIGIMESH_H		0x00
#define SCAN_TIME_DIGIMESH_L		0x82

//Encryption Mode
#define ENCRYPT_MODE_DIGIMESH		0x00

//Power Level
#define POWER_LEVEL_DIGIMESH		0x04

//Time RSSI
#define TIME_RSSI_DIGIMESH		0x20

//Sleep Options
#define	SLEEP_OPTIONS_DIGIMESH		0x00


/************************* 802.15.4 AT COMMANDS ************************************************/
#define	set_retries_802		"7E0005085252520000"
#define	get_retries_802		"7E00040852525201"
#define set_delay_slots_802	"7E00050852524E0000"
#define get_delay_slots_802	"7E00040852524E05"
#define set_mac_mode_802	"7E000508524D4D0000"
#define get_mac_mode_802	"7E000408524D4D0B"
#define set_energy_thres_802	"7E0005085243410000"
#define get_energy_thres_802	"7E00040852434121"
#define get_CCA_802		"7E0004085245431D"
#define reset_CCA_802		"7E000508524543001D"
#define get_ACK_802		"7E0004085245411F"
#define reset_ACK_802		"7E000508524541001F"
#define	set_duration_energy	"7E0005085245440000"

/************************* 868 AT COMMANDS ************************************************/
#define	get_RF_errors_868	"7E0004085245520E"
#define	get_good_pack_868	"7E0004085247441A"
#define	get_channel_RSSI_868	"7E0005085252430000"
#define	get_trans_errors_868	"7E000408525452FF"
#define	get_temperature_868	"7E00040852545001"
#define	get_supply_Volt_868	"7E0004085225562A"
#define	get_device_type_868	"7E0004085244441D"
#define	get_payload_bytes_868	"7E000408524E5007"
#define	set_mult_broadcast_868	"7E000508524D540000"
#define	get_mult_broadcast_868	"7E000408524D5404"
#define	set_retries_868		"7E0005085252520000"
#define	get_retries_868		"7E00040852525201"
#define	get_duty_cicle_868	"7E0004085244431E"
#define	get_reset_reason_868	"7E00040852522330"
#define	get_ACK_errors_868	"7E00040852544110"

/************************* CORE AT COMMANDS ************************************************/
#define	get_own_mac_low		"7E00040852534C06"
#define	get_own_mac_high	"7E0004085253480A"
#define	set_own_net_address	"7E000608524D59000000"
#define	get_own_net_address	"7E000408524D59FF"
#define	set_baudrate		"7E0005085242440000"
#define	set_api_mode		"7E0005085241500000"
#define	set_api_options		"7E00050852414F0000"
#define	set_pan			"7E000608524944000000"
#define	set_pan_zb		"7E000C08524944000000000000000000"
#define	get_pan			"7E00040852494418"
#define	set_sleep_mode_xbee	"7E00050852534D0000"
#define	get_sleep_mode_xbee	"7E00040852534D05"
#define	set_awake_time		"7E000608525354000000"
#define	set_awake_time_DM	"7E00070852535400000000"
#define	set_sleep_time		"7E000608525350000000"
#define	set_sleep_time_DM	"7E00070852535000000000"
#define	set_channel		"7E0005085243480000"
#define	get_channel		"7E0004085243481A"
#define	get_NI			"7E000408524E490E"
#define	set_scanning_time	"7E000508524E540000"
#define	set_scanning_time_DM	"7E000608524E54000000"
#define	get_scanning_time	"7E000408524E5403"
#define	set_discov_options	"7E000508524E4F0000"
#define	get_discov_options	"7E000408524E4F08"
#define	write_values		"7E000408525752FC"
#define	set_scanning_channel	"7E000608525343000000"
#define	get_scanning_channel	"7E0004085253430F"
#define	get_duration_energy	"7E0004085253440E"
#define	set_link_key		"7E001408524B590000000000000000000000000000000000"
#define	set_encryption		"7E0005085245450000"
#define	set_power_level		"7E00050852504C0000"
#define	get_RSSI		"7E0004085244421F"
#define	get_hard_version	"7E00040852485607"
#define	get_soft_version	"7E000408525652FD"
#define	set_RSSI_time		"7E0005085252500000"
#define	get_RSSI_time		"7E00040852525003"
#define	apply_changes		"7E00040852414321"
#define	reset_xbee		"7E0004085246520D"
#define	reset_defaults_xbee	"7E0004085252450E"
#define	set_sleep_options_xbee	"7E00050852534F0000"
#define	get_sleep_options_xbee	"7E00040852534F03"
#define	scan_network		"7E000408524E4413"

/************************* DIGIMESH/900 AT COMMANDS ************************************************/
#define	get_RF_errors_DM	"7E0004085245520E"
#define	get_good_pack_DM	"7E0004085247441A"
#define	get_channel_RSSI_DM	"7E0005085252430000"
#define	get_trans_errors_DM	"7E000408525452FF"
#define	set_network_hops_DM	"7E000508524E480000"
#define	get_network_hops_DM	"7E000408524E480F"
#define	set_network_delay_DM	"7E000508524E4E0000"
#define	get_network_delay_DM	"7E000408524E4E09"
#define	set_network_route_DM	"7E000508524E510000"
#define	get_network_route_DM	"7E000408524E5106"
#define	set_network_retries_DM	"7E000508524D520000"
#define	get_network_retries_DM	"7E000408524D5206"
#define	get_temperature_DM	"7E00040852545001"
#define	get_supply_Volt_DM	"7E0004085225562A"
#define	restore_compiled_DM	"7E00040852523122"

/************************* ZIGBEE AT COMMANDS ************************************************/
#define	reset_network_ZB	"7E000508524E520000"
#define	get_parent_NA_ZB	"7E000408524D5008"
#define	get_rem_children_ZB	"7E000408524E4314"
#define	set_device_type_ZB	"7E0005085244440000"
#define	get_device_type_ZB	"7E0004085244441D"
#define	get_payload_ZB		"7E000408524E5007"
#define	get_ext_PAN_ZB		"7E000408524F5006"
#define	get_opt_PAN_ZB		"7E000408524F490D"
#define	set_max_uni_hops_ZB	"7E000508524E480000"
#define	get_max_uni_hops_ZB	"7E000408524E480F"
#define	set_max_brd_hops_ZB	"7E0005085242480000"
#define	get_max_brd_hops_ZB	"7E0004085242481B"
#define	set_stack_profile_ZB	"7E000508525A530000"
#define	get_stack_profile_ZB	"7E000408525A53F8"
#define	set_period_sleep_ZB	"7E00050852534E0000"
#define	set_join_time_ZB	"7E000508524E4A0000"
#define	get_join_time_ZB	"7E000408524E4A0D"
#define	set_channel_verif_ZB	"7E000508524A560000"
#define	get_channel_verif_ZB	"7E000408524A5605"
#define	set_join_notif_ZB	"7E000508524A4E0000"
#define	get_join_notif_ZB	"7E000408524A4E0D"
#define	set_aggreg_notif_ZB	"7E0005085241520000"
#define	get_aggreg_notif_ZB	"7E00040852415212"
#define	get_assoc_indic_ZB	"7E0004085241491B"
#define	set_encryp_options_ZB	"7E00050852454F0000"
#define	get_encryp_options_ZB	"7E00040852454F11"
#define	set_netwk_key_ZB	"7E001408524E4B0000000000000000000000000000000000"
#define	set_power_mode_ZB	"7E00050852504D0000"
#define	get_power_mode_ZB	"7E00040852504D08"
#define	get_supply_Volt_ZB	"7E0004085225562A"
#define	set_duration_energy_ZB	"7E0005085253440000"

/**************************** Re-Programming OTA COMMANDS **************************************/
#define NEW_FIRMWARE_MESSAGE_OK		"PROGRAM RECEIVED OK$$$$$$$$$$$$$"
#define NEW_FIRMWARE_MESSAGE_ERROR	"PROGRAM RECEIVED ERROR$$$$$$$$$$"
#define UPLOAD_FIRWARE_MESSAGE_OK	"START WITH FIRMWARE OK$$$$$$$$$$$$$$$$$$$$$$$$$$"
#define UPLOAD_FIRWARE_MESSAGE_ERROR	"START WITH FIRMWARE ERROR$$$$$$$$$$$$$$$$$$$$$$$"
#define REQUEST_ID_MESSAGE		"READ FIRMWARE ID$$$$$$$$$$$$$$"
#define REQUEST_BOOTLIST_MESSAGE	"READ BOOTLIST$$$$$$$$$$$$$$$$$"
#define ANSWER_START_WITH_FIRMWARE_OK 	"NEW PROGRAM RUNNING$$$$$$$$$$$$$"
#define ANSWER_START_WITH_FIRMWARE_ERR	"PREVIOUS PROGRAM RUNNING$$$$$$$$"
#define RESET_MESSAGE		 	"RESTARTING$$$$$$$$$$$$$$$$$$$$$$"
#define DELETE_MESSAGE_OK	 	"FIRMWARE DELETED$$$$$$$$$$$$$$$$"
#define DELETE_MESSAGE_ERROR	 	"FIRMWARE NOT DELETED$$$$$$$$$$$$"
#define START_SECTOR 			"FIRMWARE_FILE_FOR_WASPMOTE######"
#define	BOOT_LIST			"boot.txt"
#define	delay_start			1
#define	delay_end			1000
#define	MAX_OTA_RETRIES			3
#define	OTA_TIMEOUT			10000 //milliseconds

#endif
