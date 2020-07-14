
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

NKV792S = NKV792.$(SrcSuf) NKV792Dict.$(SrcSuf)
NKV792H = NKV792.$(HdrSuf) LinkDef.h
NKV792O = $(NKV792S:.cc=.o)
NKV792SO = libNKV792.$(DllSuf)

all: $(NKV792SO)
clean:
	@rm -rf $(NKV792SO) $(NKV792O) *Dict.$(SrcSuf) *Dict.$(HdrSuf)

install: $(NKV792SO) NKV792.h
	#/bin/rm -f $(INSTALL_LIB)/$<
	#/bin/rm -f $(INSTALL_INC)/NKV792.h
	install $< $(INSTALL_LIB)
	install *.pcm $(INSTALL_LIB)
	install NKV792.h $(INSTALL_INC)

$(NKV792SO): $(NKV792O)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) $(OutPutOpt) $@

NKV792Dict.$(SrcSuf): $(NKV792H) LinkDef.h
	@echo "Generating dictionary $@..."
	#$(ROOTCINT) -f $@ -c -I$(NOTICE_INC) $^
	rootcling -f $@ -c -I$(NOTICE_INC) $^

#dependicies
$(NKV792O):$(NKV792H)
