/* ASK Shield - Phototransistor */

float getLight(int lightPin, int sensorReadings) {
  return map(getAverage(lightPin, sensorreadings, 1), 0, 1023, 0, 100);
}
