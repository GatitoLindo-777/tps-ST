//katz,perez vogues

#include <SPI.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define DHTTYPE DHT11
#define DHTPIN 23
DHT dht(DHTPIN, DHTTYPE);
#define PIN_BTN1 34
#define PIN_BTN2 35

#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT, &Wire, -1);

int estado;
#define PANTALLA1 0
#define COMPROBACION_BTN_1 1
#define PANTALLA2 2
#define COMPROBACION_BTN_2 3
#define COMPROBACION_BTN_SUMA 4
#define COMPROBACION_BTN_RESTA 5
float temp;
int VU = 27;
int lecturaBtn1;
int lecturaBtn2;
int flag;
long tiempoActual;
//int cicloGuardado;

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
//const char* WIFI_SSID "a"
//const char* WIFI_PASSWORD "12345678"

#define WIFI_SSID "a"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAn3racaSbJKQIphvrENv_wThfYT6J-wsI"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "gaspikatz2005@gmail.com"
#define USER_PASSWORD "17122005"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://tp5-st-646bd-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/temperature";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long cicloGuardado = 60;
void initWiFi();

void setup() {
  Serial.begin (9600);
  Serial.println("Iniciando...");
  dht.begin(); //inicializo el sensor de temperatura
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Inicializamos el oled
  display.clearDisplay(); // Borrar imagen en el OLED

  pinMode(PIN_BTN1, INPUT);
  pinMode(PIN_BTN2, INPUT);
  flag = 1;
  estado = PANTALLA1;

  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}


void loop() {
  temp = dht.readTemperature(); //leemos la temperatura
  //Serial.print("Temperatura ");
  //Serial.println(temp);

  if (Firebase.ready() && (millis() - sendDataPrevMillis > cicloGuardado || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath = databasePath + "/" + String(timestamp);

    json.set(tempPath.c_str(), String(temp));
       json.set(timePath, String(timestamp));
  }


  switch (estado) {
    case PANTALLA1:
      display.setTextSize(2); // Tamaño del texto
      display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
      display.setCursor(5, 10);
      display.println("VR:");
      display.setCursor(40, 10);
      display.println(temp);
      display.setCursor(5, 35);
      display.println("VU:"); // Escribimos en el buffer el texto con la posición (X,Y) en la pantalla
      display.setCursor(40, 35);
      display.println(VU);
      display.display();
      display.clearDisplay();

      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
        Serial.println(PANTALLA2);
      }
      break;
    case COMPROBACION_BTN_1:
      if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
        if (flag == 1) {
          estado = PANTALLA2;
          Serial.print("pantalla2");
          flag = 2;
        } else if (flag == 2) {
          estado = PANTALLA1;
          Serial.print("pantalla1");
          flag = 1;
        }
        Serial.print("pantalla2");
      }
      break;
    case PANTALLA2:
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      if (digitalRead(PIN_BTN1) == 0) {
        tiempoActual = millis();
        estado = COMPROBACION_BTN_SUMA;
      }
      if (digitalRead(PIN_BTN2) == 0) {
        tiempoActual = millis();
        estado = COMPROBACION_BTN_RESTA;
      }
      display.setCursor(5, 35);
      display.println("Ciclo:"); // Escribimos en el buffer el texto con la posición (X,Y) en la pantalla
      display.setCursor(60, 35);
      display.println(cicloGuardado);
      display.display();
      display.clearDisplay();


      break;
    /*case COMPROBACION_BTN_2:
        delay(10);
        if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
          estado = PANTALLA1;
        }
        break;*/
    case COMPROBACION_BTN_SUMA:
      if (digitalRead(PIN_BTN1) == 1 && millis() - tiempoActual > 200) {
        cicloGuardado = cicloGuardado + 30;
        estado = PANTALLA2;
        if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
          estado = COMPROBACION_BTN_1;
        }
      }
      break;
    case COMPROBACION_BTN_RESTA:
      if (digitalRead(PIN_BTN2) == 1 && millis() - tiempoActual > 200) {
        cicloGuardado = cicloGuardado - 30;
        if (cicloGuardado <= 0) {
          cicloGuardado = 0;
        }
        estado = PANTALLA2;
      }
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
  }
  Serial.println(estado);
  //Serial.println(VU);
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}
