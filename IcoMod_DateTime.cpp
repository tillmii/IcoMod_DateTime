/*
  IcoMod_DateTime.cpp - IcoDesk Module to display the current date and time.
  Created by Till Michels, February 13, 2023.
*/

#include "Arduino.h"
#include "IcoMod_DateTime.h"
#include "time.h"
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "TextUtils.h"

IcoMod_DateTime::IcoMod_DateTime(Adafruit_ST7735* tft, unsigned int colors[], JsonObject &config)
{
  _tft = tft;
  _colors = colors;

  _gmtOffsetSec = config["gmtOffsetSec"];
  _daylightOffsetSec = config["daylightOffsetSec"];

  _ntpServer = "pool.ntp.org";

  _displayTime = true;

  configTime(_gmtOffsetSec, _daylightOffsetSec, _ntpServer);
}

void printTime(Adafruit_ST7735 *tft, unsigned int colors[], struct tm timeinfo)
{
  char hour[3];
  strftime(hour, 3, "%H", &timeinfo);

  char minute[3];
  strftime(minute, 3, "%M", &timeinfo);

  TextUtils::printCentered(tft, hour, 30, 6, colors[1]);
  TextUtils::printCentered(tft, minute, 84, 6, colors[1]);
}

void printDate(Adafruit_ST7735 *tft, unsigned int colors[], struct tm timeinfo)
{
  char month[20];
  strftime(month, 20, "%B", &timeinfo);

  char day[5];
  strftime(day, 5, "%d", &timeinfo);

  char dayOfTheWeek[20];
  strftime(dayOfTheWeek, 20, "%A", &timeinfo);

  TextUtils::printCentered(tft, month, 24, 2, colors[1]); // 24 -> height(160) / 5 - textSize(16) / 2
  TextUtils::printCentered(tft, day, 56, 6, colors[1]); // 56 -> height(160) / 2 - textSize(48) / 2
  TextUtils::printCentered(tft, dayOfTheWeek, 120, 2, colors[1]); // 56 -> height(160) / 5 * 4 - textSize(16) / 2
}

void printLocalTime(Adafruit_ST7735 *tft)
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
}

void IcoMod_DateTime::onClick()
{
  _displayTime = !_displayTime;
  initialize();
}

void IcoMod_DateTime::initialize()
{
  _tft->fillScreen(_colors[0]);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  
  _displayTime ? printTime(_tft, _colors, timeinfo) : printDate(_tft, _colors, timeinfo);
}

void IcoMod_DateTime::refresh()
{
  static unsigned int lastMinute = 0; // Minute from last cycle

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  if (lastMinute != timeinfo.tm_min)
  {
    initialize();
  }

  lastMinute = timeinfo.tm_min;
}