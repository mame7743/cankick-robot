#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "MotorDriver.h"
#include "Encoder.h"
class StateMachine {
public:
  enum class State : uint8_t { IDLE, BARK, RUN, DECEL, SEARCH, STOPPED, KICK, END };
  void begin(MotorDriver* md, Encoder* enc, State s);
  void update();
private:
  static const char* stateNames[];
  MotorDriver* _motor = nullptr;
  Encoder* _enc = nullptr;
  unsigned long _t0 = 0;
  unsigned long _t1 = 0;
  long long _encStart = 0;
  int _pwm = 0;
  State _st = State::IDLE;
  void change(State s);
  float dis_dist = 0;
  float dist = 0;
  bool minset_flag = false;
};
#endif // STATEMACHINE_H