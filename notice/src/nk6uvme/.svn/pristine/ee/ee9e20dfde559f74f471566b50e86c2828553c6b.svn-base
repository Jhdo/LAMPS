NOTICE_LIB = $(NKHOME)/lib
NOTICE_INC = $(NKHOME)/include

INSTALL_LIB = $(NKHOME)/lib
INSTALL_INC = $(NKHOME)/include

SrcSuf = c
HdrSuf = h
ObjSuf = o
DllSuf = so
OutPutOpt = -o #
CFLAGS = -I. -I$(LIBUSB_INC) -I$(NOTICE_INC) -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -O2 -fPIC -m64

#SOFLAGS = -shared -Wall,-soname -nostartfiles -s -m64
SOFLAGS = -shared -Wall
LDFLAGS = $(SOFLAGS)
LDLIBS = -L$(NOTICE_LIB) -lnkusb
CC = gcc
LD = gcc

NK6UVMES = nk6uvme.$(SrcSuf)
NK6UVMEH = nk6uvme.$(HdrSuf)
NK6UVMEO = nk6uvme.$(ObjSuf)
NK6UVMESO = libnk6uvme.$(DllSuf)

PROGRAMS = $(NK6UVMESO)

all: $(PROGRAMS)
clean:
	@rm -rf $(NK6UVMEO) $(PROGRAMS)

$(NK6UVMESO): $(NK6UVMEO)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) $(OutPutOpt) $@ $(EXPLLINKLIBS)

install: $(NK6UVMESO) $(NK6UVMEH)
	#/bin/rm -f $(INSTALL_LIB)/$<
	#/bin/rm -f $(INSTALL_INC)/$(NK6UVMEH)
	install $< $(INSTALL_LIB)
	install $(NK6UVMEH) $(INSTALL_INC)

#dependicies
$(NK6UVMEO):$(NK6UVMEH)
