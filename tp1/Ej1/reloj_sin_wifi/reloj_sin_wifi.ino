//katz y perez
#include <SPI.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

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
#define COMPROBACION_BTN_HORAS 4
#define COMPROBACION_BTN_MINUTOS 5
float temp;
int VU = 27;
int lecturaBtn1;
int lecturaBtn2;
int flag;
long tiempoActual;

int seg;
int minutos;
int hs;
int mil;
long actual_millis;
void segundos();
void reloj();

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

  actual_millis = millis();
}

void loop() {
  segundos();
  reloj();
  temp = dht.readTemperature(); //leemos la temperatura

  switch (estado) {
    case PANTALLA1:
      display.setCursor(05, 0);
      display.println("temp:");
      display.setCursor(40, 0);
      display.println(temp);
      display.setCursor(5, 30);
      display.println("tiempo:");
      display.setCursor(60, 30);
      display.println(hs);
      display.setCursor(70, 30);
      display.println(":");
      display.setCursor(75, 30);
      display.println(minutos);
      display.setCursor(85, 30);
      display.println(":");
      display.setCursor(90, 30);
      display.println(seg);
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
        //Serial.print("pantalla2");
      }
      break;
    case PANTALLA2:
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      if (digitalRead(PIN_BTN1) == 0) {
        tiempoActual = millis();
        estado = COMPROBACION_BTN_HORAS;
      }
      if (digitalRead(PIN_BTN2) == 0) {
        tiempoActual = millis();
        estado = COMPROBACION_BTN_MINUTOS;
      }
      display.setCursor(5, 30);
      display.println("tiempo:");
      display.setCursor(60, 30);
      display.println(hs);
      display.setCursor(70, 30);
      display.println(":");
      display.setCursor(75, 30);
      display.println(minutos);
      display.setCursor(85, 30);
      display.println(":");
      display.setCursor(90, 30);
      display.println(seg);
      display.display();
      display.clearDisplay();


      break;
    /*case COMPROBACION_BTN_2:
        delay(10);
        if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
          estado = PANTALLA1;
        }
        break;*/
    case COMPROBACION_BTN_HORAS:
      if (digitalRead(PIN_BTN1) == 1 && millis() - tiempoActual > 200) {
        hs = hs + 1;
        estado = PANTALLA2;
      }
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
    case COMPROBACION_BTN_MINUTOS:
      if (digitalRead(PIN_BTN2) == 1 && millis() - tiempoActual > 200) {
        minutos = minutos + 1;
        estado = PANTALLA2;
      }
      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
  }
  //Serial.print(minutos);
}


void segundos() {
  mil = millis() - actual_millis;
  if (mil > 1000) {
    seg = seg + 1;
    actual_millis = millis();
    //Serial.println(seg);
  }
}

void reloj() {
  if (seg >= 60) {
    seg = 0;
    minutos = minutos + 1;
  }
  if (minutos >= 60) {
    minutos = 0;
    hs = hs + 1;
  }
  if (hs >= 24) {
    hs = 0;
  }
}
