const unsigned short moduleID = 0x2000;
//#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
//R__ADD_INCLUDE_PATH("/opt/notice/include")
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV792.so)
//#endif

using namespace std;
void v792_daq(int nevt = 100)
{
    // Tree Branches
    int nadc = -999;
    long adc[100] = {-999,};
    int adc_ch[100] = {-999,};
    //long adc0 = -999;
    //int adc_ch0 = -999;
    //long adc1 = -999;
    //int adc_ch1 = -999;
    int triggerID = -999;

    int devnum = 0; // Dev. Mount number in linux

    // get NKHOME enviernment
    //TString mypath = gSystem->Getenv("NKHOME");
    //cout<<"NKHOME pass : "<<mypath<<endl;
    //TString myvme  = mypath + TString("/lib/libNK6UVMEROOT.so");
    //TString myadc = mypath + TString("/lib/libNKV792.so");

    TFile *file_out = new TFile("AnaResult.root", "Recreate");
    TTree *tree_out = new TTree("tree_out", "adc_tree");
    tree_out->SetAutoFlush(10000);
    tree_out->Branch("nadc", &nadc);
    tree_out->Branch("adc", adc, "adc[100]/L");
    tree_out->Branch("adc_ch", adc_ch, "adc_ch[100]/I");
    //tree_out->Branch("adc0", &adc0, "adc0/L");
    //tree_out->Branch("adc_ch0", &adc_ch0);
    //tree_out->Branch("adc1", &adc1, "adc1/L");
    //tree_out->Branch("adc_ch1", &adc_ch1);
    tree_out->Branch("triggerID", &triggerID);

    NKV792 *adc_module = new NKV792();
    cout << "Starting v792..." << endl;
    cout << "ADC Module Initialized" << endl;
    adc_module->VMEopen(devnum);
//    adc_module->ADCInit(devnum, moduleID);


    for (int ievt = 0; ievt < nevt; ievt++) {
        cout << "Event " << ievt << endl;
        int itry = 0;
	    while(true) {
            int dr = adc_module->ADC_IsDataReady(devnum, moduleID);
            int bs = adc_module->ADC_IsBusy(devnum, moduleID);
            itry++;
            cout << "dr " << dr << " bs " << bs << endl;
	        if (dr == 1) break;
	        if (itry > 500000) return;
        }

	    unsigned long words[20000];
        unsigned long nw = adc_module->ADCRead_Buffer(devnum, moduleID, words);

      for (int i = 0; i < 10; i++) {
        cout << "Data Word : " << bitset<32>(words[i]) << endl;
      }

    	if (nw > 50) {
          adc_module->ADCClear_Buffer(devnum, moduleID);
	      continue;
	    }

        cout << "NWord " << nw << endl;
        ADCEvent *adc_evt = new ADCEvent();
        adc_module->ADCEventBuild(words, nw, 0, adc_evt);

        cout << "TriggerID : " << adc_evt->TriggerID << endl;
        cout << "ADC1 : " << adc_evt->adc_ch[0] << " " << adc_evt->adc[0] << endl;
        cout << "ADC2 : " << adc_evt->adc_ch[1] << " " << adc_evt->adc[1] << endl;

        nadc = adc_evt->nadc;
        if (true) {
          for (int ih = 0; ih < 100; ih++) {
            adc[ih] = -999;
            adc_ch[ih] = -999;
          }
          triggerID = -999;
          for (int ih = 0; ih < nadc; ih++) {
            adc[ih] = (long) adc_evt->adc[ih]/40;
            adc_ch[ih] = (int) adc_evt->adc_ch[ih];
          }
            nadc = adc_evt->nadc;
            triggerID = (int) adc_evt->TriggerID;
            tree_out->Fill();
        }

        adc_module->ADCClear_Buffer(devnum, moduleID);

        delete adc_evt;
    }

    tree_out->Write();
    file_out->Close();
    cout << "Nice work! All Done" << endl;
}
