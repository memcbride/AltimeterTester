/***************************************************************************
 An Altimeter Tester for checking dual deploy events.  Design is by Dave
 Combs of WSR703 (www.WSR703.org) and MacModeler. 

 The "Runnable.h"  class header file defines the Runnable class for managing
 the time-slicing, setup, and loops for the other objects.  The class is 
 straight from: http://paulmurraycbr.github.io/ArduinoTheOOWay.html which was 
 particularly helpful.  
 
  Author:  Mark E. McBride
  Last Update:  2019.12.25

  ChangeLog:
  next up:     n/a
  2019.12.19:  setup initial file for project
  
 ***************************************************************************/
#ifndef Runnable_H
#define Runnable_H

/* Arduino Libraries */

/* MacModeler Headers */
#include "Globals.h"

class Runnable {
  private:
    /* start of the linked list of runnable classes */
    static Runnable *headRunnable;
    Runnable *nextRunnable;
    
  protected:
  
  public:
    Runnable() {
      nextRunnable = headRunnable;
      headRunnable = this;
    }
    virtual void setup() = 0;
    virtual void loop() = 0;
    static void setupAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable)
        r->setup();
    }
    static void loopAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable)
        r->loop();
    }
};

/* define the runnable object for the code */
Runnable *Runnable::headRunnable = NULL;

#endif
