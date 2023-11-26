void Window3()
{
  //ウィンドウ
  fill(96);
  noStroke();
  rect(screen_width * 3 / 4 - 5, 20, (screen_width / 2 - 30) / 2, screen_height - 30, 8);
  
  //情報
  fill(198, 121, 240);
  noStroke();
  ellipse(screen_width * 3 / 4 + 40, 50, 30, 30);
  
  fill(240);
  textFont(font[1]);
  textSize(32);
  text("方位", screen_width * 3 / 4 + 80, 62);
  
  for (int i = 0; i < robot_num; i++)
  {
    //小ウィンドウ
    fill(48);
    stroke(0);
    rect(screen_width * 3 / 4 + 5, (screen_height - 80) / robot_num * i + 80,
      (screen_width / 2 - 30) / 2 - 20, (screen_height - 80) / robot_num - 20, 8);
    line(screen_width * 3 / 4 + 10, (screen_height - 80) / robot_num * i + 81,
      screen_width * 3 / 4 + 265, (screen_height - 80) / robot_num * i + 81);
      
    //情報
    fill(240);
    textFont(font[1]);
    textSize(24);
    text("方位 : " + robot_direction[i], screen_width * 3 / 4 + 80, (screen_height - 80) / robot_num * i + 108);
    
    //フィールド
    strokeWeight(3);
    stroke(105, 181, 242);
    line(screen_width * 3 / 4 + 180, (screen_height - 80) / robot_num * i + 150,
      screen_width * 3 / 4 + 180, (screen_height - 80) / robot_num * i + 250);
    stroke(242, 105, 105);
    line(screen_width * 3 / 4 + 40, (screen_height - 80) / robot_num * i + 150,
      screen_width * 3 / 4 + 40, (screen_height - 80) / robot_num * i + 250);
    stroke(224);
    line(screen_width * 3 / 4 + 40, (screen_height - 80) / robot_num * i + 150,
      screen_width * 3 / 4 + 180, (screen_height - 80) / robot_num * i + 150);
    line(screen_width * 3 / 4 + 40, (screen_height - 80) / robot_num * i + 250,
      screen_width * 3 / 4 + 180, (screen_height - 80) / robot_num * i + 250);
    
    //方位
    int needle = 40;
    stroke(218, 196, 237);
    line(screen_width * 3 / 4 + 110, (screen_height - 80) / robot_num * i + 200,
      screen_width * 3 / 4 + 110 + cos(robot_direction[i] * PI / 180) * needle,
      (screen_height - 80) / robot_num * i + 200 + sin(robot_direction[i] * PI / 180) * needle);
    
    //北針
    fill(224);
    stroke(242, 80, 80);
    strokeWeight(2);
    triangle(screen_width * 3 / 4 + 200, (screen_height - 80) / robot_num * i + 190,
      screen_width * 3 / 4 + 200, (screen_height - 80) / robot_num * i + 210,
      screen_width * 3 / 4 + 240, (screen_height - 80) / robot_num * i + 200);
    strokeWeight(1);
  }
}
