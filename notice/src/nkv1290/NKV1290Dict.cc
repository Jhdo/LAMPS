// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME NKV1290Dict

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
#include "NKV1290.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TDCEvent_Dictionary();
   static void TDCEvent_TClassManip(TClass*);
   static void *new_TDCEvent(void *p = 0);
   static void *newArray_TDCEvent(Long_t size, void *p);
   static void delete_TDCEvent(void *p);
   static void deleteArray_TDCEvent(void *p);
   static void destruct_TDCEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TDCEvent*)
   {
      ::TDCEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TDCEvent));
      static ::ROOT::TGenericClassInfo 
         instance("TDCEvent", "NKV1290.h", 37,
                  typeid(::TDCEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TDCEvent_Dictionary, isa_proxy, 0,
                  sizeof(::TDCEvent) );
      instance.SetNew(&new_TDCEvent);
      instance.SetNewArray(&newArray_TDCEvent);
      instance.SetDelete(&delete_TDCEvent);
      instance.SetDeleteArray(&deleteArray_TDCEvent);
      instance.SetDestructor(&destruct_TDCEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TDCEvent*)
   {
      return GenerateInitInstanceLocal((::TDCEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TDCEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TDCEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TDCEvent*)0x0)->GetClass();
      TDCEvent_TClassManip(theClass);
   return theClass;
   }

   static void TDCEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NKV1290(void *p = 0);
   static void *newArray_NKV1290(Long_t size, void *p);
   static void delete_NKV1290(void *p);
   static void deleteArray_NKV1290(void *p);
   static void destruct_NKV1290(void *p);
   static void streamer_NKV1290(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NKV1290*)
   {
      ::NKV1290 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NKV1290 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NKV1290", ::NKV1290::Class_Version(), "NKV1290.h", 49,
                  typeid(::NKV1290), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NKV1290::Dictionary, isa_proxy, 16,
                  sizeof(::NKV1290) );
      instance.SetNew(&new_NKV1290);
      instance.SetNewArray(&newArray_NKV1290);
      instance.SetDelete(&delete_NKV1290);
      instance.SetDeleteArray(&deleteArray_NKV1290);
      instance.SetDestructor(&destruct_NKV1290);
      instance.SetStreamerFunc(&streamer_NKV1290);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NKV1290*)
   {
      return GenerateInitInstanceLocal((::NKV1290*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NKV1290*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NKV1290::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NKV1290::Class_Name()
{
   return "NKV1290";
}

//______________________________________________________________________________
const char *NKV1290::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NKV1290*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NKV1290::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NKV1290*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NKV1290::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NKV1290*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NKV1290::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NKV1290*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TDCEvent(void *p) {
      return  p ? new(p) ::TDCEvent : new ::TDCEvent;
   }
   static void *newArray_TDCEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TDCEvent[nElements] : new ::TDCEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TDCEvent(void *p) {
      delete ((::TDCEvent*)p);
   }
   static void deleteArray_TDCEvent(void *p) {
      delete [] ((::TDCEvent*)p);
   }
   static void destruct_TDCEvent(void *p) {
      typedef ::TDCEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TDCEvent

//______________________________________________________________________________
void NKV1290::Streamer(TBuffer &R__b)
{
   // Stream an object of class NKV1290.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      NK6UVMEROOT::Streamer(R__b);
      R__b >> fDebug;
      R__b.CheckByteCount(R__s, R__c, NKV1290::IsA());
   } else {
      R__c = R__b.WriteVersion(NKV1290::IsA(), kTRUE);
      NK6UVMEROOT::Streamer(R__b);
      R__b << fDebug;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NKV1290(void *p) {
      return  p ? new(p) ::NKV1290 : new ::NKV1290;
   }
   static void *newArray_NKV1290(Long_t nElements, void *p) {
      return p ? new(p) ::NKV1290[nElements] : new ::NKV1290[nElements];
   }
   // Wrapper around operator delete
   static void delete_NKV1290(void *p) {
      delete ((::NKV1290*)p);
   }
   static void deleteArray_NKV1290(void *p) {
      delete [] ((::NKV1290*)p);
   }
   static void destruct_NKV1290(void *p) {
      typedef ::NKV1290 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_NKV1290(TBuffer &buf, void *obj) {
      ((::NKV1290*)obj)->::NKV1290::Streamer(buf);
   }
} // end of namespace ROOT for class ::NKV1290

namespace {
  void TriggerDictionaryInitialization_NKV1290Dict_Impl() {
    static const char* headers[] = {
"NKV1290.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/notice/include",
"/home/hj0521/root/include",
"/usr/local/notice/src/nkv1290/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "NKV1290Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$NKV1290.h")))  TDCEvent;
class __attribute__((annotate("$clingAutoload$NKV1290.h")))  NKV1290;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "NKV1290Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "NKV1290.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"NKV1290", payloadCode, "@",
"TDCEvent", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("NKV1290Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_NKV1290Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_NKV1290Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_NKV1290Dict() {
  TriggerDictionaryInitialization_NKV1290Dict_Impl();
}
