#!/usr/bin/python
import serial
import syslog
import time
import urllib2
import datetime


#The following line is for serial over GPIO
port = '/dev/ttyUSB0'
baseURL = 'https://api.thingspeak.com/update?api_key=NLPQVVN8W0P48X42'

ard = serial.Serial(port,57600,timeout=5)
time.sleep(4) #seconds

#!
seconds = int(round(time.time()))
#seconds = datetime.datetime.now() //TODO: localtime instead of utc
msg = 'T' + str(seconds) + '\n'

while (1):
    # Serial write section

    #ard.flush()

    #ard.write('CSTART\n')
    #time.sleep(4)
    #ard.write('CSTOP\n')

    print ("Value sent: ")
    print (msg)
    ard.write(msg)


    time.sleep(4) #seconds

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
	humidity = response.split(';')[1:4]
	print humidity
	f = urllib2.urlopen(baseURL + "&field1=%s&field2=%s&field3=%s" % humidity)
	f.close()
    else:
        msg = 'CGIVEHUM\n'
        print 'Unexpected response:'
	print response


#else:
    #ard.close()
    #print "Exiting"
exit()
