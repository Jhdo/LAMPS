# 1 "NKV1290.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "NKV1290.h"
# 18 "NKV1290.h"
# 1 "Notice6UVME.h" 1



# 1 "NKUSBVME_Linux.h" 1
# 23 "NKUSBVME_Linux.h"
int uopen(int devnum);
int uclose(int devnum);
int uwrite(int devnum, char *buffer, unsigned long length);
int uread(int devnum, char *buffer, unsigned long length);
# 5 "Notice6UVME.h" 2
# 15 "Notice6UVME.h"
class NK6UVME {
 public:
           NK6UVME();
  virtual ~NK6UVME();


int VMEopen(int devnum);

int VMEclose(int devnum);

int VMEwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);

unsigned long VMEread(int devnum, unsigned short am, unsigned short tout, unsigned long address);

int VMEblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);

int Vwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);

int Vread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long *data);

int Vblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);


  ClassDef(NK6UVME,2)


     };
# 19 "NKV1290.h" 2


class TDCEvent
{
 public:
  unsigned long TriggerID;
  unsigned long EventNumber;
  unsigned long tdc[2];
  unsigned long tdc_ch[2];
  unsigned long tdc_err;
  unsigned long nword;
};


class NKV1290 : public NK6UVME
{
 public:
           NKV1290();
  virtual ~NKV1290();

  void TDCInit(int devnum, unsigned long mid);
  void TDCEventBuild(unsigned long *words, int nw, int i, TDCEvent *data);
  void TDCClear_Buffer(int devnum, unsigned long mid);
  unsigned long TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words);
  unsigned long TDCRead_NW(int devnum, unsigned long mid);
  unsigned long TDCRead_NEVT(int devnum, unsigned long mid);
  unsigned long TDCRead_Status(int devnum, unsigned long mid);
  int TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words);

  int fDebug;



  ClassDef(NKV1290, 1)
};
