#ifndef climber
#define climber
#include "Arduino.h"
#define climberDown_io1 48
#define climberDown_io2 49
#define climberDown_pwm 4
#define climberAbove_io1 50
#define climberAbove_io2 51
#define climberAbove_pwm 3
#define high_speed 200
#define low_speed  100

void climber_init();
void climb();
void floating();
void get_down();
void climber_stop();

#endif
