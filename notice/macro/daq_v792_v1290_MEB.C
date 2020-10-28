#include <unistd.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "../include/NKV792.h"
#include "../include/NKV1290.h"
#include <csignal>

bool bStop = false;

void sigint_handler(int sig) {
  bStop = true;
}

const unsigned short moduleID_tdc = 0x1000;
const unsigned short moduleID_adc = 0x2000;
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV1290.so)
R__LOAD_LIBRARY(libNKV792.so)

using namespace std;
void daq_v792_v1290_MEB(int nevt = 15000)
{
  std::signal(SIGINT, sigint_handler);

  int BunchMode = 0; // 0 : Continueous beam, 1 : Bunch-like beam
  int devnum = 0; // Dev. Mount number in linux

  const int buffer_evt = v792_NEVENT_BUFFER + 10;
  long evt_taken = 0;
  // Tree Branches
  int ntdc = -999;
  double tdc[32] = {-999,};
  int tdc_ch[32] = {-999,};
  long triggerID_tdc = -999;
  long EventNumber = 0;
  int nadc = -999;
  long adc[32] = {-999,};
  int adc_ch[32] = {-999,};
  long triggerID_adc = -999;
  long unix_time = -999;
  int nevt_buffer_adc = -999;
  int nevt_buffer_tdc = -999;
  int tdc_buffer = 0;
  //int tdc_nevt_clear = 600; // Maximum number of event in tdc buffer (32k words)

  TDCEvent tdc_data_arr[buffer_evt];
  ADCEvent adc_data_arr[buffer_evt];

  for (int i = 0; i < buffer_evt; i++) {
    tdc_data_arr[i] = TDCEvent();
    tdc_data_arr[i].reset();
    adc_data_arr[i] = ADCEvent();
    adc_data_arr[i].reset();
  }

  TFile *file_out = new TFile("AnaResult.root", "Recreate");
  TTree *tree_out = new TTree("tree_out", "tdc_tree");
  tree_out->SetAutoFlush(1000000);
  tree_out->Branch("ntdc", &ntdc);
  tree_out->Branch("tdc", tdc, "tdc[ntdc]/D");
  tree_out->Branch("tdc_ch", tdc_ch, "tdc_ch[ntdc]/I");
  tree_out->Branch("triggerID_tdc", &triggerID_tdc, "triggerID_tdc/L");
  tree_out->Branch("nadc", &nadc);
  tree_out->Branch("adc", adc, "adc[nadc]/L");
  tree_out->Branch("adc_ch", adc_ch, "adc_ch[nadc]/I");
  tree_out->Branch("triggerID_adc", &triggerID_adc, "triggerID_adc/L");
  tree_out->Branch("EventNumber", &EventNumber, "EventNumber/L");
  tree_out->Branch("unix_time", &unix_time, "unix_time/L");
  tree_out->Branch("nevt_buffer_tdc", &nevt_buffer_tdc, "nevt_buffer_tdc/I");
  tree_out->Branch("nevt_buffer_adc", &nevt_buffer_adc, "nevt_buffer_adc/I");

  std::cout << "Starting v1290..." << std::endl;
  NKV1290 *tdc_module = new NKV1290();
  tdc_module->VMEopen(devnum);
  tdc_module->TDCInit(devnum, moduleID_tdc, 1);
  //tdc_module->TDCSet_AlmostFullLevel(devnum, moduleID_tdc, 0x100);

  std::cout << "Starting v792..." << std::endl;
  NKV792 *adc_module = new NKV792();
  //adc_module->ADC_SoftReset(devnum, moduleID_adc);
  adc_module->ADCInit(devnum, moduleID_adc);

  //Resetting Both Trigger count
  tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
  adc_module->ADCReset_TriggerCounter(devnum, moduleID_adc);
  
  std::cout << "TDC Module Initialized" << std::endl;
  std::cout << "ADC Module Initialized" << std::endl;

  long TriggerID_Offset = 0;
  for (int icycle = 0; icycle < nevt; icycle++) {
    std::cout << "Event Process Cycle : " << icycle << ", Event Taken : " << evt_taken << std::endl;
    cout << "TDC buffer " << tdc_buffer << endl;
    // Saving TriggerID offset before mem clear (only in bunch mode)
    // Memory Buffer Clear
//    int TDC_BufferClear = 1;
//    if (BunchMode == 1) {
//      int TDCAlmostFull = tdc_module->TDC_IsAlmostFull(devnum, moduleID_tdc);
//      cout <<"AlmostFull " << TDCAlmostFull << endl;
//      if (TDCAlmostFull == 0) TDC_BufferClear = 0;
//    }

    if (BunchMode == 1) {
      unsigned long TrID_adc = (unsigned long) adc_module->ADCRead_TriggerCounter(devnum, moduleID_adc);
      unsigned long TrID_tdc = (unsigned long) tdc_module->TDCRead_EventCounter(devnum, moduleID_tdc);
      TriggerID_Offset = TrID_adc - TrID_tdc;
    }

    while (icycle == 1) {
      tdc_buffer = 0;
      tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
      adc_module->ADCClear_Buffer(devnum, moduleID_adc);
      int evt_count_tdc = tdc_module->TDCRead_Event_Stored(devnum, moduleID_tdc);
      cout << "Buffer Clearing : " << evt_count_tdc << endl;
      // Check if tdc memory is empty if not, clear again, if trigger rate is about 8khz or larger, it may need few trial
      if (evt_count_tdc == 0) break;
      cout << "TDC buffer is not empty, Retrying Clear.." << endl;
    }
    
//    if (TDC_BufferClear == 0) adc_module->ADCClear_Buffer(devnum, moduleID_adc);
   
    int itry = 0;
    while (true) {
      unsigned long stat_tdc = tdc_module->TDCRead_Status(devnum, moduleID_tdc);
      int dr_adc = adc_module->ADC_IsDataReady(devnum, moduleID_adc);
      //int bs_adc = adc_module->ADC_IsBusy(devnum, moduleID_adc);
      itry++;
      //cout << "dr " << dr << " bs " << bs << endl;
      if ((stat_tdc & 0x1) == 1 && dr_adc == 1) break;
      if (itry > 500000) return;
    }

    if (BunchMode == 1) usleep(200); // 200us waiting until bunch end
    // Measuring elapsed time per an event process
    auto start = std::chrono::high_resolution_clock::now();

    unsigned long words_tdc[2048];
    unsigned long words_adc[2048];

    unsigned long nw_tdc = tdc_module->TDCRead_Buffer_MEB(devnum, moduleID_tdc, words_tdc);
    unsigned long nw_adc = adc_module->ADCRead_Buffer(devnum, moduleID_adc, words_adc);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "Elapsed time 1 : " << microseconds << " micro seconds" << std::endl;

    int nevt_adc = adc_module->ADCEventBuild_MEB(words_adc, nw_adc, adc_data_arr);
    int nevt_tdc = tdc_module->TDCEventBuild_MEB(words_tdc, nw_tdc, tdc_data_arr);

    // event overflow
    if (nevt_adc >= 32 || nevt_tdc >= 40) {
      cout << "Event Overflow, Discarding event" << endl;
      continue;
    }

    nevt_buffer_adc = nevt_adc;
    nevt_buffer_tdc = nevt_tdc;

    cout << "Found Events in buffer : " << nevt_tdc << ", " << nevt_adc << endl;

    if (nevt_tdc > buffer_evt || nevt_adc > buffer_evt) {
      cout << "Warning : Number of Event in buffer overflow" << endl;
      continue;
    }

    if (nevt_tdc != nevt_adc) {
      cout << "Warning : Number of Event in buffer of ADC, TDC are different" << endl;
    }

    elapsed = std::chrono::high_resolution_clock::now() - start;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "Elapsed time 2 : " << microseconds << " micro seconds" << std::endl;


    if (BunchMode == 0 && icycle == 1) TriggerID_Offset = adc_data_arr[0].TriggerID - tdc_data_arr[0].TriggerID;

    int niter = nevt_adc;
    if (nevt_adc > nevt_tdc)
      niter = nevt_tdc;
    for (int ievt = 0; ievt < niter; ievt++) {
      // Sync offset correction, positive correction means adc event is late relative to tdc event
      // Positive -> adc should match with later tdc event
      int Sync_Correction = (adc_data_arr[ievt].TriggerID - tdc_data_arr[ievt].TriggerID) - TriggerID_Offset;
      int tdc_index_correction = 0;
      int adc_index_correction = 0;
      // ADC-Late casd
      if (Sync_Correction > 0) tdc_index_correction = Sync_Correction;
      if (Sync_Correction < 0) adc_index_correction = -1*Sync_Correction;
      // Fill TDC Tree
      for (int i = 0; i < 32; i++) {
        tdc[i] = -999;
        tdc_ch[i] = -999;
      }

      triggerID_tdc = -999;
      ntdc = -999;

      ntdc = tdc_data_arr[ievt + tdc_index_correction].ntdc;
      //	        cout << "Test NTDC " << ntdc << endl;
      for (int ih = 0; ih < ntdc; ih++) {
        tdc[ih] = (double)tdc_data_arr[ievt + tdc_index_correction].tdc[ih] / 40.;
        tdc_ch[ih] = (int)tdc_data_arr[ievt + tdc_index_correction].tdc_ch[ih];
        //	          cout <<"Test TDC : " << tdc[ih] << " " << tdc_ch[ih] << endl;
      }

      cout << "Ref TrOffset " << TriggerID_Offset << endl;
      cout << "Evt " << ievt << " trID offset : " << adc_data_arr[ievt + tdc_index_correction].TriggerID - tdc_data_arr[ievt + tdc_index_correction].TriggerID << endl;

      triggerID_tdc = tdc_data_arr[ievt + tdc_index_correction].TriggerID + TriggerID_Offset;

      //triggerID_tdc = (long) tdc_module->TDCRead_EventCounter(devnum, moduleID_tdc);

      // Filling ADC Tree
      for (int ih = 0; ih < 32; ih++) {
        adc[ih] = -999;
        adc_ch[ih] = -999;
      }
      triggerID_adc = -999;
      nadc = -999;

      nadc = adc_data_arr[ievt + adc_index_correction].nadc;
      //	        cout << "NADC : " << nadc << endl;
      for (int ih = 0; ih < nadc; ih++) {
        adc[ih] = (long)adc_data_arr[ievt + adc_index_correction].adc[ih];
        adc_ch[ih] = (int)adc_data_arr[ievt + adc_index_correction].adc_ch[ih];
        //	          cout << "ADC " << adc[ih] << " Ch " << adc_ch[ih] << endl;
      }

      nadc = adc_data_arr[ievt + adc_index_correction].nadc;
      triggerID_adc = (int)adc_data_arr[ievt + adc_index_correction].TriggerID;
      //triggerID_adc = (long) adc_module->ADCRead_TriggerCounter(devnum, moduleID_adc);

      unix_time = std::time(0);

      // Fill Tree only when both tdc and adc are available
      if (triggerID_adc > 0 && triggerID_tdc > 0) {
        tree_out->Fill();
        EventNumber += 1;
      }

      if (icycle%100 == 0) tree_out->Write();
      
      //adc_data_arr[ievt].reset();
      //tdc_data_arr[ievt].reset();
    }

    evt_taken += niter;
    tdc_buffer += nevt_tdc;

    if (bStop) {
      std::cout << "terminated!" << std::endl;
      break;
    }

    elapsed = std::chrono::high_resolution_clock::now() - start;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "Elapsed time Total : " << microseconds << " micro seconds per cycle" << std::endl;
  }

  delete tdc_module;
  delete adc_module;

  tree_out->Write();
  file_out->Close();
  cout << "Nice work! All Done" << endl;

  return;
}
