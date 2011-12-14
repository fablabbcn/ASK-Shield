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
 *  Version:		0.10
 *  Design:		David Gascón
 *  Implementation:	Alberto Bielsa, David Cuartielles
 */
  
 
#ifndef WaspGPRSconstants_h
#define WaspGPRSconstants_h

#include <inttypes.h> 

#define	AT_COMMAND	"AT"
#define AT_SHUTDOWN  	"#SHDN"  // shutdown the device

// Calls Constants
#define AT_CALL      "D"  // start phonecall, needs phone number
#define AT_CALL_R    "OK"
#define AT_HANG      "H"  // hang phonecall, no parameters
#define AT_HANG_R    "OK"

// Voice Mode Constants
#define AT_VOICE_MODE  "+FCLASS=8"  // activate voice mode
#define AT_VOICE_MODE_R  "OK"
#define	AT_ID_INCALL	"+CLIP=1"
#define	AT_ID_INCALL_R	"OK"

// SMS Constants
#define AT_SMS       "+CMGS="    // set phone number to send SMS to, needs phone number
#define AT_SMS_R     ">"
#define	AT_SMS_TEXT_R	"OK"
#define AT_SMS_MODE  "+CMGF=1"   // select text mode for SMS
#define AT_SMS_MODE_R "OK"
#define AT_SMS_UNREAD  "+CMGL=\"REC UNREAD\""  // get the list of unread SMS messages
#define AR_SMS_UNREAD_R  "OK"
#define	AT_SMS_INFO	"+CNMI=2,1,0,0,0"
#define	AT_SMS_INFO_R	"OK"
#define	AT_SMS_READ	"+CMGR=" // needs index of sms to read
#define	AT_SMS_READ_R	"OK"

// Sound Constants
#define AT_SOUND_INT  "#CAP=2"   // set internal audio path
#define AT_SOUND_INT_R  "OK"
#define AT_SOUND_EXT  "#CAP=1"   // set external audio path
#define AT_SOUND_EXT_R  "OK"
#define AT_VOLUME_SET  "+CLVL="  // set volume for selected audio path, needs number (min)0..10(MAX)
#define AT_VOLUME_SET_R  "OK"

// SIM Constants
#define AT_SIM_ACTIVATE  "+CPBS=\"SM\""  // set SIM card mode to the most basic one
#define AT_SIM_ACTIVATE_R "OK"
#define AT_SIM_CHECK_NUMBERS  "+CPBF=\"BSP\""  // look for the numbers with the entry BSP in the SIM
#define AT_SIM_CHECK_NUMBERS_R "OK"
#define AT_PIN	"+CPIN="
#define AT_PIN_R	"OK"

// Power Modes Constants
#define POWER_HIBERNATE	"*PSCPOF"
#define	POWER_SLEEP	"+KSLEEP=1"
#define	POWER_R		"OK"

// Reset Constants
#define RESET_GPRS "ATZ"
#define RESET_GPRS_R "OK"

// GPRS Constants
#define AT_GPRS_PATTERN "+KPATTERN="
#define	AT_GPRS_PATTERN_R "OK"
#define	GPRS_PATTERN	"ENDMES"
#define	AT_GPRS_CONN_CFG "+KCNXCFG="
#define	AT_GPRS_CONN_CFG_R "OK"
#define	AT_GPRS "GPRS"
//#define	AT_GPRS_APN "movistar.es"
#define	AT_GPRS_APN "internetmas"
//#define	AT_GPRS_LOGIN "MOVISTAR"
#define	AT_GPRS_LOGIN ""
// #define	AT_GPRS_PASSW "MOVISTAR"
#define	AT_GPRS_PASSW ""
#define	AT_GPRS_IP "0.0.0.0"
#define	AT_GPRS_DNS1 "194.79.1.100"
#define	AT_GPRS_DNS2 "194.79.1.101"
#define	AT_GPRS_TCP_CFG "AT+KTCPCFG="
#define	AT_GPRS_TCP_CFG_R "OK"
#define	AT_GPRS_TCP_CNX	"AT+KTCPCNX="
#define	AT_GPRS_CNX_TIMER "+KCNXTIMER="
#define	AT_GPRS_CNX_TIMER_R "OK"
#define	AT_GPRS_CNX_PROFILE "+KCNXPROFILE=0"
#define	AT_GPRS_CNX_PROFILE_R "OK"
#define	AT_GPRS_CHECK "+CGATT=1"
#define	AT_GPRS_CHECK_R "OK"
#define	AT_GPRS_TCP_SND "AT+KTCPSND="
#define	AT_GPRS_TCP_SND_R "OK"
#define	AT_GPRS_TCP_RCV "AT+KTCPRCV="
#define	AT_GPRS_TCP_RCV_R "OK"
#define	AT_GPRS_TCP_CLOSE "+KTCPCLOSE="
#define AT_GPRS_TCP_CLOSE_R "OK"
#define	AT_GPRS_TCP_DEL "+KTCPDEL="
#define AT_GPRS_TCP_DEL_R "OK"
#define	AT_GPRS_CELLID	"+KCELL=0"
#define	AT_GPRS_K3	"&K3"
#define	AT_GPRS_K3_R	"OK"

// SMTP Constants
#define	AT_SMTP_PARAM		"+KSMTPPARAM="
#define	AT_SMTP_PARAM_R		"OK"
#define	AT_SMTP_PWD		"+KSMTPPWD="
#define	AT_SMTP_PWD_R		"OK"
#define	AT_SMTP_TO		"+KSMTPTO="
#define	AT_SMTP_TO_R		"OK"
#define	AT_SMTP_SUBJECT		"+KSMTPSUBJECT="
#define	AT_SMTP_SUBJECT_R	"OK"
#define	AT_SMTP_SEND		"+KSMTPUL="
#define	AT_SMTP_SEND_R		"OK"

// POP3 Constants
#define	AT_POP3_PARAM		"+KPOPCNX="
#define	AT_POP3_PARAM_R		"OK"
#define	AT_POP3_LIST		"AT+KPOPLIST"
#define	AT_POP3_READ		"AT+KPOPREAD="
#define	MAX_SIZE_POP3		2048

// FTP Constants
#define	AT_FTP_PARAM		"+KFTPCFG=0"
#define	AT_FTP_SEND		"+KFTPSND="
#define	AT_FTP_RECV		"+KFTPRCV="
#define	AT_FTP_CLOSE		"+KFTPCLOSE="
#define	AT_FTP_CLOSE_R		"OK"
#define	MAX_SIZE_FTP		1024

// IMEI - IMSI Constants
#define AT_GPRS_IMEI "AT+CGSN"
#define AT_GPRS_IMSI "AT+CIMI"


#endif