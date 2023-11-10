//パッケージ
import processing.serial.*;

//マクロ
final int DISPLAY_W = 1200; //ウィンドウサイズ
final int DISPLAY_H = 800;
final int ZUMO_NUM = 1; //接続するzumoの数

//変数
Serial[] port = new Serial[3]; //ポート
int[][] objectPos = new int[3][2]; //オブジェクトの座標
int[] mode = new int[3]; //zumoの状態
int[][] colorSensor = new int[3][3]; //カラーセンサ
float[] distSensor = new float[3]; //超音波センサ
int[][] p_colorSensor = new int[3][3];
boolean sof_f = false;

void setup()
{
  //Processing設定
  size(1200, 800); //ウィンドウサイズ
  port[0] = new Serial(this, "/dev/ttyUSB0", 9600); //ポート設定
  /*port[1] = new Serial(this, "null", 9600);
  port[2] = new Serial(this, "null", 9600);*/
  PFont font = createFont("Meiryo", 50); //フォント設定
  //colorSensor[0][0] = 255;
  //colorSensor[0][1] = 0;
  //colorSensor[0][2] = 0;
  p_colorSensor[0][0] = 255;
  p_colorSensor[0][1] = 0;
  p_colorSensor[0][2] = 0;
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
  /*if (ZUMO_NUM >= 2) //二台目
  {
    fill(150, 150, 150);
    rect(objectPos[1][0], objectPos[1][1], DISPLAY_W / 2, DISPLAY_H / 2);
  }
  if (ZUMO_NUM >= 3) //三台目
  {
    fill(200, 200, 200);
    rect(objectPos[2][0], objectPos[2][1], DISPLAY_W / 2, DISPLAY_H / 2);
  }*/
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
    /*switch (mode[no])
    {
      case 0:*/
      text(" mode = " + mode[no], objectPos[no][0] + 20, objectPos[no][1] +250);
       // break;
    //}
    
    //色
    fill(p_colorSensor[no][0], p_colorSensor[no][1], p_colorSensor[no][2]);
    rect(objectPos[no][0] + 250, objectPos[no][1] + 25, 100, 100);
    
    //超音波センサの距離
    fill(0);
    textSize(30);
    text("距離 : " + distSensor[no], objectPos[no][0] + 10, objectPos[no][1] + 360);

  }
  
  return true;
}

void input()
{
  for (int i = 0; i < 0; i++)
  {
    for (int v = 0; v < 2; v++)
    {
      colorSensor[i][v] = p_colorSensor[i][v];
    }
  }
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
void serialEvent(Serial p) {
  int l = p.available(); // 受信バッファ内のデータ数
  boolean bod_f = false; // 1組のデータ(block of data)が得られたか？
  //boolean sof_f = false;

  while (l>0) { // 受信バッファ内にデータがある場合
    if (sof_f == false) { // SoFを発見していない場合
      if (p.read() == 'H') { // SoF(Start of Frame)の検査
        sof_f = true; // SoFの発見
      }
      l--; // 受信バッファのデータ数の修正
    }
    if (sof_f == true) { // SoFを発見している場合
      if (l >= 4) { // 受信バッファのデータ数が4以上
        
        mode[0] = p.read();
        p_colorSensor[0][0] = p.read();
        p_colorSensor[0][1] = p.read();
        p_colorSensor[0][2] = p.read();
        
       /* if (p_colorSensor[0][0] >= 150 && p_colorSensor[0][1] <= 90 && p_colorSensor[0][2] <= 70)
        {
          p.write("R");
        }
        
        else if (p_colorSensor[0][0] <= 80 && p_colorSensor[0][1] <= 100 && p_colorSensor[0][2] >= 150)
        {
          p.write("B");
        }*/
        print(" RGB = ");
        println(p_colorSensor[0][0], p_colorSensor[0][1], p_colorSensor[0][2]);
        print(" mode = ");
        println(mode[0]);

        bod_f = true; // 1組のデータを読み込んだ
        sof_f = false; // 1組のデータを読み取ったのでSoFの発見をクリア
        l -= 4; // 受信バッファのデータ数の修正
      } else { // 受信バッファのデータ数不足の場合
        break; // whileループを中断
      }
    }
  }
  if (bod_f == true) // 1組のデータを読み込んだので
          if (p_colorSensor[0][0] >= 150 && p_colorSensor[0][1] <= 70 && p_colorSensor[0][2] <= 50)
        {
          p.write("R");
        }
        
        else if (p_colorSensor[0][0] <= 80 && p_colorSensor[0][1] <= 100 && p_colorSensor[0][2] >= 150)
        {
          p.write("B");
        }
    else { p.write("A"); // 次のデータ送信要求を送信
    }
}
