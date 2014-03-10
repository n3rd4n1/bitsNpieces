#!/bin/bash

set -e

. ./env.sh

NAME=kernel
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_KERNEL} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
cp -r ${SRC}/*/. .
mkdir -p ${SYSROOT}/usr/include
make ${MAKE_FLAGS} ARCH=${ARCH} CROSS_COMPILE=${INSTALL}/bin/${CROSS_COMPILE} INSTALL_HDR_PATH=${SYSROOT}/usr headers_install

