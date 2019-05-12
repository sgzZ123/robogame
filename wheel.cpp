#include "wheel.h"
#include "Arduino.h"
//前 后 左 右
// 1  2  3  4
int state;     
//向前运动时的pwm波      左前 左后 右前 右后
int forward_speed[4]=      { 255, 250, 225, 255};
//向后运动时的pwm波      左前 左后 右前 右后
int back_speed[4]=      { 245, 220, 255, 255};
//向右运动时的pwm波      左前 左后 右前 右后
int right_speed[4]=      { 255, 218,255,198};
//向左运动时的pwm波      左前 左后 右前 右后
int left_speed[4]=      { 252, 208, 255, 208};

void wheel_init()//设置模式：输出
{
  
    pinMode(l_f_wheel_pwm,OUTPUT);  
    pinMode(l_f_wheel_io1,OUTPUT);
    pinMode(l_f_wheel_io2,OUTPUT);
    pinMode(l_b_wheel_pwm,OUTPUT);
    pinMode(l_b_wheel_io1,OUTPUT);
    pinMode(l_b_wheel_io2,OUTPUT);
    pinMode(r_f_wheel_pwm,OUTPUT);  
    pinMode(r_f_wheel_io1,OUTPUT);
    pinMode(r_f_wheel_io2,OUTPUT);
    pinMode(r_b_wheel_pwm,OUTPUT);  
    pinMode(r_b_wheel_io1,OUTPUT);
    pinMode(r_b_wheel_io2,OUTPUT);
}


/*******************************************************************************************************************************************************************/    

void wheel_forward(int direct)
{
    switch(direct){
        case front: digitalWrite(l_f_wheel_io1,HIGH);
                    digitalWrite(l_f_wheel_io2,LOW);
                    analogWrite(l_f_wheel_pwm,forward_speed[left_front]);
                    digitalWrite(r_f_wheel_io1,HIGH);
                    digitalWrite(r_f_wheel_io2,LOW);
                    analogWrite(r_f_wheel_pwm,forward_speed[right_front]);
                    break;
        case back:  digitalWrite(l_b_wheel_io1,HIGH);
                    digitalWrite(l_b_wheel_io2,LOW);
                    analogWrite(l_b_wheel_pwm,forward_speed[left_back]);
                    digitalWrite(r_b_wheel_io1,HIGH);
                    digitalWrite(r_b_wheel_io2,LOW);
                    analogWrite(r_b_wheel_pwm,forward_speed[right_back]);
                    break;
    }
}


/*******************************************************************************************************************************************************************/   

void wheel_backward(int direct)
{
    switch(direct){
        case front: digitalWrite(l_f_wheel_io1,LOW);
                    digitalWrite(l_f_wheel_io2,HIGH);
                    analogWrite(l_f_wheel_pwm,back_speed[left_front]);
                    digitalWrite(r_f_wheel_io1,LOW);
                    digitalWrite(r_f_wheel_io2,HIGH);
                    analogWrite(r_f_wheel_pwm,back_speed[right_front]);
                    break;
        case back: digitalWrite(l_b_wheel_io1,LOW);
                   digitalWrite(l_b_wheel_io2,HIGH);
                   analogWrite(l_b_wheel_pwm,back_speed[left_back]);
                   digitalWrite(r_b_wheel_io1,LOW);
                   digitalWrite(r_b_wheel_io2,HIGH);
                   analogWrite(r_b_wheel_pwm,back_speed[right_back]);
                   break;
    }
}

/*******************************************************************************************************************************************************************/   

void wheel_rightward(int direct)
{
    switch(direct){
        case left: digitalWrite(l_f_wheel_io1,HIGH);
                   digitalWrite(l_f_wheel_io2,LOW);
                   analogWrite(l_f_wheel_pwm,right_speed[left_front]);
                   digitalWrite(l_b_wheel_io1,LOW);
                   digitalWrite(l_b_wheel_io2,HIGH);
                   analogWrite(l_b_wheel_pwm,right_speed[left_back]);
                   break;
        case right: digitalWrite(r_f_wheel_io1,LOW);
                    digitalWrite(r_f_wheel_io2,HIGH);
                    analogWrite(r_f_wheel_pwm,right_speed[right_front]);
                    digitalWrite(r_b_wheel_io1,HIGH);
                    digitalWrite(r_b_wheel_io2,LOW);
                    analogWrite(r_b_wheel_pwm,right_speed[right_back]);
                    break;
    }
}

/*******************************************************************************************************************************************************************/   
void wheel_leftward(int direct)
{
    switch(direct){
        case left:  digitalWrite(l_f_wheel_io1,LOW);
                    digitalWrite(l_f_wheel_io2,HIGH);
                    analogWrite(l_f_wheel_pwm,left_speed[left_front]);
                    digitalWrite(l_b_wheel_io1,HIGH);
                    digitalWrite(l_b_wheel_io2,LOW);
                    analogWrite(l_b_wheel_pwm,left_speed[left_back]);
                    break;
        case right: digitalWrite(r_f_wheel_io1,HIGH);
                    digitalWrite(r_f_wheel_io2,LOW);
                    analogWrite(r_f_wheel_pwm,left_speed[right_front]);
                    digitalWrite(r_b_wheel_io1,LOW);
                    digitalWrite(r_b_wheel_io2,HIGH);
                    analogWrite(r_b_wheel_pwm,left_speed[right_back]);
                    break;
    }
}

/*******************************************************************************************************************************************************************/   

void wheel_stop()
{
    digitalWrite(l_f_wheel_io1,LOW);
    digitalWrite(l_f_wheel_io2,LOW);
    digitalWrite(l_f_wheel_pwm,0);

    digitalWrite(l_b_wheel_io1,LOW);
    digitalWrite(l_b_wheel_io2,LOW);
    digitalWrite(l_b_wheel_pwm,0);

    digitalWrite(r_f_wheel_io1,LOW);
    digitalWrite(r_f_wheel_io2,LOW);
    digitalWrite(r_f_wheel_pwm,0);

    digitalWrite(r_b_wheel_io1,LOW);
    digitalWrite(r_b_wheel_io2,LOW);
    digitalWrite(r_b_wheel_pwm,0);
}
/*******************************************************************************************************************************************************************/   
void wheel_move(int cardirection){
    switch(state){
        case 1: wheel_forward(cardirection);
                break;
        case 2: wheel_backward(cardirection);
                break;
        case 3: wheel_leftward(cardirection);
                break;
        case 4: wheel_rightward(cardirection);
                break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slowForward(int direct)
{   
    switch(direct){
        case front: digitalWrite(l_f_wheel_io1,HIGH);
                    digitalWrite(l_f_wheel_io2,LOW);
                    analogWrite(l_f_wheel_pwm,forward_speed[left_front]*0.6);
                    digitalWrite(r_f_wheel_io1,HIGH);
                    digitalWrite(r_f_wheel_io2,LOW);
                    analogWrite(r_f_wheel_pwm,forward_speed[right_front]*0.6);
                    break;
        case back:  digitalWrite(l_b_wheel_io1,HIGH);
                    digitalWrite(l_b_wheel_io2,LOW);
                    analogWrite(l_b_wheel_pwm,forward_speed[left_back]*0.6);
                    digitalWrite(r_b_wheel_io1,HIGH);
                    digitalWrite(r_b_wheel_io2,LOW);
                    analogWrite(r_b_wheel_pwm,forward_speed[right_back]*0.6);
                    break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slowBackward(int direct)
{   
    switch(direct){
        case front: digitalWrite(l_f_wheel_io1,LOW);
                    digitalWrite(l_f_wheel_io2,HIGH);
                    analogWrite(l_f_wheel_pwm,back_speed[left_front]*0.6);
                    digitalWrite(r_f_wheel_io1,LOW);
                    digitalWrite(r_f_wheel_io2,HIGH);
                    analogWrite(r_f_wheel_pwm,back_speed[right_front]*0.6);
                    break;
        case back:  digitalWrite(l_b_wheel_io1,LOW);
                    digitalWrite(l_b_wheel_io2,HIGH);
                    analogWrite(l_b_wheel_pwm,back_speed[left_back]*0.6);
                    digitalWrite(r_b_wheel_io1,LOW);
                    digitalWrite(r_b_wheel_io2,HIGH);
                    analogWrite(r_b_wheel_pwm,back_speed[right_back]*0.6);
                    break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slowLeftward(int direct)
{   
    switch(direct){
        case left:  digitalWrite(l_f_wheel_io1,LOW);
                    digitalWrite(l_f_wheel_io2,HIGH);
                    analogWrite(l_f_wheel_pwm,left_speed[left_front]*0.6);
                    digitalWrite(l_b_wheel_io1,HIGH);
                    digitalWrite(l_b_wheel_io2,LOW);
                    analogWrite(l_b_wheel_pwm,left_speed[left_back]*0.6);
                    break;
        case right: digitalWrite(r_f_wheel_io1,HIGH);
                    digitalWrite(r_f_wheel_io2,LOW);
                    analogWrite(r_f_wheel_pwm,left_speed[right_front]*0.6);
                    digitalWrite(r_b_wheel_io1,LOW);
                    digitalWrite(r_b_wheel_io2,HIGH);
                    analogWrite(r_b_wheel_pwm,left_speed[right_back]*0.6);
                    break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slowRightward(int direct)
{   
    switch(direct){
        case left: digitalWrite(l_f_wheel_io1,HIGH);
                   digitalWrite(l_f_wheel_io2,LOW);
                   analogWrite(l_f_wheel_pwm,right_speed[left_front]*0.6);
                   digitalWrite(l_b_wheel_io1,LOW);
                   digitalWrite(l_b_wheel_io2,HIGH);
                   analogWrite(l_b_wheel_pwm,right_speed[left_back]*0.6);
                   break;
        case right: digitalWrite(r_f_wheel_io1,LOW);
                    digitalWrite(r_f_wheel_io2,HIGH);
                    analogWrite(r_f_wheel_pwm,right_speed[right_front]*0.6);
                    digitalWrite(r_b_wheel_io1,HIGH);
                    digitalWrite(r_b_wheel_io2,LOW);
                    analogWrite(r_b_wheel_pwm,right_speed[right_back]*0.6);
                    break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slow_move(int cardirection){
    switch(state){
        case 1: wheel_slowForward(cardirection);
                break;
        case 2: wheel_slowBackward(cardirection);
                break;
        case 3: wheel_slowLeftward(cardirection);
                break;
        case 4: wheel_slowRightward(cardirection);
                break;
    }
}
/*******************************************************************************************************************************************************************/   
void wheel_slowslowForward(int direct)
{   
    switch(direct){
        case front: digitalWrite(l_f_wheel_io1,HIGH);
                    digitalWrite(l_f_wheel_io2,LOW);
                    analogWrite(l_f_wheel_pwm,255*0.5);
                    digitalWrite(r_f_wheel_io1,HIGH);
                    digitalWrite(r_f_wheel_io2,LOW);
                    analogWrite(r_f_wheel_pwm,230*0.5);
                    break;
        case back:  digitalWrite(l_b_wheel_io1,HIGH);
                    digitalWrite(l_b_wheel_io2,LOW);
                    analogWrite(l_b_wheel_pwm,250*0.5);
                    digitalWrite(r_b_wheel_io1,HIGH);
                    digitalWrite(r_b_wheel_io2,LOW);
                    analogWrite(r_b_wheel_pwm,255*0.5);
                    break;
    }
}

