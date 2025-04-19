#include "MotorDriver.h"
#include <Arduino.h>
void MotorDriver::_applyDir() {
  if (_dir == Dir::FWD) {
    digitalWrite(Pin::InA, HIGH);
    digitalWrite(Pin::InB, LOW);
  } else {  // REV
    digitalWrite(Pin::InA, LOW);
    digitalWrite(Pin::InB, HIGH);
  }
  digitalWrite(Pin::Mode, LOW);  // 固定（INxモード）
  digitalWrite(Pin::PSB, HIGH);
  digitalWrite(Pin::Full, HIGH);
}
void MotorDriver::begin(Dir dir) {
  ledcAttach(Pin::PWM, Motor::PWM_FREQ, Motor::PWM_RES);
  pinMode(Pin::InA, OUTPUT);
  pinMode(Pin::InB, OUTPUT);
  pinMode(Pin::Mode, OUTPUT);
  pinMode(Pin::PSB, OUTPUT);
  pinMode(Pin::BRK, OUTPUT);
  pinMode(Pin::Full, OUTPUT);
  _dir = dir;
  _applyDir();
  brake(true);  // 初期はブレーキON
}
void MotorDriver::setDirection(Dir dir) {
  if (_dir == dir) return;  // 変化なし
  _dir = dir;
  _applyDir();
}
void MotorDriver::setSpeed(int duty) {
  _duty = constrain(duty, 0, (1 << Motor::PWM_RES) - 1);
  ledcWrite(0, _duty);
}
void MotorDriver::brake(bool on) {
  digitalWrite(Pin::BRK, on ? HIGH : LOW);
}

void MotorDriver::releasePoweSave(){
  digitalWrite(Pin::PSB, HIGH);
  delay(500);
  digitalWrite(Pin::PSB, LOW);
  delay(1500);
  digitalWrite(Pin::PSB, HIGH);
}