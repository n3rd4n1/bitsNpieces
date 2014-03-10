#!/bin/bash

set -e

. ./env.sh

NAME=libelf
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_LIBELF} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${WORKDIR_OUT} --disable-shared --build=${BUILD} --target=${TARGET} --host=${HOST} --disable-nls
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

