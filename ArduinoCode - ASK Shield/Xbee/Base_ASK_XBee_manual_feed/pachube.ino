/*---------------------------------------------------------------------* 
 * Send data to Pachube.                                               *
 * csvData is the comma-separated variable data to be sent.            *
 * intFeed is the Pachube feed number to send it to.                   *
 *---------------------------------------------------------------------*/

void putData(char csvData[], int intFeed) {

  char txMsg[21] = "Pachube TX ", txAddr[8];    //status message

  //if there's incoming data from the net connection, send it out the serial port.
  //this is for debugging purposes only.
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    //send the HTTP PUT request. 
    client.print("PUT /api/feeds/");
    client.print(manualFeedID, DEC);    //include the feed number
    client.print(".csv HTTP/1.1\n");
    client.print("Host: www.pachube.com\n");
    client.print("X-PachubeApiKey: ");
    client.print(PachubeApiKey);
    client.print("\n");
    client.print("Content-Length: ");
    client.println(strlen(csvData), DEC);
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");
    client.println(csvData);        //include the CSV data
    client.stop();
    itoa(intFeed, txAddr, 10);      //build a status message
    strcat(txMsg, txAddr);
    Serial.println(txMsg);          //print it
  } 
  else {
    Serial.println("FAIL: Pachube connect");
  }
}

