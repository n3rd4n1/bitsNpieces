#!/bin/bash

. ../modify.me

sudo cat /dev/null

INSTALL=${PREFIX}/${TARGET}
SYSROOT=${INSTALL}/${TARGET}/libc
WORKDIR=${PREFIX}/scratch
WORKDIR_PKG=${WORKDIR}/PKG
WORKDIR_SRC=${WORKDIR}/SRC
WORKDIR_BLD=${WORKDIR}/BLD
WORKDIR_OUT=${WORKDIR}/OUT
CROSS_COMPILE=${TARGET}-
PATH=$PATH:${INSTALL}/bin
UCLIBC_CONFIG=`pwd`/config.uclibc

sudo mkdir -p ${PREFIX}
sudo chown -R `id -r -u -n`:`id -r -g -n` ${PREFIX}
mkdir -p ${WORKDIR_PKG}
mkdir -p ${WORKDIR_SRC}
mkdir -p ${WORKDIR_BLD}
mkdir -p ${WORKDIR_OUT}
