## TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt.svg)](https://github.com/khoih-prog/TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt.svg)](http://github.com/khoih-prog/TimerInterrupt/issues)
[![star this repo](https://githubbadges.com/star.svg?user=khoih-prog&repo=TimerInterrupt)](https://github.com/khoih-prog/TimerInterrupt)
[![fork this repo](https://githubbadges.com/fork.svg?user=khoih-prog&repo=TimerInterrupt)](https://github.com/khoih-prog/TimerInterrupt/fork)

---
---

This library enables you to use Interrupt from Hardware Timers on an Arduino, such as Nano, UNO, Mega, etc.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

### Why do we need this Hardware Timer Interrupt?

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Important Notes:

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

### Releases v1.1.1

1. Add example [**Change_Interval**](examples/Change_Interval)
2. Bump up version to sync with other TimerInterrupt Libraries. Modify Version String.
3. Add new h-only code besides conventional h/cpp code

### Release v1.0.3

1. Add example [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) and optimize example [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple) to demonstrate the usage of **16 ISR-based timers**

### Release v1.0.2

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds)
**The accuracy is nearly perfect** compared to software timers. The most important feature is they're ISR-based timers
Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

### Supported Arduino Boards

- Arduino Uno / Mega / Leonardo / Duemilanove / Diecimila / LilyPad / Mini / Fio / Nano etc.
- Teensy 1.0 / 1.0++ / 2.0 / 2++ / 3.0 / 3.1 / Teensy-LC;
- Sanguino
- ATmega8, 48, 88, 168, 328
- ATmega8535, 16, 32, 164, 324, 644, 1284,
- ATmega64, 128
- ATtiny 84 / 85

---
---


## Prerequisites

1. [`Arduino IDE 1.8.13+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`Arduino AVR core 1.8.3+`](https://github.com/arduino/ArduinoCore-avr) for Arduino AVR boards. Use Arduino Board Manager to install.
3. [`Teensy core 1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (3.1, 3.0, LC, 2.0) boards.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**TimerInterrupt**](https://github.com/khoih-prog/TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**TimerInterrupt**](https://github.com/khoih-prog/TimerInterrupt) page.
2. Download the latest release `TimerInterrupt-master.zip`.
3. Extract the zip file to `TimerInterrupt-master` directory 
4. Copy whole `TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**TimerInterrupt** library](https://platformio.org/lib/show/11520/TimerInterrupt) or [**TimerInterrupt** library](https://platformio.org/lib/show/6857/TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/11520/TimerInterrupt/installation). Search for TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---
---

## More useful Information

From [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)

#### 1. Timer0:

Timer0 is a 8-bit timer.

In the Arduino world timer0 is been used for the timer functions, like delay(), millis() and micros(). If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.

#### 2. Timer1:

Timer1 is a 16-bit timer.
In the Arduino world the Servo library uses timer1 on Arduino Uno (Timer5 on Arduino Mega).

#### 3. Timer2:

Timer2 is a 8-bit timer like Timer0.
In the Arduino work the tone() function uses Timer2.

#### 4. Timer3, Timer4, Timer5:

Timer 3,4,5 are only available on Arduino Mega boards. These timers are all 16-bit timers.

---
---

## New from v1.0.2

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds)
**The accuracy is nearly perfect** compared to software timers. The most important feature is they're ISR-based timers
Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

## Supported Arduino Boards

- Arduino Uno / Mega / Leonardo / Duemilanove / Diecimila / LilyPad / Mini / Fio / Nano etc.
- Teensy 1.0 / 1.0++ / 2.0 / 2++ / 3.0 / 3.1 / Teensy-LC;
- Sanguino
- ATmega8, 48, 88, 168, 328
- ATmega8535, 16, 32, 164, 324, 644, 1284,
- ATmega64, 128
- ATtiny 84 / 85

---


## Usage

Before using any Timer, you have to make sure the **Timer has not been used by any other purpose.**

Only Timer1 and Timer2 are supported for Nano, UNO, etc. boards possessing 3 timers.

Timer3, Timer4 and Timer5 are only available for Arduino Mega boards.

---
---

### How to use

```
//These define's must be placed at the beginning before #include "TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"

void TimerHandler1(void)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle1);
  toggle1 = !toggle1;
}

void TimerHandler2(void)
{
  static bool toggle2 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(A0, OUTPUT);
  }
  
  //timer interrupt toggles outputPin
  digitalWrite(A0, toggle2);
  toggle2 = !toggle2;
}

#define TIMER1_INTERVAL_MS    1000

#define TIMER2_INTERVAL_MS    2000

void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");

  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency
  ITimer1.init();
   
  // Using ATmega328 used in UNO => 16MHz CPU clock , 
  
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");

  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency
  ITimer2.init();
      
  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq. or timer");
}

void loop()
{
  
}

```

---
---

### Examples: 

 1. [Argument_Complex](examples/Argument_Complex)
 2. [Argument_None](examples/Argument_None)
 3. [Argument_Simple](examples/Argument_Simple) 
 4. [ISR_RPM_Measure](examples/ISR_RPM_Measure) 
 5. [ISR_Switch](examples/ISR_Switch)
 6. [ISR_Timer_Complex](examples/ISR_Timer_Complex)
 7. [ISR_Timer_Switch](examples/ISR_Timer_Switch)
 8. [ISR_Timer_Switches](examples/ISR_Timer_Switches) 
 9. [RPM_Measure](examples/RPM_Measure)
10. [SwitchDebounce](examples/SwitchDebounce)
11. [TimerDuration](examples/TimerDuration)
12. [TimerInterruptTest](examples/TimerInterruptTest)
13. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex). New.
14. [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple). New.
15. [**Change_Interval**](examples/Change_Interval). New.

---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```cpp
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     false
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"
#include "ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

ISR_Timer ISR_Timer2;

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER2_INTERVAL_MS            5L

volatile uint32_t startMillis = 0;

void TimerHandler2()
{
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_Timer2.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS) / TIMER2_INTERVAL_MS) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

  typedef struct 
  {
    irqCallback   irqCallbackFunc;
    uint32_t      TimerInterval;
    unsigned long deltaMillis;
    unsigned long previousMillis;
  } ISRTimerData;
  
  // In NRF52, avoid doing something fancy in ISR, for example Serial.print()
  // The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
  // Or you can get this run-time error / crash
  
  void doingSomething(int index);

#else

  volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  // You can assign any interval for any timer here, in milliseconds
  uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
  {
    5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
    45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    deltaMillis[index]    = currentMillis - previousMillis[index];
    previousMillis[index] = currentMillis;
  }

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

  ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
  {
    //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
    { doingSomething0,    5000L, 0, 0 },
    { doingSomething1,   10000L, 0, 0 },
    { doingSomething2,   15000L, 0, 0 },
    { doingSomething3,   20000L, 0, 0 },
    { doingSomething4,   25000L, 0, 0 },
    { doingSomething5,   30000L, 0, 0 },
    { doingSomething6,   35000L, 0, 0 },
    { doingSomething7,   40000L, 0, 0 },
    { doingSomething8,   45000L, 0, 0 },
    { doingSomething9,   50000L, 0, 0 },
    { doingSomething10,  55000L, 0, 0 },
    { doingSomething11,  60000L, 0, 0 },
    { doingSomething12,  65000L, 0, 0 },
    { doingSomething13,  70000L, 0, 0 },
    { doingSomething14,  75000L, 0, 0 },
    { doingSomething15,  80000L, 0, 0 }
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
    curISRTimerData[index].previousMillis = currentMillis;
  }

#else

  irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
  {
    doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
    doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
    doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
    doingSomething12, doingSomething13, doingSomething14, doingSomething15
  };

#endif

////////////////////////////////////////////////


#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  //Serial.printf("SimpleTimer : %lus, ms = %lu, Dms : %lu\n", SIMPLE_TIMER_MS / 1000, currMillis, currMillis - previousMillis);
  Serial.println("SimpleTimer : " + String(SIMPLE_TIMER_MS / 1000) + ", ms : " + String(currMillis) + ", Dms : " + String(currMillis - previousMillis));

  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.println("Timer : " +String(i) + ", programmed : " + String(curISRTimerData[i].TimerInterval) + ", actual : " + String(curISRTimerData[i].deltaMillis));
#else
    Serial.println("Timer : " +String(i) + ", programmed : " + String(TimerInterval[i]) + ", actual : " + String(deltaMillis[i]));
#endif    
  }

  previousMillis = currMillis;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting ISR_16_Timers_Array_Complex");
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  ITimer2.init();

  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq., duration or timer"); 

  //ISR_Timer2.setInterval(2000L, doingSomething2s);
  //ISR_Timer2.setInterval(5000L, doingSomething5s);

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_Timer2.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    ISR_Timer2.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif    
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```

---
---

### Debug Terminal Output Samples

1. The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **Arduino Nano V3** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 10.000s in loop().

```
Starting ISR_16_Timers_Array_Complex
TimerInterrupt v1.1.1
CPU Frequency = 16 MHz
Starting  ITimer2 OK, millis() = 1
SimpleTimer : 2, ms : 10007, Dms : 10007
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 10005
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20071, Dms : 10064
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30136, Dms : 10065
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 25007
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40202, Dms : 10066
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25007
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35011
Timer : 7, programmed : 40000, actual : 40007
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50270, Dms : 10068
Timer : 0, programmed : 5000, actual : 4998
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35011
Timer : 7, programmed : 40000, actual : 40007
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50007
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60338, Dms : 10068
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 14997
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 35011
Timer : 7, programmed : 40000, actual : 40007
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50007
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60006
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70408, Dms : 10070
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 14997
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 34999
Timer : 7, programmed : 40000, actual : 40007
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50007
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60006
Timer : 12, programmed : 65000, actual : 65008
Timer : 13, programmed : 70000, actual : 70010
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80479, Dms : 10071
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25001
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 34999
Timer : 7, programmed : 40000, actual : 40003
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50007
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60006
Timer : 12, programmed : 65000, actual : 65008
Timer : 13, programmed : 70000, actual : 70010
Timer : 14, programmed : 75000, actual : 75008
Timer : 15, programmed : 80000, actual : 80010

```

---

2. The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on AVR Mega2560 to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on AVR
TimerInterrupt v1.1.1
CPU Frequency = 16 MHz
Starting  ITimer1 OK, millis() = 1
Starting  ITimer2 OK, millis() = 4
Time = 10001, Timer1Count = 97, Timer2Count = 49
Time = 20002, Timer1Count = 195, Timer2Count = 99
Changing Interval, Timer1 = 200,  Timer2 = 400
Time = 30003, Timer1Count = 244, Timer2Count = 123
Time = 40004, Timer1Count = 294, Timer2Count = 148
Changing Interval, Timer1 = 100,  Timer2 = 200
Time = 50006, Timer1Count = 391, Timer2Count = 197
Time = 60007, Timer1Count = 489, Timer2Count = 247
Changing Interval, Timer1 = 200,  Timer2 = 400
Time = 70008, Timer1Count = 538, Timer2Count = 271
Time = 80009, Timer1Count = 588, Timer2Count = 296
Changing Interval, Timer1 = 100,  Timer2 = 200
```


---
---

### Releases v1.1.1

1. Add example [**Change_Interval**](examples/Change_Interval)
2. Bump up version to sync with other TimerInterrupt Libraries. Modify Version String.
3. Add new h-only code besides conventional h/cpp code

### Release v1.0.3

1. Add example [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) and optimize example [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple) to demonstrate the usage of **16 ISR-based timers**

### Release v1.0.2

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds)
**The accuracy is nearly perfect** compared to software timers. The most important feature is they're ISR-based timers
Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

### Supported Arduino Boards

- Arduino Uno / Mega / Leonardo / Duemilanove / Diecimila / LilyPad / Mini / Fio / Nano etc.
- Teensy 1.0 / 1.0++ / 2.0 / 2++ / 3.0 / 3.1 / Teensy-LC;
- Sanguino
- ATmega8, 48, 88, 168, 328
- ATmega8535, 16, 32, 164, 324, 644, 1284,
- ATmega64, 128
- ATtiny 84 / 85

---
---

### Issues ###

Submit issues to: [TimerInterrupt issues](https://github.com/khoih-prog/TimerInterrupt/issues)

---

### TO DO

1. Search for bug and improvement.


### DONE

For current version v1.0.2

1. Longer Interval for timers.
2. Reduce code size if use less timers. Eliminate compiler warnings.
3. Now supporting complex object pointer-type argument.
3. 16 hardware-initiated software-enabled timers while using only 1 hardware timer.
4. Fix some bugs in v1.0.0
5. Add more examples.
6. Similar library for ESP32, ESP8266, SAMD21/SAMD51, nRF52, Mbed-OS Nano-33-BLE, STM32


---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## License and credits

- The library is licensed under [MIT](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2019- Khoi Hoang


