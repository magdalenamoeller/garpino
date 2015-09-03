#!/usr/bin/python
import serial  
import time

try:  
    arduino = serial.Serial('/dev/ttyUSB0', 57600)  
except:  
    print "Failed to connect on /dev/ttyUSB0"  

time.sleep(2) #seconds

try:    
    arduino.write('CGIVEHUM\n')    
    time.sleep(1)  
    print arduino.readline()  
    print arduino.readline()  
except:    
    print "Failed to send!"  
