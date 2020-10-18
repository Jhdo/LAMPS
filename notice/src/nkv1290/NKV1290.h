// Derived Class from NK6UVME

// Reg : Status 1002, Control 1000, Data 0000(D32), SoftWare Clear 1016, Event FIFO 1038(D32), Event Counter 101C(D32),
// Software Reset 1016(Including Event counter reset)

#define VME_VENDOR_ID (0x04b4)
#define VME_PRODUCT_ID (0x00f0)

#define VME6U_SF_READ (0x82)
#define VME6U_SF_WRITE (0x06)

// VME mode definition
#define A16D16  (0x69)
#define A16D32  (0x29)
#define A24D16  (0x79)
#define A24D32  (0x39)
#define A32D16  (0x49)
#define A32D32  (0x09)

// Registers
#define v1290_ADDR_FIFO  0x1038 //(D32)
#define v1290_ADDR_FIFO_STORED 0x103C
#define v1290_ADDR_SOFT_EVENTCOUNT_RESET 0x1018
#define v1290_ADDR_EVTCOUNTER 0x101C //(D32)
#define v1290_ADDR_DATA 0x0000 //(D32)
#define v1290_ADDR_STATUS 0x1002
#define v1290_ADDR_SW_CLEAR 0x1016
#define v1290_ADDR_MICRO 0x102E
#define v1290_ADDR_MICRO_HS 0x1030
#define v1290_ADDR_CONTROL 0x1000

// Parameters
#define v1290_TM_WIDTH 0x0008 // 8 (0x0014 default) 25ns per 1
#define v1290_TM_OFFSET 0xFFF7 // -8 (signed short) 25ns per 1
#define v1290_READOUT_SIZE 200 // Read data buffer (doesn't matter with its contents) and check if it  contains EOB word if not, repeat readout

//#include "Notice6UVME.h"
#include "NK6UVMEROOT.h"

class TDCEvent
{
 public:
  unsigned long TriggerID;
  unsigned long EventNumber;
  unsigned long tdc[32];
  unsigned long tdc_ch[32];
  unsigned long tdc_err;
  unsigned long ntdc; // Number of data words (Expect 2)

  void reset() {
    TriggerID = -999;
    EventNumber = -999;
    tdc_err = -999;
    ntdc = 0;
    for (int i = 0; i < 32; i++){
      tdc[i] = -999;
      tdc_ch[i] = -999;
    }
  }
};


class NKV1290 : public NK6UVMEROOT
{
 public:
           NKV1290();
  virtual ~NKV1290();

  void TDCInit(int devnum, unsigned long mid, int ReadOutMode = 1);
  void TDCEventBuild(unsigned long *words, int nw, int iw, TDCEvent *data); // Decoding words into event object (try to search i_th event)
  int TDCEventBuild_MEB(unsigned long *words, int nw, TDCEvent data_arr[]); // Decoding words into event object (try to search i_th event)
  void TDCClear_Buffer(int devnum, unsigned long mid);
  void TDCSet_TM_Width(int devnum, unsigned long mid, unsigned short wd);
  void TDCSet_TM_Offset(int devnum, unsigned long mid, short os);
  void TDCWrite_Control(int devnum, unsigned long mid, unsigned long word); // Write Control Reg
  void TDCSet_Subtract_TriggerTime(int devnum, unsigned long mid, int v); // Write Control Reg
  unsigned long TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words);
  unsigned long TDCRead_Buffer_Test(int devnum, unsigned long mid, unsigned long *words);
  unsigned long TDCRead_NW(int devnum, unsigned long mid); // Get Number of Words in Buffer
  unsigned long TDCRead_EVTID(int devnum, unsigned long mid); // Get Number of Events in Buffer
  unsigned long TDCRead_Status(int devnum, unsigned long mid); // Get Status bit (First bit is DATA_READY)
  unsigned long TDCRead_Control(int devnum, unsigned long mid); // Read Control Reg
  unsigned long TDCRead_FIFO_Stored(int devnum, unsigned long mid); // Get Number of Events in FIFO
  unsigned long TDCRead_EventCounter(int devnum, unsigned long mid);
  
  int TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words);

  const int fDebug = 1; 
  int fReadOutMode;
  // const unsigned short v1290_TM_WIDTH = 20; //20(default) // Trigger matching window width (each step is 25ns)
  // const signed short v1290_TM_OFFSET = -40; //-40(default) // Trigger matching window offset

  ClassDef(NKV1290, 1)
};


















