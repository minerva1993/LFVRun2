// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIrootdict
#define R__NO_DEPRECATION

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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "src/NanoAODAnalyzerrdframe.h"
#include "src/TopLFVAnalyzer.h"
#include "src/SkimEvents.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *NanoAODAnalyzerrdframe_Dictionary();
   static void NanoAODAnalyzerrdframe_TClassManip(TClass*);
   static void delete_NanoAODAnalyzerrdframe(void *p);
   static void deleteArray_NanoAODAnalyzerrdframe(void *p);
   static void destruct_NanoAODAnalyzerrdframe(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NanoAODAnalyzerrdframe*)
   {
      ::NanoAODAnalyzerrdframe *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NanoAODAnalyzerrdframe));
      static ::ROOT::TGenericClassInfo 
         instance("NanoAODAnalyzerrdframe", "src/NanoAODAnalyzerrdframe.h", 34,
                  typeid(::NanoAODAnalyzerrdframe), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NanoAODAnalyzerrdframe_Dictionary, isa_proxy, 4,
                  sizeof(::NanoAODAnalyzerrdframe) );
      instance.SetDelete(&delete_NanoAODAnalyzerrdframe);
      instance.SetDeleteArray(&deleteArray_NanoAODAnalyzerrdframe);
      instance.SetDestructor(&destruct_NanoAODAnalyzerrdframe);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NanoAODAnalyzerrdframe*)
   {
      return GenerateInitInstanceLocal((::NanoAODAnalyzerrdframe*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NanoAODAnalyzerrdframe*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NanoAODAnalyzerrdframe_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NanoAODAnalyzerrdframe*)0x0)->GetClass();
      NanoAODAnalyzerrdframe_TClassManip(theClass);
   return theClass;
   }

   static void NanoAODAnalyzerrdframe_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TopLFVAnalyzer_Dictionary();
   static void TopLFVAnalyzer_TClassManip(TClass*);
   static void delete_TopLFVAnalyzer(void *p);
   static void deleteArray_TopLFVAnalyzer(void *p);
   static void destruct_TopLFVAnalyzer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TopLFVAnalyzer*)
   {
      ::TopLFVAnalyzer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TopLFVAnalyzer));
      static ::ROOT::TGenericClassInfo 
         instance("TopLFVAnalyzer", "src/TopLFVAnalyzer.h", 13,
                  typeid(::TopLFVAnalyzer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TopLFVAnalyzer_Dictionary, isa_proxy, 4,
                  sizeof(::TopLFVAnalyzer) );
      instance.SetDelete(&delete_TopLFVAnalyzer);
      instance.SetDeleteArray(&deleteArray_TopLFVAnalyzer);
      instance.SetDestructor(&destruct_TopLFVAnalyzer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TopLFVAnalyzer*)
   {
      return GenerateInitInstanceLocal((::TopLFVAnalyzer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TopLFVAnalyzer*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TopLFVAnalyzer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TopLFVAnalyzer*)0x0)->GetClass();
      TopLFVAnalyzer_TClassManip(theClass);
   return theClass;
   }

   static void TopLFVAnalyzer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SkimEvents_Dictionary();
   static void SkimEvents_TClassManip(TClass*);
   static void delete_SkimEvents(void *p);
   static void deleteArray_SkimEvents(void *p);
   static void destruct_SkimEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SkimEvents*)
   {
      ::SkimEvents *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SkimEvents));
      static ::ROOT::TGenericClassInfo 
         instance("SkimEvents", "src/SkimEvents.h", 14,
                  typeid(::SkimEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &SkimEvents_Dictionary, isa_proxy, 4,
                  sizeof(::SkimEvents) );
      instance.SetDelete(&delete_SkimEvents);
      instance.SetDeleteArray(&deleteArray_SkimEvents);
      instance.SetDestructor(&destruct_SkimEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SkimEvents*)
   {
      return GenerateInitInstanceLocal((::SkimEvents*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SkimEvents*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SkimEvents_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SkimEvents*)0x0)->GetClass();
      SkimEvents_TClassManip(theClass);
   return theClass;
   }

   static void SkimEvents_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NanoAODAnalyzerrdframe(void *p) {
      delete ((::NanoAODAnalyzerrdframe*)p);
   }
   static void deleteArray_NanoAODAnalyzerrdframe(void *p) {
      delete [] ((::NanoAODAnalyzerrdframe*)p);
   }
   static void destruct_NanoAODAnalyzerrdframe(void *p) {
      typedef ::NanoAODAnalyzerrdframe current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NanoAODAnalyzerrdframe

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TopLFVAnalyzer(void *p) {
      delete ((::TopLFVAnalyzer*)p);
   }
   static void deleteArray_TopLFVAnalyzer(void *p) {
      delete [] ((::TopLFVAnalyzer*)p);
   }
   static void destruct_TopLFVAnalyzer(void *p) {
      typedef ::TopLFVAnalyzer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TopLFVAnalyzer

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SkimEvents(void *p) {
      delete ((::SkimEvents*)p);
   }
   static void deleteArray_SkimEvents(void *p) {
      delete [] ((::SkimEvents*)p);
   }
   static void destruct_SkimEvents(void *p) {
      typedef ::SkimEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SkimEvents

namespace ROOT {
   static TClass *vectorlEvarinfogR_Dictionary();
   static void vectorlEvarinfogR_TClassManip(TClass*);
   static void *new_vectorlEvarinfogR(void *p = 0);
   static void *newArray_vectorlEvarinfogR(Long_t size, void *p);
   static void delete_vectorlEvarinfogR(void *p);
   static void deleteArray_vectorlEvarinfogR(void *p);
   static void destruct_vectorlEvarinfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<varinfo>*)
   {
      vector<varinfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<varinfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<varinfo>", -2, "vector", 210,
                  typeid(vector<varinfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvarinfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<varinfo>) );
      instance.SetNew(&new_vectorlEvarinfogR);
      instance.SetNewArray(&newArray_vectorlEvarinfogR);
      instance.SetDelete(&delete_vectorlEvarinfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEvarinfogR);
      instance.SetDestructor(&destruct_vectorlEvarinfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<varinfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<varinfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvarinfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<varinfo>*)0x0)->GetClass();
      vectorlEvarinfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvarinfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvarinfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<varinfo> : new vector<varinfo>;
   }
   static void *newArray_vectorlEvarinfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<varinfo>[nElements] : new vector<varinfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvarinfogR(void *p) {
      delete ((vector<varinfo>*)p);
   }
   static void deleteArray_vectorlEvarinfogR(void *p) {
      delete [] ((vector<varinfo>*)p);
   }
   static void destruct_vectorlEvarinfogR(void *p) {
      typedef vector<varinfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<varinfo>

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 210,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEhist1dinfogR_Dictionary();
   static void vectorlEhist1dinfogR_TClassManip(TClass*);
   static void *new_vectorlEhist1dinfogR(void *p = 0);
   static void *newArray_vectorlEhist1dinfogR(Long_t size, void *p);
   static void delete_vectorlEhist1dinfogR(void *p);
   static void deleteArray_vectorlEhist1dinfogR(void *p);
   static void destruct_vectorlEhist1dinfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<hist1dinfo>*)
   {
      vector<hist1dinfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<hist1dinfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<hist1dinfo>", -2, "vector", 210,
                  typeid(vector<hist1dinfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEhist1dinfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<hist1dinfo>) );
      instance.SetNew(&new_vectorlEhist1dinfogR);
      instance.SetNewArray(&newArray_vectorlEhist1dinfogR);
      instance.SetDelete(&delete_vectorlEhist1dinfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEhist1dinfogR);
      instance.SetDestructor(&destruct_vectorlEhist1dinfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<hist1dinfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<hist1dinfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEhist1dinfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<hist1dinfo>*)0x0)->GetClass();
      vectorlEhist1dinfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEhist1dinfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEhist1dinfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<hist1dinfo> : new vector<hist1dinfo>;
   }
   static void *newArray_vectorlEhist1dinfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<hist1dinfo>[nElements] : new vector<hist1dinfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEhist1dinfogR(void *p) {
      delete ((vector<hist1dinfo>*)p);
   }
   static void deleteArray_vectorlEhist1dinfogR(void *p) {
      delete [] ((vector<hist1dinfo>*)p);
   }
   static void destruct_vectorlEhist1dinfogR(void *p) {
      typedef vector<hist1dinfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<hist1dinfo>

namespace ROOT {
   static TClass *vectorlEcutinfogR_Dictionary();
   static void vectorlEcutinfogR_TClassManip(TClass*);
   static void *new_vectorlEcutinfogR(void *p = 0);
   static void *newArray_vectorlEcutinfogR(Long_t size, void *p);
   static void delete_vectorlEcutinfogR(void *p);
   static void deleteArray_vectorlEcutinfogR(void *p);
   static void destruct_vectorlEcutinfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<cutinfo>*)
   {
      vector<cutinfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<cutinfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<cutinfo>", -2, "vector", 210,
                  typeid(vector<cutinfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEcutinfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<cutinfo>) );
      instance.SetNew(&new_vectorlEcutinfogR);
      instance.SetNewArray(&newArray_vectorlEcutinfogR);
      instance.SetDelete(&delete_vectorlEcutinfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEcutinfogR);
      instance.SetDestructor(&destruct_vectorlEcutinfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<cutinfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<cutinfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEcutinfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<cutinfo>*)0x0)->GetClass();
      vectorlEcutinfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEcutinfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEcutinfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cutinfo> : new vector<cutinfo>;
   }
   static void *newArray_vectorlEcutinfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cutinfo>[nElements] : new vector<cutinfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEcutinfogR(void *p) {
      delete ((vector<cutinfo>*)p);
   }
   static void deleteArray_vectorlEcutinfogR(void *p) {
      delete [] ((vector<cutinfo>*)p);
   }
   static void destruct_vectorlEcutinfogR(void *p) {
      typedef vector<cutinfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<cutinfo>

namespace ROOT {
   static TClass *vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_Dictionary();
   static void vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p = 0);
   static void *newArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p);
   static void deleteArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p);
   static void destruct_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >*)
   {
      vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >", -2, "vector", 210,
                  typeid(vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >) );
      instance.SetNew(&new_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >*)0x0)->GetClass();
      vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > : new vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >;
   }
   static void *newArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >[nElements] : new vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p) {
      delete ((vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >*)p);
   }
   static void deleteArray_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p) {
      delete [] ((vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >*)p);
   }
   static void destruct_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgRsPgR(void *p) {
      typedef vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >

namespace ROOT {
   static TClass *maplEstringcOvectorlEstringgRsPgR_Dictionary();
   static void maplEstringcOvectorlEstringgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEstringgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEstringgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEstringgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEstringgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEstringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<string> >*)
   {
      map<string,vector<string> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<string> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<string> >", -2, "map", 96,
                  typeid(map<string,vector<string> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEstringgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<string> >) );
      instance.SetNew(&new_maplEstringcOvectorlEstringgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEstringgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<string> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<string> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEstringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<string> >*)0x0)->GetClass();
      maplEstringcOvectorlEstringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEstringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEstringgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<string> > : new map<string,vector<string> >;
   }
   static void *newArray_maplEstringcOvectorlEstringgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<string> >[nElements] : new map<string,vector<string> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEstringgRsPgR(void *p) {
      delete ((map<string,vector<string> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEstringgRsPgR(void *p) {
      delete [] ((map<string,vector<string> >*)p);
   }
   static void destruct_maplEstringcOvectorlEstringgRsPgR(void *p) {
      typedef map<string,vector<string> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<string> >

namespace ROOT {
   static TClass *maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_Dictionary();
   static void maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p);
   static void deleteArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p);
   static void destruct_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,ROOT::RDF::RResultPtr<TH1D> >*)
   {
      map<string,ROOT::RDF::RResultPtr<TH1D> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,ROOT::RDF::RResultPtr<TH1D> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,ROOT::RDF::RResultPtr<TH1D> >", -2, "map", 96,
                  typeid(map<string,ROOT::RDF::RResultPtr<TH1D> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,ROOT::RDF::RResultPtr<TH1D> >) );
      instance.SetNew(&new_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR);
      instance.SetDelete(&delete_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,ROOT::RDF::RResultPtr<TH1D> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,ROOT::RDF::RResultPtr<TH1D> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,ROOT::RDF::RResultPtr<TH1D> >*)0x0)->GetClass();
      maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,ROOT::RDF::RResultPtr<TH1D> > : new map<string,ROOT::RDF::RResultPtr<TH1D> >;
   }
   static void *newArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,ROOT::RDF::RResultPtr<TH1D> >[nElements] : new map<string,ROOT::RDF::RResultPtr<TH1D> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p) {
      delete ((map<string,ROOT::RDF::RResultPtr<TH1D> >*)p);
   }
   static void deleteArray_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p) {
      delete [] ((map<string,ROOT::RDF::RResultPtr<TH1D> >*)p);
   }
   static void destruct_maplEstringcOROOTcLcLRDFcLcLRResultPtrlETH1DgRsPgR(void *p) {
      typedef map<string,ROOT::RDF::RResultPtr<TH1D> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,ROOT::RDF::RResultPtr<TH1D> >

namespace {
  void TriggerDictionaryInitialization_rootdict_Impl() {
    static const char* headers[] = {
"src/NanoAODAnalyzerrdframe.h",
"src/TopLFVAnalyzer.h",
"src/SkimEvents.h",
0
    };
    static const char* includePaths[] = {
"/opt/ohpc/pub/root/root_v6.20.04/include",
"/opt/ohpc/pub/root/root_v6.20.04/include/",
"/home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "rootdict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace ROOT{namespace Math{template <typename T> class __attribute__((annotate("$clingAutoload$Math/Vector4Dfwd.h")))  __attribute__((annotate("$clingAutoload$src/NanoAODAnalyzerrdframe.h")))  PtEtaPhiM4D;
}}
namespace ROOT{namespace Math{template <class CoordSystem> class __attribute__((annotate("$clingAutoload$Math/Vector4Dfwd.h")))  __attribute__((annotate("$clingAutoload$src/NanoAODAnalyzerrdframe.h")))  LorentzVector;
}}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$src/NanoAODAnalyzerrdframe.h")))  NanoAODAnalyzerrdframe;
class __attribute__((annotate("$clingAutoload$src/TopLFVAnalyzer.h")))  TopLFVAnalyzer;
class __attribute__((annotate("$clingAutoload$src/SkimEvents.h")))  SkimEvents;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "rootdict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/NanoAODAnalyzerrdframe.h"
#include "src/TopLFVAnalyzer.h"
#include "src/SkimEvents.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NanoAODAnalyzerrdframe", payloadCode, "@",
"SkimEvents", payloadCode, "@",
"TopLFVAnalyzer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("rootdict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_rootdict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_rootdict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_rootdict() {
  TriggerDictionaryInitialization_rootdict_Impl();
}
