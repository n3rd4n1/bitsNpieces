#!/bin/bash

set -e

. ./env.sh

NAME=gcc
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}-stage1

./helper-extract.sh ${RES_GCC} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --build=${BUILD} --host=${HOST} --target=${TARGET} --prefix=${INSTALL} --with-sysroot=${SYSROOT} --with-build-sysroot=${SYSROOT} --with-build-time-tools=${INSTALL}/bin --with-cpu=sh2a --with-multilib-list= --with-endian=big --disable-fdpic --disable-shared --disable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --enable-incomplete-targets --with-gnu-as --with-gnu-ld --enable-lto --disable-nls --disable-libgomp --without-headers --with-newlib --disable-decimal-float --disable-libffi --disable-libquadmath --disable-libitm --disable-libatomic --enable-languages=c --with-gmp=${WORKDIR_OUT} --with-mpfr=${WORKDIR_OUT} --with-mpc=${WORKDIR_OUT} --with-ppl=${WORKDIR_OUT} --with-cloog=${WORKDIR_OUT} --with-libelf=${WORKDIR_OUT} --enable-poison-system-directories '--with-host-libstdcxx=-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm'
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install
