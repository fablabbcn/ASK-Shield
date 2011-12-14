/*! \file WaspGPRS.h
    \brief Library for managing the GPRS module
    
    Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
    Version:		0.13

    Design:		David Gascón

    Implementation:	Alberto Bielsa, David Cuartielles

*/

 /*! \def WaspGPRS_h
    \brief The library flag
    
  */
#ifndef WaspGPRS_h
#define WaspGPRS_h

/******************************************************************************
 * Includes
 ******************************************************************************/
 
#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def GPRS_ON
    \brief Working mode. ON in this case
 */
#define GPRS_ON         0

/*! \def GPRS_SLEEP
    \brief Working mode. SLEEP in this case
 */
#define GPRS_SLEEP      1

/*! \def GPRS_HIBERNATE
    \brief Working mode. HIBERNATE in this case
 */
#define GPRS_HIBERNATE  2

/*! \def GPRS_OFF
    \brief Working mode. OFF in this case
 */
#define GPRS_OFF  3

/*! \def GPRS_ERROR_POWER
    \brief Flag Value. Error while powering in this case
 */
#define	GPRS_ERROR_POWER		1

/*! \def GPRS_ERROR_CALL
    \brief Flag Value. Error while calling in this case
 */
#define	GPRS_ERROR_CALL			2

/*! \def GPRS_ERROR_HANG
    \brief Flag Value. Error while hanging up in this case
 */
#define	GPRS_ERROR_HANG			4

/*! \def GPRS_ERROR_SMS
    \brief Flag Value. Error while sending a SMS in this case
 */
#define	GPRS_ERROR_SMS			8

/*! \def GPRS_ERROR_PIN
    \brief Flag Value. Error while setting PIN in this case
 */
#define	GPRS_ERROR_PIN			16

/*! \def GPRS_ERROR_CALLINFO
    \brief Flag Value. Error while setting call info in this case
 */
#define	GPRS_ERROR_CALLINFO		32

/*! \def GPRS_ERROR_SMSINFO
    \brief Flag Value. Error while setting SMS info in this case
 */
#define	GPRS_ERROR_SMSINFO		64

/*! \def GPRS_ERROR_CONF
    \brief Flag Value. Error while configuring GPRS connection in this case
 */
#define	GPRS_ERROR_CONF			128

/*! \def GPRS_ERROR_PATTERN
    \brief Flag Value. Error while setting pattern in this case
 */
#define GPRS_ERROR_PATTERN		256

/*! \def GPRS_ERROR_TIMER
    \brief Flag Value. Error while setting timer in this case
 */
#define	GPRS_ERROR_TIMER		512

/*! \def GPRS_ERROR_PROFILE
    \brief Flag Value. Error while setting profile in this case
 */
#define	GPRS_ERROR_PROFILE		1024

/*! \def GPRS_ERROR_SOCKET
    \brief Flag Value. Error while opening a socket in this case
 */
#define	GPRS_ERROR_SOCKET		2048

/*! \def GPRS_ERROR_CHECK
    \brief Flag Value. Error while checking connection in this case
 */
#define	GPRS_ERROR_CHECK		4096

/*! \def GPRS_ERROR_CLOSE
    \brief Flag Value. Error while closing a socket in this case
 */
#define	GPRS_ERROR_CLOSE		8192

/*! \def GPRS_ERROR_SMTP
    \brief Flag Value. Error while using SMTP
 */
#define	GPRS_ERROR_SMTP			16384

/*! \def GPRS_ERROR_DELETE
    \brief Flag Value. Error while deleting a socket
 */
#define	GPRS_ERROR_DELETE		32768



/*! \def GPRS_RATE
    \brief GPRS Baud Rate
 */
#define	GPRS_RATE	38400

/*! \def DELAY_ON_SEND
    \brief Constants for AT commands. Delay after every sending attempt in this case
 */
#define DELAY_ON_SEND 1500

/*! \def DEFAULT_TIMEOUT
    \brief Constants for AT commands. Default attempts to try in this case
 */
#define DEFAULT_TIMEOUT 10

/*! \def SEND_ONCE
    \brief Constants for AT commands. Sending only once in this case
 */
#define SEND_ONCE 1

/*! \def PORT_USED
    \brief Constants for AT commands. Port used in AT commands functions in this case
 */
#define PORT_USED  1

/*! \def GPRS_CLIENT
    \brief When opening a socket, it can be opened as client or server. Client in this case
 */
#define	GPRS_CLIENT	0

/*! \def GPRS_SERVER
    \brief When opening a socket, it can be opened as client or server. Server in this case
 */
#define	GPRS_SERVER	1

/*! \def GPRS_DATA_LENGTH
    \brief Maximum data length expressed as a string
 */
#define GPRS_DATA_LENGTH "100"

/*! \def GPRS_DATA_LENGTH_U
    \brief Maximum data length expressed as an integer
 */
#define	GPRS_DATA_LENGTH_U 100

/*! \def GPRS_MAX_DATA
    \brief Maximum data length 
 */
#define	GPRS_MAX_DATA	100

/******************************************************************************
 * Class
 ******************************************************************************/
 
//! WaspGPRS Class
/*!
	WaspGPRS Class defines all the variables and functions used to manage GPRS module
 */
class WaspGPRS
{
	private:

	//! It sets the pattern for using in sending and receiving messages
    	/*!
    	\param const char* pattern : pattern to set
    	\return '1' on success, '0' if error, '-1' if no memory
    	\sa setConnectionTimer(), setConnectionProfile()
    	 */ 
	int8_t setPattern(const char* pattern);
	
	//! It sets the GPRS connection time out
    	/*!
	It gets the timer from 'WaspGPRSconstants.h' file
	\param void
	\return '1' on success, '0' if error, '-1' if no memory
	\sa setPattern(const char* pattern), setConnectionProfile()
	 */ 
	int8_t setConnectionTimer();
	
	//! It sets the GPRS connection profile
    	/*!
	It gets the profile from 'WaspGPRSconstants.h' file
	\param void
	\return '1' on success, '0' if error
	\sa setPattern(const char* pattern), setConnectionTimer()
	 */
	uint8_t setConnectionProfile();
	
	//! It sets GPRS flow control
    	/*!
	It gets the flow control from 'WaspGPRSconstants.h' file
	\param void
	\return '1' on success, '0' if error
	\sa setPattern(const char* pattern), setConnectionTimer()
	 */
	int8_t setFlowControl();
	
	//! It checks if GPRS connection is OK
    	/*!
	\param void
	\return '1' on success, '0' if error
	 */
	uint8_t checkGPRS();
	
	//! It writes data to send via GPRS socket
    	/*!
	\param const char* data : data to send via socket
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t writeData(const char* data);
	
	//! It parses GSM string and specifies if it is a call or a SMS
    	/*!
	\param const char* data : string to parse
	\return '0' on GSM incoming call, '1' on GSM incoming SMS, '2' if error
	 */
	uint8_t parse_GSM(const char* data);
	
	//! It sets the email parameters
    	/*!
	\param char* smtp_server : smtp server address
	\param uint16_t port : smtp server port
	\param char* to : origin or user address
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t setEmailParams(const char* smtp_server, uint16_t port, const char* to);
	
	//! It sets the email user and password
    	/*!
	\param char* user : email user
	\param char* pwd : email password
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t setEmailPwd(const char* user, const char* pwd);
	
	//! It sets the email destination
    	/*!
	\param char* destination : email destination
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t setEmailDestination(const char* destination);
	
	//! It sets the email subject
    	/*!
	\param char* subject : email subject
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t setEmailSubject(const char* subject);
	
	//! It gets if GPRS module is ready or not
    	/*!
	\return nothing. It changes the value of 'not_ready'
	 */
	void getIfReady();
	
	//! Variable : stores if the module is ready or not (1:not ready, 0:ready)
    	/*!
	 */
	uint8_t not_ready;
	
	//! Variable : array for storing part of the data to send to the module
    	/*!
     	*/
	char theCommand[100];
	
	//! Variable : array for storing part of the data to send to the module
    	/*!
	 */
	char theEnd[10];
	
	//! It sends an AT command to the module
    	/*!
	\param char* ATcommand : string to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\return '1' on success, '0' if error
	 */
	byte sendATCommand(const char* ATcommand, const char* expectedAnswer);
	
	//! It sends an AT command to the module
    	/*!
	\param char* ATcommand : string to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int sendOnce : specifies if the data is sent once
	\return '1' on success, '0' if error
	 */
	byte sendATCommand(const char* ATcommand, const char* expectedAnswer, int sendOnce);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param int endOfCommand : integer to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param int endOfCommand : integer to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int MAX_TIMEOUT : specifies the maximum timeout
	\param int sendOnce : specifies if the data is sent once
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param int endOfCommand : integer to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int sendOnce : specifies if the data is sent once
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, int endOfCommand, const char* expectedAnswer, int sendOnce);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char endOfCommand : character to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char endOfCommand : character to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int MAX_TIMEOUT : specifies the maximum timeout	
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char endOfCommand : character to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int MAX_TIMEOUT : specifies the maximum timeout	
	\param int sendOnce : specifies if the data is sent once	
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, char endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char* endOfCommand : string to send to the module
	\param char* expectedAnswer : string expected to be answered by the module	
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char* endOfCommand : string to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int sendOnce : specifies if the data is sent once	
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer, int sendOnce);
	
	//! It sends an AT command to the module
    	/*!
	\param char* theText : string to send to the module
	\param char* endOfCommand : string to send to the module
	\param char* expectedAnswer : string expected to be answered by the module
	\param int MAX_TIMEOUT : specifies the maximum timeout	
	\param int sendOnce : specifies if the data is sent once	
	\return '1' on success, '0' if error
	 */
	byte sendCommand(const char* theText, const char* endOfCommand, const char* expectedAnswer, int MAX_TIMEOUT, int sendOnce);
	
	//! It waits for data from the module
    	/*!
	\param char* expectedAnswer : string expected to be answered by the module
	\param int MAX_TIMEOUT : specifies the maximum timeout	
	\param int timeout : specifies the timeout
	\param int seconds : specifies the number of seconds to wait before listening to the module
	\return '1' on success, '0' if error
	 */
	byte waitForData(const char* expectedAnswer, int MAX_TIMEOUT, int timeout, int seconds);
	
        //! It waits for data from the module
    	/*!
        \param char* data : data where the 'expectedAnswer' has to be found
        \param char* expectedAnswer : string expected to be answered by the module
        \return '1' on success, '0' if error
         */
	uint16_t waitForData(const char* data, const char* expectedAnswer);
	
        //! It sends data via FTP
    	/*!
        \param char* file : file to upload
        \param char* path : path within SD card to find the file to upload
        \param uint8_t id : ftp session id
        \return '1' on success, '0' if error, '-1' if no memory
         */
	int8_t sendDataFTP(const char* file, const char* path, uint8_t id);
	
        //! It reads data via FTP
    	/*!
        \param char* file : file to download
        \param char* path : path within SD card to store the file to download
        \param uint8_t id : ftp session id
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t readDataFTP(const char* file, const char* path, uint8_t id);



    
	public:
		
	//! Variable : status flag, used to see if there was an error while communicating with the module
    	/*!
		Possible values are : GPRS_ERROR_POWER, GPRS_ERROR_CALL, GPRS_ERROR_HANG, GPRS_ERROR_SMS, GPRS_ERROR_PIN, GPRS_ERROR_CALLINFO, GPRS_ERROR_SMSINFO, GPRS_ERROR_CONF, GPRS_ERROR_PATTERN, GPRS_ERROR_TIMER, GPRS_ERROR_PROFILE, GPRS_ERROR_SOCKET, GPRS_ERROR_CHECK, GPRS_ERROR_CLOSE
     	*/
	int   flag;
	
	//! Variable : last call or SMS received telephone number
    	/*!
		It stores telephone numbers with country code if it appears
	 */
	char tlfIN[15];
	
	//! Variable : baud rate used to communicate with the GPRS module
    	/*!
		By default, it is 38400bps
	 */
	uint16_t _baudRate;
	
	//! Variable : power mode
    	/*!
		Possible values are: GPRS_ON, GPRS_SLEEP, GPRS_HIBERNATE
	 */
	uint8_t _pwrMode;
	
	//! Variable : UART used by the GPRS module
    	/*!
		GPRS module is connected to the UART1
	 */
	uint8_t	_uart;
	
	//! Variable : last SMS text received
    	/*!
		It stores up to 100bytes from the last SMS received
	 */
	char sms[100];
	
	//! Variable : socket ID
    	/*!
		It stores the ID of the last socket opened
	 */
	uint8_t socket_ID[4];
	
	//! Variable : data received when reading an URL
    	/*!
		It stores up to 100 bytes from an URL
	 */
	char data_URL[100];
	
	//! Variable : amount of data read from an URL or from a socket
    	/*!
	 */
	uint16_t data_read;
	
	//! Variable : Cell ID number
    	/*!
	 */
	char cellID[4];
      
	//! Variable : RSSI value
    	/*!
	 */
	char RSSI[2];
	
	//! Variable : Array for storing the answer to the command sent
    	/*!
	 */
	char answer_command[50];
	
	//! Variable : Last sms received index
    	/*!
	 */
	char sms_index[4];
	
	//! Variable : Email address from the last email received
    	/*!
	 */
	char emailAddress[31];
	
	//! Variable : Last email received subject
    	/*!
	 */
	char subject[31];
	
	//! Variable : Last email received subject
    	/*!
	 */
	char body[101];
        
        //! Variable : IMSI from the SIM card
    	/*!
         */
        char IMSI[20];
        
        //! Variable : IMEI from the SIM card
    	/*!
         */
        char IMEI[20];
        
        //! Variable : specifies if the GPRS modules is connected to the network (1:connected ; 0:connected)
    	/*!
         */
        uint8_t connected;
	
      	//! class constructor
    	/*!
		It initializes some variables
	\param void
	\return void
     	*/ 
	WaspGPRS();
    
	
	//! It opens UART1 and powers the GPRS module
    	/*!
	\param void
	\return void
	\sa close(), begin()
     	*/ 
	void ON();
	
	//! It closes UART1 and powers off the GPRS module
    	/*!
	\param void
	\return void
	\sa close(), begin()
	 */ 
	void OFF();
	
	//! It opens UART and sets multiplexer properly
    	/*!
	\param void
	\return void
	 */ 
	void begin();
	
	//! It closes UART and sets multiplexer properly
    	/*!
	\param void
	\return void
	 */ 
	void close();
	
	//! It sets Power Mode
    	/*!
	\param uint8_t pwrMode : the desired power mode (GPRS_ON, GPRS_SLEEP or GPRS_HIBERNATE)
	\return '1' on success and '0' if error
	 */
	uint8_t setMode(uint8_t pwrMode);
	
	//! It gets Power Mode
    	/*!
	\param void
	\return the power mode (GPRS_ON, GPRS_SLEEP or GPRS_HIBERNATE)
	 */
	uint8_t getMode();
	
	//! It checks if GPRS is connected to the network
    	/*!
	\param void
	\return '1' if connected, '0' if not, '-1' if no memory
	 */
	int8_t check();
	
	//! It sets PIN
    	/*!
	\param const char* PIN : the SIM's PIN
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t	setPIN(const char* pin);
	
	//! It sets the info shown when an incoming call is received
    	/*!
	\param void
	\return '1' on success, '0' if error
	 */
	uint8_t	setInfoIncomingCall();
	
	//! It sets the info shown when an incoming SMS is received
    	/*!
	\param void
	\return '1' on success, '0' if error
	 */
	uint8_t setInfoIncomingSMS();
	
	//! It sets 'text mode' for sending SMS
    	/*!
	\param void
	\return '1' on success, '0' if error
	 */
	uint8_t setTextModeSMS();
	
	//! It makes a call to the specified number
    	/*!
	\param const char* tlfNumber : telephone number to call. It must be a telephone number with 9 digits.
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t makeCall(const char* tlfNumber);
	
	//! It makes a lost call to the specified number
    	/*!
	\param const char* tlfNumber : telephone number to call. It must be a telephone number with 9 digits.
	\param uint8_t timeCall : time the module is calling (in seconds)
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t makeLostCall(const char* tlfNumber, uint8_t timeCall);
	
	//! It hangs all the active calls up
    	/*!
	\return '1' on success, '0' if error
	 */
	uint8_t hangUp();
	
	//! It sends a SMS to the specified number
    	/*!
	\param const char* smsText : SMS text (up to 80bytes)
	\param const char* tlfNumber : telephone number to call. It must be a telephone number with 9 digits.
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t sendSMS(const char* smsText, const char* tlfNumber);
	
	//! It stores in 'tlfIN' variable the telephone number of the incoming call
    	/*!
	\param char* data : data returned by the module when a call is being received
	\return '1' on success, '0' if error
	 */
	uint8_t	readCall(const char* data);
	
	//! It stores in 'tlfIN' and 'sms' variables the telephone number and the text of the incoming SMS. It stores in 'sms_index' variable the last sms received index
    	/*!
	\param char* data : data returned by the module when an SMS is received
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t readSMS(const char* data);
	
	//! It manages incoming data from serial port, executing proper functions to store received data
    	/*!
	\param void
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t	manageIncomingGSMData();
	
	//! It configures GPRS connection with login, password and some other parameters
    	/*!
	It takes the configuration parameters from 'WaspGPRSconstants.h' file
	\param void
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t configureGPRS();
	
	//! It creates a TCP/IP connection to the specified IP and PORT
    	/*!
	\param const char* ip : the IP to open a socket to
	\param const char* port : the PORT to open a socket to
	\param uint8_t mode : GPRS_CLIENT or GPRS_SERVER
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t createSocket(const char* ip,const char* port,uint8_t mode);
	
	//! It accesses to the specified URL and stores the info read in 'data_URL' variable
    	/*!
	It stores in 'data_URL' variable up to 100 bytes
	\param const char* url : the URL to get the information from
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t readURL(const char* url);
	
	//! It sends 'data' to the specified 'socket'
    	/*!
	\param const char* data : the data to send to the socket
	\param uint8_t socket: the socket's ID to communicate with
	\return '1' on success, '0' if error, '-1' if no memory
	 */
        int8_t sendData(const char* data, uint8_t* socket);
	
	//! It reads data of 'data_length' size from socket ID 'socket'
    	/*!
	\param uint8_t socket: the socket's ID to communicate with
	\param const char* data_length : the data length to read from the socket
	\return '1' on success, '0' if error, '-1' if no memory
	 */
        int8_t readData(uint8_t* socket, const char* data_length);
	
	//! It closes 'socket' TCP/IP connection
    	/*!
	\param uint8_t socket: the socket's ID to close
	\return '1' on success, '0' if error, '-1' if no memory
	 */
        int8_t closeSocket(uint8_t* socket);
	
	//! It deletes 'socket' TCP/IP connection
    	/*!
	\param uint8_t socket: the socket's ID to delete
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t deleteSocket(uint8_t* socket);
	
	//! It sends any command to the GPRS module
    	/*!
	\param char* ATcommand : the command to send to the GPRS module
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t sendCommand(const char* ATcommand);
	
	//! It sends an email to the specified address
    	/*!
	\param char* from : sender address
	\param char* to : receiver address
	\param char* subject : email subject
	\param char* body : text of the email
	\param char* user : login to connect to the smtp server
	\param char* passw : password to connect to the smtp server
	\param char* smtp_server : smtp server
	\param uint16_t port : smtp server port
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t sendMail(const char* from, const char* to, const char* subject,const  char* body, const char* user, const char* passw, const char* smtp_server, uint16_t port);
	
	//! It reads an email from the pop3 server
    	/*!
	\param char* user : login to connect to the smtp server
	\param char* passw : password to connect to the smtp server
	\param char* pop3_server : pop3 server
	\param uint16_t port : pop3 server port
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t readMail(const char* user, const char* passw, const char* pop3_server, uint16_t port);
	
	//! It uploads a file to a FTP server
    	/*!
	\param char* file : file to upload (the name that will have in the FTP server and the name of the file in the SD card)
	\param char* path : path in the SD card where file is stored 
	\param char* user : login to connect to the ftp server
	\param char* passw : password to connect to the ftp server
	\param char* ftp_server : ftp server
	\param uint8_t ftp_port : ftp server port
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t uploadFile(const char* file, const char* path, const char* user, const char* passw, const char* ftp_server, uint8_t ftp_port);
	
	//! It downloads a file from a FTP server
    	/*!
	\param char* file : file to download (the file name in the FTP server)
	\param char* path : path in the SD card where file is going to be stored (it may have a different name from the FTP server file)
	\param char* user : login to connect to the ftp server
	\param char* passw : password to connect to the ftp server
	\param char* ftp_server : ftp server
	\param uint8_t ftp_port : ftp server port	
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t downloadFile(const char* file, const char* path, const char* user, const char* passw, const char* ftp_server, uint8_t ftp_port);
	
	//! It closes the FTP connection
    	/*!
	\return '1' on success, '0' if error, '-1' if no memory
	 */
	int8_t closeFTP();
	
	//! It gets info from the cell where the modules is connected
    	/*!
	It stores in 'RSSI' and 'cellID' variables the RSSI and the Cell ID
	\return '1' if connected, '0' if not, '-1' if no memory
	 */
	int8_t getCellInfo();

        //! It gets the IMSI from the SIM card
    	/*!
        It stores the IMSI into 'IMSI' variable.
	\return '1' if connected, '0' if not, '-1' if no memory
	 */
        int8_t getIMSI();
	
        //! It gets the IMEI from the SIM card
    	/*!
        It stores the IMEI into 'IMEI' variable.
	\return '1' if connected, '0' if not, '-1' if no memory
	 */
        int8_t getIMEI();
	
        //! It resets the GPRS module
    	/*!
	\return '1' if connected, '0' if not, '-1' if no memory
	 */
        int8_t reset();
};

extern WaspGPRS GPRS;

#endif

