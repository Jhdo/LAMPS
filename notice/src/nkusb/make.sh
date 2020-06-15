gcc -I. -I/usr/include/libusb-1.0 -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -O2 -fPIC -m64   -c -o nkusb.o nkusb.c
nkusb.c:69: warning: ‘remove_device_handle’ defined but not used
gcc -shared -Wall nkusb.o -L/usr/lib64 -lusb-1.0 -o  libnkusb.so 

