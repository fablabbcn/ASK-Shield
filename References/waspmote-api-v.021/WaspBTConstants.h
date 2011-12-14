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
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
  
 
#ifndef WaspBTConstants_h
#define WaspBTConstants_h

// Service Parameters
#define	BT_SERVICE_NAME		"LIBELIUM"
#define	BT_SERVICE_CHANNEL	2
#define	BT_SERVICE_COD		"111111"

// SECURITY
#define	BT_SECURITY_PREDEFINED		1
#define	BT_SECURITY_NONE		2
#define	BT_SECURITY_ON_DEMAND		3
#define	BT_PIN				"1111"

// Accept connections
#define	BT_AUTO_ACC	1
#define	BT_MANUAL_ACC	0

// Discovering options
#define	BT_NO_DISC	0
#define	BT_INQ_EN	1
#define	BT_PAGE_EN	2
#define	BT_DISCOVERABLE	3

// Public name
#define	BT_PUBLIC_NAME	"waspmote_bt"

// Maximum number of discoverable devices
#define	BT_MAX_DEVICES	8

// Maximum data
#define BT_MAX_DATA	400

// AT Commands
#define	BT_AT_SECURITY		"AT+JSEC="
#define	BT_AT_PUBLIC_NAME	"AT+JSLN="
#define	BT_AT_AUTO_ACCEPT	"AT+JAAC="
#define	BT_AT_DISCOV_OPT	"AT+JDIS="
#define	BT_AT_PIN_REQUEST	"AT+JPCR="
#define	BT_AT_ACCEPT_CONN	"AT+JACR="
#define	BT_AT_SERVICE_PAR	"AT+JRLS="
#define	BT_AT_RESET		"AT+JRES"
#define	BT_AT_DISCOVER		"AT+JDDS="
#define	BT_AT_DISCOVER_DEV	"AT+JSDS="
#define	BT_AT_CONNECT		"AT+JCCR="
#define	BT_AT_OWN_MAC		"AT+JRBD="
#define	BT_AT_DELETE_TD		"AT+JRTD="
#define	BT_AT_SEND_DATA		"AT+JSDA="
#define	BT_AT_DISCONNECT	"AT+JSDR"
#define	BT_AT_STREAM_CONN	"AT+JSCR"


#endif
