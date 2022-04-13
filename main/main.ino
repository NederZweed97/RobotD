#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
#define SSD1306_NO_SPLASH;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// Wifi information here.
const char* ssid = "Wifi is saai";
const char* password = "2much4u2day";

// Websocket information here.
const char host[] = "battlebot.oohnoo.nl";
const int port = 3003;

WiFiClient client;
WebSocketsClient webSocket;
StaticJsonDocument<96> doc;

// Robot state
bool isLoggedIn = false;
bool isPreparing = false;
bool isStarting = false;
bool isFinished = false;

// These values have to be< update by the game script.
String currentGame = "";
String robotStatus = "preparing";
bool isDriving = false;
int acceleration = 0;

unsigned long previousMillis = 0;
const long interval = 5000;


int ldrLeft = 39;
int ldrRight = 34;
int forwardLeft = 18;
int forwardRight = 16;
int reverseLeft = 5;
int reverseRight = 17;



String action = "ready";

void setup() {
  analogWrite(reverseLeft, LOW);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop to stop the program from executing the rest of the code.
  }
  display.clearDisplay();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    drawText("Connecting...", true, 10, 10 , 1);
  }

  drawText("WiFi connected", true, 10, 9, 1);
  drawText("IP:" + WiFi.localIP().toString(), false, 0, 20, 1);
  display.startscrollleft(2, 3);
  display.display();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP().toString());

  // server address, port and URL
  webSocket.begin(host, port, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(5000, 5000, 2);



  //butler setup
  butlerSetup();

  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(ldrLeft, INPUT);
  pinMode(ldrRight, INPUT);
}

void loop() {
  webSocket.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    doc.clear();
    doc["status"] = robotStatus;
    doc["isDriving"] = isDriving;
    doc["acceleration"] = acceleration;

    char statusUpdate[100];
    serializeJsonPretty(doc, statusUpdate);

    webSocket.sendTXT(statusUpdate);
    if (robotStatus == "finished") {
      robotStatus = "ready";
    }
  }

  if (isPreparing) {
    doc.clear();
    doc["status"] = true;
    doc["game"] = currentGame;

    char prepStatus[100];
    serializeJsonPretty(doc, prepStatus);

    webSocket.sendTXT(prepStatus);
    robotStatus = "ready";
    isPreparing = false;
  }

  if (isStarting) {
    startGame();
  }
}

// Draw text to the arduino display.
void drawText(String tekst, bool clearDisp, int x, int y, int textSize) {
  if (clearDisp) {
    display.clearDisplay();
  }

  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(tekst);
}

// Websocket event handler executes everytime upon a new message between server and client.
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Disconnected!");

      isLoggedIn = false;
      break;

    case WStype_CONNECTED:
      Serial.println("[WS] Connected");
      robotStatus = "ready";
      doc.clear();
      doc["action"] = "login";
      doc["id"] = WiFi.macAddress();
      doc["status"] = "ready";
      char data[100];
      serializeJsonPretty(doc, data);

      webSocket.sendTXT(data);
      break;

    case WStype_TEXT:
      Serial.println("[WS] server says: " + String((char * )payload));

      if (!isLoggedIn) {
        handleMessage(payload, 1);
      } else if (!isPreparing) {
        handleMessage(payload, 2);
      }
      break;

    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

// Handles the message that the server is sending towards the robot.
void handleMessage(uint8_t *payload, int stageNumber) {
  const uint8_t size = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<size> json;
  DeserializationError err = deserializeJson(json, payload);

  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
    return;
  }

  // Logged in stage
  if (stageNumber == 1) {
    bool loggedin = json["loggedin"];

    if (loggedin) {
      isLoggedIn = true;
    }
  }

  // Preparing/Start/Ending stage
  if (stageNumber == 2) {
    String action = json["action"];
    String game = json["game"];

    if (action == "prepare") {
      // Prepare the game here.
      isPreparing = true;
      currentGame = game;
      robotStatus = "preparing_game";
      if (currentGame == "butler") {
        butlerSetup();
      }
    } else if (action == "start" && game == currentGame) {
      isStarting = true;
    } else if (action == "ended") {
      stopGame();
    }
  }
}

void startGame() {
  if (currentGame == "maze") {
    robotStatus = "in_game";
    startMaze();
  }

  if (currentGame == "race") {
    robotStatus = "in_game";
    startRace();
  }

  if (currentGame == "butler") {
    robotStatus = "in_game";
    startButler();
  }
}

void stopGame() {
  robotStatus = "stopped";
  currentGame = "";
  acceleration = 0;
  isStarting = false;
  isDriving = false;
  isFinished = false;
  isPreparing = false;
  stopVehicle();
}

void finishGame() {
  stopVehicle();
  robotStatus = "finished";
  isDriving = false;
  isFinished = true;
}
