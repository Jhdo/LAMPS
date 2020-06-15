
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

#LDFLAGS +=-shared -Wall,-soname
SOFLAGS += -Wall
LDFLAGS = $(SOFLAGS)
LDLIBS = -L$(NOTICE_LIB) -lNK6UVMEROOT

NKV1290S = NKV1290.$(SrcSuf) NKV1290Dict.$(SrcSuf)
NKV1290H = NKV1290.$(HdrSuf) LinkDef.h
NKV1290O = $(NKV1290S:.cc=.o)
NKV1290SO = libNKV1290.$(DllSuf)

all: $(NKV1290SO)
clean:
	@rm -rf $(NKV1290SO) $(NKV1290O) *Dict.$(SrcSuf) *Dict.$(HdrSuf)

install: $(NKV1290SO) NKV1290.h
	#/bin/rm -f $(INSTALL_LIB)/$<
	#/bin/rm -f $(INSTALL_INC)/NKV1290.h
	install $< $(INSTALL_LIB)
	install *.pcm $(INSTALL_LIB)
	install NKV1290.h $(INSTALL_INC)

$(NKV1290SO): $(NKV1290O)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) $(OutPutOpt) $@

NKV1290Dict.$(SrcSuf): $(NKV1290H) LinkDef.h
	@echo "Generating dictionary $@..."
	#$(ROOTCINT) -f $@ -c -I$(NOTICE_INC) $^
	rootcling -f $@ -c -I$(NOTICE_INC) $^

#dependicies
$(NKV1290O):$(NKV1290H)
