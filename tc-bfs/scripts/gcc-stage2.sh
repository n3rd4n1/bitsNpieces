#!/bin/bash

set -e

. ./env.sh

NAME=gcc
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}-stage2

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --build=${BUILD} --host=${HOST} --target=${TARGET} --prefix=${INSTALL} --with-sysroot=${SYSROOT} --with-build-sysroot=${SYSROOT} --with-build-time-tools=${INSTALL}/bin --with-cpu=sh2a --with-multilib-list= --with-endian=big --disable-fdpic --disable-shared --enable-threads=posix --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --enable-incomplete-targets --with-gnu-as --with-gnu-ld --enable-lto --disable-nls --disable-libffi --disable-libquadmath --disable-libatomic --enable-languages=c --with-gmp=${WORKDIR_OUT} --with-mpfr=${WORKDIR_OUT} --with-mpc=${WORKDIR_OUT} --with-ppl=${WORKDIR_OUT} --with-cloog=${WORKDIR_OUT} --with-libelf=${WORKDIR_OUT} --disable-libgomp --disable-libitm --enable-poison-system-directories '--with-host-libstdcxx=-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm'
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

