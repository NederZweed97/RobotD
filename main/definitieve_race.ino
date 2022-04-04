#include <analogWrite.h>

int valueLdrLeft = analogRead(ldrLeft);
int valueLdrRight = analogRead(ldrRight);

void startRace() {
  int valueLdrLeft = analogRead(ldrLeft);
  int valueLdrRight = analogRead(ldrRight);

  if(valueLdrLeft < 120 && valueLdrRight < 120 ) {
    moveForward();
    isDriving = true;
  } 

  if(valueLdrLeft < 120 && valueLdrRight > 120) {
    turnRight();
    isDriving = true;
  } 

  if(valueLdrLeft > 120 && valueLdrRight < 120) {
    turnLeft();
    isDriving = true;
  } 
  
  if (valueLdrLeft > 120 && valueLdrRight > 120){ 
    moveBackwards();
    isDriving = true;
  }
  //finished
  if(valueLdrLeft < 40 && valueLdrRight < 40){
    stopVehicle();
    robotStatus = "finished";
    isDriving = false;
    isFinished = true;
  }
}


void drive(int fL, int fR, int rL, int rR){
  analogWrite(forwardLeft, fL);
  analogWrite(forwardRight, fR);
  analogWrite(reverseLeft, rL);
  analogWrite(reverseRight, rR);
}

void moveForward() {
  drive(235, 235, LOW, LOW);
  bool isDriving = true;
}

void moveBackwards() {
  drive(LOW, LOW, 200, 200);
}

void turnRight() {
  drive(LOW, 225, LOW, LOW);
}

void turnLeft() {
  drive(225, LOW, LOW, LOW);
}

void stopVehicle() {
  drive(LOW, LOW, LOW, LOW);
}
