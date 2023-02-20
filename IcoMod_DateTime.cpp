/*
  IcoMod_DateTime.cpp - IcoDesk Module to display the current date and time.
  Created by Till Michels, February 13, 2023.
*/

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "IcoMod_DateTime.h"
#include "time.h"

IcoMod_DateTime::IcoMod_DateTime(Adafruit_ST7735 *tft, const unsigned long gmtOffsetSec, const unsigned long daylightOffsetSec)
{
  _tft = tft;

  _gmtOffsetSec = gmtOffsetSec;
  _daylightOffsetSec = daylightOffsetSec;

  _ntpServer = "pool.ntp.org";

  _displayTime = true;

  configTime(_gmtOffsetSec, _daylightOffsetSec, _ntpServer);
}

void placeTextInCenter(Adafruit_ST7735 *tft, const String &textBuf, uint16_t x, uint16_t y, uint8_t textSize, uint16_t textColor)
{
  int16_t x1, y1;
  uint16_t size[2] = {0, 0};
  tft->setTextSize(textSize);
  tft->setTextColor(textColor);
  tft->getTextBounds(textBuf, 0, 0, &x1, &y1, &size[0], &size[1]); // calc width of new string
  tft->setCursor(x - size[0] / 2, y - size[1] / 2);
  tft->print(textBuf);
}

void printTime(Adafruit_ST7735 *tft, struct tm timeinfo)
{
  char hour[3];
  strftime(hour, 3, "%H", &timeinfo);

  char minute[3];
  strftime(minute, 3, "%M", &timeinfo);

  placeTextInCenter(tft, hour, tft->width() / 2, tft->height() / 3, 6, ST77XX_WHITE);
  placeTextInCenter(tft, minute, tft->width() / 2, tft->height() / 3 * 2, 6, ST77XX_WHITE);
}

void printDate(Adafruit_ST7735 *tft, struct tm timeinfo)
{
  char month[20];
  strftime(month, 20, "%B", &timeinfo);

  char day[5];
  strftime(day, 5, "%d", &timeinfo);

  char dayOfTheWeek[20];
  strftime(dayOfTheWeek, 20, "%A", &timeinfo);

  placeTextInCenter(tft, month, tft->width() / 2, tft->height() / 5, 2, ST77XX_WHITE);
  placeTextInCenter(tft, day, tft->width() / 2, tft->height() / 2, 6, ST77XX_WHITE);
  placeTextInCenter(tft, dayOfTheWeek, tft->width() / 2, tft->height() / 5 * 4, 2, ST77XX_WHITE);
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
  _tft->fillScreen(ST77XX_BLACK);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  
  _displayTime ? printTime(_tft, timeinfo) : printDate(_tft, timeinfo);
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