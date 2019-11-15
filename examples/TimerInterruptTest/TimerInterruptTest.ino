/************************************************
 * TimerInterruptTest.ino
 * For Arduino AVR boards
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
 * Licensed under MIT license
 * Version: 1.0.0
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

#include "TimerInterrupt.h"

#define LOCAL_DEBUG       1

//volatile bool toggle = false;

void TimerHandler0(void)
{
  static volatile bool toggle0 = false;
  
  //timer interrupt toggles pin A1
  digitalWrite(A1, toggle0);
  toggle0 = !toggle0;
}

void TimerHandler1(const unsigned int outputPin = LED_BUILTIN)
{
  static volatile bool toggle1 = false;
  
  //timer interrupt toggles pin LED_BUILTIN
  //digitalWrite(LED_BUILTIN, toggle1);
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

void TimerHandler2(const unsigned int outputPin = LED_BUILTIN)
{
  static volatile bool toggle2 = false;
  
  //timer interrupt toggles outputPin
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}

const unsigned int outputPin = A0;
const unsigned int outputPin1 = LED_BUILTIN;

#define TIMER1_DURATION_MS  5000L
#define TIMER1_FREQUENCY    10
#define TIMER1_INTERVAL_MS  100

#define TIMER2_DURATION_MS  20000L
#define TIMER2_FREQUENCY    100
#define TIMER2_INTERVAL_MS  10

void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");
  pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(outputPin, OUTPUT);
  
  pinMode(A1, OUTPUT);

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency 
  
  ITimer1.init();
   
  // Using ATmega328 used in UNO => 16MHz CPU clock , 
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz
  
  //if (ITimer1.attachInterrupt(TIMER1_FREQUENCY, TimerHandler1, outputPin1 /*LED_BUILTIN*/, TIMER1_DURATION_MS))
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1 /*LED_BUILTIN*/, TIMER1_DURATION_MS))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
    
  ITimer2.init();
      
  //if (ITimer2.attachInterrupt(TIMER2_FREQUENCY, TimerHandler2, outputPin, TIMER2_DURATION_MS))
  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin, TIMER2_DURATION_MS))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq. or timer");    
}

void loop()
{
  static unsigned long lastTimer1 = 0;
  static unsigned long lastTimer2 = 0;
  
  static bool timerPaused         = false;
  static bool timerResumed        = false;
  
  if (millis() - lastTimer1 > TIMER1_DURATION_MS * 3)
  {
    if (millis() - lastTimer2 > TIMER2_DURATION_MS * 3)
    {
      lastTimer2 = millis();
      Serial.println("Re-enable ITimer2, millis() = " + String(millis()));
      ITimer2.reattachInterrupt(TIMER2_DURATION_MS);
    }
    
    lastTimer1 = millis();
    // try reinit timer
    Serial.println("Re-enable ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.reattachInterrupt(TIMER1_DURATION_MS);
    timerPaused   = false;
    timerResumed  = false;
  }
  else if ( !timerPaused && (millis() - lastTimer1 > TIMER1_DURATION_MS / 2) )
  {
    timerPaused = true;
    Serial.println("Pause ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.pauseTimer();
  }
  else if ( !timerResumed && (millis() - lastTimer1 > ( TIMER1_DURATION_MS * 3 ) / 2) )
  {
    timerResumed = true;
    Serial.println("Resume ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.resumeTimer();
  }
}
