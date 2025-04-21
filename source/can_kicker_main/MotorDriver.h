#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H
#include "Config.h"
class MotorDriver {
public:
  enum class Dir : uint8_t { FWD, REV };
  void begin(Dir dir = Dir::FWD);      // 初期化 + 方向設定
  void setSpeed(int duty);             // 0‑4095
  void brake(bool on);
  void setDirection(Dir dir);          // 途中で反転可能
  Dir  direction() const { return _dir; }
  void releasePoweSave();
  void reverseBrake(int ms);
private:
  int _duty = 0;
  Dir _dir  = Dir::FWD;
  void _applyDir();
};
#endif // MOTORDRIVER_H