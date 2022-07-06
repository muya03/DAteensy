#include "gear.h"

void initGear(){
    for(int i=0; i<7; i++){
      pinMode(i, INPUT);
    }
}

int getGear(){
  for(int i=0; i<7; i++){
    if (!digitalRead(i)){
      return i;
    }
  }
  return 255;
}