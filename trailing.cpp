#include "Arduino.h"
#include "wheel.h"
#include "trailing.h"

int SF1;
int SF2;
int SF3;//前3传感器状态 
int SB1;
int SB2;
int SB3;//后3传感器状态
int SR1;
int SR2;
int SR3;//右3传感器状态
int SL1;
int SL2;
int SL3;//左3传感器状态
int SLF;//左前传感器状态
int SLB;//左后传感器状态
int SRF;//右前传感器状态
int SRB;//右后传感器状态
extern int stage;
extern int traversing;
extern int node;
extern int aim;
extern int flags[8];
int last_state[4][2]={2,2,2,2,2,2,2,2};

void trailing_init()
{

    pinMode(SensorFront1,INPUT);
    pinMode(SensorFront2,INPUT);
    pinMode(SensorFront3,INPUT);
    pinMode(SensorBack1,INPUT);
    pinMode(SensorBack2,INPUT);
    pinMode(SensorBack3,INPUT);
    pinMode(SensorRight1,INPUT);
    pinMode(SensorRight2,INPUT);
    pinMode(SensorRight3,INPUT);
    pinMode(SensorLeft1,INPUT);
    pinMode(SensorLeft2,INPUT);
    pinMode(SensorLeft3,INPUT);
    pinMode(SensorLeftFront,INPUT);
    pinMode(SensorLeftBack,INPUT);
    pinMode(SensorRightFront,INPUT);
    pinMode(SensorRightBack,INPUT);

}

/*******************************************************************************************************************************************************************/    

int trailing_test (int sign1,int sign2,int sign3,int sign4,int sign5)   
//返回值1：左偏；2：不偏；3：右偏
{
    if (sign3==HIGH && sign2==LOW && sign4==LOW)    
        return 2;
    if (sign2==HIGH)    
        if (sign1==LOW) 
            return 3;
        else    
            return 2;
    if (sign4==HIGH)
        if (sign5==LOW)
            return 1;
        else 
            return 2;
    if(sign2==LOW && sign3==LOW && sign4==LOW)
        return 0;
} 
/*******************************************************************************************************************************************************************/    
int read_trail(int car_direction) { //direction表示哪个方向的寻迹模块，1234为前后左右
    switch (car_direction) {
        case 1:SLF = digitalRead(SensorLeftFront);
               SF1 = digitalRead(SensorFront1);
               SF2 = digitalRead(SensorFront2);
               SF3 = digitalRead(SensorFront3); 
               SRF = digitalRead(SensorRightFront);
               return(trailing_test(SLF, SF1, SF2, SF3, SRF)); break;
        case 2:SRF = digitalRead(SensorRightFront); 
               SB1 = digitalRead(SensorBack1);
               SB2 = digitalRead(SensorBack2);
               SB3 = digitalRead(SensorBack3); 
               SLB = digitalRead(SensorLeftBack);
               return(trailing_test(SRB, SB1, SB2, SB3, SLB)); break;
        case 3:SLB = digitalRead(SensorLeftBack); 
               SL1 = digitalRead(SensorLeft1);
               SL2 = digitalRead(SensorLeft2);
               SL3 = digitalRead(SensorLeft3);
               SLF = digitalRead(SensorLeftFront);
            return(trailing_test(SLB, SL1, SL2, SL3, SLF)); break;
        case 4:SRF = digitalRead(SensorRightFront);
               SR1 = digitalRead(SensorRight1);
               SR2 = digitalRead(SensorRight2);
               SR3 = digitalRead(SensorRight3);
               SRB = digitalRead(SensorRightBack);
               return(trailing_test(SRF, SR1, SR2, SR3, SRB)); break;
    }
    return(0);
}
/*******************************************************************************************************************************************************************/    
int is_node() {
    if (state <= 2)
        if (digitalRead(SensorLeft2) || digitalRead(SensorRight2))
            return(1);
        else
            return(0);
    else
        if (digitalRead(SensorBack2) || digitalRead(SensorFront2))
            return(1);
        else
            return(0);
}
/*******************************************************************************************************************************************************************/   

/*******************************************************************************************************************************************************************/   
void modify(int pwmPin)
{   
    switch(state){
        case 1: analogWrite(pwmPin,forward_speed[pwmPin-10]*0.22);
        case 2: analogWrite(pwmPin,back_speed[pwmPin-10]*0.22);
        case 3: analogWrite(pwmPin,left_speed[pwmPin-10]*0.22);
        case 4: analogWrite(pwmPin,right_speed[pwmPin-10]*0.22);
    }
}

/*******************************************************************************************************************************************************************/   
void modify_slow(int pwmPin)
{   
    switch(state){
        case 1: analogWrite(pwmPin,forward_speed[pwmPin-10]*0.1);
        case 2: analogWrite(pwmPin,back_speed[pwmPin-10]*0.1);
        case 3: analogWrite(pwmPin,left_speed[pwmPin-10]*0.1);
        case 4: analogWrite(pwmPin,right_speed[pwmPin-10]*0.1);
    }
}
/*******************************************************************************************************************************************************************/   
void modify_slowslow(int pwmPin)
{   
    switch(state){
        case 1: analogWrite(pwmPin,pwmPin!=12?(forward_speed[pwmPin-10]*0.1):(forward_speed[pwmPin-10]*0.1+0.5));
        case 2: analogWrite(pwmPin,back_speed[pwmPin-10]*0.06);
        case 3: analogWrite(pwmPin,left_speed[pwmPin-10]*0.06);
        case 4: analogWrite(pwmPin,right_speed[pwmPin-10]*0.04);
    }
}
/*******************************************************************************************************************************************************************/ 
void slow_trailing_move()
{
    switch (state)          //state为1：前进；2：后退；3：左行；4：右行 
    {
    case 1: wheel_slow_move(front);
            wheel_slow_move(back);
            switch (read_trail(front)){
                case 0: switch(last_state[0][0]){
                            case 1:modify_slow(r_f_wheel_pwm);break;
                            case 3:modify_slow(l_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(r_f_wheel_pwm);last_state[1][0]=1;last_state[0][0]=1;break;
                case 2: last_state[0][0]=2;last_state[1][0]=2;break;
                case 3: modify_slow(l_f_wheel_pwm);last_state[1][0]=3;last_state[0][0]=3;break;
            }
            switch (read_trail(back)){
                case 0: switch(last_state[0][1]){
                            case 1:modify_slow(l_b_wheel_pwm);break;
                            case 3:modify_slow(r_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(l_b_wheel_pwm);last_state[1][1]=1;last_state[0][1]=1;break;
                case 2: last_state[0][1]=2;last_state[1][1]=2;break;
                case 3: modify_slow(r_b_wheel_pwm);last_state[1][1]=3;last_state[0][1]=3;break;
            }
            last_state[2][1]=read_trail(left);
            last_state[3][1]=last_state[2][1];
            last_state[2][0]=read_trail(right);
            last_state[3][0]=last_state[2][0];
            break;
    case 2: wheel_slow_move(front);
            wheel_slow_move(back);
            switch (read_trail(front)){
                case 0: switch(last_state[1][0]){
                            case 1:modify_slow(l_f_wheel_pwm);break;
                            case 3:modify_slow(r_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(l_f_wheel_pwm);last_state[0][0]=1;last_state[1][0]=1;break;
                case 2: last_state[1][0]=2;last_state[0][0]=2;break;
                case 3: modify_slow(r_f_wheel_pwm);last_state[0][0]=3;last_state[1][0]=3;break;
            }
            switch (read_trail(back)){
                case 0: switch(last_state[1][1]){
                            case 1:modify_slow(r_b_wheel_pwm);break;
                            case 3:modify_slow(l_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(r_b_wheel_pwm);last_state[0][1]=1;last_state[1][1]=1;break;
                case 2: last_state[1][1]=2;last_state[0][1]=2;break;
                case 3: modify_slow(l_b_wheel_pwm);last_state[1][1]=3;last_state[0][1]=3;break;
            }
            last_state[2][1]=read_trail(left);
            last_state[3][1]=last_state[2][1];
            last_state[2][0]=read_trail(right);
            last_state[3][0]=last_state[2][0];
            break;
    case 3: wheel_slow_move(left);
            wheel_slow_move(right);
            switch (read_trail(right)){
                case 0: switch(last_state[2][0]){
                            case 1:modify_slow(r_f_wheel_pwm);break;
                            case 3:modify_slow(r_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(r_f_wheel_pwm);last_state[2][0]=1;last_state[3][0]=1;break;
                case 2: last_state[2][0]=2;last_state[3][0]=2;break;
                case 3: modify_slow(r_b_wheel_pwm);last_state[3][0]=3;last_state[2][0]=3;break;
            }
            switch (read_trail(left)){
                case 0: switch(last_state[2][1]){
                            case 1:modify_slow(l_f_wheel_pwm);break;
                            case 3:modify_slow(l_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(l_f_wheel_pwm);last_state[3][1]=1;last_state[2][1]=1;break;
                case 2: last_state[2][1]=2;last_state[3][1]=2;break;
                case 3: modify_slow(l_b_wheel_pwm);last_state[3][1]=3;last_state[2][1]=3;break;
            }
            last_state[0][0]=read_trail(front);
            last_state[1][0]=last_state[0][0];
            last_state[0][1]=read_trail(back);
            last_state[1][1]=last_state[0][1];
            break;
    case 4: wheel_slow_move(left);
            wheel_slow_move(right);
            switch (read_trail(right)){
                case 0: switch(last_state[3][0]){
                            case 1:modify_slow(r_b_wheel_pwm);break;
                            case 3:modify_slow(r_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(r_b_wheel_pwm);last_state[3][0]=1;last_state[2][0]=1;break;
                case 2: last_state[3][0]=2;last_state[2][0]=2;break;
                case 3: modify_slow(r_f_wheel_pwm);last_state[2][0]=3;last_state[3][0]=3;break;
            }
            switch (read_trail(left)){
                case 0: switch(last_state[3][1]){
                            case 1:modify_slow(l_b_wheel_pwm);break;
                            case 3:modify_slow(l_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify_slow(l_b_wheel_pwm);last_state[3][1]=1;last_state[2][1]=1;break;
                case 2: last_state[3][1]=2;last_state[2][1]=2;break;
                case 3: modify_slow(l_f_wheel_pwm);last_state[3][1]=3;last_state[2][1]=3;break;
            }
            last_state[0][0]=read_trail(front);
            last_state[1][0]=last_state[0][0];
            last_state[0][1]=read_trail(back);
            last_state[1][1]=last_state[0][1];
            break;
    }

}

/*******************************************************************************************************************************************************************/   
void trailing_move()
{
    switch (state)          //state为1：前进；2：后退；3：左行；4：右行 
    {
    case 1: wheel_move(front);
            wheel_move(back);
            switch (read_trail(front)){
                case 0: switch(last_state[0][0]){
                            case 1:modify(r_f_wheel_pwm);break;
                            case 3:modify(l_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify(r_f_wheel_pwm);last_state[0][0]=1;last_state[1][0]=1;break;
                case 2: last_state[0][0]=2;last_state[1][0]=2;break;
                case 3: modify(l_f_wheel_pwm);last_state[0][0]=3;last_state[1][0]=3;break;
            }
            switch (read_trail(back)){
                case 0: switch(last_state[0][1]){
                            case 1:modify(l_b_wheel_pwm);break;
                            case 3:modify(r_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify(l_b_wheel_pwm);last_state[0][1]=1;last_state[1][1]=1;break;
                case 2: last_state[0][1]=2;last_state[1][1]=2;break;
                case 3: modify(r_b_wheel_pwm);last_state[0][1]=3;last_state[1][1]=3;break;
            }
            last_state[2][1]=read_trail(left);
            last_state[3][1]=last_state[2][1];
            last_state[2][0]=read_trail(right);
            last_state[3][0]=last_state[2][0];
            break;
    case 2: wheel_move(front);
            wheel_move(back);
            switch (read_trail(front)){
                case 0: switch(last_state[1][0]){
                            case 1:modify(l_f_wheel_pwm);break;
                            case 3:modify(r_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify(l_f_wheel_pwm);last_state[1][0]=1;last_state[0][0]=1;break;
                case 2: last_state[1][0]=2;last_state[0][0]=2;break;
                case 3: modify(r_f_wheel_pwm);last_state[1][0]=3;last_state[0][0]=3;break;
            }
            switch (read_trail(back)){
                case 0: switch(last_state[1][1]){
                            case 1:modify(r_b_wheel_pwm);break;
                            case 3:modify(l_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify(r_b_wheel_pwm);last_state[1][1]=1;last_state[0][1]=1;break;
                case 2: last_state[1][1]=2;last_state[0][1]=2;break;
                case 3: modify(l_b_wheel_pwm);last_state[1][1]=3;last_state[0][1]=3;break;
            }
            last_state[2][1]=read_trail(left);
            last_state[3][1]=last_state[2][1];
            last_state[2][0]=read_trail(right);
            last_state[3][0]=last_state[2][0];
            break;
    case 3: wheel_move(left);
            wheel_move(right);
            switch (read_trail(right)){
                case 0: switch(last_state[2][0]){
                            case 1:modify(r_f_wheel_pwm);break;
                            case 3:modify(r_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify(r_f_wheel_pwm);last_state[2][0]=1;last_state[3][0]=1;break;
                case 2: last_state[2][0]=2;last_state[3][0]=2;break;
                case 3: modify(r_b_wheel_pwm);last_state[2][0]=3;last_state[3][0]=3;break;
            }
            switch (read_trail(left)){
                case 0: switch(last_state[2][1]){
                            case 1:modify(l_f_wheel_pwm);break;
                            case 3:modify(l_b_wheel_pwm);break;
                        }
                        break;
                case 1: modify(l_f_wheel_pwm);last_state[2][1]=1;last_state[3][1]=1;break;
                case 2: last_state[2][1]=2;last_state[3][1]=2;break;
                case 3: modify(l_b_wheel_pwm);last_state[2][1]=3;last_state[3][1]=3;break;
            }
            last_state[0][0]=read_trail(front);
            last_state[1][0]=last_state[0][0];
            last_state[0][1]=read_trail(back);
            last_state[1][1]=last_state[0][1];
            break;
    case 4: wheel_move(left);
            wheel_move(right);
            switch (read_trail(right)){
                case 0: switch(last_state[3][0]){
                            case 1:modify(r_b_wheel_pwm);break;
                            case 3:modify(r_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify(r_b_wheel_pwm);last_state[3][0]=1;last_state[2][0]=1;break;
                case 2: last_state[3][0]=2;last_state[2][0]=2;break;
                case 3: modify(r_f_wheel_pwm);last_state[3][0]=3;last_state[2][0]=3;break;
            }
            switch (read_trail(left)){
                case 0: switch(last_state[3][1]){
                            case 1:modify(l_b_wheel_pwm);break;
                            case 3:modify(l_f_wheel_pwm);break;
                        }
                        break;
                case 1: modify(l_b_wheel_pwm);last_state[3][1]=1;last_state[2][1]=1;break;
                case 2: last_state[3][1]=2;last_state[2][1]=2;break;
                case 3: modify(l_f_wheel_pwm);last_state[3][1]=3;last_state[2][1]=3;break;
            }
            last_state[0][0]=read_trail(front);
            last_state[1][0]=last_state[0][0];
            last_state[0][1]=read_trail(back);
            last_state[1][1]=last_state[0][1];
            break;
    }

}

  


/*******************************************************************************************************************************************************************/   
void slow_back(){
    switch(state){
                case 1:state=2;break;
                case 2:state=1;break;
                case 3:state=4;break;
                case 4:state=3;break;
    }
    while(!is_node()){
        slow_trailing_move();
        delay(25);
    }
    wheel_stop();
    delay(500);
}


void slowslow_trailing_move()
{
    wheel_slowslowForward(front);
    wheel_slowslowForward(back);
    switch (read_trail(front)){
         case 0: switch(last_state[0][0]){
                    case 1:modify_slowslow(r_f_wheel_pwm);break;
                    case 3:modify_slowslow(l_f_wheel_pwm);break;
                 }
                 break;
         case 1: last_state[0][0]=1;last_state[1][0]=1;modify_slowslow(r_f_wheel_pwm);break;
         case 2: last_state[0][0]=2;last_state[1][0]=2;break;
         case 3: last_state[0][0]=3;modify_slowslow(l_f_wheel_pwm);last_state[1][0]=3;break;
    }
    switch (read_trail(back)){
                case 0: switch(last_state[0][1]){
                            case 1:modify_slowslow(l_b_wheel_pwm);break;
                            case 3:modify_slowslow(r_b_wheel_pwm);break;
                        }
                        break;
                case 1: last_state[0][1]=1;modify_slowslow(l_b_wheel_pwm);last_state[1][1]=1;break;
                case 2: last_state[0][1]=2;last_state[1][1]=2;break;
                case 3: last_state[0][1]=3;last_state[1][1]=3;modify_slowslow(r_b_wheel_pwm);break;
            }
    last_state[2][1]=read_trail(left);
    last_state[3][1]=last_state[2][1];
    last_state[2][0]=read_trail(right);
    last_state[3][0]=last_state[2][0];
}

