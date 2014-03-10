#!/bin/bash

set -e

. ./env.sh

NAME=gdb
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}server

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/gdb/gdbserver/configure --build=${BUILD} --host=${TARGET} --prefix=/usr
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} DESTDIR=${SYSROOT} install
