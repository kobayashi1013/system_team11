//パッケージ
import processing.serial.*;

//マクロ
final int DISPLAY_W = 1200; //ウィンドウサイズ
final int DISPLAY_H = 800;
final int ZUMO_NUM = 3; //接続するzumoの数

//変数
Serial port1;
Serial port2;
//Serial port3;//ポート
int zumo_id = 0;  //zu-moID
int[][] objectPos = new int[3][2]; //オブジェクトの座標
int[] mode = new int[3]; //zumoの状態
int[][] colorSensor = new int[3][3]; //カラーセンサ
float[] distSensor = new float[3]; //超音波センサ
int[][] p_colorSensor = new int[3][3];
int[][] motor = new int[3][2]; // モーター（[][0] = R, [][1] = L）
int[][] compass = new int[3][2];   //地磁気 ([][0] = mx, [][1] = my)
boolean sof_f = false;

void setup()
{
  //Processing設定
  size(1200, 800); //ウィンドウサイズ
  port1 = new Serial(this, "/dev/ttyUSB0", 9600); //ポート設定
  port1.clear();
  //port1.bufferUntil(0x0d);
  port2 = new Serial(this, "/dev/ttyUSB1", 9600); //ポート設定
  port2.clear();
  //port3 = new Serial(this, "/dev/ttyUSB2", 9600); //ポート設定
  //port3.clear();
  PFont font = createFont("Meiryo", 50); //フォント設定
  //test input//
  p_colorSensor[0][0] = 255;
  p_colorSensor[0][1] = 0;
  p_colorSensor[0][2] = 0;
  motor[0][0] = 100;
  motor[0][1] = -100;
  //         //
  textFont(font);
  
  //初期設定
  SetupPos(0, 0, 0);
  SetupPos(1, DISPLAY_W / 2, 0);
  SetupPos(2, 0, DISPLAY_H / 2);
}

//各ウィンドウの座標を決定
boolean SetupPos(int no, int x, int y)
{
  if (no >= 3 || no <= -1)
  {
    println("warning : SetupPos()");
    return false;
  }
  else
  {
    objectPos[no][0] = x;
    objectPos[no][1] = y;
  }
  
  return true;
}

//各ウィンドウの更新
void ClearWindow()
{
  background(0); //背景色
  
  if (ZUMO_NUM >= 1) //一台目
  {
    fill(255, 255, 255);
    rect(objectPos[0][0], objectPos[0][1], DISPLAY_W / 2, DISPLAY_H / 2);
  }
  if (ZUMO_NUM >= 2) //二台目
  {
    fill(150, 255, 255);
    rect(objectPos[1][0], objectPos[1][1], DISPLAY_W / 2, DISPLAY_H / 2);
  }
  if (ZUMO_NUM >= 3) //三台目
  {
    fill(200, 200, 200);
    rect(objectPos[2][0], objectPos[2][1], DISPLAY_W / 2, DISPLAY_H / 2);
  }
}

//各ウィンドウの描画
boolean DrawObject1(int no)
{
  if (no >= 3 || no <= -1)
  {
    println("warning : SetupObject1()");
    return false;
  }
  else
  {
    //ポート番号
    fill(0);
    textSize(30);
    text("USB" + no, objectPos[no][0] + 10, objectPos[no][1] + 40);
    
    //状態
    fill(0);
    textSize(35);
      text(" mode = " + mode[no], objectPos[no][0] + 220, objectPos[no][1] + DISPLAY_H / 4);
      
    //モーター
    fill(0);
    textSize(30);
    text("motorR =" + motor[no][0], objectPos[no][0] + 50, objectPos[no][1] +260);
    text("motorL =" + motor[no][1], objectPos[no][0] + 50, objectPos[no][1] +300);
    
    //色
    fill(p_colorSensor[no][0], p_colorSensor[no][1], p_colorSensor[no][2]);
    rect(objectPos[no][0] + 250, objectPos[no][1] + 25, 100, 100);
    
    //地磁気
    fill(0);
    textSize(30);
    text("mx = " +compass[no][0], objectPos[no][0] +300, objectPos[no][1] +260);
    text("my = " +compass[no][1], objectPos[no][0] +300, objectPos[no][1] +300);

    
    //超音波センサの距離
    fill(0);
    textSize(30);
    text("距離 : " + distSensor[no], objectPos[no][0] + 10, objectPos[no][1] + 360);

  }
  
  return true;
}

void draw()
{
  //input();
  ClearWindow();
  
  for (int i = 0; i < ZUMO_NUM; i++)
  {
    DrawObject1(i);
  }

}


void keyPressed() { // keyが押されると呼ばれる関数
  if (key == 's') {
    noLoop(); // draw()の繰返し処理を停止
    println("stop");
  } else if (key == 'c') {
    background(255); // draw()の繰返し処理を再開
    loop();
  }
}

// 通信方式2
//USB0
void serialEvent(Serial p) {
  int l = p.available(); // 受信バッファ内のデータ数
  boolean bod_f = false; // 1組のデータ(block of data)が得られたか？

  while (l>0) { // 受信バッファ内にデータがある場合
          //ポートの識別//
        if(p == port1) zumo_id = 0; 
        else if(p == port2) zumo_id = 1;
    if (sof_f == false) { // SoFを発見していない場合
      if (p.read() == 'H') { // SoF(Start of Frame)の検査
        sof_f = true; // SoFの発見
      }
      l--; // 受信バッファのデータ数の修正
    }
    if (sof_f == true) { // SoFを発見している場合
      if (l >= 6) { // 受信バッファのデータ数が6以上
        
        mode[zumo_id] = p.read();              //mode
        p_colorSensor[zumo_id][0] = p.read();  //RGB値
        p_colorSensor[zumo_id][1] = p.read();
        p_colorSensor[zumo_id][2] = p.read();
        motor[zumo_id][0] = p.read();          //モーター値
        motor[zumo_id][1] = p.read();
        
        //確認用表示//
        println("p = " +p);
        print(" RGB = ");
        println(p_colorSensor[zumo_id][0], p_colorSensor[zumo_id][1], p_colorSensor[zumo_id][2]);
        print(" mode = ");
        println(mode[zumo_id]);
        print("motorR =" +motor[zumo_id][0]);
        println("motorL =", +motor[zumo_id][1]);

        bod_f = true; // 1組のデータを読み込んだ
        sof_f = false; // 1組のデータを読み取ったのでSoFの発見をクリア
        l -= 6; // 受信バッファのデータ数の修正
      } else { // 受信バッファのデータ数不足の場合
        break; // whileループを中断
      }
    }
  }
 if (bod_f == true) // 1組のデータを読み込んだので
         /* if (p_colorSensor[0][0] >= 150 && p_colorSensor[0][1] <= 70 && p_colorSensor[0][2] <= 50)
        {
          p.write("R");
        }
        
        else if (p_colorSensor[0][0] <= 80 && p_colorSensor[0][1] <= 100 && p_colorSensor[0][2] >= 150)
        {
          p.write("B");
        }
    else */{ p.write("A"); // 次のデータ送信要求を送信
    }
}
