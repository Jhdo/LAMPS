
#gcc -DHAVE_CONFIG_H -I. -I.. -I../libusb   -std=gnu99 -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -O2 -MT dpfp.o -MD -MP -MF .deps/dpfp.Tpo -c -o dpfp.o dpfp.c
#mv -f .deps/dpfp.Tpo .deps/dpfp.Po
#/bin/sh ../libtool  --tag=CC   --mode=link gcc -std=gnu99 -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -O2   -o dpfp dpfp.o ../libusb/libusb-1.0.la -lrt -pthread
#libtool: link: gcc -std=gnu99 -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -O2 -o .libs/dpfp dpfp.o -pthread  ../libusb/.libs/libusb-1.0.so -lrt -pthread -Wl,-rpath -Wl,/usr/local/lib

NOTICE_LIB = $(NKHOME)/lib
NOTICE_INC = $(NKHOME)/include

INSTALL_LIB = $(NKHOME)/lib
INSTALL_INC = $(NKHOME)/include

RC     := root-config
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
endif
ifneq ($(MKARCH),)
include $(MKARCH)
else
ROOTSYS = ..
include $(ROOTSYS)/etc/Makefile.arch
endif
-include ../MyConfig.mk

#------------------------------------------------------------------------------

SrcSuf = cc
HdrSuf = h
ObjSuf = o
DllSuf = so
OutPutOpt = -o #keep the space after the option "-o"

CXXFLAGS += -I. -I$(LIBUSB_INC) -I$(NOTICE_INC)
ifneq ($(DEBUG),)
CXXFLAGS += -g
endif

#LDFLAGS += -L$(LIB_nkusb) -lnkusb -shared -Wall,-soname
SOFLAGS += -Wall
LDFLAGS = $(SOFLAGS)
LDLIBS = -L$(NOTICE_LIB) -lNKUSBROOT -lnk6uvme

NK6UVMEROOTS = NK6UVMEROOT.$(SrcSuf) NK6UVMEROOTDict.$(SrcSuf)
NK6UVMEROOTH = NK6UVMEROOT.$(HdrSuf) LinkDef.h
NK6UVMEROOTO = $(NK6UVMEROOTS:.cc=.o)
NK6UVMEROOTSO = libNK6UVMEROOT.$(DllSuf)

all: $(NK6UVMEROOTSO)
clean:
	@rm -rf $(NK6UVMEROOTSO) $(NK6UVMEROOTO) *Dict.$(SrcSuf) *Dict.$(HdrSuf)

install: $(NK6UVMEROOTSO) NK6UVMEROOT.h
	#/bin/rm -f $(INSTALL_LIB)/$<
	#/bin/rm -f $(INSTALL_INC)/NK6UVMEROOT.h
	install $< $(INSTALL_LIB)
	install *.pcm $(INSTALL_LIB)
	install NK6UVMEROOT.h $(INSTALL_INC)

$(NK6UVMEROOTSO): $(NK6UVMEROOTO)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) $(OutPutOpt) $@

NK6UVMEROOTDict.$(SrcSuf): $(NK6UVMEROOTH) LinkDef.h
	@echo "Generating dictionary $@..."
	#$(ROOTCINT) -f $@ -c -I$(NOTICE_INC) $^
	rootcling -f $@ -I$(NOTICE_INC) $^

#dependicies
$(NK6UVMEROOTO):$(NK6UVMEROOTH)
