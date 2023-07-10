//katz y perez 
#include <SPI.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>

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
int lecturaBtn1;
int lecturaBtn2;
int flag;
long tiempoActual;

void pedir_lahora(void);
void setup_rtc_ntp(void);
struct tm timeinfo;
ESP32Time rtc;
/// time
long unsigned int timestamp; // hora
long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;
const char *servidor = "south-america.pool.ntp.org";

const char* red = "PixelGaspar";
const char* clave = "aaaaa12345";

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
  
  display.setTextSize(1); // Tamaño del texto
  display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
  
  tiempoActual = millis();
  initWiFi();
  setup_rtc_ntp();
}

void loop() {
  pedirHora();
  temp = dht.readTemperature(); //leemos la temperatura

  switch (estado) {
    case PANTALLA1:
      display.setCursor(5, 0);
      display.println("temp:");
      display.setCursor(40, 0);
      display.println(temp);
      printTime();
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
     if (digitalRead(PIN_BTN1) == 0){
      tiempoActual = millis();
        estado = COMPROBACION_BTN_SUMA;
     }
     if (digitalRead(PIN_BTN2) == 0) {
        tiempoActual = millis();
        estado = COMPROBACION_BTN_RESTA;
     }
      printTime();

      
      break;
    /*case COMPROBACION_BTN_2:
        delay(10);
        if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
          estado = PANTALLA1;
        }
        break;*/
    case COMPROBACION_BTN_SUMA:
      if (digitalRead(PIN_BTN1) == 1 && millis() - tiempoActual > 200) {
        gmtOffset_sec = gmtOffset_sec + 3600;
        setup_rtc_ntp();
        estado = PANTALLA2;
      }
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
    case COMPROBACION_BTN_RESTA:
      if (digitalRead(PIN_BTN2) == 1 && millis() - tiempoActual > 200) {
        gmtOffset_sec = gmtOffset_sec - 3600;
        setup_rtc_ntp();
        estado = PANTALLA2;
      }
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
  }
 }

void initWiFi() {
  WiFi.begin(red , clave);
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup_rtc_ntp()
{
  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, servidor);
  timestamp = time(NULL);
  rtc.setTime(timestamp + gmtOffset_sec);
}

void pedirHora()
{
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("veo la hora del rtc interno ");
    timestamp = rtc.getEpoch() - gmtOffset_sec;
    timeinfo = rtc.getTimeStruct();
     Serial.println(&timeinfo, "%H:%M:%S");
  }
  else
  {
    Serial.print("NTP Time:");
    timestamp = time(NULL);
    Serial.println(&timeinfo, "%H:%M:%S");
  }
}
  
void printTime (){
  display.setCursor(5, 20);
  display.println("tiempo:");
  display.setCursor(40, 20);
  display.println(&timeinfo, "%H:%M:%S");
  display.display();
  display.clearDisplay();
}
