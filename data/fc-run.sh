#!/bin/sh

LOGFILE=$HOME/tmp/fc-run.log

echo "Running fullcircle toolchain"
echo "got commandline parameters $*"
date >> $LOGFILE
echo $* >> $LOGFILE
echo "getting id..."
ID=`fc-getid`
echo "using ID ${ID}" >> $LOGFILE
echo "printing badge."
fc-badge -n ${ID}

#TODO: Render Perlin.
