/****************************************************************************************************************************
  TimerInterrupt.h
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

  Version: 1.7.0

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
****************************************************************************************************************************/

#pragma once

#ifndef TimerInterrupt_h
#define TimerInterrupt_h

#if defined(BOARD_TYPE)
  #undef BOARD_TYPE
#endif

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))
  #if defined(TIMER_INTERRUPT_USING_ATMEGA2560)
    #undef TIMER_INTERRUPT_USING_ATMEGA2560
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA2560      true   
  #define BOARD_TYPE    "Arduino AVR Mega2560/ADK"
  #warning Using Arduino AVR Mega, Mega640(P), Mega2560/ADK. Timer1-5 available
  
#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) )      
  #define BOARD_TYPE    "Arduino AVR UNO, Nano, etc." 
  #warning Using Aduino AVR ATMega644(P), ATMega328(P) such as UNO, Nano. Only Timer1,2 available

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define BOARD_TYPE    "Adafruit AVR ATMega328(P)"
  #warning Using Adafruit ATMega328(P), such as AVR_FEATHER328P or AVR_METRO. Only Timer1,2 available
        
#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_TYPE    "Arduino AVR ATMega32U4"
  #warning Using Arduino ATMega32U4, such as Leonardo or Leonardo ETH. Only Timer1,3,4 available
  
#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_TYPE    "Adafruit AVR ATMega32U4"
  #warning Using Adafruit ATMega32U4, such as Feather_32u4, AVR_CIRCUITPLAY, etc.. Only Timer1,3,4 available

#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega32U4"
  #warning Using Generic ATMega32U4, such as Sparkfun AVR_MAKEYMAKEY, AVR_PROMICRO, etc. Only Timer1,3,4 available

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega328P"
  #warning Using Generic ATMega328P, such as Sparkfun AVR_DIGITAL_SANDBOX, REDBOT, etc.

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega128RFA1"
  #warning Using Generic ATMega128RFA1, such as Sparkfun ATMEGA128RFA1_DEV_BOARD, etc.
  
#elif ( defined(ARDUINO_AVR_GEMMA) || defined(ARDUINO_AVR_TRINKET3) || defined(ARDUINO_AVR_TRINKET5) )
  #error These AVR boards are not supported! Please check your Tools->Board setting.
     
#else
  #error This is designed only for Arduino or Adafruit AVR board! Please check your Tools->Board setting.
#endif

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#include "TimerInterrupt_Generic_Debug.h"

#ifndef TIMER_INTERRUPT_VERSION
  #define TIMER_INTERRUPT_VERSION       "TimerInterrupt v1.7.0"
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

#define MAX_COUNT_8BIT            255
#define MAX_COUNT_10BIT           1023
#define MAX_COUNT_16BIT           65535

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
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

typedef void (*timer_callback)();
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

    bool            _timerDone;
    int8_t          _timer;
    unsigned int    _prescalerIndex;
    uint32_t        _OCRValue;
    uint32_t        _OCRValueRemaining;
    volatile long   _toggle_count;
    double           _frequency;

    void*           _callback;        // pointer to the callback function
    void*           _params;          // function parameter

    void set_OCR();

  public:

    TimerInterrupt()
    {
      _timer              = -1;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _prescalerIndex     = NO_PRESCALER;
      _OCRValue           = 0;
      _OCRValueRemaining  = 0;
      _toggle_count       = -1;
    };

    explicit TimerInterrupt(uint8_t timerNo)
    {
      _timer              = timerNo;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _prescalerIndex     = NO_PRESCALER;
      _OCRValue           = 0;
      _OCRValueRemaining  = 0;
      _toggle_count       = -1;
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
    bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "setInterval() callback argument size must be <= 4 bytes");
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    template<typename TArg>
    bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterrupt() callback argument size must be <= 4 bytes");
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterruptInterval() callback argument size must be <= 4 bytes");
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p> (callback), /*NULL*/ 0, duration);
    }

    void detachInterrupt();

    void disableTimer()
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
    void pauseTimer();

    // Just reconnect clock source, continue from the current count
    void resumeTimer();

    // Just stop clock source, clear the count
    void stopTimer()
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

    long getCount() __attribute__((always_inline))
    {
      return _toggle_count;
    };

    void setCount(long countInput) __attribute__((always_inline))
    {
      //cli();//stop interrupts
      //noInterrupts();

      _toggle_count = countInput;

      //sei();//enable interrupts
      //interrupts();
    };

    long get_OCRValue() __attribute__((always_inline))
    {
      return _OCRValue;
    };

    long get_OCRValueRemaining() __attribute__((always_inline))
    {
      return _OCRValueRemaining;
    };

    void adjust_OCRValue() //__attribute__((always_inline))
    {
      //cli();//stop interrupts
      noInterrupts();

      if (_timer != 2)
      {
#if TIMER_INTERRUPT_USING_ATMEGA_32U4
        if (_timer == 4)
        {
          if (_OCRValueRemaining < MAX_COUNT_8BIT)
          {
            set_OCR();
          }
          
          _OCRValueRemaining -= min(MAX_COUNT_8BIT, _OCRValueRemaining);
        }
        else
        {
          if (_OCRValueRemaining < MAX_COUNT_16BIT)
          {
            set_OCR();
          }
        
          _OCRValueRemaining -= min(MAX_COUNT_16BIT, _OCRValueRemaining);
        }
#else
        if (_OCRValueRemaining < MAX_COUNT_16BIT)
        {
          set_OCR();
        }
          
        _OCRValueRemaining -= min(MAX_COUNT_16BIT, _OCRValueRemaining);
#endif      
      }
      else
      {
        if (_OCRValueRemaining < MAX_COUNT_8BIT)
        {
          set_OCR();
        }
          
        _OCRValueRemaining -= min(MAX_COUNT_8BIT, _OCRValueRemaining);
      }

      if (_OCRValueRemaining <= 0)
      {
        // Reset value for next cycle
        _OCRValueRemaining = _OCRValue;
        _timerDone = true;
      }
      else
        _timerDone = false;

      //sei();//enable interrupts
      interrupts();
    };

    void reload_OCRValue() //__attribute__((always_inline))
    {
      //cli();//stop interrupts
      noInterrupts();

      // Reset value for next cycle, have to deduct the value already loaded to OCR register
      
      _OCRValueRemaining = _OCRValue;
      set_OCR();

      _timerDone = false;

      //sei();//enable interrupts
      interrupts();
    };

    bool checkTimerDone() //__attribute__((always_inline))
    {
      return _timerDone;
    };

}; // class TimerInterrupt

//////////////////////////////////////////////

// To be sure not used Timers are disabled
#if !defined(USE_TIMER_1)
  #define USE_TIMER_1     false
#endif

#if !defined(USE_TIMER_2)
  #define USE_TIMER_2     false
#elif ( USE_TIMER_2 && TIMER_INTERRUPT_USING_ATMEGA_32U4 )
  #error Timer2 is disabled for ATMEGA_32U4, only available for ATMEGA_328(P) and Mega
#endif

#if !defined(USE_TIMER_3)
  #define USE_TIMER_3     false
#elif ( USE_TIMER_3 && ( TIMER_INTERRUPT_USING_ATMEGA_32U4 || TIMER_INTERRUPT_USING_ATMEGA2560 ) )
  #warning Timer3 (16-bit) is OK to use for ATMEGA_32U4 and Mega
#elif USE_TIMER_3
  #error Timer3 is only available for ATMEGA_32U4 and Mega
#endif

#if !defined(USE_TIMER_4)
  #define USE_TIMER_4     false
#elif ( USE_TIMER_4 && ( TIMER_INTERRUPT_USING_ATMEGA_32U4 || TIMER_INTERRUPT_USING_ATMEGA2560 ) )
  #warning Timer4 is OK to use for ATMEGA_32U4 (10-bit but using as 8-bit) and Mega (16-bit)
#elif USE_TIMER_4
  #error Timer4 is only available for ATMEGA_32U4 and Mega
#endif

#if !defined(USE_TIMER_5)
  #define USE_TIMER_5     false
#elif ( USE_TIMER_5 && TIMER_INTERRUPT_USING_ATMEGA2560 )
  #warning Timer5 is OK to use for Mega
#elif USE_TIMER_5
  #error Timer5 is only available for Mega
#endif

//////////////////////////////////////////////

#if USE_TIMER_1 
  #ifndef TIMER1_INSTANTIATED
    // To force pre-instatiate only once
    #define TIMER1_INSTANTIATED
    TimerInterrupt ITimer1(HW_TIMER_1);
    
    // Timer0 is used for micros(), millis(), delay(), etc and can't be used
    // Pre-instatiate

    ISR(TIMER1_COMPA_vect)
    {
      long countLocal = ITimer1.getCount();
           
      if (ITimer1.getTimer() == 1)
      {
        if (countLocal != 0)
        {
          if (ITimer1.checkTimerDone())
          {
            TISR_LOGDEBUG3(("T1 callback, _OCRValueRemaining ="), ITimer1.get_OCRValueRemaining(), (", millis ="), millis());
            
            ITimer1.callback();

            if (ITimer1.get_OCRValue() > MAX_COUNT_16BIT)
            {
              // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT if _OCRValueRemaining > MAX_COUNT_16BIT
              ITimer1.reload_OCRValue();
            }
                 
            if (countLocal > 0)                  
              ITimer1.setCount(countLocal - 1);
          }
          else
          {
            //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
            // If _OCRValue == 0, flag _timerDone for next cycle  
            // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
            ITimer1.adjust_OCRValue();
          }         
        }
        else
        {
          TISR_LOGWARN(("T1 done"));
          
          ITimer1.detachInterrupt();
        }
      }
    }
    
  #endif  //#ifndef TIMER1_INSTANTIATED
#endif    //#if USE_TIMER_1

#if USE_TIMER_2
  #ifndef TIMER2_INSTANTIATED
    #define TIMER2_INSTANTIATED
    TimerInterrupt ITimer2(HW_TIMER_2);
    
    ISR(TIMER2_COMPA_vect)
    {
      long countLocal = ITimer2.getCount();
     
      if (ITimer2.getTimer() == 2)
      {
        if (countLocal != 0)
        {
          if (ITimer2.checkTimerDone())
          {
            TISR_LOGDEBUG3(("T2 callback, _OCRValueRemaining ="), ITimer2.get_OCRValueRemaining(), (", millis ="), millis());
             
            ITimer2.callback();
            
            if (ITimer2.get_OCRValue() > MAX_COUNT_8BIT)
            {
              // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_8BIT if _OCRValueRemaining > MAX_COUNT_8BIT
              ITimer2.reload_OCRValue();
            }
            
            if (countLocal > 0)
             ITimer2.setCount(countLocal - 1);

          }
          else
          {           
            //Deduct _OCRValue by min(MAX_COUNT_8BIT, _OCRValue)
            // If _OCRValue == 0, flag _timerDone for next cycle
            ITimer2.adjust_OCRValue();
          }          
        }    
        else
        {
          TISR_LOGWARN(("T2 done"));
          
          ITimer2.detachInterrupt();
        }
      }
    }    
  #endif  //#ifndef TIMER2_INSTANTIATED
#endif    //#if USE_TIMER_2

#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

  // Pre-instatiate
  #if USE_TIMER_3
    #ifndef TIMER3_INSTANTIATED
      // To force pre-instatiate only once
      #define TIMER3_INSTANTIATED
      TimerInterrupt ITimer3(HW_TIMER_3);
      
      ISR(TIMER3_COMPA_vect)
      {
        long countLocal = ITimer3.getCount();
        
        if (ITimer3.getTimer() == 3)
        {
          if (countLocal != 0)
          {
            if (ITimer3.checkTimerDone())
            { 
              TISR_LOGDEBUG3(("T3 callback, _OCRValueRemaining ="), ITimer3.get_OCRValueRemaining(), (", millis ="), millis());
              
              ITimer3.callback();

              if (ITimer3.get_OCRValue() > MAX_COUNT_16BIT)
              {
                // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT if _OCRValueRemaining > MAX_COUNT_16BIT
                ITimer3.reload_OCRValue();
              }
                           
              if (countLocal > 0)
                ITimer3.setCount(countLocal - 1);     
            }
            else
            {
              //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
              // If _OCRValue == 0, flag _timerDone for next cycle          
              // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
              ITimer3.adjust_OCRValue();
            }
          }
          else
          {
            TISR_LOGWARN(("T3 done"));
            
            ITimer3.detachInterrupt();
          }
        }
      }  
      
    #endif  //#ifndef TIMER3_INSTANTIATED
  #endif    //#if USE_TIMER_3

#endif      //#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

  // Even 32u4 Timer4 has 10-bit counter, we use only 8-bit to simplify by not using 2-bit High Byte Register (TC4H)
  // Check 15.2.2 Accuracy, page 141 of ATmega16U4/32U4 [DATASHEET]

  #if USE_TIMER_4
    #ifndef TIMER4_INSTANTIATED
      // To force pre-instatiate only once
      #define TIMER4_INSTANTIATED
      TimerInterrupt ITimer4(HW_TIMER_4);
      
      ISR(TIMER4_COMPA_vect)
      {
        long countLocal = ITimer4.getCount();
                
        if (ITimer4.getTimer() == 4)
        {
          if (countLocal != 0)
          {
            if (ITimer4.checkTimerDone())
            {  
              TISR_LOGDEBUG3(("T4 callback, _OCRValueRemaining ="), ITimer4.get_OCRValueRemaining(), (", millis ="), millis());
              
              ITimer4.callback();
              
              if (ITimer4.get_OCRValue() > MAX_COUNT_16BIT)
              {
                // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT if _OCRValueRemaining > MAX_COUNT_16BIT
                ITimer4.reload_OCRValue();
              }
                           
              if (countLocal > 0)
                ITimer4.setCount(countLocal - 1);       
            }
            else
            {
              //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue) or min(MAX_COUNT_8BIT, _OCRValue)
              // If _OCRValue == 0, flag _timerDone for next cycle     
              // If last one (_OCRValueRemaining < MAX_COUNT_16BIT / MAX_COUNT_8BIT) => load _OCR register _OCRValueRemaining
              ITimer4.adjust_OCRValue();
            }
          }
          else
          {
            TISR_LOGWARN(("T4 done"));
            
            ITimer4.detachInterrupt();
          }
        }
      }
      
      
    #endif  //#ifndef TIMER4_INSTANTIATED
  #endif    //#if USE_TIMER_4

#endif      //#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

#if TIMER_INTERRUPT_USING_ATMEGA2560
  
  #if USE_TIMER_5
    #ifndef TIMER5_INSTANTIATED
      // To force pre-instatiate only once
      #define TIMER5_INSTANTIATED
      TimerInterrupt ITimer5(HW_TIMER_5);
      
      ISR(TIMER5_COMPA_vect)
      {
        long countLocal = ITimer5.getCount();
                
        if (ITimer5.getTimer() == 5)
        {
          if (countLocal != 0)
          {
            if (ITimer5.checkTimerDone())
            {
              TISR_LOGDEBUG3(("T5 callback, _OCRValueRemaining ="), ITimer5.get_OCRValueRemaining(), (", millis ="), millis());
              
              ITimer5.callback();

              if (ITimer5.get_OCRValue() > MAX_COUNT_16BIT)
              {
                // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT if _OCRValueRemaining > MAX_COUNT_16BIT
                ITimer5.reload_OCRValue();
              }
                            
              if (countLocal > 0)
                ITimer5.setCount(countLocal - 1);
            }
            else
            {
              //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
              // If _OCRValue == 0, flag _timerDone for next cycle           
              // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
              ITimer5.adjust_OCRValue();
            }
          }
          else
          {
            TISR_LOGWARN(("T5 done"));
            
            ITimer5.detachInterrupt();
          }
        }
      }
          
    #endif  //#ifndef TIMER5_INSTANTIATED
  #endif    //#if USE_TIMER_5
  
#endif      //#if TIMER_INTERRUPT_USING_ATMEGA2560

#endif      //#ifndef TimerInterrupt_h
