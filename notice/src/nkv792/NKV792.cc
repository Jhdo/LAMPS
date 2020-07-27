#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <TROOT.h>

#include "NKV792.h"

ClassImp(NKV792)

NKV792::NKV792()
{
   printf("NKV792 Initialization \n");
}

NKV792::~NKV792()
{
   printf("Leaving NKV792 now \n");
}
#endif


void NKV792::ADCInit(int devnum, unsigned long mid)
{
  cout << "Initializing v792.." << endl;
  // Enable Zero suppresion
  ADCSet_ZeroSup(devnum, mid, 1);
  // Zero suppression threshold : Need to make ch by ch value
  for (int ch = 0; ch < 16; ch++) {
    ADCSet_Threshold(devnum, mid, ch, v792_THRESHOLD);
  }
  // Pedestal is recommended to set larger than 60, see manual p16
  ADCSet_Pedestal(devnum, mid, v792_PED);
  
  ADCClear_Buffer(devnum, mid);
  
  ADCReset_TriggerCounter(devnum, mid);
  
  return;
}


void NKV792::ADCReset_TriggerCounter(int devnum, unsigned long mid);
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_EVENTCOUNTER_RESET;

  VMEwrite(devnum, A32D16, 100, addr, 0x0);

  return;
}


unsigned long NKV792::ADCRead_TriggerCounter(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr_low = baseaddr + v792_ADDR_EVENTCOUNTER_LOW;

  unsigned long addr_high = baseaddr + v792_ADDR_EVENTCOUNTER_HIGH;

  unsigned short v_low = VMEread(devnum, A32D16, 100, addr_low);
  
  unsigned short v_high = VMEread(devnum, A32D16, 100, addr_high);

  v_high = v_high & 0xF; // 8 bits are vailid

  unsigned long v = (v_high << 16) + v_low;

  return v;
}


void NKV792::ADCSet_ZeroSup(int devnum, unsigned long mid, int v)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_BITSET2;

  unsigned short bitset2_v = VMEread(devnum, A32D16, 100, addr);

  if (v == 0) {
    bitset2_v = bitset2_v | (0x0001 << 4); // Disable zero_sup
    cout << "ZeroSuppresion Disabled" << endl;
  }

  else if (v == 1) {
    bitset2_v = ~(bitset2_v) & ~(0x0001 << 4); // Enable zero_sup
    cout << "ZeroSuppresion Enabled" << endl;
  }

  VMEwrite(devnum, A32D16, 100, addr, bitset2_v);

  return;
}


void NKV792::ADCSet_Threshold(int devnum, unsigned long mid, int ch, unsigned short v)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_THRESHOLD + 4*ch; // v792N (+4*ch), v792 (+2*ch)

  v = v & 0xFF; // only 8 bits are valid

  VMEwrite(devnum, A32D16, 100, addr, v);

  return;
}


unsigned long NKV792::ADCRead_Threshold(int devnum, unsigned long mid, int ch)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_THRESHOLD + 4*ch; // v792N (+4*ch), v792 (+2*ch)

  unsigned long v = VMEread(devnum, A32D16, 100, addr);

  return v;
}


void NKV792::ADCSet_Pedestal(int devnum, unsigned long mid, unsigned short pd);
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_PED;

  pd = pd & 0xFF; // only 8 bits are valid

  VMEwrite(devnum, A32D16, 100, addr, pd);

  return;
}


unsigned long NKV792::ADCRead_Status1(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;

  unsigned long addr = baseaddr + v792_ADDR_STATUS1;

  unsigned long v = VMEread(devnum, A32D16, 100, addr);

  v = v & 0xF;

  return v;
}


int NKV792::ADC_IsBusy(int devnum, unsigned long mid)
{
  int ret = 0;

  unsigned long st1 = ADCRead_Status1(devnum, mid);

  if (st1 & 0x4) ret = 1;

  return ret; 
}


int NKV792::ADC_IsDataReady(int devnum, unsigned long mid)
{
  int ret = 0;

  unsigned long st1 = ADCRead_Status1(devnum, mid);

  if (st1 & 0x1) ret = 1;

  return ret; 
}


int NKV792::ADC_IsValidData(unsigned long word)
{
  int ret = 1;

  word = (word >> 24) & 0x7;

  if (word == 0x6) ret = 0; 

  return ret;
}


unsigned long NKV792::ADCRead_Buffer(int devnum, unsigned long mid, unsigned long *words)
{
  if (fDebug) cout << "Reading ADC Data" << endl;
  unsigned long baseaddr;
  unsigned long i;
  unsigned long addr;
  unsigned long nw_read = v792_READOUT_SIZE;
  unsigned long nw_data = 0;
  unsigned char rdat[10000];
  unsigned long rdat_32bit[10000];
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  addr = baseaddr + v792_ADDR_DATA;

  VMEblockread(devnum, A32D32, 100, addr, 4*nw_read, rdat);
  
  // Decoding Words : 32bit
  for (i = 0; i < 4*nw_read - 3; i=i+4) {
    // If v1290 encoded as big endian
  //  rdat_32bit[i] = (rdat[i] & 0xFF) << 24;
  //  rdat_32bit[i+1] = (rdat[i+1] & 0xFF) << 16;
  //  rdat_32bit[i+2] = (rdat[i+2] & 0xFF) << 8;
  //  rdat_32bit[i+3] = (rdat[i+3] & 0xFF);

    // If v1290 encoded as little endian
    rdat_32bit[i+3] = (rdat[i+3] & 0xFF) << 24;
    rdat_32bit[i+2] = (rdat[i+2] & 0xFF) << 16;
    rdat_32bit[i+1] = (rdat[i+1] & 0xFF) << 8;
    rdat_32bit[i] = (rdat[i] & 0xFF);
    
    words[i/4] = rdat_32bit[i] + rdat_32bit[i+1] + rdat_32bit[i+2] + rdat_32bit[i+3];

    if (!ADC_IsValidData(words[i/4])) {
      if (fDebug) cout << "v792 Met End of data block" << endl;
      if (fDebug) cout << "NW : " << nw_data << endl;
      return nw_data;
    }

    nw_data++;
//    cout << "rdat " << endl;
//    cout << bitset<32>(rdat[i+3]) << endl;
//    cout << bitset<32>(rdat[i+2]) << endl;
//    cout << bitset<32>(rdat[i+1]) << endl;
//    cout << bitset<32>(rdat[i]) << endl;
//    cout << "rdat_32" << endl;
//    cout << bitset<32>(rdat_32bit[i+3]) << endl;
//    cout << bitset<32>(rdat_32bit[i+2]) << endl;
//    cout << bitset<32>(rdat_32bit[i+1]) << endl;
//   cout << bitset<32>(rdat_32bit[i]) << endl;
//    cout << "Data word " << bitset<32>(words[i]) << endl;
  }


  return nw_data;
}


void NKV792::ADCEventBuild(unsigned long *words, int nw, int i, ADCEvent *data)
{
  int nhit = 0;
  unsigned long nevt = 0;
  string type_name[4] = {"ADC Data", "ADC Header", "ADC Trailer", "ADC Invalid Data"};

  for (i = 0; i < nw; i++) {
    int type = -1; // type 0(data) 1(ADC header) 2(ADC trailer) 3(global header) 4(ADC error) 5(global trailer)
    unsigned long type_code = (words[i] >> 24) & 0x7;
    if (type_code == 0x00) type = 0;
    else if (type_code == 0x02) type = 1;
    else if (type_code == 0x04) type = 2;
    else if (type_code == 0x06) type = 3;
    if (fDebug) cout << "Word Type : " << type_name[type].c_str() << endl;

    /////////////////////////////////////////////////@@@@@@@@@@@@@
    if (type_code == 1) {
      unsigned long nch = (words[i] >> 8) & 0x003F;
      if (fDebug) cout << "ADC NCH  : " << nch << endl;
    }

    if (type_code == 2) {
      unsigned long EventCounter = words[i] & 0xFFFFFF;
      if (fDebug) cout << "ADC EventCounter  : " << EventCounter << endl;
      nevt++;
    }

    if (type_code == 3) {
      cout << "Warning : ADC Invalid Data is in word" << endl;
      return;
    }

    if (type_code == 0) {
      unsigned long adc_raw = words[i] & 0xFFF;
      unsigned long adc_ch = (words[i] >> 17) & 0xF;
      if (fDebug) cout << "ADC Ch " << adc_ch << " ADC : " << adc_raw << endl;
      if (nhit >= 500) {
        cout << "Number of tdc hits are too many" << endl;
        return;
      }

      data->adc[nhit] = adc_raw;
      data->adc_ch[nhit] = adc_ch;
      nhit++;
      data->nadc = nhit;
    }
  }

  if (fDebug)  cout << "NEvent : " << nevt << endl;
  
  return;
}


unsigned long NKV792::ADCRead_Status(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v792_ADDR_STATUS;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);
 
  return word;
}


void NKV792::ADCClear_Buffer(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v792_ADDR_BITSET2;
  
  unsigned long addr_clear = baseaddr + v792_ADDR_BITSET2_CLEAR;

  unsigned short bitset2_v = VMEread(devnum, A32D16, 100, addr);

  bitset2_v = bitset2_v | (0x0001 << 2);

  VMEwrite(devnum, A32D16, 100, addr, bitset2_v);

  unsigned short v = 0x4;

  VMEwrite(devnum, A32D16, 100, addr_clear, v);
}