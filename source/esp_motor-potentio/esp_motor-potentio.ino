#include <Arduino.h>
#include "driver/ledc.h"

const int pwmPin = 15;
const int pwmFreq = 1000;
const int pwmResolution = 12;

const int InA = 2;
const int InB = 0;
const int Mode = 4;
const int PSB = 16;
const int BRK = 17;
const int Full = 5;

const int botton = 22;
const int sw = 23;

const int lineSensor = 32; // センサ入力（そのまま残す）
const int potPin = 33;     // ★ ポテンショメータ入力ピン（新規追加）

int speed = 0;
const int maxSpeed = 255;

// ボタン状態記録用
bool lastButtonState = HIGH;
bool lastSwState = HIGH;

int cnt = 0;

bool stopFlag = false;

void setup() {
  Serial.begin(115200);

  ledcAttach(pwmPin, pwmFreq, pwmResolution);

  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(Mode, OUTPUT);
  pinMode(PSB, OUTPUT);
  pinMode(BRK, OUTPUT);
  pinMode(Full, OUTPUT);

  pinMode(botton, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);

  pinMode(lineSensor, INPUT);
  pinMode(potPin, INPUT);  // 新しいアナログ入力ピン

  digitalWrite(InA, HIGH);
  digitalWrite(InB, LOW);
  digitalWrite(Mode, LOW);
  digitalWrite(PSB, HIGH);
  digitalWrite(BRK, LOW); // ブレーキON
  digitalWrite(Full, HIGH);

  Serial.println("PWMモータ制御スタート");
  delay(2000);
}

void loop() {
  bool currentButtonState = digitalRead(botton);
  bool currentSwState = digitalRead(sw);

  int potValue = analogRead(potPin);         // ポテンショメータ値（0〜4095）
  int sensorValue = analogRead(lineSensor);  // ラインセンサ値（未使用だが保持）

  // ポテンショメータ値を速度にマッピング（0〜maxSpeed）
  // speed = map(potValue, 0, 4095, 0, maxSpeed);
  speed = potValue;

  // if (currentSwState == LOW) {
  //   digitalWrite(BRK, HIGH); // ブレーキON
  //   speed = 0;
  //   Serial.println("停止（SW押下）");
  // } else {
  //   digitalWrite(BRK, LOW); // ブレーキ解除
  // }

  if (cnt > 300 || stopFlag) {
    digitalWrite(BRK, HIGH); // ブレーキON
    speed = 0;
    stopFlag = true;
    Serial.println("停止（SW押下）");
  } else {
    digitalWrite(BRK, LOW); // ブレーキ解除
  }

  ledcWrite(pwmPin, 500);  // PWM出力

  // // ボタン・スイッチ状態の更新（今回はトリガー用途では未使用）
  // lastButtonState = currentButtonState;
  // lastSwState = currentSwState;

  if (cnt % 100 == 0) {
    Serial.print("Pot = ");
    Serial.print(potValue);
    Serial.print(" → Speed = ");
    Serial.println(speed);

    Serial.print("LineSensor = ");
    Serial.println(sensorValue);
  }

  cnt++;
  delay(10);
}
