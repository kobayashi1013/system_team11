int dist_G = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

int isSearch(int time)
{
  const int distDiff = 5;

  static int initTime = millis();
  static int mode = 0;
  static int initDist = 0;

  if (mode == 0) //計測開始
  {
    initTime = millis();
    initDist = dist_G;
    mode = 1;

    return 0;
  }
  else if (mode == 1) //計測中
  {
    if (millis() - initTime >= time)
      mode = 2;

    return 0;
  }
  else if (mode == 2) //計測終了
  {
    mode = 0;

    if (abs(dist_G - initDist) >= distDiff) //ロボット
      return 1;
    else
      return 2;
  }
}