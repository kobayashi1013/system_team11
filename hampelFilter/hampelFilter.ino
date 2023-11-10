//以下インターフェース
void setup() {}
void loop() {}

//以下本体
const float hampel_diff = 3; //標準偏差倍数

//hampelフィルタ
float hampelFilter(const float _array[], const int _arrayNum, float _value)
{
  float _center = center(_array, _arrayNum); //中央値
  float _deviation = sqrt(dispersion(_array, _arrayNum)); //標準偏差

  //上界下界の設定
  float _upper = hampel_diff * _deviation;
  float _lower = hampel_diff * _deviation;

  //値の調整
  if (_value > _upper || _value < _lower) _value = _center;

  return _value;
}

//バブルソート（計算量が多いため、今後変えるかも）
void sort(float _array[], const int _arrayNum)
{
  for (int i = 0; i < _arrayNum; i++)
  {
    for (int j = 0; j < _arrayNum; j++)
    {
      if (_array[i] > _array[j])
      {
        int _tmp = _array[i];
        _array[i] = _array[j];
        _array[j] = _tmp;
      }
    }
  }
}

//中央値
float center(const float _array[], const int _arrayNum)
{
  float _tmpArray[_arrayNum];

  //配列のコピー
  for (int i = 0; i < _arrayNum; i++) _tmpArray[i] = _array[i];

  //ソート
  sort(_tmpArray, _arrayNum);

  //中央値の計算
  float _center = 0;
  if (_arrayNum % 2 == 1)
  {
    _center = _tmpArray[_arrayNum / 2];
  }
  else
  {
    _center = (_tmpArray[_arrayNum / 2] + _tmpArray[_arrayNum / 2 + 1]) / 2;
  }

  return _center;
}

//分散
float dispersion(const float _array[], const int _arrayNum)
{
  float _average = average(_array, _arrayNum); //平均
  float _diffArray[_arrayNum]; //平均差分の二乗

  for (int i = 0; i < _arrayNum; i++) _diffArray[i] = (_array[i] - _average) * (_array[i] - _average);

  return average(_diffArray, _arrayNum); //平均
}

//平均
float average(const float _array[], const int _arrayNum)
{
  float _sigma = 0;

  for (int i = 0; i < _arrayNum; i++) _sigma += _array[i];

  return _sigma / _arrayNum;
}