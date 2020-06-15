#include "Notice6UVME.h"

#ifndef NKC
class NKDISPLAY : public NK6UVME {
 public:
           NKDISPLAY();
  virtual ~NKDISPLAY();
#endif
void DISPwriteData(int devnum, unsigned long mid, unsigned long data);
unsigned long DISPreadData(int devnum, unsigned long mid);
void DISPsetTimeout(int devnum, unsigned long mid, unsigned long data);
unsigned long DISPreadTimeout(int devnum, unsigned long mid);
#ifdef NKROOT
  ClassDef(NKDISPLAY,2)
#endif
#ifndef NKC
     };
#endif
