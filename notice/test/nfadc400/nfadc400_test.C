// Test program of NFADC400 
// Usgage :  ROOT> .L nfadc400_test.C
//           ROOT> run_nfadc400(1000) //for 1000 events data case
//
//                                         Sept 2006 H.J.Kim hongjoo@knu.ac.kr
//                                         Updated Dec 2006 H.J. Kim
//                                         Updated April 26, 2015 H.O.Kim(hokim@noticekorea.com)

#include "nfadc400_test.h"
R__LOAD_LIBRARY(libNoticeNFADC400_root.so)

unsigned short  data[NDATA];
int run_nfadc400(int Nevent = 10) {
//int run_nfadc400(int Nevent = 1) {

  // check setting variable
  cout << "Total evt number for data taking= "<< Nevent <<endl;
  cout << "FADC channel number = "<< NCH <<endl;
  cout << "Total data point = "<<DATA_POINT <<endl;
  cout << "FADC array size = "<<NDATA <<endl;

  //calculation of recording length from DATA_POINT
  const float bintons = 2.5; //2.5 for 400Mhz
  
  int record_length=DATA_POINT/0x80;   //0x80 = 0.32us,0x100 = 0.64us,DATA_POINT = 10.24us
//  int record_length=16;
  int event_number =0x80000/DATA_POINT;   //0x80000 = 4096,DATA_POINT = 32
//  int event_number =1000;
  
  if(event_number > 512) event_number = 512;  //Refer Manual

  int record_time = int(DATA_POINT * bintons); 
  cout << "Total recording time in ns  = "<<record_time <<endl;
 
  // get NKHOME enviernment
  TString mypath = gSystem->Getenv("NKHOME");
  cout<<"NKHOME pass : "<<mypath<<endl;
  TString myvme  = mypath + TString("/lib/libNotice6UVME_root.so");  
  TString myfadc = mypath + TString("/lib/libNoticeNFADC400_root.so");

  
  // Loading VME & FADC lib.
  //gSystem->Load(myvme);
  //gSystem->Load(myfadc);
   
  //local variable 
  Int_t i;  
  ULong_t NC = NCH;
  ULong_t ND = NDATA;
  unsigned char tag[6];
  ULong_t trigger;

  int mid = 0x0;    // Module ID --> Set by DIP switch on the board

  // open Fadc next
  int nkusb =0; //nkvmeusb driver id

  // Class
  NKNFADC400 kadc;
  
  //VME open  
  kadc.VMEopen(nkusb);
  //tree class
  fadc fadc1; //2017.09.29
  head head1; //2017.09.29

  TCanvas *c1 = new TCanvas("c1", "NFADC400", 800, 800);
  c1->Divide(1,2);
  
  TFile *hfile=new TFile("nfadc400.root","RECREATE");
  TTree *fadc400=new TTree("nfadc400","nfadc400 file");
  TTree *head=new TTree("head","Head of Run");
  TString bfadc = TString("trigger[2]/b:tag[6]/b:FADC[") + ND + TString("]/F");  
  cout<<"bfadc = "<<bfadc<<endl;  
  fadc400->Branch("fadc",&fadc1,bfadc);
  TString bhead = TString("rl/I:offset[") + NC + TString("]/I:gain[") + NC + TString("]/I:ped[") + NC 
    + TString("]/I:delay[") + NC + TString("]/I:tlt/I:tow/I:dce/I:pol[") + NC + TString("]/I:thr[")+ NC 
    + TString("]/I:dt[") + NC + TString("]/I:cw[") + NC + TString("]/I:tm[") + NC 
    + TString("]/I:pct[") + NC + TString("]/I:pci[") + NC + TString("]/I:pwt[") + NC + TString("]/F");
  cout<<"head = "<<bhead<<endl;  
  cout << "START !@#!#!!!!!!! " << endl;
  head->Branch("head1",&head1,bhead);
  TH1F *hoscd1 = new TH1F("hoscd1", "Channel1", DATA_POINT, 0, DATA_POINT*bintons);
  hoscd1->SetOption("HIST");
  TH1F *hoscd2 = new TH1F("hoscd2", "Channel2", DATA_POINT, 0, DATA_POINT*bintons);
  hoscd2->SetOption("HIST");

  cout << "weqeSTART !@#!#!!!!!!! " << endl;
  // open NFADC400
  kadc.NFADC400open(nkusb, mid);
  cout << "START !@#!#!!!!!!! " << endl;
  
  // check FADC status
  cout << "START !@#!#!!!!!!! " << endl;
  printf("Status = %lX\n", kadc.NFADC400read_STAT(nkusb, mid));
  cout << "START !@#!#!!!!!!! " << endl;

  // set reset mode, arguments = (device number, mid, timer reset, event number reset, register reset)
  kadc.NFADC400write_RM(nkusb, mid, 0, 1, 0);

  // reset FADC
  kadc.NFADC400reset(nkusb, mid);

  // set recording length
  kadc.NFADC400write_RL(nkusb, mid, record_length);    
  head1.rl =  kadc.NFADC400read_RL(nkusb, mid); 
      
  // set offset to 3200(range = 0~4095)
  kadc.NFADC400write_DACOFF(nkusb, mid, 0, 3200);
  for (i = 0; i < NCH; i++)
    head1.off[i] =  kadc.NFADC400read_DACOFF(nkusb, mid, i + 1); 

  // set gain to 1000(range = 0~4095)
  kadc.NFADC400write_DACGAIN(nkusb, mid, 0, 1000);
  for (i = 0; i < NCH; i++)
    head1.gain[i] =  kadc.NFADC400read_DACGAIN(nkusb, mid, i + 1); 

  // measure and show pedestal
  kadc.NFADC400measure_PED(nkusb, mid, 0);
  for (i = 1; i <= 4; i++)
    printf("pedestal @ch%d = %ld\n", i, kadc.NFADC400read_PED(nkusb, mid, i));
  for (i = 0; i < NCH; i++)
    head1.ped[i] =  kadc.NFADC400read_PED(nkusb, mid, i + 1); 

  // set input delay to 12000ns
  kadc.NFADC400write_DLY(nkusb, mid, 0, 1200);  
  for (i = 0; i < NCH; i++)
    head1.dly[i] =  kadc.NFADC400read_DLY(nkusb, mid, i + 1); 

  // select Trigger Lookup Table to be used
  // 0xFFFE for all ORed, 0x8000 for all ANDed
  kadc.NFADC400write_TLT(nkusb, mid, 0xFFFE);
  head1.tlt =  kadc.NFADC400read_TLT(nkusb, mid); 

  // set Trigger output(@ TRIG OUT connector) width to 1000ns
  kadc.NFADC400write_TOW(nkusb, mid, 1000);
  head1.tow =  kadc.NFADC400read_TOW(nkusb, mid); 

  // enable Trigger Daisy chain (use NFADC400disable_DCE if daisy chain is not used)
  kadc.NFADC400enable_DCE(nkusb, mid);
  head1.dce =  kadc.NFADC400read_DCE(nkusb, mid); 

  // set pulse polarity to negative(0 for negative, 1 for positive)
  kadc.NFADC400write_POL(nkusb, mid, 0, 0);  
  for (i = 0; i < NCH; i++)
    head1.pol[i] =  kadc.NFADC400read_POL(nkusb, mid, i + 0); 
            
  // set discriminator threshold to 10  --- 500mV -> THR = 315
  kadc.NFADC400write_THR(nkusb, mid, 0, 66); //66
  for (i = 0; i < NCH; i++)
    head1.thr[i] =  kadc.NFADC400read_THR(nkusb, mid, i + 1); 

  // set trigger deadtime to 0(in nano seconds)
  kadc.NFADC400write_DT(nkusb, mid, 0, 10000);
  for (i = 0; i < NCH; i++)
    head1.dt[i] =  kadc.NFADC400read_DT(nkusb, mid, i + 1); 

  // set coincidence width to 1000ns
  kadc.NFADC400write_CW(nkusb, mid, 0, 1000);
  for (i = 0; i < NCH; i++)
    head1.cw[i] =  kadc.NFADC400read_CW(nkusb, mid, i + 1); 

  // Trigger mode setting(device, mid, ch, enable pulse width, enable pulse count)
  kadc.NFADC400write_TM(nkusb, mid, 0, 1, 1);
  for (i = 0; i < NCH; i++)
    head1.tm[i] =  kadc.NFADC400read_TM(nkusb, mid, i + 1); 

  // set pulse count threshold to 1
  kadc.NFADC400write_PCT(nkusb, mid, 0, 1);
  for (i = 0; i < NCH; i++)
    head1.pct[i] =  kadc.NFADC400read_PCT(nkusb, mid, i + 1); 

  // set pulse count interval in nano seconds(if PCT=1, this setting is meaningless)
  kadc.NFADC400write_PCI(nkusb, mid, 0, 1000);
  for (i = 0; i < NCH; i++)
    head1.pci[i] =  kadc.NFADC400read_PCI(nkusb, mid, i + 1); 
  
  // set pulse width threshold to 200ns
  kadc.NFADC400write_PWT(nkusb, mid, 0, 200);
  for (i = 0; i < NCH; i++)
    head1.pwt[i] =  kadc.NFADC400read_PWT(nkusb, mid, i + 1); 

  head->Fill();

  cout << "START !@#!#!!!!!!! " << endl;
  // Start ADC DAQ ( Start buffer page 0 and 1 )
  kadc.NFADC400startL(nkusb, mid); 
  kadc.NFADC400startH(nkusb, mid);
      
  gSystem->Exec("date");

  Int_t bufnum =0;
  Int_t evt = 0;
  Int_t dpoint =0;
  Int_t flag = 1;
  
  while(flag) {
      
    Int_t IsFill=1;

    while(1) {

      if(bufnum == 0) 
      	IsFill = !(kadc.NFADC400read_RunL(nkusb, mid));
      else if(bufnum ==1)
      	IsFill = !(kadc.NFADC400read_RunH(nkusb, mid)); // Read event number, Flag =0 :all fill

      if(IsFill) break;
   }

    for(Int_t ii=0;ii<event_number;ii++){ 


      // reset histogram
      if (ii == 0) {
	hoscd1->Reset(); 
	hoscd2->Reset();
      }
      
      kadc.NFADC400read_TTIME(nkusb, mid, 1, bufnum * event_number + ii, tag);
      trigger = kadc.NFADC400read_TPattern(nkusb, mid, 1, bufnum * event_number + ii);
	
      //tag index   061121
      fadc1.trigger[0] = (trigger >> 8) & 0xFF; //trigger pattern
      fadc1.trigger[1] = trigger & 0xFF;
      
      for(Int_t kk =0;kk<6;kk++) 
	fadc1.tag[kk]=tag[kk] & 0xFF;

      for(Int_t kk =0;kk<NCH;kk++) {
	dpoint = kk*DATA_POINT;
	  
	kadc.NFADC400read_BUFFER(nkusb, mid, kk + 1, record_length, event_number*bufnum+ii ,data+dpoint);  //data is unsigned short

	for(int jj=0;jj<DATA_POINT;jj++){
	  fadc1.FADC[jj+dpoint]= float(data[jj+dpoint]&0x3FF);  //10bit

	  //only plot first events 
	  if(kk==0&&ii==0) hoscd1->Fill((jj+0.5)*bintons, fadc1.FADC[jj]);
	  if(kk==1&&ii==0) hoscd2->Fill((jj+0.5)*bintons, fadc1.FADC[jj+dpoint]);
	  //if(kk==2&&ii==0) hoscd3->Fill((jj+0.5)*bintons, fadc1.FADC[jj+dpoint]);
	}
      }

      fadc400->Fill();
      evt++;
      if(evt >= Nevent) {
	flag = 0;
	break;
      } 
    }

   // if(Form("evt%d\n",evt)%1280==0){ 
    c1->cd(1);
    hoscd1->Draw("HIST");
    c1->cd(2);
    hoscd2->Draw("HIST");
    c1->Modified();
    c1->Update();
    printf("evt number = %d\n",evt);
    gSystem->Exec("date");
   
    //}
    //getchar();

    if (flag) {
      if(bufnum == 0 ) { 
	bufnum = 1;
	kadc.NFADC400startL(nkusb, mid); 
      }
      else if(bufnum == 1) { 
	bufnum =0; 
	kadc.NFADC400startH(nkusb, mid);
      }
    }

    fadc400->AutoSave();
    FILE *fp = fopen("KILLME","r");
    if(fp) {
      fclose(fp);
      printf(" KILLME file exist  BYE \n");
      gSystem->Exec("rm -f KILLME");
      break;
    }
  }

  printf("finished!!\n");
  gSystem->Exec("date");
  hfile->Write();
 
  return 0;
}



