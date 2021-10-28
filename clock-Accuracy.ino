
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

#define led_out 13
#define sensor_in 2

Adafruit_SSD1306 display(128, 32, &Wire, 4);

boolean val;
boolean oldval; 

uint32_t mytime = 0 ;
uint32_t oldtime = 0;
uint32_t diftime = 0;

uint32_t tmr, tmrold = micros();
uint32_t dispSek, dispmSek;
float HZ;


void setup() {
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // указываем адрес устройства на шине
display.clearDisplay();
   display.setTextSize(1, 2); // указываем размер шрифта
   display.setTextColor(SSD1306_WHITE); // указываем цвет надписи

   display.setCursor(30, 10);
   display.println("Clock accuracy");
   display.display();
    delay(3000);
     display.clearDisplay(); // очищаем экран

  
  // put your setup code here, to run once:
  pinMode (led_out, OUTPUT);
  pinMode (sensor_in, INPUT);
}

void loop() {

  oldval = val;
  val = (digitalRead (sensor_in));

  if ((val == HIGH) && (oldval == LOW))  {
    digitalWrite (led_out, HIGH);
  }
  else if ((val == LOW) && (oldval == HIGH))  {
    digitalWrite (led_out, LOW);
    
      mytime = micros();
      diftime = mytime - oldtime;      
      
    tmr = micros();
    if( tmrold - tmr > 1000000 ){
      dispSek = (uint32_t)diftime / 1000000;
      dispmSek = (uint32_t) (diftime - (dispSek * 1000000));

       HZ = diftime; HZ = (float) (1000000 / HZ );
       
     display.clearDisplay();
     display.setCursor(0, 0);  display.print(dispSek); display.print(".");  display.print(dispmSek); display.print(" cek"); 
     display.setCursor(0, 17); display.print(HZ); display.print(" Hz");
     display.display();
     tmrold = tmr;
    }


   
    oldtime = mytime;
  }
}
