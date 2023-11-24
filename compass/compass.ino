//以下インターフェース
#include <Wire.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <LSM303.h>
ZumoMotors motor;
ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);

LSM303 compass;
float speed = 150;
int mode = 0;
void setup()
{
  Serial.begin(9600);
  setupCompass();

  button.waitForButton();
  //calibrationCompass();
  //button.waitForButton();
}
void loop()
{
  getCompass();

  switch (mode)
  {
    case 0:
      mode = 1;
      break;
    case 1:
      /*if (worldTurn(&speed, 90)) {
        mode = 99;
      }*/
      //compassMonitor(50);
      //if (isStack()) buzzer.play(">>a32");
      break;
    case 99:
      break;
  }

  //motor.setSpeeds(speed, speed);
}

//以下本体
#define CRB_REG_M_2_5GAUSS 0x60
#define CRA_REG_M_220HZ 0x1C

const float rc = 0.5; //RCフィルタ
const float dirCorrection = 0; //補正角度

float filter[2][2];
float mx, my = 0;
float ax, ay = 0;
float sum_e = 0;
int stamp = 0;

//デバッグ
void compassMonitor(const int _interval)
{
  static unsigned long _timePrev = millis();

  if (millis() - _timePrev >= _interval)
  {
    _timePrev = millis();

    //実値
    /*compass.read();
    Serial.print(compass.m.x);
    Serial.print(" ");
    Serial.println(compass.m.y);*/

    //調整値
    //getCompass();
    /*Serial.print(mx);
    Serial.print(" ");
    Serial.println(my);*/
    /*Serial.print(ax);
    Serial.print(" ");
    Serial.println(ay);*/

    //方向
    /*getCompass();
    Serial.println(heading(mx, my));*/
    //加速
    /*getCompass();
    Serial.println(sqrt(pow(ax, 2) + pow(ay, 2)));*/
  }
}

//デバッグ
void putStamp(int _time)
{
  static unsigned long _timePrev = millis();

  if (millis() - _timePrev >= _time)
  {
    _timePrev = millis();
    Serial.println(stamp++);
  }
}

void calibrationCompass()
{
  //最初の計測
  compass.read();
  filter[0][0] = compass.m.x;
  filter[0][1] = compass.m.y;

  int mx_min = 32767, my_min = 32767;
  int mx_max = -32767, my_max = -32767;

  motor.setSpeeds(200, -200);

  for (int i = 0; i < 100; i++)
  {
    compass.read();
    RC_Filter(compass.m.x, compass.m.y);

    mx_min = min(mx_min, filter[0][0]);
    my_min = min(my_min, filter[0][1]);
    mx_max = max(mx_max, filter[0][0]);
    my_max = max(my_max, filter[0][1]);

    delay(50);
  }

  motor.setSpeeds(0, 0);

  Serial.print(mx_min);
  Serial.print(' ');
  Serial.println(my_min);
  Serial.print(mx_max);
  Serial.print(' ');
  Serial.println(my_max);
}

void setupCompass()
{
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS);
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);

  //キャリブレーションの初期値を設定
  compass.m_min.x = 151;
  compass.m_min.y = -3769;
  compass.m_max.x = 2946;
  compass.m_max.y = -361;
  
  delay(1000); //必要
}

//地磁気センサ
void getCompass()
{
  compass.read(); //センサ値取得

  //地磁気センサ
  mx = map(compass.m.x, compass.m_min.x, compass.m_max.x, -128, 127);
  my = map(compass.m.y, compass.m_min.y, compass.m_max.y, -128, 127);

  //加速度センサ
  ax = compass.a.x / 256;
  ay = compass.a.y / 256;
}

//RCフィルタ
void RC_Filter(float _mx, float _my)
{
  //移動
  filter[1][0] = filter[0][0];
  filter[1][1] = filter[0][1];

  //フィルタ
  filter[0][0] = rc * filter[1][0] + (1 - rc) * _mx;
  filter[0][1] = rc * filter[1][1] + (1 - rc) * _my;
}

//方角を変換
float correction(float _angle)
{
  while (_angle < 0) _angle += 360;
  while (_angle > 360) _angle -= 360;

  return _angle;
}

//向いている方向を計算
float heading(float _mx, float _my)
{
  float angle = atan2(_my, _mx) * 180 / M_PI + dirCorrection;
  angle = correction(angle);
  return angle;
}

//向く方向を調整（絶対量）
bool worldTurn(float* _rotSpeed, float _angle)
{
  //定数
  const float PItrg = 45.0; //PI制御とP制御の境界
  const float Kp = 4.0; //比例ゲイン
  const float Ti = 2; //積分時間
  const float u_limit = 180; //最大速度制限
  const float e_limit = 5; //制御時の閾値

  static unsigned long _timePrev = millis();
  float u;
  bool ret = false;
  float TIinv = Ti / 1000.0;

  float e = _angle - heading(mx, my); //方向の残差
  if (e < -180) e += 360; //回転の向きを最適化
  if (e > 180) e -= 360;

  if (abs(e) > PItrg) //P制御
  {
    sum_e = 0;
    u = Kp * e;
  }
  else //PI制御
  {
    sum_e += TIinv * e * (millis() - _timePrev);
    u = Kp * (e + sum_e);
  }

  if (u > u_limit) u = u_limit; //飽和
  if (u < -1 * u_limit) u = -1 * u_limit;

  //終了
  if (abs(e) <= e_limit)
  {
    ret = true;
    u = 0;
  }

  *_rotSpeed = u;
  _timePrev = millis();
  return ret;
}

//スタック検知
bool isStack()
{
  //定数
  const float stack_limit = 10; //スタック検知値

  bool ret = false;
  float acculate = sqrt(pow(ax, 2) + pow(ay, 2));
  if (acculate >= stack_limit) ret = true;

  return ret;
}