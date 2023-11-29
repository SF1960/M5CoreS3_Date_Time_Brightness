#pragma once

/***************************************************************************
* soundHelper.h
* local library to play sounds
*
* sound::playWelcome()
* sound::playTone()
*
****************************************************************************/

#include "globals.h"

namespace sound{

  void playWelcome(){

    if (M5.Speaker.isEnabled()) {

      M5.Speaker.setVolume(48);                                  // set master volume (0~255)

      //while (M5.Speaker.isPlaying()) { M5.delay(1); }            // wait done

      //while (M5.Speaker.isPlaying()) { M5.delay(1); }            // wait play beep sound 2000Hz 100msec (background task)

      M5.Speaker.playRaw(wav_8bit_44100, sizeof(wav_8bit_44100), 44100, false);

    }

  }

  void playTone(){
    M5.Speaker.tone(783.991, 100);
  }

}