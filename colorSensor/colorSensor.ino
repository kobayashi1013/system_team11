//以下インターフェース
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include "Adafruit_TCS34725.h"
ZumoMotors motor;
Pushbutton button(ZUMO_BUTTON);
float red, green, blue = 0;
void setup() {
  Serial.begin(9600);
  setupColorSensor();
  button.waitForButton();
  //calibrationColorSensor();
}
void loop()
{
  colorSensorMonitor(500);
}

//以下本体
#define colorNum 4

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);
float red_max, green_max, blue_max = 0;
float red_min, green_min, blue_min = 0;
unsigned int colorPos[colorNum][3] = {
  {255, 255, 255}, //白
  {12, 25, 28}, //黒
  {121, 2, 4}, //赤
  {1, 32, 96}, //青
};

//キャリブレーション用
void calibrationColorSensor()
{
  red_min = 32767, green_min = 32767, blue_min = 32767;
  red_max = -32767, green_max = -32767, blue_max = -32767;

  motor.setSpeeds(50, 50); //前進

  for (int i = 0; i < 100; i++)
  {
    colorSensor.getRGB(&red, &green, &blue);

    red_min = min(red_min, red);
    green_min = min(green_min, green);
    blue_min = min(blue_min, blue);
    red_max = max(red_max, red);
    green_max = max(green_max, green);
    blue_max = max(blue_max, blue);

    delay(50);
  }

  motor.setSpeeds(0, 0); //停止

  Serial.print("min : ");
  Serial.print(red_min);
  Serial.print(' ');
  Serial.print(green_min);
  Serial.print(' ');
  Serial.println(blue_min);
  Serial.print("max : ");
  Serial.print(red_max);
  Serial.print(' ');
  Serial.print(green_max);
  Serial.print(' ');
  Serial.println(blue_max);
}

//センサ監視
void colorSensorMonitor(int time)
{
  static unsigned long _timePrev = millis();

  if (millis() - _timePrev >= time)
  {
    _timePrev = millis();

    //色の観測
    /*getColorSensor(&red, &green, &blue);
    Serial.print("color : ");
    Serial.print(red);
    Serial.print(' ');
    Serial.print(green);
    Serial.print(' ');
    Serial.println(blue);*/

    //色の判定
    //Serial.println(identifyColor(red, green, blue));
  }
}

//センサのセットアップ
void setupColorSensor()
{
  colorSensor.begin();

  red_min = 47;
  green_min = 66;
  blue_min = 67;
  red_max = 255;
  green_max = 255;
  blue_max = 255;
}

//センサ値取得
void getColorSensor(float* _red, float* _green, float* _blue)
{
  //センサ値取得
  float r, g, b = 0;
  colorSensor.getRGB(&r, &g, &b);

  //線形変換
  *_red = map(r, red_min, red_max, 0, 255);
  *_green = map(g, green_min, green_max, 0, 255);
  *_blue = map(b, blue_min, blue_max, 0, 255);
}

//色の判別(N近傍法)
int identifyColor(int r, int g, int b)
{
  int color = -1;
  float minDistance = 99999;

  //全色との距離を計算
  for (int i = 0; i < colorNum; i++)
  {
    float distance = sqrt(pow(r - colorPos[i][0], 2)
      + pow(g - colorPos[i][1], 2)
      + pow(b - colorPos[i][2], 2));
    
    if (distance < minDistance)
    {
      minDistance = distance;
      color = i;
    }
  }

  return color;
}
