//#include "nkusbconst.h"
#define NK_SID_ANY 0xFF

void test() {
   const Short_t vendor_id = 0x04b4;
   const Short_t product_id = 0x00f0;
   const Int_t sid = 1;
   const Int_t interface = 0;

   gSystem->Load("libnkusb.so");
   gSystem->Load("libnk6uvme.so");
   gSystem->Load("libNK6UVMEROOT.so");

   NKVMEROOT *nkvme = new NKVMEROOT;

   nkvme->VMEOpen(vendor_id, product_id, sid);
   nkvme->VMEClaimInterface(vendor_id, product_id, sid, interface);
   nkvme->VMEPrintOpenDevices();

   UChar_t am = 0x09;
   UChar_t tout = 10;
   ULong_t address = 0xFF000000;
   //ULong_t data = 0x12345678;

   /*
   TBenchmark b;
   b.Start("b");

   for (Int_t i = 0; i < 100000; i++) {
      nkvme->VMEWrite(vendor_id, product_id, sid, am, tout, address, i); 
      //cout <<"i = " << i << endl;
   }

   b.Stop("b");
   b.Show("b");
   */
   //uncomment for vme read
   /*for (Int_t i = 0; i < 32; i++) {
      data = 1 << i;
      nkvme->VMEWrite(vendor_id, product_id, sid, am, tout, address, data); 
      ULong_t read_value = nkvme->VMERead(vendor_id, product_id, sid, am, tout, address); 
      getchar(); 
   }

return 0;
*/
   //uncomment for vme block read
   
   const char size_bulk = 1024;
   const Int_t nburst = 16;
   ULong_t nbytes = size_bulk * nburst;
   //   UChar_t *data = (UChar_t *)malloc(nbytes);
   
   //nbytes = 16*1024;
   //nbytes = 16384 - 100;
   nbytes = 32 * 1024;
   UChar_t data[1024 * 1024 * 512]; 

   TBenchmark b;
   b.Start("b");
   for (Int_t i = 0; i < 30000; i++) {
       nkvme->VMEBlockRead(vendor_id, product_id, sid, am, tout, address, nbytes, data);
   }
   b.Stop("b");
   b.Show("b");

   //   cout << "data = ";
   //   for (ULong_t i = 0; i < nbytes; i++) {
   //     cout << "%lu " << data[i];
   //  }
   //   cout << endl;
   //   free(data);
    
   nkvme->VMEReleaseInterface(vendor_id, product_id, sid, interface);
   nkvme->VMEClose(vendor_id, product_id, sid);

   return;
}
