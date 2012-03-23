

/* Debugging! */

#define debuggingsensors false // if true turns off xbee and debuggs the sensors in serial
#define logsd false // if true turns off xbee and debuggs the sensors in serial


/* Settings */

#define blinkpin 13  //heartbeat LED must change. Use it by the GPS.

// SENSOR DEF

#define vref 4.95 // For the computeVolts(); funtcion.

#define micpin A0
#define lightpin A1
#define temppin A2
#define tgspin A3
#define humpin A4 // Pacthed to ENERGY.
#define micspin A5
#define figaroCO2Heater  5 // Heat VCC
#define figaroCOcircuit  6 // Sensing VCC

#define readsensors 60000 // timer in milisecons for data send

//other global variables
unsigned long msNow = 0;
unsigned long msLast = 0;     //for timing transmissions, etc.

int temp;
int humidity;
int light;
int sound;
int tgs2442;
int tgs4161;


void setup() {

  pinMode(micpin, INPUT);             
  pinMode(lightpin, INPUT);             
  pinMode(temppin, INPUT);            
  pinMode(tgspin, INPUT);  
  pinMode(humpin, INPUT);    
  pinMode(micspin, INPUT);        
  pinMode(figaroCO2Heater, OUTPUT);
  pinMode(figaroCOcircuit, OUTPUT);
  digitalWrite(figaroCO2Heater, HIGH); // turn OFF heater VCC TGS4161

  if (debuggingsensors == false) {                  
    setupWiFly();
  } 
  else {
    Serial.begin(9600);               //does Serial.begin()
  }

  if (logsd == true){
    setupSD();
  }

}




void loop() {

  static boolean blink;

  msNow = millis();

  if (msNow > msLast + readsensors || debuggingsensors == true) {                 //defined at readsensors

    msLast = msNow;

    temp = getTemp(temppin, 100); 
    humidity = getHum(humpin, 10); 
    light = getLight(lightpin, 100); 
    sound = getSound(micpin); 
    tgs4161 = getTgs4161(tgspin); // wait for 30s minimum

    if (logsd == true){
      logSD();
    }

    if (debuggingsensors == true) { 
      Serial.print("Temp: ");      
      Serial.print(temp);   
      Serial.print(" ");              
      Serial.print("Hum: ");      
      Serial.print(humidity);
      Serial.print(" ");                
      Serial.print("Light: ");      
      Serial.print(light);   
      Serial.print(" ");             
      Serial.print("Sound: ");      
      Serial.print(sound); 
      Serial.print(" ");                 
      Serial.print("Tgs2442: ");      
      Serial.print(tgs2442);  
      Serial.print(" ");              
      Serial.print("tgs4161: ");      
      Serial.println(tgs4161);        
    }
    else {                  
      txWiFly();
    }

    digitalWrite(blinkpin, blink = !blink);  //blink the heartbeat LED

  }

  // Detection of wifi desconection using ping function!!!!! if yes, reconnect again!

}


/* Sensor global methods are now on the methods tab */












