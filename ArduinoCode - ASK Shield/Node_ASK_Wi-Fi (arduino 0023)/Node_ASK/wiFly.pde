#include "WiFly.h"

// PACHUBE
#define PACHUBEFEED "42045"
#define APIKEY "nKn8ldoeUYHFdcLymuWAAm6KfblczFJFCIc8GT2_G30"
#define TIMETOUPDATE 15000  // frequency of update - every 15 seconds

// WIFI

// FabLab
char passphrase[] = "66616231666162326661623366"; //fab1fab2fab3f in HEX
char ssid[] = "IAAC-FAB";
boolean mode = WEP_MODE; //or WPA_MODE

// Iaac
//char passphrase[] = "31663266336634663566366637"; //1f2f3f4f5f6f7 in HEX
//char ssid[] = "IAAC";
//boolean mode = WEP_MODE; //or WPA_MODE



Client client("api.pachube.com", 80);

uint32_t timeLastUpdated;
int i;
//char buff[64];
char buff[96];  // message lenght 

void setupWiFly() {
  delay(10000);
  Serial.begin(9600);
  WiFly.setUart(&Serial); 
  WiFly.begin();    // startup the WiFly
  if (!WiFly.join(ssid, passphrase, WEP_MODE)) {
    while (1) {
    }
  } 
}

void txWiFly() {
//  
//    long tempC = 999.0;
//    sprintf(buff,"0,%d\n1,%d",i++,tempC);
    
  sprintf(buff,"0,%d\n1,%d\n2,%d\n",light,temp,humidity);

  if (client.connect()) {
    client.print("PUT /v2/feeds/");  // APIV2
    client.print(PACHUBEFEED);
    client.println(".csv HTTP/1.1");
    client.println("Host: api.pachube.com");
    client.print("X-PachubeApiKey: ");
    client.println(APIKEY);
    client.println("User-Agent: Arduino (WiFly RN-XV)");
    client.print("Content-Type: text/csv\nContent-Length: ");
    client.println(strlen(buff));
    client.println("Connection: close");
    client.println();
    client.print(buff);
    client.println();
  } 

  delay(2000);

  if (client.connected()) {
    client.stop();
  }
}





