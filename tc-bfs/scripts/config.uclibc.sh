#!/bin/bash

set -e

. ./env.sh

NAME=uclibc
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}-stage1

./helper-extract.sh ${RES_UCLIBC} ${SRC}

cd ${SRC}/*
cp ${UCLIBC_CONFIG} .config
make menuconfig
cp .config ${UCLIBC_CONFIG}
