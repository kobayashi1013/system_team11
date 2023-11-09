//マクロ
final int width = 1200;
final int height = 800;
final int filterWidth = 1000;
final int filterHeight = 100;
final int dataNum = 200;
final float angleSpeed = 0.10;

//変数
int[] data = new int[dataNum];

//hampelFilter
final int hampel_dataNum = 5;
final int hampel_center = 2;
int[] hampel = new int [hampel_dataNum];
int[] centerDiff = new int[hampel_dataNum];
int[] center = new int[hampel_dataNum];
int hampel_dataNo = 0;
int mode = 0;

void setup()
{
  //Processing初期化
  size(1200, 800);
  background(255);
  
  //データ初期化
  initData();
  
  //描画
  for (int i = 0; i < dataNum; i++)
  {
    data[i] = hampelFilter(data[i]);
    if (data[i] != -1) drawData(i);
  }
}

//データ設定
void initData()
{
  float angle = 0;
  
  for (int i = 0; i < dataNum; i++)
  {
    data[i] = (int)(filterHeight * sin(angle));  
    angle += angleSpeed;
  }
  
  //外れ値の設定
  data[60] = 300;
  data[100] = -200;
  data[102] = 250;
}

//データ描画
void drawData(int no)
{
  strokeWeight(5);
  stroke(0, 0, 255);
  
  //点描画
  point(map(no, 0, dataNum - 1, width / 2 - filterWidth / 2, width / 2 + filterWidth / 2),
    map(data[no], -1 * filterWidth / 2, filterWidth / 2, height / 2 + filterWidth / 2, height / 2 - filterWidth / 2));

  strokeWeight(1);
  stroke(255, 0, 0);
  
  //線描画
  if (no >= hampel_dataNum)
  {
    line(map(no - 1, 0, dataNum - 1, width / 2 - filterWidth / 2, width / 2 + filterWidth / 2),
      map(data[no - 1], -1 * filterWidth / 2, filterWidth / 2, height / 2 + filterWidth / 2, height / 2 - filterWidth / 2),
      map(no, 0, dataNum - 1, width / 2 - filterWidth / 2, width / 2 + filterWidth / 2),
      map(data[no], -1 * filterWidth / 2, filterWidth / 2, height / 2 + filterWidth / 2, height / 2 - filterWidth / 2));
  }
}

//フィルタ処理
int hampelFilter(int value)
{
  int ret = 0;
  
  if (mode == 0) //初期データ取得
  {
    hampel[hampel_dataNo++] = value;
    if (hampel_dataNo >= hampel_dataNum) mode = 1;
    ret = -1;
  }
  else if (mode == 1)
  {
    int _center = centerValue(hampel);
    for (int i = 0; i < hampel_dataNum; i++) centerDiff[i] = abs(hampel[i] - _center);
    int _centerDiff = centerValue(centerDiff);
    int _centerUp = _center + 3 * _centerDiff;
    int _centerDown = _center - 3 * _centerDiff;
    
    if (value > _centerUp || value < _centerDown) value = _center;
    
    hampel = arrayMove(hampel, value);
    ret = value;
  }
  
  return ret;
}

//中央値
int centerValue(int[] array)
{
  array = sort(array);
  return array[array.length / 2];
}

//窓の移動
int[] arrayMove(int[] array, int value)
{
  for (int i = 0; i < hampel_dataNum - 1; i++)
  {
    array[i] = array[i + 1];
  }
  
  array[hampel_dataNum - 1] = value;
  
  return array;
}
