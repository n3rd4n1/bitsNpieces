#!/bin/bash

set -e

LOGFILE=`pwd`/build.log

cd scripts

STARTTIME=$(date +%s)

echo "`date` BEGIN" > $LOGFILE

echo "`date` dev.sh" >> $LOGFILE
./dev.sh

echo "`date` prep.sh" >> $LOGFILE
./prep.sh

echo "`date` binutils.sh" >> $LOGFILE
./binutils.sh

echo "`date` kernel-headers.sh" >> $LOGFILE
./kernel-headers.sh

echo "`date` gcc-stage1.sh" >> $LOGFILE
./gcc-stage1.sh

echo "`date` uclibc-stage1.sh" >> $LOGFILE
./uclibc-stage1.sh

echo "`date` gcc-stage2.sh" >> $LOGFILE
./gcc-stage2.sh

echo "`date` uclibc-stage2.sh" >> $LOGFILE
./uclibc-stage2.sh

echo "`date` gcc-stage3.sh" >> $LOGFILE
./gcc-stage3.sh

echo "`date` elf2flt.sh" >> $LOGFILE
./elf2flt.sh

echo "`date` gdb.sh" >> $LOGFILE
./gdb.sh

echo "`date` gdbserver.sh" >> $LOGFILE
./gdbserver.sh

echo "`date` clean-up.sh" >> $LOGFILE
./clean-up.sh

echo "`date` sanity-check.sh" >> $LOGFILE
./sanity-check.sh

echo "`date` archive.sh" >> $LOGFILE
./archive.sh

ENDTIME=$(date +%s)
ELAPSED=$(($ENDTIME - $STARTTIME))
MINUTES=$(($ELAPSED / 60))
SECONDS=$(($ELAPSED % 60))
echo "`date` FINISHED in ${MINUTES}m${SECONDS}s" >> $LOGFILE

echo
echo "[Build Summary]"
cat $LOGFILE
echo

