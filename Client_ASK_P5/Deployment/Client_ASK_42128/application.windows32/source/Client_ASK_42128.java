import processing.core.*; 
import processing.xml.*; 

import eeml.*; 
import processing.serial.*; 
import controlP5.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Client_ASK_42128 extends PApplet {

// RAW Script for the ASK_Shield





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


String PachubeFEED = "42128";

String PachubeAPIKEY = "nKn8ldoeUYHFdcLymuWAAm6KfblczFJFCIc8GT2_G30";

String PachubeFEEDWeb = "https://pachube.com/feeds/" + PachubeFEED;



public void setup() {
  updatePachubesSettings();

  GUI_setup();
  UpdateMessageGUI("Starting...", false);

  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
}




public void draw()
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


public void updatePachubesSettings() {

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

public String printResponse (int response) {
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


ControlP5 controlP5;

Textfield PachubeFEEDInput;
Textfield PachubeAPIKEYInput;
Textfield PachubeFEEDWebInput;
Textlabel MessageBoardGUI;


float counter;
float blink;

int colorA = 0xff000000;
int colorB = 0xffFFFFFF;
int colorC = 0xff26EC00;
int colorC2 = 0xff26EC00;
int colorD = 0xffDDDDDD;

public void GUI_setup() {
  size(500, 300);
  background(255);
  smooth();

  controlP5 = new ControlP5(this);

  controlP5.setColorActive(colorC);
  controlP5.setColorForeground(colorD);
  controlP5.setColorBackground(colorD);
  controlP5.setColorLabel(colorA);
  controlP5.setColorValue(colorA);

  mainGUI();

  loadConfigGUI();
}

public void GUI_refresh() {
  background(255);
}


public void mainGUI() { 

  controlP5.addButton("Quit", 1, 470, 0, 30, 17);

  MessageBoardGUI = controlP5.addTextlabel("MessageBoard", "", 10, 230);
  MessageBoardGUI.setColorValue(colorA);

   
  PachubeFEEDInput = controlP5.addTextfield("PachubeFEED", 10, 50, 400, 20);
  PachubeAPIKEYInput =  controlP5.addTextfield("PachubeAPIKEY", 10, 90, 400, 20); 
  PachubeFEEDWebInput = controlP5.addTextfield("", 10, 270, 400, 15);

  controlP5.addButton("Save", 1, 10, 130, 60, 20);
}

public void Quit() {
  UpdateMessageGUI("Quitting...", true);
  exit();
}

public void loadConfigGUI() {
  PachubeFEEDInput.setText(PachubeFEED);
  PachubeAPIKEYInput.setText(PachubeAPIKEY);
  PachubeFEEDWebInput.setText(PachubeFEEDWeb);
}

public void updateConfigGUI() {
  PachubeFEED =  PachubeFEEDInput.getText();
  PachubeAPIKEY =  PachubeAPIKEYInput.getText();
  PachubeFEEDWeb =  PachubeFEEDWebInput.getText();
  updatePachubesSettings();
}


public void Save() {
  updateConfigGUI();
  println("updated");
}

public void UpdateMessageGUI(String MessageBoard, Boolean Time) {
  if (Time == true) {
    int s = second();  
    int m = minute();  
    int h = hour();
    MessageBoard = h + ":" + m  + ":" + s + "\t" + MessageBoard;
  }
    MessageBoardGUI.setValue(MessageBoard);


}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#FFFFFF", "Client_ASK_42128" });
  }
}
