const unsigned short moduleID = 0x1000;

//
//#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
//R__ADD_INCLUDE_PATH("/opt/notice/include")
R__LOAD_LIBRARY(libNK6UVMEROOT.so)
R__LOAD_LIBRARY(libNKV1290.so)
//#endif

using namespace std;
void v1290_daq(int nevt = 10)
{
    int devnum = 0; // Dev. Mount number in linux

    // get NKHOME enviernment
    TString mypath = gSystem->Getenv("NKHOME");
    cout<<"NKHOME pass : "<<mypath<<endl;
    TString myvme  = mypath + TString("/lib/libNK6UVMEROOT.so");  
    TString mytdc = mypath + TString("/lib/libNKV1290.so");
  
    // Loading VME & FADC lib.
    // gSystem->Load("/usr/local/notice/lib/libNK6UVMEROOT.so");
    // gSystem->Load("/usr/local/notice/lib/libNKV1290.so");
     
  
    NKV1290 *tdc_module = new NKV1290();
    cout << "Starting v1290..." << endl;
    cout << "TDC Module Initialized" << endl;
    tdc_module->VMEopen(devnum);
    unsigned long stt = tdc_module->TDCRead_Status(devnum, moduleID);
//    cout << "Stat : " << stt << endl;
    //tdc_module->TDCInit(devnum, moduleID);
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

        tdc_module->TDCClear_Buffer(devnum, moduleID);

        delete tdc_evt;
    }

    cout << "Nice work! All Done" << endl;
}
