#!/bin/bash

set -e

. ./env.sh

NAME=mpfr
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_MPFR} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${WORKDIR_OUT} --disable-shared --build=${BUILD} --target=${TARGET} --host=${HOST} --disable-nls --with-gmp=${WORKDIR_OUT}
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install
make ${MAKE_FLAGS} check

