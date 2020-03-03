/***************************************************************************
  An Altimeter Tester for checking dual deploy events.  Design is by Dave
  Combs of WSR703 (www.WSR703.org) and MacModeler. 

  The TFT35 class handles the interactions with the 3.5" TFT board/SD
  including reading, displaying, and writing to SD a data struct.  The
  touch screen events are not enabled in this version. The Adafruit tutorial
  at https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout
  provided invaluable guidance in developing this class.
   
  Author:  Mark E. McBride
  Last Update:  2020.01.25

  ChangeLog:
  next up:     Remove flicker by buffering smaller parts of screen
  2020.01.25:  Changed font for numbers to 7 segment full
  2020.01.25:  Display working with flicker
  2020.01.25:  Basic HX8357 class displaying labels
  2020.01.19:  Create initial Runnable setup 
  
 ***************************************************************************/
#ifndef TFT35_H
#define TFT35_H

/* Arduino Libraries */
#include <SPI.h>
#include <Adafruit_HX8357.h>
#include <Adafruit_GFX.h>

/* MacModeler Headers */
#include "Globals.h"

/* sensor defines */
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#define TFT_RST -1


Adafruit_HX8357 myTFT = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);  // hardware SPI interface

// Fonts created by http://oleddisplay.squix.ch/
#include <Fonts/ArialRoundedMTBold_36.h>
#include <Fonts/Font_32_Segment_7_Full.h>
int rotation = 4;  // set screen orientation

//define debug_tft  // comment out for final version

class TFT35: public Runnable {

  private:
    /* internal variables with default initializers */
    Adafruit_HX8357* theTFT;  // pointer to the screen
    Readings* current;  // pointer to current readings structure
    int rotation {3};
    /* screen locations */
    const int left = 0;
    const int top = 30;
    const int bot = 305;
    const int mid = 155;
    const int tempy = 305;
    const int tempx =260;
    const int eley = 220;
    const int elex = 260;
    const int agly = 135;
    const int aglx = 260;

    /* methods */
      /* write lables to screen */
      void showLabels() {
        theTFT->setTextSize(1);
        theTFT->setFont(&ArialRoundedMTBold_36);
        theTFT->setCursor(left,top);
        theTFT->setTextColor(HX8357_RED, HX8357_BLACK);
        theTFT->print("WSR");
        theTFT->setTextColor(HX8357_WHITE, HX8357_BLACK);
        theTFT->print("703");
        theTFT->setCursor(left+37,agly-14);
        theTFT->println("Above Gnd:");
        theTFT->setCursor(left+68,eley-14);
        theTFT->println("Elevation:");
        theTFT->setCursor(left,bot-14);
        theTFT->println("Temperature:");
      }

      /* write current altitude to screen */
      void showElevation() {
        char data[10];
        static int oldValue;
        int newValue;
        newValue = current->elevation;
        theTFT->setTextSize(1);
        theTFT->setFont(&Font_32_Segment_7_Full);
        theTFT->setCursor(elex,eley);
        theTFT->setTextColor(HX8357_BLACK, HX8357_BLACK);
        theTFT->print(oldValue);
        theTFT->setCursor(elex,eley);
        theTFT->setTextColor(HX8357_GREEN, HX8357_BLACK);
        theTFT->print(newValue);
        oldValue = newValue;
      }

      /* write current altitude to screen */
      void showTemperature() {
        char data[10];
        static int oldValue;
        int newValue;
        newValue = current->temperature;
        theTFT->setTextSize(1);
        theTFT->setFont(&Font_32_Segment_7_Full);
        theTFT->setCursor(tempx,tempy);
        theTFT->setTextColor(HX8357_BLACK, HX8357_BLACK);
        theTFT->print(oldValue);
        theTFT->setCursor(tempx,tempy);
        theTFT->setTextColor(HX8357_GREEN, HX8357_BLACK);
        theTFT->print(newValue);
        oldValue = newValue;
      }

      /* write current altitude to screen */
      void showAGL() {
        char data[10];
        static int oldValue;
        int newValue;
        newValue = current->agl;
        theTFT->setTextSize(1);
        theTFT->setFont(&Font_32_Segment_7_Full);
        theTFT->setCursor(aglx,agly);
        theTFT->setTextColor(HX8357_BLACK, HX8357_BLACK);
        theTFT->print(oldValue);
        theTFT->setCursor(aglx,agly);
        theTFT->setTextColor(HX8357_GREEN, HX8357_BLACK);
        theTFT->print(newValue);
        oldValue = newValue;
      }

  public:
    /* Constructor */
    TFT35(Adafruit_HX8357* screen, Readings* values) : theTFT{screen}, current{values} {}
    TFT35(Adafruit_HX8357* screen,Readings* values, int rot) : theTFT{screen}, current{values}, rotation {rot} {}  

    /* Setup method */
    void setup() {
      /* startup the sensor */
      theTFT->begin(HX8357D);
      #ifdef debug_tft
        Serial.println("HX8357D starting up...");
      #endif
      theTFT->fillScreen(HX8357_BLACK);
      theTFT->setRotation(rotation);
      /* Read read diagnostics in debug mode */
      #ifdef debug_tft
        uint8_t x = theTFT->readcommand8(HX8357_RDPOWMODE);
        Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
        x = theTFT->readcommand8(HX8357_RDMADCTL);
        Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
        x = theTFT->readcommand8(HX8357_RDCOLMOD);
        Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
        x = theTFT->readcommand8(HX8357_RDDIM);
        Serial.print("Image Format: 0x"); Serial.println(x, HEX);
        x = theTFT->readcommand8(HX8357_RDDSDR);
        Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
        delay(1000);
      #endif
      /* Setup screen display */
      showLabels();
    }

    /* Loop method reads the sensor and sets global value */
    void loop() {
      showAGL();
      showElevation();
      showTemperature();
    }
};

/* Create the sensor instance */
TFT35 myScrren = TFT35(&myTFT,&curPressure);

#endif
