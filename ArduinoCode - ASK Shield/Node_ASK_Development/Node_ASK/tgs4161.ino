/* ASK Shield - Tgs4161 */

//CALIBRATION FOR EVERY SENSOR

//#define TGS4161_350  0.29 // victor
//#define TGS4161_350  0.09
#define TGS4161_350  0.31

//#define TGS4161_350  0.32

int figaroCO2Val = 0;
float figaroCO2Log = 0;
int figaroCOVal = 0;
float figaroCO2Log_last=0;
float incEmc = 0;

float getTgs4161(int tgsPin)
{

  digitalWrite(figarocircuit, LOW); // disconect R12 of the circuit (only for tgs2442)
  digitalWrite(figaroheater, LOW); // turn ON heater VCC 


  figaroCO2Val = getAverage(tgspin, 32, 200); // take average measurement here
  //figaroCO2Val = analogRead(tgspin);
  incEmc = computeAdc(TGS4161_350) - figaroCO2Val;

  //CALIBRACION CON RESPECTO AL VOLTAJE DE 350PPM EN CONDICIONES NORMALES 

  //figaroCO2Log = computeFscale( 0, 19, 350, 10000, computeAdc(0.0), -3.8); // 10 in RAW is 91,2mV in the scale

  figaroCO2Log = computeFscale( 0, 19, 350, 10000, incEmc , -3.8); // 1 in RAW is 91,2mV in the scale
  figaroCO2Log = (alpha * figaroCO2Log_last  + (1 - alpha) * figaroCO2Log); //smoothing
  figaroCO2Log_last = figaroCO2Log;

  return figaroCO2Log;

}


