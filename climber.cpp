#include "climber.h"
#include "Arduino.h"

void climber_init(){
    pinMode(climberAbove_io1,OUTPUT);
    pinMode(climberAbove_io2,OUTPUT);
    pinMode(climberDown_io1,OUTPUT);
    pinMode(climberDown_io2,OUTPUT);
}
void climb(){
    digitalWrite(climberAbove_io1,HIGH);
    digitalWrite(climberAbove_io2,LOW);
    analogWrite(climberAbove_pwm,200);
    digitalWrite(climberDown_io1,HIGH);
    digitalWrite(climberDown_io2,LOW);
    analogWrite(climberDown_pwm,225);
}
void floating(){
    digitalWrite(climberAbove_io1,HIGH);
    digitalWrite(climberAbove_io2,LOW);
    analogWrite(climberAbove_pwm,low_speed);
    digitalWrite(climberDown_io1,HIGH);
    digitalWrite(climberDown_io2,LOW);
    analogWrite(climberDown_pwm,low_speed);
}
void get_down(){
    digitalWrite(climberAbove_io1,LOW);
    digitalWrite(climberAbove_io2,HIGH);
    analogWrite(climberAbove_pwm,100);
}
void climber_stop(){
    digitalWrite(climberAbove_io1,LOW);
    digitalWrite(climberAbove_io2,LOW);
    analogWrite(climberAbove_pwm,0);
    digitalWrite(climberDown_io1,LOW);
    digitalWrite(climberDown_io2,LOW);
    analogWrite(climberDown_pwm,0);
}
