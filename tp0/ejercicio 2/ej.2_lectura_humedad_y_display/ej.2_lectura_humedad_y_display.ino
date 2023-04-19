#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"


#define OLED_MOSI 9 //seteo pines del display
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define DHTTYPE DHT11 //seteo pines sensor de humedad
#define DHTPIN 23
DHT dht(DHTPIN, DHTTYPE);

float lecturaHumedad;
  
void setup() {
  dht.begin(); //inicializo el sensor de temperatura
  display.begin(SSD1306_SWITCHCAPVCC); // Inicia el display OLED
  display.clearDisplay(); // Borrar imagen en el OLED

}

void loop() {
  lecturaHumedad = dht.readTemperature(); //leemos la temperatura

  display.setTextSize(1); // Tamaño del texto
  display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
  display.setCursor(25,0);
  display.println("HUMEDAD");
  display.setCursor(25,15); 
  display.println(lecturaHumedad); // Carga la información al buffer
  display.display(); // Actualiza display con datos en Buffer
  delay(1000);

}
