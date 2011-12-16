void txPachube() {
  buildPachubePayload(PACHUBE_FEED_NBR);            //build the XBee payload
  xbeeTX(coordAddr, strlen(&xbeePayload[0].C));     //send it
}

void xbeeTX(XBeeAddress64 destAddr, int payloadSize ) {

  //build the tx request
  zbTx.setAddress64(destAddr);
  zbTx.setAddress16(0xFFFE);
  zbTx.setPayload(&xbeePayload[0].B);
  zbTx.setPayloadLength(payloadSize);
  xbee.send(zbTx);
}

void readXBee() {                                                 //process incoming traffic from the XBee
  byte delyStatus, discStatus;

  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {    //is this an rx packet
      xbee.getResponse().getZBRxResponse(rx);               //yes, get the received data
      Serial.println("XBee unexpected RX");                 //this sensor unit is send-only, so we do not actually expect to receive data
    } 

    else if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {    //transmit status for packets we've sent
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      //get the delivery status, the fifth byte
      delyStatus = txStatus.getDeliveryStatus();
      discStatus = txStatus.getDiscoveryStatus();
      if (delyStatus == SUCCESS) {
        Serial.println("XBee TX OK");
      }
      else {  //the remote XBee did not receive our packet. is it powered on and configured correctly?
        Serial.println("XBee TX ERROR");
      }
    }

    else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {    //XBee administrative messages
      xbee.getResponse().getModemStatusResponse(msr);
      if (msr.getStatus() == ASSOCIATED) {
        Serial.println("XBee associated");
      } 
      else if (msr.getStatus() == DISASSOCIATED) {
        Serial.println("XBee disassociated");
      } 
      else {
        Serial.println("XBee other status");        //some other status
      }
    }

    else {
      Serial.println("XBee unexpected input");        //something else we were not expecting
    }
  }
}

/*----------------------------------------------------------------------*
 * Build the XBee data payload (packet).                                *
 * Our data packet is defined as follows:                               *
 * Bytes 0, 1: 16-bit integer representation of the Pachube feed        *
 *             number, MSB first.                                       *
 * Bytes 2-n:  CSV data to be sent to Pachube, terminated by a zero     *
 *             byte. Note it is the remote unit's responsibility to     *
 *             format the CSV data.                                     *
 *                                                                      *
 * The maximum XBee packet size is set by XBEE_PAYLOAD_LEN in the main  *
 * module. Note there is an upper limit, see the XBee ATNP command.     *
 *----------------------------------------------------------------------*/

void buildPachubePayload(int feedNumber) {

  char cPayload[XBEE_PAYLOAD_LEN], myInt[8];

  cPayload[0] = highByte(feedNumber);
  cPayload[1] = lowByte(feedNumber);
  cPayload[2] = 0;
  //cPayload[3] = 0;                    //terminator so strcat finds the end ok
  //cPayload[4] = 0;                    //terminator so strcat finds the end ok

  //    ltoa(msNow / 60000, myInt, 10);     //approx minutes sketch has been running
  //    strcat(cPayload, myInt);
  //    strcat(cPayload, ",");

  itoa(light, myInt, 10);        //light
  strcat(cPayload, myInt);
  strcat(cPayload, ",");

  itoa(temp, myInt, 10);        //temperature
  strcat(cPayload, myInt);
  strcat(cPayload, ",");

  itoa(humidity, myInt, 10);        //humidity
  strcat(cPayload, myInt);
  strcat(cPayload, ",");

  itoa(int(sound), myInt, 10);        //mic
  strcat(cPayload, myInt);
  strcat(cPayload, ",");

//  itoa(tgs2442, myInt, 10);        //tgs2442
//  strcat(cPayload, myInt);
//  strcat(cPayload, ",");

  itoa(int(tgs4161), myInt, 10);        //tgs4142
  strcat(cPayload, myInt);
//  strcat(cPayload, ",");


  /*

   UPGRADE for Pachube API 2.0 - Not finished
   
   strcat(cPayload, "1");
   strcat(cPayload, ",");
   itoa(light, myInt, 10);      
   strcat(cPayload, myInt);
   strcat(cPayload, "\n");
   
   strcat(cPayload, "2");
   strcat(cPayload, ",");
   itoa(temp, myInt, 10);      
   strcat(cPayload, myInt);
   strcat(cPayload, "\n");
   
   strcat(cPayload, "3");
   strcat(cPayload, ",");
   itoa(humidity, myInt, 10);      
   strcat(cPayload, myInt);
   strcat(cPayload, "\n");
   
   */

  for (int i=0; i<XBEE_PAYLOAD_LEN; i++) {
    xbeePayload[i].C = cPayload[i];
  }
}



