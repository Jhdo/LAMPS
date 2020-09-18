// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME NKNFADC400Dict

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
#include "NKNFADC400.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_NKNFADC400(void *p = 0);
   static void *newArray_NKNFADC400(Long_t size, void *p);
   static void delete_NKNFADC400(void *p);
   static void deleteArray_NKNFADC400(void *p);
   static void destruct_NKNFADC400(void *p);
   static void streamer_NKNFADC400(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NKNFADC400*)
   {
      ::NKNFADC400 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NKNFADC400 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NKNFADC400", ::NKNFADC400::Class_Version(), "NKNFADC400.h", 49,
                  typeid(::NKNFADC400), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NKNFADC400::Dictionary, isa_proxy, 16,
                  sizeof(::NKNFADC400) );
      instance.SetNew(&new_NKNFADC400);
      instance.SetNewArray(&newArray_NKNFADC400);
      instance.SetDelete(&delete_NKNFADC400);
      instance.SetDeleteArray(&deleteArray_NKNFADC400);
      instance.SetDestructor(&destruct_NKNFADC400);
      instance.SetStreamerFunc(&streamer_NKNFADC400);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NKNFADC400*)
   {
      return GenerateInitInstanceLocal((::NKNFADC400*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NKNFADC400*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NKNFADC400::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NKNFADC400::Class_Name()
{
   return "NKNFADC400";
}

//______________________________________________________________________________
const char *NKNFADC400::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NKNFADC400*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NKNFADC400::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NKNFADC400*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NKNFADC400::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NKNFADC400*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NKNFADC400::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NKNFADC400*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NKNFADC400::Streamer(TBuffer &R__b)
{
   // Stream an object of class NKNFADC400.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      NK6UVME::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, NKNFADC400::IsA());
   } else {
      R__c = R__b.WriteVersion(NKNFADC400::IsA(), kTRUE);
      NK6UVME::Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NKNFADC400(void *p) {
      return  p ? new(p) ::NKNFADC400 : new ::NKNFADC400;
   }
   static void *newArray_NKNFADC400(Long_t nElements, void *p) {
      return p ? new(p) ::NKNFADC400[nElements] : new ::NKNFADC400[nElements];
   }
   // Wrapper around operator delete
   static void delete_NKNFADC400(void *p) {
      delete ((::NKNFADC400*)p);
   }
   static void deleteArray_NKNFADC400(void *p) {
      delete [] ((::NKNFADC400*)p);
   }
   static void destruct_NKNFADC400(void *p) {
      typedef ::NKNFADC400 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_NKNFADC400(TBuffer &buf, void *obj) {
      ((::NKNFADC400*)obj)->::NKNFADC400::Streamer(buf);
   }
} // end of namespace ROOT for class ::NKNFADC400

namespace {
  void TriggerDictionaryInitialization_NKNFADC400Dict_Impl() {
    static const char* headers[] = {
"NKNFADC400.h",
0
    };
    static const char* includePaths[] = {
"/home/n-ext/ROOT-v6.14.04/object/include",
"/home/n-ext/LAMPS/notice/src/nfadc400/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "NKNFADC400Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$NKNFADC400.h")))  NKNFADC400;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "NKNFADC400Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "NKNFADC400.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"NKNFADC400", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("NKNFADC400Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_NKNFADC400Dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_NKNFADC400Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_NKNFADC400Dict() {
  TriggerDictionaryInitialization_NKNFADC400Dict_Impl();
}
