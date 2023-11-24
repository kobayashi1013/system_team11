//マクロ
final int robot_max = 3;
final float screen_width = 1200;
final float screen_height = 800;
float[][] screen_pos1 = new float[robot_max][2];

//変数
int[] robot_mode = new int[robot_max]; //ロボットの状態
int[] robot_distance = new int[robot_max]; //距離
int[][] robot_color = new int[robot_max][3]; //カラー
int[] robot_direction = new int[robot_max]; //方向
int[] robot_speed = new int[robot_max]; //スピード
int button_no = 0;

void setup()
{
  //初期設定
  size(1200, 800);
  frameRate(60);
  PFont font = createFont("Meiryo", 50);
  textFont(font);
  
  //データ設定
  screen_pos1[0][0] = screen_width / 2; //USB0
  screen_pos1[0][1] = screen_height / 3;
  screen_pos1[1][0] = screen_width * 3 / 4; //USB1
  screen_pos1[1][1] = screen_height / 3;
  screen_pos1[2][0] = screen_width / 2; //USB2
  screen_pos1[2][1] = screen_height * 2 / 3;
  
  robot_color[0][0] = 250;
  robot_color[0][1] = 100;
  robot_color[0][2] = 0;
}

void draw()
{
  InitScreen();
  
  Window1();
  Window2();
  Window3();
  Window4();
  
  if (button_no == 1) ColorWindow();
}

//スクリーンの初期化
void InitScreen()
{
  background(255);
  stroke(0);
}

//ロボットの基本情報
void Window1()
{
  for (int i = 0; i < robot_max; i++)
  {
    //ウィンドウ
    fill(255);
    rect(0, (screen_height / robot_max) * i, screen_width / 4, screen_height / robot_max);
    
    //名前
    fill(0, 255, 0);
    rect(0, (screen_height / robot_max) * i, 85, 40);
    
    fill(0);
    textSize(24);
    text("USB" + i, 10, (screen_height / robot_max) * i + 30);
    
    //状態
    switch (robot_mode[i])
    {
      case 0:
        fill(0);
        textSize(60);
        text("テスト中", 20, (screen_height / robot_max) * i + 150);
        break;
    }
  }
}

//ロボットのセンサ情報
void Window2()
{
  for (int i = 0; i < robot_max; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      //ウィンドウ
      fill(255);
      rect(screen_width / 4, (screen_height / (robot_max * 4)) * (4 * i + j), screen_width / 4, screen_height / (robot_max * 4));
    }
    
    //距離
    fill(0);
    textSize(32);
    text("distance : " + robot_distance[i], screen_width / 4 + 10, (screen_height / (robot_max * 4)) * (4 * i + 0) + 45);
    
    //カラー
    fill(0);
    textSize(32);
    text("color", screen_width / 4 + 10, (screen_height / (robot_max * 4)) * (4 * i + 1) + 45);
    
    fill(robot_color[i][0], robot_color[i][1], robot_color[i][2]);
    rect(screen_width * 3 / 8, (screen_height / (robot_max * 4)) * (4 * i + 1), screen_width / 8, screen_height / (robot_max * 4));
    
    //方向
    fill(0);
    textSize(32);
    text("direction : " + robot_direction[i], screen_width / 4 + 10, (screen_height / (robot_max * 4)) * (4 * i + 2) + 45);
    
    //スピード
    fill(0);
    textSize(32);
    text("speed : " + robot_direction[i], screen_width / 4 + 10, (screen_height / (robot_max * 4)) * (4 * i + 3) + 45);
  }
}

//テキスト
void Window3()
{
  //ウィンドウ
  fill(255);
  rect(screen_width / 2, 0, screen_width / 2, screen_height / 6);
}

//ユーザボタン
void Window4()
{
  //ウィンドウ
  fill(255);
  rect(screen_width / 2, screen_height / 6, screen_width / 4, screen_height / 12);
  rect(screen_width * 3 / 4, screen_height / 6, screen_width / 4, screen_height / 12);
  rect(screen_width / 2, screen_height / 4, screen_width / 4, screen_height / 12);
  rect(screen_width * 3 / 4, screen_height / 4, screen_width / 4, screen_height / 12);
  
  //選択ボタン
  fill(192, 192, 192);
  if (button_no == 0) rect(screen_width / 2, screen_height / 6, screen_width / 4, screen_height / 12);
  else if (button_no == 1) rect(screen_width * 3 / 4, screen_height / 6, screen_width / 4, screen_height / 12);
  else if (button_no == 2) rect(screen_width / 2, screen_height / 4, screen_width / 4, screen_height / 12);
  else if (button_no == 3) rect(screen_width * 3 / 4, screen_height / 4, screen_width / 4, screen_height / 12);
  
  //テキスト
  fill(0);
  textSize(40);
  text("distance", screen_width / 2 + 65, screen_height / 6 + 48);
  text("color", screen_width * 3 / 4 + 65, screen_height / 6 + 48);
  text("direction", screen_width / 2 + 65, screen_height / 4 + 48);
  text("speed", screen_width * 3 / 4 + 65, screen_height / 4 + 48);
}
