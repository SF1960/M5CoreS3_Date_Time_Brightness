#pragma once

/***************************************************************************
* prefHelper.h
* local library to set up and handle the EEPROM read/write
*
* states::saveBrightness()
* states::getBrightness()
*
****************************************************************************/

#include "globals.h"

namespace states{

  void saveBrightness(int bright){
    config.begin("states", false);
    config.putInt("brightness", bright);
    config.end();
  }

  // get saved chosenBrightness value
  int getBrightness(){
    config.begin("states", false);
    int bright = config.getInt("brightness", 1);
    config.end();
    return bright;
  }

}