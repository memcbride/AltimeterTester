/***************************************************************************
 An Altimeter Tester for checking dual deploy events.  Design is by Dave
 Combs of WSR703 (www.WSR703.org) and MacModeler. 

 The "Globals.h" header file defines all the global definitions, enums,
 functions, etc.
 
  Author:  Mark E. McBride
  Last Update:  2020.01.22

  ChangeLog:
  next up:     n/a
  2020.01.25:  Changed readings to rounding to integers
  2020.01.25:  Adjusted what is being sampled
  2020.01.19:  Finalized BMP3XX related globals
  2020.01.17:  Setup initial file.
  
 ***************************************************************************/
#ifndef Global_H
#define Global_H

/* Arduino libraries */

/* MacModeler Altimeter header files */

/* BMP388 Sensor Defines */
/* sensor data structure */
typedef struct Readings {
//  unsigned long sTime = 1000;
  int temperature = 24;
//  double pressure = 990.00;
  int elevation = 900;
  int agl = 0;
};
Readings curPressure;

#endif
