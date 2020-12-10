/****************************************************************************************************************************
  FakeAnalogWrite.ino
  For Arduino boards (UNO, Nano, Mega, etc. )
  Written by Khoi Hoang
  
  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
  Licensed under MIT license
    
  Now we can use these new 16 ISR-based timers, while consuming only 1 hardware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
  
  Notes:
  Special design is necessary to share data between interrupt code and the rest of your program.
  Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
  variable can not spontaneously change. Because your function may change variables while your program is using them,
  the compiler needs this hint. But volatile alone is often not enough.
  When accessing shared variables, usually interrupts must be disabled. Even with volatile,
  if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
  If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
  or the entire sequence of your code which accesses the data.
*****************************************************************************************************************************/
/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || \
    defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || \
    defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || \
    defined(ARDUINO_AVR_MINI) || defined(ARDUINO_AVR_ETHERNET) || defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT) || \
    defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED)

#else
  #error This is designed only for Arduino AVR board! Please check your Tools->Board setting.
#endif
  
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     false
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#define LOCAL_DEBUG         1

#include "TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

// For PWM_Value from 0-255.You can change to 1024 or 2048
#define MAX_PWM_VALUE         256

#define MAPPING_TABLE_SIZE    (MAX_PWM_VALUE / 10) + 1

// You have to calibrate and update this mapping table
float mappingTable[MAPPING_TABLE_SIZE] =
{ 0.0,     3.281,   6.860,  10.886,  15.285,  20.355,  26.096,  32.732,  40.785,  50.180,
  62.557,  79.557, 104.461, 136.075, 163.066, 181.930, 195.724, 207.132, 216.228, 223.684,
  230.395, 236.136, 241.206, 245.680, 249.781, 253.509
};

#define TIMER2_FREQUENCY_HZ     10000UL
#define TIMER2_INTERVAL_US      (1000000UL / TIMER2_FREQUENCY_HZ)

volatile uint32_t startMillis = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

//ISR_Timer ISR_Timer2;

#define LED_TOGGLE_INTERVAL_MS        500L

#define NUMBER_ISR_TIMERS         16

void TimerHandler(void)
{
  static bool toggle  = false;
  static uint32_t timeRun  = 0;

  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    doingSomething(i);
  }

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 500ms = 0.5s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * TIMER2_FREQUENCY_HZ) / 1000) )
  {   
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////



typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////


typedef struct
{
  int16_t       PWM_Value;          // Writing negative value to stop and free this PWM
  int16_t       PWM_PremapValue;    // To detect if use the same PWM_Value setting => don't do anything
  uint16_t      pin;
  uint32_t      countPWM;
  bool          beingUsed;
} ISRTimerData;

// Avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

///////////////////////////////////

volatile ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
};

void doingSomething(int index)
{
  // First check if enabled and pin != 0
  if ( (curISRTimerData[index].beingUsed) && (curISRTimerData[index].PWM_Value > 0) && (curISRTimerData[index].pin != 0) )
  {
    // Divide the time into MAX_PWM_VALUE slots.
    // PWM_Value = 0 => no digitalWrite(pin, 1)
    // PWM_Value > 0 => digitalWrite(pin, 1) from countPWM = 0 to countPWM = PWM_Value

    if (curISRTimerData[index].countPWM == 0)
    {
      if (curISRTimerData[index].PWM_Value > 0)
        digitalWrite(curISRTimerData[index].pin, 1);
      else
        digitalWrite(curISRTimerData[index].pin, 0);
    }
    else if (curISRTimerData[index].countPWM == curISRTimerData[index].PWM_Value)
    {
      digitalWrite(curISRTimerData[index].pin, 0);
    }
  }

  curISRTimerData[index].countPWM = (curISRTimerData[index].countPWM + 1) % MAX_PWM_VALUE;
}

///////////////////////////////////////////


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting FakeAnalogWrite on AVR board");
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.print("CPU Frequency = ");
  Serial.print(F_CPU / 1000000);
  Serial.println(" MHz");

  ITimer2.init();

  //if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler))
  if (ITimer2.attachInterrupt(TIMER2_FREQUENCY_HZ, TimerHandler))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq., duration or timer"); 

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    curISRTimerData[i].beingUsed      = false;
    curISRTimerData[i].pin            = 0;
    curISRTimerData[i].PWM_Value      = 0;

    //ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
  }
}

#define USING_MAPPING_TABLE   false

void fakeAnalogWrite(uint16_t pin, uint16_t value)
{
  uint16_t localValue;
  uint16_t localIndex = 0;

  // First check if already got that pin, then just update the PWM_Value
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    if ( (curISRTimerData[i].beingUsed) && (curISRTimerData[i].pin == pin) )
    {
      localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

      if (curISRTimerData[i].PWM_PremapValue == localValue)
      {
#if (LOCAL_DEBUG > 0)
        Serial.print("Ignore : Same Value for index = ");
        Serial.println(i);
#endif

        return;
      }
      else if (curISRTimerData[i].PWM_Value >= 0)
      {
        curISRTimerData[i].PWM_PremapValue = localValue;

        // Mapping to corect value
        if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
        {
          // Keep MAX_PWM_VALUE
          curISRTimerData[i].PWM_Value = localValue;
        }
        else
        {

#if USING_MAPPING_TABLE

          // Get the mapping index
          for (int j = 0; j < MAPPING_TABLE_SIZE; j++)
          {
            if ( (float) localValue < mappingTable[j])
            {
              localIndex = j - 1;
              break;
            }
          }

#if (LOCAL_DEBUG > 1)
          Serial.print("localIndex = ");
          Serial.println(localIndex);
#endif

          // Can use map() function
          // Can use map() function
          curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
                                         ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );

#else
          curISRTimerData[i].PWM_Value = localValue;
#endif

#if (LOCAL_DEBUG > 0)
          Serial.print("Update index = ");
          Serial.print(i);
          Serial.print(", pin = ");
          Serial.print(pin);
          Serial.print(", input PWM_Value = ");
          Serial.print(value);
          Serial.print(", mapped PWM_Value = ");
          Serial.println(curISRTimerData[i].PWM_Value);
#endif
        }
      }
      else
      {
        curISRTimerData[i].beingUsed      = false;
        curISRTimerData[i].pin            = 0;
        curISRTimerData[i].PWM_Value      = 0;
      }

      // Reset countPWM
      curISRTimerData[i].countPWM = 0;

      return;
    }
  }

  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    if (!curISRTimerData[i].beingUsed)
    {
      curISRTimerData[i].beingUsed    = true;
      curISRTimerData[i].pin          = pin;

      // Mapping to corect value
      localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

      if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
      {
        // Keep MAX_PWM_VALUE
        curISRTimerData[i].PWM_Value = localValue;
      }
      else
      {
        curISRTimerData[i].PWM_PremapValue = localValue;

#if USING_MAPPING_TABLE

        // Get the mapping index
        for (int j = 0; j < MAPPING_TABLE_SIZE; j++)
        {
          if ( (float) localValue < mappingTable[j])
          {
            localIndex = j - 1;
            break;
          }
        }

#if (LOCAL_DEBUG > 1)
        Serial.printf("localIndex = %d\n", localIndex);
#endif

        // Can use map() function
        // Can use map() function
        curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
                                       ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );
#else
        curISRTimerData[i].PWM_Value = localValue;
#endif
      }

      curISRTimerData[i].countPWM     = 0;

      pinMode(pin, OUTPUT);

#if (LOCAL_DEBUG > 0)
      Serial.print("Add index = ");
      Serial.print(i);
      Serial.print(", pin = ");
      Serial.print(pin);
      Serial.print(", input PWM_Value = ");
      Serial.print(value);
      Serial.print(", mapped PWM_Value = ");
      Serial.println(curISRTimerData[i].PWM_Value);
#endif

      return;
    }
  }
}

#define DELAY_BETWEEN_CHANGE_MS     5000L
#define REPEAT_INTERVAL_MS          10000L

#define DIVIDER                     5

void loop()
{
  for (int i = 0; i <= MAX_PWM_VALUE / DIVIDER; i++)
  {
    // Change the pin according to board
    // UNI, Nano, etc can use pins from 2-12. Pin 13 is used for LED_BUILTIN
    // Mega can use many more pins, such as 22-53

#if (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
    fakeAnalogWrite(22, i * DIVIDER);
    fakeAnalogWrite(23, i * DIVIDER);
    fakeAnalogWrite(24, i * DIVIDER);
    fakeAnalogWrite(25, i * DIVIDER);
    fakeAnalogWrite(26, i * DIVIDER);
    fakeAnalogWrite(27, i * DIVIDER);
    fakeAnalogWrite(28, i * DIVIDER);
    fakeAnalogWrite(29, i * DIVIDER);
    fakeAnalogWrite(30, i * DIVIDER);
    fakeAnalogWrite(31, i * DIVIDER);
    fakeAnalogWrite(32, i * DIVIDER);
    fakeAnalogWrite(33, i * DIVIDER);
    fakeAnalogWrite(34, i * DIVIDER);
    fakeAnalogWrite(35, i * DIVIDER);
    fakeAnalogWrite(36, i * DIVIDER);
    fakeAnalogWrite(37, i * DIVIDER);
#else
    fakeAnalogWrite( 2, i * DIVIDER);
    fakeAnalogWrite( 3, i * DIVIDER);
    fakeAnalogWrite( 4, i * DIVIDER);
    fakeAnalogWrite( 5, i * DIVIDER);
    fakeAnalogWrite( 6, i * DIVIDER);
    fakeAnalogWrite( 7, i * DIVIDER);
    fakeAnalogWrite( 8, i * DIVIDER);
    fakeAnalogWrite( 9, i * DIVIDER);
    fakeAnalogWrite(10, i * DIVIDER);
    fakeAnalogWrite(11, i * DIVIDER);
    fakeAnalogWrite(12, i * DIVIDER);
    fakeAnalogWrite(A0, i * DIVIDER);
    fakeAnalogWrite(A1, i * DIVIDER);
    fakeAnalogWrite(A2, i * DIVIDER);
    fakeAnalogWrite(A3, i * DIVIDER);
    fakeAnalogWrite(A4, i * DIVIDER);
#endif

#if (LOCAL_DEBUG > 0)
    Serial.print("Test PWM_Value = ");
    Serial.print(i * DIVIDER);
    Serial.print(", max = ");
    Serial.println(MAX_PWM_VALUE - 1);
#endif

    delay(DELAY_BETWEEN_CHANGE_MS);
  }

  Serial.println("===================");
  delay(REPEAT_INTERVAL_MS);
}
