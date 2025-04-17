// AMT102


const int pinA = 34;  // A相
const int pinB = 35;  // B相

volatile long encoderValue = 0;
volatile int lastEncoded = 0;

void IRAM_ATTR updateEncoder() {
  int MSB = digitalRead(pinA);  // 現在のA相の状態
  int LSB = digitalRead(pinB);  // 現在のB相の状態

  // 2ビットの組み合わせにまとめる
  int encoded = (MSB << 1) | LSB;
  // 前回の状態と合わせて4ビットにして回転方向を判定する
  int sum = (lastEncoded << 2) | encoded;

  // 以下のパターンでエンコーダのカウントを増やす
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
  }
  // 以下のパターンでカウントを減らす
  else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--;
  }

  lastEncoded = encoded; // 状態を更新
}

void setup() {
  Serial.begin(115200);

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), updateEncoder, CHANGE);

  Serial.println("Encoder test start");
}

void loop() {
  // カウント値を表示
  // 必要に応じて 角度や回転数に換算するなどして使う
  Serial.print("Encoder Value: ");
  Serial.println(encoderValue);

  delay(100); // 更新頻度調整
}
