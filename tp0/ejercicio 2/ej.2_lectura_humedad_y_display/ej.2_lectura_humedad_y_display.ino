#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include "DHT.h"

#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT, &Wire, -1);


#define DHTTYPE DHT11 //seteo pines sensor de humedad
#define DHTPIN 23
DHT dht(DHTPIN, DHTTYPE);

float lecturaTemp;
  
void setup() {
  dht.begin(); //inicializo el sensor de temperatura
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicia el display OLED
  display.clearDisplay(); // Borrar imagen en el OLED
  Serial.begin(9600);

}

void loop() {
  lecturaTemp = dht.readTemperature(); //leemos la temperatura

  display.setTextSize(1); // Tamaño del texto
  display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
  display.setCursor(0,0);
  display.println("TEMPERATURA");
  display.setCursor(25,30); 
  display.println(lecturaTemp); // Carga la información al buffer
  display.display(); // Actualiza display con datos en Buffer
  display.clearDisplay();
  delay(1000);

}
