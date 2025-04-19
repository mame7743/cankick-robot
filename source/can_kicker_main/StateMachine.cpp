#include "StateMachine.h"
#include "Config.h"
#include <Arduino.h>
void StateMachine::begin(MotorDriver* md, Encoder* enc) {
  _motor = md;
  _enc = enc;
  _t0 = millis();
  _motor->brake(false);
}
void StateMachine::change(State s) {
  _st = s;
  _t0 = millis();
  Serial.print("[STATE] -> ");
  Serial.println((int)s);
}
void StateMachine::update() {
  float dist = _enc->meters();
  int sensor = analogRead(Pin::Sensor);
  unsigned long t = millis() - _t0;
  switch (_st) {
    case State::RUN:
      if (_pwm < Motor::SPEED_MAX) {
        _pwm += Motor::ACC_STEP;
      }
      _motor->setSpeed(_pwm);
      if (dist >= Enc::TARGET_DIST) {
        change(State::DECEL);
      }
      break;
    case State::DECEL:
      if (_pwm > Motor::SPEED_MIN) {
        _pwm -= Motor::DEC_STEP;
        _motor->setSpeed(_pwm);
      } else {
        _pwm = Motor::SPEED_MIN;
        _motor->setSpeed(_pwm);
        _encStart = Encoder::_counts;
        change(State::SEARCH);
      }
      break;
    case State::SEARCH:
      {
        _motor->setSpeed(_pwm);
        if (sensor < SensorConst::STOPLINE_THRESHOLD) {
          _motor->setSpeed(0);
          _motor->brake(true);
          change(State::STOPPED);
          break;
        }
        float moved = (_enc->_counts - _encStart) * Enc::M_PER_CNT;
        if (moved >= Enc::SEARCH_STEP) {
          _motor->setSpeed(0);
          _motor->brake(true);
          change(State::STOPPED);
        }
        break;
      }
    case State::STOPPED:
      _motor->setSpeed(0);
      _motor->brake(true);
      change(State::KICK);
      break;
    case State::KICK:
      if (t >= 1000 && t < 3000) digitalWrite(Pin::Solenoid, HIGH);
      else if (t >= 3000) {
        digitalWrite(Pin::Solenoid, LOW);
        change(State::END);
      }
      break;
    case State::END:
      _motor->setSpeed(0);
      _motor->brake(true);
      break;
  }
}