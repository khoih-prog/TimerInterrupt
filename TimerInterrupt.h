/************************************************
 * TimerInterrupt.h
 * For Arduino AVR boards
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
 * Licensed under MIT license
 * Version: 1.0.0
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      13/11/2019 Initial coding
*************************************************/

#ifndef TimerInterrupt_h
#define TimerInterrupt_h

#ifdef ESP8266
#error This code is designed to run on Arduino AVR (Nano, UNO, Mega, etc.) platform, not ESP8266! Please check your Tools->Board setting.
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

class TimerInterrupt;

// Pre-instatiate
extern TimerInterrupt ITimer1;
extern TimerInterrupt ITimer2;

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
// Pre-instatiate
extern TimerInterrupt ITimer3;
extern TimerInterrupt ITimer4;
extern TimerInterrupt ITimer5;

#define TCCR2A TCCR2
#define TCCR2B TCCR2
#define COM2A1 COM21
#define COM2A0 COM20
#define OCR2A OCR2
#define TIMSK2 TIMSK
#define OCIE2A OCIE2
#define TIMER2_COMPA_vect TIMER2_COMP_vect
#define TIMSK1 TIMSK
#endif

typedef void (*timer_callback)(void);
typedef void (*timer_callback_p)(void *);

enum 
{
  HW_TIMER_0 = 0,
  HW_TIMER_1,
  HW_TIMER_2,
  HW_TIMER_3,
  HW_TIMER_4,
  HW_TIMER_5,
  NUM_HW_TIMERS
};

enum 
{
  NO_CLOCK_SOURCE = 0,
  NO_PRESCALER,
  PRESCALER_8,
  PRESCALER_64,
  PRESCALER_256,
  PRESCALER_1024,
  NUM_ITEMS
};

enum 
{
  T2_NO_CLOCK_SOURCE = 0,
  T2_NO_PRESCALER,
  T2_PRESCALER_8,
  T2_PRESCALER_32,
  T2_PRESCALER_64,
  T2_PRESCALER_128,
  T2_PRESCALER_256,
  T2_PRESCALER_1024,    
  T2_NUM_ITEMS
};

const unsigned int prescalerDiv   [NUM_ITEMS]     = { 1, 1, 8, 64, 256, 1024 };
const unsigned int prescalerDivT2 [T2_NUM_ITEMS]  = { 1, 1, 8, 32,  64,  128, 256, 1024 };

class TimerInterrupt
{
  private:
  
    int8_t          _timer;
    unsigned int    _prescalerIndex;
    uint32_t        _OCRValue;
    volatile long   _toggle_count;
    float           _frequency;

    void*           _callback;        // pointer to the callback function
    void*           _params;          // function parameter   
           
  public:

  TimerInterrupt() 
  { 
    _timer = -1; 
    _frequency = 0; 
    _callback = NULL; 
    _params = NULL; 
  };
  
  TimerInterrupt(uint8_t timerNo) 
  { 
    _timer = timerNo; 
    _frequency = 0; 
    _callback = NULL; 
    _params = NULL; 
  };
  
  void callback() __attribute__((always_inline))
  {
    if (_callback != NULL)
    {
      if (_params != NULL)
          (*(timer_callback_p)_callback)(_params);
      else
          (*(timer_callback)_callback)();
    }
  }  

  void init(int8_t timer);
 
  void init()
  {
    init(_timer);
  };

  // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely 
  bool setFrequency(float frequency, timer_callback_p callback, void* params, unsigned long duration = 0);

  // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
  bool setFrequency(float frequency, timer_callback callback, unsigned long duration)
  {
    return setFrequency(frequency, (timer_callback_p) callback, NULL, duration);
  }

  // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
  bool setInterval(unsigned long interval, timer_callback_p callback, void* params, unsigned long duration)
  {  
    return setFrequency((float) (1000.0f / interval), callback, params, duration);  
  }

  // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
  bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration)
  { 
    return setFrequency((float) (1000.0f / interval), (timer_callback_p) callback, NULL, duration);  
  }
  
  bool attachInterrupt(float frequency, timer_callback_p callback, void* params, unsigned long duration)
  {
    return setFrequency(frequency, callback, params, duration);
  }

  bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration)
  {
    return setFrequency(frequency, (timer_callback_p) callback, NULL, duration);
  }
    
  // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely 
  bool attachInterruptInterval(unsigned long interval, timer_callback_p callback, void* params, unsigned long duration = 0)
  {
     return setFrequency( (float) (1000.0f / interval), callback, params, duration);
  }

  // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely 
  bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
  {
     return setFrequency( (float) (1000.0f / interval), (timer_callback_p) callback, NULL, duration);
  }

  void detachInterrupt();

  void disableTimer(void)
  {
    detachInterrupt();
  }
  
  // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
  void reattachInterrupt(unsigned long duration = 0);

  // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
  void enableTimer(unsigned long duration = 0) __attribute__((always_inline))
  {
    reattachInterrupt(duration); 
  }

  // Just stop clock source, still keep the count
  void pauseTimer(void);
 
  // Just reconnect clock source, continue from the current count
  void resumeTimer(void);

 // Just stop clock source, clear the count
  void stopTimer(void)
  {
    detachInterrupt();
  }

  // Just reconnect clock source, start current count from 0
  void restartTimer(unsigned long duration = 0)
  {
    reattachInterrupt(duration);
  }

  int8_t getTimer() __attribute__((always_inline))
  { 
    return _timer; 
  };
  
  volatile long getCount() __attribute__((always_inline))
  { 
    return _toggle_count;
  };
  
  void setCount(long countInput) __attribute__((always_inline))
  { 
    _toggle_count = countInput; 
  };

}; // class TimerInterrupt

#endif
