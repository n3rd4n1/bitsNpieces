#!/bin/bash

set -e

. ./env.sh

NAME=gmp
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_GMP} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${WORKDIR_OUT} --disable-shared --build=${BUILD} --target=${BUILD} --host=${HOST} --enable-cxx --disable-nls
make ${MAKE_FLAGS} CFLAGS='-g -O2'
make ${MAKE_FLAGS} install
make ${MAKE_FLAGS} check

