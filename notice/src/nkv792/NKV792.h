#define v792_ADDR_DATA 0x0000 //(D32)
#define v792_ADDR_STATUS1 0x100E // fitst bit : DATA_READY, 3rd bit : BUSY
#define v792_ADDR_CONTROL1 0x1010
#define v792_ADDR_BITSET1 0x1006
#define v792_ADDR_BITSET2 0x1032 // 0x4 is bit for data clear
#define v792_ADDR_BITSET2_CLEAR 0x1034 // 0x4 is bit for data clear // bit 11 is for auto increment
#define v792_ADDR_EVENTCOUNTER_LOW 0x1024 // Lower bits (0-16) of event counter it can count absolute event number (set via bit set2)
#define v792_ADDR_EVENTCOUNTER_HIGH 0x1026
#define v792_ADDR_EVENTCOUNTER_RESET 0x1040
#define v792_ADDR_THRESHOLD  0x1080 // ch is set by 0x1080 + ?  (+0x4 per 1 channel : 792N, defaut 16x)
#define v792_ADDR_PED 0x1060 // 8bit

#define v792_THRESHOLD 0x0002 // 32, Mutiplied by 16, See manual p20
#define v792_PED 0x0040 // Must be bigger than 60
#define v792_READOUT_SIZE 128 // Read data buffer (doesn't matter with its contents) and check if it  contains EOB word if not, repeat readout

#include "NK6UVMEROOT.h"

class ADCEvent{
 public:
  unsigned long TriggerID;
  unsigned long adc[500];
  unsigned long adc_ch[500];
  unsigned long adc_err;
  unsigned long nadc; // Number of data words (Expect 2)
};


class NKV792 : public NK6UVMEROOT
{
 public:
           NKV792();
  virtual ~NKV792();

  void ADCInit(int devnum, unsigned long mid);
  void ADCEventBuild(unsigned long *words, int nw, int i, ADCEvent *data); // Decoding words into event object (try to search i_th event)
  void ADCClear_Buffer(int devnum, unsigned long mid);
  void ADCSet_ZeroSup(int devnum, unsigned long mid, int v);
  void ADCSet_Threshold(int devnum, unsigned long mid, unsigned short t);
  void ADCSet_Pedestal(int devnum, unsigned long mid, unsigned char pd);
  unsigned long ADCRead_Buffer(int devnum, unsigned long mid, unsigned long *words);
  unsigned long ADCRead_Status(int devnum, unsigned long mid); // Get Status bit (First bit is DATA_READY)

  unsigned long ADCRead_NW(int devnum, unsigned long mid); // Get Number of Words in Buffer
  unsigned long ADCRead_NEVT(int devnum, unsigned long mid); // Get Number of Events in Buffer
  unsigned long ADC_IsBusy(int devnum, unsigned long mid);

  int IsEOB(unsigned long word);

  const int fDebug = 1;

  ClassDef(NKV792, 1)

};


















