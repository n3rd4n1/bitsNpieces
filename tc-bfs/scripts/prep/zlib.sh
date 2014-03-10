#!/bin/bash

set -e

. ./env.sh

NAME=zlib
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_ZLIB} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
cp -r ${SRC}/*/. .
./configure --prefix=${WORKDIR_OUT}
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

