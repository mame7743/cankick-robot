#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
namespace Pin {
constexpr int PWM = 15;
constexpr int InA = 2;
constexpr int InB = 18;
constexpr int Mode = 4;
constexpr int PSB = 16;
constexpr int BRK = 17;
constexpr int Full = 5;
constexpr int Sensor = 32;    // 停止線センサ
constexpr int Solenoid = 22;  // キック機構
constexpr int EncA = 34;
constexpr int EncB = 35;
constexpr int StartSW = 25;
constexpr int DinosaurBark = 26;
}
namespace Motor {
constexpr int PWM_FREQ = 1000;
constexpr int PWM_RES = 12;  // 0‑4095
constexpr int SPEED_MAX = 3000;
constexpr int SPEED_MIN = 600;
constexpr int ACC_STEP = 120;
constexpr int DEC_STEP = 200;
}
namespace Enc {
constexpr float CPR = 2048.0f * 4;  // 8192 counts/rot
constexpr float WHEEL_D_M = 0.049f;
constexpr float CIRCUMF = PI * WHEEL_D_M;
constexpr float M_PER_CNT = CIRCUMF / CPR;
constexpr float TARGET_DIST = 4.0f;
constexpr float SEARCH_STEP = 1.1f;
}
namespace SensorConst {
constexpr int STOPLINE_THRESHOLD = 100;
}
#endif  // CONFIG_H