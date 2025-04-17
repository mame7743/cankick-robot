#include "esp32-hal-gpio.h"    // digitalRead, attachInterruptArg 等
#include "esp32-hal-timer.h"   // 古いタイマーAPIが定義されている
#include <Arduino.h>
#include "encoder.h"

// ==============================
// クラス内部のstatic変数の実体定義
// ==============================
Encoder* Encoder::s_instance = nullptr;

// ==============================
// コンストラクタ
// ==============================
Encoder::Encoder(int pinA_, int pinB_)
: pinA(pinA_), pinB(pinB_) {
  // 必要に応じて初期化処理
}

// ==============================
// デストラクタ（今回は空でOK）
// ==============================
// Encoder::~Encoder() {}

// ==============================
// まだ未実装のメソッド (サンプルとして0返す)
// ==============================
int Encoder::getEncoderValue() {
  return 0;
}

// ==============================
// タイマー設定 & A/B割り込み設定
// ==============================
void Encoder::setup() {
  // 1) 自分自身のアドレスを静的変数 s_instance に登録
  //    （単一インスタンスのみ対応）
  s_instance = this;

  // --- A/B相入力設定 ---
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  attachInterruptArg(
      digitalPinToInterrupt(pinA),
      onEncoderChangeA_wrapper,
      this,
      CHANGE
  );
  attachInterruptArg(
      digitalPinToInterrupt(pinB),
      onEncoderChangeB_wrapper,
      this,
      CHANGE
  );

  // --- タイマー初期化 (古いコアAPI例) ---
  //    timerBegin(uint32_t freq) で 周波数=100 などを指定できるが、
  //    実際の動作はコアの実装に依存する。
  //    ここでは仮に100Hzを指定 ⇒ 1周期=10ms 近辺のイメージ
  _timer = timerBegin(100);

  // 2) タイマー停止
  timerStop(_timer);

  // ※ より正確に10msごとに割り込みしたいなら、
  //    timerSetOverflow(_timer, 10000);  // 1tick=1µsと仮定 (要コア確認)
  //    timerSetAutoReload(_timer, true); // 周期リピート
  //    といった設定が必要な場合もあります。
  //    ただし本当に古いコアだとこの辺りのAPI名が異なることもあるので注意。

  // 3) タイマーISRとして「staticラッパ関数」を登録
  timerAttachInterrupt(_timer, &onTimer_wrapper);

  // 4) タイマースタート
  timerStart(_timer);
}

// ==============================
// 割り込み (A相 or B相) ラッパ
// ==============================
void IRAM_ATTR Encoder::onEncoderChangeA_wrapper(void* arg) {
  Encoder* self = static_cast<Encoder*>(arg);
  int MSB = digitalRead(self->pinA);
  int LSB = digitalRead(self->pinB);
  self->updateEncoder(MSB, LSB);
}

void IRAM_ATTR Encoder::onEncoderChangeB_wrapper(void* arg) {
  Encoder* self = static_cast<Encoder*>(arg);
  int MSB = digitalRead(self->pinA);
  int LSB = digitalRead(self->pinB);
  self->updateEncoder(MSB, LSB);
}

// ==============================
// A/B相変化の実際の処理
// ==============================
void IRAM_ATTR Encoder::updateEncoder(int pinStateA, int pinStateB) {
  int encoded = (pinStateA << 1) | pinStateB;
  int sum = (_lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    _encoderCount++;
  }
  else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    _encoderCount--;
  }

  _lastEncoded = encoded;
}

// ==============================
// タイマーISRラッパ (static)
// ==============================
void IRAM_ATTR Encoder::onTimer_wrapper() {
  // s_instanceが登録されていれば、それを呼び出す
  if (s_instance) {
    s_instance->onTimerISR();
  }
}

// ==============================
// 実際のタイマーISR本体 (メンバ関数)
// ==============================
void IRAM_ATTR Encoder::onTimerISR() {
  portENTER_CRITICAL_ISR(&_timerMux);

  // 今のカウントを取得
  long currentCount = _encoderCount;
  
  // 位置を更新 (_position に反映)
  _position = currentCount;

  // 速度計算 (前回との差分)
  long delta = currentCount - _prevCount;
  double intervalSec = (double)_timerIntervalUs * 1e-6; // 例: 10000µs=0.01s
  _speed = (long)(delta / intervalSec);

  // 次の計測用に保存
  _prevCount = currentCount;

  portEXIT_CRITICAL_ISR(&_timerMux);
}

// ==============================
// 速度取得
// ==============================
long Encoder::getSpeed() {
  portENTER_CRITICAL(&_timerMux);
  long s = _speed;
  portEXIT_CRITICAL(&_timerMux);
  return s;
}

// ==============================
// 位置取得
// ==============================
long Encoder::getPosition() {
  portENTER_CRITICAL(&_timerMux);
  long p = _position;
  portEXIT_CRITICAL(&_timerMux);
  return p;
}

// ==============================
// 位置を任意の値に設定
// ==============================
void Encoder::setPosition(long newPos) {
  portENTER_CRITICAL(&_timerMux);
  _position     = newPos;
  _encoderCount = newPos;
  _prevCount    = newPos;
  portEXIT_CRITICAL(&_timerMux);
}
