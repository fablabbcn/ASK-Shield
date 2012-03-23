/* ASK Shield - Sound Level ELECTREC CAPSULE */

/*
TO DO:
Implementation of DBspl equation:
20*log(P1/P0) 
http://es.wikipedia.org/wiki/Decibelio
http://es.wikipedia.org/wiki/Nivel_de_presi%C3%B3n_sonora
*/

//SMOTHING FILTER CONSTANT
#define alphaSound 0.98

//LIMITS FOR CALIBRATION SENSOR
#define minSound  4
#define maxSound  150
int soundSilence = 530;

//SERIAL DEBUGING
#define debugSound 0

int dbValue;
double smoothValue;
double smoothDbValue;
double lastValue;
double lastDbValue;
double newValue;


void calSound()
{
  soundSilence = getAverage(micpin, 1000, 2);
  Serial.print("sound calibration: ");
  Serial.println(soundSilence); 
 
}


int getSound(byte micPin)
{

  for(int i = 0; i<500 ; i++) // to ensure the filter is stable
  {   
    newValue = analogRead(micpin); // take measurement here  
    //newValue = getAverage(micpin, 10, 25); // take measurement here every 25us (40khz)

    if (newValue > soundSilence) newValue = (newValue-soundSilence)*2;
    else newValue = (soundSilence-newValue)*2;

    smoothValue = alphaSound * lastValue + (1 - alphaSound) * newValue;
    if(smoothValue < 0)  smoothValue = 0;
    lastValue = smoothValue;

    //dbValue = 20*log10(smoothValue*smoothValue*2); //NOT WORKING IN THE CORRECT SCALE
    if(smoothValue > maxSound)
      dbValue = map(smoothValue, maxSound, 600, 93, 99);
    else
      dbValue = computeFscale( minSound, maxSound, 45, 93, smoothValue, 7.5); // logarithm response mapping - 5.5!!!
    
      
    smoothDbValue = alphaSound * smoothDbValue + (1 - alphaSound) * dbValue;
    //lastDbValue = smoothDbValue;
    
    if(debugSound)
    {
      Serial.print("sensor RAW: ");
      Serial.print(newValue);
      Serial.print("  sensor smooth: ");
      Serial.print((int)smoothValue);
      Serial.print("  DB ");
      Serial.println(dbValue);
    }
  }

  return smoothDbValue;

}



