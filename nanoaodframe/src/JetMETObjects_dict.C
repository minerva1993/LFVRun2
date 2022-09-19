// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIJetMETObjects_dict
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
#include "src/JetCorrectorParameters.h"
#include "src/SimpleJetCorrector.h"
#include "src/FactorizedJetCorrector.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *JetCorrectorParameters_Dictionary();
   static void JetCorrectorParameters_TClassManip(TClass*);
   static void *new_JetCorrectorParameters(void *p = 0);
   static void *newArray_JetCorrectorParameters(Long_t size, void *p);
   static void delete_JetCorrectorParameters(void *p);
   static void deleteArray_JetCorrectorParameters(void *p);
   static void destruct_JetCorrectorParameters(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetCorrectorParameters*)
   {
      ::JetCorrectorParameters *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetCorrectorParameters));
      static ::ROOT::TGenericClassInfo 
         instance("JetCorrectorParameters", "src/JetCorrectorParameters.h", 16,
                  typeid(::JetCorrectorParameters), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JetCorrectorParameters_Dictionary, isa_proxy, 4,
                  sizeof(::JetCorrectorParameters) );
      instance.SetNew(&new_JetCorrectorParameters);
      instance.SetNewArray(&newArray_JetCorrectorParameters);
      instance.SetDelete(&delete_JetCorrectorParameters);
      instance.SetDeleteArray(&deleteArray_JetCorrectorParameters);
      instance.SetDestructor(&destruct_JetCorrectorParameters);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetCorrectorParameters*)
   {
      return GenerateInitInstanceLocal((::JetCorrectorParameters*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JetCorrectorParameters*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetCorrectorParameters_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetCorrectorParameters*)0x0)->GetClass();
      JetCorrectorParameters_TClassManip(theClass);
   return theClass;
   }

   static void JetCorrectorParameters_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JetCorrectorParameterscLcLDefinitions_Dictionary();
   static void JetCorrectorParameterscLcLDefinitions_TClassManip(TClass*);
   static void *new_JetCorrectorParameterscLcLDefinitions(void *p = 0);
   static void *newArray_JetCorrectorParameterscLcLDefinitions(Long_t size, void *p);
   static void delete_JetCorrectorParameterscLcLDefinitions(void *p);
   static void deleteArray_JetCorrectorParameterscLcLDefinitions(void *p);
   static void destruct_JetCorrectorParameterscLcLDefinitions(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetCorrectorParameters::Definitions*)
   {
      ::JetCorrectorParameters::Definitions *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetCorrectorParameters::Definitions));
      static ::ROOT::TGenericClassInfo 
         instance("JetCorrectorParameters::Definitions", "src/JetCorrectorParameters.h", 23,
                  typeid(::JetCorrectorParameters::Definitions), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JetCorrectorParameterscLcLDefinitions_Dictionary, isa_proxy, 4,
                  sizeof(::JetCorrectorParameters::Definitions) );
      instance.SetNew(&new_JetCorrectorParameterscLcLDefinitions);
      instance.SetNewArray(&newArray_JetCorrectorParameterscLcLDefinitions);
      instance.SetDelete(&delete_JetCorrectorParameterscLcLDefinitions);
      instance.SetDeleteArray(&deleteArray_JetCorrectorParameterscLcLDefinitions);
      instance.SetDestructor(&destruct_JetCorrectorParameterscLcLDefinitions);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetCorrectorParameters::Definitions*)
   {
      return GenerateInitInstanceLocal((::JetCorrectorParameters::Definitions*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JetCorrectorParameters::Definitions*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetCorrectorParameterscLcLDefinitions_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetCorrectorParameters::Definitions*)0x0)->GetClass();
      JetCorrectorParameterscLcLDefinitions_TClassManip(theClass);
   return theClass;
   }

   static void JetCorrectorParameterscLcLDefinitions_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JetCorrectorParameterscLcLRecord_Dictionary();
   static void JetCorrectorParameterscLcLRecord_TClassManip(TClass*);
   static void *new_JetCorrectorParameterscLcLRecord(void *p = 0);
   static void *newArray_JetCorrectorParameterscLcLRecord(Long_t size, void *p);
   static void delete_JetCorrectorParameterscLcLRecord(void *p);
   static void deleteArray_JetCorrectorParameterscLcLRecord(void *p);
   static void destruct_JetCorrectorParameterscLcLRecord(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetCorrectorParameters::Record*)
   {
      ::JetCorrectorParameters::Record *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetCorrectorParameters::Record));
      static ::ROOT::TGenericClassInfo 
         instance("JetCorrectorParameters::Record", "src/JetCorrectorParameters.h", 50,
                  typeid(::JetCorrectorParameters::Record), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JetCorrectorParameterscLcLRecord_Dictionary, isa_proxy, 4,
                  sizeof(::JetCorrectorParameters::Record) );
      instance.SetNew(&new_JetCorrectorParameterscLcLRecord);
      instance.SetNewArray(&newArray_JetCorrectorParameterscLcLRecord);
      instance.SetDelete(&delete_JetCorrectorParameterscLcLRecord);
      instance.SetDeleteArray(&deleteArray_JetCorrectorParameterscLcLRecord);
      instance.SetDestructor(&destruct_JetCorrectorParameterscLcLRecord);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetCorrectorParameters::Record*)
   {
      return GenerateInitInstanceLocal((::JetCorrectorParameters::Record*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JetCorrectorParameters::Record*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetCorrectorParameterscLcLRecord_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetCorrectorParameters::Record*)0x0)->GetClass();
      JetCorrectorParameterscLcLRecord_TClassManip(theClass);
   return theClass;
   }

   static void JetCorrectorParameterscLcLRecord_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SimpleJetCorrector_Dictionary();
   static void SimpleJetCorrector_TClassManip(TClass*);
   static void *new_SimpleJetCorrector(void *p = 0);
   static void *newArray_SimpleJetCorrector(Long_t size, void *p);
   static void delete_SimpleJetCorrector(void *p);
   static void deleteArray_SimpleJetCorrector(void *p);
   static void destruct_SimpleJetCorrector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SimpleJetCorrector*)
   {
      ::SimpleJetCorrector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SimpleJetCorrector));
      static ::ROOT::TGenericClassInfo 
         instance("SimpleJetCorrector", "src/SimpleJetCorrector.h", 12,
                  typeid(::SimpleJetCorrector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &SimpleJetCorrector_Dictionary, isa_proxy, 4,
                  sizeof(::SimpleJetCorrector) );
      instance.SetNew(&new_SimpleJetCorrector);
      instance.SetNewArray(&newArray_SimpleJetCorrector);
      instance.SetDelete(&delete_SimpleJetCorrector);
      instance.SetDeleteArray(&deleteArray_SimpleJetCorrector);
      instance.SetDestructor(&destruct_SimpleJetCorrector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SimpleJetCorrector*)
   {
      return GenerateInitInstanceLocal((::SimpleJetCorrector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SimpleJetCorrector*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SimpleJetCorrector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SimpleJetCorrector*)0x0)->GetClass();
      SimpleJetCorrector_TClassManip(theClass);
   return theClass;
   }

   static void SimpleJetCorrector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FactorizedJetCorrector_Dictionary();
   static void FactorizedJetCorrector_TClassManip(TClass*);
   static void *new_FactorizedJetCorrector(void *p = 0);
   static void *newArray_FactorizedJetCorrector(Long_t size, void *p);
   static void delete_FactorizedJetCorrector(void *p);
   static void deleteArray_FactorizedJetCorrector(void *p);
   static void destruct_FactorizedJetCorrector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FactorizedJetCorrector*)
   {
      ::FactorizedJetCorrector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FactorizedJetCorrector));
      static ::ROOT::TGenericClassInfo 
         instance("FactorizedJetCorrector", "src/FactorizedJetCorrector.h", 16,
                  typeid(::FactorizedJetCorrector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &FactorizedJetCorrector_Dictionary, isa_proxy, 4,
                  sizeof(::FactorizedJetCorrector) );
      instance.SetNew(&new_FactorizedJetCorrector);
      instance.SetNewArray(&newArray_FactorizedJetCorrector);
      instance.SetDelete(&delete_FactorizedJetCorrector);
      instance.SetDeleteArray(&deleteArray_FactorizedJetCorrector);
      instance.SetDestructor(&destruct_FactorizedJetCorrector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FactorizedJetCorrector*)
   {
      return GenerateInitInstanceLocal((::FactorizedJetCorrector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FactorizedJetCorrector*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FactorizedJetCorrector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FactorizedJetCorrector*)0x0)->GetClass();
      FactorizedJetCorrector_TClassManip(theClass);
   return theClass;
   }

   static void FactorizedJetCorrector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_JetCorrectorParameters(void *p) {
      return  p ? new(p) ::JetCorrectorParameters : new ::JetCorrectorParameters;
   }
   static void *newArray_JetCorrectorParameters(Long_t nElements, void *p) {
      return p ? new(p) ::JetCorrectorParameters[nElements] : new ::JetCorrectorParameters[nElements];
   }
   // Wrapper around operator delete
   static void delete_JetCorrectorParameters(void *p) {
      delete ((::JetCorrectorParameters*)p);
   }
   static void deleteArray_JetCorrectorParameters(void *p) {
      delete [] ((::JetCorrectorParameters*)p);
   }
   static void destruct_JetCorrectorParameters(void *p) {
      typedef ::JetCorrectorParameters current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetCorrectorParameters

namespace ROOT {
   // Wrappers around operator new
   static void *new_JetCorrectorParameterscLcLDefinitions(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::JetCorrectorParameters::Definitions : new ::JetCorrectorParameters::Definitions;
   }
   static void *newArray_JetCorrectorParameterscLcLDefinitions(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::JetCorrectorParameters::Definitions[nElements] : new ::JetCorrectorParameters::Definitions[nElements];
   }
   // Wrapper around operator delete
   static void delete_JetCorrectorParameterscLcLDefinitions(void *p) {
      delete ((::JetCorrectorParameters::Definitions*)p);
   }
   static void deleteArray_JetCorrectorParameterscLcLDefinitions(void *p) {
      delete [] ((::JetCorrectorParameters::Definitions*)p);
   }
   static void destruct_JetCorrectorParameterscLcLDefinitions(void *p) {
      typedef ::JetCorrectorParameters::Definitions current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetCorrectorParameters::Definitions

namespace ROOT {
   // Wrappers around operator new
   static void *new_JetCorrectorParameterscLcLRecord(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::JetCorrectorParameters::Record : new ::JetCorrectorParameters::Record;
   }
   static void *newArray_JetCorrectorParameterscLcLRecord(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::JetCorrectorParameters::Record[nElements] : new ::JetCorrectorParameters::Record[nElements];
   }
   // Wrapper around operator delete
   static void delete_JetCorrectorParameterscLcLRecord(void *p) {
      delete ((::JetCorrectorParameters::Record*)p);
   }
   static void deleteArray_JetCorrectorParameterscLcLRecord(void *p) {
      delete [] ((::JetCorrectorParameters::Record*)p);
   }
   static void destruct_JetCorrectorParameterscLcLRecord(void *p) {
      typedef ::JetCorrectorParameters::Record current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetCorrectorParameters::Record

namespace ROOT {
   // Wrappers around operator new
   static void *new_SimpleJetCorrector(void *p) {
      return  p ? new(p) ::SimpleJetCorrector : new ::SimpleJetCorrector;
   }
   static void *newArray_SimpleJetCorrector(Long_t nElements, void *p) {
      return p ? new(p) ::SimpleJetCorrector[nElements] : new ::SimpleJetCorrector[nElements];
   }
   // Wrapper around operator delete
   static void delete_SimpleJetCorrector(void *p) {
      delete ((::SimpleJetCorrector*)p);
   }
   static void deleteArray_SimpleJetCorrector(void *p) {
      delete [] ((::SimpleJetCorrector*)p);
   }
   static void destruct_SimpleJetCorrector(void *p) {
      typedef ::SimpleJetCorrector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SimpleJetCorrector

namespace ROOT {
   // Wrappers around operator new
   static void *new_FactorizedJetCorrector(void *p) {
      return  p ? new(p) ::FactorizedJetCorrector : new ::FactorizedJetCorrector;
   }
   static void *newArray_FactorizedJetCorrector(Long_t nElements, void *p) {
      return p ? new(p) ::FactorizedJetCorrector[nElements] : new ::FactorizedJetCorrector[nElements];
   }
   // Wrapper around operator delete
   static void delete_FactorizedJetCorrector(void *p) {
      delete ((::FactorizedJetCorrector*)p);
   }
   static void deleteArray_FactorizedJetCorrector(void *p) {
      delete [] ((::FactorizedJetCorrector*)p);
   }
   static void destruct_FactorizedJetCorrector(void *p) {
      typedef ::FactorizedJetCorrector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FactorizedJetCorrector

namespace ROOT {
   static TClass *vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_Dictionary();
   static void vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p);
   static void destruct_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<FactorizedJetCorrector::VarTypes> >*)
   {
      vector<vector<FactorizedJetCorrector::VarTypes> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<FactorizedJetCorrector::VarTypes> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<FactorizedJetCorrector::VarTypes> >", -2, "vector", 210,
                  typeid(vector<vector<FactorizedJetCorrector::VarTypes> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<FactorizedJetCorrector::VarTypes> >) );
      instance.SetNew(&new_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<FactorizedJetCorrector::VarTypes> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<FactorizedJetCorrector::VarTypes> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<FactorizedJetCorrector::VarTypes> >*)0x0)->GetClass();
      vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<FactorizedJetCorrector::VarTypes> > : new vector<vector<FactorizedJetCorrector::VarTypes> >;
   }
   static void *newArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<FactorizedJetCorrector::VarTypes> >[nElements] : new vector<vector<FactorizedJetCorrector::VarTypes> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p) {
      delete ((vector<vector<FactorizedJetCorrector::VarTypes> >*)p);
   }
   static void deleteArray_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p) {
      delete [] ((vector<vector<FactorizedJetCorrector::VarTypes> >*)p);
   }
   static void destruct_vectorlEvectorlEFactorizedJetCorrectorcLcLVarTypesgRsPgR(void *p) {
      typedef vector<vector<FactorizedJetCorrector::VarTypes> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<FactorizedJetCorrector::VarTypes> >

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
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 210,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlESimpleJetCorrectormUgR_Dictionary();
   static void vectorlESimpleJetCorrectormUgR_TClassManip(TClass*);
   static void *new_vectorlESimpleJetCorrectormUgR(void *p = 0);
   static void *newArray_vectorlESimpleJetCorrectormUgR(Long_t size, void *p);
   static void delete_vectorlESimpleJetCorrectormUgR(void *p);
   static void deleteArray_vectorlESimpleJetCorrectormUgR(void *p);
   static void destruct_vectorlESimpleJetCorrectormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<SimpleJetCorrector*>*)
   {
      vector<SimpleJetCorrector*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<SimpleJetCorrector*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<SimpleJetCorrector*>", -2, "vector", 210,
                  typeid(vector<SimpleJetCorrector*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlESimpleJetCorrectormUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<SimpleJetCorrector*>) );
      instance.SetNew(&new_vectorlESimpleJetCorrectormUgR);
      instance.SetNewArray(&newArray_vectorlESimpleJetCorrectormUgR);
      instance.SetDelete(&delete_vectorlESimpleJetCorrectormUgR);
      instance.SetDeleteArray(&deleteArray_vectorlESimpleJetCorrectormUgR);
      instance.SetDestructor(&destruct_vectorlESimpleJetCorrectormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<SimpleJetCorrector*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<SimpleJetCorrector*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlESimpleJetCorrectormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<SimpleJetCorrector*>*)0x0)->GetClass();
      vectorlESimpleJetCorrectormUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlESimpleJetCorrectormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlESimpleJetCorrectormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<SimpleJetCorrector*> : new vector<SimpleJetCorrector*>;
   }
   static void *newArray_vectorlESimpleJetCorrectormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<SimpleJetCorrector*>[nElements] : new vector<SimpleJetCorrector*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlESimpleJetCorrectormUgR(void *p) {
      delete ((vector<SimpleJetCorrector*>*)p);
   }
   static void deleteArray_vectorlESimpleJetCorrectormUgR(void *p) {
      delete [] ((vector<SimpleJetCorrector*>*)p);
   }
   static void destruct_vectorlESimpleJetCorrectormUgR(void *p) {
      typedef vector<SimpleJetCorrector*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<SimpleJetCorrector*>

namespace ROOT {
   static TClass *vectorlEJetCorrectorParameterscLcLRecordgR_Dictionary();
   static void vectorlEJetCorrectorParameterscLcLRecordgR_TClassManip(TClass*);
   static void *new_vectorlEJetCorrectorParameterscLcLRecordgR(void *p = 0);
   static void *newArray_vectorlEJetCorrectorParameterscLcLRecordgR(Long_t size, void *p);
   static void delete_vectorlEJetCorrectorParameterscLcLRecordgR(void *p);
   static void deleteArray_vectorlEJetCorrectorParameterscLcLRecordgR(void *p);
   static void destruct_vectorlEJetCorrectorParameterscLcLRecordgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<JetCorrectorParameters::Record>*)
   {
      vector<JetCorrectorParameters::Record> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<JetCorrectorParameters::Record>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<JetCorrectorParameters::Record>", -2, "vector", 210,
                  typeid(vector<JetCorrectorParameters::Record>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEJetCorrectorParameterscLcLRecordgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<JetCorrectorParameters::Record>) );
      instance.SetNew(&new_vectorlEJetCorrectorParameterscLcLRecordgR);
      instance.SetNewArray(&newArray_vectorlEJetCorrectorParameterscLcLRecordgR);
      instance.SetDelete(&delete_vectorlEJetCorrectorParameterscLcLRecordgR);
      instance.SetDeleteArray(&deleteArray_vectorlEJetCorrectorParameterscLcLRecordgR);
      instance.SetDestructor(&destruct_vectorlEJetCorrectorParameterscLcLRecordgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<JetCorrectorParameters::Record> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<JetCorrectorParameters::Record>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEJetCorrectorParameterscLcLRecordgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<JetCorrectorParameters::Record>*)0x0)->GetClass();
      vectorlEJetCorrectorParameterscLcLRecordgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEJetCorrectorParameterscLcLRecordgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEJetCorrectorParameterscLcLRecordgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JetCorrectorParameters::Record> : new vector<JetCorrectorParameters::Record>;
   }
   static void *newArray_vectorlEJetCorrectorParameterscLcLRecordgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<JetCorrectorParameters::Record>[nElements] : new vector<JetCorrectorParameters::Record>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEJetCorrectorParameterscLcLRecordgR(void *p) {
      delete ((vector<JetCorrectorParameters::Record>*)p);
   }
   static void deleteArray_vectorlEJetCorrectorParameterscLcLRecordgR(void *p) {
      delete [] ((vector<JetCorrectorParameters::Record>*)p);
   }
   static void destruct_vectorlEJetCorrectorParameterscLcLRecordgR(void *p) {
      typedef vector<JetCorrectorParameters::Record> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<JetCorrectorParameters::Record>

namespace ROOT {
   static TClass *vectorlEFactorizedJetCorrectorcLcLVarTypesgR_Dictionary();
   static void vectorlEFactorizedJetCorrectorcLcLVarTypesgR_TClassManip(TClass*);
   static void *new_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p = 0);
   static void *newArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(Long_t size, void *p);
   static void delete_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p);
   static void deleteArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p);
   static void destruct_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FactorizedJetCorrector::VarTypes>*)
   {
      vector<FactorizedJetCorrector::VarTypes> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FactorizedJetCorrector::VarTypes>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FactorizedJetCorrector::VarTypes>", -2, "vector", 210,
                  typeid(vector<FactorizedJetCorrector::VarTypes>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFactorizedJetCorrectorcLcLVarTypesgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<FactorizedJetCorrector::VarTypes>) );
      instance.SetNew(&new_vectorlEFactorizedJetCorrectorcLcLVarTypesgR);
      instance.SetNewArray(&newArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR);
      instance.SetDelete(&delete_vectorlEFactorizedJetCorrectorcLcLVarTypesgR);
      instance.SetDeleteArray(&deleteArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR);
      instance.SetDestructor(&destruct_vectorlEFactorizedJetCorrectorcLcLVarTypesgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FactorizedJetCorrector::VarTypes> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FactorizedJetCorrector::VarTypes>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFactorizedJetCorrectorcLcLVarTypesgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FactorizedJetCorrector::VarTypes>*)0x0)->GetClass();
      vectorlEFactorizedJetCorrectorcLcLVarTypesgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFactorizedJetCorrectorcLcLVarTypesgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FactorizedJetCorrector::VarTypes> : new vector<FactorizedJetCorrector::VarTypes>;
   }
   static void *newArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FactorizedJetCorrector::VarTypes>[nElements] : new vector<FactorizedJetCorrector::VarTypes>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p) {
      delete ((vector<FactorizedJetCorrector::VarTypes>*)p);
   }
   static void deleteArray_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p) {
      delete [] ((vector<FactorizedJetCorrector::VarTypes>*)p);
   }
   static void destruct_vectorlEFactorizedJetCorrectorcLcLVarTypesgR(void *p) {
      typedef vector<FactorizedJetCorrector::VarTypes> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FactorizedJetCorrector::VarTypes>

namespace ROOT {
   static TClass *vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_Dictionary();
   static void vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_TClassManip(TClass*);
   static void *new_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p = 0);
   static void *newArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(Long_t size, void *p);
   static void delete_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p);
   static void deleteArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p);
   static void destruct_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FactorizedJetCorrector::LevelTypes>*)
   {
      vector<FactorizedJetCorrector::LevelTypes> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FactorizedJetCorrector::LevelTypes>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FactorizedJetCorrector::LevelTypes>", -2, "vector", 210,
                  typeid(vector<FactorizedJetCorrector::LevelTypes>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<FactorizedJetCorrector::LevelTypes>) );
      instance.SetNew(&new_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR);
      instance.SetNewArray(&newArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR);
      instance.SetDelete(&delete_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR);
      instance.SetDeleteArray(&deleteArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR);
      instance.SetDestructor(&destruct_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FactorizedJetCorrector::LevelTypes> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FactorizedJetCorrector::LevelTypes>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FactorizedJetCorrector::LevelTypes>*)0x0)->GetClass();
      vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFactorizedJetCorrectorcLcLLevelTypesgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FactorizedJetCorrector::LevelTypes> : new vector<FactorizedJetCorrector::LevelTypes>;
   }
   static void *newArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FactorizedJetCorrector::LevelTypes>[nElements] : new vector<FactorizedJetCorrector::LevelTypes>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p) {
      delete ((vector<FactorizedJetCorrector::LevelTypes>*)p);
   }
   static void deleteArray_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p) {
      delete [] ((vector<FactorizedJetCorrector::LevelTypes>*)p);
   }
   static void destruct_vectorlEFactorizedJetCorrectorcLcLLevelTypesgR(void *p) {
      typedef vector<FactorizedJetCorrector::LevelTypes> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FactorizedJetCorrector::LevelTypes>

namespace {
  void TriggerDictionaryInitialization_JetMETObjects_dict_Impl() {
    static const char* headers[] = {
"src/JetCorrectorParameters.h",
"src/SimpleJetCorrector.h",
"src/FactorizedJetCorrector.h",
0
    };
    static const char* includePaths[] = {
"/opt/ohpc/pub/root/root_v6.20.04/include",
"/opt/ohpc/pub/root/root_v6.20.04/include/",
"/home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "JetMETObjects_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$src/JetCorrectorParameters.h")))  JetCorrectorParameters;
class __attribute__((annotate("$clingAutoload$src/SimpleJetCorrector.h")))  SimpleJetCorrector;
class __attribute__((annotate("$clingAutoload$src/FactorizedJetCorrector.h")))  FactorizedJetCorrector;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "JetMETObjects_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/JetCorrectorParameters.h"
#include "src/SimpleJetCorrector.h"
#include "src/FactorizedJetCorrector.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"FactorizedJetCorrector", payloadCode, "@",
"JetCorrectorParameters", payloadCode, "@",
"JetCorrectorParameters::Definitions", payloadCode, "@",
"JetCorrectorParameters::Record", payloadCode, "@",
"SimpleJetCorrector", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("JetMETObjects_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_JetMETObjects_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_JetMETObjects_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_JetMETObjects_dict() {
  TriggerDictionaryInitialization_JetMETObjects_dict_Impl();
}
