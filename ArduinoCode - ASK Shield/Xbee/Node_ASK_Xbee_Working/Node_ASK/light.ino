/* ASK Shield - Phototransistor */

float getLight(int lightPin) {
  return map(getAverage(lightPin), 0, 1023, 0, 100);
}
