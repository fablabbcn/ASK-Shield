/* ASK Shield - Tgs2442 */

int getTgs2442(int tgsPin, int sensorReadings) { 
  digitalWrite(figarocircuit, HIGH); // turn off sensing VCC
  analogWrite(figaroheater, 245); // turn on heating (recommended 4.8 volts)
  delay(14);
  analogWrite(figaroheater, 0); // turn off heating
  delay(981);
  digitalWrite(figarocircuit, LOW);// turn on sensing VCC
  delay(3);
  int figaroCOVal = getAverage(tgsPin, sensorReadings); // take average measurement here
  delay(2);
  return figaroCOVal;
}

