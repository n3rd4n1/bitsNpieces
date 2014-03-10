#!/bin/bash

set -e

. ./env.sh

NAME=uclibc
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}-stage2

FLAGS="V=1 ARCH_CFLAGS= CPU_CFLAGS= KERNEL_SOURCE=${SYSROOT}/usr/ KERNEL_HEADERS=${SYSROOT}/usr/include/ CROSS_COMPILER_PREFIX=${INSTALL}/bin/${CROSS_COMPILE} RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/"

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
cp -r ${SRC}/*/. .
make ${MAKE_FLAGS} ${FLAGS}
make ${MAKE_FLAGS} ${FLAGS} PREFIX=${SYSROOT} install

