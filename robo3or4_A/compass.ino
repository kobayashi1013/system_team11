//地磁気センサ

#define CALIBRATION_SAMPLES 70   // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60  // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ 0x1C     // CRA_REG_M value for magnetometer 220 Hz update rate

const float rc = 0.5;  //RCフィルタ
float filter[2][2];
const float dirCorrection = 0;
// float mx, my;
// float sum_e = 0;
int stamp = 0;

//狩野ロボのキャリブレーション値
void setupCompass() {
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS);  // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);     // 220 Hz compass update rate

  //キャリブレーションの初期値を設定
  //狩野
  // compass.m_min.x = -890;  
  // compass.m_min.y = -3533;
  // compass.m_max.x = 2558;
  // compass.m_max.y = 456;

  //小林君
  compass.m_min.x = 636;
  compass.m_min.y = -2237;
  compass.m_max.x = 3780;
  compass.m_max.y = 1629;


  // //宇和佐君
  // compass.m_min.x = -1618;
  // compass.m_min.y = -4990;
  // compass.m_max.x = 1689;
  // compass.m_max.y = -1159;


  delay(1000);  // 良く分からないが必要
}


void getCompass() {
  compass.read();  //センサ値取得

  // 地磁気センサ
  mx_G = map(compass.m.x, compass.m_min.x, compass.m_max.x, -128, 127);  //マッピング
  my_G = map(compass.m.y, compass.m_min.y, compass.m_max.y, -128, 127);

  //加速度センサ
  ax_G = compass.a.x / 256;
  ay_G = compass.a.y / 256;
}

//方角を変換
float correction(float angle) {
  while (angle < 0) angle += 360;
  while (angle > 360) angle -= 360;

  return angle;
}

//向いている方向を計算
float heading(float _mx, float _my) {
  float angle = atan2(_my, _mx) * 180 / M_PI + dirCorrection;
  angle = correction(angle);
  return angle;
}

//向く方向を調整（絶対量）
bool worldTurn(float* _rotSpeed, float angle) {
  //定数
  const float PItrg = 45.0;   //PI制御とP制御の境界
  const float Kp = 4.0;       //比例ゲイン
  const float Ti = 2;         //積分時間
  const float u_limit = 120;  //最大速度制限
  const float e_limit = 7;    //制御時の閾値(10かもしれない)

  static unsigned long _timePrev = millis();
  float u;
  bool ret = false;
  float TIinv = Ti / 1000.0;

  float e = angle - heading(mx_G, my_G);  //方向の残差
  if (e < -180) e += 360;                 //回転の向きを最適化
  if (e > 180) e -= 360;

  if (abs(e) > PItrg)  //P制御
  {
    sum_e = 0;
    u = Kp * e;
  } else  //PI制御
  {
    sum_e += TIinv * e * (millis() - _timePrev);  //----------------------------------------------
    u = Kp * (e + sum_e);
  }

  if (u > u_limit) u = u_limit;  //飽和
  if (u < -1 * u_limit) u = -1 * u_limit;

  //終了
  if (abs(e) <= e_limit) {
    ret = true;
    u = 0;
  }

  *_rotSpeed = u;
  _timePrev = millis();
  return ret;
}

//スタック検知
bool isStack() {
  //定数
  const float stack_limit = 50;  //スタック検知値

  bool ret = false;
  float acculate = sqrt(pow(ax_G, 2) + pow(ay_G, 2));
  if (acculate >= stack_limit) ret = true;

  return ret;
}










void CalibrationCompassManual() {
  compass.m_min.x = 0;
  compass.m_min.y = 0;
  compass.m_max.x = 0;
  compass.m_max.y = 0;
}

template<typename T> float heading(LSM303::vector<T> v) {
  float x_scaled = 2.0 * (float)(v.x - compass.m_min.x) / (compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled = 2.0 * (float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

// Yields the angle difference in degrees between two headings
float relativeHeading(float heading_from, float heading_to) {
  float relative_heading = heading_to - heading_from;

  // constrain to -180 to 180 degree range
  if (relative_heading > 180)
    relative_heading -= 360;
  if (relative_heading < -180)
    relative_heading += 360;

  return relative_heading;
}

// Average 10 vectors to get a better measurement and help smooth out
// the motors' magnetic interference.
float averageHeading() {
  LSM303::vector<int32_t> avg = {
    0, 0, 0
  };

  for (int i = 0; i < 10; i++) {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;

  // avg is the average measure of the magnetic vector.
  return heading(avg);
}

float averageHeadingLP() {
  static LSM303::vector<int32_t> avg = {
    0, 0, 0
  };

  compass.read();
  avg.x = 0.2 * compass.m.x + 0.8 * avg.x;
  avg.y = 0.2 * compass.m.y + 0.8 * avg.y;


  // avg is the average measure of the magnetic vector.
  return heading(avg);
}
