#include "forceSensor.h"
#include "Arduino.h"
int forceCheck(){
    if(analogRead(A3)>900)
        return(1);
    return(0);
}
