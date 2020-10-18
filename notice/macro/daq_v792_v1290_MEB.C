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

const unsigned short moduleID_tdc = 0x1000;
const unsigned short moduleID_adc = 0x2000;
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV1290.so)
R__LOAD_LIBRARY(libNKV792.so)

using namespace std;
void daq_v792_v1290_MEB(int nevt = 2000)
{
    int devnum = 0; // Dev. Mount number in linux

    const int buffer_evt = v792_NEVENT_BUFFER;
    // Tree Branches
    int ntdc = -999;
    double tdc[32] = {-999,};
    int tdc_ch[32] = {-999,};
    long triggerID_tdc = -999;
    long eventID_tdc = -999;
    int nadc = -999;
    long adc[32] = {-999,};
    int adc_ch[32] = {-999,};
    long triggerID_adc = -999;
    long eventID_adc = -999;
    long unix_time = -999;
    int nevt_buffer_adc = -999;
    int nevt_buffer_tdc = -999;
    
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
    tree_out->SetAutoFlush(10000);
    tree_out->Branch("ntdc", &ntdc);
    tree_out->Branch("tdc", tdc, "tdc[ntdc]/D");
    tree_out->Branch("tdc_ch", tdc_ch, "tdc_ch[ntdc]/I");
    tree_out->Branch("triggerID_tdc", &triggerID_tdc, "triggerID_tdc/L");
    tree_out->Branch("eventID_tdc", &eventID_tdc, "eventID_tdc/L");
    tree_out->Branch("nadc", &nadc);
    tree_out->Branch("adc", adc, "adc[nadc]/L");
    tree_out->Branch("adc_ch", adc_ch, "adc_ch[nadc]/I");
    tree_out->Branch("triggerID_adc", &triggerID_adc, "triggerID_adc/L");
    tree_out->Branch("eventID_adc", &eventID_adc, "eventID_adc/L");
    tree_out->Branch("unix_time", &unix_time, "unix_time/L");
    tree_out->Branch("nevt_buffer_tdc", &nevt_buffer_tdc, "nevt_buffer_tdc/I");
    tree_out->Branch("nevt_buffer_adc", &nevt_buffer_adc, "nevt_buffer_adc/I");

    NKV1290 *tdc_module = new NKV1290();
    tdc_module->VMEopen(devnum);

    std::cout << "Starting v1290..." << std::endl;
    tdc_module->TDCInit(devnum, moduleID_tdc, 1);

    std::cout << "Starting v792..." << std::endl;
    NKV792 *adc_module = new NKV792();
    adc_module->ADC_SoftReset(devnum, moduleID_adc);
    adc_module->ADCInit(devnum, moduleID_adc);
    tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
    std::cout << "TDC Module Initialized" << std::endl;
    std::cout << "ADC Module Initialized" << std::endl;

    for (int ievt = 0; ievt < nevt; ievt++) {
      std::cout << "Event " << ievt << std::endl;
      adc_module->ADCClear_Buffer(devnum, moduleID_adc);
      tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
      int itry = 0;
	    while(true) {
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
        
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Elapsed time 1 : " << microseconds << " micro seconds" << std::endl;
        
        unsigned long nw_tdc = tdc_module->TDCRead_Buffer_Test(devnum, moduleID_tdc, words_tdc);
        unsigned long nw_adc = adc_module->ADCRead_Buffer(devnum, moduleID_adc, words_adc);

        elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Elapsed time 2 : " << microseconds << " micro seconds" << std::endl;

//	      cout << "TDC NWord " << nw_tdc << endl;
//        cout << "ADC NWord " << nw_adc << endl;

        elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Elapsed time 3 : " << microseconds << " micro seconds" << std::endl;

        int nevt_tdc = tdc_module->TDCEventBuild_MEB(words_tdc, nw_tdc, tdc_data_arr);
        int nevt_adc = adc_module->ADCEventBuild_MEB(words_adc, nw_adc, adc_data_arr);

        nevt_buffer_adc = nevt_adc;
        nevt_buffer_tdc = nevt_tdc;

        cout << "Found Events in buffer : " << nevt_tdc << ", " << nevt_adc << endl;

        if (nevt_tdc != nevt_adc) {
          cout << "Warning : Number of Event in buffer of ADC, TDC are different" << endl;
        }

        elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Elapsed time 4 : " << microseconds << " micro seconds" << std::endl;

//        cout << "TDC Evt Number : " << tdc_evt->EventNumber << endl;
//        cout << "TDC1 : " << tdc_evt->tdc_ch[0] << " " << tdc_evt->tdc[0] << endl;
//        cout << "TDC2 : " << tdc_evt->tdc_ch[1] << " " << tdc_evt->tdc[1] << endl;
//        cout << "EventID : " << adc_evt->TriggerID << endl;
//        cout << "ADC1 : " << adc_evt->adc_ch[0] << " " << adc_evt->adc[0] << endl;
//        cout << "ADC2 : " << adc_evt->adc_ch[1] << " " << adc_evt->adc[1] << endl;

        for (int ievt = 0; ievt < nevt_tdc; ievt++){
          // Fill TDC Tree
          for (int i = 0; i < 32; i++) {
            tdc[i] = -999;
            tdc_ch[i] = -999;
          }

          triggerID_tdc = -999;
          eventID_tdc = -999;
          ntdc = -999;

          ntdc = tdc_data_arr[ievt].ntdc;
	        cout << "Test NTDC " << ntdc << endl;
          for (int ih = 0; ih < ntdc; ih++) {
            tdc[ih] = (double) tdc_data_arr[ievt].tdc[ih]/40.;
            tdc_ch[ih] = (int) tdc_data_arr[ievt].tdc_ch[ih];
	          cout <<"Test TDC : " << tdc[ih] << " " << tdc_ch[ih] << endl;
          }

          triggerID_tdc = tdc_data_arr[ievt].TriggerID;
          //triggerID_tdc = (long) tdc_module->TDCRead_EventCounter(devnum, moduleID_tdc);
	        eventID_tdc = (int) tdc_data_arr[ievt].EventNumber;

          // Filling ADC Tree
          for (int ih = 0; ih < 32; ih++) {
            adc[ih] = -999;
            adc_ch[ih] = -999;
          }
          triggerID_adc = -999;
          eventID_adc = -999;
          nadc = -999;

          nadc = adc_data_arr[ievt].nadc;
	        cout << "NADC : " << nadc << endl;
          //triggerID_adc = (long) adc_module->ADCRead_TriggerCounter(devnum, moduleID_adc);
          for (int ih = 0; ih < nadc; ih++) {
            adc[ih] = (long) adc_data_arr[ievt].adc[ih];
            adc_ch[ih] = (int) adc_data_arr[ievt].adc_ch[ih];
	          cout << "ADC " << adc[ih] << " Ch " << adc_ch[ih] << endl;
          }
          
          nadc = adc_data_arr[ievt].nadc;
          triggerID_adc = (int) adc_data_arr[ievt].TriggerID;
          
          unix_time = std::time(0);

          tree_out->Fill();
  
          //adc_data_arr[ievt].reset();
          //tdc_data_arr[ievt].reset();
          elapsed = std::chrono::high_resolution_clock::now() - start;
          microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
          std::cout << "Elapsed time 5 : " << microseconds << " micro seconds" << std::endl;
          std::cout << "Total Trigger Count, TDC : " << triggerID_tdc << " ADC : " << triggerID_adc << std::endl;

          if(bStop){
            std::cout << "terminated!" << std::endl;
            break;
          }
        }


        elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Elapsed time Total : " << microseconds << " micro seconds per cycle" << std::endl;
    }

    tree_out->Write();
    file_out->Close();
    cout << "Nice work! All Done" << endl;

    return;
}
