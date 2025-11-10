#include "electro_utils.h"
#include "config.h"
#include <Arduino.h>

void PinDisplayOff(){
  digitalWrite(DISPLAY_PIN, LOW);
}


void PinDisplaySet(int number){
  digitalWrite(DISPLAY_PIN, HIGH);
}