#!/usr/bin/python
import subprocess
print "start"
subprocess.call("./sendTimeSync.sh", shell=True)
print "end"
