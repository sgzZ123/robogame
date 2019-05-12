#include "pusher.h"
#include "trailing.h"
#include "wheel.h"
#include "Arduino.h"
void pusher_stop(){
    digitalWrite(pusher_io1,LOW);
    digitalWrite(pusher_io2,LOW);
}
void pusher_init(){
    pinMode(pusher_io1,OUTPUT);
    pinMode(pusher_io2,OUTPUT);
    pinMode(pusher_pwm,OUTPUT);
}
void pusher_out(){
    digitalWrite(pusher_io1,HIGH);
    digitalWrite(pusher_io2,LOW);
    analogWrite(pusher_pwm,255);
}
void pusher_in(){
    digitalWrite(pusher_io1,LOW);
    digitalWrite(pusher_io2,HIGH);
    analogWrite(pusher_pwm,255);
    int count=0;
    while(count<430){
        slowslow_trailing_move();
        delay(25);
        count++;
    }
    wheel_stop();
    pusher_stop();
}

