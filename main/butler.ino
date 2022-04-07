#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Check for engine startup
bool bFirst = true;

// Millis base values
unsigned long bLastMillis = 0;
unsigned long bLastMillisSearch = 0;
unsigned long bLastMillisReverse = 0;

// booleans to reset the above values
bool bReset = false;
bool bResetSearch = false;
bool bResetReverse = false;

bool bStart = false;

bool bAgain = false;

int bInterval = 0;

void butlerSetup() {
  if (!lox.begin()) {
    while (1); // Infinite loop to stop the program from executing the rest of the code.
  }
}

void startButler() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeMilliMeter >= 300) {
    bDrive();
  } else {
    bBrake();
    bSearch();
  }
}

void bDrive() {
  isDriving = true;
  bResetSearch = true;
  bResetReverse = true;
  if (bStart) {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 140);
    analogWrite(reverseRight, 140);
  } else {
    unsigned long bCurrentMillis = millis();
    if (bReset) {
      bLastMillis = bCurrentMillis;
      bReset = false;
    }
    if (bCurrentMillis - bLastMillis >= 100) {
      bStart = true;
    }
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 200);
    analogWrite(reverseRight, 200);
  }
}

void bBrake() {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
    bStart = false;
    bReset = true;
    isDriving = false;
}

void bTurnRight() {
  analogWrite(forwardLeft, 0);
  analogWrite(forwardRight, 175);
  analogWrite(reverseLeft, 175);
  analogWrite(reverseRight, 0);
}

void bTurnLeft() {
  analogWrite(forwardLeft, 175);
  analogWrite(forwardRight, 0);
  analogWrite(reverseLeft, 0);
  analogWrite(reverseRight, 175);
}

void bSearch() {

  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (bAgain) {
    bInterval = 1800;
    bAgain = false;
  } else {
    bInterval = 600;
  }

  unsigned long bCurrentMillisSearch = millis();
  if (bResetSearch) {
    bLastMillisSearch = bCurrentMillisSearch;
    bResetSearch = false;
  }
  if (bCurrentMillisSearch - bLastMillisSearch >= bInterval) {
    bBrake();
    int bScanOne = measure.RangeMilliMeter;
    if (bScanOne >= 300) {
      bResetSearch = true;
      return;
    } else {
      unsigned long bCurrentMillisReverse = millis();
      if (bResetReverse) {
        bLastMillisReverse = bCurrentMillisReverse;
        bResetReverse = false;
      }
      if (bCurrentMillisReverse - bLastMillisReverse >= 1000) {
        bBrake();
        int bScanTwo = measure.RangeMilliMeter;
        if (bScanTwo >= 300) {
          bResetSearch = true;
          bResetReverse = true;
          return;
        } else {
          bAgain = true;
          bResetSearch = true;
          bResetReverse = true;
          return;
        }
      } else {
        bTurnLeft();
      }
    }
  } else {
    bTurnRight();
  }
}
