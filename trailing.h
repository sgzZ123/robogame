/*******************
trailing.h

*******************/

#ifndef _TRAILING_H__
#define _TRAILING_H__

#include "Arduino.h"     


#define SensorFront1 32//前3传感器 
#define SensorFront2 33
#define SensorFront3 34
#define SensorBack1 35//后3传感器 
#define SensorBack2 36
#define SensorBack3 37
#define SensorRight1 38//右3传感器 
#define SensorRight2 39
#define SensorRight3 40
#define SensorLeft1 41//左3传感器 
#define SensorLeft2 42
#define SensorLeft3 43
#define SensorLeftFront 44//左前传感器
#define SensorLeftBack 45//左后传感器 
#define SensorRightFront 46//右前传感器
#define SensorRightBack 47//右后传感器



extern int SF1;
extern int SF2;
extern int SF3;//前3传感器状态 
extern int SB1;
extern int SB2;
extern int SB3;//后3传感器状态
extern int SR1;
extern int SR2;
extern int SR3;//右3传感器状态
extern int SL1;
extern int SL2;
extern int SL3;//左3传感器状态
extern int SLF;//左前传感器状态
extern int SLB;//左后传感器状态
extern int SRF;//右前传感器状态
extern int SRB;//右后传感器状态

void trailing_init();
int trailing_test (int sign1,int sign2,int sign3,int sign4,int sign5);
void modify(int pwmPin);
void trailing_move();
int read_trail(int car_direction);
int is_node();
void slow_trailing_move();
void modify_slow(int pwmPin);
void slow_back();
void slowslow_trailing_move();
void modify_slowslow(int pwmPin);
#endif



/******************************************************************************************************************/



