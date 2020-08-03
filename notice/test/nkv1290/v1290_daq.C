const unsigned short moduleID = 0x1000;
//#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
//R__ADD_INCLUDE_PATH("/opt/notice/include")
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV1290.so)
//#endif

using namespace std;
void v1290_daq(int nevt = 100)
{
    // Tree Branches
    int ntdc = -999;
    long tdc[100] = {-999,};
    int tdc_ch[100] = {-999,};
//    long tdc1 = -999;
//    int tdc_ch1 = -999;
    int triggerID = -999;
    int eventID = -999;

    int devnum = 0; // Dev. Mount number in linux

    // get NKHOME enviernment
    TString mypath = gSystem->Getenv("NKHOME");
    cout<<"NKHOME pass : "<<mypath<<endl;
    TString myvme  = mypath + TString("/lib/libNK6UVMEROOT.so");
    TString mytdc = mypath + TString("/lib/libNKV1290.so");

    TFile *file_out = new TFile("AnaResult.root", "Recreate");
    TTree *tree_out = new TTree("tree_out", "tdc_tree");
    tree_out->SetAutoFlush(10000);
    tree_out->Branch("ntdc", &ntdc);
    tree_out->Branch("tdc", tdc, "tdc[100]/L");
    tree_out->Branch("tdc_ch", tdc_ch, "tdc_ch[100]/I");
//    tree_out->Branch("tdc1", &tdc1, "tdc1/L");
//    tree_out->Branch("tdc_ch1", &tdc_ch1);
    tree_out->Branch("triggerID", &triggerID);
    tree_out->Branch("eventID", &eventID);

    // Loading VME & FADC lib.
    // gSystem->Load("/usr/local/notice/lib/libNK6UVMEROOT.so");
    // gSystem->Load("/usr/local/notice/lib/libNKV1290.so");
     
    NKV1290 *tdc_module = new NKV1290();
    cout << "Starting v1290..." << endl;
    cout << "TDC Module Initialized" << endl;
    tdc_module->VMEopen(devnum);
    unsigned long stt = tdc_module->TDCRead_Status(devnum, moduleID);
//    cout << "Stat : " << stt << endl;
    tdc_module->TDCInit(devnum, moduleID, 1);
    tdc_module->TDCClear_Buffer(devnum, moduleID);

    for (int ievt = 0; ievt < nevt; ievt++) {
        cout << "Event " << ievt << endl;
        int itry = 0;
	    while(true) {
            unsigned long stat = tdc_module->TDCRead_Status(devnum, moduleID);
            itry++;
	        if ((stat & 0x1) == 1) break;
	        if (itry > 500000) return;
        }

	    unsigned long words[20000];
        //unsigned long nw = tdc_module->TDCRead_Buffer(devnum, moduleID, words);
        unsigned long nw = tdc_module->TDCRead_Buffer(devnum, moduleID, words);
        cout << "NWord " << nw << endl;
        TDCEvent *tdc_evt = new TDCEvent();
        tdc_module->TDCEventBuild(words, nw, 0, tdc_evt);

        cout << "Evt Number : " << tdc_evt->EventNumber << endl;
        cout << "TDC1 : " << tdc_evt->tdc_ch[0] << " " << tdc_evt->tdc[0] << endl;
        cout << "TDC2 : " << tdc_evt->tdc_ch[1] << " " << tdc_evt->tdc[1] << endl;
//        cout << "TDC3 : " << tdc_evt->tdc_ch[2] << " " << tdc_evt->tdc[2] << endl;
//        cout << "TDC4 : " << tdc_evt->tdc_ch[3] << " " << tdc_evt->tdc[3] << endl;
//        cout << "TDC5 : " << tdc_evt->tdc_ch[4] << " " << tdc_evt->tdc[4] << endl;
//        cout << "TDC6 : " << tdc_evt->tdc_ch[5] << " " << tdc_evt->tdc[5] << endl;

        ntdc = tdc_evt->ntdc;
        if (true) {
          for (int ih = 0; ih < ntdc; ih++) {
            tdc[ih] = -999;
            tdc_ch[ih] = -999;
            tdc[ih] = (long) tdc_evt->tdc[ih];
            tdc_ch[ih] = (int) tdc_evt->tdc_ch[ih];
          }

          triggerID = -999;
          eventID = -999;
          ntdc = tdc_evt->ntdc;
          triggerID = (int) tdc_evt->TriggerID;
          eventID = (int) tdc_evt->EventNumber;
          tree_out->Fill();
        }

        tdc_module->TDCClear_Buffer(devnum, moduleID);

        delete tdc_evt;
    }

    tree_out->Write();
    file_out->Close();
    cout << "Nice work! All Done" << endl;
}
