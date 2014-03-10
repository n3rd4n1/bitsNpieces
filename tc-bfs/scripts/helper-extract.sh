#!/bin/bash

set -e

. ./dev.sh

. ./env.sh

URL=$1
SRC=$2
FILE=${URL##*/}
FILE=${FILE%%\?*}

pushd ${WORKDIR_PKG}

if ! [ -f ${FILE} ]
then
   curl -O ${URL}
   rm -rf ${SRC}
   mkdir -p ${SRC}

   if [ ${FILE##*.} == "zip" ]
   then
      unzip -qq -n ${FILE} -d ${SRC}
   else
      tar xf ${FILE} -C ${SRC}
   fi

   eval $3
fi

popd
