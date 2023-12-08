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
float sum[4] = { 0 };

void setup()
{
  Serial.begin(9600);
  setupCompass();

  for (int i = 0; i < 3; i++)
  {
    button.waitForButton();
    calibrationCompass();
  }

  sendData();
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

void calibrationCompass()
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

  //停止
  motor.setSpeeds(0, 0);

  //記録
  sum[0] += mx_min;
  sum[1] += my_min;
  sum[2] += mx_max;
  sum[3] += my_max;
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

//データ送信
void sendData()
{
  Serial.println(int(sum[0] / 3));
  Serial.println(int(sum[1] / 3));
  Serial.println(int(sum[2] / 3));
  Serial.println(int(sum[3] / 3));
}