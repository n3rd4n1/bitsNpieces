#!/bin/bash

. ./env.sh

${CROSS_COMPILE}gcc a.c &> /dev/null

echo

if [ $? -gt 0 ]
then
   echo "Oh no! Broken!"
else
   echo "Looks like everything went well! Rejoice!"
fi

rm a.out a.out.gdb &> /dev/null
