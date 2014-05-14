""" Author : Chadwick Barclay - Electronics Engineering Class of 2015 UWI, Mona.
    Title: Aiming Using SimpleCV
    Description: Target Recognition system.
"""

import SimpleCV
import math
from SimpleCV import Image,Camera, Color, Display, DrawingLayer, Blob
# Initialization & Declarations
debug = True 
display = SimpleCV.Display()
cam = SimpleCV.Camera()
normaldisplay = True
count =1
# Read image stream 
size = cam.getImage().size()
disp = Display(size)
center = (size[0] /2, size[1] / 2)

 
while display.isNotDone():
 
	if display.mouseRight:
                # Labeling the display windows
		normaldisplay = not(normaldisplay)
		print "Display Mode:", "Normal" if normaldisplay else "Segmented" 
	#Flip image horizantally
	img = cam.getImage().flipHorizontal()
	# Draw a blue circle in the center of the video frame
	img.drawCircle(center,10,SimpleCV.Color.BLUE,10)
        # Red colour filter in HSV  
	redDist = img.hueDistance((255,0,0))
	# binarize image then erode the inverted image
	redDistBin = redDist.binarize(0).invert().erode(6)
	onlyRed =  redDistBin
	# find red blob
	blobs = onlyRed.findBlobs(100,200)
	if blobs:
                # filter circular blob
		circles = blobs.filter([b.isCircle(.267) for b in  blobs])
		if circles:
                                #Draw circle within the circular blob
				blobs[-1].draw(color=Color.BLACK,width=-1,alpha=128)     
				img.drawCircle((circles[-1].x, circles[-1].y), circles[-1].radius(),SimpleCV.Color.BLACK,3)
				print "x:" + str ((circles[-1].x)-320 ) + "  y:" + str(240 -(circles[-1].y))
				x1 = float(((circles[-1].x)-(size[0]/2)))
				y1 = float(((size[1]/2))-(circles[-1].y) )
				
													
	if debug:
        # Display processed image
	    if normaldisplay:
		    img.show()
		    
	    else:
		    onlyRed.show()

