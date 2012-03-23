#include "WiFly.h"

// PACHUBE
#define PACHUBE_FEED "42124"
#define APIKEY "nKn8ldoeUYHFdcLymuWAAm6KfblczFJFCIc8GT2_G30"

// WIFI

char passphrase[] = "m1cr0fug4s";
char ssid[] = "hangar_nau3";
boolean mode = WPA_MODE; //or WEP_MODE

WiFlyClient client("api.pachube.com", 80);


uint32_t timeLastUpdated;
int i;
//char buff[64];
char buff[96];  // message lenght 

void setupWiFly() {
  Serial.begin(9600);
  WiFly.setUart(&Serial); 
  WiFly.begin();    // startup the WiFly
  if (!WiFly.join(ssid, passphrase, mode)) {
    while (1) {
    }
  } 
}


void txWiFly() {

  sprintf(buff,"0,%d\n1,%d\n2,%d\n3,%d\n4,%d\n",light, temp, humidity, sound, tgs4161);

  if (client.connect()) {
    client.print("PUT /v2/feeds/");  // APIV2
    client.print(PACHUBE_FEED);
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


void pingWiFly() {

}


