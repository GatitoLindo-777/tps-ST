#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC); // Inicia el display OLED
  display.clearDisplay(); // Borrar imagen en el OLED

}

void loop() {
  display.setTextSize(1); // Tamaño del texto
 display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
 display.setCursor(25,15); // Definir posición Columna (25) Fila (15)
 display.println("HELLO WORLD"); // Carga la información al buffer
 display.display(); // Actualiza display con datos en Buffer
}
