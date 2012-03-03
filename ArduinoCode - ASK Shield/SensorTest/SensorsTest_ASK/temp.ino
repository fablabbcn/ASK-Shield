/* ASK Shield - Temperature MCP9700 */

int getTemp (byte tempPin, int sensorReadings) {
  return (computeVolts(getAverage(tempPin, sensorReadings, 10))-0.5)*100;
}
