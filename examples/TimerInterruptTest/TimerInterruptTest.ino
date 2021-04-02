/****************************************************************************************************************************
  TimerInterruptTest.ino
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

  Version: 1.4.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      23/11/2019 Initial coding
  1.0.1   K Hoang      25/11/2019 New release fixing compiler error
  1.0.2   K.Hoang      28/11/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
  1.0.3   K.Hoang      01/12/2020 Add complex examples ISR_16_Timers_Array_Complex and ISR_16_Timers_Array_Complex
  1.1.1   K.Hoang      06/12/2020 Add example Change_Interval. Bump up version to sync with other TimerInterrupt Libraries
  1.1.2   K.Hoang      05/01/2021 Fix warnings. Optimize examples to reduce memory usage
  1.2.0   K.Hoang      07/01/2021 Add better debug feature. Optimize code and examples to reduce RAM usage
  1.3.0   K.Hoang      25/02/2021 Add support to AVR ATMEGA_32U4 such as Leonardo, YUN, ESPLORA, etc.
  1.4.0   K.Hoang      01/04/2021 Add support to Adafruit 32U4 and 328(P) such as FEATHER32U4, FEATHER328P, etc.
 *****************************************************************************************************************************/

// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN     13
#endif

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )
void TimerHandler2(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle2 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("ITimer2 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles outputPin
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}
#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

#define TIMER1_INTERVAL_MS    10000
#define TIMER1_FREQUENCY      (float) (1000.0f / TIMER1_INTERVAL_MS)
#define TIMER1_DURATION_MS    0 //(10 * TIMER1_INTERVAL_MS)

#define TIMER2_INTERVAL_MS    13000
#define TIMER2_FREQUENCY      (float) (1000.0f / TIMER2_INTERVAL_MS)
#define TIMER2_DURATION_MS    0   //(20 * TIMER2_INTERVAL_MS)

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("\nStarting TimerInterruptTest on AVR"));
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

  ITimer2.init();

  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer2 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer2. Select another freq. or timer"));
    
#endif    
}

void loop()
{

#if 0
  static unsigned long lastTimer1 = 0;
  static unsigned long lastTimer2 = 0;

  static bool timerPaused         = false;
  static bool timerResumed        = false;

  if (millis() - lastTimer1 > TIMER1_DURATION_MS * 3)
  {
#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )    
    if (millis() - lastTimer2 > TIMER2_DURATION_MS * 3)
    {
      lastTimer2 = millis();
      
      Serial.print(F("Re-enable ITimer2, millis() = ")); Serial.println(lastTimer2);
      
      ITimer2.reattachInterrupt(TIMER2_DURATION_MS);
    }
#endif

    lastTimer1 = millis();
    // try reinit timer
    Serial.print(F("Re-enable ITimer1, millis() = ")); Serial.print(lastTimer1);
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount());

    ITimer1.reattachInterrupt(TIMER1_DURATION_MS);
    timerPaused   = false;
    timerResumed  = false;
  }
  else if ( !timerPaused && (millis() - lastTimer1 > TIMER1_DURATION_MS / 2) )
  {
    timerPaused = true;

    Serial.print(F("Pause ITimer1, millis() = ")); Serial.print(millis());
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount());
    
    ITimer1.pauseTimer();
  }
  else if ( !timerResumed && (millis() - lastTimer1 > ( TIMER1_DURATION_MS * 3 ) / 2) )
  {
    timerResumed = true;
    
    Serial.print(F("Resume ITimer1, millis() = ")); Serial.print(millis());
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount());
    
    ITimer1.resumeTimer();
  }
#endif

}
