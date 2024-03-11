#include <Wire.h>                      //I2C使用
#include <Adafruit_PWMServoDriver.h>
 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);  // PCA9685のI2Cアドレスを指定
 
#define SERVOMIN 110    // 最小パルス幅(~4096の範囲)　setPEMの場合
#define SERVOMAX 480    // 最大パルス幅(~4096の範囲)　setPWMの場合

//#define SERVOMIN 500    // 最小パルス幅(μs). writeMicroseconds()関数の場合
//#define SERVOMAX 2400   // 最大パルス幅(μs)　writeMicroseconds()関数の場合

int Servo_Pin[] = {0,1,2,3,4}; // サーボ接続ピンを0番に
int angle[5];

String cmds[7]; //ラズパイから送信されるサーボモーターの角度値を文字列として格納する変数
 
void setup() {
  //シリアルポート
  Serial.begin(9600);
  
  pwm.begin();         // 初期設定
  pwm.setPWMFreq(50);  // PWM周期を50Hzに設定
  delay(1000);
 
}

void loop() {
  //インプットをread
  if (Serial.available() > 0){ //raspiからの送信データがあれば取得
    String input_ras = Serial.readStringUntil('\n');
    
    //Serial.println("test2");
    //  分割数 = 分割処理(文字列, 区切り文字, 配列)
    int index = split(input_ras, ',', cmds); //split関数を用いてカンマごとに文字列を抽出
    delay(500);

    //Serial.println("test3");
    //raspiからの角度をパルス幅に変更

    //シリアル通信で受け取った値を使用可能なように配列に格納
    for(int j=0; j<5; j++){
      angle[j] = cmds[j].toInt();
    };

    //angle = map(angle,0, 180, SERVOMIN, SERVOMAX);  // 角度(0~180)をパルス幅(500~2400μs)に変換  writeMicroseconds()関数の場合
    //pwm.writeMicroseconds(Servo_pin, angle);        // サーボを動作させる. writeMicroseconds()関数の場合

    //一つづつサーボモータを動かす
    for(int j=0; j<5; j++){
      angle[j] = map(angle[j], 0, 180, SERVOMIN, SERVOMAX);  // 角度(0~180)を4096の範囲に変換
      pwm.setPWM(Servo_Pin[j], 0, angle[j]);                 // サーボを動作させる
      delay(1000); 
      Serial.println(angle[j]);
    };


    //Serial.println("test4");
    Serial.println(cmds[0]+","+cmds[1]+","+cmds[2]+","+cmds[3]+","+cmds[4]+","+cmds[5]);
    delay(100);  // ラズベリーパイが応答を受信する時間を確保する
    //Serial.print("test5");
  };
 
}


//カンマで文字列を分解し、cmdsに格納
int split(String data, char delimiter, String *dst){
  for(int i=0; i<7; i++){ //cmds配列を初期化
    dst[i]="";
  };

  int index = 0;
  int arraySize = (sizeof(data))/sizeof((data[0]));
  int datalength = data.length();
  
  for(int i = 0; i < datalength; i++){
    char tmp = data.charAt(i);
    if( tmp == delimiter ){
      index++;
      if( index > (arraySize - 1)) return -1;
    }
    else dst[index] += tmp;
  }
  return (index + 1);
}


