#!/bin/bash

. ./env.sh

cd ${INSTALL}

rm lib/libiberty.a
rm bin/${CROSS_COMPILE}gccbug

FILES="`find bin/* ${TARGET}/bin/* libexec/gcc/${TARGET}/*/* -maxdepth 0`"

for file in ${FILES}
do
   if [ -f ${file} ]
   then
      echo ${STRIP} $file
      ${STRIP} ${file}
   fi
done

FILES="`find ${SYSROOT}/usr/lib/* lib/* lib/gcc/${TARGET}/*/* ${SYSROOT}/usr/bin/* -maxdepth 0`"
FLAGS="-R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_pubtypes -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc"

for file in ${FILES}
do
   if [ -f ${file} ]
   then
      echo ${CROSS_COMPILE}objcopy $file
      ${CROSS_COMPILE}objcopy ${FLAGS} ${file}
   fi
done

exit 0
