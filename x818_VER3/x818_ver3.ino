#include <EEPROM.h>
// PB4 vacum STM32
// PA5 led ok
// PB5 led ng
// PB12 RESET
#define VACUM_PIN 15 // D8
#define RESET_PIN 0  // D3
#define LED_NG    4  // D2
#define LED_OK    5  // D1
#define LED 16
#define SIGNAL_MAKING 14  // D5
#define MAKING_PITTONG 13  // D7


#define VACUM_ON digitalWrite(VACUM_PIN, HIGH)
#define VACUM_OFF digitalWrite(VACUM_PIN, LOW)
#define STATUS_RESET digitalRead(RESET_PIN)
#define STATUS_LED_NG digitalRead(LED_NG)
#define STATUS_LED_OK digitalRead(LED_OK)
#define LED_ON digitalWrite(LED, HIGH)
#define LED_OFF digitalWrite(LED, LOW)
#define STATUS_SIGNAL_MAKING digitalRead(SIGNAL_MAKING)
#define STATUS_MAKING_PITTONG digitalRead(MAKING_PITTONG)
#define OFF 0
#define ON 1
#define ANDRESS 0


uint8_t count;
uint8_t triger;
uint8_t status_reset;
uint8_t status_led_ok;
uint8_t status_led_ng;
uint8_t status_led;
uint8_t status_making;
uint8_t status_pittong_making;
uint32_t timer ;

bool triger_reset;
uint16_t count_reset;
uint32_t timer_reset;
uint8_t press_reset;
uint8_t set_mode;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(VACUM_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(LED_NG, INPUT);
  pinMode(LED_OK, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SIGNAL_MAKING, INPUT);
  pinMode(MAKING_PITTONG, INPUT);

  EEPROM.begin(10);

  VACUM_OFF;
  LED_OFF;

  triger_reset = EEPROM.read(ANDRESS);
  delay(100);
  digitalWrite(LED, triger_reset);

  Serial.println(triger_reset);

}

void loop() {
  // put your main code here, to run repeatedly:
  status_reset     = STATUS_RESET;
  status_led_ng    = STATUS_LED_NG;
  status_led_ok    = STATUS_LED_OK;
  status_making    = STATUS_SIGNAL_MAKING;
  status_pittong_making = STATUS_MAKING_PITTONG;
  //Serial.println(status_led_ok);

  if (status_led_ng == 1 && status_led_ok == 1) status_led = 0;

  if (set_mode == 0)
  {
    if (status_reset == 0)
    {
      if (millis() - timer_reset > 100)
      {
        count_reset ++;
        timer_reset = millis();
      }
      if (count_reset >= 20)
      {
        triger_reset = !triger_reset;
        EEPROM.write(ANDRESS, triger_reset);
        EEPROM.commit();
        Serial.println(triger_reset);
        digitalWrite(LED, triger_reset);
        set_mode = 1;
      }
    }
  }
  if (set_mode == 1)
  {
    if (status_reset == 1)
    {
      count_reset = 0;
      set_mode = 0;
    }
  }


  if (triger_reset == OFF)
  {
    if (triger == 0)
    {
      // khi an nut reset hoac tin hieu making
      if (status_reset == 0 && status_pittong_making == 0)
      {
        triger = 2;
        VACUM_OFF;
      }
      if (status_making == 1) triger = 5;
    }
  }


  if (triger == 5)
  {
    if (status_making == 0) triger = 6;
  }
  if (triger == 6)
  {
    if (status_making == 1)
    {
      triger = 4;
      VACUM_OFF;
    }
  }
  // su kien khi an nut reset va led ng sang
  if (triger == 1 || triger == 4 || triger == 2)
  {
    if (millis() - timer > 100)
    {
      count++;
      timer = millis();
    }
    // doi truoc khi mo van
    if (count <= 3) VACUM_OFF;
    else
    {
      // thoi gian bat van
      if (count > 13)
      {
        VACUM_OFF;
        triger = 3;
      }
      else VACUM_ON;
    }
  }



  // ket thuc va reset bien
  if (triger == 3)
  {
    if (status_reset == 1)
    {
      triger = 0;
      count = 0;
    }
  }
}
