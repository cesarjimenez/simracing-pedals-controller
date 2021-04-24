#include <Adafruit_ADS1015.h>
#include <Joystick.h>

const int THROTTLE_INPUT = 0;
const int BRAKE_INPUT = 1;
const int CLUTCH_INPUT = 2;

Adafruit_ADS1115 ads;
Joystick_ joystick = Joystick_(
                       JOYSTICK_DEFAULT_REPORT_ID, // hidReportId
                       JOYSTICK_TYPE_JOYSTICK,     // joystickType
                       0,                          // buttonCount
                       0,                          // hatSwitchCount
                       true,                       // includeXAxis
                       true,                       // includeYAxis
                       true,                       // includeZAxis
                       false,                      // includeRxAxis
                       false,                      // includeRyAxis
                       false,                      // includeRzAxis
                       false,                      // includeRudder
                       false,                      // includeThrottle
                       false,                      // includeAccelerator
                       false,                      // includeBrake
                       false);                     // includeSteering

uint16_t throttle;
uint16_t brake;
uint16_t clutch;

void setup() {

  // Init ADS
  ads.setGain(GAIN_ONE);
  ads.begin();

  // Init Joystick
  joystick.setXAxisRange(0, 32760); // 2^15 ASD has 15 bits resolution
  joystick.setYAxisRange(0, 32760);
  joystick.setZAxisRange(0, 32760);
  joystick.begin(true);
}

void loop() {

  // initAutoSendState is set to true, so brake has precedence! REVIEW
  brake = readValueFromAds(BRAKE_INPUT, 2, 6000);
  joystick.setYAxis(brake);

  throttle = readValueFromAds(THROTTLE_INPUT, 2, 1000);
  if (throttle > 22500) {
    throttle =  22500;
  }
  joystick.setXAxis(throttle);

  clutch = readValueFromAds(CLUTCH_INPUT, 2, 5500);
  if (clutch > 3000) {
    clutch =  6000;
  }
  joystick.setZAxis(clutch);
}

int16_t readValueFromAds(int input, int num_measures, int offset) {
  int16_t value = 0;
  int16_t tmp = 0;
  for (int i = 0; i < num_measures; i++) {
    tmp = ads.readADC_SingleEnded(input);
    tmp = tmp / num_measures;
    value += tmp;
  }
  value = value - offset;
  if (value < 0) {
    value = 0;
  }
  return value;
}
