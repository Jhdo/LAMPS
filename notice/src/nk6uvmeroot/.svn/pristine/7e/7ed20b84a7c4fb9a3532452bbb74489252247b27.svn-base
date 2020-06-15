/*
 *
 */
#include "libusb.h"
#include "nkusb.h"
#include "nk6uvme.h"
#include "NK6UVMEROOT.h"

ClassImp(NK6UVMEROOT)


///////////////////////////////////////////////////////////////////////////////
NK6UVMEROOT::NK6UVMEROOT(libusb_context **ctx)
:NKUSBROOT(ctx)
{
}

///////////////////////////////////////////////////////////////////////////////
NK6UVMEROOT::~NK6UVMEROOT()
{
}

///////////////////////////////////////////////////////////////////////////////
Int_t NK6UVMEROOT::VMEopen(Int_t sid, libusb_context *ctx)
{
   return ::VMEopen(sid, ctx);
}

///////////////////////////////////////////////////////////////////////////////
void NK6UVMEROOT::VMEclose(Int_t sid)
{
   ::VMEclose(sid);
}

///////////////////////////////////////////////////////////////////////////////
Int_t NK6UVMEROOT::VMEwrite(Int_t sid, UChar_t am, UChar_t tout, ULong_t address, ULong_t data)
{
   return ::VMEwrite(sid, am, tout, address, data);
}

///////////////////////////////////////////////////////////////////////////////
ULong_t NK6UVMEROOT::VMEread(Int_t sid, UChar_t am, UChar_t tout, ULong_t address)
{
   return ::VMEread(sid, am, tout, address);
}

///////////////////////////////////////////////////////////////////////////////
Int_t NK6UVMEROOT::VMEblockread(Int_t sid, UChar_t am, UChar_t tout,
                    ULong_t address, ULong_t nbytes, UChar_t *data)
{
   return ::VMEblockread(sid, am, tout, address, nbytes, data);
}
