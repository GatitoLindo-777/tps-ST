//katz y perez voges

#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT, &Wire, -1);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicia el display OLED
  display.clearDisplay(); // Borrar imagen en el OLED

}

void loop() {
  display.setTextSize(1); // Tamaño del texto
 display.setTextColor(WHITE); // Definir color del texto (WHITE-BLACK)
 display.setCursor(25,15); // Definir posición Columna (25) Fila (15)
 display.println("HELLO WORLD"); // Carga la información al buffer
 display.display(); // Actualiza display con datos en Buffer
}
