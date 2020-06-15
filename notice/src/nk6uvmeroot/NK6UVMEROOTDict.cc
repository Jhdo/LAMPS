// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME NK6UVMEROOTDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "NK6UVMEROOT.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_NK6UVMEROOT(void *p = 0);
   static void *newArray_NK6UVMEROOT(Long_t size, void *p);
   static void delete_NK6UVMEROOT(void *p);
   static void deleteArray_NK6UVMEROOT(void *p);
   static void destruct_NK6UVMEROOT(void *p);
   static void streamer_NK6UVMEROOT(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NK6UVMEROOT*)
   {
      ::NK6UVMEROOT *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NK6UVMEROOT >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NK6UVMEROOT", ::NK6UVMEROOT::Class_Version(), "NK6UVMEROOT.h", 13,
                  typeid(::NK6UVMEROOT), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NK6UVMEROOT::Dictionary, isa_proxy, 18,
                  sizeof(::NK6UVMEROOT) );
      instance.SetNew(&new_NK6UVMEROOT);
      instance.SetNewArray(&newArray_NK6UVMEROOT);
      instance.SetDelete(&delete_NK6UVMEROOT);
      instance.SetDeleteArray(&deleteArray_NK6UVMEROOT);
      instance.SetDestructor(&destruct_NK6UVMEROOT);
      instance.SetStreamerFunc(&streamer_NK6UVMEROOT);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NK6UVMEROOT*)
   {
      return GenerateInitInstanceLocal((::NK6UVMEROOT*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NK6UVMEROOT*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NK6UVMEROOT::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NK6UVMEROOT::Class_Name()
{
   return "NK6UVMEROOT";
}

//______________________________________________________________________________
const char *NK6UVMEROOT::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NK6UVMEROOT*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NK6UVMEROOT::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NK6UVMEROOT*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NK6UVMEROOT::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NK6UVMEROOT*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NK6UVMEROOT::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NK6UVMEROOT*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NK6UVMEROOT::Streamer(TBuffer &R__b)
{
   // Stream an object of class NK6UVMEROOT.

   NKUSBROOT::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NK6UVMEROOT(void *p) {
      return  p ? new(p) ::NK6UVMEROOT : new ::NK6UVMEROOT;
   }
   static void *newArray_NK6UVMEROOT(Long_t nElements, void *p) {
      return p ? new(p) ::NK6UVMEROOT[nElements] : new ::NK6UVMEROOT[nElements];
   }
   // Wrapper around operator delete
   static void delete_NK6UVMEROOT(void *p) {
      delete ((::NK6UVMEROOT*)p);
   }
   static void deleteArray_NK6UVMEROOT(void *p) {
      delete [] ((::NK6UVMEROOT*)p);
   }
   static void destruct_NK6UVMEROOT(void *p) {
      typedef ::NK6UVMEROOT current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_NK6UVMEROOT(TBuffer &buf, void *obj) {
      ((::NK6UVMEROOT*)obj)->::NK6UVMEROOT::Streamer(buf);
   }
} // end of namespace ROOT for class ::NK6UVMEROOT

namespace {
  void TriggerDictionaryInitialization_NK6UVMEROOTDict_Impl() {
    static const char* headers[] = {
"NK6UVMEROOT.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/notice/include",
"/home/hj0521/root/include",
"/usr/local/notice/src/nk6uvmeroot/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "NK6UVMEROOTDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(nkvme wrapper class for root)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$NK6UVMEROOT.h")))  NK6UVMEROOT;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "NK6UVMEROOTDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "NK6UVMEROOT.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"NK6UVMEROOT", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("NK6UVMEROOTDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_NK6UVMEROOTDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_NK6UVMEROOTDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_NK6UVMEROOTDict() {
  TriggerDictionaryInitialization_NK6UVMEROOTDict_Impl();
}
