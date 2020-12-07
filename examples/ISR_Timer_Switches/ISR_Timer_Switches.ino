/****************************************************************************************************************************
  ISR_Timer_Switches.ino
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
  
  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      23/11/2019 Initial coding
  1.0.1   K Hoang      25/11/2019 New release fixing compiler error
  1.0.2   K.Hoang      28/11/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
  1.0.3   K.Hoang      01/12/2020 Add complex examples ISR_16_Timers_Array_Complex and ISR_16_Timers_Array_Complex
  1.1.1   K.Hoang      06/12/2020 Add example Change_Interval. Bump up version to sync with other TimerInterrupt Libraries
 *****************************************************************************************************************************/
/****************************************************************************************************************************
   ISR_Timer_Switches demontrates the use of ISR combining with Timer Interrupt to avoid being blocked by
   other CPU-monopolizing task

   In this complex example: CPU is connecting to WiFi, Internet and finally Blynk service (https://docs.blynk.cc/)
   Many important tasks are fighting for limited CPU resource in this no-controlled single-tasking environment.
   In certain period, mission-critical tasks (you name it) could be deprived of CPU time and have no chance
   to be executed. This can lead to disastrous results at critical time.
   We hereby will use interrupt to detect whenever one of 2 SWs is active, then use a hardware timer to poll and switch
   ON/OFF the corresponding sample relay (lamp)
   We'll see this ISR-based operation will have highest priority, preempts all remaining tasks to assure its
   functionality.
*****************************************************************************************************************************/
/****************************************************************************************************************************
    This example is currently written for Arduino Mega 2560 with ESP-01 WiFi or Mega2560-WiFi-R3
    You can easily convert to UNO and ESP-01
    Mega: Digital pin 18 – 21,2 and 3 can be used to provide hardware interrupt from external devices.
    UNO/Nano: Digital pin 2 and 3 can be used to provide hardware interrupt from external devices.
    To upload program to MEGA2560+WiFi, only turn ON SW 3+4 (USB <-> MCU).
    To run MEGA+WiFi combined, turn ON SW 1+2 (MCU <-> ESP) and SW 3+4 (USB <-> MCU)
 *****************************************************************************************************************************/

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG true

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt.h"
#include "ISR_Timer.h"

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#define BLYNK_HARDWARE_PORT     8080

#define USE_LOCAL_SERVER        true

// If local server
#if USE_LOCAL_SERVER
char blynk_server[]   = "yourname.duckdns.org";
//char blynk_server[]   = "192.168.2.110";
#else
char blynk_server[]   = "";
#endif

char auth[]     = "****";
char ssid[]     = "****";
char pass[]     = "****";

//Mega2560
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial3   //Serial1

// Your MEGA <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

#define TIMER_INTERVAL_MS           100

#define DEBOUNCE_TIME               25
#define LONG_BUTTON_PRESS_TIME_MS   100
#define DEBUG_ISR                   0

// Lamp 1
#define VPIN                        V1
#define LAMPSTATE_PIN               V2
// Lamp 2
#define V2PIN                       V3
#define LAMP2STATE_PIN              V4

//Blynk Color in format #RRGGBB
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"

// Lamp1 and Relay1
WidgetLED  LampStatus(LAMPSTATE_PIN);

volatile unsigned long  lastDebounceTime  = 0;
volatile bool           buttonPressed     = false;
volatile bool           alreadyTriggered  = false;

volatile bool LampState    = false;
volatile bool SwitchReset  = true;

#define RELAY_PIN     A0        // Pin A0 of Mega2560 / UNO / Nano
#define BUTTON_PIN    2         // Pin 2 can be use for external interrupt source on Maga/UNO/Nano

// Lamp2 and Relay2
WidgetLED  Lamp2Status(LAMP2STATE_PIN);

volatile unsigned long  lastDebounceTime2  = 0;
volatile bool           buttonPressed2     = false;
volatile bool           alreadyTriggered2  = false;

volatile bool Lamp2State    = false;
volatile bool Switch2Reset  = true;

#define RELAY2_PIN     A1
#define BUTTON2_PIN    3          // Pin 2 can be use for external interrupt source on Maga/UNO/Nano

unsigned int myWiFiTimeout        =  3200L;  //  3.2s WiFi connection timeout   (WCT)
unsigned int buttonInterval       =  511L;   //  0.5s update button state

// Lamp1
void Falling();
void Rising();

void lightOn();
void lightOff();
void ButtonCheck();
void ToggleRelay();

// Lamp2
void Falling2();
void Rising2();

void light2On();
void light2Off();
void ButtonCheck2();
void ToggleRelay2();

BlynkTimer blynkTimer;

BLYNK_CONNECTED()
{
  // Lamp 1
  LampStatus.on();
  Blynk.setProperty(LAMPSTATE_PIN, "color", LampState ? BLYNK_RED : BLYNK_GREEN );

  // Lamp 2
  Lamp2Status.on();
  Blynk.setProperty(LAMP2STATE_PIN, "color", Lamp2State ? BLYNK_RED : BLYNK_GREEN );

  Blynk.syncVirtual(VPIN);
}

// Lamp1
// Make this a autoreleased pushbutton
BLYNK_WRITE(VPIN)
{
  if (param.asInt())
    ToggleRelay();
}

void Rising()
{
  unsigned long currentTime  = millis();
  unsigned long TimeDiff;

  TimeDiff = currentTime - lastDebounceTime;
  if ( digitalRead(BUTTON_PIN) && (TimeDiff > DEBOUNCE_TIME) )
  {
    buttonPressed = false;
    lastDebounceTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Falling, FALLING);
  }
}

void Falling()
{
  unsigned long currentTime  = millis();

  if ( !digitalRead(BUTTON_PIN) && (currentTime > lastDebounceTime + DEBOUNCE_TIME))
  {
    lastDebounceTime = currentTime;
    buttonPressed = true;
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Rising, RISING);
  }
}

// Lamp2
// Make this a autoreleased pushbutton
BLYNK_WRITE(V2PIN)
{
  if (param.asInt())
    ToggleRelay2();
}

void Rising2()
{
  unsigned long currentTime  = millis();
  unsigned long TimeDiff;

  TimeDiff = currentTime - lastDebounceTime2;
  if ( digitalRead(BUTTON2_PIN) && (TimeDiff > DEBOUNCE_TIME) )
  {
    buttonPressed2 = false;
    lastDebounceTime2 = currentTime;
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Falling2, FALLING);
  }
}

void Falling2()
{
  unsigned long currentTime  = millis();

  if ( !digitalRead(BUTTON2_PIN) && (currentTime > lastDebounceTime2 + DEBOUNCE_TIME))
  {
    lastDebounceTime2 = currentTime;
    buttonPressed2 = true;
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Rising2, RISING);
  }
}

// Share for both buttons
void HWCheckButton()
{
  // Button 1
  if (!alreadyTriggered && buttonPressed)
  {
    alreadyTriggered = true;
    ButtonCheck();

  }
  else if (!buttonPressed)
  {
    // Reset flag when button released to avoid triggered repeatedly
    alreadyTriggered = false;
    ButtonCheck();
  }

  // Button 2
  if (!alreadyTriggered2 && buttonPressed2)
  {
    alreadyTriggered2 = true;
    ButtonCheck2();

  }
  else if (!buttonPressed2)
  {
    // Reset flag when button released to avoid triggered repeatedly
    alreadyTriggered2 = false;
    ButtonCheck2();
  }

}

void heartBeatPrint(void)
{
  static int num = 1;

  Serial.print("B");

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void checkButton()
{
  heartBeatPrint();

  // Lamp 1
  if (LampState)
    Blynk.setProperty(LAMPSTATE_PIN, "color", BLYNK_RED);
  else
    Blynk.setProperty(LAMPSTATE_PIN, "color", BLYNK_GREEN);

  // Lamp2
  if (Lamp2State)
    Blynk.setProperty(LAMP2STATE_PIN, "color", BLYNK_RED);
  else
    Blynk.setProperty(LAMP2STATE_PIN, "color", BLYNK_GREEN);
}

//Lamp 1
void ButtonCheck()
{
  boolean SwitchState = (digitalRead(BUTTON_PIN));

  if (!SwitchState && SwitchReset)
  {
    ToggleRelay();
    SwitchReset = false;
  }
  else if (SwitchState)
  {
    SwitchReset = true;
  }
}

void ToggleRelay()
{
  if (LampState)
    lightOff();
  else
    lightOn();
}

void lightOn()
{
  digitalWrite(RELAY_PIN, HIGH);
  LampState = true;
}

void lightOff()
{
  digitalWrite(RELAY_PIN, LOW);
  LampState = false;
}

// Lamp2
void ButtonCheck2()
{
  boolean SwitchState = (digitalRead(BUTTON2_PIN));

  if (!SwitchState && Switch2Reset)
  {
    ToggleRelay2();
    Switch2Reset = false;
  }
  else if (SwitchState)
  {
    Switch2Reset = true;
  }
}

void ToggleRelay2()
{
  if (Lamp2State)
    light2Off();
  else
    light2On();
}

void light2On()
{
  digitalWrite(RELAY2_PIN, HIGH);
  Lamp2State = true;
}

void light2Off()
{
  digitalWrite(RELAY2_PIN, LOW);
  Lamp2State = false;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting ISR_Timer_Switches");
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  LampState = false;

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Serial.println("\nStarting");
  Serial.print("ESPSerial using ");
  Serial.println(ESP8266_BAUD);


  // Lamp 1
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, LOW);
  LampState = false;

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Falling, FALLING);

  // Lamp 2
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  digitalWrite(RELAY2_PIN, LOW);
  Lamp2State = false;

  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Falling2, FALLING);

  // Interval in millisecs
  // Be sure to place this HW Timer well ahead blocking calls, because it needs to be initialized.
  if (ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS, HWCheckButton))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or interval");

  Blynk.begin(auth, wifi, ssid, pass, blynk_server, BLYNK_HARDWARE_PORT);

  if (Blynk.connected())
    Serial.println("Blynk connected");
  else
    Serial.println("Blynk not connected yet");

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(buttonInterval, checkButton);
}

void loop()
{
  Blynk.run();
  blynkTimer.run();
}
