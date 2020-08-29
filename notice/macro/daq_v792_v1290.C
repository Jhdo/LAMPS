#include <unistd.h>

const unsigned short moduleID_tdc = 0x1000;
const unsigned short moduleID_adc = 0x2000;
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV1290.so)
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV792.so)

using namespace std;
void daq_v792_v1290(int nevt = 1000)
{
    int devnum = 0; // Dev. Mount number in linux

    // Tree Branches
    int ntdc = -999;
    double tdc[100] = {-999,};
    int tdc_ch[100] = {-999,};
    long triggerID_tdc = -999;
    long eventID_tdc = -999;
    int nadc = -999;
    long adc[100] = {-999,};
    int adc_ch[100] = {-999,};
    long triggerID_adc = -999;
    long eventID_adc = -999;

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

    NKV1290 *tdc_module = new NKV1290();
    tdc_module->VMEopen(devnum);

    cout << "Starting v1290..." << endl;
    tdc_module->TDCInit(devnum, moduleID_tdc, 1);
    tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
    cout << "TDC Module Initialized" << endl;

    NKV792 *adc_module = new NKV792();
    cout << "Starting v792..." << endl;
    adc_module->ADCInit(devnum, moduleID_adc);
    cout << "ADC Module Initialized" << endl;

    for (int ievt = 0; ievt < nevt; ievt++) {
        cout << "Event " << ievt << endl;
        int itry = 0;
	    while(true) {
            unsigned long stat_tdc = tdc_module->TDCRead_Status(devnum, moduleID_tdc);
            int dr_adc = adc_module->ADC_IsDataReady(devnum, moduleID_adc);
            int bs_adc = adc_module->ADC_IsBusy(devnum, moduleID_adc);
            itry++;
            //cout << "dr " << dr << " bs " << bs << endl;
	        if ((stat_tdc & 0x1) == 1 && dr_adc == 1) break;
	        if (itry > 500000) return;
      }
        
        unsigned long words_tdc[20000];
        unsigned long words_adc[20000];

        unsigned long nw_tdc = tdc_module->TDCRead_Buffer(devnum, moduleID_tdc, words_tdc);
        unsigned long nw_adc = adc_module->ADCRead_Buffer(devnum, moduleID_adc, words_adc);

	      cout << "TDC NWord " << nw_tdc << endl;
        cout << "ADC NWord " << nw_adc << endl;
        TDCEvent *tdc_evt = new TDCEvent();
        ADCEvent *adc_evt = new ADCEvent();

        tdc_module->TDCEventBuild(words_tdc, nw_tdc, 0, tdc_evt);
        adc_module->ADCEventBuild(words_adc, nw_adc, 0, adc_evt);
        cout << "TDC Evt Number : " << tdc_evt->EventNumber << endl;
        cout << "TDC1 : " << tdc_evt->tdc_ch[0] << " " << tdc_evt->tdc[0] << endl;
        cout << "TDC2 : " << tdc_evt->tdc_ch[1] << " " << tdc_evt->tdc[1] << endl;
        cout << "EventID : " << adc_evt->TriggerID << endl;
        cout << "ADC1 : " << adc_evt->adc_ch[0] << " " << adc_evt->adc[0] << endl;
        cout << "ADC2 : " << adc_evt->adc_ch[1] << " " << adc_evt->adc[1] << endl;

        // Fill TDC Tree
        for (int i = 0; i < 100; i++) {
          tdc[i] = -999;
          tdc_ch[i] = -999;
        }
        triggerID_tdc = -999;
        eventID_tdc = -999;
        ntdc = -999;

        ntdc = tdc_evt->ntdc;
        if (true) {
          for (int ih = 0; ih < ntdc; ih++) {
            tdc[ih] = (double) tdc_evt->tdc[ih]/40.;
            tdc_ch[ih] = (int) tdc_evt->tdc_ch[ih];
          }

          ntdc = tdc_evt->ntdc;
          //triggerID_tdc = tdc_evt->TriggerID;
          triggerID_tdc = (long) tdc_module->TDCRead_EventCounter(devnum, moduleID_tdc);
          eventID_tdc = (int) tdc_evt->EventNumber;
        }

        // Filling ADC Tree
        nadc = adc_evt->nadc;
        if (true) {
          for (int ih = 0; ih < 100; ih++) {
            adc[ih] = -999;
            adc_ch[ih] = -999;
          }
          triggerID_adc = -999;
          eventID_adc = -999;
          triggerID_adc = (long) adc_module->ADCRead_TriggerCounter(devnum, moduleID_adc);
          for (int ih = 0; ih < nadc; ih++) {
            adc[ih] = (long) adc_evt->adc[ih];
            adc_ch[ih] = (int) adc_evt->adc_ch[ih];
          }
            nadc = adc_evt->nadc;
            eventID_adc = (int) adc_evt->EventID;
        }

        tree_out->Fill();
        //tdc_module->TDCClear_Buffer(devnum, moduleID_tdc);
        adc_module->ADCClear_Buffer(devnum, moduleID_adc);
        delete tdc_evt;
        delete adc_evt;
        usleep(100);
    }

    tree_out->Write();
    file_out->Close();
    cout << "Nice work! All Done" << endl;

    return;
}