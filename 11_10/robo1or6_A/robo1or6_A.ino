//ロボ➀or➅
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>
#include <ZumoBuzzer.h>

ZumoBuzzer buzzer;
LSM303 compass;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
//超音波センサ
const int trig = 2;  //TrigピンをArduinoの2番ピンに
const int echo = 4;  //EchoピンをArduinoの4番ピンに

//カラーセンサ
#define colorNum 4
unsigned int colorPos[colorNum][3] = {
  { 255, 255, 255 },  //白:0
  { 12, 25, 28 },     //黒:1
  { 121, 2, 4 },      //赤:2
  { 1, 32, 96 },      //青:3
};

//グローバル変数
float red_G, green_G, blue_G;                     // カラーセンサで読み取ったRGB値（0-255）
static int mode_G;                                // タスクのモード
unsigned long timeInit_G, timeNow_G, timePrev_G;  //  スタート時間，経過時間
int motorR_G, motorL_G;                           // 左右のZumoのモータに与える回転力
float mx_G = 0, my_G = 0, mz_G = 0;               //地磁気センサで読み取った値
float ax_G = 0, ay_G = 0, az_G = 0;
float heading_G = 0;
int timeTes = 0;
float i;


float sum_e = 0;     //turnTo関数で使用
float rotSpeed = 0;  //worldTurn関数で使用
int count_not0 = 0;  //judge_RorC関数で使用，前の距離の差が０じゃない回数

//ループ内で使用
int first = 0;  //最初のみ，初期位置の角度を取得する用
int color = 0;  //自陣の色　赤：１，青：２
float kakudo = 0;
float kakudo180 = 0;  //初期位置と，反対方向の角度


void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupCompass();  //地磁気センサ
  //ボタン→カラーセンサ→ボタン→ループに入る
  button.waitForButton();    // Zumo buttonが押されるまで待機
  CalibrationColorSensor();  // カラーセンサーのキャリブレーション
  //button.waitForButton();

  timeInit_G = millis();
  timePrev_G = 0;
  mode_G = 0;
  motorR_G = 0;
  motorL_G = 0;
}

void loop() {
  // getRGB(red_G, green_G, blue_G);  // カラーセンサでRGB値を取得(0-255)
  getColorSensor(&red_G, &green_G, &blue_G);  // カラーセンサでRGB値を取得(0-255)
  getCompass();                               //地磁気センサでmx_G, my_Gを取得

  timeNow_G = millis() - timeInit_G;     // 経過時間
  motors.setSpeeds(motorL_G, motorR_G);  // 左右モーターへの回転力入力
  sendData();                            // データ送信

  if (timeNow_G - timePrev_G < 100) {
    return;
  }
  i += 1;
  if (i >= 100) {
    i = 0;
  }
  // if (first == 0) {
  //   kakudo = heading(mx_G, my_G);
  //   // Serial.println(kakudo);
  //   if (60 <= kakudo && kakudo <= 120) {          //ロボが東(90)を向いていたら（printfで角度確認する？？）
  //     color = 1;                                  //自陣は赤色
  //   } else if (240 <= kakudo && kakudo <= 300) {  //ロボが西(270)を向いていたら
  //     color = 2;                                  //自陣は青色
  //   }

  //   first = 1;  //適当な数字を代入
  // }

  // if (color == 1) {  //自陣：赤
  // task_Red();
  // } else if (color == 2) {  //自陣：青
  //   //task_Blue();
  // }


  //Serial.println(mode_G);
  //task_Red();
  // task_Blue();

  Tes();
  timePrev_G = timeNow_G;
}

void Tes(){
  if (timeNow_G - timeTes > 1000){
    if (mode_G = 10) {
      mode_G = 0;
    }
    mode_G += 1;
    timeTes = timeNow_G;
  }
}

// 通信方式2
void sendData() {
  static unsigned long timePrev = 0;
  static boolean flag_start = true;  // 最初の通信かどうか
  int inByte;

  // if文の条件： 最初の通信である || 最後のデータ送信から500ms経過 || (データ送信要求が来ている && 最後のデータ送信から50ms経過)
  if (flag_start == true || timeNow_G - timePrev > 500 || (Serial.available() > 0 && timeNow_G - timePrev > 50)) {
    flag_start = false;
    while (Serial.available() > 0) {  // 送信要求が複数来ていた場合は全て読み込む
      inByte = Serial.read();
    }
    ///チームの色識別
      if (inByte == 'R')
    {
      motors.setSpeeds(90, 90);
    }

    if (inByte == 'B')
    {
      motors.setSpeeds(-90, -90);
    }
    //---------------何を送信するか後で！！！！---------------------------------------------------------

    Serial.write('H');
    Serial.write(mode_G);
    Serial.write((int)red_G);
    Serial.write((int)green_G);
    Serial.write((int)blue_G);
    //Serial.write((int)motors); 

    timePrev = timeNow_G;
  }
}
