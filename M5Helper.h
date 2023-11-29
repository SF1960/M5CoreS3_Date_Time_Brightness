#pragma once

/***************************************************************************
* M5Helper.h
* local library to set up and handle the M5
*
* coreS3::setup()
* coreS3::background()
* coreS3::printDateTime()
* coreS3::displayBattery()
* coreS3::clearDisplay()
* 
****************************************************************************/

#include "globals.h"

namespace coreS3{

  void setup(){
    width = M5.Lcd.width();                                  // save the screen's width
    height = M5.Lcd.height();                                // save the screen's height
    M5.Lcd.setTextSize(2);                                   // Set text size.
    M5.Lcd.setRotation(CAMERA_TOP);                          // Sreen rotation
    M5.Lcd.setTextDatum(CC_DATUM);                           // set drawString justification to centre
    M5.Lcd.fillScreen(BLACK);                                // Black screen
    M5.Lcd.setTextWrap(true);
  }

  void background(){
    M5.Lcd.fillScreen(BLACK);                                // Black screen
    M5.Lcd.fillRoundRect(0, 0, width, height, 5, TFT_BLUE);  // box around title       
    M5.Lcd.fillRoundRect(5, 25, width -10, height -30, 5, BLACK);// fill rounded black box
    M5.Lcd.drawRoundRect(5, 25, width -10, height -30, 5, CYAN);// cyan rounded box at screen extremties
    M5.Lcd.drawRoundRect(48, 133, 225, 60, 3, DARKGREY);     // box around time 
    M5.Lcd.drawRoundRect(48,  53, 225, 60, 3, DARKGREY);     // box around day and date 
         
  }
 
  void printDateTime(){
    // print the date and time
    M5.Lcd.setTextSize(2);                                           // Set the text size. 
    M5.Lcd.setTextColor(DARKGREY, BLACK);                            // white text on black background
    M5.Lcd.setTextDatum(TC_DATUM);                                   // set text to top centre
    M5.Lcd.drawString(dayStrbuff, width/2, 60, 1);                   // print the day of the week centred
    M5.Lcd.drawString(dateStrbuff, width/2, 90, 1);                  // print date centred on screen in font 1
    M5.Lcd.setTextSize(1);                                           // Set the text size.  
    M5.Lcd.drawString(timeStrbuff, width/2, 140, 7);                 // print time centred on screen in font 7
  }

  // display the battery level top right of screen
  void displayBattery(int dispPower){

    batteryLevel = M5.Power.getBatteryLevel();
    batteryVoltage = M5.Power.getBatteryVoltage();
    batteryCurrent = M5.Power.getBatteryCurrent();
    float bVolts = batteryVoltage * 0.001;
    float bAmps = batteryCurrent; //* 0.01;
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.fillRect(175, 5, 155, 15, BLUE);

    switch (dispPower){   // 1 = Level, 2 = Volts, 3 = Amps
      case 1:
        M5.Lcd.drawString("Battery Level: ", 245, 11, 1);
        M5.Lcd.drawFloat(batteryLevel, 0, 295, 11);
        M5.Lcd.drawString("%", 310, 11, 1);
        break;
      case 2:
        M5.Lcd.drawString("Battery Voltage: ", 230, 11, 1);
        M5.Lcd.drawFloat(bVolts, 2, 293, 11);
        M5.Lcd.drawString("V", 310, 11, 1);
        break;
      case 3:
        M5.Lcd.drawString("Charge current: ", 230, 11, 1);
        M5.Lcd.drawFloat(bAmps, 2, 290, 11);
        M5.Lcd.drawString("mA", 310, 11, 1);
        break;
      default:
        break;
    }

  }

  void clearScreen(){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,0);
  }

}