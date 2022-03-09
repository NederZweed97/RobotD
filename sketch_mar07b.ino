//This code makes the robot drive over thin lines. 
// On the display the values that the LDR picks up will be shown. These values are used to see if the robot follows the line. 

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
 
int ldrLeft = 34;
int ldrRight = 39;
int forwardLeft = 18;
int forwardRight = 16;
int reverseLeft = 5;
int reverseRight = 17;
 
void setup() {
  Serial.begin(9600);
 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Infinite loop to stop the program from executing the rest of the code.
  }
 
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
 
  // Clear the buffer
  display.clearDisplay();
 
  display.display();
  delay(2000);

  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(ldrLeft, INPUT);
  pinMode(ldrRight, INPUT);
}
 
void loop() {
  int valueLdrLeft = analogRead(ldrLeft);
  int valueLdrRight = analogRead(ldrRight);
 
  display.setCursor(15, 10);
  drawText("Left: " + String(valueLdrRight), 1);
 
  display.setCursor(15, 20);
  drawText("Right: " + String(valueLdrLeft), 1);
  display.display();

  if(valueLdrLeft > 65 && valueLdrRight > 65) {
     moveForward();
 
  } else if(valueLdrLeft > 64 && valueLdrRight < 40) {
    turnLeft();
 
  } else if(valueLdrRight < 40 && valueLdrLeft < 60) {
    turnRight();  
 
  } else {
    stopVehicle();
 
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
  analogWrite(forwardRight, 170);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
void turnRight() {
  analogWrite(forwardLeft, LOW);
  analogWrite(forwardRight, 190);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
void turnLeft() {
  analogWrite(forwardLeft, 190);
  analogWrite(forwardRight, LOW);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
void stopVehicle() {
  analogWrite(forwardLeft, LOW);
  analogWrite(forwardRight, LOW);
  analogWrite(reverseLeft, LOW);
  analogWrite(reverseRight, LOW);
}
 
