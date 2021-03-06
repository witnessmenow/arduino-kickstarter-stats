/*******************************************************************
 *  Get Kickstarter Project statistics using an ESP8266
 *
 *  By Brian Lough
 *  https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/

#include <KickstarterStats.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

//------- Replace the following! ------
char ssid[] = "ssid";       // your network SSID (name)
char password[] = "password";  // your network key

#define KS_USER "idlehandsdev" // User name on kickstarter: https://www.kickstarter.com/projects/idlehandsdev 

#define KS_PROJECT "hcc-mod-ready-to-solder-arduino-module" // Project name on kickstarter: 
// https://www.kickstarter.com/projects/idlehandsdev/hcc-mod-ready-to-solder-arduino-module

WiFiClientSecure client;
KickstarterStats kss(client);

unsigned long delayBetweenRequests = 60000; // Time between requests (1 minute)
unsigned long requestDueTime;   //time when request due

void setup() {

  Serial.begin(115200);

  // kss._debug = true;

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {
    
      if (millis() > requestDueTime)  {
        Serial.println("Getting Data:");
        if(kss.getProjectStats(KS_USER, KS_PROJECT))
        {
          Serial.println("---------Project Stats---------");
          Serial.print("Id: ");
          Serial.println(kss.projectStats.id);
          Serial.print("State Changed At: ");
          Serial.println(kss.projectStats.state_changed_at);
          Serial.print("State: ");
          Serial.println(kss.projectStats.state);
          Serial.print("Backers Count: ");
          Serial.println(kss.projectStats.backers_count);
          Serial.print("Pledged: CA$");
          Serial.println(kss.projectStats.pledged); //This comes back in the KS user's currency
          Serial.print("Comments Count: ");
          Serial.println(kss.projectStats.comments_count);
          Serial.println("------------------------");
    
        }
    
        requestDueTime = millis() + delayBetweenRequests;
      }
    }