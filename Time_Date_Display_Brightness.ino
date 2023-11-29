/***************************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5CoreS3 sample source code
*
* Visit for more information: https://docs.m5stack.com/en/core/CoreS3
*
* Describe: Hello World
* Date: 2023/5/3
****************************************************************************************/
/***************************************************************************************
* Description:  Display the time and date from the internet using ntp server
* Sketch:       Time_Date_Display_Brightness.ino
* Version:      1.0
* Version Desc: 1.0 Default sketch using ntp server, touch for brightness and EEPROM storage
*                   Displays power characteristics (Level, Volts, Amps) of battery
*                   Added a welcome sound
* Board:        M5 Core S3
* Author:       Steve Fuller
* Website:      sgfpcb@gmail.com 
* Comments      This code modified from Hello World sketch example from M5
***************************************************************************************/
#include <Arduino.h>                                      // Not really required in Arduino IDE but good practice to include
#include <SD.h>                                           // SD card library - MUST COME BEFORE DISPLAY LIBRARY
#include <M5ModuleDisplay.h>                              // replaces CoreS3 library
#include <M5Unified.h>                                    // library for all M5 available units
#include <WiFi.h>                                         // library to connect to a WiFi network
#include <WiFiManager.h>                                  // library to present a list if WiFi networks allowing user to select their own network 
#include <M5GFX.h>                                        // library to handle screen touches
#include <Preferences.h>                                  // library to store to EEPROM

M5GFX display;                                            // set up an object called display

Preferences config;                                       // set up an object called config used to store brightness choice

#include "globals.h"                                      // local library that holds global variables and constants
#include "M5Helper.h"                                     // local library to manage display on M5
#include "WiFiHelper.h"                                   // local library to manage WiFi connections
#include "prefHelper.h"                                   // local library to manage EEPROM
#include "soundHelper.h"                                  // local library to manage sound

/***************************************************************************
* After M5CoreS3 is started or reset the program in the setUp()
* function will be run, and this part will only be run once.
****************************************************************************/
void setup() {

  auto cfg = M5.config();
  #if defined ( ARDUINO )
    cfg.serial_baudrate = 115200;                          // default=115200. if "Serial" is not needed, set it to 0.
  #endif
  cfg.clear_display = true;                                // default=true. clear the screen when begin.
  cfg.internal_imu  = true;                                // default=true. use internal IMU.
  cfg.internal_rtc  = true;                                // default=true. use internal RTC.
  cfg.internal_spk  = true;                                // default=true. use internal speaker.
  cfg.led_brightness = 64;                                 // default= 0. system LED brightness (0=off / 255=max) (※ not NeoPixel)

  M5.begin(cfg);                                           // begin M5Unified.
                 
  chosenBrightness = states::getBrightness();              // get the stored value of screen brightness from EEPROM
  display.init();                                          // initialised display object (for touch)
  coreS3::setup();                                         // set up the Core S3 display

  sound::playWelcome();                                    // play the welcome tune

  // use WiFiManager library to connect to an available WiFi network
  if (wifi::connect()) {
    wifi::connected(WiFi.SSID().c_str());                  // display success message
  } else {
    wifi::connectError();                                  // display error message and restart ESP32
  }

  wifi::ntp();                                             // connect to the ntp server and obtain date and time
  coreS3::background();                                    // draw screen's background
  drawBars();                                              // draw brightness bars
  drawBGSecondBars();                                      // draw background of seconds bars

  Serial.printf("Setup complete. \n");

} // setup()

/********************************************************************************
* After the program in setup() runs, it runs the program in loop()
* The loop() function is an infinite loop in which the program runs repeatedly
*********************************************************************************/
void loop() {

  lgfx::touch_point_t tp[3];

  // check screen touch and update brightness if pressed
  int touch = display.getTouchRaw(tp, 3);

  if (touch == 1){
    chosenBrightness++;
    if (chosenBrightness >= 5){
      chosenBrightness = 0;
    }

    M5.Display.setBrightness(brightness[chosenBrightness]);
    sound::playTone();

    drawBars();                                              // redraw the brightness bars
    delay(250);                                              // mini debounce 
    
    states::saveBrightness(chosenBrightness);                // save the new brightness value to EEPROM

  }

  // toggle the power display
  unsigned long CurrentPowerMillis = millis();
  if (CurrentPowerMillis - PreviousPowerMillis >= UpdatePower) {

    PreviousPowerMillis = CurrentPowerMillis;
    PowerToggle++;

    if (PowerToggle > 3){
      PowerToggle = 1;
    }

    coreS3::displayBattery(PowerToggle);                     // display battery information dependent on value PowerToggle

  }

  // timer for calling second bar display
  unsigned long CurrentMillis = millis();                    // get the system millis()
  if (CurrentMillis - PreviousMillis >= UpdateInterval) {    // when millis() is greater than UpdateInterval then execute code

    PreviousMillis = CurrentMillis;                          // save the last time was updated
    flushTime();                                             // get the time and display
    if (seconds++>=60){                                      // if seconds >= 60
      seconds = 0;                                           // reset the seconds counter 
      drawBGSecondBars();                                    // draw the background bars for seconds
    }
    drawSecondBars(seconds);                                 // draw the seconds bar

  }

} // loop()

// draw screen brightness bars
void drawBars(){

  // brightness gauge background bars
  for(int i=0; i<=5; i++)
  M5.Lcd.fillRect(10+(i*6), 9, 4, 12, colorDarkGrey);

  // brightness gauge bars
  for(int i=0; i<chosenBrightness+1; i++)
  M5.Lcd.fillRect(10+(i*6), 9, 4, 12, colGreen);

} // drawBars()

// draw Second Bars background in dark grey
void drawBGSecondBars(){

  // second background bars
  for(int i=0; i<60; i++)
  M5.Lcd.fillRect(10+(i*5), 220, 4, 12, colorDarkGrey);

} // drawBGSecondBars()

// void draw Second Bars. It displays bars based on secs value/
void drawSecondBars(int secs){

  // brightness second bars
  for(int i=0; i<secs; i++)
  M5.Lcd.fillRect(10+(i*5), 220, 4, 12, colGreen);

} // drawSecondBars()

// Get the time and print to the display
void flushTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  // extract data from timeinfo variable returned by the ntp server
  char timeHour[3];   strftime(timeHour, 3,   "%H", &timeinfo);     // Set the hour
  char timeMins[3];   strftime(timeMins, 3,   "%M", &timeinfo);     // set the minute
  char timeSecs[3];   strftime(timeSecs, 3,   "%S", &timeinfo);     // set the seconds
  char dateDate[3];   strftime(dateDate, 3,   "%d", &timeinfo);     // set the date
  char dateDay[12];   strftime(dateDay, 12,   "%A", &timeinfo);     // set the day
  char dateMonth[10]; strftime(dateMonth, 10, "%B", &timeinfo);     // set the month
  char dateYear[5];   strftime(dateYear, 5,   "%Y", &timeinfo);     // set the year

  // get the number of seconds returned as an integer
  int msd = timeSecs[0]-'0';                                        // get most significant digit from timeSecs[]. For example, 23 will return 2 to msd
  int lsd = timeSecs[1]-'0';                                        // get least significant digit from timeSecs[] For example, 23 will rerurn 3 to lsd
  int tot = (msd*10) + lsd;                                         // add the msd and lsd and store in tot vatiable. The string for seconds
  seconds = tot;                                                    // set the variable seconds to the value of tot
  if (seconds >= 59){                                               // when greater than or equal to 59
    seconds=0;                                                      // set the seconds variable to 0
    drawBGSecondBars();                                             // redraw the background of the second bars
  }

  // Stores real-time time and date data to timeStrbuff and dateStrbuff
  sprintf(dayStrbuff, "%s", dateDay);                               // store the date in dayStrbuff
  sprintf(timeStrbuff, "%s:%s:%s", timeHour, timeMins, timeSecs);   // store the hour, mins and secs in timeStrbuff
  sprintf(dateStrbuff, "%s %s %s", dateDate, dateMonth, dateYear);  // store the date month and year in the dateStrbuff

  coreS3::printDateTime();                                          // print the date and time on the Core S3

} // flushTime()
