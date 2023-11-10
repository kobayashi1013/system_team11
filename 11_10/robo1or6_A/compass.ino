//地磁気センサ

#define CALIBRATION_SAMPLES 70   // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60  // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ 0x1C     // CRA_REG_M value for magnetometer 220 Hz update rate


//狩野ロボのキャリブレーション値
void setupCompass() {
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS);  // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);     // 220 Hz compass update rate

  //キャリブレーションの初期値を設定
  compass.m_min.x = -3060;
  compass.m_min.y = -5763;
  compass.m_max.x = -18;
  compass.m_max.y = -2222;

  delay(1000);  // 良く分からないが必要
}

// void calibrationCompass() {
//   int mx_min = 32767, my_min = 32767;
//   int mx_max = -32767, my_max = -32767;

//   motors.setSpeeds(200, -200);

//   for (int i = 0; i < 70; i++) {
//     compass.read();

//     mx_min = min(mx_min, compass.m.x);
//     my_min = min(my_min, compass.m.y);
//     mx_max = max(mx_max, compass.m.x);
//     my_max = max(my_max, compass.m.y);

//     delay(50);
//   }

//   motors.setSpeeds(0, 0);

//   Serial.print(mx_min);
//   Serial.print(' ');
//   Serial.println(my_min);
//   Serial.print(mx_max);
//   Serial.print(' ');
//   Serial.println(my_max);
// }


void getCompass() {
  compass.read();                                       //センサ値取得
  compass.m_min.x = min(compass.m_min.x, compass.m.x);  //キャリブレーション
  compass.m_min.y = min(compass.m_min.y, compass.m.y);
  compass.m_max.x = max(compass.m_max.x, compass.m.x);
  compass.m_max.y = max(compass.m_max.y, compass.m.y);
  mx_G = map(compass.m.x, compass.m_min.x, compass.m_max.x, -128, 127);  //マッピング
  my_G = map(compass.m.y, compass.m_min.y, compass.m_max.y, -128, 127);
}

//方角を変換
float correction(float angle) {
  while (angle < 0) angle += 360;
  while (angle > 359) angle -= 360;

  return angle;
}

//向いている方向を計算
float heading(float _mx, float _my) {
  float angle = atan2(_my, _mx) * 180 / M_PI;
  angle = correction(angle);
  return angle;
}

//向く方向を調整（絶対量）
bool worldTurn(float angle) {
  //定数
  const float PItrg = 45.0;   //PI制御とP制御の境界
  const float Kp = 4.0;       //比例ゲイン
  const float Ti = 0.5;       //積分時間
  const float u_limit = 180;  //最大速度制限
  const float e_limit = 5;    //制御時の閾値(10かもしれない)

  float u;
  float TIinv = 1 / (Ti * 1000);

  float e = angle - heading(mx_G, my_G);  //方向の残差
  if (e < -180) e += 360;                 //回転の向きを最適化
  if (e > 180) e -= 360;

  if (abs(e) <= e_limit) return true;

  if (abs(e) > PItrg)  //P制御
  {
    u = Kp * e;
  } else  //PI制御
  {
    sum_e += TIinv * e * (timeNow_G - timePrev_G);
    u = Kp * (e + sum_e);
  }

  if (u > u_limit) u = u_limit;  //飽和
  if (u < -1 * u_limit) u = -1 * u_limit;

  rotSpeed = u;
  return false;
}

//向く方向を調整（変位量）
bool localTurn(float angleDiff) {
  static int _mode = 0;
  static float _angleOffset = 0;

  if (_mode == 0) {
    _angleOffset = heading(mx_G, my_G) + angleDiff;  //目標の角度を計算
    if (_angleOffset > 360) _angleOffset -= 360;

    _mode = 1;
    return false;
  } else if (_mode == 1) {
    if (worldTurn(_angleOffset)) {
      _mode = 0;
      return true;
    } else {
      return false;
    }
  }
  return false;
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
