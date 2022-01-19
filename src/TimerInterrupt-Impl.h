/****************************************************************************************************************************
  TimerInterrupt.cpp
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
****************************************************************************************************************************/

#pragma once

#ifndef TimerInterrupt_Impl_h
#define TimerInterrupt_Impl_h

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

void TimerInterrupt::init(int8_t timer)
{    
  // Set timer specific stuff
  // All timers in CTC mode
  // 8 bit timers will require changing prescalar values,
  // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
  
  //cli();//stop interrupts
  noInterrupts();
  
  switch (timer)
  {    
    #if defined(TCCR1A) && defined(TCCR1B) && defined(WGM12)
    case 1:
      // 16 bit timer
      TCCR1A = 0;
      TCCR1B = 0;
      // Page 172-173. ATmega 328/328P or Page 145-146 of ATmega 640/1280/2560
      // Mode 4 => Clear Timer on Compare match (CTC) using OCR1A for counter value
      bitWrite(TCCR1B, WGM12, 1);
      // No scaling now
      bitWrite(TCCR1B, CS10, 1);

      TISR_LOGWARN(F("T1"));
      
      break;
    #endif

    #if defined(TCCR2A) && defined(TCCR2B)
    case 2:
      // 8 bit timer
      TCCR2A = 0;
      TCCR2B = 0;
      // Page 205-206. ATmegal328, Page 184-185 ATmega 640/1280/2560
      // Mode 2 => Clear Timer on Compare match (CTC) using OCR2A for counter value
      bitWrite(TCCR2A, WGM21, 1);
      // No scaling now
      bitWrite(TCCR2B, CS20, 1);

      TISR_LOGWARN(F("T2"));
      
      break;
    #endif

    #if defined(TCCR3A) && defined(TCCR3B) &&  defined(TIMSK3)
    case 3:
      // 16 bit timer
      TCCR3A = 0;
      TCCR3B = 0;
      bitWrite(TCCR3B, WGM32, 1);
      bitWrite(TCCR3B, CS30, 1);

      TISR_LOGWARN(F("T3"));
      
      break;
    #endif  

    #if defined(TCCR4A) && defined(TCCR4B) &&  defined(TIMSK4)
    case 4:
      // 16 bit timer
      TCCR4A = 0;
      TCCR4B = 0;
      #if defined(WGM42)
        bitWrite(TCCR4B, WGM42, 1);
      #elif defined(CS43)
        // TODO this may not be correct
        // atmega32u4
        bitWrite(TCCR4B, CS43, 1);
      #endif
      bitWrite(TCCR4B, CS40, 1);

      TISR_LOGWARN(F("T4"));
      
      break;
    #endif

    #if defined(TCCR5A) && defined(TCCR5B) &&  defined(TIMSK5)
    case 5:
      // 16 bit timer
      TCCR5A = 0;
      TCCR5B = 0;
      bitWrite(TCCR5B, WGM52, 1);
      bitWrite(TCCR5B, CS50, 1);

      TISR_LOGWARN(F("T5"));
      
      break;
    #endif
  }

  _timer = timer;

  //sei();//enable interrupts
  interrupts();
  
}

void TimerInterrupt::set_OCR()
{
  // Run with noInterrupt()
  // Set the OCR for the given timer,
  // set the toggle count,
  // then turn on the interrupts
  uint32_t _OCRValueToUse;

  switch (_timer)
  {
    case 1:
      _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
      OCR1A = _OCRValueToUse;
      _OCRValueRemaining -= _OCRValueToUse;

#if defined(OCR1A) && defined(TIMSK1) && defined(OCIE1A)
      // Bit 1 – OCIEA: Output Compare A Match Interrupt Enable
      // When this bit is written to '1', and the I-flag in the Status Register is set (interrupts globally enabled), the
      // Timer/Counter Output Compare A Match interrupt is enabled. The corresponding Interrupt Vector is
      // executed when the OCFA Flag, located in TIFR1, is set.
      bitWrite(TIMSK1, OCIE1A, 1);
#elif defined(OCR1A) && defined(TIMSK) && defined(OCIE1A)
      // this combination is for at least the ATmega32
      bitWrite(TIMSK, OCIE1A, 1);
#endif
      break;

#if defined(OCR2A) && defined(TIMSK2) && defined(OCIE2A)
    case 2:
      _OCRValueToUse = min(MAX_COUNT_8BIT, _OCRValueRemaining);
      OCR2A = _OCRValueToUse;
      _OCRValueRemaining -= _OCRValueToUse;

      bitWrite(TIMSK2, OCIE2A, 1);
      break;
#endif

#if defined(OCR3A) && defined(TIMSK3) && defined(OCIE3A)
    case 3:
      _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
      OCR3A = _OCRValueToUse;
      _OCRValueRemaining -= _OCRValueToUse;

      bitWrite(TIMSK3, OCIE3A, 1);
      break;
#endif

#if defined(OCR4A) && defined(TIMSK4) && defined(OCIE4A)
    case 4:
    
#if TIMER_INTERRUPT_USING_ATMEGA_32U4
      _OCRValueToUse = min(MAX_COUNT_8BIT, _OCRValueRemaining);
#else    
      _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
#endif
      
      OCR4A = _OCRValueToUse;
      _OCRValueRemaining -= _OCRValueToUse;

      bitWrite(TIMSK4, OCIE4A, 1);
      break;
#endif

#if defined(OCR5A) && defined(TIMSK5) && defined(OCIE5A)
    case 5:
      _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
      OCR5A = _OCRValueToUse;
      _OCRValueRemaining -= _OCRValueToUse;

      bitWrite(TIMSK5, OCIE5A, 1);
      break;
#endif
  }

  // Flag _OCRValue == 0 => end of long timer
  if (_OCRValueRemaining == 0)
    _timerDone = true;

}

// frequency (in hertz) and duration (in milliseconds).
// Return true if frequency is OK with selected timer (OCRValue is in range)
bool TimerInterrupt::setFrequency(float frequency, timer_callback_p callback, uint32_t params, unsigned long duration)
{
  uint8_t       andMask = 0b11111000;
  unsigned long OCRValue;
  bool isSuccess = false;

  //frequencyLimit must > 1
  float frequencyLimit = frequency * 17179.840;

  // Limit frequency to larger than (0.00372529 / 64) Hz or interval 17179.840s / 17179840 ms to avoid uint32_t overflow
  if ((_timer <= 0) || (callback == NULL) || ((frequencyLimit) < 1) )
  {
    return false;
  }
  else      
  {       
    // Calculate the toggle count. Duration must be at least longer then one cycle
    if (duration > 0)
    {   
      _toggle_count = frequency * duration / 1000;

      TISR_LOGWARN1(F("setFrequency => _toggle_count ="), _toggle_count);
      TISR_LOGWARN3(F("Frequency ="), frequency, F(", duration ="), duration);
           
      if (_toggle_count < 1)
      {
        return false;
      }
    }
    else
    {
      _toggle_count = -1;
    }
      
    //Timer0 and timer2 are 8 bit timers, meaning they can store a maximum counter value of 255.
    //Timer2 does not have the option of 1024 prescaler, only 1, 8, 32, 64  
    //Timer1 is a 16 bit timer, meaning it can store a maximum counter value of 65535.
    int prescalerIndexStart;
    
    //Use smallest prescaler first, then increase until fits (<255)
    if (_timer != 2)
    {     
      if (frequencyLimit > 64)
        prescalerIndexStart = NO_PRESCALER;        
      else if (frequencyLimit > 8)
        prescalerIndexStart = PRESCALER_8;
      else
        prescalerIndexStart = PRESCALER_64;

        
      for (int prescalerIndex = prescalerIndexStart; prescalerIndex <= PRESCALER_1024; prescalerIndex++)
      {
        OCRValue = F_CPU / (frequency * prescalerDiv[prescalerIndex]) - 1;
  
        TISR_LOGWARN1(F("Freq * 1000 ="), frequency * 1000);
        TISR_LOGWARN3(F("F_CPU ="), F_CPU, F(", preScalerDiv ="), prescalerDiv[prescalerIndex]);
        TISR_LOGWARN3(F("OCR ="), OCRValue, F(", preScalerIndex ="), prescalerIndex);

        // We use very large _OCRValue now, and every time timer ISR activates, we deduct min(MAX_COUNT_16BIT, _OCRValueRemaining) from _OCRValueRemaining
        // So that we can create very long timer, even if the counter is only 16-bit.
        // Use very high frequency (OCRValue / MAX_COUNT_16BIT) around 16 * 1024 to achieve higher accuracy
#if TIMER_INTERRUPT_USING_ATMEGA_32U4
        uint16_t MAX_COUNT_32U4 = (_timer == 4) ? MAX_COUNT_8BIT : MAX_COUNT_16BIT;
       
        if ( (OCRValue / MAX_COUNT_32U4) < 16384 )
#else        
        if ( (OCRValue / MAX_COUNT_16BIT) < 16384 )
#endif        
        {
          _OCRValue           = OCRValue;
          _OCRValueRemaining  = OCRValue;
          _prescalerIndex = prescalerIndex;
  
          TISR_LOGWARN1(F("OK in loop => _OCR ="), _OCRValue);
          TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDiv[_prescalerIndex]);
             
          isSuccess = true;
         
          break;
        }       
      }

      if (!isSuccess)
      {
        // Always do this
        _OCRValue           = OCRValue;
        _OCRValueRemaining  = OCRValue;
        _prescalerIndex = PRESCALER_1024;
  
        TISR_LOGWARN1(F("OK out loop => _OCR ="), _OCRValue);
        TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDiv[_prescalerIndex]);
      }            
    }
    else
    {
      if (frequencyLimit > 64)
        prescalerIndexStart = T2_NO_PRESCALER;        
      else if (frequencyLimit > 8)
        prescalerIndexStart = T2_PRESCALER_8;
      else if (frequencyLimit > 2)
        prescalerIndexStart = T2_PRESCALER_32;        
      else
        prescalerIndexStart = T2_PRESCALER_64;
          
      // Page 206-207. ATmegal328
      //8-bit Timer2 has more options up to 1024 prescaler, from 1, 8, 32, 64, 128, 256 and 1024  
      for (int prescalerIndex = prescalerIndexStart; prescalerIndex <= T2_PRESCALER_1024; prescalerIndex++)
      {
        OCRValue = F_CPU / (frequency * prescalerDivT2[prescalerIndex]) - 1;
  
        TISR_LOGWARN3(F("F_CPU ="), F_CPU, F(", preScalerDiv ="), prescalerDivT2[prescalerIndex]);
        TISR_LOGWARN3(F("OCR2 ="), OCRValue, F(", preScalerIndex ="), prescalerIndex);

        // We use very large _OCRValue now, and every time timer ISR activates, we deduct min(MAX_COUNT_8BIT, _OCRValue) from _OCRValue
        // to create very long timer, even if the counter is only 16-bit.
        // Use very high frequency (OCRValue / MAX_COUNT_8BIT) around 16 * 1024 to achieve higher accuracy
        if ( (OCRValue / MAX_COUNT_8BIT) < 16384 )
        {
          _OCRValue           = OCRValue;
          _OCRValueRemaining  = OCRValue;
          // same as prescalarbits
          _prescalerIndex = prescalerIndex;
  
          TISR_LOGWARN1(F("OK in loop => _OCR ="), _OCRValue);
          TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDivT2[_prescalerIndex]);
          
          isSuccess = true;
          
          break;
        }       
      }

      if (!isSuccess)
      {
        // Always do this
        _OCRValue           = OCRValue;
        _OCRValueRemaining  = OCRValue;
        // same as prescalarbits
        _prescalerIndex = T2_PRESCALER_1024;
  
        TISR_LOGWARN1(F("OK out loop => _OCR ="), _OCRValue);
        TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDivT2[_prescalerIndex]);
      } 
    }

    //cli();//stop interrupts
    noInterrupts();

    _frequency = frequency;
    _callback  = (void*) callback;
    _params    = reinterpret_cast<void*>(params);

    _timerDone = false;
          
    // 8 bit timers from here     
    #if defined(TCCR2B)
    if (_timer == 2)
    {
      TCCR2B = (TCCR2B & andMask) | _prescalerIndex;   //prescalarbits;
      
      TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
    }
    #endif

    // 16 bit timers from here
    #if defined(TCCR1B)
    #if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )
    if (_timer == 1)
    #else
    else if (_timer == 1)
    #endif
    {
      TCCR1B = (TCCR1B & andMask) | _prescalerIndex;   //prescalarbits;
      
      TISR_LOGWARN1(F("TCCR1B ="), TCCR1B);
    }
    #endif
    
    #if defined(TCCR3B)
    else if (_timer == 3)
      TCCR3B = (TCCR3B & andMask) | _prescalerIndex;   //prescalarbits;
    #endif
    
    #if defined(TCCR4B)
    else if (_timer == 4)
      TCCR4B = (TCCR4B & andMask) | _prescalerIndex;   //prescalarbits;
    #endif
    
    #if defined(TCCR5B)
    else if (_timer == 5)
      TCCR5B = (TCCR5B & andMask) | _prescalerIndex;   //prescalarbits;
    #endif
       
    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts     
    set_OCR();
    
    //sei();//allow interrupts
    interrupts();

    return true;
  }
}

void TimerInterrupt::detachInterrupt(void)
{
  //cli();//stop interrupts
  noInterrupts();
  
  switch (_timer)
  {
    #if defined(TIMSK1) && defined(OCIE1A)
    case 1:
      bitWrite(TIMSK1, OCIE1A, 0);
 
      TISR_LOGWARN(F("Disable T1"));
       
      break;
    #endif

    case 2:
      #if defined(TIMSK2) && defined(OCIE2A)
        bitWrite(TIMSK2, OCIE2A, 0); // disable interrupt
      #endif
      
      TISR_LOGWARN(F("Disable T2"));
            
      break;

#if defined(TIMSK3) && defined(OCIE3A)
    case 3:
      bitWrite(TIMSK3, OCIE3A, 0);
      
      TISR_LOGWARN(F("Disable T3"));
            
      break;
#endif

#if defined(TIMSK4) && defined(OCIE4A)
    case 4:
      bitWrite(TIMSK4, OCIE4A, 0);

      TISR_LOGWARN(F("Disable T4"));
            
      break;
#endif

#if defined(TIMSK5) && defined(OCIE5A)
    case 5:
      bitWrite(TIMSK5, OCIE5A, 0);

      TISR_LOGWARN(F("Disable T5"));
            
      break;
#endif
  }
  
  //sei();//allow interrupts
  interrupts();
}

// Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
void TimerInterrupt::reattachInterrupt(unsigned long duration)
{
  //cli();//stop interrupts
  noInterrupts();

  // Calculate the toggle count
  if (duration > 0)
  {
    _toggle_count = _frequency * duration / 1000;
  }
  else
  {
    _toggle_count = -1;
  }
    
  switch (_timer)
  {
#if defined(TIMSK1) && defined(OCIE1A)
    case 1:
      bitWrite(TIMSK1, OCIE1A, 1);

      TISR_LOGWARN(F("Enable T1"));
       
      break;
#endif

    case 2:
      #if defined(TIMSK2) && defined(OCIE2A)
        bitWrite(TIMSK2, OCIE2A, 1); // enable interrupt
      #endif

      TISR_LOGWARN(F("Enable T2"));
            
      break;

#if defined(TIMSK3) && defined(OCIE3A)
    case 3:
      bitWrite(TIMSK3, OCIE3A, 1);
      
      TISR_LOGWARN(F("Enable T3"));
            
      break;
#endif

#if defined(TIMSK4) && defined(OCIE4A)
    case 4:
      bitWrite(TIMSK4, OCIE4A, 1);

      TISR_LOGWARN(F("Enable T4"));
            
      break;
#endif

#if defined(TIMSK5) && defined(OCIE5A)
    case 5:
      bitWrite(TIMSK5, OCIE5A, 1);

      TISR_LOGWARN(F("Enable T5"));
            
      break;
#endif
  }
  
  //sei();//allow interrupts
  interrupts();
}

// Just stop clock source, still keep the count
void TimerInterrupt::pauseTimer(void)
{
  uint8_t andMask = 0b11111000;
  
  //Just clear the CSx2-CSx0. Still keep the count in TCNT and Timer Interrupt mask TIMKSx. 
  
  // 8 bit timers from here     
  #if defined(TCCR2B)
  if (_timer == 2)
  {
    TCCR2B = (TCCR2B & andMask);

    TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
  }
  #endif
  
  // 16 bit timers from here
  #if defined(TCCR1B)
  #if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )
  if (_timer == 1)
  #else
  else if (_timer == 1)
  #endif
  {
    TCCR1B = (TCCR1B & andMask);
    
    TISR_LOGWARN1(F("TCCR1B ="), TCCR1B);
  }
  #endif
  
  #if defined(TCCR3B)
  else if (_timer == 3)
    TCCR3B = (TCCR3B & andMask);
  #endif
  
  #if defined(TCCR4B)
  else if (_timer == 4)
    TCCR4B = (TCCR4B & andMask);
  #endif
  
  #if defined(TCCR5B)
  else if (_timer == 5)
    TCCR5B = (TCCR5B & andMask);
  #endif   
}

// Just reconnect clock source, continue from the current count
void TimerInterrupt::resumeTimer(void)
{
  uint8_t andMask = 0b11111000;
  
  //Just restore the CSx2-CSx0 stored in _prescalerIndex. Still keep the count in TCNT and Timer Interrupt mask TIMKSx. 
  // 8 bit timers from here     
  #if defined(TCCR2B)
  if (_timer == 2)
  {
    TCCR2B = (TCCR2B & andMask) | _prescalerIndex;   //prescalarbits;

    TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
  }
  #endif

  // 16 bit timers from here
  #if defined(TCCR1B)
  #if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )
  if (_timer == 1)
  #else
  else if (_timer == 1)
  #endif
  {
    TCCR1B = (TCCR1B & andMask) | _prescalerIndex;   //prescalarbits;
    
    TISR_LOGWARN1(F("TCCR1B ="), TCCR1B); 
  }
  #endif
  
  #if defined(TCCR3B)
  else if (_timer == 3)
    TCCR3B = (TCCR3B & andMask) | _prescalerIndex;   //prescalarbits;
  #endif
  
  #if defined(TCCR4B)
  else if (_timer == 4)
    TCCR4B = (TCCR4B & andMask) | _prescalerIndex;   //prescalarbits;
  #endif
  
  #if defined(TCCR5B)
  else if (_timer == 5)
    TCCR5B = (TCCR5B & andMask) | _prescalerIndex;   //prescalarbits;
  #endif  
}


#if USE_TIMER_1 
  #ifndef TIMER1_INSTANTIATED
    // To force pre-instatiate only once
    #define TIMER1_INSTANTIATED
    static TimerInterrupt ITimer1(HW_TIMER_1);
    
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
    static TimerInterrupt ITimer2(HW_TIMER_2);
    
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
      static TimerInterrupt ITimer3(HW_TIMER_3);
      
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
      static TimerInterrupt ITimer4(HW_TIMER_4);
      
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
      static TimerInterrupt ITimer5(HW_TIMER_5);
      
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

#endif // TimerInterrupt_Impl_h
