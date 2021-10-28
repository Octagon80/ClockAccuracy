
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


#define DISPLAY_TIMEOUT 1000000
#define led_out 13
#define sensor_in 2

Adafruit_SSD1306 display(128, 32, &Wire, 4);

boolean val;
boolean oldval; 

uint32_t mytime = 0 ;
uint32_t oldtime = 0;
uint32_t diftime = 0;

uint32_t displayTimer, displayTimerOld = micros();



void displayInit(){
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // указываем адрес устройства на шине
   display.clearDisplay();
   display.setTextSize(1, 2); // указываем размер шрифта
   display.setTextColor(SSD1306_WHITE); // указываем цвет надписи

   display.setCursor(30, 10);
   display.println("Clock accuracy");
   display.display();
   delay(3000);
   display.clearDisplay(); // очищаем экран
}



/**
 * Отобразить на экране параметры 
 * Обновления экрана не чаще DISPLAY_TIMEOUT-uСек
 * @param period_us  uint32_t период колебания маятника, микросекунды
 */
void displayParam(uint32_t period_us){
  uint32_t dispSek, dispmSek;
  float f;
  //char str[40];
  
  
    displayTimer = micros();
    if( displayTimerOld - displayTimer > DISPLAY_TIMEOUT ){
     display.clearDisplay();
     f = period_us; f = (float) (f / 1000000 );
     display.setCursor(0, 0);  
     display.print(f);display.print(" cek"); 
     //dtostrf(f, 4, 2, str); display.print( str );display.print(" cek");
     f = diftime; f = (float) (1000000 / f );
     display.setCursor(0, 17); display.print(f); display.print(" Hz");
     display.display();
     displayTimerOld = displayTimer;
    }
}




void setup() {
   displayInit();
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

    oldtime = mytime;
    displayParam( diftime );
  }
}
