//マクロ
final int screen_width = 1200;
final int screen_height = 800;
final int robot_num = 3;

//変数
PFont[] font = new PFont[2];

/////ネットワーク部との連携部分/////
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
