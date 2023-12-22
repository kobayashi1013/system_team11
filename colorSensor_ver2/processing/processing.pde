import processing.serial.*;

Serial port1;
boolean sod = false;

int red = 0;
int green = 0;
int blue = 0;
int colorNo = 0;

void setup()
{
  size(200, 100);
  port1 = new Serial(this, "COM5", 9600);
  port1.clear();
}

void draw()
{
  fill(red, green, blue);
  rect(0, 0, 100, 100);
  
  switch (colorNo)
  {
    case 0:
      fill(255, 255, 255);
      break;
    case 1:
      fill(12, 25, 28);
      break;
    case 2:
      fill(121, 2, 4);
      break;
    case 3:
      fill(1, 32, 96);
      break;
  }
  
  rect(0, 100, 100, 100);
}

void serialEvent(Serial p)
{
  int byteSize = p.available();
  boolean bod = false;
  
  while (byteSize > 0)
  {
    if (sod)
    {
      if (byteSize >= 4)
      {
        
        red = p.read();
        green = p.read();
        blue = p.read();
        colorNo = p.read();
        
        bod = true;
        sod = false;
        byteSize -= 4;
      }
      else
      {
        break;
      }
    }
    else
    {
      if (p.read() == 'H')
      {
        sod = true;
      }
      
      byteSize -= 1;
    }
  }
  
  if (bod)
  {
    port1.write('H');
  }
}
