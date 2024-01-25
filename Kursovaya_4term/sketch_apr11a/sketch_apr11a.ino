#include <Servo.h>
#include "Wire.h" 
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C LCD(0x3F, 16, 2);
int SEC = 0;   // переменные для отсчета минут и секунд
int MIN = 0;
unsigned long timer;
byte w = 0;

//bool flag = false;
    
Servo myservo;
int pos = 0;
const byte PIN_ANALOG_X = 1;
const byte PIN_ANALOG_Y = 0;
int xPin = A1;
int yPin = A0;
int xPosition = 0;
int yPosition = 0;
const int X_THRESHOLD_LOW = 480;
const int X_THRESHOLD_HIGH = 540;
const int Y_THRESHOLD_LOW = 480;
const int Y_THRESHOLD_HIGH = 540;
int x_position;
int y_position;
int y2_position;
int x2_position;
int x_direction;
int y_direction;
int x2_direction = 2;
int y2_direction = 2;
int x3_direction = 0;
int y3_direction = 0;

void setup() 
{
Serial.begin(9600);
pinMode(xPin, INPUT);
pinMode(yPin, INPUT);
myservo.attach(6);
//pinMode(4, INPUT_PULLUP);

LCD.init();                        // инициализация дисплея
LCD.backlight();
LCD.setCursor(2, 0);
LCD.print("TIMER  STOP");
LCD.setCursor(5, 1);
LCD.print(MIN);
LCD.print(" : ");
LCD.print(SEC);
}

void loop() 
{
/*bool button = !digitalRead(4);
if (button && !flag) {  // обработчик нажатия
    flag = true;
    Serial.println("press");
  }
  if (!button && flag) {  // обработчик отпускания
    flag = false;  
    //Serial.println("release");
  }*/
x_direction = 0;
y_direction = 0;
x_position = analogRead(PIN_ANALOG_X);
y_position = analogRead(PIN_ANALOG_Y);

if (x_position > X_THRESHOLD_HIGH) 
{
  x_direction = 1;
} 
else if (x_position < X_THRESHOLD_LOW) 
{
  x_direction = -1;
}
if (y_position > Y_THRESHOLD_HIGH) 
{
  y_direction = 1;
} 
else if (y_position < Y_THRESHOLD_LOW) 
{
  y_direction = -1;
}

if (x_direction == 0 && y_direction == 0)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  centered");
  }
}
else if (x_direction == -1 && y_direction == -1) 
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  left-down");
  }
} 
else if (x_direction == -1 && y_direction == 0) 
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  left");
    if(SEC > 30)
    {
      SEC = SEC - 30;
    }
    else if(SEC == 0)
    {
      if(MIN != 0)
      {
        MIN--;
        SEC = 30;
      }
    }
    else
    {
      SEC = 0;
    }
    LCD.clear(); 
    LCD.setCursor(2, 0); 
    LCD.print("TIMER   SET"); 
    LCD.setCursor(5, 1); 
    LCD.print(MIN); 
    LCD.print(" : "); 
    LCD.print(SEC);
  }
} 

else if (x_direction == -1 && y_direction == 1)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  left-up");
  }
} 
else if (x_direction == 0 && y_direction == -1)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  down");
    w = 1;
    while (w == 1 ) 
    {
      // если прошло 1000 мс - вычитаем одну секунду от переменной SEC
      if (millis() - timer > 1000) 
      {
        timer = millis();
        if(SEC > 0)
        {
           SEC--;
        }
        // если отсчет закончился - включаем сигнал и выходим из цикла
        if (SEC == 0 && MIN == 0) 
        {
          SEC = 0; MIN = 0;
          LCD.clear();
          LCD.setCursor(2, 0);
          LCD.print("TIMER  STOP");
          LCD.setCursor(5, 1);
          LCD.print(MIN);
          LCD.print(" : ");
          LCD.print(SEC);
          digitalWrite(10, HIGH);
          tone(12, 100);
          delay(500);
          noTone(12);
          break;
        }

        // если секунды дошли до нуля - вычитаем одну минуту
        if (SEC == 0) 
        {
           SEC = 59; MIN = MIN - 1;
           if (MIN <= 0 ) 
           { 
             MIN = 0; 
           }
        }
        // если из цикла while еще не вышли - выводим информацию на дисплей
        if (w == 1) 
        {
          LCD.clear();
          LCD.setCursor(2, 0);
          LCD.print("TIMER START");
          LCD.setCursor(5, 1);
          LCD.print(MIN);
          LCD.print(" : ");
          LCD.print(SEC);
        }
      }
    } 
  }
}
else if (x_direction == 0 && y_direction == 1)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  up");
    MIN = 0; SEC = 0;
    digitalWrite(10, HIGH);
    LCD.clear();
    LCD.setCursor(2, 0);
    LCD.print("TIMER STOP");
    LCD.setCursor(5, 1);
    LCD.print(MIN);
    LCD.print(" : ");
    LCD.print(SEC);
    
  }
}
else if (x_direction == 1 && y_direction == -1)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  right-down");
  }
}
else if (x_direction == 1 && y_direction == 0)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  right");
    SEC = SEC + 30;
    if(SEC == 60)
    {
      MIN++;
      SEC = 0;
    }
    LCD.clear(); 
    LCD.setCursor(2, 0); 
    LCD.print("TIMER   SET"); 
    LCD.setCursor(5, 1); 
    LCD.print(MIN); 
    LCD.print(" : "); 
    LCD.print(SEC);
  }
} 
else if (x_direction == 1 && y_direction == 1)
{
  if(x_direction != x2_direction || y_direction != y2_direction)
  {
    Serial.println("  right-up");
  }
}
int X = analogRead(xPin);
X = map(X, 0, 1023, 0, 180);
myservo.write(X);
x2_direction = x_direction;
y2_direction = y_direction;
}
