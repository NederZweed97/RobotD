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

bool bGo = true; //will only drive if true
bool bStart = false; //will check if engines have startboosted
bool bAgain = false; //Will turn true if first search gave no results
bool bFinishable = false; //Will turn true after first search
int bInterval = 0; //search turn interval

void butlerSetup() {
  if (!lox.begin()) {
    while (1); // Infinite loop to stop the program from executing the rest of the code.
  }
  // reset to inital values when preparing
  bFirst = true;
  bLastMillis = 0;
  bLastMillisSearch = 0;
  bLastMillisReverse = 0;
  bReset = false;
  bResetSearch = false;
  bResetReverse = false;
  bGo = true;
  bStart = false;
  bAgain = false;
  bFinishable = false;
  bInterval = 0;
}

void startButler() {
  if (analogRead(ldrLeft) > 1500 && analogRead(ldrRight) > 1500) {
    if (bFinishable) {
      stopVehicle();
      robotStatus = "finished";
      isDriving = false;
      isFinished = true;
    }
  } else {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (bGo) {
      if (measure.RangeMilliMeter >= 340) {
        bDrive();
      } else {
        bGo = false;
        bBrake();
        bSearch();
      }
    } else {
      bSearch();
    }
  }
}

void bDrive() {
  isDriving = true;
  bResetSearch = true;
  bResetReverse = true;
  if (bStart) {
    drive(0, 0, 155, 155);
  } else {
    unsigned long bCurrentMillis = millis();
    if (bReset) {
      bLastMillis = bCurrentMillis;
      bReset = false;
    }
    if (bCurrentMillis - bLastMillis >= 100) {
      bStart = true;
    }
    drive(0, 0, 210, 210);
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
    bInterval = 825;
    bAgain = false;
  } else {
    bInterval = 275;
  }
  unsigned long bCurrentMillisSearch = millis();
  if (bResetSearch) {
    bLastMillisSearch = bCurrentMillisSearch;
    bResetSearch = false;
  }
  if (bCurrentMillisSearch - bLastMillisSearch >= bInterval) {
    bBrake();
    int bScanOne = measure.RangeMilliMeter;
    if (bScanOne >= 340) {
      bGo = true;
      bResetSearch = true;
      return;
    } else {
      unsigned long bCurrentMillisReverse = millis();
      if (bResetReverse) {
        bLastMillisReverse = bCurrentMillisReverse;
        bResetReverse = false;
      }
      if (bCurrentMillisReverse - bLastMillisReverse >= bInterval*2.6) {
        bBrake();
        int bScanTwo = measure.RangeMilliMeter;
        if (bScanTwo >= 340) {
          bGo = true;
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
        drive(200, 0, 0, 200);
      }
    }
  } else {
    drive(0, 200, 200, 0);
  }
}
