#!/usr/bin/env bash

# Sorry y'all, I don't really want to bother with cmake or makefile or anything

# run `export SYSROOT=/usr/local` if you want to use the local development environment, or set it to another value for other use cases
SYSROOT="${SYSROOT:-/usr}"

# set to lib64 if you get an error
LIBDIR="${LIBDIR:-lib}"

if [ "$#" -ne 1 ]; then

 flags="-Wall -Wpedantic -Werror -O2"

 clang dispcsvf.c -o dispcsvf.o -fPIC $flags -c
 clang csv2csvf.c -o csv2csvf.o -fPIC $flags -c

 clang -shared *.o -o libcsvf.so

 rm *.o

 mv libcsvf.so $SYSROOT/$LIBDIR

 cp libcsvf.h $SYSROOT/include

 clang csv2csvf-cmd.c -o csv2csvf $flags -lcsvf
 clang dispcsvf-cmd.c -o dispcsvf $flags -lcsvf

 mv csv2csvf dispcsvf $SYSROOT/bin

 cp csview $SYSROOT/bin

 chmod +x $SYSROOT/bin/csview

else

 if [ "$1" = "uninstall" ]; then

  rm -f $SYSROOT/$LIBDIR/libcsvf.so

  rm -f $SYSROOT/include/libcsvf.h

  rm -f $SYSROOT/bin/csv2csvf $SYSROOT/bin/dispcsvf

  rm -f $SYSROOT/bin/csview

 fi

fi
