/*! \file WaspGPS.h
    \brief Library for managing the GPS A1084 receiver
    
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
  
    Version:		0.6

    Design:		David Gascón

    Implementation:	Alberto Bielsa, David Cuartielles, Mikal Hart

*/

/*! \def WaspGPS_h
    \brief The library flag
    
 */
#ifndef WaspGPS_h
#define WaspGPS_h

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <inttypes.h>

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/
 
/*! \def VERSION
    \brief Version Control

 */
#define VERSION "WaspGPS 01c, (c) 2009 A.Bielsa for Libelium.com\n"

/*! \def GPS_BINARY
    \brief Enables binary mode.

	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

	Setting this option Binary Mode is enabled.
 */
/*! \def GPS_NMEA_GGA
    \brief Enables NMEA -> GGA sentences.

	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA GGA sentences are enabled.
 */
/*! \def GPS_NMEA_GLL
    \brief Enables NMEA -> GLL sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA GLL sentences are enabled.
 */
/*! \def GPS_NMEA_GSA
    \brief Enables NMEA -> GSA sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA GSA sentences are enabled.
    
 */
/*! \def GPS_NMEA_GSV
    \brief Enables NMEA -> GSV sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA GSV sentences are enabled.
    
 */
/*! \def GPS_NMEA_RMC
    \brief Enables NMEA -> RMC sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA RMC sentences are enabled.
    
 */
/*! \def GPS_NMEA_VTG
    \brief Enables NMEA -> VTG sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, only NMEA VTG sentences are enabled.
    
 */
/*! \def GPS_NMEA
    \brief Enables NMEA -> All sentences.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, all NMEA sentences are enabled.
    
 */
/*! \def GPS_BINARY_OFF
    \brief In binary mode, set GPS module in request mode.
    
    	There are differents ways to set communication with GPS module: Binary mode or NMEA mode.

    	Setting this option, GPS module stops sending binary automatic responses. It will send binary commands only when we ask him.
    
 */
#define GPS_BINARY 	1	
#define	GPS_NMEA_GGA	2
#define	GPS_NMEA_GLL	4
#define	GPS_NMEA_GSA	8
#define	GPS_NMEA_GSV	16
#define	GPS_NMEA_RMC	32
#define	GPS_NMEA_VTG	64
#define	GPS_NMEA	128
#define	GPS_BINARY_OFF	256


/*! \def GPS_ON 
    \brief GPS power mode: ON.
    
    	Using GPS_ON makes GPS module to switch on.
    
 */
/*! \def GPS_OFF
    \brief GPS power mode: OFF
   
   	Using GPS_OFF makes GPS module to switch off.
 */
#define GPS_ON 0
#define GPS_OFF 1


/*! \def HOT
    \brief GPS waking up mode: HOT.
    
    	Using HOT makes GPS module to restart on a hot one, keeping ephemeris and almanacs.

	This mode is used by default.
   
 */
/*! \def WARM
    \brief GPS waking up mode: WARM.
    
    	Using WARM makes GPS module to restart on a warm one, keeping almanacs.

 */
/*! \def COLD
    \brief GPS waking up mode: COLD.
    
    	Using COLD makes GPS module to restart on a cold one deleting all the previously stored information.

 */
#define HOT 0
#define WARM 1
#define COLD 2


/*! \def ACK
    \brief Flag value
    
    	Initial value for 'flag'.

 */
/*! \def GPS_TIMEOUT
    \brief Flag value

	The GPS timed out while waiting for a string.
 */
/*! \def GPS_INVALID
    \brief Flag value

	The reading was not valid, satellite data isn't good enough.
 */
/*! \def GPS_ERROR_EPHEMERIS
    \brief Flag value

	Error trying to get an ephemeris string.
 */
/*! \def GPS_ERROR_SAVE_EPHEMERIS
    \brief Flag value

	Error trying to save an ephemeris string.
 */
/*! \def GPS_DATA_ERROR
    \brief Flag value

	Error while parsing e.g. the amount of satellites.
 */
/*! \def GPS_BAD_SENTENCE
    \brief Flag value

	Error, arrival of different sentence than expected.
 */
/*! \def GPS_BAD_CHECKSUM
    \brief Flag value

	Error, data corrupted in some mysterious way.
 */
/*! \def GPS_ERROR_FILE_EPHEMERIS
    \brief Flag value

	Error, couldn't use the ephemeris file.
 */
/*! \def GPS_FILE_EPHEMERIS_EMPTY
    \brief Flag value

	Warning, ephemeris file is empty.
 */
#define ACK 				0
#define GPS_TIMEOUT 			1
#define GPS_INVALID 			2
#define GPS_ERROR_EPHEMERIS 		4
#define GPS_ERROR_SAVE_EPHEMERIS 	8
#define GPS_DATA_ERROR 			16
#define GPS_BAD_SENTENCE 		32
#define GPS_BAD_CHECKSUM 		64
#define GPS_ERROR_FILE_EPHEMERIS  	128
#define GPS_FILE_EPHEMERIS_EMPTY  	256


/*! \def GPS_TIMEOUT_em
    \brief Error message for flag

	String error --> "error: the GPS timed out"
 */
/*! \def GPS_ERROR_EPHEMERIS_em
    \brief Error message for flag

	String error --> "error: couldn't get ephemeris"
 */
/*! \def GPS_ERROR_SAVE_EPHEMERIS_em
    \brief Error message for flag

	String error --> "error: couldn't save ephemeris to SD"
 */
/*! \def GPS_CREATE_FILE_EPHEMERIS_em
    \brief Error message for flag

	String error --> "error: couldn't create ephemeris file"
 */
/*! \def GPS_DATA_ERROR_em
    \brief Error message for flag

	String error --> "error: data not consistent"
 */
/*! \def GPS_BAD_SENTENCE_em
    \brief Error message for flag

	String error --> "error: different sentence than expected"
 */
#define GPS_TIMEOUT_em "error: the GPS timed out"
#define GPS_ERROR_EPHEMERIS_em "error: couldn't get ephemeris"
#define GPS_ERROR_SAVE_EPHEMERIS_em "error: couldn't save ephemeris to SD"
#define GPS_CREATE_FILE_EPHEMERIS_em "error: couldn't create ephemeris file"
#define GPS_DATA_ERROR_em "error: data not consistent"
#define GPS_BAD_SENTENCE_em "error: different sentence than expected"


/*! \def ATTEMPTS_TO_READ
    \brief Values for timings, etc

 */
#define ATTEMPTS_TO_READ 5

/*! \def FILE_EPHEMERIS
    \brief File used to save and load ephemeris
    
    "ephemeris.txt"
 */
#define FILE_EPHEMERIS "ephemeris.txt"


/*! \def GPS_BUFFER_SIZE
    \brief internal 'inBuffer' size, needs to be at least 148 bytes for the ephemeris
 */
#define GPS_BUFFER_SIZE 160

/******************************************************************************
 * Class
 ******************************************************************************/

//! WaspGPS Class
/*!
	WaspGPS Class defines all the variables and functions used to manage GPS A1084 receiver
 */
class WaspGPS
{
  private:
    
    	//! It extracts Date from a NMEA RMC sentence
    	/*!
	\param void
	\return void
	\sa extractTime(), getTime(), getDate()
     	*/ 
	void extractDate(void);
	
	//! It extracts Time from a NMEA GGA sentence
    	/*!
	\param void
	\return void
	\sa extractTime(), getTime(), getDate()
	 */ 
    	void extractTime(void);
    
	//! It extracts a decimal number from a string
    	/*!
	\param char *str: The string which contains the decimal number to extract
	\return the number extracted from the string given as parameter.
	\sa extractTime(), getTime(), getDate(), parse_degrees(char *str), gpsatol(char *str), gpsisdigit(char c)
	 */ 
    	long parse_decimal(char *str);
	
	//! It extracts a degree number from a string
    	/*!
	\param char *str: The string which contains the degree number to extract
	\return the number extracted from the string given as parameter.
	\sa extractTime(), getTime(), getDate(), parse_degrees(char *str), gpsatol(char *str), gpsisdigit(char c)
	 */ 
    	unsigned long parse_degrees(char *str);
	
	//! It extracts a decimal number from a string, getting the integer part of a float number in case
    	/*!
	\param char *str: The string which contains the decimal or float number to extract
	\return the number extracted from the string given as parameter.
	\sa extractTime(), getTime(), getDate(), parse_degrees(char *str), gpsatol(char *str), gpsisdigit(char c)
	 */ 
    	long gpsatol(char *str);
	
	//! It specifies if a char is a number
    	/*!
	\param char c: The char to get if it is a number
	\return TRUE if a number, FALSE if not
	\sa extractTime(), getTime(), getDate(), parse_degrees(char *str), gpsatol(char *str), gpsisdigit(char c)
	 */ 
    	bool gpsisdigit(char c) { return c >= '0' && c <= '9'; };
	
	//! It calculates the NMEA checkSum, leave out $, *, and the checkSum bytes
    	/*!
	\param char *gpsString: the string containing the NMEA sentence to get the checksum from
	\return '1' if checksum is correct, '0' if not
	 */ 
	uint8_t checkSum(const char* gpsString);
	
	//! It calculates the NMEA checkSum, leave out $, *, and the checkSum bytes
    	/*!
	\param char *gpsString: the string containing the NMEA sentence to get the checksum from
	\return '1' if checksum is correct, '0' if not
	\sa setChecksum()
	 */ 
	uint8_t getChecksum(const char* gpsString);
	
	//! It sets checksum to the end of Utils.inBuffer
    	/*!
	\param void	
	\return void 
	\sa init()
	 */ 
	void setChecksum();
	
	//! It calculates checksum for a secuence given as a parameter
    	/*!
	\param uint8_t* buffer: the secuence to get the checksum from
	\return void. It stores in 'checkSUM' variable the result
	\sa saveEphems()
	 */ 
	void getChecksum(uint8_t* buffer);
	
	//! It gets if the last reading of data was valid or not
    	/*!
	\param void
	\return 'TRUE' if valid, 'FALSE' if not
	 */ 
	bool dataValid(void) {return fixValid;};
	
	//! It gets a data string from the GPS
    	/*!
	\param int byteAmount: the number of bytes to read from GPS
	\return char*. It retunrs 'Utils.inBuffer', where the data read from GPS has been parsed and stored
	\sa extractDate(), extractTime(), setCommMode(mode), check(), getLatitude(), getLongitude(), getAltitude(),getSpeed(), getCourse()
	 */ 
	char* getRaw(int);
	
    
  public:

    //! Variable : Baudrate used to init the UART and communicate with GPS receiver
    /*!
    	Possible values are 4800,9600,19200,38400,57600,115200.
    
    	4800 is the only value supported now by the API.
    
    	The reason is GPS receiver always starts at 4800bps, so it would be necessary to open UART at 4800, change speed,
    	close UART and open it again at new speed.
    
    	If a higher speed wants to be used, previously explained reason has to be managed properly.
     */ 
    long _baudRate;
    
    //! Variable : UART used to communicate with GPS receiver
    /*!
    	Possible values are: 0 or 1.
    
    	UART used now to communicate with GPS is UART1, so '1' must be set.
     */ 
    uint8_t _uart;
    
    //! Variable : Determines whether the last reading was valid or not
    /*!
    	TRUE if the last reading is valid.
    
    	FALSE if the last reading is not valid.
     */ 
    bool fixValid;
    
    //! Variable : Flag used to determine if an error occurred while executing some function
    /*!
    	Possible values are related below.
    
    \sa ACK, GPS_TIMEOUT, GPS_ERROR_EPHEMERIS, GPS_ERROR_SAVE_EPHEMERIS, GPS_DATA_ERROR, GPS_BAD_SENTENCE,
    GPS_BAD_CHECKSUM, GPS_ERROR_FILE_EPHEMERIS, GPS_FILE_EPHEMERIS_EMPTY, GPS_ERROR_FILE_ALMANAC,
    GPS_FILE_ALMANAC_EMPTY, GPS_ERROR_SAVE_ALMANAC
     */ 
    uint16_t flag;
    
    //! Variable : Communication mode. Two possibilities available --> binary mode or NMEA mode.
    /*!
    	Possible values are related below.
    
    \sa GPS_BINARY, GPS_NMEA_GGA, GPS_NMEA_GLL, GPS_NMEA_GSA, GPS_NMEA_GSV, GPS_NMEA_RMC, GPS_NMEA_VTG, GPS_NMEA,
    	GPS_BINARY_OFF
     */ 
    uint16_t commMode;
    
    //! Variable : Power mode used for GPS receiver
    /*!
    	Possible values are related below.
    
    \sa	 GPS_ON, GPS_OFF, GPS_SLEEP, GPS_HIBERNATE
     */ 
    uint8_t pwrMode;
    
    //! Variable : Waking up mode used for GPS receiver
    /*!
    
    	Possible values are related below.
    
    \sa HOT, WARM, COLD
     */ 
    uint8_t wakeMode;
    
    //! Variable : Time extracted from NMEA GGA sentence received from GPS receiver
    /*!
    	String that contains the time extracted from NMEA GGA sentence.
    
    	To store the time 13:30:00, it is stored in this way: '133000'
     */ 
    char timeGPS[MAX_ARGS];
    
    //! Variable : Date extracted from NMEA RMC sentence received from GPS receiver
    /*!
    	String that contains the date extracted from NMEA RMC sentence.
    
    	To store the date 23rd of March 1994, it is stored in this way: '230394'
     */ 
    char dateGPS[MAX_ARGS];
    
    //! Variable : Clock drift of the GPS receiver used to initialize it
    /*!
    	Value in Hz. Use '0' to use the last value saved, if not available 96250Hz will be used.
     */ 
    char* clkOffset;
    
    //! Variable : GPS Time of Week used to initialize it
    /*!
    	Time of week specified in seconds.
     */ 
    char* timeOfWeek;
    
    //! Variable : Extended GPS Week Number used to initialize it
    /*!
     */ 
    char* weekNo;
    
    //! Variable : Channel Counter used to initialize it
    /*!
     */ 
    char* channel;
    
    //! Variable : Reset Configuration
    /*!
     */ 
    char* resetCfg;
    
    //! Variable : Default Latitude. + = North (Range 90 to -90). Expressed in degrees. Example-->37.3875111
    /*!
     */ 
    char* coordinateLat;
    
    //! Variable : Default Longitude. + = East (Range 180 to -180). Expressed in degrees. Example-->-121.97232
    /*!
     */ 
    char* coordinateLon;
    
    //! Variable : Default Altitude position. Expressed in meters. Example-->0
    /*!
     */ 
    char* coordinateAl;
    
    //! Variable : the frame checksum
    /*!
     */ 
    uint8_t checksum;
    
    //! Variable : it stores the latitude given by the GPS module
    /*!
     */ 
    char latitude[MAX_ARGS];
    
    //! Variable : it stores the longitude given by the GPS module
    /*!
     */ 
    char longitude[MAX_ARGS];
    
    //! Variable : it stores the speed given by the GPS module
    /*!
     */ 
    char speed[MAX_ARGS];
    
    //! Variable : it stores the altitude given by the GPS module
    /*!
     */ 
    char altitude[MAX_ARGS];
    
    //! Variable : it stores the course given by the GPS module
    /*!
     */ 
    char course[MAX_ARGS];
    
    //! Variable : for rebooting the GPS module
    /*!
     */ 
    uint8_t reboot;
    
    //! Variable : for storing the frame checksum
    /*!
     */ 
    uint8_t checkSUM[2];

    //! Variable : buffer to store incoming data
    /*!
    */ 
    char inBuffer[GPS_BUFFER_SIZE];


    
    //! class constructor
    /*!
    It initializes the variables with the default values.
    \param void
    \return void
    */ 
    WaspGPS();
    
    //! It opens UART1 and powers the GPS module
    /*!
    \param void
    \return void
    \sa close(), begin()
    */ 
    void ON();
    
    //! It closes UART1 and powers off the GPS module
    /*!
    \param void
    \return void
    \sa close(), begin()
     */ 
    void OFF();
    
    //! It powers up GPS module and opens UART1 to communicate with it
    /*!
    \param void
    \return void
    \sa init()
    */ 
    void begin(void);
    
    //! It closes UART1
    /*!
    \param void
    \return void
     */ 
    void close(void);
    
    //! It initializes GPS module with the default values
    /*!
    \param void
    \return void
    \sa init(const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*)
     */ 
    void init(void);
    
    //! It initializes GPS module with the values introduced as parameters
    /*!
    \param const char* _coordinateLat : Latitude + = North (Range 90 to -90). Expressed in degrees. Example-->37.3875111
    \param const char* _coordinateLon : Longitude + = East (Range 180 to -180). Expressed in degrees. Example-->-121.97232
    \param const char* _coordinateAl : Altitude position. Expressed in meters. Example-->0
    \param const char* _clkOffset : Clock Drift of the Receiver. Expressed in Hz. Example-->96000
    \param const char* _timeOfWeek : GPS Time Of Week. Expressed in seconds. Example-->237759
    \param const char* _weekNo : Extended GPS Week Number. Example-->1946
    \param const char* _channel : Range 1 to 12
    \param const char* _resetCfg : Reset Configuration Bit
    \return void
    \sa init()
     */ 
    void init(const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*);
    
    //! It sets the current internal Power Mode on the GPS
    /*!
    \param uint8_t mode : power mode to set the GPS. Possible values are GPS_ON, GPS_OFF
    \return void
    \sa getMode()
     */ 
    void setMode(uint8_t);
    
    //! It gets the current internal Power Mode on the GPS
    /*!
    \param void
    \return 'pwrMode' variable that contains the power mode on use
    \sa setMode()
     */ 
    uint8_t getMode(void);

    //! It checks if the receiver is connected to sattelites
    /*!
    \param void
    \return '1' if connected, '0' if not.
    \sa init()
     */ 
    bool check();

    //! It gets the current time given by GPS
    /*!
    \param void
    \return the currect time in a string with the following structure--> "hhmmss.mmmm"
    \sa getDate(), extractTime(), extractDate()
     */ 
    char* getTime(void);
    
    //! It gets the current date given by GPS
    /*!
    \param void
    \return the currect date in a string with the following structure--> "ddmmyy"
    \sa getDate(), extractTime(), extractDate()
    */ 
    char* getDate(void);
    
    //! It gets the Latitude from the GPS
    /*!
    \param void
    \return the Latitude in a string, expressed in degrees
    \sa init()
     */ 
    char* getLatitude(void);
    
    //! It gets the Longitude from the GPS
    /*!
    \param void
    \return the Longitude in a string, expressed in degrees
    \sa init()
     */ 
    char* getLongitude(void);
    
    //! It gets the Speed from the GPS
    /*!
    \param void
    \return the Speed in a string, expressed in kilometers per hour
    \sa init()
     */ 
    char* getSpeed(void);
    
    //! It gets the Altitude from the GPS
    /*!
    \param void
    \return the Altitude in a string, expressed in meters
    \sa init()
     */ 
    char* getAltitude(void);
    
    //! It gets the Course from the GPS
    /*!
    \param void
    \return the True Track made good, expressed in degrees
    \sa init()
     */ 
    char* getCourse(void);
    
    //! It saves ephemeris into a file in the SD card. This file is called 'FILE_EPHEMERIS'.
    /*!
    \param void
    \return '2' when no ephemeris are returned by GPS receiver, '0' when error on writing and '1' on succesful.
    \sa saveEphems(const char* filemane)
     */ 
    int8_t saveEphems();
    
    //! It saves ephemeris into a file in the SD card
    /*!
    \param const char* filename : The file name to store ephemeris in
    \return '2' when no ephemeris are returned by GPS receiver, '0' when error on writing and '1' on succesful.
    \sa saveEphems()
     */ 
    int8_t saveEphems(const char* filename);
    
    //! It loads ephemeris from the SD card to the GPS receiver. It loads from file called 'FILE_EPHEMERIS'
    /*!
    \param void
    \return '1' on success and '0' on error.
    \sa loadEphems(const char* filename)
     */ 
    int8_t loadEphems();
    
    //! It loads ephemeris from the SD card to the GPS receiver
    /*!
    \param const char* filename : The file name to get the ephemeris from
    \return '1' on success and '0' on error.
    \sa loadEphems()
     */ 
    int8_t loadEphems(const char* filename);

    //! It gets the library version
    /*!
    \param void
    \return const char*. The GPS library version.
    \sa init()
     */ 
    const char* getLibVersion(void) {return VERSION;};
    
    //! It sets communication mode: Binary or NMEA
    /*!
    \param uint16_t mode : the communication mode. Possible values are GPS_BINARY, GPS_NMEA_GGA, GPS_NMEA_GLL, GPS_NMEA_GSA, GPS_NMEA_GSV, GPS_NMEA_RMC, GPS_NMEA_VTG, GPS_NMEA, GPS_BINARY_OFF
    \return '1' on succes, '0' if not
    \sa getCommMode()
     */ 
    uint8_t setCommMode(uint16_t);
    
    //! It gets the communication mode on use
    /*!
    \param void
    \return 'commMode' variable that contains the communication mode on use
    \sa setCommMode()
     */ 
    uint8_t getCommMode(void);
    
    //! It gets the latitude, longitude, altitude, speed, course, time and date
    /*!
    \param void
    \return '1' if success, '0' if error
     */ 
    uint8_t getPosition();

    //! It clears the inBuffer array
    /*!
    \param void
    \return void
     */
    void clearBuffer(void);


};

extern WaspGPS GPS;

#endif

