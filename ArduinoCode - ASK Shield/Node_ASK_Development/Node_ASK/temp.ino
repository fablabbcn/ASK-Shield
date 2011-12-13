/* ASK Shield - Temperature MCP9700 */

int getTemp (int tempPin, int sensorReadings) {
  return (computeVolts(getAverage(tempPin, sensorreadings))-0.5)*100;
}


