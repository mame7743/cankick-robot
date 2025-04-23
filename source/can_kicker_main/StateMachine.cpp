#include "esp32-hal.h"
#include "HardwareSerial.h"
#include "esp32-hal-gpio.h"
#include "StateMachine.h"
#include "Config.h"
#include <Arduino.h>

const char* StateMachine::stateNames[] = {
  "IDLE", "BARK", "RUN", "DECEL", "SEARCH", "STOPPED", "KICK", "END"
};

void StateMachine::begin(MotorDriver* md, Encoder* enc, State s) {
  _motor = md;
  _enc = enc;
  change(s);
  _t0 = millis();
  _motor->brake(false);
}
void StateMachine::change(State s) {
  _st = s;
  _t0 = millis();
  Serial.print("[STATE] -> ");
  Serial.println(stateNames[static_cast<uint8_t>(s)]);
}
void StateMachine::update() {
  dis_dist = _enc->meters() - dist;
  dist = _enc->meters();
  int sensor = digitalRead(Pin::LineSensor);
  unsigned long t = millis() - _t0;
  switch (_st) {
    case State::IDLE:
      _motor->setSpeed(0);
      _motor->brake(true);
      if(!digitalRead(Pin::StartSW)){
        _motor->releasePoweSave();
        change(State::BARK);
      }
      break;
    case State::BARK:
      // digitalWrite(Pin::DinosaurBark, LOW); //吠え初め
      // delay(3000);
      // digitalWrite(Pin::DinosaurBark, HIGH); //一度Lowにする
      // delay(10);
      // digitalWrite(Pin::DinosaurBark, LOW); //吠え終わり
      change(State::RUN);
      // change(State::SEARCH);
      _t1 = millis();
      _motor->brake(false);
      break;
    case State::RUN:
      if (_pwm < Motor::SPEED_MAX) {
        _pwm += Motor::ACC_STEP;
        // if(!minset_flag && dis_dist > 0.00f){
        //   Motor::SPEED_MIN = _pwm + 10;
        // }
      }
      // else{
      //   if(dis_dist == 0.f){
      //     change(State::DECEL);
      //   }
      // }
      _motor->setSpeed(_pwm);
      // if (dist >= Enc::TARGET_DIST || dist <= -Enc::TARGET_DIST) {
      if ((millis() - _t1) > 1000) {
        change(State::DECEL);
        // change(State::STOPPED);
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
        // _motor->brake(true);
        // delay(10);
      }
      break;
    case State::SEARCH:
      {
        _motor->brake(false);
        // if(!minset_flag){
        //   if(dis_dist > 0.00f){
        //   // if(dis_dist > 0.00001f){
        //     _pwm -= 1;
        //   }else{
        //     _pwm += 1;
        //     // minset_flag = true;
        //   }
        // // if (_pwm > Motor::SPEED_MIN) {
        // //   _pwm = Motor::DEC_STEP;
        // //  }
        //   Serial.println(_pwm);
          delay(1);
        // }
        _motor->setSpeed(_pwm);
        // if (sensor < SensorConst::STOPLINE_THRESHOLD) {
        if (!sensor) {
          _motor->setSpeed(0);
          _motor->brake(true);
          change(State::STOPPED);
          break;
        }
        // float moved = (_enc->_counts - _encStart) * Enc::M_PER_CNT;
        // if (moved >= Enc::SEARCH_STEP || moved <= -Enc::SEARCH_STEP) {
        //   _motor->setSpeed(0);
        //   _motor->brake(true);
        //   change(State::STOPPED);
        // }
        break;
      }
    case State::STOPPED:
      _motor->setSpeed(0);
      _motor->brake(true);
      _encStart = _enc->_counts;
      change(State::KICK);
      break;
    case State::KICK:
      if (t >= 2000) {
        digitalWrite(Pin::Solenoid, HIGH);
        change(State::END);
      }
      break;
    case State::END:
      _motor->setSpeed(0);
      _motor->brake(true);
      Serial.print("moved: ");
      Serial.println((_enc->_counts - _encStart) * Enc::M_PER_CNT);
      if(t >= 10000){
        digitalWrite(Pin::Solenoid, LOW);
      }
      break;
  }
}