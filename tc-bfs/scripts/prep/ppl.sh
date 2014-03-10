#!/bin/bash

set -e

. ./env.sh

NAME=ppl
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_PPL} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${WORKDIR_OUT} --disable-shared --build=${BUILD} --target=${TARGET} --host=${HOST} --disable-nls --with-libgmp-prefix=${WORKDIR_OUT} --with-gmp-prefix=${WORKDIR_OUT} CPPFLAGS=-I${WORKDIR_OUT}/include LDFLAGS=-L${WORKDIR_OUT}/lib --disable-watchdog
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

