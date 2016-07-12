#!/usr/bin/python
import serial  
import time
import subprocess

COL = 1 # display graph for COL sensor
try:  
    arduino = serial.Serial('/dev/ttyUSB0', 57600)  
except:  
    print "Failed to connect on /dev/ttyUSB0"  

time.sleep(2) #seconds

garpinof = open('garpinof.dat', 'w')

i = 0;
while i < 100:
  try:    
    arduino.write('CGIVEHUM\n')    
    time.sleep(1)  
    str = arduino.readline() 
    print str 
  except:    
    print "Failed to send!"  

  list = str.split(';', 5 )
  print list[COL]

  garpinof.write(list[COL])
  garpinof.write('\n')

  i = i + 1;
  time.sleep(5) # delays for 5 seconds

#end of while

garpinof.close();


proc = subprocess.Popen(['gnuplot','-p'], 
                        shell=True,
                        stdin=subprocess.PIPE,
                        )
proc.stdin.write('plot "garpinof.dat" with lines\n')
proc.stdin.write('set t wxt persist\n') # graph will stay on the screen
proc.stdin.write('quit\n') #close the gnuplot window






