#include "myservo.h"
#include "Arduino.h"
Servo hand1_servo1;
Servo hand1_servo2;
Servo hand2_servo1;
Servo hand2_servo2;
Servo platform_servo;

void servo_init() {
	hand1_servo1.attach(hand1_servo1_pin);
	hand1_servo2.attach(hand1_servo2_pin);
	hand2_servo1.attach(hand2_servo1_pin);
	hand2_servo2.attach(hand2_servo2_pin);
	pinMode(platform_pin,OUTPUT);
}
void platform_rotate(float angle) {
        pinMode(platform_pin,OUTPUT);
	    int pulsewidth = angle * 11 + 500;
	    digitalWrite(platform_pin, HIGH);
	    delayMicroseconds(pulsewidth);
	    digitalWrite(platform_pin, LOW);
	    delayMicroseconds(20000 - pulsewidth);
}

void open_hand1() {
	hand1_servo1.write(hand1_open_angle);
}
void open_hand2() {
	hand2_servo1.write(hand2_open_angle);
}
void close_hand1() {
	hand1_servo1.write(hand1_close_angle);
}
void close_hand2() {
	hand2_servo1.write(hand2_close_angle);
}
void hand1_up() {
	hand1_servo2.write(hand1_up_angle);
}
void hand1_down() {
	hand1_servo2.write(hand1_down_angle);
}
void hand2_up() {
	hand2_servo2.write(hand2_up_angle);
}
void hand2_down() {
	hand2_servo2.write(hand2_down_angle);
}
