/*********************************************/
/* ASK Shield - TEST AND CALIBRATION SENSORS */
/*********************************************/

#define vref 4.95 // For the computeVolts(); funtcion.

#define micpin A0
#define lightpin A1
#define temppin A2
#define tgspin A3
#define humpin A4 // Pacthed to ENERGY.
#define micspin A5
#define figaroCO2Heater  5 // Heat VCC
#define figaroCOcircuit  6 // Sensing VCC


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
  
  Serial.begin(9600);

  // CALIBRATION OF SOUND SENSOR ON START
  //calSound();

}

void loop() 
{
   
  Serial.print("Light->");
  Serial.print( getLight(lightpin, 100) );
  Serial.print(" Hum->");
  Serial.print( getHumidity(humpin, 10) );
  Serial.print(" Temp->");
  Serial.print( getTemp(temppin, 100) );
  Serial.print(" DB->");
  Serial.println( getSound(micpin) );
  Serial.print(" CO2->");
  //Serial.println( getTgs4161(tgspin) );
  Serial.println();
  
}

