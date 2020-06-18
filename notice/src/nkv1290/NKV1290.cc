#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <TROOT.h>
#include "NKV1290.h"

ClassImp(NKV1290)

using namespace std;

NKV1290::NKV1290()
{
  fDebug = 1;
  printf("NKV1290 Initialization \n");
}

NKV1290::~NKV1290()
{
   printf("Leaving NKV1290 now \n");
}


void NKV1290::TDCSet_TM_Width(int devnum, unsigned long mid, unsigned long wd)
{
  unsigned short opcode[10];

  opcode[0]=0x1000; opcode[1] = wd;

  TDCWrite_Opcode(devnum, mid, 2, opcode);

  return;
}


void NKV1290::TDCSet_TM_Offset(int devnum, unsigned long mid, signed short os)
{
  signed short opcode[10];

  opcode[0]=0x1100; opcode[1]= os;
  
  TDCWrite_Opcode(devnum, mid, 2, opcode);

  return;
}


void NKV1290::TDCInit(int devnum, unsigned long mid)
{
  cout << "Initializing v1290.." << endl;

  unsigned short opcode[10];

  // Enable Trigger matching mode
  opcode[0]=0x0000;
  TDCWrite_Opcode(devnum, mid, 1, opcode);

  // Trigger matching Window width
  TDCSet_TM_Width(devnum, mid, v1290_TM_WIDTH);
  
  //opcode[0]=0x1000; opcode[1] = v1290_TM_WIDTH;
  //TDCWrite_Opcode(devnum, mid, 2, opcode);

  // Trigger matching Window offset
  TDCSet_TM_Offset(devnum, mid, v1290_TM_OFFSET);

  //opcode[0]=0x1100; opcode[1]= v1290_TM_OFFSET;
  //TDCWrite_Opcode(devnum, mid, 2, opcode);

// opcd[0]=0x3100; // Disable TDC Header/Trailer
// TDCWrite_Opcode(devnum, mid, 1, opcode)
  cout << "Done" << endl;

  return;
}


unsigned long NKV1290::TDCRead_Buffer(int devnum, unsigned long mid, unsigned long *words)
{
  if (fDebug) cout << "Reading TDC Data" << endl;
  unsigned long baseaddr;
  unsigned long i;
  unsigned long addr;
  unsigned long nw = 0; // Number of words
  unsigned char rdat[0x100000];
  
  baseaddr = (mid & 0xFFFF) << 16; //A32 mode
  
  addr = baseaddr + v1290_ADDR_DATA;

  nw = TDCRead_NW(devnum, mid);
  int nevt = TDCRead_NEVT(devnum, mid);

  if (nw <= 0) {
    cout << "Empty Buffer" << endl;
    return 0;
  }

  if (fDebug) cout << "NW : " << nw << " NEVT " << nevt << endl;

  if (nevt > 1) {
    cout << "Warning Multiple Events in buffer" << endl;
    return 0;
  }

  VMEblockread(devnum, A32D32, 100, addr, 4*nw, rdat);
  
  // Decoding Words : 32bit
  for (i = 0; i < 4*nw - 3; i=i+4) {
    rdat[i] = rdat[i] << 24;
    rdat[i+1] = rdat[i+1] << 16;
    rdat[i+2] = rdat[i+2] << 8;

    words[i] = rdat[i] + rdat[i+1] + rdat[i+2] + rdat[i+3];
  }

  return nw;
}


// Decoding Words : 32bit
void NKV1290::TDCEventBuild(unsigned long *words, int nw, int i, TDCEvent *data)
{
  int nhit = 0;
//  unsigned long nevt = 0;
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
      data->TriggerID = BunchID;
      data->EventNumber = 0;
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
unsigned long NKV1290::TDCRead_NW(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 1000, addr);

  unsigned long nw = word & 0xFFFF;
  
  return nw;
}


// Number of Events in buffer
unsigned long NKV1290::TDCRead_NEVT(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 100, addr);

  unsigned long ne = (word >> 16) & 0xFFFF;
 
  return ne;
}


// Note : First bit of Status bit is DATA_READY
unsigned long NKV1290::TDCRead_Status(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_STATUS;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);
 
  return word;
}


void NKV1290::TDCClear_Buffer(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_SW_CLEAR;
  
  unsigned long data = 0x1;

  VMEwrite(devnum, A32D16, 100, addr, data);
}



int NKV1290::TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words)
{
  int i, timeout=0;
	unsigned short hs;
  unsigned long baseaddr;
  baseaddr = (mid & 0xFFFF) << 16;
  unsigned long addr_hs = baseaddr + v1290_ADDR_MICRO_HS;
  unsigned long addr = baseaddr + v1290_ADDR_MICRO;
 
  cout << "base " << baseaddr << endl;
  cout << "hs " << v1290_ADDR_MICRO_HS << endl;
  
  for(i=0; i<nw; i++) {
    do {
        hs = VMEread(devnum, A32D16, 100, addr_hs);
        timeout++;
        usleep(1000);
        if (timeout == 3000) return 1;
    }
    while (((hs & 0x01)==0) && (timeout < 3000)); /* wait to write */

    VMEwrite(devnum, A32D16, 100, addr, words[i]);
  }

	return 0;
}
