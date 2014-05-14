""" Author : Ikechukwu Ojuro - Electronics Engineering Class of 2015 UWI, Mona.
    Title: Aiming for PiCam module - Using OpenCV
    Description: Refactored Code...but inconsistent with format chosen for serial message exchanges between arduino and pi
"""

""" Uses PICAM if the picamera library is installed on the PI, otherwise uses USB VideoCapture Device"""

import serial
import numpy as np
import cv2
import time
import io
try:
    import picamera
    PICAM_IMPORTED = True
except ImportError:
    PICAM_IMPORTED = False

#config
PICAM_AVAILABLE = PICAM_IMPORTED
SERIAL_PORT = '/dev/ttyACM0'
DEBUG = False

#constants
FRAME_WIDTH = 160 #picam mounted sideways
FRAME_HEIGHT = 120 #picam mounted sideways
FRAME_CENTER_X = FRAME_WIDTH / 2
FRAME_CENTER_Y = FRAME_HEIGHT / 2
Y_OFFSET  = 35
X_OFFSET = 0
X_THRESHOLD = 5
Y_THRESHOLD = 0

#states
onTarget = False
noTarget = True
x_aimComplete = False
y_aimComplete = False

#serial
ser = serial.Serial(SERIAL_PORT, 9600)
ser.timeout = 3

#camera
cap = None

#Helper Functions
def sendData(string):
    global ser
    #delay if necessary
    ser.write(string)

def readData():
    global ser
    return ser.readline()

def pixelsToSteps(pixels):
    #set for a frame size of 160 x 120
    return constrain(pixels / 2)

def stepsToPixels(steps):
    #set for a frame size of 160 x 120
    return constrain(steps * 6)

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
         _, im = camera.read() #deal with opencv buffering
         _, im = camera.read()
         _, im = camera.read()
         if DEBUG:
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
        """print image.size
        rows, cols, h = image.size
        M = cv2.getRotationMatrix2D((cols/2, rows/2), -90, 1)
        image = cv2.warpAffine(image, M, (cols,rows))"""
        cv2.imshow("im", image)
        #cv2.waitKey(5)
        return image

#updated move function    
def move(Direction,steps):
    global ser
    ser.write(Direction)
    ser.write(str(steps).zfill(3))

    ard =  ser.readline()
    print "arduino says: ",ard
    return ard

#old move function
"""
def move(direction, pixels):
    val = pixelsToSteps(pixels)
    sendData(direction+str(chr(val)))
    x = readData()
    print x
    ard = int(x)
    print "To: ", val, pixels
    print "From: ", x
    while (ard == -1):
        time.sleep(0.5)
        sendData(direction+str(chr(val)))
        x = readData()
        ard = int(x)
        print "To: ", val, pixels
        print "From: ", x
    return ard
"""

def constrain(value):
    return min(255, max(0, value))

def yaw(pixels):
    if pixels > 0:
        return move("R", pixelsToSteps(pixels))
    else:
        return move("L", pixelsToSteps(-pixels))

def pitch(pixels):
    if pixels > 0:
        pixels = pixels * 10
        return move("D", pixelsToSteps(pixels))
    else:
        pixels = pixels * 10
        return move("U", pixelsToSteps(-pixels))

def initialize():
    global cap
    #cap = getCamera()

cap = getCamera()

#Main Functions
def getTargetOffset():
    global cap
    #Get a frame of the image
    im = getImage(cap)

    #Resize the frame to fit the PI if not using PICAM
    if not PICAM_AVAILABLE:
        im = cv2.resize(im, (FRAME_WIDTH, FRAME_HEIGHT))

    #make copies of original frame
    imm2 = im #For a clean reference later
    imm4 = im #Not Sure yet

    #Convert from BGR to HSV
    im = cv2.cvtColor(im,cv2.COLOR_BGR2HSV)

    #Filter Threshold Values
    l_col = np.array([0,118,111],np.uint8)
    u_col = np.array([16,255,255],np.uint8)

    #Create the mask/Filter the image
    mask = cv2.inRange(im,l_col,u_col)
    if DEBUG:
        cv2.imshow("mask", mask)
        cv2.waitKey(6)
    #Threshold image: Convert to Binary (1 for Target, 0 for rest)
    ret, thresh = cv2.threshold(mask,190,255,cv2.THRESH_BINARY)

    #Custom Kernel - 5x5 Block
    kernel = np.ones((3,3),np.uint8)

    #Morphology: Close - To Complete the red square (fill circle)
    imm5 = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

    #Morphology: Open - To reduce noise
    imm5 = cv2.morphologyEx(imm5, cv2.MORPH_OPEN, kernel)

    #Dilate image to remove noise
    imm5 =  cv2.dilate(imm5,kernel,iterations = 10)

    #Initialize cx, cy to -1 (Sentinel Value) for when target not on screen
    cy = -1
    cx = -1

    #Get Contours of Filtered Image
    contours, hierarchy = cv2.findContours(imm5,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)

    #Contours returned include many blocks, however, the one required is the largest one.
    #The following procedure will find and return the largest block
    
    #If any found
    if contours != []:
        
        cnt = contours[0]
        #adjust any out of shape image to a geometrical shape (square)
        epsilon = 0.001*cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,epsilon,True)

        #Max Width seen so far
        maxWidth =0

        #Max Height seen so far
        maxHeight =0

        #Top Left Coordinates for biggest contour found so far
        ysv = xsv = 0

        #Maximum area for target recognition
        MAREA = 0

        #Area of bounding rectangle of contour. For Thresholding.
        AA1 = 0

        #For Every Contour
        for i in xrange(0, len(contours)):
            
            cnt = contours[i]
            #Bounding Rectangle for contour
            x,y,w,h = cv2.boundingRect(cnt)

            #Draw Contour on image
            cv2.rectangle(imm2,(x,y),(x+w,y+h),(0,255,0),5)

            AA1 = w*h
            
            #if width of contour is greater then maxWidth, minimum height is 5, and height is greater than maximumHeight and Area is greater than 1500 pixels^2
            if w > maxWidth and h > 5 and h > maxHeight and AA1 > 900:
                xsv = x
                ysv = y
                maxWidth = w
                maxHeight = h
                MAREA = AA1
                

        #Calculate Coordinates of Center of Square (Biggest Contour found)
        cx = (xsv + (xsv + maxWidth)) / 2
        cy = (ysv + (ysv + maxHeight)) / 2

        if DEBUG:
            print"cx......",cx
            print"cy......",cy
            print"Area....",MAREA
            #cv2.imshow('iii',thresh)
            #cv2.imshow('inranged',imm5)
            #cv2.imshow('ranged',imm4)
            cv2.imshow('im2',imm2)    
            cv2.waitKey(5)
            
    return cx, cy


def aim():
    global onTarget, noTarget, x_aimComplete, y_aimComplete
    if not onTarget:
        cx, cy = getTargetOffset()
        pass
        cx, cy = getTargetOffset()
        

        #If Target not in frame at all
        if cx  == -1:
                if noTarget:
                    #print "Target Not Found, moving Left: ", move('L', stepsToPixels(80))
                    noTarget = False
                else:
                    #print "Target Not Found, moving Right: ",move('R', stepsToPixels(40))
                    noTarget = True
        else:
            #Find the difference between center of target and frame
            dy = cy - (FRAME_CENTER_Y + Y_OFFSET)
            dx = cx - (FRAME_CENTER_X + X_OFFSET)

            #Find the absolute values of those numbers
            absdx = dx if dx >= 0 else -dx
            absdy = dy if dy >= 0 else -dy

            #Correcting YAW:
            #If the difference is more than threshold: correct it
            if absdx - X_THRESHOLD > 0: #and not x_aimComplete
                print "yaw correction: ", absdx
                yawArd = yaw(dx)
                #print "Corrected", yawArd            
                x_aimComplete = False
            else:
                x_aimComplete = True
                
            #Correcting Pitch:
            #If the difference is more than threshold: correct it
            if absdy - Y_THRESHOLD > 0 and  x_aimComplete:
                print "pitch correction: ", absdy
                pitchArd = pitch(dy)
                #print "Corrected: ", dy               
                y_aimComplete = False
            else:
                y_aimComplete = True
                
        if x_aimComplete and y_aimComplete:
            ser.timeout = 10
            move('F',111)
            #readData()
            ser.timeout = 3
            return 'F'
        else:
            return 'A'

    

        

                
            
#Driver
def main():
    global noTarget, onTarget, aimComplete
    initialize()
    time.sleep(3)
    ser.write('G')
    print ser.readline()
    while 1:
        #cv2.imshow('im',getImage(cap))
        #cv2.waitKey(5)
        onTarget = False
        noTarget = True
        x_aimComplete = False
        y_aimComplete = False
        arduino = readData()
        while arduino != 'F':
            arduino = aim()
            #time.sleep(.2)
            
            #print "ard", arduino
    destroy()

main()

        
