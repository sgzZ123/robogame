# -*- coding: utf-8 -*- 
'''
颜色特征识别测试代码
'''
import numpy as np
import cv2
import picamera
import serial    #import ser ial module
#import RPi.GPIO as GPIO
import os
import time
from time import sleep
from color_feature import color_block_finder,draw_color_block_rect

flag=[0,0,0,0,0,0,0,0]
state=[0,0,0,0]

Haverage=[0,0,0,0,0,0,0,0]





def leftstagecut(order):
    newimg = cv2.imread('panorama.jpg',cv2.IMREAD_COLOR)

    if(order==1):
        cut = newimg[1041:1273,37:149]
    elif(order==2):
        cut = newimg[1013:1245,553:649]
    elif(order==3):
        cut = newimg[989:1190,1009:1085]
    elif(order==4):
        cut = newimg[957:1145,1421:1489]
    elif(order==5):
        cut = newimg[945:1109,1733:1801]
    elif(order==6):
        cut = newimg[929:1077,2045:2097]
    elif(order==7):
        cut = newimg[907:1049,2301:2337]
    elif(order==8):
        cut = newimg[885:1033,2523:2565]

    
    #cv2.imshow('result', cut)
    cv2.imwrite('panorama-'+str(order)+'.jpg',cut)
    # 等待任意按键按下
    #cv2.waitKey(0)
    # 关闭其他窗口
    #cv2.destroyAllWindows()




def rightstagecut(order):
    newimg = cv2.imread('panorama.jpg',cv2.IMREAD_COLOR)
    print('check-order'+str(order))

    if(order==1):
        cut = newimg[921:1173,2429:2535]
    elif(order==2):
        cut = newimg[925:1137,1941:2053]
    elif(order==3):
        cut = newimg[905:1125,1513:1601]
    elif(order==4):
        cut = newimg[895:1089,1165:1237]
    elif(order==5):
        cut = newimg[901:1069,841:897]
    elif(order==6):
        cut = newimg[889:1061,545:605]
    elif(order==7):
        cut = newimg[877:1041,305:357]
    elif(order==8):
        cut = newimg[885:1033,89:129]

    
    #cv2.imshow('result', cut)
    cv2.imwrite('panorama-'+str(order)+'.jpg',cut)
    # 等待任意按键按下
    #cv2.waitKey(0)
    # 关闭其他窗口
    #cv2.destroyAllWindows()


def averageH():
    print('average')
    for k in range(1,9):

        Img=cv2.imread('panorama-'+str(k)+'.jpg',cv2.IMREAD_COLOR)
        HSV =cv2.cvtColor(Img,cv2.COLOR_BGR2HSV)
        H,S,V=cv2.split(HSV)
        sum=0

        for i in range (0,len(H)):
            for j in range(0,len(H[0])):
                sum=sum+H[i][j]

        size=len(H)*len(H[0])
        Haverage[k-1]=sum/size
    print('checkpoint0')
    print(Haverage)
    #print(sum/size)
    num=1
    for j in range(0,8):
        for i in range(0,8):
            if (Haverage[j]>Haverage[i]):
                num=num+1
        flag[j]=num
        num=1    
    print (flag)
    
    for j in range(0,8):
        if(flag[j]==2):
            if Haverage[j]>35:
                for i in range(0,8):
                    if flag[i]!=8:
                        flag[i]=flag[i]+1
                    else:
                        flag[i]=1

                print (flag)
    
if __name__ == "__main__":

    stage=0
    
    while True:
        try:
            ser = serial.Serial('/dev/ttyUSB0', 9600,timeout=1)
            break
        except:
            try:
                ser=serial.Serial('/dev/tty/USB1',9600,timeout=1)
                break
            except:
                pass
            
        
        
    print("check")   
    while True:
       stage=ser.read()
       if stage:
           ser.write('r')
           break
            
    camera = picamera.PiCamera()
    camera.resolution=(2592,1944)
    camera.brightness=55
    camera.capture('panorama.jpg')    
            
    if(stage=='l'):  
        for j in list(range(1,9)):
            leftstagecut(j)
            

            
        averageH()   
        for j in range(0,8):
            ser.write(str(flag[j]))
    elif(stage=='r'):
                
        for j in list(range(1,9)):
            rightstagecut(j)
        averageH()   
        for j in range(0,8):
            ser.write(str(flag[j]))
            file=open('log.txt','a')
            file.write(str(flag[j]))
    print('checkpoint1')
    ser.write('0')
    print('checkpoint2')
    while True:
        #print(ser.read())
        while True:
            while True:
                 try:
                     ser = serial.Serial('/dev/ttyUSB0', 9600,timeout=1)
                     break
                 except:
                     try:
                         ser=serial.Serial('/dev/tty/USB1',9600,timeout=1)
                         break
                     except:
                         pass
            response=ser.read()
            if (response=='1'):
                state[0]=1
                #print("receive 1")
            elif (response=='2'):
                state[1]=1
                #print("receive 2")
            elif (response=='3'):
                state[2]=1
                #print("receive 3")
            elif (response=='4'):
                state[3]=1
                #print("receive 4")
            elif(response=='l')or(response=='r'):
                #print("restart ")
                ser.write('r')
                break
            
            if(response):
                print('response',response)
        print (state)
        for j in list(range(1,9)):
            ser.write(str(flag[j-1]))
            print('flag again')

        ser.write('1')
        for j in list(range(0,4)):
            ser.write(str(state[j]))
        









    
   