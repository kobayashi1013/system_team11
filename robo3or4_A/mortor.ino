//モーター制御
void task_Red() {  //自陣：赤の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  static int s_count = 0;  //search_count 探索位置（自陣か真ん中）のきりかえ
  int cup_dist = 0;        //カップとの距離
  static int c_count = 0;  //カップを見失った際の探索の最大回数
  static int m_count = 0;  //↑モータ回転の切り替え

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 14;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = 160;      //時計回り
      if (waitfor(700)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;        //mode 2へ
        // sum_e = 0;   //初期化---(worldturn関数使い終わった時)----------------------------------------
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 200;
      rotSpeed = 0;
      if (s_count % 2 == 0) {  //偶数の場合↓
        if (waitfor(2300)) {   //----------------------2秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;          //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {               //奇数の場合↓
        if (waitfor(700)) {  //----------------------1秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;        //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      }
      break;

    case 3:  //【time取得】，探索モード(回転)
      speed = 0;
      rotSpeed = -150;
      if (timeNow_G - startTime < 2000) {  //の間に↓
        if (search()) {                    //障害物を見つけたら↓
          rotSpeed = 0;                    //停止させる
          mode_G = 5;                      //mode 5へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {
        mode_G = 4;  //1秒すぎたらmode 4
        startTime = timeNow_G;
      }
      break;

    case 4:  //【time取得】,探索モード（直進）
      speed = 200;
      rotSpeed = 0;
      if (timeNow_G - startTime < 750) {  //の間に↓
        if (search()) {                   //障害物を見つけたら↓
          rotSpeed = 0;                   //停止させる
          mode_G = 5;                     //mode 5へ
          startTime = timeNow_G;          //スタート時間取得
        }
      } else {
        mode_G = 3;  //すぎたらmode 3(探索　回転)
        startTime = timeNow_G;
      }
      break;

    case 5:                                //【time取得】，障害物を見つけたら，ロボかカップか判断
      if (timeNow_G - startTime < 1800) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 101;                    //mode 101　　へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
      }
      break;

    case 6:  // カップを見失った場合の探索
      speed = 0;
      // rotSpeed = 150;
      if (m_count % 2 == 0) {  //時計回りか反時計回りに切り替え
        rotSpeed = 150;
      } else {
        rotSpeed = -150;
      }

      if (c_count < 3) {
        if (search()) {           //障害物を見つけたら↓
          rotSpeed = 0;           //停止させる
          mode_G = 7;             //mode 7へ
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {        //カップ探しきったら
        mode_G = 4;   //mode 4
        c_count = 0;  //初期化
        startTime = timeNow_G;
      }
      break;

    case 7:                                //【time取得】, カップを見失った場合のカップかロボかの判断
      if (timeNow_G - startTime < 1800) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 6;                      //カップ探索に戻る
          c_count++;                       //探索回数かぞえる
          m_count++;                       //モータ回転切り替え
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
      }
      break;

    case 10:        //カップホールドモード
      speed = 200;  //直進
      rotSpeed = 0;
      cup_dist = distance();  //カップとの距離計測
      if (cup_dist >= 50) {   //カップつかめなかったら
        speed = 0;
        mode_G = 6;
        startTime = timeNow_G;
      }
      if (Cup_dist()) {  //カップつかんだら
        speed = 0;       //停止
        mode_G = 11;     //mode 11へ
      }
      break;

    case 11:                            //ゴールモード，ゴール方向へ回転
      if (worldTurn(&rotSpeed, 110)) {  //南東（180-90）方向へ回転したら
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:
      speed = 180;  //直進
      rotSpeed = 0;
      if (color == BLACK) {  //黒を見つけたら
        mode_G = 13;
        speed = 0;
      }
      break;

    case 13:  //自陣方向へ回転
      speed = 0;
      rotSpeed = 140;      //時計回り
      if (waitfor(900)) {  //------------------------------------------------------------------
        speed = 190;       //前進
        rotSpeed = 0;
        mode_G = 14;
      }
      break;

    case 14:
      if (color == RED) {  //赤を見つけたら
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 99;
        startTime = timeNow_G;  //スタート時間取得
        // Serial.println("d2");
        buzzer.play(">c32");  //一回ブザー音を鳴らす
      }
      break;

    case 99:        //【time取得】，カップをゴールライン外へ出す
      speed = 160;  //0.4秒前進
      rotSpeed = 0;
      if (timeNow_G - startTime > 2700) {
        s_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      } else if (timeNow_G - startTime > 1200) {
        speed = 0;
        rotSpeed = -190;  //1.8秒間回転，反時計回り
      } else if (timeNow_G - startTime > 400) {
        speed = -190;  //カップを外に出したら0.5秒後退
      }
      // if (timeNow_G - startTime > 700) {
      //   speed = -160;  //カップを外に出したら1秒後退
      // } else if (timeNow_G - startTime > 1700) {
      //   speed = 0;
      //   rotSpeed = 160;  //1.4秒間回転，時計回り
      // } else if (timeNow_G - startTime > 3100) {
      //   s_count++;   //探索位置きりかえ
      //   mode_G = 2;  //探索モード2へ
      // }
      break;

      // case 100:  //カップをつかんでいる時に黒をふんだ場合(ライントレースしながらゴールへ向かうor後退して自陣ゴール側に斜め方向にむかう???)-------------------------------------------------------------------

      //   break;

    case 101:  //ほかのロボ検知時，探索中に黒をふんだ時
      //後退→回転
      if (color == BLACK || color == BLUE || color == RED) {
        speed = -170;
        rotSpeed = 0;
        // startTime = timeNow_G;
      } else {  //回転→色かわった（白）検知したとき
        speed = 0;
        rotSpeed = 190;
      }

      if (waitfor(800)) {
        mode_G = 4;
        startTime = timeNow_G;
      }
      break;

    case 102:  //【time取得】スタック検知（他のロボと衝突）,カップホールドモード
      //後退→停止→カップホールドモードに戻る
      speed = -140;
      if (timeNow_G - startTime > 1000) {
        mode_G = 10;
      } else if (timeNow_G - startTime > 500) {
        speed = 0;
      }
      break;

    case 103:  //スタック検知（他のロボと衝突）, ゴールモード11
      //停止→元のモードへ
      speed = 0;
      rotSpeed = 0;
      if (waitfor(700)) {
        mode_G = 11;
      }
      break;

    case 104:  //スタック検知（他のロボと衝突）, ゴールモード12
      //停止→元のモードへ
      speed = 0;
      rotSpeed = 0;
      if (waitfor(700)) {
        mode_G = 12;
      }
      break;
  }


  if (mode_G == 10) {  //探索中に黒を見つけたら
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 3;             //探索（回転）へ
      startTime = timeNow_G;  //スタート時間取得
    }
  } else if (mode_G == 2 || mode_G == 3 || mode_G == 4 || mode_G == 5) {
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 101;  //mode 101　　へ
    }
  }

  if (mode_G == 10) {  //スタック検知
    if (isStack()) {
      mode_G = 102;
      speed = 0;
      rotSpeed = 0;
      startTime = timeNow_G;
    }
  } else if (mode_G == 11) {
    if (isStack()) {
      mode_G = 103;
      speed = 0;
      rotSpeed = 0;
      // startTime = timeNow_G;
    }
  } else if (mode_G == 12) {
    if (isStack()) {
      mode_G = 104;
      speed = 0;
      rotSpeed = 0;
      // startTime = timeNow_G;
    }
  }




  // if () {  //ゴールモード中に黒を見つけたら
  // }

  motorL_G = speed + rotSpeed;
  motorR_G = speed - rotSpeed;
}

void task_Blue() {  //自陣：青の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  static int s_count = 0;  //search_count 探索位置（自陣か真ん中）のきりかえ
  int cup_dist = 0;        //カップとの距離
  static int c_count = 0;  //カップを見失った際の探索の最大回数
  static int m_count = 0;  //↑モータ回転の切り替え

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 14;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = 160;      //時計回り
      if (waitfor(700)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;        //mode 2へ
        // sum_e = 0;   //初期化---(worldturn関数使い終わった時)----------------------------------------
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 200;
      rotSpeed = 0;
      if (s_count % 2 == 0) {  //偶数の場合↓
        if (waitfor(2300)) {   //----------------------2秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;          //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {               //奇数の場合↓
        if (waitfor(700)) {  //----------------------1秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;        //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      }
      break;

    case 3:  //【time取得】，探索モード(回転)
      speed = 0;
      rotSpeed = -150;
      if (timeNow_G - startTime < 2000) {  //の間に↓
        if (search()) {                    //障害物を見つけたら↓
          rotSpeed = 0;                    //停止させる
          mode_G = 5;                      //mode 5へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {
        mode_G = 4;  //1秒すぎたらmode 4
        startTime = timeNow_G;
      }
      break;

    case 4:  //【time取得】,探索モード（直進）
      speed = 200;
      rotSpeed = 0;
      if (timeNow_G - startTime < 750) {  //の間に↓
        if (search()) {                   //障害物を見つけたら↓
          rotSpeed = 0;                   //停止させる
          mode_G = 5;                     //mode 5へ
          startTime = timeNow_G;          //スタート時間取得
        }
      } else {
        mode_G = 3;  //すぎたらmode 3(探索　回転)
        startTime = timeNow_G;
      }
      break;

    case 5:                                //【time取得】，障害物を見つけたら，ロボかカップか判断
      if (timeNow_G - startTime < 1800) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 101;                    //mode 101　　へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
      }
      break;

    case 6:  // カップを見失った場合の探索
      speed = 0;
      // rotSpeed = 150;
      if (m_count % 2 == 0) {  //時計回りか反時計回りに切り替え
        rotSpeed = 150;
      } else {
        rotSpeed = -150;
      }

      if (c_count < 3) {
        if (search()) {           //障害物を見つけたら↓
          rotSpeed = 0;           //停止させる
          mode_G = 7;             //mode 7へ
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {        //カップ探しきったら
        mode_G = 4;   //mode 4
        c_count = 0;  //初期化
        startTime = timeNow_G;
      }
      break;

    case 7:                                //【time取得】, カップを見失った場合のカップかロボかの判断
      if (timeNow_G - startTime < 1800) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 6;                      //カップ探索に戻る
          c_count++;                       //探索回数かぞえる
          m_count++;                       //モータ回転切り替え
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
      }
      break;

    case 10:        //カップホールドモード
      speed = 200;  //直進
      rotSpeed = 0;
      cup_dist = distance();  //カップとの距離計測
      if (cup_dist >= 50) {   //カップつかめなかったら
        speed = 0;
        mode_G = 6;
        startTime = timeNow_G;
      }
      if (Cup_dist()) {  //カップつかんだら
        speed = 0;       //停止
        mode_G = 11;     //mode 11へ
      }
      break;

    case 11:                            //ゴールモード，ゴール方向へ回転
      if (worldTurn(&rotSpeed, 290)) {  //北西（270-0）方向へ回転したら
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:
      speed = 180;  //直進
      rotSpeed = 0;
      if (color == BLACK) {  //黒を見つけたら
        mode_G = 13;
        speed = 0;
      }
      break;

    case 13:  //自陣方向へ回転
      speed = 0;
      rotSpeed = 140;      //時計回り
      if (waitfor(900)) {  //------------------------------------------------------------------
        speed = 190;       //前進
        rotSpeed = 0;
        mode_G = 14;
      }
      break;

    case 14:
      if (color == BLUE) {  //赤を見つけたら
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 99;
        startTime = timeNow_G;  //スタート時間取得
        // Serial.println("d2");
        buzzer.play(">c32");  //一回ブザー音を鳴らす
      }
      break;

    case 99:        //【time取得】，カップをゴールライン外へ出す
      speed = 160;  //0.4秒前進
      rotSpeed = 0;
      if (timeNow_G - startTime > 2700) {
        s_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      } else if (timeNow_G - startTime > 1200) {
        speed = 0;
        rotSpeed = -190;  //1.8秒間回転，反時計回り
      } else if (timeNow_G - startTime > 400) {
        speed = -190;  //カップを外に出したら0.5秒後退
      }
      break;

      // case 100:  //カップをつかんでいる時に黒をふんだ場合(ライントレースしながらゴールへ向かうor後退して自陣ゴール側に斜め方向にむかう???)-------------------------------------------------------------------

      //   break;

    case 101:  //ほかのロボ検知時，探索中に黒をふんだ時
      //後退→回転
      if (color == BLACK || color == BLUE || color == RED) {
        speed = -170;
        rotSpeed = 0;
        // startTime = timeNow_G;
      } else {  //回転→色かわった（白）検知したとき
        speed = 0;
        rotSpeed = 190;
      }

      if (waitfor(800)) {
        mode_G = 4;
        startTime = timeNow_G;
      }
      break;

    case 102:  //【time取得】スタック検知 ,カップホールドモード10
      //後退→停止→カップホールドモードに戻る
      speed = -140;
      if (timeNow_G - startTime > 1000) {
        mode_G = 10;
      } else if (timeNow_G - startTime > 500) {
        speed = 0;
      }
      break;

    case 103:  //スタック検知（他のロボと衝突）, ゴールモード11
      //停止→元のモードへ
      speed = 0;
      rotSpeed = 0;
      if (waitfor(700)) {
        mode_G = 11;
      }
      break;

    case 104:  //スタック検知（他のロボと衝突）, ゴールモード12
      //停止→元のモードへ
      speed = 0;
      rotSpeed = 0;
      if (waitfor(700)) {
        mode_G = 12;
      }
      break;
  }


  if (mode_G == 10) {  //探索中に黒を見つけたら
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 3;             //探索（回転）へ
      startTime = timeNow_G;  //スタート時間取得
    }
  } else if (mode_G == 2 || mode_G == 3 || mode_G == 4 || mode_G == 5) {
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 101;  //mode 101　　へ
    }
  }

  if (mode_G == 10) {  //スタック検知
    if (isStack()) {
      mode_G = 102;
      speed = 0;
      rotSpeed = 0;
      startTime = timeNow_G;
    }
  } else if (mode_G == 11) {
    if (isStack()) {
      mode_G = 103;
      speed = 0;
      rotSpeed = 0;
      // startTime = timeNow_G;
    }
  } else if (mode_G == 12) {
    if (isStack()) {
      mode_G = 104;
      speed = 0;
      rotSpeed = 0;
      // startTime = timeNow_G;
    }
  }




  // if () {  //ゴールモード中に黒を見つけたら
  // }

  motorL_G = speed + rotSpeed;
  motorR_G = speed - rotSpeed;
}


void linetrace_1or6()  //➀or➅Roboの場合 , P制御，反時計回り
{
  static float lightMin = 0;    // 各自で設定
  static float lightMax = 155;  // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 120;     // パラメーター
  static float Kp = 0.3;        // パラメーター
  float lightNow;
  float Threshold = 160;  //閾値
  bool onLine;            //線の上にいるかいないかの判断

  lightNow = (red_G + green_G + blue_G) / 3.0;
  onLine = lightNow < Threshold;  //カラーセンサーの値が閾値以下なら黒線上にあると判定
  if (onLine) {
    rotSpeed = 0;
  } else {
    // rotSpeed = Kp * speed;
    rotSpeed = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);
    // 反時計回り
  }
  // rotSpeed = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);
  // 反時計回り
  motorL_G = speed - rotSpeed;
  motorR_G = speed + rotSpeed;
  // motorL_G = speed - speedDiff;
  // motorR_G = speed + speedDiff;
}

float turnTo(float theta_r) {  //PI制御
  float u;
  float KP = 4.0;
  float TIinv = 2 / 1000.0;
  heading_G = atan2(my_G, mx_G) * 180 / M_PI;
  if (heading_G < 0) heading_G += 360;
  float e = theta_r - heading_G;
  if (e < -180) e += 360;
  if (e > 180) e -= 360;
  if (abs(e) > 45.0) {  // |e|>45のときはP制御
    u = KP * e;         // P制御
  } else {              // |e|<=45 の時はPI制御
    sum_e += TIinv * e * (timeNow_G - timePrev_G);
    u = KP * (e + sum_e);  // PI 制御
  }
  if (u > 180) u = 180;    // 飽和
  if (u < -180) u = -180;  // 飽和
  return u;
}

int waitfor(unsigned long period) {
  static int flagStart = 0;  // 0:待ち状態, 1:現在計測中
  static unsigned long startTime = 0;

  if (flagStart == 0) {
    startTime = timeNow_G;
    flagStart = 1;  // 現在計測中にしておく
  }

  if (timeNow_G - startTime > period) {  // 計測開始からの経過時間が指定時間を超えた
    flagStart = 0;                       // 待ち状態に戻しておく
    startTime = 0;                       // なくてもよいが, 形式的に初期化
    return 1;
  }
  return 0;
}

// int identify_color(int red, int green, int blue) {
//   float d2;
//   float d2_max = 50;  // パラメーター（適宜調整）

//   d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
//   Serial.println(d2);
//   if (d2 < d2_max * d2_max)
//     return 1;
//   else
//     return 0;
// }



int maintainState(unsigned long period) {
  static int flagStart = 0;  // 0:待ち状態，1:現在計測中
  static unsigned long startTime = 0;

  if (flagStart == 0) {
    startTime = timeNow_G;  // 計測を開始したtimeNow_Gの値を覚えておく
    flagStart = 1;          // 現在計測中にしておく
  }

  if (timeNow_G - startTime > period) {  // 計測開始からの経過時間が指定時間を越えた
    flagStart = 0;                       // 待ち状態に戻しておく
    startTime = 0;                       // なくても良いが，形式的に初期化
    return 1;
  } else
    return 0;
}