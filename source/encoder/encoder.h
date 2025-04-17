#ifndef ENCODER_H_
#define ENCODER_H_

#include <Arduino.h>

class Encoder {
public:
  Encoder(int pinA_, int pinB_);
  ~Encoder() {}

  void setup();

  int  getEncoderValue();     // サンプルとして残す(未実装のまま)
  long getSpeed();
  long getPosition();
  void setPosition(long Position);

private:
  // ---- A/B相変化を処理する関数 ----
  void IRAM_ATTR updateEncoder(int pinStateA, int pinStateB);
  static void IRAM_ATTR onEncoderChangeA_wrapper(void* arg);
  static void IRAM_ATTR onEncoderChangeB_wrapper(void* arg);

  // ---- タイマーISR ----
  static void IRAM_ATTR onTimer_wrapper(); // staticラッパISR
  void IRAM_ATTR onTimerISR();             // 実際の処理

private:
  int pinA;
  int pinB;

  // 10ms周期などにしたい場合の割り込み周期(µs)
  // 例: 10000 µs = 10ms
  unsigned long _timerIntervalUs = 10000;

  // タイマー関連
  hw_timer_t*   _timer = nullptr;

  // クリティカルセクション用
  portMUX_TYPE _timerMux = portMUX_INITIALIZER_UNLOCKED;

  // エンコーダカウント用
  volatile long _encoderCount = 0; 
  volatile int  _lastEncoded  = 0;

  // 速度・位置
  volatile long _position = 0; 
  volatile long _speed    = 0; 

  // 前回カウント値 (速度計算用)
  volatile long _prevCount = 0;

  // ★ static変数で「単一インスタンス」ポインタを保持
  static Encoder* s_instance;
};

#endif // ENCODER_H_
