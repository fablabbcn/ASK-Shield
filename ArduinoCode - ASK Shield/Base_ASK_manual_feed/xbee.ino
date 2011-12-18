/*----------------------------------------------------------------------*
 * Read incoming XBee traffic.                                          *
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

void readXBee() {
  int intFeed;         //Pachube feed number  
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {                       //incoming traffic
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {    //rx data packet
      xbee.getResponse().getZBRxResponse(rx);               //fill our zb rx class
      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        for (int i=0; i<XBEE_PAYLOAD_LEN; i++) {          //copy the received data to our buffer
          xbeePayload[i].B = rx.getData(i);
        }
        Serial.println("XBee RX/ACK");
      } 
      else {
        Serial.println("XBee RX no ACK");  //packet received and not ACKed
      }
      //process the received data
      intFeed = (xbeePayload[0].B << 8) + xbeePayload[1].B;    //reconstruct the pachube feed number
      putData(&xbeePayload[2].C, intFeed);                     //send it off to pachube
    } 
    else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      xbee.getResponse().getModemStatusResponse(msr);     //XBee administrative messages
      if (msr.getStatus() == ASSOCIATED) {
        Serial.println("XBee associated");
      } 
      else if (msr.getStatus() == DISASSOCIATED) {
        Serial.println("XBee disassociated");
      } 
      else {
        Serial.println("XBee other status");            //some other status
      }
    } 
    else {
      Serial.println("XBee unexpected input");            //not something we were expecting
    }
  } 
  else {
//    Serial.println("XBee not avaiable");            //not something we were expecting
  }
}



