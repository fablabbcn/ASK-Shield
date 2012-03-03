/* ASK Shield - CO2 TGS4161 */

/* Linearization equation from Libelium Forum (All values in millivolts)
 y = 62.877*log(x) - 158.631 
 x = 10^( (y + 158.631)/62.877 )
 x= pow(10, ((y + 158,631)/62,877));
 where x: ppm concentration and y: DELTA(EMF) */

//SMOTHING FILTER CONSTANT
#define alphaTgs 0.95

//TIME FOR HEATING BEFORE MEASURE
#define heatTime 30000

//CALIBRATION FOR EVERY CO2 SENSOR
#define TGS4161_350  0.38 

//SERIAL DEBUGING
#define debugCO2 0

int figaroCO2Val = 0;
float ppm = 0;
float ppm_last = 0;
float incEmc = 0;

unsigned long currentMillis = 0; 
unsigned long previousMillis = 0;


int getTgs4161(byte tgsPin)
{

  //digitalWrite(figaroCircuit, LOW); // disconect R12 of the circuit (only for tgs2442)
  digitalWrite(figaroCO2Heater, LOW); // turn ON heater VCC 
  
  currentMillis = millis();
  previousMillis = currentMillis;
  
  while(currentMillis - previousMillis < heatTime) { //read during heater Time

    currentMillis = millis();   
    //Serial.println(currentMillis - previousMillis);
        
    figaroCO2Val = getAverage(tgsPin, 32, 1000); // take average measurement here every 1ms

    incEmc = computeVolts((computeAdc(TGS4161_350) - figaroCO2Val))*1000 ; 
    if(incEmc < 0)
      incEmc = 0;

    ppm = pow(10, ((incEmc+158.631)/62.877)) ;
    if(ppm > 10000)
      ppm = 10000;

    ppm = (alphaTgs * ppm_last  + (1 - alphaTgs) * ppm); //smoothing
    ppm_last = ppm;

    if(debugCO2)
    {
    Serial.print("RAW CO2: ");
    Serial.print(computeVolts(figaroCO2Val));
    Serial.print("  AEMC: ");
    Serial.print( incEmc );
    Serial.print("  PPM: ");
    Serial.println( ppm );
    }
  }

  digitalWrite(figaroCO2Heater, HIGH); // turn OFF heater VCC 
  
  //Serial.print("Final PPM: ");
  //Serial.println( ppm );
  
  return((int)ppm);

}


