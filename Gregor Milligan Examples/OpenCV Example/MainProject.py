
#importing libraries
import cv2
import numpy as np
import sys
import os

#Global Variables 

drawing = False
ix,iy = -1,-1
mode = True
#The image that is being loaded
img = cv2.imread('c1.jpg')


#######colour Filter Variables

##Upper and Lower colour values the solution checks for (HSV)
#lower_red = np.array([0,70,70]) #-------------red
#upper_red = np.array([10,255,255])
lower_yellow = np.array([20,100,100]) #-------------yellow
upper_yellow = np.array([30,255,255])
#lower_blue = np.array([90,130,0]) #------------blue
#upper_blue = np.array([140,255,255])
#lower_green = np.array([65,60,60]) #----------green
#upper_green = np.array([80,255,255])
#Converting BGR to HSV
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
#Creating a mask from the colour values
mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
#incorporating smoothing filters so the colour filter is smoother
res= cv2.bitwise_and(img, img, mask=mask)
bilateral = cv2.bilateralFilter(res, 15,75,75)

#------------------Classifier-------------

#Calssifier file
cc_cascade = cv2.CascadeClassifier('cc.xml')
#converting to grayscale so the classifier can evaluate the image
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
cc = cc_cascade.detectMultiScale(gray, 2 ,3)
#Showing the holds in the image
for (x,y,w,h) in cc:

        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img, 'hold', (x, y), font, 0.5, (0,255,255), 2, cv2.LINE_AA)
        cv2.rectangle(img, (x,y), (x+w, y+h), (255,255,255), 2)

#---------Drawing / Pen Function------------------------

def draw_circle(event,x,y,flags,param):
    global ix,iy,drawing,mode
        #IF the left button is down then draw
    if event == cv2.EVENT_LBUTTONDOWN:
        drawing = True
        ix,iy = x,y
        #IF the mouse moves then draw where the pen moves
    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing == True:
            if mode == True:

                cv2.circle(img,(x,y),15,(0,0,0),-1)
        #If the left button is no longer being pressed stop drawing
    elif event == cv2.EVENT_LBUTTONUP:
        drawing = False

#-------Image Croping and Zooming----------------------------------
#Create a "region of image" window for the image
r = cv2.selectROI(img)
#The location of the selection
imgCrop = img[int(r[1]):int(r[1]+r[3]), int(r[0]):int(r[0]+r[2])]
fromCenter = False
resize = cv2.resize(imgCrop, (600,600)) #size of the image 

#The widnows that display the software
cv2.namedWindow('image') # displays the image 
cv2.setMouseCallback('image',draw_circle) #if mouse is clicked draw a circle  
cv2.imshow('Zoomed In', resize)#displays the zoomed in image 


while(1):
    cv2.imshow('image',img)
    cv2.imshow('filtered image', bilateral)
    cv2.setMouseCallback('image',draw_circle)    
        #if the key is pressed reload the windows 
    k = cv2.waitKey(1) & 0xFF
    if k == ord('m'):
        mode = not mode
    elif k == 27:
        break
        # when d is pressed all the windows are destroyed 
    elif k == ord("d"):
        img = cv2.imread('c2.jpg')
        r = cv2.selectROI(img)
        imgCrop = img[int(r[1]):int(r[1]+r[3]), int(r[0]):int(r[0]+r[2])]
        fromCenter = False
        r = cv2.selectROI(img, fromCenter)
        res = cv2.resize(imgCrop, (600,600))

        break


cv2.destroyAllWindows()
