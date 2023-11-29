#pragma once

/***************************************************************************
* WiFiHelper.h
* local library to set up and handle the WiFi connections and calls
*
* wifi::connect()
* wifi::connectError()
* wifi::connected()
* wifi::connectionFailed()
* wifi::ntp()
* wifi::disconnect()
*
****************************************************************************/

#include "globals.h"

#define CONN_TIMEOUT 500
#define CONN_ATTEMPTS 5
#define MESSAGE_DELAY 3000

int gmtOffset_sec = 0;
int daylightOffset_sec = 0;
const char* ntpServer = "europe.pool.ntp.org"; 
struct tm timeinfo;

namespace wifi {

  // connect to ssid using password and return true or false
  bool connect(){    // for non ESP32 device add (String ssid, String password)

    //connect to WiFi
    coreS3::clearScreen();
    M5.Lcd.setTextColor(BLUE, CYAN);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("\n    ** WIFI CONNECT **    \n");
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.printf("\nUse a device to check\nWiFi and chose Select_WiFi \nA web page will list all\navailable WiFi networks.");
    M5.Lcd.printf("\n\nSelect your network and\nenter your password.\n\nThis device will connect\nto your chosen network.");

    // use WiFiManager library to obtain available WiFi networks
    WiFi.mode(WIFI_STA);
    WiFiManager wm;
    //wm.resetSettings(); //uncomment to force selecting WiFi network 
    bool res;
    res = wm.autoConnect("Select_WiFi","");

    if (!res) {
      Serial.println("Failed to connect.");
      coreS3::clearScreen();
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("Failed to connect!\n");
      M5.Lcd.printf("\nRestarting device in 5s.");
      delay(5000);
      ESP.restart();
    } else {
      return true;
    }

    return true;                                     // connection successful

  } // connect()

  // error connection message
  void connectError(){

    coreS3::clearScreen();
    M5.Lcd.setTextColor(RED, BLACK);         
    M5.Lcd.printf("  **  FAILED TO CONNECT  **  \n");
    //M5.Lcd.printf("  **  %s  **\n", ssid);

    M5.Lcd.setTextColor(WHITE, BLACK);     
    M5.Lcd.printf("\nRestarting Device in 5s.\n");

    delay(5000);
    ESP.restart();

  } // connectError()

  // display final connections failed message
  void connectionFailed(){

    coreS3::clearScreen();
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("   **  FAILED TO CONNECT  **  \n\n");
    M5.Lcd.printf("  **  TO ANY WIFI NETWORK  **  ");
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("\n\nReattempt in 10s.");

  } // connecttionFailed()

  // when connected to the internet display success
  void connected(String ssid){

    coreS3::clearScreen();
    M5.Lcd.setTextColor(BLACK, GREEN);         
    M5.Lcd.printf("\n    **  CONNECTED  **     \n\n");

    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("\n\nSSID: %s\n", WiFi.SSID().c_str());         
    M5.Lcd.printf("\nIP  : "); M5.Lcd.println(WiFi.localIP());

    delay(1500);

  } // connected()

  // connect to the ntp server
  void ntp(){

    //init and get the time
    Serial.printf(" Obtaining WWW time..\n");
    M5.Lcd.printf("\nObtaining WWW time...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);   // get the internet time
    delay(MESSAGE_DELAY);

  } // ntp()

  void disconnect(){

    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

  } // disconnect()

}