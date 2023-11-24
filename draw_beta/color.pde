void ColorWindow()
{
  //背景
  fill(32);
  rect(screen_width / 2, screen_height / 3, screen_width / 2, screen_height * 2 / 3);
  
  for (int i = 0; i < robot_max; i++)
  {
    //ウィンドウ
    fill(32);
    stroke(255);
    rect(screen_pos1[i][0], screen_pos1[i][1], screen_width / 4, screen_height / 3);
    
    //情報
    fill(255);
    textSize(20);
    text("USB" + i, screen_pos1[i][0] + 120, screen_pos1[i][1] + 25);
    text("r : " + robot_color[i][0], screen_pos1[i][0] + 10, screen_pos1[i][1] + 50);
    text("g : " + robot_color[i][1], screen_pos1[i][0] + 110, screen_pos1[i][1] + 50);
    text("b : " + robot_color[i][2], screen_pos1[i][0] + 210, screen_pos1[i][1] + 50);
    
    //rgbグラフ
    noStroke();
    
    fill(255, 0, 0);
    rect(screen_pos1[i][0] + 220, screen_pos1[i][1] + 240 - map(robot_color[i][0], 0, 255, 0, 150),
      10, map(robot_color[i][0], 0, 255, 0, 150));
    
    fill(0, 255, 0);
    rect(screen_pos1[i][0] + 240, screen_pos1[i][1] + 240 - map(robot_color[i][1], 0, 255, 0, 150),
      10, map(robot_color[i][1], 0, 255, 0, 150));
    
    fill(0, 0, 255);
    rect(screen_pos1[i][0] + 260, screen_pos1[i][1] + 240 - map(robot_color[i][2], 0, 255, 0, 150),
      10, map(robot_color[i][2], 0, 255, 0, 150));
    
    //rgb平均
    fill(robot_color[i][0], robot_color[i][1], robot_color[i][2]);
    rect(screen_pos1[i][0] + 60, screen_pos1[i][1] + 110, 100, 100);
  }
}
