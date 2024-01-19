void Window1() //ロボットの基本情報
{
  for (int i = 0; i < robot_num; i++)
  {
    //ウィンドウ
    fill(64);
    noStroke();
    rect(20, (screen_height - 80) / robot_num * i + 80,
      screen_width / 2 - 40, (screen_height - 80) / robot_num - 20, 8);
      
    //名前
    fill(240);
    textFont(font[1]);
    textSize(28);
    text("USB " + i, 30, (screen_height - 80) / robot_num * i + 110);
    
    //状態
    fill(112);
    stroke(0);
    rect(30, (screen_height - 80) / robot_num * i + 180, 230, 60, 8);
    
    fill(240);
    textFont(font[1]);
    textSize(48);
    switch (robot_mode[i])
    {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 101:
      case 300:
        text("探索中", 40, (screen_height - 80) / robot_num * i + 228);
        break;
      case 11:
      case 12:
      case 13:
      case 99:
      case 100:
        text("運搬中", 40, (screen_height - 80) / robot_num * i + 228);
        break;
    }
    
    
    
    //得点
    fill(227, 37, 94);
    noStroke();
    rect(screen_width / 4 - 30, (screen_height - 80) / robot_num * i + 85,
      6, 38);
    
    fill(240);
    textFont(font[1]);
    textSize(32);
    text("得点", screen_width / 4 - 15, (screen_height - 80) / robot_num * i + 115);
    
    fill(48);
    stroke(0);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 85,
      120, 38, 8);
    line(screen_width / 2 - 145, (screen_height - 80) / robot_num * i + 86,
      screen_width / 2 - 35, (screen_height - 80) / robot_num * i + 86);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text(robot_score[i], screen_width / 2 - 140, (screen_height - 80) / robot_num * i + 117);
    
    //距離
    fill(237, 223, 69);
    noStroke();
    rect(screen_width / 4 - 30, (screen_height - 80) / robot_num * i + 128,
      6, 38);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text("距離", screen_width / 4 - 15, (screen_height - 80) / robot_num * i + 158);
    
    fill(48);
    stroke(0);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 128,
      120, 38, 8);
    line(screen_width / 2 - 145, (screen_height - 80) / robot_num * i + 129,
      screen_width / 2 - 35, (screen_height - 80) / robot_num * i + 129);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text(robot_distance[i], screen_width / 2 - 140, (screen_height - 80) / robot_num * i + 160);
    
    //カラー
    fill(157, 229, 170);
    noStroke();
    rect(screen_width / 4 - 30, (screen_height - 80) / robot_num * i + 171,
      6, 38);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text("カラー", screen_width / 4 - 15, (screen_height - 80) / robot_num * i + 201);
    
    fill(robot_color[i][0], robot_color[i][1], robot_color[i][2]);
    stroke(0);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 171,
      120, 38, 8);
    
    //方位
    fill(198, 121, 240);
    noStroke();
    rect(screen_width / 4 - 30, (screen_height - 80) / robot_num * i + 214,
      6, 38);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text("方位", screen_width / 4 - 15, (screen_height - 80) / robot_num * i + 244);
    
    fill(48);
    stroke(0);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 214,
      120, 38, 8);
    line(screen_width / 2 - 145, (screen_height - 80) / robot_num * i + 215,
      screen_width / 2 - 35, (screen_height - 80) / robot_num * i + 215);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text(robot_direction[i], screen_width / 2 - 140, (screen_height - 80) / robot_num * i + 246);
    
    //スピード
    fill(144, 219, 227);
    noStroke();
    rect(screen_width / 4 - 30, (screen_height - 80) / robot_num * i + 257,
      6, 38);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text("速度", screen_width / 4 - 15, (screen_height - 80) / robot_num * i + 287);
    
    fill(48);
    stroke(0);
    rect(screen_width / 2 - 150, (screen_height - 80) / robot_num * i + 257,
      120, 38, 8);
    line(screen_width / 2 - 145, (screen_height - 80) / robot_num * i + 258,
      screen_width / 2 - 35, (screen_height - 80) / robot_num * i + 258);
      
    fill(240);
    textFont(font[1]);
    textSize(32);
    text(robot_speed[i], screen_width / 2 - 140, (screen_height - 80) / robot_num * i + 289);
  }
}
