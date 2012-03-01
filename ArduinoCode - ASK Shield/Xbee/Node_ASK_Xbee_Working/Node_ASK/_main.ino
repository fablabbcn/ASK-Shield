#include <XBee.h>                //http://code.google.com/p/xbee-arduino/
#include <math.h>

#define blinkPin 13  //heartbeat LED must change. Use it by the GPS.

#define vref 4.99 // For the computeVolts(); funtcion.

#define micpin A0
#define lightpin A1
#define temppin A2
#define tgspin A3
#define humpin A4 // Pacthed to ENERGY.
#define micspin A5

#define sensorreadings 500


/*----------------------------------------------------------------------*
 * SET THE FOLLOWING VARIABLES FOR YOUR PACHUBE ACCOUNT AND XBee!!!     *
 *                                                                      *
 * o Pachube feed number: See the site.                                 *
 * o XBee network coordinator address: Read it out using Digi's         *
 *   X-CTU configuration program, it's also on the bottom of the XBee.  *
 *----------------------------------------------------------------------*/

#define PACHUBE_FEED_NBR 25978  //Pachube feed number to send the data to
XBeeAddress64 coordAddr = XBeeAddress64(0x0013A200, 0x407A1FB0);  //Network coordinator forwards data to Pachube

//other global variables
unsigned long msNow, msLast;     //for timing transmissions, etc.

// NEW
int temp;
int humidity;
int light;

//XBee global variables
#define XBEE_PAYLOAD_LEN 40      //this should agree with the definition in the remote code
XBee xbee = XBee();              //create the XBee object
union {
  byte B; 
  char C;
} 

xbeePayload[XBEE_PAYLOAD_LEN];
ZBTxRequest zbTx = ZBTxRequest();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

void setup() {
  pinMode(blinkPin, OUTPUT);

  pinMode(micpin, INPUT);             
  pinMode(lightpin, INPUT);             
  pinMode(temppin, INPUT);            
  pinMode(tgspin, INPUT);  
  pinMode(humpin, INPUT);    
  pinMode(micspin, INPUT);        
  xbee.begin(9600);               //does Serial.begin()
  delay(5000);                    //give the XBee time to associate
}

void loop(void) {
  static boolean blink;
  static int seconds;             //counts seconds so we can transmit once a minute

  msNow = millis();
  readXBee();                                  //check for incoming XBee traffic
  if (msNow > msLast + 1000) {                 //once through here per second
    msLast = msNow;

    temp = getTemp(temppin); 
    humidity = getHumidity(humpin); 
    light = getLight(lightpin); 

//    temp = 999; 
//    humidity = 777; 
//    light = 888; 

    if (seconds++ == 30) {                   //transmit pachube data every 60 sec
      seconds = 0;
      txPachube();
    }

    digitalWrite(blinkPin, blink = !blink);  //blink the heartbeat LED
  }
}

float computeVolts(int rawADC) {
  return (rawADC*vref)/1024.0;
}

int getAverage(int sensorPin) {
  float value = 0;
  for (int i = 0 ; i < sensorreadings; i++) {
    value += analogRead(sensorPin);
  }
  return int(value/sensorreadings);
}



