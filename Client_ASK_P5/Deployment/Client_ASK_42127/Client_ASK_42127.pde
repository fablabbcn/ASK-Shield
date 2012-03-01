// RAW Script for the ASK_Shield


import eeml.*   ;
import processing.serial.*;

// these are arrayLists , where the datas will be stored

//ArrayList Latitude  =new ArrayList() ;
//ArrayList Longitude =new ArrayList() ;
ArrayList Temp =new ArrayList() ;
ArrayList Humidity = new ArrayList() ;   
ArrayList Light = new ArrayList() ;
ArrayList Noise = new ArrayList() ;
//ArrayList co = new ArrayList() ;
ArrayList C02 = new ArrayList() ;


DataOut dOut    ;
Serial myPort;  
int val; 
String tempBuffer=null ;
boolean sendToPachube=false ;
float lastUpdate;
int position=0    ;


String PachubeFEED = "42127";

String PachubeAPIKEY = "nKn8ldoeUYHFdcLymuWAAm6KfblczFJFCIc8GT2_G30";

String PachubeFEEDWeb = "https://pachube.com/feeds/" + PachubeFEED;



void setup() {
  updatePachubesSettings();

  GUI_setup();
  UpdateMessageGUI("Starting...", false);

  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
}




void draw()
{



  GUI_refresh();  

  if ( myPort.available() > 0) {  // If data is available,
    delay(100);
    String inBuffer = myPort.readString();
    String first="$" ; //This is to indentify a good string and avoid adding invalid data

    String tempFirst=Character .toString(inBuffer.charAt(0)) ; 
    if (tempFirst.equals(first)==true ) {
      //tempBuffer=inBuffer;//tempBuffer+inBuffer;
      println("good string received:");
      println(inBuffer);
      String [] values = split (inBuffer, ','); 
      //float  tempLatitude  = float (values [3])/100;
      //float  tempLongitude = float (values [5])/100;
      if (values.length<6) {
        println("bad data received");
        println("split by commas:");
        println(values);
      }
      else {
        println("split by commas:");
        println(values);       
        String tempTemperature = values [1];
        String tempHumidity = values [3];
        String tempLight = values [5];
        String tempNoise = values [7];
        String tempCo2 = values [9];
        
//        UpdateMessageGUI(" Light: " + tampLight + " Temp: " + tempTemperature + " Hum: " + tempHumidity  + " Noise: " + tempNoise + " C02: " + temp Co2, true);

        String tempMessage = "Light: " + tempLight + " Temp: " + tempTemperature + " Hum: " + tempHumidity  + " Noise: " + tempNoise + " C02: " + tempCo2;


        UpdateMessageGUI(tempMessage, true);

        // Latitude.add (tempLatitude) ;
        //Longitude.add (tempLongitude) ;


        Temp.add (tempTemperature) ;
        Humidity.add (tempHumidity) ;
        Light.add (tempLight) ;
        Noise.add (tempNoise) ;
        C02.add (tempCo2) ;

        sendToPachube=true ;
        //tempBuffer=null ;
      }
    }
    else {
      println("bad string, does not start with $");
    }
    inBuffer=null;
  }

  // here the values that are going to be sent to pachube

  if ((millis() - lastUpdate) > 5000  && sendToPachube) {
    println("ready to POST: ");

    // dOut.update(0, (Float )Latitude.get (position)) ; 
    //dOut.update(1, (Float)Longitude.get (position)) ; 
    dOut.update(0, (String)Light.get (position)) ; 
    dOut.update(1, (String)Temp.get (position));  
    dOut.update(2, (String)Humidity.get (position)); 
    dOut.update(3, (String)Noise.get (position)); 
    dOut.update(4, (String)C02.get (position)); 

    //dOut.update(3, (String)Co2.get (position)); 
    int response = dOut.updatePachube(); 
    println(response);
    println(printResponse (response));

    UpdateMessageGUI(printResponse(response), true);

    //  println(str(val)); 
    lastUpdate = millis();
    position=position+1;
  }
}


void updatePachubesSettings() {

  String PachubeAPIURL = "http://api.pachube.com/v1/feeds/" + PachubeFEED + ".xml";

  PachubeFEEDWeb = "https://pachube.com/feeds/" + PachubeFEED;

  dOut = new DataOut(this, PachubeAPIURL, PachubeAPIKEY);   

  dOut.addData(0, "Light");
  dOut.addData(1, "Temp");
  dOut.addData(2, "Hum");
  dOut.addData(3, "Noise");
  //  dOut.addData(4, "Hum");
  dOut.addData(5, "Sound");
}

String printResponse (int response) {
  String responseString = null;
  switch(response)
  {
  case 200: 
    responseString = "Successfully published to PACHUBE!";
    break;
  case 401:          
    responseString = "Warning: Unauthorized"; 
    break;     
  case 404:             
    responseString = "Warning: Feed doesn't exist" ;
    break;
  case 400:
    responseString = "Warning: Some trouble happend! We'll try it again." ;
    break;
  }
  return responseString;
}

