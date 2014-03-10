#!/bin/bash

. ./env.sh

if [ "$1" == "all" ]
then
   sudo rm -rf ${PREFIX}
else
   rm -rf ${INSTALL}
fi
