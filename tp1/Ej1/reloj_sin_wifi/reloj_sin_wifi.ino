//katz y perez 
int seg;
int minutos;
int hs;
int mil;
long actual_millis;
void setup() {
  actual_millis = millis();
}

void loop() {
  if (seg >= 60){
    seg = 0;
    minutos = minutos + 1;
  }
  if (minutos >= 60){
    minutos = 0;
    hs = hs + 1;
  }
  if (hs >= 12){
    hs = 0;
  }

}

void segundos (){
  mil = millis() - actual_millis;
  if (mil > 1000){
    seg = seg + 1;
    actual_millis = millis();
  }
}
