/* ASK Shield - Humidity 808H5V5 */

int getHumidity(byte humidityPin, int sensorReadings) 
{
  // return 32.117*computeVolts(rawHumidity)-26.342;
  // return (computeVolts(getAverage(humidityPin, sensorreadings, 1))-0.8)/0.031;
  float voltsHumidity = computeVolts(getAverage(humidityPin, sensorReadings, 1));
  return int(0.6342*voltsHumidity*voltsHumidity + 29.139*voltsHumidity - 23.381);   
}


