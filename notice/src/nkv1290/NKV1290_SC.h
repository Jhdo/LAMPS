// Derived Class from NK6UVME

// Reg : Status 1002, Control 1000, Data 0000(D32), SoftWare Clear 1016, Event FIFO 1038(D32), Event Counter 101C(D32),
// Software Reset 1016(Including Event counter reset)
// Registers
#define v1290_ADDR_FIFO  0x1038 //(D32)
#define v1290_ADDR_EVTCOUNTER 0x101C //(D32)
#define v1290_ADDR_DATA 0x0000 //(D32)
#define v1290_ADDR_STATUS 0x1002
#define v1290_ADDR_SW_CLEAR 0x1016
#define v1290_ADDR_MICRO 0x102E
#define v1290_ADDR_MICRO_HS 0x1030

// Parameters
#define v1290_TM_WIDTH 0x0014 // 20
#define v1290_TM_OFFSET 0xFFD8 // -40 (signed short)

//#include "Notice6UVME.h" // nk6uvme packed in class
#include "nk6uvme.h"

class TDCEvent
{
 public:
  unsigned long TriggerID;
  unsigned long EventNumber;
  unsigned long tdc[2];
  unsigned long tdc_ch[2];
  unsigned long tdc_err;
  unsigned long nword; // Number of data words (Expect 2)
};


class NKV1290// : public NK6UVME
{
 public:
           NKV1290();
  virtual ~NKV1290();

  void TDCInit(int devnum, unsigned long mid);
  void TDCEventBuild(unsigned long *words, int nw, int i, TDCEvent *data); // Decoding words into event object (try to search i_th event)
  void TDCClear_Buffer(int devnum, unsigned long mid);
  unsigned long TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words);
  unsigned long TDCRead_NW(int devnum, unsigned long mid); // Get Number of Words in Buffer
  unsigned long TDCRead_NEVT(int devnum, unsigned long mid); // Get Number of Events in Buffer
  unsigned long TDCRead_Status(int devnum, unsigned long mid); // Get Status bit (First bit is DATA_READY)
  int TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words);
  int VMEOpen(int sid, libusb_context *ctx) {return VMEopen(sid, ctx);}
  int VMEClose(int sid) {VMEclose(sid);}

  int fDebug;
  // const unsigned short v1290_TM_WIDTH = 20; //20(default) // Trigger matching window width (each step is 25ns)
  // const signed short v1290_TM_OFFSET = -40; //-40(default) // Trigger matching window offset

  ClassDef(NKV1290, 1)
};


















