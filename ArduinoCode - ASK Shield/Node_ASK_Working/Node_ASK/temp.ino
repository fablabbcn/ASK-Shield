/* ASK Shield - Temperature MCP9700 */

int getTemp (int tempPin) {
  return (computeVolts(getAverage(tempPin))-0.5)*100;
}


