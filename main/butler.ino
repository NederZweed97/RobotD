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

bool bFinishable = false;

int bInterval = 0;

void butlerSetup() {
  if (!lox.begin()) {
    while (1); // Infinite loop to stop the program from executing the rest of the code.
  }
}

void startButler() {
  if (analogRead(ldrLeft) > 1500 && analogRead(ldrRight) > 1500) {
    if (bFinishable) {
      robotStatus = "finished";
    isDriving = false;
    isFinished = true;
    }
  } else {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeMilliMeter >= 300) {
      bDrive();
    } else {
      bBrake();
      bSearch();
    }
  }
}

void bDrive() {
  isDriving = true;
  bResetSearch = true;
  bResetReverse = true;
  if (bStart) {
    drive(0, 0, 140, 140);
  } else {
    unsigned long bCurrentMillis = millis();
    if (bReset) {
      bLastMillis = bCurrentMillis;
      bReset = false;
    }
    if (bCurrentMillis - bLastMillis >= 100) {
      bStart = true;
    }
    drive(0, 0, 200, 200);
  }
}

void bBrake() {
  if (isDriving) {
    stopVehicle();
    bStart = false;
    bReset = true;
    isDriving = false;
  }
}

void bSearch() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (!bFinishable) {
    bFinishable = true;
  }
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
        drive(175, 0, 0, 175);
      }
    }
  } else {
    drive(0, 175, 175, 0);
  }
}
