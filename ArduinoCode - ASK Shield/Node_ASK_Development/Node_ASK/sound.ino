int getSound(int micPin, int sensorReadings) {
  return getAverage(micPin, sensorReadings, 1);
}

//
//
//#define alpha 0.98
//
//int soundPeak = 0;
//int soundAvg = 0;
//
//double smoothValue = 0;
//double prevsmoothValue = 0;
//double lastValue = 0;
//double newValue = 0;
//
//void getSound()
//{
//
//  //sound = analogRead(micpin); // take measurement here
//  //soundAvg = average(micpin, 10, 100);
//  //soundPeak = peak(micpin, 10, 10);
//  
//  prevsmoothValue = smoothValue;
//  
//  newValue = analogRead(micpin); // take measurement here  
//
//  if (newValue > 550) newValue = (newValue-550)*2;
//  else newValue = (550-newValue)*2;
//  
//  smoothValue = alpha * lastValue + (1 - alpha) * newValue;
//  
//  if(smoothValue < 0)  smoothValue = 0;
//  
//  lastValue = smoothValue;
//  
//  dbValue = fscale( 40, 800, 35, 92, smoothValue, 5.5); // logarithm response mapping!!!
//  
//  /*
//  if(((int)prevsmoothValue) != ((int)smoothValue))
//  {
//    Serial.print("sensor smooth: ");
//    Serial.print((int)smoothValue);
//    Serial.print("  DB ");
//    Serial.println(dbValue);
//  }
//*/
//
//}
//
//
//float fscale( float originalMin, float originalMax, float newBegin, float
//newEnd, float inputValue, float curve){
//
//  float OriginalRange = 0;
//  float NewRange = 0;
//  float zeroRefCurVal = 0;
//  float normalizedCurVal = 0;
//  float rangedValue = 0;
//  boolean invFlag = 0;
//
//
//  // condition curve parameter
//  // limit range
//
//  if (curve > 10) curve = 10;
//  if (curve < -10) curve = -10;
//
//  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
//  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function
//
//  /*
//   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
//   Serial.println(); 
//   */
//
//  // Check for out of range inputValues
//  if (inputValue < originalMin) {
//    inputValue = originalMin;
//  }
//  if (inputValue > originalMax) {
//    inputValue = originalMax;
//  }
//
//  // Zero Refference the values
//  OriginalRange = originalMax - originalMin;
//
//  if (newEnd > newBegin){ 
//    NewRange = newEnd - newBegin;
//  }
//  else
//  {
//    NewRange = newBegin - newEnd; 
//    invFlag = 1;
//  }
//
//  zeroRefCurVal = inputValue - originalMin;
//  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float
//
//  /*
//  Serial.print(OriginalRange, DEC);  
//   Serial.print("   ");  
//   Serial.print(NewRange, DEC);  
//   Serial.print("   ");  
//   Serial.println(zeroRefCurVal, DEC);  
//   Serial.println();  
//   */
//
//  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
//  if (originalMin > originalMax ) {
//    return 0;
//  }
//
//  if (invFlag == 0){
//    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
//
//  }
//  else     // invert the ranges
//  {   
//    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
//  }
//
//  return rangedValue;
//}
//
//

