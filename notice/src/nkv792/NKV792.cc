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


#ifdef NKC
void TDCInit(int devnum, unsigned long mid)
#else
void NKV792::TDCInit(int devnum, unsigned long mid)
#endif
{
  cout << "Initializing v1290.." << endl;
  unsigned short opcode[10];

  opcode[0]=0x0000;
  TDCWrite_Opcode(devnum, mid, 1, opcode)

  opcode[0]=0x1000; opcd[1] = v1290_TM_WIDTH;
  TDCWrite_Opcode(devnum, mid, 2, opcode)

  opcode[0]=0x1100; opcd[1]= v1290_TM_OFFSET;
  TDCWrite_Opcode(devnum, mid, 2, opcode)

// opcd[0]=0x3100; // Disable TDC Header/Trailer
// TDCWrite_Opcode(devnum, mid, 1, opcode)
  cout << "Done" << endl;

  return;
}


#ifdef NKC
unsigned long TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words)
#else
unsigned long NKV792::TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words)
#endif
{
  if (fDebug) cout << "Reading TDC Data" << endl;
  unsigned long baseaddr;
  unsigned long i;
  unsigned long addr;
  unsigned long nw = 0; // Number of words
  char rdat[0x100000];
  
  baseaddr = (mid & 0xFF) << 24; //A32 mode
  
  addr = baseaddr + v1290_ADDR_DATA;

  nw = TDCRead_NW(devnum, mid);
  nevt = TDCRead_NEVT(devnum, mid);

  if (nw <= 0) {
    cout << "Empty Buffer" << endl;
    return -1;
  }

  if (fDebug) cout << "NW : " << nw << " NEVT " << nevt << endl;

  if (nevt > 1) {
    cout << "Warning Multiple Events in buffer" << endl;
    return -1;
  }

  VMEblockread(devnum, A32D32, 100, addr, 4*nw, rdat);
  
  // Decoding Words : 32bit
  for (i = 0; i < 4*nw - 3; i=i+4) {
    rdat[i] = rdat[i] << 24;
    rdat[i+1] = rdat[i+1] << 16;
    rdat[i+2] = rdat[i+2] << 8;

    unsigned long words[i] = rdat[i] + rdat[i+1] + rdat[i+2] + rdat[i+3];
  }

  return nw;
}


// Decoding Words : 32bit
#ifdef NKC
void TDCEventBuild(unsigned long *words, int nw, int i, TDCEvent *data)
#else
void NKV792::TDCEventBuild(unsigned long *words, int nw, int i, TDCEvent *data)
#endif
{
  int nhit = 0;
  unsigned long nevt = 0;
  for (i = 0; i < nw; i++) {
    int type = -1; // type 0(data) 1(tdc header) 2(tdc trailer) 3(global header) 4(tdc error) 5(global trailer)
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
      unsigned long tdc_raw = words[i] & 0x1FFFFF;
      unsigned long tdc_ch = (words[i] >> 21) & 0x1F;
      if (fDebug) cout << "TDC Ch " << tdc_ch << " TDC : " << tdc_raw << endl;
      if (nhit > 1) {
        cout << "Number of tdc hits are too many" << endl;
        return;
      }

      data->tdc[nhit] = tdc_raw;
      data->tdc_ch[nhit] = tdc_raw;
      nhit++;
    }

  }
}


// Number of words in buffer
#ifdef NKC
int TDCRead_NW(int devnum, unsigned long mid)
#else
int NKV792::TDCRead_NW(int devnum, unsigned long mid)
#endif
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 100, addr);

  unsigned long nw = word & 0xFFFF;
  
  return nw;
}


// Number of Events in buffer
#ifdef NKC
int TDCRead_NEVT(int devnum, unsigned long mid)
#else
int NKV792::TDCRead_NEVT(int devnum, unsigned long mid)
#endif
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 100, addr);

  unsigned long ne = (word >> 16) & 0xFFFF;
 
  return ne;
}


// Note : First bit of Status bit is DATA_READY
#ifdef NKC
unsigned long TDCRead_Status(int devnum, unsigned long mid)
#else
unsigned long NKV792::TDCRead_Status(int devnum, unsigned long mid)
#endif
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v1290_ADDR_STATUS;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);
 
  return word;
}


#ifdef NKC
void TDCClear_Buffer(int devnum, unsigned long mid)
#else
void NKV792::TDCClear_Buffer(int devnum, unsigned long mid)
#endif
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFF) << 24;
  
  unsigned long addr = baseaddr + v1290_ADDR_SW_CLEAR;
  
  unsigned long data = 0x1;

  VMEwrite(devnum, A32D16, 100, addr, data);
}



#ifdef NKC
int TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words)
#else
int NKV792::TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words)
#endif
{
  int i, timeout=0;
	unsigned short hs;
  unsigned long baseaddr;
  baseaddr = (mid & 0xFF) << 24;
  unsigned long addr_hs = baseaddr + v1290_ADDR_MICRO_HS;
  unsigned long addr = baseaddr + v1290_ADDR_MICRO;
 
  for(i=0; i<nw; i++) {        
    do {
        hs = VMEread(devnum, A32D16, 100, addr_hs);
        timeout++;
        Sleep(1);
        if (timeout == 3000) return 1;
    }
    while (((hs & 0x01)==0) && (timeout < 3000)); /* wait to write */

    VMEwrite(devnum, A32D16, 100, addr, words[i]);
  }

	return 0;
}