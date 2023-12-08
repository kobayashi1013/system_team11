#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>

#define CRB_REG_M_2_5GAUSS 0x60
#define CRA_REG_M_220HZ 0x1C
const float rc = 0.5;

ZumoMotors motor;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

float filter[2][2];
float sensor[4][3];

void setup()
{
  Serial.begin(9600);
  setupCompass();

  while (true)
  {
    for (int i = 0; i < 3; i++)
    {
      button.waitForButton();
      calibrationCompass(i);
    }

    sendData();
  }
}

void loop() {}

void setupCompass()
{
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS);
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);

  delay(1000);
}

void calibrationCompass(int count)
{
  //センサ取得
  compass.read();

  //初期化
  filter[0][0] = compass.m.x;
  filter[0][1] = compass.m.y;
  int mx_min = 32767, my_min = 32767;
  int mx_max = -32767, my_max = -32767;

  //計測開始
  motor.setSpeeds(150, -150);

  for (int i = 0; i < 90; i++)
  {
    compass.read();
    RC_Filter(compass.m.x, compass.m.y);

    mx_min = min(mx_min, filter[0][0]);
    my_min = min(my_min, filter[0][1]);
    mx_max = max(mx_max, filter[0][0]);
    my_max = max(my_max, filter[0][1]);

    delay(50);
  }

  //停止
  motor.setSpeeds(0, 0);

  //記録
  sensor[0][count] = mx_min;
  sensor[1][count] = my_min;
  sensor[2][count] = mx_max;
  sensor[3][count] = my_max;
}

//RCフィルタ
void RC_Filter(float mx, float my)
{
  //移動
  filter[1][0] = filter[0][0];
  filter[1][1] = filter[0][1];

  //フィルタ
  filter[0][0] = rc * filter[1][0] + (1 - rc) * mx;
  filter[0][1] = rc * filter[1][1] + (1 - rc) * my;
}

//バブルソート（計算量が多いため、今後変えるかも）
void sort(float _array[], const int _arrayNum)
{
  for (int i = 0; i < _arrayNum; i++)
  {
    for (int j = 0; j < _arrayNum; j++)
    {
      if (_array[i] > _array[j])
      {
        int _tmp = _array[i];
        _array[i] = _array[j];
        _array[j] = _tmp;
      }
    }
  }
}

//中央値
float center(const float _array[], const int _arrayNum)
{
  float _tmpArray[_arrayNum];

  //配列のコピー
  for (int i = 0; i < _arrayNum; i++) _tmpArray[i] = _array[i];

  //ソート
  sort(_tmpArray, _arrayNum);

  //中央値の計算
  float _center = 0;
  if (_arrayNum % 2 == 1)
  {
    _center = _tmpArray[_arrayNum / 2];
  }
  else
  {
    _center = (_tmpArray[_arrayNum / 2] + _tmpArray[_arrayNum / 2 + 1]) / 2;
  }

  return _center;
}

//データ送信
void sendData()
{
  Serial.println(int(center(sensor[0], 3)));
  Serial.println(int(center(sensor[1], 3)));
  Serial.println(int(center(sensor[2], 3)));
  Serial.println(int(center(sensor[3], 3)));
}