#define CRB_REG_M_2_5GAUSS 0x60
#define CRA_REG_M_220HZ 0x1C

void calibrationCompass()
{
  int mx_min = 32767, my_min = 32767;
  int mx_max = -32767, my_max = -32767;

  motor.setSpeeds(200, -200);

  for (int i = 0; i < 70; i++)
  {
    compass.read();

    mx_min = min(mx_min, compass.m.x);
    my_min = min(my_min, compass.m.y);
    mx_max = max(mx_max, compass.m.x);
    my_max = max(my_max, compass.m.y);

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
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS);
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);

  //キャリブレーションの初期値を設定
  compass.m_min.x = 1370;
  compass.m_min.y = -308;
  compass.m_max.x = 3500;
  compass.m_max.y = 2286;
  
  delay(1000); //必要
}

//地磁気センサ
void getCompass()
{
  compass.read(); //センサ値取得
  compass.m_min.x = min(compass.m_min.x, compass.m.x); //キャリブレーション
  compass.m_min.y = min(compass.m_min.y, compass.m.y);
  compass.m_max.x = max(compass.m_max.x, compass.m.x);
  compass.m_max.y = max(compass.m_max.y, compass.m.y);
  mx = map(compass.m.x, compass.m_min.x, compass.m_max.x, -128, 127); //マッピング
  my = map(compass.m.y, compass.m_min.y, compass.m_max.y, -128, 127);
}

//方角を変換
float correction(float _angle)
{
  while (_angle < 0) _angle += 360;
  while (_angle > 359) _angle -= 360;

  return _angle;
}

//向いている方向を計算
float heading(float _mx, float _my)
{
  float angle = atan2(_my, _mx) * 180 / M_PI;
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
  const float e_limit = 3; //制御時の閾値

  float u;
  float TIinv = Ti / 1000.0;

  float e = _angle - heading(mx, my); //方向の残差
  if (e < -180) e += 360; //回転の向きを最適化
  if (e > 180) e -= 360;

  if (abs(e) <= e_limit) return true;

  if (abs(e) > PItrg) //P制御
  {
    u = Kp * e;
  }
  else //PI制御
  {
    sum_e += TIinv * e * (timeNow - timePrev);
    u = Kp * (e + sum_e);
  }

  if (u > u_limit) u = u_limit; //飽和
  if (u < -1 * u_limit) u = -1 * u_limit;

  *_rotSpeed = u;
  return false;
}

//向く方向を調整（変位量）
bool localTurn(float* _rotSpeed, float _angleDiff)
{
  static int _mode = 0;
  static float _angleOffset = 0;

  if (_mode == 0)
  {
    _angleOffset = heading(mx, my) + _angleDiff; //目標の角度を計算
    _angleOffset = correction(_angleOffset);
    
    _mode = 1;
    return false;
  }
  else if (_mode == 1)
  {
    if (worldTurn(_rotSpeed, _angleOffset))
    {
      _mode = 0;
      return true;
    }
    else
    {
      return false;
    }
  }

  return false;
}