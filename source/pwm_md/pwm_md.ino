// PWM制御ピン設定
const int pwmPin = 3;  // PWM出力 (IN1)
const int InA = 4;
const int InB = 5;
const int Mode = 6;
const int PSB = 7;
const int BRK = 8;
const int Full = 9;

int sensorPin = A5;

int sensorValue=0;

void setup() {
  // シリアルモニタ開始
  Serial.begin(115200);

  // ピンモード設定
  pinMode(pwmPin, OUTPUT);
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(Mode, OUTPUT);
  pinMode(PSB, OUTPUT);
  pinMode(BRK, OUTPUT);

  pinMode(sensorPin, INPUT);

  digitalWrite(InA, HIGH);
  digitalWrite(InB, LOW);

  digitalWrite(Mode, LOW);
  digitalWrite(PSB, HIGH);
  digitalWrite(BRK, LOW);
  digitalWrite(Full, HIGH);

  int counter = 0;

  //Serial.println("PWMモータ制御スタート");
}

void loop() {
  // digitalWrite(pwmPin, HIGH);
  sensorValue  = analogRead(sensorPin);
    if(sensorValue < 100){
    // count++;
    // delay(10);
    // if(count > 150){
    //     flag = HIGH;
    // }
    digitalWrite(BRK, HIGH);
    analogWrite(pwmPin, 0);
  }else{
    // count = 0;
    digitalWrite(BRK, LOW);
    analogWrite(pwmPin, 127);
    // digitalWrite(solenoidValve, LOW);
  }
  Serial.println(sensorValue);
delay(10);
  // Serial.println("正転（低速→高速）");
  //   digitalWrite(InA, HIGH);
  // digitalWrite(InB, LOW);
  // for (int speed = 0; speed <= 200; speed += 5) {
  //   analogWrite(pwmPin, speed);  // PWMで速度制御
  //   delay(30);
  // }


  // // delay(1000);  // 一定時間動作

  // // Serial.println("逆転（高速→低速）");
  // for (int speed = 200; speed >= 0; speed -= 5) {
  //   analogWrite(pwmPin, speed);
  //   delay(30);
  // }

  //     digitalWrite(InA, LOW);
  // digitalWrite(InB, HIGH);
  // for (int speed = 0; speed <= 200; speed += 5) {
  //   analogWrite(pwmPin, speed);  // PWMで速度制御
  //   delay(30);
  // }


  // // delay(1000);  // 一定時間動作

  // // Serial.println("逆転（高速→低速）");
  // for (int speed = 200; speed >= 0; speed -= 5) {
  //   analogWrite(pwmPin, speed);
  //   delay(30);
  // }
  // analogWrite(pwmPin, 255);

  // delay(10);  // 停止前待機
}
