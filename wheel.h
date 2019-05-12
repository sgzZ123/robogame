#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

#define l_f_wheel_pwm 10 //左前轮pwm
#define l_f_wheel_io2 23 //左前轮io1
#define l_f_wheel_io1 22 //左前轮pio2

#define l_b_wheel_pwm 11
#define l_b_wheel_io2 25
#define l_b_wheel_io1 24

#define r_f_wheel_pwm 12
#define r_f_wheel_io1 27
#define r_f_wheel_io2 26

#define r_b_wheel_pwm 13
#define r_b_wheel_io2 29
#define r_b_wheel_io1 28
#define portrait 0
#define vertical !portrait
#define left_front  0
#define left_back   1
#define right_front 2
#define right_back  3
#define front 1
#define back  2
#define left  3
#define right 4


extern int state;
extern int stage;
extern int forward_speed[4];
extern int back_speed[4];
extern int left_speed[4];
extern int right_speed[4];

void wheel_init();//设置模式：输出 
void wheel_forward(int direct);//前进 
void wheel_backward(int direct);//后退 
void wheel_leftward(int direct);//左行 
void wheel_rightward(int direct);//右行 
void wheel_stop();//停止 
void wheel_move(int cardirection);
void wheel_slowForward(int direct);
void wheel_slowBackward(int direct);
void wheel_slowLeftward(int direct);
void wheel_slowRightward(int direct);
void wheel_slow_move(int cardirection);
void wheel_slowslowForward(int direct);
#endif

