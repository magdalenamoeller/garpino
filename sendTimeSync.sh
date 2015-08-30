#!/bin/bash          
STR="Send time sync on Serial";
echo $STR;   

TZ_adjust=2;
d=$(date +%s);
t=$(echo "60*60*$TZ_adjust/1" | bc);
echo T$(echo $d+$t | bc ) > /dev/ttyUSB0
