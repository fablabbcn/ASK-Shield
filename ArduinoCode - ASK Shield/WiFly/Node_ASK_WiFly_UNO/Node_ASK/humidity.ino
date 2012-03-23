/* ASK Shield - Humidity 808H5V5 */

int getHum(byte humidityPin, int sensorReadings) 
{

  return (computeVolts(getAverage(humidityPin, sensorReadings, 1))-0.8)/0.031;
  //float voltsHumidity = computeVolts(getAverage(humidityPin, sensorReadings, 1));
  //return int(0.6342*voltsHumidity*voltsHumidity + 29.139*voltsHumidity - 23.381);   
}


