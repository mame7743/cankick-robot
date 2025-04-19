#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "MotorDriver.h"
#include "Encoder.h"
class StateMachine {
public:
  enum class State : uint8_t { IDLE, BARK, RUN, DECEL, SEARCH, STOPPED, KICK, END };
  void begin(MotorDriver* md, Encoder* enc);
  void update();
private:
  MotorDriver* _motor = nullptr;
  Encoder* _enc = nullptr;
  unsigned long _t0 = 0;
  long long _encStart = 0;
  int _pwm = 0;
  State _st = State::IDLE;
  void change(State s);
};
#endif // STATEMACHINE_H