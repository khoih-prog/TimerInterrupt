/****************************************************************************************************************************
 * ISR_Timer.h
 * For Arduino boards (UNO, Nano, Mega, etc. )
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
 * Licensed under MIT license
 * Version: v1.0.2
 *
 * Timer0: Timer0 is a 8bit timer. In the Arduino world timer0 is been used for the timer functions, like delay(), millis() 
 * and micros(). 
 * If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.
 * Timer1: Timer1 is a 16bit timer. In the Arduino world the Servo library uses timer1 on Arduino Uno (Timer5 on Arduino Mega).
 * Timer2: Timer2 is a 8bit timer like Timer0. In the Arduino work the tone() function uses Timer2.
 * Timer3, Timer4, Timer5: Timer 3,4,5 are only available on Arduino Mega boards. These timers are all 16bit timers.
 * 
 * Now we can use these new 16 ISR-based timers, while consuming only 1 hardware Timer.
 * Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
 * The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
 * Therefore, their executions are not blocked by bad-behaving functions / tasks.
 * This important feature is absolutely necessary for mission-critical tasks.
 *
 * Based on SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 * 
 * Based on BlynkTimer.h
 * Author: Volodymyr Shymanskyy
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      23/11/2019 Initial coding
 *  1.0.1   K Hoang      25/11/2019 New release fixing compiler error
 *  1.0.2   K.Hoang      28/11/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
*****************************************************************************************************************************/


#ifndef ISR_TIMER_H
#define ISR_TIMER_H

#include <stddef.h>
#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

typedef void (*timer_callback)(void);
typedef void (*timer_callback_p)(void *);

class ISR_Timer {

public:
    // maximum number of timers
    const static int MAX_TIMERS = 16;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    // constructor
    ISR_Timer();

    void  init();

    // this function must be called inside loop()
    void  run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(unsigned long d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(unsigned long d, timer_callback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(unsigned long d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(unsigned long d, timer_callback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(unsigned long d, timer_callback f, unsigned n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n);

    // updates interval of the specified timer
    bool changeInterval(unsigned numTimer, unsigned long d);

    // destroy the specified timer
    void deleteTimer(unsigned numTimer);

    // restart the specified timer
    void restartTimer(unsigned numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(unsigned numTimer);

    // enables the specified timer
    void enable(unsigned numTimer);

    // disables the specified timer
    void disable(unsigned numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void toggle(unsigned numTimer);

    // returns the number of used timers
    unsigned  getNumTimers();

    // returns the number of available timers
    unsigned  getNumAvailableTimers() { return MAX_TIMERS - numTimers; };

private:
    // deferred call constants
    const static int DEFCALL_DONTRUN = 0;       // don't call the callback function
    const static int DEFCALL_RUNONLY = 1;       // call the callback function but don't delete the timer
    const static int DEFCALL_RUNANDDEL = 2;     // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int  setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n);

    // find the first available slot
    int  findFirstFreeSlot();

    typedef struct {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void* callback;                   // pointer to the callback function
      void* param;                      // function parameter
      bool hasParam;                 // true if callback takes a parameter
      unsigned long delay;              // delay value
      unsigned maxNumRuns;              // number of runs to be executed
      unsigned numRuns;                 // number of executed runs
      bool enabled;                  // true if enabled
      unsigned toBeCalled;              // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[MAX_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;
};

#endif
