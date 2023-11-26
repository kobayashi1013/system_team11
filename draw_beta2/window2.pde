void Window2() //カラーウィンドウ
{
  //ウィンドウ
  fill(96);
  noStroke();
  rect(screen_width / 2 - 5, 20, (screen_width / 2 - 30) / 2, screen_height - 30, 8);
  
  //情報
  fill(157, 229, 170);
  noStroke();
  ellipse(screen_width / 2 + 40, 50, 30, 30);
  
  fill(240);
  textFont(font[1]);
  textSize(32);
  text("カラー", screen_width / 2 + 80, 62);
  
  for (int i = 0; i < robot_num; i++)
  {
    //小ウィンドウ
    fill(48);
    stroke(0);
    rect(screen_width / 2 + 5, (screen_height - 80) / robot_num * i + 80,
      (screen_width / 2 - 30) / 2 - 20, (screen_height - 80) / robot_num - 20, 8);
    line(screen_width / 2 + 10, (screen_height - 80) / robot_num * i + 81,
      screen_width / 2 + 265, (screen_height - 80) / robot_num * i + 81);
      
    //情報
    fill(240);
    textFont(font[1]);
    textSize(24);
    text("r : " + robot_color[i][0], screen_width / 2 + 20, (screen_height - 80) / robot_num * i + 108);
    text("g : " + robot_color[i][1], screen_width / 2 + 100, (screen_height - 80) / robot_num * i + 108);
    text("b : " + robot_color[i][2], screen_width / 2 + 180, (screen_height - 80) / robot_num * i + 108);
    
    //グラフ
    noStroke();
    fill(255, 0, 0);
    rect(screen_width / 2 + 210, (screen_height - 80) / robot_num * i + 280 - map(robot_color[i][0], 0, 255, 0, 150),
      10, map(robot_color[i][0], 0, 255, 0, 150));
    fill(0, 255, 0);
    rect(screen_width / 2 + 225, (screen_height - 80) / robot_num * i + 280 - map(robot_color[i][1], 0, 255, 0, 150),
      10, map(robot_color[i][1], 0, 255, 0, 150));
    fill(0, 0, 255);
    rect(screen_width / 2 + 240, (screen_height - 80) / robot_num * i + 280 - map(robot_color[i][2], 0, 255, 0, 150),
      10, map(robot_color[i][2], 0, 255, 0, 150));
    
    //色
    fill(robot_color[i][0], robot_color[i][1], robot_color[i][2]);
    noStroke();
    rect(screen_width / 2 + 50, (screen_height - 80) / robot_num * i + 155, 100, 100);
  }
}
