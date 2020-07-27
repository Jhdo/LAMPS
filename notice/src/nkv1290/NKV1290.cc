#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <TROOT.h>
#include <bitset>
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


void NKV1290::TDCSet_TM_Width(int devnum, unsigned long mid, unsigned short wd)
{
  unsigned short opcode[10];

  opcode[0]=0x1000; opcode[1] = (wd & 0xFFFF);

  TDCWrite_Opcode(devnum, mid, 2, opcode);

  return;
}


void NKV1290::TDCSet_TM_Offset(int devnum, unsigned long mid, signed short os)
{
  unsigned short opcode[10];

  opcode[0]=0x1100; opcode[1]= (os & 0xFFFF);
  
  TDCWrite_Opcode(devnum, mid, 2, opcode);

  return;
}


void NKV1290::TDCInit(int devnum, unsigned long mid, int ReadOutMode)
{
  cout << "Initializing v1290.." << endl;
  fReadOutMode = ReadOutMode;

  unsigned short control = TDCRead_Control(devnum, mid);

  control = control | (0x0001 << 8); // set 1 on 8th bit

  //cout << "Control bit : " << std::hex << control << endl;

  TDCWrite_Control(devnum, mid, control);

  unsigned short opcode[10];

  // Enable Trigger matching mode
  if (ReadOutMode) opcode[0]=0x0000;
  else opcode[0] = 0x0100;
  TDCWrite_Opcode(devnum, mid, 1, opcode);

  // Trigger matching Window width
  TDCSet_TM_Width(devnum, mid, v1290_TM_WIDTH);

  // Trigger matching Window offset
  TDCSet_TM_Offset(devnum, mid, v1290_TM_OFFSET);

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
  unsigned long nw_read = 0; // Number of words
  unsigned char rdat[10000];
  unsigned long rdat_32bit[10000];
  
  baseaddr = (mid & 0xFFFF) << 16; //A32 mode
  
  addr = baseaddr + v1290_ADDR_DATA;

  int nevt = TDCRead_FIFO_Stored(devnum, mid);
  for (int ie = 0; ie < nevt; ie++) {    
    unsigned long nw = TDCRead_NW(devnum, mid);
    nw_read = nw_read + nw;
  }

  // Testing
  nw_read = nw_read+5;

  if (fReadOutMode) cout << "Trigger Matching Mode" << endl;
  else {
    cout << "Continueous Storing Mode Testing.." << endl;
    nw_read = 15;
  }

  if (fDebug) cout << "NW : " << nw_read << " NEVT " << nevt << endl;

  if (nw_read <= 0 ) {
    cout << "Empty Buffer" << endl;
    return 0;
  }

  if (nw_read >= 10000 ) {
    cout << "Too many words" << endl;
    return 0;
  }

  if (nevt > 1) {
    cout << "Warning Multiple Events in buffer" << endl;
  //  return 0;
  }

  // Note : lower idex in rdat lower addr, lower parts of bits?
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

  return nw_read;
}


// Testing data buffer addr approaching 
unsigned long NKV1290::TDCRead_Buffer_Test(int devnum, unsigned long mid, unsigned long *words)
{
  if (fDebug) cout << "Reading TDC Data" << endl;
  unsigned long baseaddr;
  unsigned long addr;
  unsigned long nw_read = 0; // Number of words
  
  baseaddr = (mid & 0xFFFF) << 16; //A32 mode
  
  addr = baseaddr + v1290_ADDR_DATA;

  int nevt = TDCRead_FIFO_Stored(devnum, mid);
  for (int ie = 0; ie < nevt; ie++) {    
    unsigned long nw = TDCRead_NW(devnum, mid);
    nw_read = nw_read + nw;
  }

  if (fReadOutMode) cout << "Trigger Matching Mode" << endl;
  else {
    cout << "Continueous Storing Mode Testing.." << endl;
    nw_read = 15;
  }

  if (fDebug) cout << "NW : " << nw_read << " NEVT " << nevt << endl;

  if (nw_read <= 0 ) {
    cout << "Empty Buffer" << endl;
    return 0;
  }

  if (nw_read >= 10000 ) {
    cout << "Too many words" << endl;
    return 0;
  }

  if (nevt > 1) {
    cout << "Warning Multiple Events in buffer" << endl;
  //  return 0;
  }

  for (int ir = 0; ir < (int) nw_read; ir++) {
    addr = baseaddr + v1290_ADDR_DATA + ir*4;
    words[ir] = VMEread(devnum, A32D32, 100, addr);
  }

  return nw_read;
}


// Decoding Words : 32bit
void NKV1290::TDCEventBuild(unsigned long *words, int nw, int iw, TDCEvent *data)
{
  int nhit = 0;
  string type_name[6] = {"TDC Data", "TDC Header", "TDC Trailer", "TDC Global Header",  "TDC Error", "TDC Global Trailer"};
//  unsigned long nevt = 0;
  for (int i = iw; i < nw; i++) {
    int type = -1; // type 0(data) 1(tdc header) 2(tdc trailer) 3(global header) 4(tdc error) 5(global trailer)
    cout << "DataWord " << std::bitset<32>(words[i]) << endl;
    unsigned long type_code = (words[i] >> 27) & 0x1F;
    if (fDebug) cout << "Type Code : " << type_code << endl;
//    unsigned long type_code = words[i] & 0xA98A58;
    if (type_code == 0x0) type = 0;
    else if (type_code == 0x1) type = 1;
    else if (type_code == 0x3) type = 2;
    else if (type_code == 0x8) type = 3;
    else if (type_code == 0x4) type = 4;
    else if (type_code == 0x10) type = 5;
    if (fDebug) cout << "Word Type : " << type_name[type].c_str() << endl;

    if (type_code == 0) {
      unsigned long tdc_raw = words[i] & 0x1FFFFF;
      unsigned long tdc_ch = (words[i] >> 21) & 0x0F;
      if (fDebug) cout << "TDC Ch " << tdc_ch << " TDC : " << tdc_raw << endl;
      if (nhit >= 500) {
        cout << "Number of tdc hits are too many" << endl;
        return;
      }

      data->tdc[nhit] = tdc_raw;
      data->tdc_ch[nhit] = tdc_ch;
      nhit++;
      data->ntdc = nhit;
    }

    if (type_code == 1) {
      unsigned long BunchID = words[i] & 0xFFF;
      unsigned long EventID = (words[i] >> 12) & 0xFFF;
      if (fDebug) cout << "BunchID : " << BunchID << " EventID : " << EventID << endl;
      data->TriggerID = BunchID;
      data->EventNumber = 0;
    }

    if (type_code == 2) {
      unsigned long WordCount = words[i] & 0xFFF;
      unsigned long EventID = (words[i] >> 12) & 0xFFF;
      if (fDebug) cout << "WordCount : " << WordCount << " EventID : " << EventID << endl;
    }

    if (type_code == 3) {
      unsigned long EventCount = (words[i] >> 5) & 0x3FFFFF;
      if (fDebug) cout << "EventCount : " << EventCount << endl;
    }

    if (type_code == 4) {
      cout << "Warning TDC Error Word found" << endl;
    }

    if (type_code == 5) {
      cout << "Met Global Trailer Word" << endl;
    }
  } // nw loop

  return;
}


// Number of words in buffer
unsigned long NKV1290::TDCRead_NW(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 100, addr);

  unsigned long nw = word & 0xFFFF;
  
  return nw;
}


// Number of Events in buffer
unsigned long NKV1290::TDCRead_EVTID(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO;
  
  unsigned long word = VMEread(devnum, A32D32, 100, addr);

  unsigned long ne = (word >> 16) & 0xFFFF;
 
  return ne;
}


unsigned long NKV1290::TDCRead_FIFO_Stored(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_FIFO_STORED;
  
  unsigned long word = VMEread(devnum, A32D16, 100, addr);

  unsigned long ne = word & 0x07FF;
  
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
  
  //unsigned short data = 0;

  VMEwrite(devnum, A32D16, 100, addr, 0);
}


unsigned long NKV1290::TDCRead_Control(int devnum, unsigned long mid)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_CONTROL;

  unsigned long word = VMEread(devnum, A32D16, 100, addr);

  return word;
}


void NKV1290::TDCWrite_Control(int devnum, unsigned long mid, unsigned long word)
{
  unsigned long baseaddr;
  
  baseaddr = (mid & 0xFFFF) << 16;
  
  unsigned long addr = baseaddr + v1290_ADDR_CONTROL;

  VMEwrite(devnum, A32D16, 100, addr, word);

  return;
}


int NKV1290::TDCWrite_Opcode(int devnum, unsigned long mid, int nw, unsigned short *words)
{
  int i, timeout=0;
	unsigned short hs;
  unsigned long baseaddr;
  baseaddr = (mid & 0xFFFF) << 16;
  unsigned long addr_hs = baseaddr + v1290_ADDR_MICRO_HS;
  unsigned long addr = baseaddr + v1290_ADDR_MICRO;
 
  //cout << "base " << baseaddr << endl;
  //cout << "hs " << v1290_ADDR_MICRO_HS << endl;
  
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
