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

#define v792_ADDR_DATA 0x0000 //(D32)
#define v792_ADDR_STATUS1 0x100E // fitst bit : DATA_READY, 3rd bit : BUSY
#define v792_ADDR_CONTROL1 0x1010
#define v792_ADDR_BITSET1 0x1006
#define v792_ADDR_BITSET1_CLEAR 0x1008
#define v792_ADDR_BITSET2 0x1032 // 0x4 is bit for data clear
#define v792_ADDR_BITSET2_CLEAR 0x1034 // 0x4 is bit for data clear // bit 11 is for auto increment
#define v792_ADDR_EVENTCOUNTER_LOW 0x1024 // Lower bits (0-16) of event counter it can count absolute event number (set via bit set2)
#define v792_ADDR_EVENTCOUNTER_HIGH 0x1026
#define v792_ADDR_EVENTCOUNTER_RESET 0x1040
#define v792_ADDR_THRESHOLD  0x1080 // ch is set by 0x1080 + ?  (+0x4 per 1 channel : 792N, defaut 16x)
#define v792_ADDR_PED 0x1060 // 8bit

#define v792_THRESHOLD 0x01 // 8x16 = 128, Mutiplied by 16, See manual p20
#define v792_PED 0x00B4 // Must be bigger than 60
#define v792_READOUT_SIZE 200 // Read data buffer (doesn't matter with its contents) and check if it  contains EOB word if not, repeat readout
#define v792_NEVENT_BUFFER 34
#include "NK6UVMEROOT.h"
#include <bitset>

class ADCEvent{
 public:
  unsigned long EventNumber;
  unsigned long TriggerID;
  unsigned long adc[32];
  unsigned long adc_ch[32];
  unsigned long adc_err;
  unsigned long nadc; // Number of data words (Expect 2)

  void reset() {
    TriggerID = -999;
    EventNumber = -999;
    adc_err = -999;
    nadc = 0;
    for (int i = 0; i < 32; i++){
      adc[i] = -999;
      adc_ch[i] = -999;
    }
  }
};


class NKV792 : public NK6UVMEROOT
{
 public:
           NKV792();
  virtual ~NKV792();

  void ADCInit(int devnum, unsigned long mid);
  void ADC_SoftReset(int devnum, unsigned long mid);
  void ADCEventBuild(unsigned long *words, int nw, int i, ADCEvent *data); // Decoding words into event object (try to search i_th event)
  int ADCEventBuild_MEB(unsigned long *words, int nw, ADCEvent data_arr[]); // Decoding words into event object (try to search i_th event)
  void ADCClear_Buffer(int devnum, unsigned long mid);
  void ADCSet_ZeroSup(int devnum, unsigned long mid, int v);
//  void ADCSet_ZeroSup_bitset2(int devnum, unsigned long mid, int v);
  void ADCReset_TriggerCounter(int devnum, unsigned long mid);
  void ADCSet_Threshold(int devnum, unsigned long mid, int ch, unsigned char v);
  void ADCSet_Pedestal(int devnum, unsigned long mid, unsigned short pd);
  void ADCSet_AllowEmptyEvent(int devnum, unsigned long mid, int v); // Allow writing empty events
  unsigned long ADCRead_TriggerCounter(int devnum, unsigned long mid);
  unsigned long ADCRead_Threshold(int devnum, unsigned long mid, int ch);
  unsigned long ADCRead_Status1(int devnum, unsigned long mid); // Get Status bit1 (First bit is DATA_READY)
  unsigned long ADCRead_Buffer(int devnum, unsigned long mid, unsigned long *words);
  int ADC_IsBusy(int devnum, unsigned long mid);
  int ADC_IsDataReady(int devnum, unsigned long mid);
  int ADC_IsValidData(unsigned long word);

  const int fDebug = 1;

  ClassDef(NKV792, 1)

};


















