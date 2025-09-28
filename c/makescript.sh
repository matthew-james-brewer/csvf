#!/usr/bin/env bash

# Sorry y'all, I don't really want to bother with cmake or makefile or anything

# run `export SYSROOT=/usr/local` if you want to use the local development environment, or set it to another value for other use cases
SYSROOT="${SYSROOT:-/usr}"

# set to lib64 if you get an error
LIBDIR="${LIBDIR:-lib}"

# we all love clang, but if you don't have it, set it to gcc or something
CC="${CC:-clang}"

case "$(uname -s)" in
 Linux*) libext="so";;
 Darwin*) libext="dylib";;
 CYGWIN*|MINGW*|MSYS*) libext="dll";;
 # fallback to so
 *) libext="so";;
esac

if [ "$#" -ne 1 ]; then

 flags="-Wall -Wpedantic -Werror -O2"

 $CC dispcsvf.c -o dispcsvf.o -fPIC $flags -c
 $CC csv2csvf.c -o csv2csvf.o -fPIC $flags -c

 $CC -shared *.o -o libcsvf.$libext

 rm *.o

 mv libcsvf.$libext $SYSROOT/$LIBDIR

 cp libcsvf.h $SYSROOT/include

 $CC csv2csvf-cmd.c -o csv2csvf $flags -lcsvf
 $CC dispcsvf-cmd.c -o dispcsvf $flags -lcsvf

 mv csv2csvf dispcsvf $SYSROOT/bin

 cp csview $SYSROOT/bin

 chmod +x $SYSROOT/bin/csview

else

 if [ "$1" = "uninstall" ]; then

  rm -f $SYSROOT/$LIBDIR/libcsvf.$libext

  rm -f $SYSROOT/include/libcsvf.h

  rm -f $SYSROOT/bin/csv2csvf $SYSROOT/bin/dispcsvf

  rm -f $SYSROOT/bin/csview

 fi

fi
