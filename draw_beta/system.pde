boolean Collider(int no)
{
  boolean ret = false;
  
  switch (no)
  {
    case 0: //距離センサボタン
      if (mouseX >= screen_width / 2
        && mouseX <= screen_width * 3 / 4
        && mouseY >= screen_height / 6
        && mouseY <= screen_height / 4)
        ret = true;
      break;
    case 1: //カラーセンサボタン
      if (mouseX >= screen_width * 3 / 4
        && mouseX <= screen_width
        && mouseY >= screen_height / 6
        && mouseY <= screen_height / 4)
        ret = true;
      break;
    case 2: //方向
      if (mouseX >= screen_width / 2
        && mouseX <= screen_width * 3 / 4
        && mouseY >= screen_height / 4
        && mouseY <= screen_height / 3)
        ret = true;
      break;
    case 3: //スピード
      if (mouseX >= screen_width * 3 / 4
        && mouseX <= screen_width
        && mouseY >= screen_height / 4
        && mouseY <= screen_height / 3)
        ret = true;
      break;
  }
  
  return ret;
}

void mousePressed()
{
  if (Collider(0) && mousePressed) button_no = 0;
  else if (Collider(1) && mousePressed) button_no = 1;
  else if (Collider(2) && mousePressed) button_no = 2;
  else if (Collider(3) && mousePressed) button_no = 3;
}
