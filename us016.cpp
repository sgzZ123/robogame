#include "us016.h"
#include "Arduino.h"
int us016(int direct){
  int data[3]={0},avd;
  float distance;
  for(int i=0;i<3;i++){
    switch(direct){
        case left:data[i]=analogRead(A1);break;
        case right:data[i]=analogRead(A0);break;
        case front:data[i]=analogRead(A2);break;
    }
    delay(10);
  }
  avd=data[0]+data[1]+data[2];
  avd/=3;
  float voltage_percentage = (float)avd/1023;
  switch(direct){
    case left:distance = voltage_percentage*300;break;
    case right:distance = voltage_percentage*300;break;
    case front:distance = voltage_percentage*100;break;
  }
  return(distance);
  if(distance>25)
    return(1);
  else if(distance>20)
    return(1);
  else if(direct==front)
    return(distance);
}
