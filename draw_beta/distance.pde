void DistanceWindow()
{
  //背景
  fill(0);
  rect(screen_width / 2, screen_height / 3, screen_width / 2, screen_height * 2 / 3);
  
  for (int i = 0; i < robot_max; i++)
  {
    //ウィンドウ
    fill(255);
    stroke(0);
    rect(screen_pos1[i][0], screen_pos1[i][1], screen_width / 4, screen_height / 3);
    
    fill(0);
    textSize(20);
    text("USB" + i, screen_pos1[i][0] + 120, screen_pos1[i][1] + 25);
    text("distance : " + robot_distance[i], screen_pos1[i][0] + 80, screen_pos1[i][1] + 50);
    
    //障害物
    fill(255, 0, 0);
    /*rect(screen_pos1[i][0] + screen_width / 8 - robot_distance[i],
      screen_pos1[i][0] + 100 - robot_distance[i],
      100 - robot_distance[i], 100 - robot_distance[i]);*/
    
    rect(screen_pos1[i][0] + screen_width / 8 + robot_distance[i],
      screen_pos1[i][1] + screen_height / 6 + robot_distance[i],
      100 - robot_distance[i] * 2, 100 - robot_distance[i] * 2);
      
    /*strokeWeight(5);
    stroke(255, 0, 0);
    line(screen_pos1[i][0] + 20, screen_pos1[i][1] + 70, screen_pos1[i][0] + 20, screen_pos1[i][1] + 240);
    stroke(0, 0, 255);
    line(screen_pos1[i][0] + 280, screen_pos1[i][1] + 70, screen_pos1[i][0] + 280, screen_pos1[i][1] + 240);
    stroke(0);
    line(screen_pos1[i][0] + 20, screen_pos1[i][1] + 70, screen_pos1[i][0] + 280, screen_pos1[i][1] + 70);
    line(screen_pos1[i][0] + 20, screen_pos1[i][1] + 240, screen_pos1[i][0] + 280, screen_pos1[i][1] + 240);
    strokeWeight(1);
    
    //針
    int needle = 50;
    stroke(255, 0, 0);
    strokeWeight(3);
    line(screen_pos1[i][0] + screen_width / 8, screen_pos1[i][1] + screen_height / 6 + 20,
      screen_pos1[i][0] + screen_width / 8 + cos(robot_direction[i] * PI / 180) * needle,
      screen_pos1[i][1] + screen_height / 6 + 20 + sin(robot_direction[i] * PI / 180) * needle);
    strokeWeight(1);
      
    //北
    fill(255, 0, 0);
    triangle(screen_pos1[i][0] + 240, screen_pos1[i][1] + 20,
      screen_pos1[i][0] + 240, screen_pos1[i][1] + 40,
      screen_pos1[i][0] + 280, screen_pos1[i][1] + 30);*/
  }
}
