/****************************************************************************************************************************
  ISR_Timer.cpp
  For Arduino boards (UNO, Nano, Mega, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
  Licensed under MIT license

  TCNTx - Timer/Counter Register. The actual timer value is stored here.
  OCRx - Output Compare Register
  ICRx - Input Capture Register (only for 16bit timer)
  TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.8.0

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
  1.4.1   K.Hoang      02/04/2021 Add support to Sparkfun 32U4, 328(P), 128RFA1 such as AVR_PROMICRO, REDBOT, etc.
  1.5.0   K.Hoang      08/05/2021 Add Timer 3 and 4 to 32U4. Add Timer auto-selection to examples.
  1.6.0   K.Hoang      15/11/2021 Fix bug resulting half frequency when using high frequencies.
  1.7.0   K.Hoang      19/11/2021 Fix bug resulting wrong frequency for some frequencies.
  1.8.0   K.Hoang      18/01/2022 Fix `multiple-definitions` linker error
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_IMPL_H
#define ISR_TIMER_IMPL_H

#include <string.h>

// Select time function:
//static inline unsigned long elapsed() { return micros(); }
static inline unsigned long elapsed() {
  return millis();
}


ISR_Timer::ISR_Timer()
  : numTimers (-1)
{
}

void ISR_Timer::init() 
{
  unsigned long current_millis = millis();   //elapsed();

  for (uint8_t i = 0; i < MAX_TIMERS; i++) 
  {
    memset((void*) &timer[i], 0, sizeof (timer_t));
    timer[i].prev_millis = current_millis;
  }

  numTimers = 0;
}


void ISR_Timer::run() 
{
  uint8_t i;
  unsigned long current_millis;

  // get current time
  current_millis = millis();   //elapsed();

  for (i = 0; i < MAX_TIMERS; i++) 
  {
    timer[i].toBeCalled = DEFCALL_DONTRUN;

    // no callback == no timer, i.e. jump over empty slots
    if (timer[i].callback != NULL) 
    {
      // is it time to process this timer ?
      // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

      if ((current_millis - timer[i].prev_millis) >= timer[i].delay) 
      {
        unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
        // update time
        timer[i].prev_millis += timer[i].delay * skipTimes;

        // check if the timer callback has to be executed
        if (timer[i].enabled) 
        {
          // "run forever" timers must always be executed
          if (timer[i].maxNumRuns == RUN_FOREVER) 
          {
            timer[i].toBeCalled = DEFCALL_RUNONLY;
          }
          // other timers get executed the specified number of times
          else if (timer[i].numRuns < timer[i].maxNumRuns) 
          {
            timer[i].toBeCalled = DEFCALL_RUNONLY;
            timer[i].numRuns++;

            // after the last run, delete the timer
            if (timer[i].numRuns >= timer[i].maxNumRuns) 
            {
              timer[i].toBeCalled = DEFCALL_RUNANDDEL;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].toBeCalled == DEFCALL_DONTRUN)
      continue;

    if (timer[i].hasParam)
      (*(timer_callback_p)timer[i].callback)(timer[i].param);
    else
      (*(timer_callback)timer[i].callback)();

    if (timer[i].toBeCalled == DEFCALL_RUNANDDEL)
      deleteTimer(i);
  }
}


// find the first available slot
// return -1 if none found
int ISR_Timer::findFirstFreeSlot() 
{
  // all slots are used
  if (numTimers >= MAX_TIMERS) 
  {
    return -1;
  }

  // return the first slot with no callback (i.e. free)
  for (uint8_t i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback == NULL) 
    {
      return i;
    }
  }

  // no free slots found
  return -1;
}


int ISR_Timer::setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n) {
  int freeTimer;

  if (numTimers < 0) 
  {
    init();
  }

  freeTimer = findFirstFreeSlot();
  
  if (freeTimer < 0) 
  {
    return -1;
  }

  if (f == NULL) 
  {
    return -1;
  }

  timer[freeTimer].delay        = d;
  timer[freeTimer].callback     = f;
  timer[freeTimer].param        = p;
  timer[freeTimer].hasParam     = h;
  timer[freeTimer].maxNumRuns   = n;
  timer[freeTimer].enabled      = true;
  timer[freeTimer].prev_millis  = elapsed();

  numTimers++;

  return freeTimer;
}


int ISR_Timer::setTimer(unsigned long d, timer_callback f, unsigned n) 
{
  return setupTimer(d, (void *)f, NULL, false, n);
}

int ISR_Timer::setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n) 
{
  return setupTimer(d, (void *)f, p, true, n);
}

int ISR_Timer::setInterval(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RUN_FOREVER);
}

int ISR_Timer::setInterval(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RUN_FOREVER);
}

int ISR_Timer::setTimeout(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RUN_ONCE);
}

int ISR_Timer::setTimeout(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RUN_ONCE);
}

bool ISR_Timer::changeInterval(unsigned numTimer, unsigned long d) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return false;
  }

  // Updates interval of existing specified timer
  if (timer[numTimer].callback != NULL) 
  {
    timer[numTimer].delay = d;
    timer[numTimer].prev_millis = elapsed();
    return true;
  }
  
  // false return for non-used numTimer, no callback
  return false;
}

void ISR_Timer::deleteTimer(unsigned timerId) 
{
  if (timerId >= MAX_TIMERS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numTimers == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the
  // specified slot is already empty
  if (timer[timerId].callback != NULL) 
  {
    memset((void*) &timer[timerId], 0, sizeof (timer_t));
    timer[timerId].prev_millis = elapsed();

    // update number of timers
    numTimers--;
  }
}


// function contributed by code@rowansimms.com
void ISR_Timer::restartTimer(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].prev_millis = elapsed();
}


bool ISR_Timer::isEnabled(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return false;
  }

  return timer[numTimer].enabled;
}


void ISR_Timer::enable(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = true;
}


void ISR_Timer::disable(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = false;
}

void ISR_Timer::enableAll() 
{
  // Enable all timers with a callback assigned (used)
  for (uint8_t i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RUN_FOREVER) 
    {
      timer[i].enabled = true;
    }
  }
}

void ISR_Timer::disableAll() 
{
  // Disable all timers with a callback assigned (used)
  for (uint8_t i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RUN_FOREVER) 
    {
      timer[i].enabled = false;
    }
  }
}

void ISR_Timer::toggle(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = !timer[numTimer].enabled;
}


unsigned ISR_Timer::getNumTimers() 
{
  return numTimers;
}

#endif  // ISR_TIMER_IMPL_H
