#ifndef pusher
#define pusher
#include "Arduino.h"

#define pusher_io1 53
#define pusher_io2 52
#define pusher_pwm 2

void pusher_init();
void pusher_out();
void pusher_in();
void pusher_stop();

#endif
