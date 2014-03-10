#!/bin/bash

set -e

. ./env.sh

NAME=cloog
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_CLOOG} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${WORKDIR_OUT} --disable-shared --build=${BUILD} --host=${HOST} --target=${TARGET} --disable-nls --with-ppl=${WORKDIR_OUT} --with-gmp=${WORKDIR_OUT} --disable-watchdog
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install
make ${MAKE_FLAGS} check

