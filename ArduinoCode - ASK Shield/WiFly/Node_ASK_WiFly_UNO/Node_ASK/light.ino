/* ASK Shield - LDR */

int getLight(byte lightPin, int sensorReadings) {
  
  return map(getAverage(lightPin, sensorReadings, 1), 0, 1023, 0, 100);

}
