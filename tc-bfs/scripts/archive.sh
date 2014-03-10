#!/bin/bash

set -e

. ./env.sh

ARCHIVE=`pwd`/../${TARGET}.tar.gz

cd ${PREFIX}
tar cfz ${ARCHIVE} ${TARGET}
