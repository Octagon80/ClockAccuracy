
/**
 * Калибровка мачтниковых часов.
 * Для работы датчик преграды установить в
 * крайнем положении маятника, что он своим дальним от центра краем 
 * попадал в зону действия датчика (на датчике будет зажигаться лампа,
 * при попадании в его зону действия мачтника)
 * 
 * Подключение
 * Датчик преграды  ---- Arduino
 *   + 5v
 *   gnd gnd
 *   out   ----->        D2  (для Arduino Uno, Nano, Mini)
 *   
 * Oled    ---- Arduino
 *   + 5v
 *   gnd gnd
 *   SDA   ----->        A4
 *   SCL   ----->        A5 
 */
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <TimerOne.h>
const int RPM_PIN=2;
const int PIN_LED=13;

Adafruit_SSD1306 display(128, 32, &Wire, 4);

volatile int rpm                = 0;
volatile int rpm_tik            = 0;   // Импульсы тахометра
volatile boolean kontrol;
volatile float rpm_array[3] = {0,0,0};   // массив для усреднения
volatile float rpm_result         = 0;   // частота вращения

uint32_t tmr, tmrold = micros();

void rpm_count(){
  rpm_tik++;
}


void SensorData(){
  rpm = rpm_tik;
  rpm_tik = 0;
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(500);
  digitalWrite(PIN_LED, LOW);
}
void setup(){
  Serial.begin(57600);


   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // указываем адрес устройства на шине
   display.clearDisplay();
   display.setTextSize(1, 2); // указываем размер шрифта
   display.setTextColor(SSD1306_WHITE); // указываем цвет надписи

   display.setCursor(30, 10);
   display.println("Clock accuracy");
   display.display();
    delay(3000);
     display.clearDisplay(); // очищаем экран

   

   pinMode(RPM_PIN,INPUT);
   attachInterrupt(digitalPinToInterrupt(RPM_PIN),  rpm_count,  RISING);
   Timer1.attachInterrupt(SensorData);
   Timer1.initialize(1000000);
   pinMode(PIN_LED, OUTPUT);
}


void loop(){
    rpm_result = 0;
    for (int i=0; i<=1; i++)    {
      rpm_array[i]=rpm_array[i+1];
    }    
    rpm_array[2] = /*60* определяем  в секунду*/rpm/2;
    for (int i=0; i<=2; i++)    {
      rpm_result = rpm_result+rpm_array[i]; 
    } 
    //Среднее
    rpm_result = rpm_result/3;


    tmr = micros();
    if( tmrold - tmr > 1000 ){
          display.clearDisplay();
     display.setCursor(0, 0);
     display.println(rpm_result);
     display.display();
     tmrold = tmr;
    }
      
    
}
