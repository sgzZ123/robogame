#ifndef myservo_H_
#define myservo_H_
#include "Arduino.h"
#include<Servo.h>

#define hand1_open_angle  40
#define hand1_close_angle 160
#define hand2_open_angle  0
#define hand2_close_angle 60
#define hand1_up_angle    20
#define hand1_down_angle  120
#define hand2_up_angle    40
#define hand2_down_angle  145
#define hand1_front       0
#define hand1_left        57
#define hand1_back        118
#define hand1_right       180
#define hand1_servo1_pin  5
#define hand1_servo2_pin  6
#define hand2_servo1_pin  7
#define hand2_servo2_pin  8
#define platform_pin      9
#define stage2photo       12
#define stage1photo       102


extern Servo hand1_servo1;
extern Servo hand1_servo2;
extern Servo hand2_servo1;
extern Servo hand2_servo2;
extern Servo platform_servo;

void servo_init();
void platform_rotate(float angle);
void open_hand1();
void open_hand2();
void close_hand1();
void close_hand2();
void hand1_up();
void hand1_down();
void hand2_up();
void hand2_down();
#endif
