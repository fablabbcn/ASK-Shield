float computeVolts(int rawADC) {
  return (rawADC*vref)/1024.0;
}

float computeAdc(float volts) {
  return (volts*1023.0)/vref;
}

int getAverage(int sensorPin, int avgNum, int del) {
  int i=0;
  long sum = 0;
  unsigned int value = 0;
  unsigned int result = 0;

  for(i=0 ; i<avgNum ; i++)
  {
    value = analogRead(sensorPin);
    sum = sum + value;
    delayMicroseconds(del);
  }
  result = sum/avgNum;
  return(result);


}


int getPeak(int peakPin, int peakNum, int del)
{
  int i=0;
  unsigned int value = 0;
  unsigned int valueMax = 0;
  unsigned int result = 0;

  for(i=0 ; i<peakNum ; i++)
  {
    value = analogRead(peakPin);

    // record the maximum sensor value
    if (value > valueMax) 
      valueMax = value;

    delay(del);

  }
  return(valueMax);
}

float computeFscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}





