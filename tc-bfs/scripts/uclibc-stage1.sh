#!/bin/bash

set -e

. ./env.sh

NAME=uclibc
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}-stage1

./helper-extract.sh ${RES_UCLIBC} ${SRC}

cd ${SRC}/*
cp ${UCLIBC_CONFIG} .config
make oldconfig
cp .config ${UCLIBC_CONFIG}

FLAGS="V=1 ARCH_CFLAGS= CPU_CFLAGS= KERNEL_SOURCE=${SYSROOT}/usr/ KERNEL_HEADERS=${SYSROOT}/usr/include/ CROSS_COMPILER_PREFIX=${INSTALL}/bin/${CROSS_COMPILE} RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/"

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
cp -r ${SRC}/*/. .
mkdir -p ${SYSROOT}/usr/include
make ${MAKE_FLAGS} ${FLAGS} headers
make ${MAKE_FLAGS} ${FLAGS} PREFIX=${SYSROOT} install_headers


