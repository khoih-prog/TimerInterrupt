/****************************************************************************************************************************
  Argument_Complex.ino
  For Arduino boards (UNO, Nano, Mega, etc. )
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
  
  Version: 1.1.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      23/11/2019 Initial coding
  1.0.1   K Hoang      25/11/2019 New release fixing compiler error
  1.0.2   K.Hoang      28/11/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
  1.0.3   K.Hoang      01/12/2020 Add complex examples ISR_16_Timers_Array_Complex and ISR_16_Timers_Array_Complex
  1.1.1   K.Hoang      06/12/2020 Add example Change_Interval. Bump up version to sync with other TimerInterrupt Libraries
  1.1.2   K.Hoang      05/01/2021 Fix warnings. Optimize examples to reduce memory usage
*****************************************************************************************************************************/

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || \
    defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || \
    defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || \
    defined(ARDUINO_AVR_MINI) || defined(ARDUINO_AVR_ETHERNET) || defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT) || \
    defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED)

#else
  #error This is designed only for Arduino AVR board! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"

struct pinStruct
{
  unsigned int Pin1;
  unsigned int Pin2;
  unsigned int Pin3;
};

volatile pinStruct myOutputPins = { LED_BUILTIN, A0, A1 };

void TimerHandler1(unsigned int outputPinsAddress)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(((pinStruct *) outputPinsAddress)->Pin1, OUTPUT);
    pinMode(((pinStruct *) outputPinsAddress)->Pin2, INPUT_PULLUP);
    pinMode(((pinStruct *) outputPinsAddress)->Pin3, INPUT_PULLUP);
  }

  //timer interrupt toggles pins
#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("Toggle pin1 = "); Serial.println( ((pinStruct *) outputPinsAddress)->Pin1 );
#endif
  
  digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle1);

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("Read pin2 A0 ("); Serial.print(((pinStruct *) outputPinsAddress)->Pin2 );
  Serial.print(") = ");
  Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin2) ? "HIGH" : "LOW" );                          

  Serial.print("Read pin3 A1 ("); Serial.print(((pinStruct *) outputPinsAddress)->Pin3 );
  Serial.print(") = ");
  Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin3) ? "HIGH" : "LOW" );  
#endif
                 
  toggle1 = !toggle1;
}

#define TIMER1_INTERVAL_MS    1000

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("\nStarting Argument_Complex on AVR"));
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, (unsigned int) &myOutputPins))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
}
