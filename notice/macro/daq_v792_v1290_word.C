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
void daq_v792_v1290_word(int nevt = 3000)
{
  std::signal(SIGINT, sigint_handler);

  int devnum = 0; // Dev. Mount number in linux
  long evt_taken = 0;

  // Tree Branches
  int nword_tdc = -999;
  long tdc_word[32] = {-999,};
  int nword_adc = -999;
  long adc_word[32] = {-999,};
  long unix_time = -999;
  //int tdc_nevt_clear = 600; // Maximum number of event in tdc buffer (32k words)

  TFile *file_out = new TFile("AnaResultRaw.root", "Recreate");
  TTree *tree_out = new TTree("tree_out", "tdc_tree");
  tree_out->SetAutoFlush(10000);
  tree_out->Branch("nword_tdc", &nword_tdc);
  tree_out->Branch("tdc_word", tdc_word, "tdc_word[nword_tdc]/L");
  tree_out->Branch("nword_adc", &nword_adc);
  tree_out->Branch("adc_word", adc_word, "adc_word[nword_adc]/L");
  tree_out->Branch("unix_time", &unix_time, "unix_time/L");

  NKV1290 *tdc_module = new NKV1290();
  tdc_module->VMEopen(devnum);

  std::cout << "Starting v1290..." << std::endl;
  tdc_module->TDCInit(devnum, moduleID_tdc, 1);

  std::cout << "Starting v792..." << std::endl;
  NKV792 *adc_module = new NKV792();
  //adc_module->ADC_SoftReset(devnum, moduleID_adc);
  adc_module->ADCInit(devnum, moduleID_adc);
  int tdc_buffer = 0;
  std::cout << "TDC Module Initialized" << std::endl;
  std::cout << "ADC Module Initialized" << std::endl;

  for (int icycle = 0; icycle < nevt; icycle++) {
    std::cout << "Event Process Cycle : " << icycle << std::endl;
    while (true) {
      tdc_buffer = 0;
      tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
      adc_module->ADCClear_Buffer(devnum, moduleID_adc);
      int evt_count_tdc = tdc_module->TDCRead_Event_Stored(devnum, moduleID_tdc);
      cout << "Buffer Clearing : " << evt_count_tdc << endl;
      // Check if tdc memory is empty if not, clear again, if trigger rate is about 8khz or larger, it may need few trial
      if (evt_count_tdc == 0) break;
      cout << "TDC buffer is not empty, Retrying Clear.." << endl;
    }

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
    // Measuring elapsed time per an event process
    auto start = std::chrono::high_resolution_clock::now();

    unsigned long words_tdc[2048];
    unsigned long words_adc[2048];

    unsigned long nw_tdc = tdc_module->TDCRead_Buffer_MEB(devnum, moduleID_tdc, words_tdc);
    unsigned long nw_adc = adc_module->ADCRead_Buffer(devnum, moduleID_adc, words_adc);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "Elapsed time 1 : " << microseconds << " micro seconds" << std::endl;

    // Fill TDC Tree
    nword_tdc = 0;
    for (int i = 0; i < 32; i++) {
      tdc_word[i] = -999;
    }

    nword_tdc = (int) nw_tdc;
    cout << "TDC Valid NWord : " << nword_tdc << endl;
    for (int iw = 0; iw < nw_tdc; iw++) {
      tdc_word[iw] = (long) words_tdc[iw];
    }

    // Fill ADC Tree
    nword_adc = 0;
    for (int i = 0; i < 32; i++) {
      adc_word[i] = -999;
    }

    nword_adc = (int) nw_adc;
    cout << "ADC Valid NWord : " << nword_adc << endl;
    for (int iw = 0; iw < nw_adc; iw++) {
      adc_word[iw] = (long) words_adc[iw];
    }

    unix_time = std::time(0);

    tree_out->Fill();

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
