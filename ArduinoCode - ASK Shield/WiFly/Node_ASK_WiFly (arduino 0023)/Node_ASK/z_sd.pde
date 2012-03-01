/*
  SD card datalogger
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 	 
 */

#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

const int chipSelect = 10;

char name[9];

void setupSD() {
  pinMode(10, OUTPUT);
  SD.begin(chipSelect);
  sprintf(name, "%d.CSV", PACHUBE_FEED);
//  Serial.println(name);
}

void logSD() {
  File logfile = SD.open(name, FILE_WRITE);
  //    File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (logfile) {
    logfile.print(light);
    logfile.print(",");
    logfile.print(temp);
    logfile.print(",");
    logfile.print(humidity);
    logfile.print(",");
    logfile.print(sound, 2);
    logfile.print(",");
    logfile.println(tgs4161, 2);
    logfile.close();
    Serial.println("Log!");
  } 
  else {
    Serial.println("Error log!");
  }
}


















