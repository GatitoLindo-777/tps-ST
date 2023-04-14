#include <U8g2lib.h>
#include "DHT.h"
#include "Wire.h"
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT11
#define DHTPIN 23
DHT dht(DHTPIN, DHTTYPE);
#define PIN_BTN1 11
#define PIN_BTN2 12

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int estado;
#define PANTALLA1 0
#define COMPROBACION_BTN_1 1
#define PANTALLA2 2
#define COMPROBACION_BTN_2 3
#define COMPROBACION_BTN_SUMA 4
#define COMPROBACION_BTN_RESTA 5

float VU;
int lecturaBtn1;
int lecturaBtn2;
String prtTemp;
String prtVU;

void setup() {
  Serial.begin (9600);
  Serial.println("Iniciando...");
  dht.begin(); //inicializo el sensor de temperatura
  u8g2.begin(); //Inicializamos el oled

  VU = 27;
  u8g2.clearBuffer();

  pinMode(PIN_BTN1, INPUT);
  pinMode(PIN_BTN2, INPUT);
}

void loop() {
  float temp = dht.readTemperature(); //leemos la temperatura
  //Serial.print("Temperatura ");
  

  switch (estado) {
    case PANTALLA1:
      u8g2.setFont(u8g2_font_logisoso16_tr);    // Seleccionamos la fuente
      u8g2.drawStr(5, 25, "VR:");
      //prtTemp = String (temp, 2);
      //u8g2.print(prtTemp);
      //Serial.println(temp);
      //u8g2.drawStr(15,35,prtTemp);
      sprintf (prtTemp , "%f", temp);
      u8g2.drawStr(5, 45, "VU:"); // Escribimos en el buffer el texto con la posición (X,Y) en la pantalla
      //prtVU = String (VU, 0);
      //u8g2.print(prtVU);
      sprintf (prtVU , "%f", VU);
      //Serial.println(VU);
      u8g2.sendBuffer();
      u8g2.clearBuffer();
      //Serial.println(digitalRead(PIN_BTN1));
      //Serial.println(digitalRead(PIN_BTN2));

      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_1;
      }
      break;
    case COMPROBACION_BTN_1:
      delay(10);
      if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
        estado = PANTALLA2;
        Serial.print("pantalla2");
      }
      break;
    case PANTALLA2:
      if (digitalRead(PIN_BTN1) == 0) {
        estado = COMPROBACION_BTN_SUMA;
      }
      if (digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_RESTA;
      }
      u8g2.setFont(u8g2_font_logisoso16_tr);    // Seleccionamos la fuente
      u8g2.drawStr(5, 45, "VU:"); // Escribimos en el buffer el texto con la posición (X,Y) en la pantalla
      //prtVU = String (VU, 0);
      sprintf (prtVU , "%f", VU);
      Serial.println(VU);
      u8g2.sendBuffer();
      u8g2.clearBuffer();

      if (digitalRead(PIN_BTN1) == 0 && digitalRead(PIN_BTN2) == 0) {
        estado = COMPROBACION_BTN_2;
      }
      break;
    case COMPROBACION_BTN_2:
      delay(10);
      if (digitalRead(PIN_BTN1) == 1 && digitalRead(PIN_BTN2) == 1) {
        estado = PANTALLA1;
      }
      break;
    case COMPROBACION_BTN_SUMA:
      delay(10);
      if (digitalRead(PIN_BTN1) == 1) {
        VU = VU + 1;
        estado = PANTALLA2;
      }
      break;
    case COMPROBACION_BTN_RESTA:
      delay(10);
      if (digitalRead(PIN_BTN2) == 1) {
        VU = VU - 1;
        estado = PANTALLA2;
      }
      break;

  }

sprintf (datostring , "%f", datofloat);
  /*u8g2.clearBuffer();                    // Vaciamos el buffer del dispositivo
    u8g2.setFont(u8g2_font_logisoso16_tr);    // Seleccionamos la fuente
    u8g2.drawStr(5,35,"hola");    // Escribimos en el buffer el texto con la posición (X,Y) en la pantalla
    u8g2.sendBuffer();                    // Enviamos el buffer a nuestra pantalla*/
}
