#include <analogWrite.h>

void startRace() {
  int valueLdrLeft = analogRead(ldrLeft);
  int valueLdrRight = analogRead(ldrRight);

  if(valueLdrLeft < 120 && valueLdrRight < 120 ) {
    moveForward(235, 235);
    isDriving = true;
  } 

  if(valueLdrLeft < 120 && valueLdrRight > 120) {
    turnRight(205);
    isDriving = true;
  } 

  if(valueLdrLeft > 120 && valueLdrRight < 120) {
    turnLeft(205);
    isDriving = true;
  } 
  
  if (valueLdrLeft > 120 && valueLdrRight > 120){ 
    moveBackwards(200, 200);
    isDriving = true;
  }
  //finished
  if(valueLdrLeft < 40 && valueLdrRight < 40){
    stopVehicle();
    finishGame();
  }
}
