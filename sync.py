#!/usr/bin/python
import serial
import syslog
import time

#The following line is for serial over GPIO
port = '/dev/ttyUSB0'


ard = serial.Serial(port,57600,timeout=5)
time.sleep(2) #seconds

#!
millis = int(round(time.time() * 1000))
msg = 'T' + str(millis) + '\n'

while (1):
    # Serial write section

    ard.flush()

    #ard.write('CSTART\n')
    #time.sleep(4)
    #ard.write('CSTOP\n')

    print ("Value sent: ")
    print (msg)
    ard.write(msg)


    time.sleep(1) #seconds

    # Serial read section
    response = ard.readline()
    #msg = ard.read(ard.inWaiting()) # read all characters in buffer
    print ("Message from arduino: ")
    print (response)

    time.sleep(1) #seconds, not tooo long, because then seems that port closes


    #decide about next message
    if('waiting for sync message' in str(response) ):
        millis = int(round(time.time() * 1000))
	msg = 'T' + str(millis) + '\n'
    elif(len(str(response)) > 0 and str(response)[0].isdigit()):
        #parse values and send over network
 	msg = 'CGIVEHUM\n'
        print response
    else:
        msg = 'CGIVEHUM\n'
        print 'Unexpected response:'
	print response


#else:
    #ard.close()
    #print "Exiting"
exit()
