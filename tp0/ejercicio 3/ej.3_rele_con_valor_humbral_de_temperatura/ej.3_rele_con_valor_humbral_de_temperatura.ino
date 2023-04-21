#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT11
#define DHTPIN 23
DHT dht(DHTPIN, DHTTYPE);

float lecturaHumedad;
#define VALOR_HUMBRAL 27 //seteo el valor de humbral
#define PIN_RELE 18//seteo el pin del rele

void setup() {
  dht.begin(); //inicializo el sensor de temperatura
  pinMode(PIN_RELE, OUTPUT); //configuro el pin del rele como output
  Serial.begin(9600);
}

void loop() {
  lecturaHumedad = dht.readTemperature(); //leemos la temperatura
  Serial.println(lecturaHumedad);
  
  if (lecturaHumedad > VALOR_HUMBRAL){  //comprovamos si la temperatura supera el valor humbral
    digitalWrite(PIN_RELE, HIGH);   //prendemos el rele
  }
  if (lecturaHumedad < VALOR_HUMBRAL){  //comprovamos si la temperatura supera el valor humbral
    digitalWrite(PIN_RELE, LOW);   //apago el rele
  }
}
