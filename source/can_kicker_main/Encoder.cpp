#include "Encoder.h"
volatile long long Encoder::_counts = 0;
volatile int Encoder::_lastEncoded = 0;
void Encoder::begin() {
  pinMode(Pin::EncA, INPUT_PULLUP);
  pinMode(Pin::EncB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Pin::EncA), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Pin::EncB), ISR_B, CHANGE);
}
void Encoder::reset() {
  noInterrupts();
  _counts = 0;
  interrupts();
}
float Encoder::meters() const {
  return static_cast<float>(_counts) * Enc::M_PER_CNT;
}
void Encoder::ISR_A() {
  int MSB = digitalRead(Pin::EncA);
  int LSB = digitalRead(Pin::EncB);
  int encoded = (MSB << 1) | LSB;
  int sum = (_lastEncoded << 2) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) Encoder::_counts++;
  else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) Encoder::_counts--;
  _lastEncoded = encoded;
}
void Encoder::ISR_B() {
  ISR_A();
}
