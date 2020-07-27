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
  // Zero suppression threshold
  ADCSet_Threshold(devnum, mid, v792_THRESHOLD);
  // Pedestal is recommended to set larger than 60, see manual p16
  ADCSet_Pedestal(devnum, mid, v792_PED);
  
  ADCClear_Buffer(devnum, mid);
  
  return;
}


void NKV792::ADCSet_ZeroSup(int devnum, unsigned long mid, int v)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  //bitset2, number 4 bit
  unsigned long addr = baseaddr + v792_ADDR_STATUS;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);
 
  return word;
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
  
  baseaddr = (mid & 0xFFFF) << 16; //A32 mode
  
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
    if (IsEOB(words[i/4])) {
      cout << "v792 Met EOB word" << endl;
      nw_data++;
      return nw_data;
    }

    else nw_data;
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

  if (fDebug) cout << "NW : " << nw_data << endl;

  return nw_data;
}


void NKV792::ADCEventBuild(unsigned long *words, int nw, int i, ADCEvent *data)
{
  int nhit = 0;
  unsigned long nevt = 0;
  for (i = 0; i < nw; i++) {
    int type = -1; // type 0(data) 1(ADC header) 2(ADC trailer) 3(global header) 4(ADC error) 5(global trailer)
    unsigned long type_code = (words[i] >> 27) & 0x1F;
//    unsigned long type_code = words[i] & 0xA98A58;
    if (type_code == 0x00) type = 0;
    else if (type_code == 0x01) type = 1;
    else if (type_code == 0x03) type = 2;
    else if (type_code == 0x08) type = 3;
    else if (type_code == 0x04) type = 4;
    else if (type_code == 0x10) type = 5;
    if (fDebug) cout << "Word Type : " << type << endl;

    if (type_code == 1) {
      unsigned long BunchID = words[i] & 0xFFF;
      unsigned long EventID = words[i] & 0xFFF000;
      EventID = EventID >> 12;
      if (fDebug) cout << "BunchID : " << BunchID << " EventID : " << EventID << endl;
      data->EventNumber = BunchID;
    }

    if (type_code == 0) {
      unsigned long ADC_raw = words[i] & 0x1FFFFF;
      unsigned long ADC_ch = (words[i] >> 21) & 0x1F;
      if (fDebug) cout << "ADC Ch " << ADC_ch << " ADC : " << ADC_raw << endl;
      if (nhit > 1) {
        cout << "Number of ADC hits are too many" << endl;
        return;
      }

      data->ADC[nhit] = ADC_raw;
      data->ADC_ch[nhit] = ADC_raw;
      nhit++;
    }

  }
}


unsigned long NKV792::ADCRead_Status(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v792_ADDR_STATUS;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);
 
  return word;
}


void NKV792::ADCClear_Buffer(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v792_ADDR_SW_CLEAR;
  
  unsigned long data = 0x1;

  VMEwrite(devnum, A32D16, 100, addr, data);
}