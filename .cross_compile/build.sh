#!/bin/bash

set -e

pac=csvf
ver=1.0.2
root=~/$pac
rpmbuild=~/rpmbuild
xwindir=~/xwin/.xwin-cache
src=$root/src/c
out=$root/build

rm -rf $out/*

cp -r $root/src $out/$pac-$ver
tar czvf $out/$pac.tar.gz --exclude .git -C $out $pac-$ver
cp $out/$pac.tar.gz $rpmbuild/SOURCES/$ver.tar.gz
rm -rf $out/$pac-$ver

for arch in aarch64-linux-gnu arm-linux-gnueabi arm-linux-gnueabihf i686-linux-gnu mips-linux-gnu mipsel-linux-gnu powerpc-linux-gnu powerpc64-linux-gnu powerpc64le-linux-gnu riscv64-linux-gnu s390x-linux-gnu x86_64-linux-gnu; do
 echo -e "\ncompiling for target $arch\n"

 mkdir $out/$arch

 echo -e "\nbuilding rpm package\n"

 sed "s/{{strip}}/$arch-strip/" $root/$pac.spec.base > $rpmbuild/SPECS/$pac.spec
 CC=$arch-gcc rpmbuild -ba $rpmbuild/SPECS/$pac.spec
 mv $rpmbuild/RPMS/*/* $out/$arch/$arch-$pac.rpm
 rm -rf $rpmbuild/RPMS/*

 # We have the compiled program files now
 rpm2cpio $out/$arch/*.rpm | cpio -idmD $out/$arch/compiled
 mv $out/$arch/compiled/usr/* $out/$arch/compiled
 rmdir $out/$arch/compiled/usr

 # compiled archive
 tar czvf $out/$arch/$arch-$pac.tar.gz -C $out/$arch compiled

 arch2=
 case $arch in
  x86_64-linux-gnu) arch2=amd64;;
  *) arch2=$(echo $arch | sed -E "s/-linux-gnu(eabi)?//")
 esac

 echo -e "\nbuilding deb package\n"

 mkdir $out/$arch/deb
 cp -r $out/$arch/compiled/* $out/$arch/deb
 mkdir $out/$arch/deb/DEBIAN
 sed "s/{{arch}}/$arch2/" $root/control.base > $out/$arch/deb/DEBIAN/control
 dpkg-deb -v --build $out/$arch/deb $out/$arch/$arch-$pac.deb
 rm -rf $out/$arch/deb

 rm -rf $out/$arch/compiled
done

echo -e "\nwindows build\n"

arch=x86_64-windows
mkdir $out/$arch
mkdir $out/$arch/compiled
export LIB="$xwindir/splat/crt/lib/x86_64;$xwindir/splat/sdk/lib/um/x86_64;$xwindir/splat/sdk/lib/ucrt/x86_64;$xwindir/unpack/Microsoft.VC.14.44.17.14.CRT.x64.Store.base.vsix/lib/x64;$xwindir/unpack/ucrt.msi/lib/ucrt/x64"
cmake -S $src -B $out/win64 -DCMAKE_INSTALL_PREFIX=$out/$arch/compiled -DCMAKE_TOOLCHAIN_FILE=$xwindir/../win-toolchain.cmake
cmake --build $out/win64
cmake --install $out/win64
rm -rf $out/win64
export LIB=""
tar czvf $out/$arch/$arch-$pac.tar.gz -C $out/$arch compiled
rm -rf $out/$arch/compiled

rm -rf $rpmbuild/*/*
