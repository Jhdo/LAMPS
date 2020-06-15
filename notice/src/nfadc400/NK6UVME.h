# 1 "Notice6UVME.h"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "Notice6UVME.h"



# 1 "//usr/local/notice/include/NKUSBVME_Linux.h" 1
# 23 "//usr/local/notice/include/NKUSBVME_Linux.h"
int uopen(int devnum);
int uclose(int devnum);
int uwrite(int devnum, char *buffer, unsigned long length);
int uread(int devnum, char *buffer, unsigned long length);
# 5 "Notice6UVME.h" 2
# 15 "Notice6UVME.h"
class NK6UVME {
 public:
           NK6UVME();
  virtual ~NK6UVME();


int VMEopen(int devnum);

int VMEclose(int devnum);

int VMEwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);

unsigned long VMEread(int devnum, unsigned short am, unsigned short tout, unsigned long address);

int VMEblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);

int Vwrite(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long data);

int Vread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long *data);

int Vblockread(int devnum, unsigned short am, unsigned short tout, unsigned long address, unsigned long counts, char *data);


  ClassDef(NK6UVME,2)


     };
