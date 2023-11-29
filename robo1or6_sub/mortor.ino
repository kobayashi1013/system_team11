//モーター制御
//ブザー：探索時にロボだった時，カップをつかんだ時，ゴールラインを見つけた時
void task_Red() {  //自陣：赤の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  // static int g_count = 0;      //goal_count 探索位置（自陣か真ん中）のきりかえ
  // int cup_dist = 0;  //カップとの距離
  // static int before_dist = 0;  //1個前の距離
  int diff_dist = 0;
  static int c_count = 0;  //カップを見失った際の探索の最大回数
  // static int m_count = 0;  //↑モータ回転の切り替え

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 14;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = -160;     //反時計回り
      if (waitfor(400)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;        //mode 2へ
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 200;
      rotSpeed = 0;
      if (g_count % 2 == 0) {  //偶数の場合↓
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
      rotSpeed = 150;
      if (timeNow_G - startTime < 2300) {  //の間に↓
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
      if (timeNow_G - startTime > 700) {
        mode_G = 3;
      }
      break;

    case 5:                                //【time取得】，障害物を見つけたら，ロボかカップか判断
      if (timeNow_G - startTime < 1500) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 101;                    //mode 101　　へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
        startTime = timeNow_G;
      }
      break;

    case 6:  //【time取得】 ちょっと回転してからまた探索？？？カップを見失った場合の探索
      if (timeNow_G - startTime < 500) {
        speed = 0;
        rotSpeed = 180;
      } else {  //0.5秒たったら↓
        speed = 0;
        rotSpeed = 150;
        if (c_count < 2) {
          if (search()) {           //障害物を見つけたら↓
            rotSpeed = 0;           //停止させる
            mode_G = 7;             //mode 7へ
            startTime = timeNow_G;  //スタート時間取得
            // c_count = 0;
          }
        } else {        //カップ探しきったら
          mode_G = 4;   //mode 4
          c_count = 0;  //初期化
          startTime = timeNow_G;
        }
      }
      break;

    case 7:                                //【time取得】, カップを見失った場合のカップかロボかの判断
      if (timeNow_G - startTime < 1500) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 6;                      //カップ探索に戻る
          startTime = timeNow_G;
          c_count++;  //探索回数かぞえる
          // m_count++;                       //モータ回転切り替え
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        c_count = 0;
        mode_G = 10;  //mode 10 (ゴールモードへ)
        startTime = timeNow_G;
      }
      break;

    case 10:        //【time取得】カップホールドモード
      speed = 200;  //直進
      rotSpeed = 0;
      diff_dist = dist_G - before_dist;  //カップとの距離計測
      // Serial.println(diff_dist);
      // if (diff_dist >= 20 || diff_dist <= -20) {  //カップつかめなかったら
      //   speed = 0;
      //   mode_G = 3;
      //   startTime = timeNow_G;
      // }
      if (timeNow_G - startTime > 2000) {  //カップつかめずに時間過ぎたら↓------------------------------------------------------------------------
        mode_G = 6;
        startTime = timeNow_G;
      } else {
        if (Cup_dist()) {  //カップつかんだら
          speed = 0;       //停止
          mode_G = 11;     //mode 11へ
        }
      }
      break;

    case 11:                            //ゴールモード，ゴール方向へ回転
      if (worldTurn(&rotSpeed, 190)) {  //南（180）方向へ回転したら
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:  //ゴールモード, ゴールラインまで直進
      speed = 200;
      if (color == RED) {  //赤を見つけたら
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 99;   //ゴールモード
        startTime = timeNow_G;
        // Serial.println("d2");
        buzzer.play(">c32");        //一回ブザー音を鳴らす
      } else if (color == BLACK) {  //黒を見つけたら
        mode_G = 13;
        speed = 0;
        rotSpeed = 0;  //停止
        startTime = timeNow_G;
      }
      break;

    case 13:  //【time取得】ゴールモード，端の黒をふんでしまったら
      //回転→0.8秒たつまで赤を見つけてから(赤ライン上を)前進
      if (color == RED) {
        rotSpeed = 0;
        speed = 180;
        if (timeNow_G - startTime > 800) {  //---------------------------------------------------------------------------
          mode_G = 14;
          speed = 0;
        }
      } else {
        rotSpeed = 150;  //時計回り
        speed = 0;
        if (timeNow_G - startTime > 4000) {  //青見つからずに時間だいぶたったら-----------------------------------------------------------
          mode_G = 101;
        }
      }
      break;

    case 14:                            //ゴールモード，北方向に回転
      if (worldTurn(&rotSpeed, 190)) {  //南（180）方向へ回転したら
        rotSpeed = 0;
        mode_G = 99;
        startTime = timeNow_G;
      }
      break;

    case 99:        //【time取得】，カップをゴールライン外へ出す
      speed = 160;  //0.4秒前進
      rotSpeed = 0;
      if (timeNow_G - startTime > 3000) {
        g_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      } else if (timeNow_G - startTime > 1200) {
        speed = 0;
        rotSpeed = 190;  //1.8秒間回転，時計回り
      } else if (timeNow_G - startTime > 400) {
        speed = -190;  //カップを外に出したら0.8秒後退
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

    case 102:  //【time取得】スタック検知（他のロボと衝突）,カップホールドモード, ゴールモード12, ゴールモード11
      //後退→回転→カップホールドモードに戻る
      speed = -180;
      if (timeNow_G - startTime > 1500) {
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 6;    //ここをスタック検知（下のif文内のモード）に設定すると繰り返してしまう？？---------------------------------------------
        startTime = timeNow_G;
      } else if (timeNow_G - startTime > 1000) {
        rotSpeed = 170;
        speed = 0;
      }
      break;
  }


  if (mode_G == 2 || mode_G == 3 || mode_G == 4 || mode_G == 5 || mode_G == 10 || mode_G == 102) {  //黒を見つけたら
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 101;  //mode 101　　へ
    }
  }

  if (mode_G == 2 || mode_G == 4 || mode_G == 10 || mode_G == 11 || mode_G == 12) {  //スタック検知
    if (isStack()) {
      mode_G = 102;
      speed = 0;
      rotSpeed = 0;
      startTime = timeNow_G;
    }
  }

  motorL_G = speed + rotSpeed;
  motorR_G = speed - rotSpeed;
}

void task_Blue() {  //自陣：青の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  // static int g_count = 0;      //goal_count 探索位置（自陣か真ん中）のきりかえ
  // int cup_dist = 0;  //カップとの距離
  // static int before_dist = 0;  //1個前の距離
  int diff_dist = 0;
  static int c_count = 0;  //カップを見失った際の探索の最大回数
  // static int m_count = 0;  //↑モータ回転の切り替え

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 14;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = -160;     //反時計回り
      if (waitfor(400)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;        //mode 2へ
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 200;
      rotSpeed = 0;
      if (g_count % 2 == 0) {  //偶数の場合↓
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
      rotSpeed = 150;
      if (timeNow_G - startTime < 2300) {  //の間に↓
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
      if (timeNow_G - startTime > 700) {
        mode_G = 3;
      }
      break;

    case 5:                                //【time取得】，障害物を見つけたら，ロボかカップか判断
      if (timeNow_G - startTime < 1500) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 101;                    //mode 101　　へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        mode_G = 10;     //mode 10 (ゴールモードへ)
        startTime = timeNow_G;
      }
      break;

    case 6:  //【time取得】 ちょっと回転してからまた探索？？？カップを見失った場合の探索
      if (timeNow_G - startTime < 500) {
        speed = 0;
        // rotSpeed = 150;
        rotSpeed = 180;
      } else {  //0.5秒たったら↓
        speed = 0;
        rotSpeed = 150;
        if (c_count < 2) {
          if (search()) {           //障害物を見つけたら↓
            rotSpeed = 0;           //停止させる
            mode_G = 7;             //mode 7へ
            startTime = timeNow_G;  //スタート時間取得
            // c_count = 0;
          }
        } else {        //カップ探しきったら
          mode_G = 4;   //mode 4
          c_count = 0;  //初期化
          startTime = timeNow_G;
        }
      }
      break;

    case 7:                                //【time取得】, カップを見失った場合のカップかロボかの判断
      if (timeNow_G - startTime < 1500) {  //の間に↓(秒数後で調整)--------------------------------------------------------------
        if (judge_RorC()) {                //ロボだったら
          buzzer.play(">c32");             //一回ブザー音を鳴らす
          mode_G = 6;                      //カップ探索に戻る
          startTime = timeNow_G;
          c_count++;  //探索回数かぞえる
          // m_count++;                       //モータ回転切り替え
        }
      } else {           //秒数を過ぎたら（カップだった）
        count_not0 = 0;  //judge_RorC関数内のカウント初期化
        c_count = 0;
        mode_G = 10;  //mode 10 (ゴールモードへ)
        startTime = timeNow_G;
      }
      break;

    case 10:        //【time取得】カップホールドモード
      speed = 200;  //直進
      rotSpeed = 0;
      diff_dist = dist_G - before_dist;  //カップとの距離計測
      // Serial.println(diff_dist);
      // if (diff_dist >= 20 || diff_dist <= -20) {  //カップつかめなかったら
      //   speed = 0;
      //   mode_G = 3;
      //   startTime = timeNow_G;
      // }
      if (timeNow_G - startTime > 2000) {  //カップつかめずに時間過ぎたら↓------------------------------------------------------------------------
        mode_G = 6;
        startTime = timeNow_G;
      } else {
        if (Cup_dist()) {  //カップつかんだら
          speed = 0;       //停止
          mode_G = 11;     //mode 11へ
        }
      }
      break;

    case 11:                           //ゴールモード，ゴール方向へ回転
      if (worldTurn(&rotSpeed, 27)) {  //北（0）方向へ回転したら
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:  //ゴールモード, ゴールラインまで直進
      speed = 200;
      if (color == BLUE) {  //青を見つけたら
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 99;   //ゴールモード
        startTime = timeNow_G;
        // Serial.println("d2");
        buzzer.play(">c32");        //一回ブザー音を鳴らす
      } else if (color == BLACK) {  //黒を見つけたら
        mode_G = 13;
        speed = 0;
        rotSpeed = 0;  //停止
        startTime = timeNow_G;
      }
      break;

    case 13:  //【time取得】ゴールモード，端の黒をふんでしまったら
      //回転→0.8秒たつまで青を見つけてから(青ライン上を)前進
      if (color == BLUE) {
        rotSpeed = 0;
        speed = 180;
        if (timeNow_G - startTime > 800) {  //---------------------------------------------------------------------------
          mode_G = 14;
          speed = 0;
        }
      } else {
        rotSpeed = 150;  //時計回り
        speed = 0;
        if (timeNow_G - startTime > 4000) {  //青見つからずに時間だいぶたったら-----------------------------------------------------------
          mode_G = 101;
        }
      }
      break;

    case 14:                           //ゴールモード，北方向に回転
      if (worldTurn(&rotSpeed, 27)) {  //北（0）方向へ回転したら
        rotSpeed = 0;
        mode_G = 99;
        startTime = timeNow_G;
      }
      break;

    case 99:        //【time取得】，カップをゴールライン外へ出す
      speed = 160;  //0.4秒前進
      rotSpeed = 0;
      if (timeNow_G - startTime > 3000) {
        g_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      } else if (timeNow_G - startTime > 1200) {
        speed = 0;
        rotSpeed = 190;  //1.8秒間回転，時計回り
      } else if (timeNow_G - startTime > 400) {
        speed = -190;  //カップを外に出したら0.8秒後退
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

    case 102:  //【time取得】スタック検知（他のロボと衝突）,カップホールドモード, ゴールモード12, ゴールモード11
      //後退→回転→カップホールドモードに戻る
      speed = -180;
      if (timeNow_G - startTime > 1500) {
        speed = 0;
        rotSpeed = 0;  //停止
        mode_G = 6;    //ここをスタック検知（下のif文内のモード）に設定すると繰り返してしまう？？---------------------------------------------
        startTime = timeNow_G;
      } else if (timeNow_G - startTime > 1000) {
        rotSpeed = 170;
        speed = 0;
      }
      break;
  }


  if (mode_G == 2 || mode_G == 3 || mode_G == 4 || mode_G == 5 || mode_G == 10 || mode_G == 102) {  //黒を見つけたら
    if (color == RED || color == BLUE || color == BLACK) {
      mode_G = 101;  //mode 101　　へ
    }
  }

  if (mode_G == 2 || mode_G == 4 || mode_G == 10 || mode_G == 11 || mode_G == 12) {  //スタック検知
    if (isStack()) {
      mode_G = 102;
      speed = 0;
      rotSpeed = 0;
      startTime = timeNow_G;
    }
  }

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