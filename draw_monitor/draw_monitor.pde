//パッケージ
import processing.serial.*;

//マクロ
final int screen_width = 1200;
final int screen_height = 800;
final int robot_num = 3;

//変数
PFont[] font = new PFont[2];

/////ネットワーク部との連携部分/////
Serial port1; //ポート
boolean sof_f = false; //データ受信確認

int robot_id = 0;

int[] robot_mode = new int[robot_num]; //ロボットの状態
int[] robot_score = new int[robot_num]; //得点
int[] robot_distance = new int[robot_num]; //距離
int[][] robot_color = new int[robot_num][3]; //カラー
int[] robot_direction = new int[robot_num]; //方向
int[] robot_speed = new int[robot_num]; //スピード
//////////////////////////////////

void setup()
{
  //初期設定
  size(1200, 800);
  frameRate(60);
  
  //フォント設定
  font[0] = loadFont("YuGothic-Bold-48.vlw");
  font[1] = loadFont("YuGothic-Medium-48.vlw");
  
  //ポート作成
  port1 = new Serial(this, "COM5", 9600); //ポート設定
  port1.clear();
}

void draw()
{
  InitWindow();
  Window1();
  Window2();
  Window3();
}

void InitWindow()
{
  background(32);
  
  //チーム名
  fill(240);
  textFont(font[0]);
  textSize(64);
  text("team 11", 30, 64);
  
  //総合得点
  fill(227, 37, 94);
  noStroke();
  rect(300, 10, 270, 60, 8);
  
  fill(240);
  textFont(font[1]);
  textSize(40);
  text("総合得点 " + (robot_score[0] + robot_score[1] + robot_score[2]), 310, 55);
}

void serialEvent(Serial p) {
  int l = p.available(); // 受信バッファ内のデータ数
  boolean bod_f = false; // 1組のデータ(block of data)が得られたか？

  while (l>0) { // 受信バッファ内にデータがある場合
          //ポートの識別//
        if(p == port1) robot_id = 0; 
    if (sof_f == false) { // SoFを発見していない場合
      if (p.read() == 'H') { // SoF(Start of Frame)の検査
        sof_f = true; // SoFの発見
      }
      l--; // 受信バッファのデータ数の修正
    }
    if (sof_f == true) { // SoFを発見している場合
      if (l >= 8) { // 受信バッファのデータ数が6以上
        
        robot_mode[robot_id] = p.read();      //mode
        robot_score[robot_id] = p.read();     //score
        robot_distance[robot_id] = p.read();  //distance
        robot_color[robot_id][0] = p.read();  //RGB値
        robot_color[robot_id][1] = p.read();
        robot_color[robot_id][2] = p.read();
        robot_direction[robot_id] = p.read();
        robot_speed[robot_id] = p.read();
        

        bod_f = true; // 1組のデータを読み込んだ
        sof_f = false; // 1組のデータを読み取ったのでSoFの発見をクリア
        l -= 8; // 受信バッファのデータ数の修正
      } else { // 受信バッファのデータ数不足の場合
        break; // whileループを中断
      }
    }
  }
 if (bod_f == true) // 1組のデータを読み込んだので
    {
      port1.write('H');
    }
}

//Arduinoに書き込む通信//
/*
Serial.write('H');
Serial.write(mode_G);         //mode
Serial.write((int)g_count);
Serial.write((int)dist_G);
Serial.write((int)red_G);     //color
Serial.write((int)green_G);
Serial.write((int)blue_G);
Serial.write(heading(mx_G, my_G));
Serial.write(speed);
*/
/////////////////////////
