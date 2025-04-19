#include "Config.h"
#include "MotorDriver.h"
#include "Encoder.h"
#include "StateMachine.h"
MotorDriver motor;
Encoder encoder;
StateMachine fsm;
void setup() {
  Serial.begin(115200);
  motor.begin();  // 方向を変える場合: motor.begin(MotorDriver::Dir::REV);
  encoder.begin();
  encoder.reset();
  pinMode(Pin::Sensor, INPUT);
  pinMode(Pin::Solenoid, OUTPUT);
  pinMode(Pin::StartSW, INPUT_PULLUP);
  pinMode(Pin::DinosaurBark, OUTPUT);
  digitalWrite(Pin::Solenoid, LOW);
  digitalWrite(Pin::DinosaurBark, HIGH);
  fsm.begin(&motor, &encoder);
}
void loop() {
  fsm.update();
}
