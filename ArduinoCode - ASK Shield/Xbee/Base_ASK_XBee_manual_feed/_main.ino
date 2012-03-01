#include <SPI.h>               //http://arduino.cc/en/Reference/Libraries
#include <Ethernet.h>          //http://arduino.cc/en/Reference/Libraries
#include <XBee.h>              //http://code.google.com/p/xbee-arduino/

#define manualFeedID 42123

#define sdPin 4                //SD card select on the Ethernet shield
#define blinkPin 8             //heartbeat LED (can't use pin 13, the Ethernet shield uses it)
                               //the heartbeat LED just blinks so we know the sketch is running
#define XBEE_PAYLOAD_LEN 40    //this should agree with the definition in the remote code

/*----------------------------------------------------------------------*
 * SET THE FOLLOWING VARIABLES FOR YOUR NETWORK AND PACHUBE ACCOUNT!!!  *
 *                                                                      *
 * o The MAC address should be on a sticker on the bottom of the        *
 *   Ethernet shield.                                                   *
 * o The IP address is determined by your network, router, etc. My      *
 *   router doesn't generate DHCP addresses > 192.168.0.199, so the     *
 *   address below is safe *for me* ... YMMV!                           *
 * o Click "My API Keys" on the Pachube web site for your API key.      *
 *----------------------------------------------------------------------*/
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x4A, 0x02};
//byte ip[] = {192, 168, 2, 190}; // Changed to DHCP
char PachubeApiKey[] = "nKn8ldoeUYHFdcLymuWAAm6KfblczFJFCIc8GT2_G30";

//other global variables
unsigned long msNow, msLast;        //for timing transmissions, etc.
byte server[] = {173, 203, 98, 29}; //www.pachube.com server
//Client client(server, 80);          //http client Changed for IDE 1.0
EthernetClient client;
XBee xbee = XBee();                 //XBee object
union {byte B; char C;} xbeePayload[XBEE_PAYLOAD_LEN];    //XBee data payload
ZBRxResponse rx = ZBRxResponse();   //XBee receive response
ModemStatusResponse msr = ModemStatusResponse();    //XBee modem status response

void setup(void) {
    pinMode(sdPin, OUTPUT);
    digitalWrite(sdPin, HIGH);		//not using sd card on ethernet shield, so deselect it
    pinMode(blinkPin, OUTPUT);
    Ethernet.begin(mac);
    Serial.begin(9600);
}

void loop(void) {
    static boolean blink;

    msNow = millis();                            //get the current "time" in milliseconds
    readXBee();                                  //watch for incoming sensor transmissions
    if (msNow > msLast + 1000) {                 //once through here per second
        msLast = msNow;
        digitalWrite(blinkPin, blink = !blink);  //blink the heartbeat LED
    }    
}

