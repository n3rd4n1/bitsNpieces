#!/bin/bash

set -e

. ./env.sh

NAME=elf2flt
SRC=${WORKDIR_SRC}/${NAME}
BLD=${WORKDIR_BLD}/${NAME}

./helper-extract.sh ${RES_ELF2FLT} ${SRC}

rm -rf ${BLD}
mkdir -p ${BLD}
cd ${BLD}
${SRC}/*/configure --build=${BUILD} --host=${HOST} --target=${TARGET} --prefix=${INSTALL} --with-binutils-include-dir=`readlink -m ${WORKDIR_SRC}/binutils/*/include/` --with-bfd-include-dir=${WORKDIR_BLD}/binutils/bfd/ --with-libbfd=${WORKDIR_BLD}/binutils/bfd/libbfd.a --with-libiberty=${WORKDIR_BLD}/binutils/libiberty/libiberty.a --with-zlib-prefix=${WORKDIR_OUT}
make ${MAKE_FLAGS}
make ${MAKE_FLAGS} install

