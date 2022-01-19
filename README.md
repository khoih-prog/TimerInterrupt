## TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt.svg)](https://github.com/khoih-prog/TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt.svg)](http://github.com/khoih-prog/TimerInterrupt/issues)

---
---

## Table of Contents

* [Why do we need this TimerInterrupt library](#why-do-we-need-this-timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Important Notes about ISR](#important-notes-about-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
  * [1. Timer0](#1-timer0)
  * [2. Timer1](#2-timer1)
  * [3. Timer2](#3-timer2)
  * [4. Timer3, Timer4, Timer5](#4-timer3-timer4-timer5)
  * [5. Important Notes](#5-important-notes)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
    * [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Important Note](#21-important-note)
    * [2.2 Init Hardware Timer and ISR-based Timer](#22-init-hardware-timer-and-isr-based-timer)
    * [2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#23-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_Complex](examples/Argument_Complex)
  * [  2. Argument_None](examples/Argument_None)
  * [  3. Argument_Simple](examples/Argument_Simple)
  * [  4. Change_Interval](examples/Change_Interval)
  * [  5. FakeAnalogWrite](examples/FakeAnalogWrite)
  * [  6. ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)
  * [  7. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  8. ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple)
  * [  9. RPM_Measure](examples/RPM_Measure)
  * [ 10. SwitchDebounce](examples/SwitchDebounce)
  * [ 11. TimerDuration](examples/TimerDuration)
  * [ 12. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 13. Change_Interval_HF](examples/Change_Interval_HF).
  * [ 14. Argument_Complex_Multi](examples/Argument_Complex_Multi). **New**
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on Arduino AVR Nano-V3 board](#1-isr_16_timers_array_complex-on-arduino-avr-nano-v3-board)
  * [2. Change_Interval on Arduino AVR Mega2560 board](#2-change_interval-on-arduino-avr-mega2560-board)
  * [3. Change_Interval_HF on Arduino AVR Nano board](#3-change_interval_hf-on-arduino-avr-nano-board)
  * [4. Change_Interval_HF on Arduino AVR Mega2560 board](#4-change_interval_hf-on-arduino-avr-mega2560-board)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [TimerInterrupt library](https://github.com/khoih-prog/TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on an Arduino or Adafruit AVR board, such as Nano, UNO, Mega, AVR_FEATHER32U4, etc.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Important Notes about ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

### Currently supported Boards

- Arduino Uno / Mega / Duemilanove / Diecimila / LilyPad / Mini / Fio / Nano, etc.
- **Arduino ATMega 16U4, 32U4** such as AVR Leonardo, Leonardo ETH, YUN, Esplora, LILYPAD_USB, AVR_ROBOT_CONTROL, AVR_ROBOT_MOTOR, AVR_INDUSTRIAL101, etc.
- **Adafruit ATMega 32U4** such as AVR_FLORA8, AVR_FEATHER32U4, AVR_CIRCUITPLAY, AVR_ITSYBITSY32U4_5V, AVR_ITSYBITSY32U4_3V, AVR_BLUEFRUITMICRO, AVR_ADAFRUIT32U4, etc.
- **Adafruit ATMega 328(P)** such as AVR_METRO, AVR_FEATHER328P, AVR_PROTRINKET5, AVR_PROTRINKET3, AVR_PROTRINKET5FTDI, AVR_PROTRINKET3FTDI, etc.
- **Generic or Sparkfun AVR ATmega_32U4** such as **AVR_MAKEYMAKEY, AVR_PROMICRO, etc.**
- **Generic or Sparkfun AVR ATmega_328(P)** such as **ARDUINO_REDBOT, ARDUINO_AVR_DIGITAL_SANDBOX, etc.**
- **Generic or Sparkfun AVR ATmega128RFA1** such as **ATMEGA128RFA1_DEV_BOARD, etc.**

---
---


## Prerequisites

1. [`Arduino IDE 1.8.16+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`Arduino AVR core 1.8.3+`](https://github.com/arduino/ArduinoCore-avr) for Arduino AVR boards. Use Arduino Board Manager to install. [![Latest release](https://img.shields.io/github/release/arduino/ArduinoCore-avr.svg)](https://github.com/arduino/ArduinoCore-avr/releases/latest/)
3. [`Adafruit AVR core 1.4.14+`](https://github.com/adafruit/Adafruit_Arduino_Boards) for Adafruit AVR boards. Use Arduino Board Manager to install. 
4. [`Sparkfun AVR core 1.1.13+`](https://github.com/sparkfun/Arduino_Boards) for Sparkfun AVR boards. Use Arduino Board Manager to install. 

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

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can use

```
#include <TimerInterrupt.hpp>         //https://github.com/khoih-prog/TimerInterrupt
#include <ISR_Timer.hpp>              //https://github.com/khoih-prog/TimerInterrupt
```

in many files. But be sure to use the following `#include <TimerInterrupt.h>` or `#include <ISR_Timer.h>` **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "TimerInterrupt.h"           //https://github.com/khoih-prog/TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ISR_Timer.h"                //https://github.com/khoih-prog/TimerInterrupt
```

Check new [**Argument_Complex_Multi** example](examples/Argument_Complex_Multi) for the demo how to avoid `Multiple Definitions` Linker Error. 

---
---

## More useful Information

From [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)

### 1. Timer0:

Timer0 is a 8-bit timer.

In the Arduino world, **Timer0 is been used for the timer functions**, like delay(), millis() and micros(). If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.

### 2. Timer1:

**Timer1** is a 16-bit timer. In the Arduino world, the Servo library uses Timer1 on Arduino Uno (Timer5 on Arduino Mega).

### 3. Timer2:

**Timer2** is a 8-bit timer like Timer0. This Timer2 is not available for **ATMEGA_16U4, ATMEGA_32U4 boards**, such as Leonardo, YUN, ESPLORA, etc.
In the Arduino world, the **tone() function uses Timer2**.

### 4. Timer3, Timer4, Timer5:

**Timer4** is only available on Arduino **ATMEGA_2560, ATMEGA_1280, ATMEGA_640, ATMEGA_16U4, ATMEGA_32U4 boards**. This Timer4 is 16-bit timer on **ATMEGA_2560, ATMEGA_1280, ATMEGA_640 boards** and 10-bit (but used as 8-bit in this library) Timer on **ATMEGA_16U4, ATMEGA_32U4 boards**

**Timer3 and Timer5** are only available on Arduino Mega boards. These 2 timers are all 16-bit timers.


### 5. Important Notes

Before using any Timer, you have to make sure the **Timer has not been used by any other purpose.**


#### Timer1,3,4 are supported for ATMEGA_16U4, ATMEGA_32U4 boards, such as Leonardo, YUN, ESPLORA, etc.

#### Only Timer1 and Timer2 are supported for Nano, UNO, etc. boards possessing 3 timers.

#### Timer1-Timer5 are available for Arduino Mega boards.

---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

### 1.1 Init Hardware Timer

```
// Select the timers you're using, here ITimer1
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

// Init timer ITimer1
ITimer1.init();
```

### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

Use one of these functions with **interval in unsigned long milliseconds**

```
// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
```

as follows

```
void TimerHandler()
{
  // Doing something here inside ISR
}

#define TIMER_INTERVAL_MS        50L

void setup()
{
  ....
  
  // Interval in unsigned long millisecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```

### 1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function

Use one of these functions with **frequency in float Hz**

```
// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0);
```

as follows

```
void TimerHandler()
{
  // Doing something here inside ISR
}

#define TIMER_FREQ_HZ        5555.555

void setup()
{
  ....
  
  // Frequency in float Hz
  if (ITimer.attachInterrupt(TIMER_FREQ_HZ, TimerHandler))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```


### 2. Using 16 ISR_based Timers from 1 Hardware Timer

### 2.1 Important Note

The 16 ISR_based Timers, designed for long timer intervals, only support using **unsigned long millisec intervals**. If you have to use much higher frequency or sub-millisecond interval, you have to use the Hardware Timers directly as in [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)

### 2.2 Init Hardware Timer and ISR-based Timer

```
#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define USE_TIMER_1     true
  #warning Using Timer1
#else          
  #define USE_TIMER_3     true
  #warning Using Timer3
#endif

// Init ISR_Timer
// Each ISR_Timer can service 16 different ISR-based timers
ISR_Timer ISR_timer;
```

### 2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler()
{
  ISR_timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In AVR, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
}
  
void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds
}

void doingSomething101s()
{
  // Doing something here inside ISR every 101 seconds
}

void setup()
{
  ....
  
#if USE_TIMER_1

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // Interval in millisecs
  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();
  
  // Interval in millisecs
  if (ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---
---

### Examples: 

 1. [Argument_Complex](examples/Argument_Complex)
 2. [Argument_None](examples/Argument_None)
 3. [Argument_Simple](examples/Argument_Simple)
 4. [Change_Interval](examples/Change_Interval)
 5. [FakeAnalogWrite](examples/FakeAnalogWrite)
 6. [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)
 7. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 8. [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple)
 9. [RPM_Measure](examples/RPM_Measure)
10. [SwitchDebounce](examples/SwitchDebounce)
11. [TimerDuration](examples/TimerDuration)
12. [TimerInterruptTest](examples/TimerInterruptTest)
13. [**Change_Interval_HF**](examples/Change_Interval_HF).
14. [**Argument_Complex_Multi**](examples/Argument_Complex_Multi). **New**

---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```cpp
// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define USE_TIMER_1     true
  #warning Using Timer1
#else          
  #define USE_TIMER_3     true
  #warning Using Timer3
#endif

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "TimerInterrupt.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

ISR_Timer ISR_timer;

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER_INTERVAL_MS            5L

volatile uint32_t startMillis = 0;

void TimerHandler()
{
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS) / TIMER_INTERVAL_MS) )
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

  Serial.print(F("SimpleTimer : "));Serial.print(SIMPLE_TIMER_MS / 1000);
  Serial.print(F(", ms : ")); Serial.print(currMillis);
  Serial.print(F(", Dms : ")); Serial.println(currMillis - previousMillis);

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(curISRTimerData[i].TimerInterval);
    Serial.print(F(", actual : ")); Serial.println(curISRTimerData[i].deltaMillis);
#else
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(TimerInterval[i]);
    Serial.print(F(", actual : ")); Serial.println(deltaMillis[i]);
#endif    
  }

  previousMillis = currMillis;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on "));
  Serial.println(BOARD_TYPE);
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 1-5 for MEGA, 1,3,4 for 16u4/32u4
  // Timer 2 is 8-bit timer, only for higher frequency
  // Timer 4 of 16u4 and 32u4 is 8/10-bit timer, only for higher frequency
  
#if USE_TIMER_1

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif

  //ISR_timer.setInterval(2000L, doingSomething2s);
  //ISR_timer.setInterval(5000L, doingSomething5s);

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    ISR_timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
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

### 1. ISR_16_Timers_Array_Complex on Arduino AVR Nano-V3 board

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **Arduino Nano V3** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 10.000s in loop().

```

Starting ISR_16_Timers_Array_Complex on Arduino AVR UNO, Nano, etc.
TimerInterrupt v1.8.0
CPU Frequency = 16 MHz
Starting  ITimer1 OK, millis() = 7
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

### 2. Change_Interval on Arduino AVR Mega2560 board

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on **AVR Mega2560** to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on Arduino AVR Mega2560/ADK
TimerInterrupt v1.8.0
CPU Frequency = 16 MHz
Starting  ITimer1 OK, millis() = 5
Starting  ITimer3 OK, millis() = 8
Time = 10001, Timer1Count = 97, TimerCount = 49
Time = 20002, Timer1Count = 195, TimerCount = 99
Changing Interval, Timer1 = 200
Changing Interval, Timer3 = 400
Time = 30003, Timer1Count = 244, TimerCount = 123
Time = 40004, Timer1Count = 294, TimerCount = 148
Changing Interval, Timer1 = 100
Changing Interval, Timer3 = 200
Time = 50006, Timer1Count = 391, TimerCount = 197
Time = 60007, Timer1Count = 489, TimerCount = 247
Changing Interval, Timer1 = 200
Changing Interval, Timer3 = 400
Time = 70008, Timer1Count = 538, TimerCount = 271
Time = 80009, Timer1Count = 588, TimerCount = 296
```

---

### 3. Change_Interval_HF on Arduino AVR Nano board

The following is the sample terminal output when running example [Change_Interval_HF](examples/Change_Interval_HF) on **AVR Nano** to demonstrate how to change High Frequency Timer Interval on-the-fly

```
Starting Change_Interval_HF on Arduino AVR UNO, Nano, etc.
TimerInterrupt v1.8.0
CPU Frequency = 16 MHz
[TISR] T1
[TISR] Freq * 1000 = 5000000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 3199 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 3199
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR1B = 9
Starting  ITimer1 OK, millis() = 24
[TISR] T2
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR2 = 15999 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 15999
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR2B = 1
Starting  ITimer2 OK, millis() = 43
Time = 10001, Timer1Count = 49905, TimerCount = 9886
Time = 20002, Timer1Count = 99912, TimerCount = 19808
[TISR] Freq * 1000 = 2500000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 6399 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 6399
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR1B = 9
Changing Frequency, Timer1 = 2500
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR2 = 31999 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 31999
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR2B = 1
Changing Frequency, Timer2 = 500
```

---

### 4. Change_Interval_HF on Arduino AVR Mega2560 board

The following is the sample terminal output when running example [Change_Interval_HF](examples/Change_Interval_HF) on **AVR Mega2560** to demonstrate how to change High Frequency Timer Interval on-the-fly

```

Starting Change_Interval_HF on Arduino AVR Mega2560/ADK
TimerInterrupt v1.8.0
CPU Frequency = 16 MHz
[TISR] T1
[TISR] Freq * 1000 = 5000000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 3199 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 3199
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR1B = 9
Starting  ITimer1 OK, millis() = 24
[TISR] T3
[TISR] Freq * 1000 = 1000000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 15999 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 15999
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
Starting  ITimer3 OK, millis() = 45
Time = 10001, Timer1Count = 49897, TimerCount = 9960
Time = 20002, Timer1Count = 99904, TimerCount = 19961
[TISR] Freq * 1000 = 2500000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 6399 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 6399
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
[TISR] TCCR1B = 9
Changing Frequency, Timer1 = 2500
[TISR] Freq * 1000 = 500000.00
[TISR] F_CPU = 16000000 , preScalerDiv = 1
[TISR] OCR = 31999 , preScalerIndex = 1
[TISR] OK in loop => _OCR = 31999
[TISR] _preScalerIndex = 1 , preScalerDiv = 1
Changing Frequency, Timer3 = 500

```

---
---


### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 3

```cpp
// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---


### Issues

Submit issues to: [TimerInterrupt issues](https://github.com/khoih-prog/TimerInterrupt/issues)

---
---

### TO DO

1. Search for bug and improvement.


### DONE

 1. Longer Interval for timers.
 2. Reduce code size if use less timers. Eliminate compiler warnings.
 3. Now supporting complex object pointer-type argument.
 3. 16 hardware-initiated software-enabled timers while using only 1 hardware timer.
 4. Fix some bugs in v1.0.0
 5. Add more examples.
 6. Similar library for ESP32, ESP8266, SAMD21/SAMD51, nRF52, Mbed-OS Nano-33-BLE, STM32
 7. Add support to **Arduino ATMega-16U4, ATMega-32U4**-based boards
 8. Add support to **Adafruit ATMega-32U4**-based boards
 9. Add support to **Adafruit ATMega-328(P)**-based boards
10. Add support to **Generic or Sparkfun AVR ATmega_32U4** such as **AVR_MAKEYMAKEY, AVR_PROMICRO, etc.**
11. Add support to **Generic or Sparkfun AVR ATmega_328(P)** such as **ARDUINO_REDBOT, ARDUINO_AVR_DIGITAL_SANDBOX, etc.**
12. Add support to **Generic or Sparkfun AVR ATmega128RFA1** such as **ATMEGA128RFA1_DEV_BOARD, etc.**
13. Add Timer3 and Timer4 support to **ATmega32U4 and ATmega16U4**.
14. Fix bug resulting half frequency when using high frequencies.
15. Fix bug resulting wrong frequency for some low frequencies.
16. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories


---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [TimerInterrupt library](https://github.com/khoih-prog/TimerInterrupt)

1. Thanks to [Django0](https://github.com/Django0) to provide the following PR [Fixed warnings from cppcheck (platformio) and -Wall arduino-cli. PR#10](https://github.com/khoih-prog/TimerInterrupt/pull/10).
2. Thanks to [eslavko](https://github.com/eslavko) to report the issue [Error compiling for arduino leonardo #13](https://github.com/khoih-prog/TimerInterrupt/issues/13) leading to new release v1.3.0 to provide support to **Arduino ATMega-16U4, ATMega-32U4**-based boards, such as AVR Leonardo, Leonardo ETH, YUN, Esplora, LILYPAD_USB, AVR_ROBOT_CONTROL, AVR_ROBOT_MOTOR, AVR_INDUSTRIAL101, etc..
3. Thanks to [bzuidgeest](https://github.com/bzuidgeest) to report the issue [Adafruit feather 32u4 #17](https://github.com/khoih-prog/TimerInterrupt/issues/17) leading to new release v1.4.0 to provide support to Adafruit **ATMega-32U4**-based boards, such as AVR_FLORA8, AVR_FEATHER32U4, AVR_CIRCUITPLAY, AVR_ITSYBITSY32U4_5V, AVR_ITSYBITSY32U4_3V, AVR_BLUEFRUITMICRO, AVR_ADAFRUIT32U4, etc. and **Adafruit ATMega-328(P)**-based boards, such as AVR_METRO, AVR_FEATHER328P, AVR_PROTRINKET5, AVR_PROTRINKET3, AVR_PROTRINKET5FTDI, AVR_PROTRINKET3FTDI, etc.
4. Thanks to [joonghochoe](https://github.com/joonghochoe) to report the issue [Timer3/4 in Arduino Micro board #18](https://github.com/khoih-prog/TimerInterrupt/issues/18) leading to new release v1.5.0 to provide Timer3 and 4 support to ATmega32U4 and ATmega16U4.
5. Thanks to [Sean Ison](https://github.com/sison54) to report the issue [Frequencies are half of what I expect #22](https://github.com/khoih-prog/TimerInterrupt/discussions/22) leading to new release v1.6.0 to fix bug in using higher frequencies than 250Hz.


<table>
  <tr>
    <td align="center"><a href="https://github.com/Django0"><img src="https://github.com/Django0.png" width="100px;" alt="Django0"/><br /><sub><b>Django0</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/eslavko"><img src="https://github.com/eslavko.png" width="100px;" alt="eslavko"/><br /><sub><b>eslavko</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/bzuidgeest"><img src="https://github.com/bzuidgeest.png" width="100px;" alt="bzuidgeest"/><br /><sub><b>bzuidgeest</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/joonghochoe"><img src="https://github.com/joonghochoe.png" width="100px;" alt="joonghochoe"/><br /><sub><b>joonghochoe</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/sison54"><img src="https://github.com/sison54.png" width="100px;" alt="sison54"/><br /><sub><b>Sean Ison</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## License

- The library is licensed under [MIT](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2019- Khoi Hoang


