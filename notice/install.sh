#!/bin/sh

# note : libusb is required for nkusb
# If you don't have installed it type : sudo apt-get install libusb-1.0-0-dev

cd $NKHOME
cd src/nkusb
make clean
make install
cd $NKHOME
cd src/nkusbroot
make clean
make install
cd $NKHOME
cd src/nk6uvme
make clean
make install
cd $NKHOME
cd src/nk6uvmeroot
make clean
make install
cd $NKHOME
cd src/nkv1290
make clean
make install
cd $NKHOME