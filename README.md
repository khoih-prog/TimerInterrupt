# Arduino TimerInterrupt Library (now included in Arduino Library Manager)

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt.svg)](https://github.com/khoih-prog/TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt.svg)](http://github.com/khoih-prog/TimerInterrupt/issues)

---
---

This library enables you to use Interrupt from Hardware Timers on an Arduino, such as Nano, UNO, Mega, etc.

### Why do we need this Hardware Timer Interrupt?

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[HOWTO Attach Interrupt](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Important Notes:

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

## Prerequisite
1. [`Arduino IDE 1.8.12+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`Arduino AVR core 1.8.3+`](https://github.com/arduino/ArduinoCore-avr) for Arduino AVR boards. Use Arduino Board Manager to install.
3. [`Teensy core 1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (3.1, 3.0, LC, 2.0) boards.

---
---

## Installation

### Use Arduino Library Manager
The best and easiest way is to use `Arduino Library Manager`. Search for `TimerInterrupt`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [TimerInterrupt](https://github.com/khoih-prog/TimerInterrupt) page.
2. Download the latest release `TimerInterrupt-master.zip`.
3. Extract the zip file to `TimerInterrupt-master` directory 
4. Copy whole `TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:
1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install **TimerInterrupt** library by using [Library Manager](https://docs.platformio.org/en/latest/librarymanager/). Search for TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

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

---

### Example [Argument_Complex](examples/Argument_Complex)

```cpp
//These define's must be placed at the beginning before #include "TimerInterrupt.h"
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
  
  // The Serial.println is just for demo/testing purpose. Don't use because crash can happen.
  
  //timer interrupt toggles pins
  Serial.println("Toggle pin1 = " + String(((pinStruct *) outputPinsAddress)->Pin1) );
  digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle1);

  Serial.println("Read pin2 A0 (" + String(((pinStruct *) outputPinsAddress)->Pin2) + ") = " + 
        String( digitalRead(((pinStruct *) outputPinsAddress)->Pin2)? "HIGH" : "LOW" ) );
  
  Serial.println("Read pin3 A1 (" + String(((pinStruct *) outputPinsAddress)->Pin3) + ") = " + 
        String( digitalRead(((pinStruct *) outputPinsAddress)->Pin3)? "HIGH" : "LOW" ) );

  toggle1 = !toggle1;
}

#define TIMER1_INTERVAL_MS    1000

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
  
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, (unsigned int) &myOutputPins))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}

void loop()
{
}
```

---

## TO DO

1. Search for bug and improvement.
2. Similar libraries for many other boards (SAMD, nRF52, STM32, etc.)


## DONE

For current version v1.0.2

1. Longer Interval for timers.
2. Reduce code size if use less timers. Eliminate compiler warnings.
3. Now supporting complex object pointer-type argument.
3. 16 hardware-initiated software-enabled timers while using only 1 hardware timer.
4. Fix some bugs in v1.0.0
5. Add more examples.
6. Similar library for ESP32 and ESP8266

---

### Issues ###

Submit issues to: [TimerInterrupt issues](https://github.com/khoih-prog/TimerInterrupt/issues)

---

## Contributing
If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## Copyright
Copyright 2019- Khoi Hoang
