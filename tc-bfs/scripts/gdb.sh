#!/bin/bash

set -e

. ./env.sh

NAME=gdb
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_GDB} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${INSTALL} --build=${BUILD} --host=${HOST} --target=${TARGET} --disable-sim --disable-libmcheck --disable-nls --with-libexpat-prefix=${WORKDIR_OUT}
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

