import controlP5.*;
ControlP5 controlP5;

Textfield PachubeFEEDInput;
Textfield PachubeAPIKEYInput;
Textfield PachubeFEEDWebInput;
Textlabel MessageBoardGUI;


float counter;
float blink;

color colorA = #000000;
color colorB = #FFFFFF;
color colorC = #26EC00;
color colorC2 = #26EC00;
color colorD = #DDDDDD;

void GUI_setup() {
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

void GUI_refresh() {
  background(255);
}


void mainGUI() { 

  controlP5.addButton("Quit", 1, 470, 0, 30, 17);

  MessageBoardGUI = controlP5.addTextlabel("MessageBoard", "", 10, 230);
  MessageBoardGUI.setColorValue(colorA);

   
  PachubeFEEDInput = controlP5.addTextfield("PachubeFEED", 10, 50, 400, 20);
  PachubeAPIKEYInput =  controlP5.addTextfield("PachubeAPIKEY", 10, 90, 400, 20); 
  PachubeFEEDWebInput = controlP5.addTextfield("", 10, 270, 400, 15);

  controlP5.addButton("Save", 1, 10, 130, 60, 20);
}

void Quit() {
  UpdateMessageGUI("Quitting...", true);
  exit();
}

void loadConfigGUI() {
  PachubeFEEDInput.setText(PachubeFEED);
  PachubeAPIKEYInput.setText(PachubeAPIKEY);
  PachubeFEEDWebInput.setText(PachubeFEEDWeb);
}

void updateConfigGUI() {
  PachubeFEED =  PachubeFEEDInput.getText();
  PachubeAPIKEY =  PachubeAPIKEYInput.getText();
  PachubeFEEDWeb =  PachubeFEEDWebInput.getText();
  updatePachubesSettings();
}


void Save() {
  updateConfigGUI();
  println("updated");
}

void UpdateMessageGUI(String MessageBoard, Boolean Time) {
  if (Time == true) {
    int s = second();  
    int m = minute();  
    int h = hour();
    MessageBoard = h + ":" + m  + ":" + s + "\t" + MessageBoard;
  }
    MessageBoardGUI.setValue(MessageBoard);


}

