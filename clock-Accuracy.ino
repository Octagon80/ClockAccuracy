
/**
 * Калибровка маятниковых часов.
 * Для работы датчик преграды установить в
 * крайнем положении отклонения маятника, так чтобы он своим дальним от центра краем 
 * попадал в зону действия датчика (на датчике будет зажигаться лампа,
 * при попадании в его зону действия маятника)
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
 *   
 *   Для разных часов - разные частоты.
 *   ОЧЗ с длиным маятником имеет 6270 полуколебаний в час, или 104.5 в минуту или 1.74 в секунду (0.57 гц)
 *   Янтарь с коротким маятником  имеет 7200 полуколебаний в час, или 120 в минуту или 2 в секунду (0.5 гц)
 *   
 *   Поэтому данный прибор 
 *      для ОЧЗ-длиный должен показывать период 1.74*2=3.48 (1.14 Гц)
 *      для янтаря-коротки  должен показывать период 2*2=4 (0.25 Гц)
 *   
 
 */
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

//Количество измерений периодов для вычисления "среднего" периода.
//при 60 - период за минуту
#define MAX 60

#define DISPLAY_TIMEOUT 1000000
#define led_out 13
#define sensor_in 2

#define DISP_HEIGHT 32
#define DISP_WIDTH 128

Adafruit_SSD1306 display(DISP_WIDTH, DISP_HEIGHT, &Wire, 4);

boolean val;
boolean oldval; 

uint32_t mytime = 0 ;
uint32_t oldtime = 0;
// "Мгновенный" период колебания, получаемый при каждом срабатывании датчика
uint32_t difTimeInSek = 0;

//Количество измерений
byte     countMotion = 0;
//Режим: 0 - стоп, 1-измерение
byte mode = 0;
float difTimeAvareage = 0;


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
 * @param countMotion byte число проведенных измерений (== числу качаний маятника)
 * @param period_us  float  период колебания маятника, микросекунды
 */
void displayParam(byte countMotion, float period_us){
  uint32_t dispSek, dispmSek;
  float f;
  //char str[40];
  
  
    displayTimer = micros();
    if( displayTimerOld - displayTimer > DISPLAY_TIMEOUT ){
     display.clearDisplay();
/*
dispSek = DISP_HEIGHT / 2;
display.drawLine(15, dispSek-3, 15, dispSek+3, SSD1306_WHITE);
display.drawLine(12, dispSek, 18, dispSek, SSD1306_WHITE);
*/
     //Число колебаний
     display.setCursor(0, 0); display.print(countMotion); 
  //Оставшееся количество
     display.setCursor(0, 17); display.print( (MAX - countMotion ) ); 

     //Период
     f = period_us; f = (float) (f / 1000000 );
     display.setCursor(17, 0);  
     display.print(f);display.print(" cek"); 

     //Частота
     //dtostrf(f, 4, 2, str); display.print( str );display.print(" cek");
     f = period_us; f = (float) (1000000 / f );
     display.setCursor(17, 17); display.print(f); display.print(" Hz");
     display.display();
     displayTimerOld = displayTimer;
    }
}




void setup() {
   displayInit();
  // put your setup code here, to run once:
  pinMode (led_out, OUTPUT);
  pinMode (sensor_in, INPUT);
  countMotion = 0; mode = 1;
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
    difTimeInSek = mytime - oldtime;    

    //количество измерений увеличиваем,  
    countMotion++; 
    //если количество измерений прошло нужно число раз, стоп
    if( countMotion > MAX ){ 
      countMotion = MAX; mode = 0;
    } else{
      //считаем среднее
      difTimeAvareage = (difTimeAvareage + difTimeInSek) / 2;
    }

    oldtime = mytime;
    displayParam( countMotion, difTimeAvareage );
  }
}
