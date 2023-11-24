//マクロ
final int screen_width = 1200;
final int screen_height = 800;
final int robot_num = 3;

//変数
PFont[] font = new PFont[2];

void setup()
{
  //初期設定
  size(1200, 800);
  frameRate(60);
  
  //フォント設定
  font[0] = loadFont("Calibri-Bold-48.vlw");
  font[1] = loadFont("Calibri-Light-48.vlw");
}

void draw()
{
  InitWindow();
  Window1();
}

void InitWindow()
{
  background(48);
}

void Window1() //ロボットの基本情報
{
  for (int i = 0; i < robot_num; i++)
  {
    //ウィンドウ
    fill(80);
    noStroke();
    rect(20, (screen_height - 80) / robot_num * i + 80,
      screen_width / 2 - 40, (screen_height - 80) / robot_num - 20, 8);
      
    //名前
    fill(224);
    textFont(font[1]);
    textSize(28);
    text("USB " + i, 30, (screen_height - 80) / robot_num * i + 110);
    
    //得点
    fill(227, 37, 94);
    rect(screen_width / 4, (screen_height - 80) / robot_num * i + 85,
      6, 38);
    
    fill(224);
    textFont(font[1]);
    textSize(32);
    text("point", screen_width / 4 + 15, (screen_height - 80) / robot_num * i + 110);
    
    fill(112);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 85,
      120, 38, 8);
    
    //距離
    fill(237, 223, 69);
    rect(screen_width / 4, (screen_height - 80) / robot_num * i + 128,
      6, 38);
      
    fill(224);
    textFont(font[1]);
    textSize(32);
    text("distance", screen_width / 4 + 15, (screen_height - 80) / robot_num * i + 153);
    
    fill(112);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 128,
      120, 38, 8);
    
    //カラー
    fill(157, 229, 170);
    rect(screen_width / 4, (screen_height - 80) / robot_num * i + 171,
      6, 38);
      
    fill(224);
    textFont(font[1]);
    textSize(32);
    text("color", screen_width / 4 + 15, (screen_height - 80) / robot_num * i + 196);
    
    fill(112);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 171,
      120, 38, 8);
    
    //方向
    fill(198, 121, 240);
    rect(screen_width / 4, (screen_height - 80) / robot_num * i + 214,
      6, 38);
      
    fill(224);
    textFont(font[1]);
    textSize(32);
    text("direction", screen_width / 4 + 15, (screen_height - 80) / robot_num * i + 239);
    
    fill(112);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 214,
      120, 38, 8);
    
    //スピード
    fill(144, 219, 227);
    rect(screen_width / 4, (screen_height - 80) / robot_num * i + 257,
      6, 38);
      
    fill(224);
    textFont(font[1]);
    textSize(32);
    text("speed", screen_width / 4 + 15, (screen_height - 80) / robot_num * i + 282);
    
    fill(112);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 257,
      120, 38, 8);
  }
}
