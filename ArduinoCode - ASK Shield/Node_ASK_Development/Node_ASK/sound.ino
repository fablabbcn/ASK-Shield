#define alpha 0.98

int soundPeak = 0;
int soundAvg = 0;

double smoothValue = 0;
double prevsmoothValue = 0;
double lastValue = 0;
double newValue = 0;

int dbValue = 0;

int getSound(int micPin)
{

  prevsmoothValue = smoothValue;

  newValue = analogRead(micpin); // take measurement here  
  
  if (newValue > 550) newValue = (newValue-550)*2;
  else newValue = (550-newValue)*2;

  smoothValue = alpha * lastValue + (1 - alpha) * newValue;

  if(smoothValue < 0)  smoothValue = 0;

  lastValue = smoothValue;

  dbValue = fscale( 50, 800, 35, 92, smoothValue, 5.5); // logarithm response mapping!!!

  return dbValue;

}



