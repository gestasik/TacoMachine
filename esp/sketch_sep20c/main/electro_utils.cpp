#include "electro_utils.h"
#include "config.h"

void PinDisplayOff(){
  DigitalWrite(DISPLAY_PIN, LOW);
}


void PinDisplaySet(int number){
  DigitalWrite(DISPLAY_PIN, HIGH);
}