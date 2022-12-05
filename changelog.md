## TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt.svg?)](https://www.ardu-badge.com/TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt.svg)](https://github.com/khoih-prog/TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt.svg)](http://github.com/khoih-prog/TimerInterrupt/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.8.0](#releases-v180)
  * [Releases v1.7.0](#releases-v170)
  * [Releases v1.6.0](#releases-v160)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.1](#releases-v141)
  * [Releases v1.4.0](#releases-v140)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.2](#releases-v112)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.0.3](#releases-v103)
  * [Releases v1.0.2](#releases-v102)

---
---

## Changelog

### Releases v1.8.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Add example `Argument_Complex_Multi` to demonstrate how to avoid `multiple-definitions` linker error in multiple-file projects

### Releases v1.7.0

1. Fix bug resulting wrong frequency for some low frequencies.

### Releases v1.6.0

1. Fix bug resulting half frequency when using high frequencies. Check [Frequencies are half of what I expect #22](https://github.com/khoih-prog/TimerInterrupt/discussions/22)
2. Add example to demo High Frequency
3. Delete Blynk-related examples
4. Add changelog.md

### Releases v1.5.0

1. Add **Timer3 and Timer4 support to ATmega32U4 and ATmega16U4.** 
2. Add Timer auto-selection to examples.

### Releases v1.4.1

1. Add support to **Generic or Sparkfun AVR ATmega_32U4** such as **AVR_MAKEYMAKEY, AVR_PROMICRO, etc.**
2. Add support to **Generic or Sparkfun AVR ATmega_328(P)** such as **ARDUINO_REDBOT, ARDUINO_AVR_DIGITAL_SANDBOX, etc.**
3. Add support to **Generic or Sparkfun AVR ATmega128RFA1** such as **ATMEGA128RFA1_DEV_BOARD, etc.**

### Releases v1.4.0

1. Add support to **Adafruit AVR ATMEGA_32U4** such as **AVR_FLORA8, AVR_FEATHER32U4, etc.**
2. Add support to **Adafruit AVR ATMEGA_328(P)** such as **AVR_FEATHER328P, AVR_METRO, etc.**
3. Update examples

### Releases v1.3.0

1. Add support to **AVR ATMEGA_16U4, ATMEGA_32U4** such as **Leonardo, YUN, ESPLORA, etc.**
2. Update examples

### Releases v1.2.0

1. Add better debug feature.
2. Optimize code and examples to reduce RAM usage


### Releases v1.1.2

1. Clean-up all compiler warnings possible.
2. Optimize examples to reduce memory usage by using Flash String whenever possible.
3. Add Table of Contents

### Releases v1.1.1

1. Add example [**Change_Interval**](examples/Change_Interval)
2. Bump up version to sync with other TimerInterrupt Libraries. Modify Version String.
3. Add new h-only code besides conventional h/cpp code

### Releases v1.0.3

1. Add example [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) and optimize example [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple) to demonstrate the usage of **16 ISR-based timers**

### Releases v1.0.2

1. Initial releases.


