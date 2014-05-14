""" Author : Ikechukwu Ojuro - Electronics Engineering Class of 2015 UWI, Mona.
    Title: Aiming for PiCam module using opencv
    Description: First Draft of Aiming Code for PiCAM....needs to be refactored.
"""

""" Uses PICAM if the picamera library is installed on the PI, otherwise uses USB VideoCapture Device"""

import serial
import numpy as np
import cv2
#import cv2 as cv
import time
import math
import io

try:
    import picamera
    PICAM_IMPORTED = True
except ImportError:
    PICAM_IMPORTED = False

PICAM_AVAILABLE = PICAM_IMPORTED #Set to False to use USB VideoCapture




#Globals
FRAME_WIDTH = 160
FRAME_HEIGHT = 120

v_move =0
im = None
noTarget = True
onTarget = False
debug = True

#send data for arduino movement
def send_M_Data(Direction,steps):
    """Sends data to arduino over serial;
    Format: Direction Letter and Three Digit number with padded 0s at the front. 
    Example: L010, R100"""
    ser.write(Direction,steps) #Write Direction
    ser.write(str(steps).zfill(3)) #Send Steps
    print ser.readline() #Wait for Arduinos acknowledgment

def getCamera(cam_type = PICAM_AVAILABLE ):
    if not cam_type:
        return cv2.VideoCapture(0)
    else:
        cam = picamera.PiCamera()
        cam.resolution = (FRAME_WIDTH, FRAME_HEIGHT)
        cam.start_preview()
        return cam
        
def destroy(cam_type = PICAM_AVAILABLE):
    global cap
    if not cam_type:
        cap.release()
    cv2.destroyAllWindows()

def getImage(camera = cap, cam_type = PICAM_AVAILABLE):
    if not cam_type:
         _, im = camera.read()
         cv2.imshow("im", im)
         cv2.waitKey(5)
         return im
    else:
        stream = io.BytesIO()
        camera.capture(stream, 'jpeg')
        # Construct a numpy array from the stream
        data = np.fromstring(stream.getvalue(), dtype=np.uint8)
        # "Decode" the image from the array, preserving colour
        image = cv2.imdecode(data, 1)
        cv2.imshow("im", image)
        cv2.waitKey(5)
        return image


def initialize():
    global cap
    cap = getCamera()

    
def getTargetOffset():
    global im, debug, cap

    im = getImage(cap)
    imm2 = im
    imm4 = im

    im = cv2.cvtColor(im,cv2.COLOR_BGR2HSV)

    l_col = np.array([0,81,102],np.uint8)
    u_col = np.array([239,255,255],np.uint8)

    mask = cv2.inRange(im,l_col,u_col)

    imm3 = mask
    imm5 = mask

    ret, thresh = cv2.threshold(mask,190,255,cv2.THRESH_BINARY)

    kernel1 = np.ones((5,5),np.uint8)
    kernel = np.ones((5,5),np.uint8)
   
    
    #imm5 = cv2.erode(imm5,kernel,iterations=3)
    imm5 = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel1)
    imm5 = cv2.morphologyEx(imm5, cv2.MORPH_OPEN, kernel)
    
    imm5 =  cv2.dilate(imm5,kernel,iterations = 5)
    cy = -1
    cx = -1
    
    j = 0
    contours, hierarchy = cv2.findContours(imm5,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    #print type(contours)
    if contours != []:
        cnt = contours[0]
        epsilon = 0.001*cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,epsilon,True)

        maxw =0
        maxh =0
        ysv = 0
        xsv = 0
        AA1 = 0
 
            

    
        #if contours.any() == True:
        for i in xrange(0, len(contours)):
            cnt = contours[i]
            x,y,w,h = cv2.boundingRect(cnt)
            cv2.rectangle(im,(x,y),(x+w,y+h),(0,255,0),5)


            j += 1
            AA1 = w*h

            #if y > 300:
            if y > ysv:
               #if w < 7*h:
                 if w > maxw:
                   if h > 5:  
                    if h > maxh:
                         if AA1 > 1500:  
                            if debug:
                                print "Area   ", AA1
                            xsv = x
                            ysv = y
                            maxw = w
                            maxh = h

            cx = (xsv + (xsv+maxw))/2
            cy = (ysv + (ysv+maxh))/2
            #if debug:
               #print xsv,"  x"
               # print ysv,"  y"

        if debug:
            cv2.circle(imm2,(320,300),10,(0,0,255),5)    
            #cv2.circle(imm2,(cx,cy),10,(0,0,255),5)
            cv2.line(imm2,(cx,0),(320,485),(255,00,00),2)

            #print maxw, maxh

            cv2.rectangle(imm2,(xsv,ysv),(xsv+maxw,maxh+ysv),(255,0,0),5)
    if debug:
        cv2.imshow('iii',thresh)
        cv2.imshow('inranged',imm5)
        cv2.imshow('ranged',imm4)
        cv2.imshow('im2',imm2)    

        k = cv2.waitKey(5)

        #if k ==27:
          #//  break

    return cx, cy


def pixelsToSteps(pixels):
    """Utility Function to convert pixels to steps"""
    val = pixels / 2
    return val

def aim():
    global debug
    global v_move
    global onTarget 
    global noTarget 
    ud = 0
    if not onTarget:
        cx, cy = getTargetOffset()


        #happens if target not in sight (pan system to the left first to search and then to far right if not found
        if cx < 1:
            if noTarget == True:
                #move camera to the right by set amount (double)
                send_M_Data('L',30)
                noTarget = False
                
            elif noTarget == False:
                #move camera to the leftt by set amount
                send_M_Data('R',15)
                noTarget = True

        if debug:
            print cx,"   cx"
            print cy,"   cy"

        #ser.flush()
        #print ser.readline()
        err = 10                
        if ud == 0:
          if cx > 0:  
           if cy > 0: 
            if cx < ((FRAME_WIDTH/2)- err):
                cm = ((FRAME_WIDTH/2) - cx)#%25
                #cm = cm  * 15
                send_M_Data('L',cm)
                onTarget = False

                if debug:
                    print pixelsToSteps(cm)
                    print 'L      val'
                    v_move += 1
                         
            elif cx > ((FRAME_WIDTH/2)+ err):
                cm = (cx - (FRAME_WIDTH/2))#%25
                #cm = cm  * 15
                send_M_Data('R',cm)
                onTarget = False
                
                if debug:
                    print pixelsToSteps(cm)
                    print 'R      val'
                                              
            elif ((FRAME_WIDTH/2)+ err) >= cx >= ((FRAME_WIDTH/2)- err):
                #ser.write('J')
                #if debug:
                   # print v_move
                ud = 99
                
        err = 5 
        if ud == 99:
            if cy < (((FRAME_HEIGHT/2) + 20) - err):
                 cm = (((FRAME_HEIGHT/2) + 20) - cy)%25
                 cm = cm * 30
                 send_M_Data('D',cm)                 
                 ud = 0
                 onTarget = False

                 if debug:
                     print pixelsToSteps(cm)
                     print "D          val"
                                       
                 
                
            elif cy > (((FRAME_HEIGHT/2) + 20) + err) :
                 # while(ser.readlines() != "H"):
                 cm = (cy - ((FRAME_HEIGHT/2) + 20))%25
                 cm = cm * 30
                 send_M_Data('U',cm)
                 ud = 0
                 onTarget = False
                 

                 if debug:
                     print pixelsToSteps(cm)
                     print "U         val"
                 
                 
                             
            elif (((FRAME_HEIGHT/2) + 20) + err) >= cy >= (((FRAME_HEIGHT/2) + 20) - err):
                #while(ser.readlines(4) != "L"):
                
                onTarget2 = True
                ud = 0
                if onTarget2:
                    onTarget = True
                    ser.write('F')
                    ser.write(str(pixelsToSteps(1)).zfill(3))
                    print ser.readline()

                if debug:
                     #print pixelsToSteps(cm)
                     print "F          val"      
    


#MAIN FUNCTION...should be in a function of its own
cap = cv2.VideoCapture(0)
ser = serial.Serial('/dev/ttyACM0',9600)
ser.timeout = 3 

time.sleep(2) #to give arduino time to initialise

while(1):
    #raw_input("Enter to get x, y centre offsets: ")
    aim()
    #print getTargetOffset()
    time.sleep(.2)
    
    '''
    command = serial.readline()
    if command == 'J':
        aim()
    ser.flush()
    '''

    
cv2.destroyAllWindows()
cap.release()
ser.close()



        
    
    

