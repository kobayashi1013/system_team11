//ロボの挙動
void linetrace_bang_bang() {
  static float lightMin = 0;    // 各自で設定
  static float lightMax = 250;  // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 100;     // パラメーター
  static float Kp = 3.0;        // パラメーター
  float lightNow;
  float speedDiff;

  lightNow = (red_G + green_G + blue_G) / 3.0;
  if (lightNow < (lightMin + lightMax) / 2.0)  // 右回転
    speedDiff = -Kp * speed;
  else  // 左回転
    speedDiff = Kp * speed;
  motorL_G = speed - speedDiff;
  motorR_G = speed + speedDiff;
}


void linetrace_P()  //P制御，反時計回り
{
  static float lightMin = 0;    // 各自で設定
  static float lightMax = 255;  // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 120;     // パラメーター
  static float Kp = 2.3;        // パラメーター
  float lightNow;
  float speedDiff;

  lightNow = (red_G + green_G + blue_G) / 3.0;
  speedDiff = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);

  motorL_G = speed - speedDiff;
  motorR_G = speed + speedDiff;
}

void linetrace_P_reverce() {    //kadai8-5-3   P制御，時計回り
  static float lightMin = 0;    // 各自で設定
  static float lightMax = 255;  // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 120;     // パラメーター　120ベスト
  static float Kp = 2.3;        // パラメーター 2.3ベスト
  float lightNow;
  float speedDiff;

  lightNow = (red_G + green_G + blue_G) / 3.0;
  speedDiff = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);

  motorR_G = speed - speedDiff;
  motorL_G = speed + speedDiff;
}

void task_A() {                    //kadai8-5-5
  static unsigned long startTime;  // static変数，時間計測ははunsigned long
  // この変数は1秒停止の開始時間を覚えておくために使用

  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      linetrace_P_reverce();                //P制御 時計回り　ライントレース（各自で作成）
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        mode_G = 2;
        startTime = millis();
      }
      break;

    case 2:
      motorL_G = 0;
      motorR_G = 0;
      if (millis() - startTime > 1000) {
        mode_G = 3;
      }
      break;

    case 3:
      linetrace_P_reverce();                //ライントレース
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}

void task_A3() {  //発展課題8-5-1

  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      linetrace_P_reverce();                //P制御 時計回り　ライントレース（各自で作成）
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        mode_G = 2;
      }
      break;

    case 2:
      motorL_G = 0;
      motorR_G = 0;
      if (maintainState(1000)) {  //1秒経過したら
        mode_G = 3;
      }
      break;

    case 3:
      linetrace_P_reverce();                //ライントレース
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}


void task_A2() {                   //kadai8-5-6
  static unsigned long startTime;  // static変数，時間計測ははunsigned long
  static int count = 0;
  // この変数は1秒停止の開始時間を覚えておくために使用

  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      linetrace_P_reverce();                //P制御 時計回り　ライントレース（各自で作成）
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        count++;                            //カウントを増やす
        if (count % 2 == 0) {               //偶数回のみmode 2へ
          mode_G = 2;
          startTime = millis();
        } else {
          mode_G = 3;
        }
      }
      break;

    case 2:
      motorL_G = 0;
      motorR_G = 0;
      if (millis() - startTime > 1000) {
        mode_G = 3;
      }
      break;

    case 3:
      linetrace_P_reverce();                //ライントレース
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}


void task_A_pre() {  //kadai8-5-4
  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      linetrace_P_reverce();                //P制御 時計回り　ライントレース（各自で作成）
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        motorL_G = 0;
        motorR_G = 0;
        mode_G = 2;
      }
      break;

    case 2:
      delay(1000);  //1秒停止
      mode_G = 3;
      break;

    case 3:
      linetrace_P_reverce();                //ライントレース
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}

void task_B() {                    //kadai8-5-7
  static unsigned long startTime;  // static変数，時間計測ははunsigned long
  static int count = 0;
  // この変数は1秒停止の開始時間を覚えておくために使用

  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      if (count % 2 == 0) {
        linetrace_P_reverce();  //P制御 時計回り　
      } else {
        linetrace_P();  //反時計回り
      }
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        mode_G = 2;
        count++;
        startTime = millis();
      }
      break;

    case 2:
      motorL_G = 150;
      motorR_G = -150;
      if (millis() - startTime > 1500) {
        mode_G = 3;
      }
      break;

    case 3:
      if (count % 2 == 0) {
        linetrace_P_reverce();  //P制御 時計回り　
      } else {
        linetrace_P();  //反時計回り
      }
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}

void task_Bsub() {                 //kadai8-5-7   違うパターンで作った
  static unsigned long startTime;  // static変数，時間計測ははunsigned long
  static int count = 0;
  // この変数は1秒停止の開始時間を覚えておくために使用

  switch (mode_G) {
    case 0:
      mode_G = 1;
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      if (count % 2 == 0) {
        linetrace_P_reverce();  //P制御 時計回り　
      } else {
        linetrace_P();  //反時計回り
      }
      if (identify_color(200, 150, 120)) {  // 赤かどうか　（引数の値は各自で設定）
        mode_G = 2;
        count++;
      }
      break;

    case 2:
      motorL_G = 150;
      motorR_G = -150;
      if (identify_color(120, 150, 180)) {  //回転→青色を見つけたら
        mode_G = 3;
      }
      break;

    case 3:
      if (count % 2 == 0) {
        linetrace_P_reverce();  //P制御 時計回り　
      } else {
        linetrace_P();  //反時計回り
      }
      if (identify_color(120, 150, 180)) {  // 青かどうか
        mode_G = 1;                         //青を観測したらmode 1へもどる
      }
      break;
  }
}

void task_Red() {  //自陣：赤の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  static int s_count = 0;  //search_count 探索位置（自陣か真ん中）のきりかえ

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = -160;      //反時計回り
      if (waitfor(1000)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;         //mode 2へ
        // sum_e = 0;   //初期化---(worldturn関数使い終わった時)----------------------------------------
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 150;
      rotSpeed = 0;
      if (s_count % 2 == 0) {  //偶数の場合↓
        if (waitfor(2200)) {   //----------------------2秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;          //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {                //奇数の場合↓
        if (waitfor(1000)) {  //----------------------1秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;         //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      }
      break;

    case 3:  //【time取得】，探索モード(回転)
      speed = 0;
      rotSpeed = 150;
      if (timeNow_G - startTime < 3000) {  //の間に↓
        if (search()) {                    //障害物を見つけたら↓
          rotSpeed = 0;                    //停止させる
          mode_G = 5;                      //mode 5へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {
        mode_G = 4;  //1秒すぎたらmode 4
      }
      break;

    case 4:  //探索モード（直進）
      speed = 150;
      rotSpeed = 0;
      if (waitfor(1000)) {  //1秒間直進して探索位置を変える---------------------------------------------
        mode_G = 3;
        startTime = timeNow_G;  //スタート時間取得
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

    case 10:        //ゴールモード
      speed = 160;  //直進
      rotSpeed = 0;
      if (Cup_dist()) {  //カップつかんだら
        speed = 0;       //停止
        mode_G = 11;     //mode 11へ
      }
      break;

    case 11:                 //ゴール方向へ回転
      if (worldTurn(225)) {  //西南（180-270）方向へ回転したら
        speed = 160;         //直進
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:
      if (identifyColor() == 1) {  //黒を見つけたら
        speed = 120;               //ライントレースのスピード-----------------------------------------------------------------------------------------
        linetrace_1or6();
      }
      if (identifyColor() == 2) {  //赤(ゴールライン)を見つけたら
        speed = 0;
        rotSpeed = 0;
        mode_G = 13;
        startTime = timeNow_G;  //スタート時間取得
      }
      break;

    case 13:  //【time取得】，カップをゴールライン外へ出す
      speed = 0;
      rotSpeed = 160;                      //時計回り
      if (timeNow_G - startTime > 1000) {  //秒後
        speed = 160;                       //0.7秒前進
        rotSpeed = 0;
      } else if (timeNow_G - startTime > 1700) {
        speed = -160;  //カップを外に出したら1秒後退
      } else if (timeNow_G - startTime > 2700) {
        speed = 0;
        rotSpeed = 160;  //2秒間回転，時計回り
      } else if (timeNow_G - startTime > 4700) {
        s_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      }
      break;

      // case 100:  //カップをつかんでいる時に黒をふんだ場合(ライントレースしながらゴールへ向かうor後退して自陣ゴール側に斜め方向にむかう???)-------------------------------------------------------------------

      //   break;

    case 101:        //【time取得】，後退して回転（ほかのロボ検知時，探索中に黒をふんだ時）
      speed = -150;  //後退
      rotSpeed = 0;
      if (timeNow_G - startTime > 800) {  //秒後退したら
        speed = 0;
        rotSpeed = 160;
        if (timeNow_G - startTime > 1800) {  //1秒間回転後，探索モードへ
          mode_G = 4;                        //探索（直進）
        }
      }
      break;
  }


  if (mode_G == 3 || mode_G == 4 || mode_G == 5) {  //探索中に黒を見つけたら
    if (identifyColor() == 1) {
      mode_G = 101;           //mode 101　　へ
      startTime = timeNow_G;  //スタート時間取得
    }
  }

  // if () {  //ゴールモード中に黒を見つけたら
  // }

  motorL_G = speed + rotSpeed;
  motorR_G = speed - rotSpeed;
}

void task_Blue() {  //➅Robo，自陣：青の場合の挙動
  static unsigned long startTime;
  float speed, diff, diff_R, diff_L;
  static int s_count = 0;  //search_count 探索位置（自陣か真ん中）のきりかえ

  switch (mode_G) {
    case 0:
      mode_G = 1;
      // mode_G = 3;
      break;

    case 1:  //まず敵陣の方へ方向転換
      speed = 0;
      rotSpeed = -160;      //反時計回り
      if (waitfor(1000)) {  //---------------------------------------------------------- -----------------------
        mode_G = 2;         //mode 2へ
        // sum_e = 0;   //初期化---(worldturn関数使い終わった時)----------------------------------------
      }
      break;

    case 2:  //自陣 or 敵陣の真ん中あたりまで前進
      speed = 150;
      rotSpeed = 0;
      if (s_count % 2 == 0) {  //偶数の場合↓
        if (waitfor(2200)) {   //----------------------2秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;          //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      } else {                //奇数の場合↓
        if (waitfor(1000)) {  //----------------------1秒間たったら（秒数は後で調整）--------------------------------------------------------------------------------------
          mode_G = 3;         //mode 3へ
          speed = 0;
          startTime = timeNow_G;  //スタート時間取得
        }
      }
      break;

    case 3:  //【time取得】，探索モード(回転)
      speed = 0;
      rotSpeed = 150;
      if (timeNow_G - startTime < 3000) {  //の間に↓
        if (search()) {                    //障害物を見つけたら↓
          rotSpeed = 0;                    //停止させる
          mode_G = 5;                      //mode 5へ
          startTime = timeNow_G;           //スタート時間取得
        }
      } else {
        mode_G = 4;  //1秒すぎたらmode 4
      }
      break;

    case 4:  //探索モード（直進）
      speed = 150;
      rotSpeed = 0;
      if (waitfor(1000)) {  //1秒間直進して探索位置を変える---------------------------------------------
        mode_G = 3;
        startTime = timeNow_G;  //スタート時間取得
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

    case 10:        //ゴールモード
      speed = 160;  //直進
      rotSpeed = 0;
      if (Cup_dist()) {  //カップつかんだら
        speed = 0;       //停止
        mode_G = 11;     //mode 11へ
      }
      break;

    case 11:                //ゴール方向へ回転
      if (worldTurn(45)) {  //北東（0-90）方向へ回転したら
        speed = 160;        //直進
        rotSpeed = 0;
        mode_G = 12;
      }
      break;

    case 12:
      if (identifyColor() == 1) {  //黒を見つけたら
        speed = 120;               //ライントレースのスピード-----------------------------------------------------------------------------------------
        linetrace_1or6();
      }
      if (identifyColor() == 3) {  //青(ゴールライン)を見つけたら
        speed = 0;
        rotSpeed = 0;
        mode_G = 13;
        startTime = timeNow_G;  //スタート時間取得
      }
      break;

    case 13:  //【time取得】，カップをゴールライン外へ出す
      speed = 0;
      rotSpeed = 160;                      //時計回り
      if (timeNow_G - startTime > 1000) {  //秒後
        speed = 160;                       //0.7秒前進
        rotSpeed = 0;
      } else if (timeNow_G - startTime > 1700) {
        speed = -160;  //カップを外に出したら1秒後退
      } else if (timeNow_G - startTime > 2700) {
        speed = 0;
        rotSpeed = 160;  //2秒間回転，時計回り
      } else if (timeNow_G - startTime > 4700) {
        s_count++;   //探索位置きりかえ
        mode_G = 2;  //探索モード2へ
      }
      break;

      // case 100:  //カップをつかんでいる時に黒をふんだ場合(ライントレースしながらゴールへ向かうor後退して自陣ゴール側に斜め方向にむかう???)-------------------------------------------------------------------

      //   break;

    case 101:        //【time取得】，後退して回転（ほかのロボ検知時，探索中に黒をふんだ時）
      speed = -150;  //後退
      rotSpeed = 0;
      if (timeNow_G - startTime > 800) {  //秒後退したら
        speed = 0;
        rotSpeed = 160;
        if (timeNow_G - startTime > 1800) {  //1秒間回転後，探索モードへ
          mode_G = 4;                        //探索（直進）
        }
      }
      break;
  }


  if (mode_G == 3 || mode_G == 4 || mode_G == 5) {  //探索中に黒を見つけたら
    if (identifyColor() == 1) {
      mode_G = 101;           //mode 101　　へ
      startTime = timeNow_G;  //スタート時間取得
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
  static float lightMax = 255;  // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 120;     // パラメーター
  static float Kp = 2.3;        // パラメーター
  float lightNow;
  // float speedDiff;

  lightNow = (red_G + green_G + blue_G) / 3.0;
  rotSpeed = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);

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

int identify_color(int red, int green, int blue) {
  float d2;
  float d2_max = 50;  // パラメーター（適宜調整）

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if (d2 < d2_max * d2_max)
    return 1;
  else
    return 0;
}

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