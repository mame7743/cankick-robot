#ifndef ENCODER_H
#define ENCODER_H
#include "Config.h"
class Encoder {
public:
  void begin();
  float meters() const;
  void reset();
  static volatile long long _counts;
private:
  static void ISR_A();
  static void ISR_B();
  static volatile int _lastEncoded;
};
#endif
