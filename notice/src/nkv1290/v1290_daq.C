const unsigned short moduleID = 0x01;

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
//R__ADD_INCLUDE_PATH("/opt/notice/include")
R__LOAD_LIBRARY(libNotice6UVME_root.so)
R__LOAD_LIBRARY(libNoticeNFADC400_root.so)
#endif

using namespace std;
void v1290_daq(int nevt = 10)
{
    int devnum = 1; // Dev. Mount number in linux

    // get NKHOME enviernment
    TString mypath = gSystem->Getenv("NKHOME");
    cout<<"NKHOME pass : "<<mypath<<endl;
    TString myvme  = mypath + TString("/lib/libNK6UVMEROOT.so");  
    TString mytdc = mypath + TString("/lib/libNKV1290.so");
  
    // Loading VME & FADC lib.
    gSystem->Load(myvme);
    gSystem->Load(mytdc);

    NKV1290 *tdc_module = new NKV1290();
    cout << "Starting v1290..." << endl;
    //tdc_module->TDCInit(devnum, moduleID);
    cout << "TDC Module Initialized" << endl;
    tdc_module->VMEopen(devnum);

    for (int ievt = 0; ievt < nevt; ievt++) {
        cout << "Event " << ievt << endl;
        while(true) {
            unsigned long stat = tdc_module->GetStatus(devnum, moduleID);
            if (stat & 0x1) break;
        }

        unsigned long ne = tdc_module->TDCRead_NEVT(devnum, moduleID);

        unsigned long words[20000];
        unsigned long nw = tdc_module->TDCRead_Buffer(devnum, moduleID, words);
        cout << "NWord " << nw << endl;
        tdc_module->TDCClear_Buffer(devnum, moduleID);
    }

    cout << "Nice work! All Done" << endl;
}
