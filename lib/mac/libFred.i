/*
    SWIG interface file for Fred - library - 2019
    N. Krah
	A.Schiavi
*/

%module FredAPI

// SWIG RAW INCLUDE
%{
#define SWIG_FILE_WITH_INIT
#include "libFred.h"

void throwPyException(const char *message){
    char fullmessage[256];
    const char *messagestub = "Fred library error: ";
    strncpy(fullmessage, messagestub, sizeof(fullmessage));
    strncat(fullmessage, message, (sizeof(fullmessage) - strlen(fullmessage)) - 1);
    PyErr_SetString(PyExc_Exception, fullmessage);
}

// Function to translate error codes in error message.
// Only a handful are implemented so far,
// check libFred.h header for more.
const char *getErrorMessage(int errCode) {
  switch (errCode) {
  case FRED_FILE_NOT_FOUND:
    return "File not found.";
  case FRED_REGION_NOT_FOUND:
    return "Region not found.";
  case FRED_ILLDEFINED_VECTOR:
    return "Ill defined vector.";
  case FRED_REGION_ALREADY_DEFINED:
    return "Region already defined.";
  default:
    printf("Error code: %d\n", errCode);
    return "Error code not implemented yet.";
  }
}
%}

// SWIG MODULES TO LOAD
/* %include "std_vector.i" */
%include <typemaps.i>
%include "numpy.i"
%init %{
import_array();
%}


/* ##############################
####       LIBRARY API       ####
################################# */

// int fredInit(const char* datadir);
%rename (Init) wrapped_fredInit;
%exception wrapped_fredInit {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredInit(const char* datadir){
    int ret;
    ret = fredInit(datadir);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

// int fredGetPThreads(); /* returns number of maximum POSIX threads for parallel execution */
%rename (GetPThreads) wrapped_fredGetPThreads;
%exception wrapped_fredGetPThreads {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredGetPThreads(){
    int ret;
    ret = fredGetPThreads();
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%}

/* int fredSetPThreads(int numThreads); */
%rename (SetPThreads) wrapped_fredSetPThreads;
%exception wrapped_fredSetPThreads {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetPThreads(int numThreads){
    int ret;
    ret = fredSetPThreads(numThreads);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredRedirectOutput(const char *out1,const char *out2); /* redirect std output to stream or filname, eg. "cout", "myout.txt" or "/dev/null" */
%rename (RedirectOutput) wrapped_fredRedirectOutput;
%exception wrapped_fredRedirectOutput {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredRedirectOutput(const char *out1,const char *out2){
    int ret;
    ret = fredRedirectOutput(out1, out2);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredRedirectError (const char *err1,const char *err2); /* redirect std error  to stream or filname, eg. "cerr", "myerr.txt" or "/dev/null"  */
%rename (RedirectError) wrapped_fredRedirectError;
%exception wrapped_fredRedirectError {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredRedirectError(const char *err1,const char *err2){
    int ret;
    ret = fredRedirectError(err1, err2);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredResetAll(); */
%rename (ResetAll) wrapped_fredResetAll;
%exception wrapped_fredResetAll {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredResetAll(){
    int ret;
    ret = fredResetAll();
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredResetFields(); */
%rename (ResetFields) wrapped_fredResetFields;
%exception wrapped_fredResetFields {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredResetFields(){
    int ret;
    ret = fredResetFields();
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredResetPencilBeams(); */
%rename (ResetPencilBeams) wrapped_fredResetPencilBeams;
%exception wrapped_fredResetPencilBeams {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredResetPencilBeams(){
    int ret;
    ret = fredResetPencilBeams();
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredResetRays(); */
%rename (ResetRays) wrapped_fredResetRays;
%exception wrapped_fredResetRays {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredResetRays(){
    int ret;
    ret = fredResetRays();
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredSetRandSeedRoot(unsigned long long  seed); */
%rename (SetRandSeedRoot) wrapped_fredSetRandSeedRoot;
%exception wrapped_fredSetRandSeedRoot {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRandSeedRoot(unsigned long long  seed){
    int ret;
    ret = fredSetRandSeedRoot(seed);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredGetRandSeedRoot(unsigned long long *seed); */
%apply long long *OUTPUT {unsigned long long *seed}
%rename (GetRandSeedRoot) wrapped_fredGetRandSeedRoot;
%exception wrapped_fredGetRandSeedRoot {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetRandSeedRoot(unsigned long long *seed){
    int ret;
    ret = fredGetRandSeedRoot(seed);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

// Note: throws missing symbol. Not implemented yet in library I guess
/* int fredClose(); */
/* %rename (Close) wrapped_fredClose;
%exception wrapped_fredClose {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredClose(){
    int ret;
    ret = fredClose();
    if(ret) throwPyException(getErrorMessage(ret));
}
%} */


/* ##############################
####       REGION API        ####
################################# */

/* int fred_NumRegions(); /* return number of defined regions */
%rename (NumRegions) wrapped_fred_NumRegions;
%exception wrapped_fred_NumRegions {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fred_NumRegions(){
    int ret;
    ret = fred_NumRegions();
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%}

/* int fredRegion_index(const char *regID); /* return index of region for given ID; returns -1 if regID is not defined*/
%rename (Region_index) wrapped_fredRegion_index;
%exception wrapped_fredRegion_index {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredRegion_index(const char *regID){
    int ret;
    ret = fredRegion_index(regID);
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return ret;
    }
    else {
        return ret;
    }
}
%}

/* int fredRegion_info(int ireg ); /* print info on region with given index*/
%rename (Region_info) wrapped_fredRegion_info;
%exception wrapped_fredRegion_info {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredRegion_info(int ireg){
    int ret;
    ret = fredRegion_info(ireg);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredAddRegion(const char *regID); /* add new region with name regID; returns index of new region or -1 if error*/
%rename (AddRegion) wrapped_fredAddRegion;
%exception wrapped_fredAddRegion {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredAddRegion(const char *regID){
    int ret;
    ret = fredAddRegion(regID);
    if(ret<0) throwPyException(getErrorMessage(ret));
}
%}

/* int fredGetRegion_extent(int ireg, float L[3]); */
%apply float ARGOUT_ARRAY1[ANY] { float L[3] };
%rename (GetRegion_extent) wrapped_fredGetRegion_extent;
%exception wrapped_fredGetRegion_extent {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetRegion_extent(int ireg, float L[3]){
    int ret;
    ret = fredGetRegion_extent(ireg, L);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float L[3]);

/* int fredSetRegion_extent(int ireg, float L[3]); */
%apply float IN_ARRAY1[ANY] { float L[3] };
%rename (SetRegion_extent) wrapped_fredSetRegion_extent;
%exception wrapped_fredSetRegion_extent {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRegion_extent(int ireg, float L[3]){
    int ret;
    ret = fredSetRegion_extent(ireg, L);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float L[3]);

/* int fredGetRegion_origin(int ireg, float O[3]); */
%apply float ARGOUT_ARRAY1[ANY] { float O[3] };
%rename (GetRegion_origin) wrapped_fredGetRegion_origin;
%exception wrapped_fredGetRegion_origin {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetRegion_origin(int ireg, float O[3]){
    int ret;
    ret = fredGetRegion_origin(ireg, O);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);

/* int fredSetRegion_origin(int ireg, float O[3]); */
%apply float IN_ARRAY1[ANY] { float O[3] };
%rename (SetRegion_origin) wrapped_fredSetRegion_origin;
%exception wrapped_fredSetRegion_origin {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRegion_origin(int ireg, float O[3]){
    int ret;
    ret = fredSetRegion_origin(ireg, O);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);

/* int fredGetRegion_pivot(int ireg, float pivot[3]); */
%apply float ARGOUT_ARRAY1[ANY] { float pivot[3] };
%rename (GetRegion_pivot) wrapped_fredGetRegion_pivot;
%exception wrapped_fredGetRegion_pivot {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetRegion_pivot(int ireg, float pivot[3]){
    int ret;
    ret = fredGetRegion_pivot(ireg, pivot);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float pivot[3]);

/* int fredSetRegion_pivot(int ireg, float pivot[3]); */
%apply float IN_ARRAY1[ANY] { float pivot[3] };
%rename (SetRegion_pivot) wrapped_fredSetRegion_pivot;
%exception wrapped_fredSetRegion_pivot {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRegion_pivot(int ireg, float pivot[3]){
    int ret;
    ret = fredSetRegion_pivot(ireg, pivot);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float pivot[3]);

/* int fredGetRegion_voxels(int ireg, int voxels[3]); */
%apply int ARGOUT_ARRAY1[ANY] { int nn[3] };
%rename (GetRegion_voxels) wrapped_fredGetRegion_voxels;
%exception wrapped_fredGetRegion_voxels {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetRegion_voxels(int ireg, int nn[3]){
    int ret;
    ret = fredGetRegion_voxels(ireg, nn);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (int nn[3]);

/* int fredSetRegion_voxels(int ireg, float voxels[3]); */
%apply int IN_ARRAY1[ANY] { int nn[3] };
%rename (SetRegion_voxels) wrapped_fredSetRegion_voxels;
%exception wrapped_fredSetRegion_voxels {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRegion_voxels(int ireg, int nn[3]){
    int ret;
    ret = fredSetRegion_voxels(ireg, nn);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (int nn[3]);

/* int fredGetRegion_material(int ireg); */
/* %rename (GetRegion_material) wrapped_fredGetRegion_material;
%exception wrapped_fredGetRegion_material {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredGetRegion_material(int ireg){
    int ret;
    ret = fredGetRegion_material(ireg);
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return ret;
    }
    else {
        return ret;
    }
}
%} */

/* int fredSetRegion_material(int ireg, int imat); */
%rename (SetRegion_material) wrapped_fredSetRegion_material;
%exception wrapped_fredSetRegion_material {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetRegion_material(int ireg, int imat){
    int ret;
    ret = fredSetRegion_material(ireg, imat);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredLoadRegion_CTscan(int ireg, const char *fpath); */
%rename (LoadRegion_CTscan) wrapped_fredLoadRegion_CTscan;
%exception wrapped_fredLoadRegion_CTscan {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredLoadRegion_CTscan(int ireg, const char *fpath){
    int ret;
    ret = fredLoadRegion_CTscan(ireg, fpath);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}


/* ##############################
####        SCORER API       ####
################################# */

// This struct is translated into a python class with the members listed in enum{}
%inline %{
struct RegionScorers {
    enum {trackScorer, eDepScorer, doseScorer, LETdScorer, countsScorer};
};
%}

/* int fredAddScorer(int ireg, int type); */
%rename (AddScorer) wrapped_fredAddScorer;
%exception wrapped_fredAddScorer {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredAddScorer(int ireg, int type){
    int ret;
    ret = fredAddScorer(ireg, type);
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return ret;
    }
    else {
        return ret;
    }
}
%}

/* int fredScorer_reset(int ireg, int iscorer); */
%rename (Scorer_reset) wrapped_fredScorer_reset;
%exception wrapped_fredScorer_reset {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredScorer_reset(int ireg, int iscorer){
    int ret;
    ret = fredScorer_reset(ireg, iscorer);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredScorer_evaluate(int ireg, int iscorer); */
%rename (Scorer_evaluate) wrapped_fredScorer_evaluate;
%exception wrapped_fredScorer_evaluate {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredScorer_evaluate(int ireg, int iscorer){
    int ret;
    ret = fredScorer_evaluate(ireg, iscorer);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredScorer_save(int ireg, int iscorer, const char *fpath); */
%rename (Scorer_save) wrapped_fredScorer_save;
%exception wrapped_fredScorer_save {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredScorer_save(int ireg, int iscorer, const char *fpath){
    int ret;
    ret = fredScorer_save(ireg, iscorer, fpath);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* float* fredScorer_buffer(int ireg, int iscorer, int nn[3], int ierr); */
%apply (float* ARGOUT_ARRAY1, int DIM1) { (float *buffer, int size) };
%rename (Scorer_buffer) wrapped_fredScorer_buffer;
%exception wrapped_fredScorer_buffer {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredScorer_buffer(int ireg, int iscorer, float *buffer, int size){
    int nn[3];
    int ret;
    ret = fredGetRegion_voxels(ireg, nn);
    if(ret) {
        throwPyException(getErrorMessage(ret));
        return;
    }
    // Note: *buffer is allocated as numpy array by python.
    // Without explicitly assigning the values to it, i.e. using
    // buffer = fredScorer_buffer(ireg, iscorer, nn, ret);
    // results in zeros on python side. Not fully sure why,
    // but this implementation works for now.
    // Need to check memory footprint.
    float *transferbuffer;
    transferbuffer = fredScorer_buffer(ireg, iscorer, nn, ret);
    if(ret) throwPyException(getErrorMessage(ret));
    for(int i = 0; i < size; i++){
        buffer[i] = transferbuffer[i];
    }
}
%}
%clear (float *buffer, int size);


/* ##############################
####      MATERIAL API       ####
################################# */

/* int fred_NumMaterials(); */
%rename (NumMaterials) wrapped_fred_NumMaterials;
%exception wrapped_fred_NumMaterials {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fred_NumMaterials(){
    int ret;
    ret = fred_NumMaterials();
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%}

/* int fredMaterial_index(const char *matID); */
%rename (Material_index) wrapped_fredMaterial_index;
%exception wrapped_fredMaterial_index {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredMaterial_index(const char *matID){
    int ret;
    ret = fredMaterial_index(matID);
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%}

/* int fredMaterial_info(int imat); */
%rename (Material_info) wrapped_fredMaterial_info;
%exception wrapped_fredMaterial_info {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredMaterial_info(int imat){
    int ret;
    ret = fredMaterial_info(imat);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* NOT IMPLEMENTED YET */
/* int fredAddMaterial(const char *matID); */
/* %rename (AddMaterial) wrapped_fredAddMaterial;
%exception wrapped_fredAddMaterial {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredAddMaterial(const char *matID){
    int ret;
    ret = fredAddMaterial(matID);
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%} */


/* #################################
####        FIELD API        ####
################################# */

/* int fredAddField(int FID); */
%rename (AddField) wrapped_fredAddField;
%exception wrapped_fredAddFieldo {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredAddField(int FID){
    int ret;
    ret = fredAddField(FID);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredDeleteField(int FID); */
%rename (DeleteField) wrapped_fredDeleteField;
%exception wrapped_fredDeleteField {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredDeleteField(int FID){
    int ret;
    ret = fredDeleteField(FID);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredSetField_FoR(int FID, float O[3], float f[3], float u[3], float l[3]); */
%apply float IN_ARRAY1[ANY] { (float O[3]) };
%apply float IN_ARRAY1[ANY] { (float f[3]) };
%apply float IN_ARRAY1[ANY] { (float u[3]) };
%apply float IN_ARRAY1[ANY] { (float l[3]) };

%rename (SetField_FoR) wrapped_fredSetField_FoR;
%exception wrapped_fredSetField_FoR {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetField_FoR(int FID, float O[3], float f[3], float u[3], float l[3]){
    int ret;
    ret = fredSetField_FoR(FID, O, f, u, l);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);
%clear (float f[3]);
%clear (float u[3]);
%clear (float l[3]);

/* int fredGetField_FoR(int FID, float O[3], float f[3], float u[3], float l[3]); */
%apply float ARGOUT_ARRAY1[ANY] { (float O[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float f[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float u[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float l[3]) };

%rename (GetField_FoR) wrapped_fredGetField_FoR;
%exception wrapped_fredGetField_FoR {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetField_FoR(int FID, float O[3], float f[3], float u[3], float l[3]){
    int ret;
    ret = fredGetField_FoR(FID, O, f, u, l);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);
%clear (float f[3]);
%clear (float u[3]);
%clear (float l[3]);


/* #################################
####       PENCIL BEAM API      ####
################################# */
// >> not fully implemented yet in the library

/* int fredAddPB(int FID, int PBID); */
%rename (AddPB) wrapped_fredAddPB;
%exception wrapped_fredAddPB {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredAddPB(int FID, int PBID){
    int ret;
    ret = fredAddPB(FID, PBID);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredDeletePB(int FID, int PBID); */
%rename (DeletePB) wrapped_fredDeletePB;
%exception wrapped_fredDeletePB {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredDeletePB(int FID, int PBID){
    int ret;
    ret = fredDeletePB(FID, PBID);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredSetPB_FoR(int FID, int PBID, float O[3], float f[3], float u[3], float l[3], int frame); */
/* %apply float IN_ARRAY1[ANY] { (float O[3]) };
%apply float IN_ARRAY1[ANY] { (float f[3]) };
%apply float IN_ARRAY1[ANY] { (float u[3]) };
%apply float IN_ARRAY1[ANY] { (float l[3]) };

%rename (SetPencilBeamFrameOfReference) wrapped_fredSetPB_FoR;
%exception wrapped_fredSetPB_FoR {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetPB_FoR(int FID, int PBID, float O[3], float f[3], float u[3], float l[3], int frame){
    int ret;
    ret = fredSetPB_FoR(FID, PBID, O, f, u, l, frame);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);
%clear (float f[3]);
%clear (float u[3]);
%clear (float l[3]); */

/* int fredGetPB_FoR(int FID, int PBID, float O[3], float f[3], float u[3], float l[3], int frame); */
/* %apply float ARGOUT_ARRAY1[ANY] { (float O[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float f[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float u[3]) };
%apply float ARGOUT_ARRAY1[ANY] { (float l[3]) };

%rename (GetPencilBeamFrameOfReference) wrapped_fredGetPB_FoR;
%exception wrapped_fredGetPB_FoR {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredGetPB_FoR(int FID, int PBID, float O[3], float f[3], float u[3], float l[3], int frame){
    int ret;
    ret = fredGetPB_FoR(FID, PBID, O, f, u, l, frame);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float O[3]);
%clear (float f[3]);
%clear (float u[3]);
%clear (float l[3]); */

/* int fredSetPB_particle(int FID, int PBID, char *particle); */
/* %rename (SetPencilBeamParticle) wrapped_fredSetPB_particle;
%exception wrapped_fredSetPB_particle {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredSetPB_particle(int FID, int PBID, char *particle){
    int ret;
    ret = fredSetPB_particle(FID, PBID, particle);
    if(ret) throwPyException(getErrorMessage(ret));
}
%} */


/* #################################
####           RAY API          ####
################################# */

// The struct could be wrapped like this, but it is not needed.
/* struct PhaseSpace_xvT_s {
    float x[3];
    float v[3];
    float T;
};
typedef PhaseSpace_xvT_s PhaseSpace_xvT; */

// A ray is essentially a list of 7 parameters, which is easiest mapped to a numpy array.
// A list of rays is then simply a 2D array: Nx7.
// Not that this wrapping requires the python side to assure that the rays array is 2D,
// even if it only contains one array. Use numpy.atleast_2d(rays) for this.
/* int fredAddRays(const char *particle, int nrays, struct PhaseSpace_xvT_s *rays); */
%apply ( float* IN_ARRAY2, int DIM1, int DIM2 ) { (float *rays, int nrays, int nentries) };
%rename (AddRays) wrapped_fredAddRays;
%exception wrapped_fredAddRays {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredAddRays(float *rays, int nrays, int nentries, const char *particle){
    if (nentries != 7){
        throwPyException("Expecting 7 entries per array row: x[3], v[3], and T.");
        return;
    }
    PhaseSpace_xvT *structArray;
    structArray = (PhaseSpace_xvT *) malloc(nrays*sizeof(PhaseSpace_xvT));
    for(int i = 0; i<nrays; i++){
        structArray[i].x[0] = rays[i*nentries];
        structArray[i].x[1] = rays[i*nentries+1];
        structArray[i].x[2] = rays[i*nentries+2];
        structArray[i].v[0] = rays[i*nentries+3];
        structArray[i].v[1] = rays[i*nentries+4];
        structArray[i].v[2] = rays[i*nentries+5];
        structArray[i].T = rays[i*nentries+6];
    }
    int ret;
    ret = fredAddRays(particle, nrays, structArray);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

// This function needs to be checked. I guess if its only purpose is
// to check rays for potential use, one can free the memory immediately after
// the function call. If the function is intended to check rays
// which have already been added to the simulation the below implementation is
// not ideal.
/* int fredCheckRays(const char *particle, int nrays, PhaseSpace_xvT *rays); */
%rename (CheckRays) wrapped_fredCheckRays;
%exception wrapped_fredCheckRays {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredCheckRays(float *rays, int nrays, int nentries, const char *particle){
    if (nentries != 7){
        throwPyException("Expecting 7 entries per array row: x[3], v[3], and T.");
        return;
    }
    PhaseSpace_xvT *structArray;
    structArray = (PhaseSpace_xvT *) malloc(nrays*sizeof(PhaseSpace_xvT));
    for(int i = 0; i<nrays; i++){
        structArray[i].x[0] = rays[i*nentries];
        structArray[i].x[1] = rays[i*nentries+1];
        structArray[i].x[2] = rays[i*nentries+2];
        structArray[i].v[0] = rays[i*nentries+3];
        structArray[i].v[1] = rays[i*nentries+4];
        structArray[i].v[2] = rays[i*nentries+5];
        structArray[i].T = rays[i*nentries+6];
    }
    int ret;
    ret = fredCheckRays(particle, nrays, structArray);
    free(structArray);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}
%clear (float *rays, int nrays, int nentries);

/* int fredGetNumRays(); */
%rename (GetNumRays) wrapped_fredGetNumRays;
%exception wrapped_fredGetNumRays {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
int wrapped_fredGetNumRays(){
    int ret;
    ret = fredGetNumRays();
    if(ret<0){
        throwPyException(getErrorMessage(ret));
        return 0;
    }
    else {
        return ret;
    }
}
%}


/* #################################
####         PLUGIN API         ####
################################# */

// NOT WRAPPED FOR NOW



/* #################################
####         CONTROL API        ####
################################# */

/* int fredCloseSetup(); */
%rename (CloseSetup) wrapped_fredCloseSetup;
%exception wrapped_fredCloseSetup {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredCloseSetup(){
    int ret;
    ret = fredCloseSetup();
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateEloss(int onoff); */
%rename (ActivateEloss) wrapped_fredActivateEloss;
%exception wrapped_fredActivateEloss {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateEloss(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateEloss(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateFluc(int onoff); */
%rename (ActivateFluc) wrapped_fredActivateFluc;
%exception wrapped_fredActivateFluc {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateFluc(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateFluc(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateMCS(int onoff); */
%rename (ActivateMCS) wrapped_fredActivateMCS;
%exception wrapped_fredActivateMCS {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateMCS(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateMCS(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateNuclear(int onoff); */
%rename (ActivateNuclear) wrapped_fredActivateNuclear;
%exception wrapped_fredActivateNuclear {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateNuclear(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateNuclear(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateNuclearElastic(int onoff); */
%rename (ActivateNuclearElastic) wrapped_fredActivateNuclearElastic;
%exception wrapped_fredActivateNuclearElastic {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateNuclearElastic(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateNuclearElastic(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredActivateNuclearInelastic(int onoff); */
%rename (ActivateNuclearInelastic) wrapped_fredActivateNuclearInelastic;
%exception wrapped_fredActivateNuclearInelastic {
    $action
    if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredActivateNuclearInelastic(bool bool_onoff){
    int int_onoff = 0;
    if (bool_onoff) int_onoff = 1;
    int ret;
    ret = fredActivateNuclearInelastic(int_onoff);
    if(ret) throwPyException(getErrorMessage(ret));
}
%}

/* int fredTrackRays(int iraymin, int iraymax); */
%rename (TrackRays) wrapped_fredTrackRays;
%exception wrapped_fredTrackRays {
  $action
  if (PyErr_Occurred()) SWIG_fail;
}
%inline %{
void wrapped_fredTrackRays(int iraymin, int iraymax){
  int ret;
  ret = fredTrackRays(iraymin, iraymax);
  if(ret) throwPyException(getErrorMessage(ret));
}
%}
