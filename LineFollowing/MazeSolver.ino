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
int greyMin = 80;
int greyMax = 300;
int blackMin = 1000;
boolean turnValueLeft = false;
boolean turnValueRight = false;

void setup() {
  Serial.begin(9600);
  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(sensorValueLeft, INPUT);
  pinMode(sensorValueRight, INPUT);
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

 
 //Displays left and right LDR values on screen.
  display.setCursor(5, 10);
  drawText("R: " + String(sensorValueR), 1);
 
  display.setCursor(15, 20);
  drawText("L: " + String(sensorValueL), 1);
  display.display();
  if(sensorValueR > blackMin && sensorValueL < greyMax or turnValueLeft == true){//90 turn left
    turnValueLeft = true;
    turnRight();
    turnValueLeft = makeFalse();
  }else if(sensorValueR < greyMax && sensorValueL > blackMin or turnValueRight == true){ //90 turn right
    turnValueRight = true;
    Serial.print(turnValueRight);
    turnLeft();
    turnValueRight = makeFalse();
    Serial.print(turnValueRight);
  }else if(sensorValueR < greyMin && sensorValueL > greyMin){ // CORRECT left
    correctToTheLeft();
  }else if(sensorValueR > greyMin && sensorValueL < greyMin){ //CORRECT right
    correctToTheRight();
  }else if(sensorValueR < greyMin && sensorValueL < greyMin){
    moveForward();
  }else if(sensorValueR > 3000 && sensorValueL > 3000){
    stopVehicle();
  }else if(sensorValueR > greyMin && sensorValueR < greyMax && sensorValueL > greyMin && sensorValueL < greyMax){
    uTurn();
  }
  display.clearDisplay();
}

// These are the functions that are used in the code above.

void drawText(String value, int delaySeconds) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(value);
}
 
void moveForward() {
  analogWrite(forwardLeft, 170);
  analogWrite(forwardRight, 166);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
void correctToTheLeft() {
  analogWrite(forwardLeft, LOW);
  analogWrite(forwardRight, 180);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
  delay(40);
  stopVehicle();
}
 
void correctToTheRight() {
  analogWrite(forwardLeft, 180);
  analogWrite(forwardRight, LOW);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
  delay(40);
  stopVehicle();
}
 
void stopVehicle() {
  analogWrite(forwardLeft, LOW);
  analogWrite(forwardRight, LOW);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
boolean turnRight(){
  stopVehicle();
  delay(700);
  moveForward();
  delay(200);
  analogWrite(forwardLeft, 180);
  analogWrite(forwardRight, LOW);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, 180);
  delay(500);
  moveForward();
  delay(100);
  stopVehicle();
  delay(500);
  return false;
}
boolean turnLeft(){
  analogWrite(forwardLeft, LOW);
  analogWrite(forwardRight, 180);
  analogWrite(reverseLeft, 180);
  analogWrite(reverseRight, LOW);
  delay(400);
  return false;
}

boolean makeFalse(){
  return false;
}
void uTurn(){
    turnLeft();
  delay(280);
  stopVehicle();
  delay(500);
  
}
