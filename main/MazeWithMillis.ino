#include <Arduino.h>
void setupMaze() {
  action = "";
}
 
void startMaze() {
   display.clearDisplay(); 
  int sensorValueL = analogRead(ldrLeft);
  int sensorValueR = analogRead(ldrRight);
  currentTime = millis();
 
 //Displays left and right LDR values on screen.
  display.setCursor(15, 10);
  drawText("R: " + String(sensorValueR), 1);
 
  display.setCursor(15, 20);
  drawText("L: " + String(sensorValueL), 1);
  display.display();

if(action == "correctToLeft"){
  correctToTheLeft();
  return;
}

if(action == "correctToRight"){
  correctToTheRight();
  return;
}

if(action == "uTurn"){
  uTurn();
  return;
}

if(action == "turnRight"){
  turnRight();
  return;
}

if(action == "turnLeft"){
  turnLeft();
  return;
}


  if(sensorValueR > 52 && sensorValueL < greyMin)
  { // CORRECT right
    action = "correctToRight";
  }else if(sensorValueR < 52 && sensorValueL > greyMin)
  { //CORRECT left
    action = "correctToLeft";
  } else if(sensorValueR < 52 && sensorValueL < greyMin) {
    moveForward();
  }else if(sensorValueR > blackMin && sensorValueL > blackMin){
    action = "turnLeft";
  } else if(sensorValueR > blackMin && sensorValueL < 400){//90 turn right
    action = "turnRight"; 
  }else if(sensorValueR < 400 && sensorValueL > blackMin){ //90 turn left
    action = "turnLeft";
  }else if(sensorValueR > 75 && sensorValueR < 400 && sensorValueL > 70 && sensorValueL < 400){
      action = "uTurn";
  }else if(sensorValueL < 15 && sensorValueR < 15){
    stopVehicle();
    robotStatus = "finished";
    isDriving = false;
    isFinished = true;
  }


 
}

// These are the functions that are used in the code above.

void drawText(String value, int delaySeconds) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(value);
}

void drive(int fL, int fR, int rL, int rR){
  analogWrite(forwardLeft, fL);
  analogWrite(forwardRight, fR);
  analogWrite(reverseLeft, rL);
  analogWrite(reverseRight, rR);
}

void moveForward() {
   drive(160, 160, LOW, LOW);
}

void uTurn(){
  currentTime = millis();
  if(timer3 == 0){
    timer3 = millis();
  }
  if(currentTime - timer3< 850){
        drive(LOW, 190, 190, LOW);
  }else if(currentTime - timer3< 1350){
      stopVehicle();
  }else{
    timer3 = 0;
    action  = "";
  }
}

void correctToTheRight() {
  if(timer1 == 0){
    timer1 = millis();
    currentTime = millis();
  }
  if(currentTime - timer1 < 20){
      drive(180, LOW, LOW, LOW);
  }else{
    timer1 = 0;
    action  = "";
  }
}
void correctToTheLeft() {
    if(timer7 == 0){
    timer7 = millis();
    currentTime = millis();
  }
  if(currentTime - timer7 < 20){
      drive(LOW, 180, LOW, LOW);
  }else{
    timer7 = 0;
    action  = "";
  }
}
 
void stopVehicle() {
  drive(LOW, LOW, LOW, LOW);
}
 
void turnRight(){
  if(timer4 == 0){
    timer4 = millis();
    currentTime = millis();
  }
  if(currentTime - timer4 < 40){
      moveForward();
  }else if(currentTime - timer4< 740){
      stopVehicle();
  }else if(currentTime - timer4< 1120){
      moveForward();
  }else if(currentTime - timer4< 1720){
      stopVehicle();
  }else if(currentTime - timer4< 2910){
       drive(170, LOW, LOW, 170);
  }else if(currentTime - timer4< 2960){
       moveForward();
  }else if(currentTime - timer4< 2960){
       stopVehicle();
  }else{
    timer4 = 0;
    action  = "";
  }
}

void turnLeft(){
  if(timer5 == 0){
    timer5 = millis();
    currentTime = millis();
  }
  if(currentTime - timer5 < 35){
      moveForward();
  }else if(currentTime - timer5< 735){
      stopVehicle();
  }else if(currentTime - timer5< 1115){
      moveForward();
  }else if(currentTime - timer5< 1715){
      stopVehicle();
  }else if(currentTime - timer5< 2915){
       drive(LOW, 170, 170, LOW);
  }else if(currentTime - timer5< 2955){
       moveForward();
  }else if(currentTime - timer5< 2955){
       stopVehicle();
  }else{
    timer5 = 0;
    action  = ""; 
  }
}


void turnBack(){
  if(timer6 == 0){
    timer6 = millis();
    currentTime = millis();
  }
  if(currentTime - timer6< 400){
      turnRight();
  }else if (currentTime = timer6 < 900){
    moveForward();
  }else{
    timer6 = 0;
    action  = "";
  }
}