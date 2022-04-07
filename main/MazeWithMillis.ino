#include <Arduino.h>

unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long timer4 = 0;
unsigned long timer5 = 0;
unsigned long timer6 = 0;
unsigned long timer7 = 0;
unsigned long currentTime;

int greyMin = 75;
int greyMinR = 80;
int greyMax = 300;
int blackMin = 1000;

String actionMaze = "";

void startMaze() {
   display.clearDisplay(); 
  int sensorValueL = analogRead(ldrRight);
  int sensorValueR = analogRead(ldrLeft);
  currentTime = millis();

if(actionMaze == "correctToLeft"){
  correctToTheLeft();
  return;
}

if(actionMaze == "correctToRight"){
  correctToTheRight();
  return;
}

if(actionMaze == "uTurn"){
  uTurn();
  return;
}

if(actionMaze == "turnRight"){
  turnRightMaze();
  return;
}

if(actionMaze== "turnLeft"){
  turnLeftMaze();
  return;
}


  if(sensorValueR > greyMinR && sensorValueL < greyMin)
  { // CORRECT right
    actionMaze = "correctToRight";
  }else if(sensorValueR < greyMinR && sensorValueL > greyMin)
  { //CORRECT left
    actionMaze = "correctToLeft";
  } else if(sensorValueR < greyMinR && sensorValueL < greyMin) {
    moveForward(155, 155);
  }else if(sensorValueR > blackMin && sensorValueL > blackMin){
    actionMaze = "turnLeft";
  } else if(sensorValueR > blackMin && sensorValueL < 400){//90 turn right
    actionMaze = "turnRight"; 
  }else if(sensorValueR < 400 && sensorValueL > blackMin){ //90 turn left
    actionMaze = "turnLeft";
  }else if(sensorValueR > greyMinR && sensorValueR < 400 && sensorValueL > greyMin && sensorValueL < 400){
      actionMaze = "uTurn";
  }else if(sensorValueL < 15 && sensorValueR < 15){
    stopVehicle();
    robotStatus = "finished";
    isDriving = false;
    isFinished = true;
  }
}

// These are the functions that are used in the code above.

void drawText(String value) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(value);
}

void uTurn(){
  currentTime = millis();
  if(timer3 == 0){
    timer3 = millis();
  }
  if(currentTime - timer3< 1200){
        drive(LOW, 180, 180, LOW);
  }else if(currentTime - timer3< 1350){
      stopVehicle();
  }else{
    timer3 = 0;
    actionMaze  = "";
  }
}

void correctToTheRight() {
  if(timer1 == 0){
    timer1 = millis();
    currentTime = millis();
  }
  if(currentTime - timer1 < 15){
      drive(170, LOW, LOW, LOW);
  }else{
    timer1 = 0;
    actionMaze  = "";
  }
}
void correctToTheLeft() {
    if(timer7 == 0){
    timer7 = millis();
    currentTime = millis();
  }
  if(currentTime - timer7 < 15){
      drive(LOW, 170, LOW, LOW);
  }else{
    timer7 = 0;
    actionMaze  = "";
  }
}
 
void turnRightMaze(){
  if(timer4 == 0){
    timer4 = millis();
    currentTime = millis();
  }
  if(currentTime - timer4 < 30){
      moveForward(155, 155);
  }else if(currentTime - timer4< 730){
      stopVehicle();
  }else if(currentTime - timer4< 1110){
      moveForward(155, 155);
  }else if(currentTime - timer4< 1710){
      stopVehicle();
  }else if(currentTime - timer4< 2745){
       drive(170, LOW, LOW, 170);
  }else if(currentTime - timer4< 2805){
       stopVehicle();
  }else{
    timer4 = 0;
    actionMaze  = "";
  }
}

void turnLeftMaze(){
  int sensorValueL = analogRead(sensorValueLeft);
  int sensorValueR = analogRead(sensorValueRight);
  if(timer5 == 0){
    timer5 = millis();
    currentTime = millis();
  }
  if(currentTime - timer5 < 20){
      moveForward(155, 155);
  }else if(currentTime - timer5< 720){
      stopVehicle();
  }else if(currentTime - timer5< 1000){
      moveForward(155, 155);
  }else if(currentTime - timer5< 1700){
      stopVehicle();
  }else if(currentTime - timer5< 2105 and sensorValueL > blackMin){
        moveForward(155, 155);
  }else if(currentTime - timer5< 2695){
       if(currentTime - timer5< 2110){
        moveForward(155, 155);
       }else{
       drive(LOW, 170, 170, LOW);
       }
  }else if(currentTime - timer5< 2755){
       moveForward(155, 155);
  }else{
    timer5 = 0;
    actionMaze  = ""; 
  }
}
