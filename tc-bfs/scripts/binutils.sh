#!/bin/bash

set -e

. ./env.sh

NAME=binutils
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_BINUTILS} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --prefix=${INSTALL} --libdir=${WORKDIR_OUT}/lib --build=${BUILD} --host=${HOST} --target=${TARGET} --disable-nls --with-sysroot=${SYSROOT} --enable-poison-system-directories --disable-werror
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

