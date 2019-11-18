/************************************************
 * examples/Argument_Simple.ino
 * For Arduino AVR boards
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
 * Licensed under MIT license
 * Version: v1.0.1
 * 
 * Notes:
 * Special design is necessary to share data between interrupt code and the rest of your program.
 * Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume 
 * variable can not spontaneously change. Because your function may change variables while your program is using them, 
 * the compiler needs this hint. But volatile alone is often not enough.
 * When accessing shared variables, usually interrupts must be disabled. Even with volatile, 
 * if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly. 
 * If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled 
 * or the entire sequence of your code which accesses the data.
 *
 ************************************************/
//These define's must be placed at the beginning before #include "TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }
  
  //timer interrupt toggles pin outputPin, default LED_BUILTIN
  Serial.println("pin1 = " + String(outputPin) + " address: " + String((uint32_t) &outputPin) );
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

void TimerHandler2(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle2 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }
  
  //timer interrupt toggles pin outputPin, default LED_BUILTIN
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}

#define TIMER1_INTERVAL_MS    1000 

#define TIMER2_INTERVAL_MS    2000 

void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency 
  
  ITimer1.init();
   
  // Using ATmega328 used in UNO => 16MHz CPU clock , 
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz
  
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1))
  {
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
    Serial.println("OutputPin1 = " + String(outputPin1) + ", address = " + String((uint32_t) &outputPin1) );
  }
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
    
  ITimer2.init();
      
  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq. or timer");    
}

void loop()
{  
}
