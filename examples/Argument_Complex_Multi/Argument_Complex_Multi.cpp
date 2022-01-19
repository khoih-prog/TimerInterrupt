/****************************************************************************************************************************
  Argument_Complex_Multi.cpp
  For Arduino and Adadruit AVR 328(P) and 32u4 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
  Licensed under MIT license

  Now we can use these new 16 ISR-based timers, while consuming only 1 hardware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Notes:
  Special design is necessary to share data between interrupt code and the rest of your program.
  Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
  variable can not spontaneously change. Because your function may change variables while your program is using them,
  the compiler needs this hint. But volatile alone is often not enough.
  When accessing shared variables, usually interrupts must be disabled. Even with volatile,
  if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
  If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
  or the entire sequence of your code which accesses the data.
*****************************************************************************************************************************/

// To demonstrate the usage of complex multiple files to avoid `multi definition linker` error
// by using TimerInterrupt.hpp in multiple files but TimerInterrupt.h in only main file

#include "Argument_Complex_Multi.h"

void TimerHandler(unsigned int outputPinsAddress)
{
  static bool toggle = false;

  //timer interrupt toggles pins
#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("Toggle pin1 = "); Serial.println( ((pinStruct *) outputPinsAddress)->Pin1 );
#endif
  
  digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle);

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("Read pin2 A0 ("); Serial.print(((pinStruct *) outputPinsAddress)->Pin2 );
  Serial.print(") = ");
  Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin2) ? "HIGH" : "LOW" );                          

  Serial.print("Read pin3 A1 ("); Serial.print(((pinStruct *) outputPinsAddress)->Pin3 );
  Serial.print(") = ");
  Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin3) ? "HIGH" : "LOW" );  
#endif
                 
  toggle = !toggle;
}
