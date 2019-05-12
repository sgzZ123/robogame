#include "Rfid_D_Think_M50.h"

// 待读取的块号 = 0
#define BLOCK_NO  0

// 使用 硬件串口Serial1 与 Rfid 模块通信
Rfid_D_Think_M50 rfid( Serial1 );
// 用于存放读取结果，一块=4字节
    uint8_t data[4];
int rfid_init(){
    //每隔100ms初始化一次
    while( !rfid.enableAntenna( rfid.ISO15693 ) )
        delay(100);
}
int read_rfid(){
    //读取，结果存在data数组里
    //success表示读到没有
    int success=0;
    rfid.enableAntenna();
    // 尝试读取第 BLOCK_NO 块数据
    if( rfid.read(BLOCK_NO, data) ){
        success=1;
    }
    else{
        // 若读取失败
        ;
    }
    // 读操作之后关闭天线，否则芯片发热
    rfid.disableAntenna();
    

    if(success)
        return(1);
    return(0);
}
/***************************************************************************************************************************************************************************************************************/
#include "myservo.h"
#include "us016.h"
#include "wheel.h"
#include "trailing.h"
#include "climber.h"
#include "pusher.h"
#include "forceSensor.h"
/***************************************************************************************************************************************************************************************************************/
int stage = 2;      //场地，1表示左边的，2表示右边的
int node = 0;       //当前在第几个节点（已过第几个节点
int flags[9]={0};   //1表示红，2表示专，3表示并，4表示进……，flags[i]表示第i+1个节点中的旗帜信息
int hand_state[2] ={0};  //抓手1、2有无抓着物体
int flagin[2];//抓手1、2中旗帜
int success[4]={0};//场地上从边缘到中间是否成功
int low_success=0;
int i;
long starttime;
long endtime;
int dis1;
int dis2;
//计时器，用于保证超声波不会再不合适的距离停下
void timer_init(){
    starttime=millis();
    endtime=millis();
}
//从节点进入通道抓起物体并出来，到最近的节点
void get_flag(int hand){
    flagin[hand-1]=flags[node-1];
    //调整移动方向
    state = stage+2;
    //调整抓手方向
    switch(stage){
        case 1: switch(hand){
                    case 1:platform_rotate(hand1_left);platform_rotate(hand1_left);break;
                    case 2:platform_rotate(hand1_right);platform_rotate(hand1_right);break;
                }
                break;
        case 2: switch(hand){
                    case 1:platform_rotate(hand1_right);platform_rotate(hand1_right);break;
                    case 2:platform_rotate(hand1_left);platform_rotate(hand1_left);break;
                }
                break;
    }
    //放下抓手
    switch(hand){
        case 1:open_hand1();hand1_down();break;
        case 2:open_hand2();hand2_down();break;
    }
    //进入
    //初始化计时器并调整阈值
    timer_init();
    if(hand==1)
        while(us016(stage+2)>7||endtime-starttime<1600){
            trailing_move();
            endtime=millis();
            delay(25);
        }
    else
        while(us016(stage+2)>16||endtime-starttime<1400){
            trailing_move();
            endtime=millis();
            delay(25);
        }
    wheel_stop();
    //抓
    switch(hand){
        case 1:close_hand1();delay(1000);hand1_up();delay(500);open_hand1();delay(75);close_hand1();break;
        case 2:close_hand2();delay(1000);hand2_up();delay(500);break;
    }
    //调整移动方向准备返回
    state=5-stage;
    //返回至最近的节点
    while(!is_node())
        trailing_move();
    read_rfid();
    if(hand==1&&!hand_state[1]){
        if(stage==1)
            platform_rotate(hand1_right);
        else
            platform_rotate(hand1_left);
    }
    else if(hand==2&&!hand_state[0]){
        if(stage==1)
            platform_rotate(hand1_left);
        else
            platform_rotate(hand1_right);
    }
    wheel_stop();
    delay(300);
    slow_back();
    node=data[1];
    hand_state[hand-1]=1;
}
//放下物块(对于矮杆来说包括了进去,出来，出来不停）
void lay_flag(int hand,int dir){
    hand_state[hand-1]=0;
    //假如放的是矮杆中的一个
    if(!success[0]||!success[1]){
        int closedis;
        //初始化计时器
        timer_init();
        state=1;
        //对于抓手1
        if(hand==1){
            platform_rotate(hand1_front);
            if(node==0x08||((flags[8]-(stage-1)*4)==1)||((flags[8]-(stage-1)*4)==2)){
                while((endtime-starttime<1550||us016(front)>19)&&(endtime-starttime<1600)){
                    trailing_move();
                    endtime=millis();
                    delay(25);
                }
            }
            else{
                while((endtime-starttime<1450||us016(front)>19)&&(endtime-starttime<1600)){
                    trailing_move();
                    endtime=millis();
                    delay(25);
                }
            }
            wheel_stop();
            platform_rotate(hand1_front);delay(1500);hand1_down();delay(500);open_hand1();delay(100);hand1_up();delay(300);platform_rotate(hand1_left);hand_state[0]=0;
        }
        //对于抓手2
        else{
            platform_rotate(hand1_back);
            if(node==0x08||((flags[8]-(stage-1)*4)==1)||((flags[8]-(stage-1)*4)==2)){
                while((endtime-starttime<1300||us016(front)>38)&&(endtime-starttime<1375)){
                    trailing_move();
                    endtime=millis();
                    delay(25);
                }
            }
            else{
                while((endtime-starttime<1100||us016(front)>38)&&(endtime-starttime<1200)){
                    trailing_move();
                    endtime=millis();
                    delay(25);
                }
            }
            wheel_stop();
            platform_rotate(hand1_back);delay(1500);hand2_down();delay(500);open_hand2();delay(100);hand2_up();delay(300);platform_rotate(hand1_left);hand_state[1]=0;
        }
        //返回，返回速度需要设定，到节点处
        state=2;
        while(!is_node()){
            trailing_move();
            delay(25);
        }
    }
    //当放的是高杆，不负责进入，只负责抓手部分
    else{
        switch(hand){
            case 1:platform_rotate(hand1_front);delay(500);platform_rotate(hand1_front);delay(1500);hand1_down();delay(1000);open_hand1();delay(500);hand1_up();delay(1000);hand_state[0]=0;break;
            case 2: if(dir==1){
                        platform_rotate(77);//往右边高杆放
                        delay(500);
                        platform_rotate(77);
                    }
                    else{
                        platform_rotate(162);//往左边
                        delay(500);
                        platform_rotate(162);
                    }
                    delay(1500);hand2_down();delay(500);open_hand2();delay(500);hand2_up();delay(500);hand_state[1]=0;break;
        }
    }
}
//不调整方向，到指定节点，不停
void go_straight_to(int aim){
    int present_node=node;
    int dir=node>aim?1:-1;
    //跑着
    while(1){
        trailing_move();
        if(is_node()){
            read_rfid();
            node=data[1];
            //到达目标后停
            if(node==aim){
                break;
            }
            else if((node-aim)*dir<0){
                wheel_stop();
                delay(300);
                switch(state){
                    case 1:state=2;break;
                    case 2:state=1;break;
                    case 3:state=4;break;
                    case 4:state=3;break;
                }
            }
        }
        delay(25);//担心去掉会影响rfid，所以保留
    }
}

void setup() {
   //put your setup code here, to run once
  //初始化各个模块
  //31口装一个开关负责重启
  //30口是继电器
  pinMode(30,OUTPUT);
  pinMode(31,INPUT);
  digitalWrite(30,LOW);
    servo_init();
    hand1_up();
    hand2_up();
    if(stage==2)
        platform_rotate(stage2photo);
    else
        platform_rotate(stage1photo);
    wheel_init();
    trailing_init();
    pusher_init();
    climber_init();
    rfid_init();
    Serial.begin(9600);
   
    //初始化方向并推出推杆
    state = stage+2;
    pusher_out();
    
    //等待树莓派启动
    //发送舞台号
    char c='n';
    while(!Serial.available()){
        if(stage==1)
            Serial.print('l');
        else
            Serial.print('r');
        delay(500);
    }
    //读掉信息
    Serial.read();
    int num=0;
    //接受旗帜信息
    for(i=0;i<8;i++){
        while(!Serial.available());
        c=Serial.read();
        //检测返回
        if(c>='1'&&c<='8')
            flags[num]=c-48;
        else
            flags[num]=0;
        if(num<6)
            num++;
        else
            num=8;
    }
    //接受是否重启的信息，假如这次是重启过的，那么就接受已经成功了哪几个
    while(!Serial.available());
    c=Serial.read();
    if(c-'0'){
        for(i=0;i<4;i++){
            while(!Serial.available());
            c=Serial.read();
            success[i]=c-'0';
        }
    }
    //在31口装1个开关，负责启动
    while(digitalRead(31)!=HIGH);
    switch(stage){
        case 1:platform_rotate(hand1_left);break;
        case 2:platform_rotate(hand1_right);break;
    }

    //如果矮杆有至少一个没成功，就先去拿矮杆
    if(!success[0]||!success[1]){
        low_success=1;
        //如果0x01里面有旗，就先去拿
        if( (((flags[0]-(stage-1)*6) == 1) && !success[stage-1]) || (((flags[0]-(stage-1)*6) == 2)&& !success[2-stage])){
            go_straight_to(0x01);
            get_flag(hand_state[0]+1);
        }
        //没有就到0x01停
        else{
            go_straight_to(0x01);
            wheel_stop();
            delay(300);
            slow_back();
        }
        //向前跑，预先看好下一个节点里面是不是要的旗，是的话就减速，不是就快速通过，到0x07退出循环
        state=1;
        for(i=1;i<9;i++){
            if((((flags[i]-(stage-1)*6) == 1) && !success[stage-1]) || (((flags[i]-(stage-1)*6) == 2)&& !success[2-stage])){
                break;
            }
        }
        go_straight_to(i+1);
        wheel_stop();
        delay(300);
        slow_back();
        get_flag(hand_state[0]+1);
        state=1;
        for(i=i+1;i<9;i++){
            if((((flags[i]-(stage-1)*6) == 1) && !success[stage-1]) || (((flags[i]-(stage-1)*6) == 2)&& !success[2-stage])){
                break;
            }
        }
        if(i<8){
            go_straight_to(i+1);
            wheel_stop();
            delay(300);
            slow_back();
            get_flag(hand_state[0]+1);
            state=1;
        }
        //停下推杆
        pusher_stop();
        //若0x09里面有旗帜，就去拿，出来会在0x08，直接前进去放旗，出来再放0x09的旗
        if( (((flags[8]-(stage-1)*6) == 1) && !success[stage-1]) || (((flags[8]-(stage-1)*6) == 2)&& !success[2-stage])){
            go_straight_to(0x09);
            wheel_stop();
            delay(300);
            slow_back();
            get_flag(hand_state[0]+1);
            if(!success[0]){
                //进入放旗
                if(stage==1)
                    lay_flag(flagin[0]==1?1:2,0);
                else
                    lay_flag(flagin[0]==8?1:2,0);
                success[0]=1;
                Serial.print(1);
                wheel_stop();
                delay(300);
                slow_back();
            }
            //到节点9
            state=5-stage;
            go_straight_to(0x09);
            wheel_stop();
            delay(300);
            slow_back();
            if(!success[1]){
                //进入放旗
                if(stage==1)
                    lay_flag(flagin[0]==2?1:2,0);
                else
                    lay_flag(flagin[0]==7?1:2,0);
                success[1]=1;
                Serial.print(2);
            }
        }
        //如果0x09里面不是要的旗，就直接不再0x09停，先放0x09的旗，再放0x08的
        else{
            if(!success[1]){
                go_straight_to(0x09);
                if(stage==1)
                    lay_flag(flagin[0]==2?1:2,0);
                else
                    lay_flag(flagin[0]==7?1:2,0);
                success[1]=1;
                Serial.print(2);
            }
            else{
                go_straight_to(0x09);
            }
           wheel_stop();
            delay(300);
            slow_back();
            if(!success[0]){
                state=stage+2;
                go_straight_to(0x08);
                wheel_stop();
                delay(300);
                slow_back();
                if(stage==1)
                    lay_flag(flagin[0]==1?1:2,0);
                else
                    lay_flag(flagin[0]==8?1:2,0);
                success[0]=1;
                Serial.print(1);
                wheel_stop();
                delay(300);
                slow_back();
                state=5-stage;
                go_straight_to(0x09);
                wheel_stop();
                delay(300);
                slow_back();
            }
        }
    }
    //检索高杆旗帜（在较小节点里的）
    for(i=0;i<9;i++){
        if(stage==1){
            if((flags[i]==3&&!success[2])||(flags[i]==4&&!success[3]))
                break;
        }
        else{
            if((flags[i]==5&&!success[3])||(flags[i]==6&&!success[2]))
                break;
        }
    }
    //假如这次放过矮杆，也就是说现在在0x09
    if(low_success){
        state=2;
        go_straight_to(i+1);
        wheel_stop();
        delay(300);
        slow_back();
        switch(stage){
            case 1: get_flag(flags[i]-2);break;
            case 2: get_flag(7-flags[i]);break;
        }
    }
    //否则，就在启动区，先跑到0x01
    else{
        state=stage+2;
        //假如0x01里面是要的旗
        if(i==0){
            go_straight_to(0x01);
            switch(stage){
                case 1: get_flag(flags[i]-2);break;
                case 2: get_flag(7-flags[i]);break;
            }
        }
        //不是的话，就去i+1节点拿旗
        else{
            go_straight_to(0x01);
            wheel_stop();
            delay(300);
            slow_back();
            state=1;
            go_straight_to(i+1);
            wheel_stop();
            delay(300);
            slow_back();
            switch(stage){
                case 1: get_flag(flags[i]-2);break;
                case 2: get_flag(7-flags[i]);break;
            }
        }
    }
    //再次检索需要旗帜
    for(i+=1;i<9;i++){
        if(stage==1){
            if((flags[i]==3&&!success[2])||(flags[i]==4&&!success[3]))
                break;
        }
        else{
            if((flags[i]==5&&!success[3])||(flags[i]==6&&!success[2]))
                break;
        }
    }
    //跑去拿
    state=1;
    go_straight_to(i+1);
    wheel_stop();
    delay(300);
    slow_back();
    switch(stage){
        case 1: get_flag(flags[i]-2);break;
        case 2: get_flag(7-flags[i]);break;
    }
    //如果不是在0x09里面拿的，也就是说现在不在0x08
    if(node!=0x08){
        state=1;
        //如果现在不是0x09，就先跑到0x09
        go_straight_to(0x09);
        wheel_stop();
        delay(300);
        slow_back();
    }
    //这种情况现在在0x08，直接往场地中间跑就行
    else{
        state=5-stage;
        go_straight_to(0x09);
    }
    state=5-stage;
    go_straight_to(0x10);
    wheel_stop();
    delay(300);
    slow_back();
    //放旗
    state=1;
    timer_init();
    //抱杆并爬
    pusher_in();
    digitalWrite(30,HIGH);
    climb();
    //等待压力传感器信号
    timer_init();
    while((!forceCheck()||endtime-starttime<8000)&&endtime-starttime<15000){
        endtime=millis();
        delay(300);
    }
    //停止上升
    climber_stop();//会自锁
    lay_flag(1,0);
    if(stage==1)
        lay_flag(2,1);//高处放旁边那个
    else
        lay_flag(2,2);
}

void loop() {
  // put your main code here, to run repeatedly:
    
}
