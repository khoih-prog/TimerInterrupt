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
3. Install [**TimerInterrupt** library](https://platformio.org/lib/show/6857/TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/6857/TimerInterrupt/installation). Search for TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
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
13. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex). New.
14. [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple). New.

---

### Example [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple)

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

ISR_Timer ISR_Timer2;

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER2_INTERVAL_MS            1L

volatile uint32_t startMillis = 0;

volatile uint32_t deltaMillis2s = 0;
volatile uint32_t deltaMillis5s = 0;

volatile uint32_t previousMillis2s = 0;
volatile uint32_t previousMillis5s = 0;


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

void doingSomething2s()
{
  unsigned long currentMillis  = millis();

  deltaMillis2s    = currentMillis - previousMillis2s;
  previousMillis2s = currentMillis;
}

void doingSomething5s()
{
  unsigned long currentMillis  = millis();

  deltaMillis5s    = currentMillis - previousMillis5s;
  previousMillis5s = currentMillis;
}

/////////////////////////////////////////////////

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

  Serial.println("SimpleTimer : programmed " + String(SIMPLE_TIMER_MS) + "ms, current time ms : " + String(currMillis) + ", Delta ms : " + String(currMillis - previousMillis));

  Serial.println("Timer2s actual : " + String(deltaMillis2s));
  Serial.println("Timer5s actual : " + String(deltaMillis5s));
  
  previousMillis = currMillis;
}

////////////////////////////////////////////////

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting ISR_Timers_Array_Simple");
  Serial.println("Version : " + String(TIMER_INTERRUPT_VERSION));
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  ITimer2.init();

  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq., duration or timer");

  ISR_Timer2.setInterval(2000L, doingSomething2s);
  ISR_Timer2.setInterval(5000L, doingSomething5s);

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
Version : v1.0.3
CPU Frequency = 16 MHz
Starting  ITimer2 OK, millis() = 0
SimpleTimer : 2, ms : 10005, Dms : 10005
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 10004
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
SimpleTimer : 2, ms : 20065, Dms : 10060
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20008
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
SimpleTimer : 2, ms : 30125, Dms : 10060
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30007
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
SimpleTimer : 2, ms : 40185, Dms : 10060
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30007
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40006
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50247, Dms : 10062
Timer : 0, programmed : 5000, actual : 4998
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30007
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40006
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50006
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60309, Dms : 10062
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 14997
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40006
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50006
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60005
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70372, Dms : 10063
Timer : 0, programmed : 5000, actual : 4997
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 14997
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40006
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50006
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60005
Timer : 12, programmed : 65000, actual : 65007
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80436, Dms : 10064
Timer : 0, programmed : 5000, actual : 4998
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 19999
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29998
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 39998
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50006
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60005
Timer : 12, programmed : 65000, actual : 65007
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 75006
Timer : 15, programmed : 80000, actual : 80009

```

---
---

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


