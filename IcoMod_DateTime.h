/*
  IcoMod_DateTime.cpp - IcoDesk Module to display the current date and time.
  Created by Till Michels, February 13, 2023.
*/

#ifndef IcoMod_DateTime_h
#define IcoMod_DateTime_h

#include "Arduino.h"
#include "IcoMod.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class IcoMod_DateTime : public IcoMod
{
  public:
    IcoMod_DateTime(Adafruit_ST7735* tft, const unsigned long gmtOffsetSec, const unsigned long daylightOffsetSec);
    void onClick();
    void initialize();
    void refresh();
  private:
    const char *_ntpServer;
    unsigned long _gmtOffsetSec;
    unsigned long _daylightOffsetSec;
    bool _displayTime;
};

#endif