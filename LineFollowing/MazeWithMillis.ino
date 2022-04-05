//This code makes the robot drive over thin lines. 
// On the display the values that the LDR picks up will be shown. These values are used to see if the robot follows the line. 
// This robot uses the left-handed stragety when goeing through a maze. put a delay in de left turn.
#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
 
int sensorValueLeft = 39;
int sensorValueRight = 34;
int forwardLeft = 16;
int forwardRight = 18;
int reverseLeft = 17;
int reverseRight = 5;
int greyMin = 60;
int greyMinR = 50;
int greyMax = 300;
int blackMin = 1000;
unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long timer4 = 0;
unsigned long timer5 = 0;
unsigned long timer6 = 0;
unsigned long currentTime;
String action = "";

void setup() {
  Serial.begin(9600);
  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(sensorValueLeft, INPUT);
  pinMode(sensorValueRight, INPUT);
  action = "";
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Infinite loop to stop the program from executing the rest of the code.
  }
 
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  display.clearDisplay();
//Displays left and right LDR values on screen.
 display.setCursor(5, 0);
drawText("INF1D",4);

  display.display();
  delay(2000);

 
}
 
void loop() {
  int sensorValueL = analogRead(sensorValueLeft);
  int sensorValueR = analogRead(sensorValueRight);
  currentTime = millis();
 
 //Displays left and right LDR values on screen.
  display.setCursor(5, 10);
  drawText("R: " + String(sensorValueR), 1);
 
  display.setCursor(15, 20);
  drawText("L: " + String(sensorValueL), 1);
  display.display();

if(action == "correctToLeft"){
  correctToTheLeft();
}

if(action == "correctToRight"){
  correctToTheRight();
}

if(action == "uTurn"){
  uTurn();
}

if(action == "turnRight"){
  turnRight();
}

if(action == "turnLeft"){
  turnLeft();
}


  if(sensorValueR < 60 && sensorValueL > 60)
  { // CORRECT left
    action = "correctToLeft";
  }else if(sensorValueR > 60 && sensorValueL < 60)
  { //CORRECT right
    action == "correctToRight";
  } else if(sensorValueR < 60 && sensorValueL < 60) {
    moveForward();
  }else if(sensorValueR > 900 && sensorValueL > 900)
  {
    stopVehicle();
  }else if(sensorValueR > 900 && sensorValueL < 60){//90 turn right
    action = "turnRight"; 
  }else if(sensorValueR < 400 && sensorValueL > 900){ //90 turn left
    action = "turnLeft";  
  }else if(sensorValueR > 60 && sensorValueR < 400 && sensorValueL > 60 && sensorValueL < 400){
      action = "uTurn";
  }

  //sensorValueR > 50 && sensorValueR < 300 && sensorValueL > greyMin && sensorValueL < 300
  display.clearDisplay(); 
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
  if(currentTime - timer1< 20){
      drive(190, LOW, LOW, LOW);
  }else{
    timer1 = 0;
    action  = "";
  }
}
void correctToTheLeft() {
    if(timer1 == 0){
    timer1 = millis();
    currentTime = millis();
  }
  if(currentTime - timer1 < 20){
      drive(LOW, 190, LOW, LOW);
  }else{
    timer1 = 0;
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
  }else if(currentTime - timer4< 920){
      moveForward();
  }else if(currentTime - timer4< 1620){
      stopVehicle();
  }else if(currentTime - timer4< 1627){
    drive(190, LOW, LOW, 190);
  }
  else if(currentTime - timer4< 2817){
       drive(175, LOW, LOW, 175);
  }else if(currentTime - timer4< 2867){
       moveForward();
  }else if(currentTime - timer4< 2867){
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
  }else if(currentTime - timer5< 915){
      moveForward();
  }else if(currentTime - timer5< 1615){
      stopVehicle();
  }else if(currentTime - timer5< 2105){
       drive(185, LOW, LOW, 185);
  }else if(currentTime - timer5< 2155){
       moveForward();
  }else if(currentTime - timer5< 2655){
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