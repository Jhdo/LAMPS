//#ifndef NK6UVME_H
//#define NK6UVME_H

#include "NKUSBVME_Linux.h"

// VME mode definition
#define A16D16  0x69
#define A16D32  0x29
#define A24D16  0x79
#define A24D32  0x39
#define A32D16  0x49
#define A32D32  0x09

#ifndef NKC
class NK6UVME {
 public:
           NK6UVME();
  virtual ~NK6UVME();
#endif
// open VME controller
int VMEopen(int devnum);
// close VME controller
int VMEclose(int devnum);
// VME write cycle
int VMEwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);
// VME read cycle
unsigned long VMEread(int devnum, unsigned short am, unsigned short tout, unsigned long address);
// VME block read cycle
int VMEblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);
// write cycle
int Vwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);
// read cycle
int Vread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long *data);
// block read cycle
int Vblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);

#ifdef NKROOT
  ClassDef(NK6UVME,2)
#endif
#ifndef NKC
     };
#endif
