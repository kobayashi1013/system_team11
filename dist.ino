//超音波センサ

int search() {  //障害物を探索
  int dist;
  int ave_dist;
  int ave_dist2;           //距離の平均
  int diff_dist;           //前の距離との差
  static int before_dist;  //1つ前の距離を保存
  static int count2 = 0;   //moving_average関数での5回ごとのカウント用

  dist = distance();  //単位【cm】で距離を観測
  if (dist == 0) {
    dist = before_dist;  //----------0（誤作動）だったら前の距離を代入する(この部分を無しでもやってみる)------------------------------------------------------------
  }
  diff_dist = dist - before_dist;  //前の距離との差
                                   // ave_dist = moving_average(dist);
                                   // count2++;

  // if (count2 == 5) {  //5回目のループで距離平均を使用
  //   // diff_dist = ave_dist - before_dist;  //前の距離との差
  //   ave_dist2 = ave_dist;
  //   count2 = 0;  //初期化
  //   Serial.println(dist);

  //   //距離が50cm以内かつ，前の距離との差が±5以内の時 ↓
  //   if (0 < ave_dist && ave_dist < 50 && -5 <= diff_dist && diff_dist <= 5) {
  //     return 1;  //障害物あり：１を返す
  //   }

  //   // if (0 < ave_dist && ave_dist < 50) {
  //   return 1;  //障害物あり：１を返す
  // }
  // }
  //距離が50cm以内かつ，前の距離との差が±5以内の時 ↓
  if (0 < dist && dist < 50 && -5 <= diff_dist && diff_dist <= 5) {
    return 1;  //障害物あり：１を返す
  }

  before_dist = dist;  //1つ前の距離を保存しておく
  return 0;            //障害物なし：0を返し続ける
}


int judge_RorC() {  //，ロボかコップか判断
  int dist;
  static int before_dist;  //1つ前の距離を保存
  // static int count_not0 = 0;   //グローバル変数にした，前の距離の差が０じゃない回数

  dist = distance();  //単位【cm】で距離を観測
  if (dist == 0) {
    dist = before_dist;  //----------0（誤作動）だったら前の距離を代入する(この部分を無しでもやってみる)------------------------------------------------------------
  }

  if (before_dist - dist != 0) {  //前の距離の差が0じゃない場合↓------------------------------------------
    count_not0++;
  }
  if (count_not0 > 5) {  //回以上差があった↓(回数は調整)----------------------------------------------------------------------------------
    count_not0 = 0;      //初期化
    return 1;            //ロボなので1を返す
  }

  before_dist = dist;
  return 0;  //回以下の時はずっと０を返す（カップだと予測される）
}


int Cup_dist() {  //カップをつかめているか判断する関数
  int dist;
  static int ave_dist;  //距離の平均
  //int diff_dist;           //前の距離との差
  static int before_dist;  //1つ前の距離を保存
  static int count2 = 0;   //moving_average関数での5回ごとのカウント用

  dist = distance();  //単位【cm】で距離を観測
  if (dist == 0) {
    dist = before_dist;  //----------0（誤作動）だったら前の距離を代入する(この部分を無しでもやってみる)------------------------------------------------------------
  }
  // ave_dist = moving_average(dist);
  // count2++;
  // if (count2 == 5) {  //5回目のループで距離平均を使用
  //   count2 = 0;       //初期化
  // }
  if (0 < dist && dist < 4) {  //------------------------------------------------------------
    buzzer.play(">c32");       //一回ブザー音を鳴らす
    return 1;                  //障害物あり：１を返す
  }

  before_dist = ave_dist;  //平均の距離を保存しておく
  return 0;                //障害物なし：0を返し続ける
}

//距離を計測(単位はcm)
int distance() {
  unsigned long interval;  //Echoのパルス幅(μs)
  int dist;                //距離(cm)

  digitalWrite(trig, HIGH);  //10μsのパルスを超音波センサのTrigピンに出力
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH, 23071);              //Echo信号がHIGHである時間(μs)を計測
  dist = (0.61 * 25 + 331.5) * interval / 10000 / 2;  //距離(cm)に変換

  delay(60);  //trigがHIGHになる間隔を60ms以上空ける（超音波センサの仕様）

  return dist;
}

int moving_average(int x) {
  static int count = 0;
  static int sum = 0;
  static int data[] = { 0, 0, 0, 0, 0 };

  if (count == 5) {
    count = 0;
  }

  sum -= data[count];  //古い値を削除
  data[count] = x;
  sum += data[count];  //新しい値を追加
  count++;

  return sum / 5;  //過去 5 件の移動平均を返す
}