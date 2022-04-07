#include <Arduino.h>

unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long timer4 = 0;
unsigned long timer5 = 0;
unsigned long timer6 = 0;
unsigned long timer7 = 0;
unsigned long currentTime;

int greyMin = 60;
int greyMinR = 50;
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


  if(sensorValueR > 52 && sensorValueL < greyMin)
  { // CORRECT right
    actionMaze = "correctToRight";
  }else if(sensorValueR < 52 && sensorValueL > greyMin)
  { //CORRECT left
    actionMaze = "correctToLeft";
  } else if(sensorValueR < 52 && sensorValueL < greyMin) {
    moveForward(160, 160);
  }else if(sensorValueR > blackMin && sensorValueL > blackMin){
    actionMaze = "turnLeft";
  } else if(sensorValueR > blackMin && sensorValueL < 400){//90 turn right
    actionMaze = "turnRight"; 
  }else if(sensorValueR < 400 && sensorValueL > blackMin){ //90 turn left
    actionMaze = "turnLeft";
  }else if(sensorValueR > 75 && sensorValueR < 400 && sensorValueL > 70 && sensorValueL < 400){
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
  if(currentTime - timer3< 850){
        drive(LOW, 190, 190, LOW);
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
  if(currentTime - timer1 < 20){
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
  if(currentTime - timer7 < 20){
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
  if(currentTime - timer4 < 40){
      moveForward(160, 160);
  }else if(currentTime - timer4< 740){
      stopVehicle();
  }else if(currentTime - timer4< 1120){
      moveForward(160, 160);
  }else if(currentTime - timer4< 1720){
      stopVehicle();
  }else if(currentTime - timer4< 2910){
       drive(170, LOW, LOW, 170);
  }else if(currentTime - timer4< 2960){
       moveForward(160, 160);
  }else if(currentTime - timer4< 2960){
       stopVehicle();
  }else{
    timer4 = 0;
    actionMaze  = "";
  }
}

void turnLeftMaze(){
  if(timer5 == 0){
    timer5 = millis();
    currentTime = millis();
  }
  if(currentTime - timer5 < 35){
      moveForward(160, 160);
  }else if(currentTime - timer5< 735){
      stopVehicle();
  }else if(currentTime - timer5< 1115){
      moveForward(160, 160);
  }else if(currentTime - timer5< 1715){
      stopVehicle();
  }else if(currentTime - timer5< 2915){
       if(sensorValueL > blackMin){
        moveForward(160,160);
       }else{
          drive(LOW, 170, 170, LOW);
       }
  }else if(currentTime - timer5< 2955){
       moveForward(160, 160);
  }else if(currentTime - timer5< 2955){
       stopVehicle();
  }else{
    timer5 = 0;
    actionMaze  = ""; 
  }
}


void turnBackMaze(){
  if(timer6 == 0){
    timer6 = millis();
    currentTime = millis();
  }
  if(currentTime - timer6< 400){
      turnRightMaze();
  }else if (currentTime = timer6 < 900){
    moveForward(160, 160);
  }else{
    timer6 = 0;
    actionMaze  = "";
  }
}
