/*
	Fred - library interface - 2018

	A.Schiavi
*/



#ifndef LIB_FRED_H
#define LIB_FRED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Error Codes */
#define FRED_SUCCESS                              0
#define FRED_GENERIC_ERROR_1                      -1
#define FRED_GENERIC_ERROR_2                      -2
#define FRED_GENERIC_ERROR_3                      -3
#define FRED_GENERIC_ERROR_4                      -4


#define FRED_GENERIC_IO_ERROR                     -10
#define FRED_FILE_NOT_FOUND                       -11
#define FRED_DIRECTORY_NOT_FOUND                  -12

#define FRED_VALUE_RANGE_ERROR                    -20
#define FRED_VALUE_IS_NAN_ERROR                   -21
#define FRED_VALUE_IS_INF_ERROR                   -22
#define FRED_NULL_POINTER_ERROR                   -29

#define FRED_ILLDEFINED_VECTOR                    -50
#define FRED_ILLDEFINED_DIMENSIONS                -51
#define FRED_ILLDEFINED_FRAME_OF_REFERENCE        -52
#define FRED_TOO_FEW_BASIS_VECTORS                -53
#define FRED_TOO_MANY_BASIS_VECTORS               -54
#define FRED_VECTORS_ARE_PERPENDICULAR            -55
#define FRED_VECTORS_ARE_NOT_PERPENDICULAR        -56
#define FRED_VECTORS_ARE_PARALLEL                 -57
#define FRED_VECTORS_ARE_NOT_PARALLEL             -58
#define FRED_LEFT_HANDED_FRAME_OF_REFERENCE       -59



#define FRED_REGION_NOT_FOUND                     -100
#define FRED_REGION_ALREADY_DEFINED               -101

#define FRED_MATERIAL_NOT_FOUND                   -200

#define FRED_SCORER_NOT_FOUND                     -300
#define FRED_SCORER_NOT_EVALUATED                 -310


#define FRED_PARTICLE_CODE_NOT_FOUND              -400

#define FRED_BEAM_NOT_FOUND                       -500

#define FRED_SETUP_IS_CLOSED                      -1000


/********************************************************************************************************/

/* fred_reference_frames */
#define FRED_FRAME_GLOBAL                         0
#define FRED_FRAME_ROOM                           0
#define FRED_FRAME_REGION                         -1
#define FRED_FRAME_BEAM                           -2

/* fred_geometryclose_status */
#define FRED_GEOMETRYCLOSE_SUCCESS                0
#define FRED_GEOMETRYCLOSE_NONE                   -1
#define FRED_GEOMETRYCLOSE_ERROR                  -2
#define FRED_GEOMETRYCLOSE_IN_PROGRESS            -3

/* fred_tracking status */
#define FRED_TRACKING_COMPLETE                    0x0
#define FRED_TRACKING_RUNNING                     0x1
// #define FRED_TRACKING_SUBMITTED                   0x2
// #define FRED_TRACKING_QUEUED                      0x3


/********************************************************************************************************/
struct PhaseSpace_xvT_s {
    float x[3]; /* ray position  in global frame of reference */
    float v[3]; /* ray direction (versor) in global frame of reference */
    float T; /* ray kinetic energy */
};
typedef PhaseSpace_xvT_s PhaseSpace_xvT;
/********************************************************************************************************/

/* particle codes */

/* PDG 2006 CODES for MC */
const int PROTON_ID      =     2212;
const int NEUTRON_ID     =     2112;
const int ELECTRON_ID    =       11;
const int POSITRON_ID    =      -11;
const int PHOTON_ID      =       22;

/* nuclear codes (PDG 2006) : ±10LZZZAAAI */
const int DEUTERON_ID    =     1000010020;
const int TRITON_ID      =     1000010030;

const int ALPHA_ID       =     1000020040;
const int He3_ID         =     1000020030;
const int He6_ID         =     1000020060;

const int Li6_ID         =     1000030060;
const int Li7_ID         =     1000030070;
const int Li8_ID         =     1000030080;
const int Li9_ID         =     1000030090;

const int Be7_ID         =     1000040070;
const int Be9_ID         =     1000040090;
const int Be10_ID        =     1000040100;

const int B8_ID          =     1000050080;
const int B9_ID          =     1000050090;
const int B10_ID         =     1000050100;
const int B11_ID         =     1000050110;

const int C9_ID          =     1000060090;
const int C10_ID         =     1000060100;
const int C11_ID         =     1000060110;
const int C12_ID         =     1000060120;

const int O16_ID         =     1000080160;

/* codes from 81 to 100 can be used by internal pseudoparticles */

const int GHOST_ID      =       88;
const int GEORAY_ID     =       99;
const int ZORK_ID       =       81;

/********************************************************************************************************/

/* Library API */

int fredInit(const char* datadir/* path to directory containing data repository libFred.data */);
int fredGetPThreads(); /* returns number of maximum POSIX threads for parallel execution */
int fredSetPThreads(int numThreads); /* set number of maximum POSIX threads for parallel execution */

int fredRedirectOutput(const char *out1,const char *out2); /* redirect std output to stream or filname, eg. "cout", "myout.txt" or "/dev/null" */
int fredRedirectError (const char *err1,const char *err2); /* redirect std error  to stream or filname, eg. "cerr", "myerr.txt" or "/dev/null" */

int fredAddSearchPath(const char *path); /* prepend path to the list of searched locations */


int fredResetAll();
// int fredResetFields(); <--- discontinued
int fredResetRegions();
int fredResetBeams();
int fredResetRays();

int fredGetRandSeedRoot(unsigned long long *seed);
int fredSetRandSeedRoot(unsigned long long  seed);

int fredClose();

/* Region API */
int fred_NumRegions(); /* return number of defined regions */
int fredRegion_index(const char *regID); /* return index of region for given ID; returns -1 if regID is not defined*/
int fredRegion_info(int ireg ); /* print info on region with given index*/
int fredAddRegion(const char *regID); /* add new region with name regID; returns index of new region or -1 if error*/

int fredGetRegion_extent(int ireg /* region index */,
                         float L[3] /* region extent */);
int fredSetRegion_extent(int ireg /* region index */,
                         float L[3] /* region extent */);

int fredGetRegion_origin(int ireg /* region index */,
                         float O[3] /* region origin */);
int fredSetRegion_origin(int ireg /* region index */,
                         float O[3] /* region origin */);

int fredGetRegion_pivot(int ireg /* region index */,
                        float pivot[3] /* region pivot */);
int fredSetRegion_pivot(int ireg /* region index */,
                        float pivot[3] /* region pivot */);

int
fredSetRegion_FoR(int ireg /* region index */,
              float O[3] /* region Origin in global frame of reference */,
              float f[3] /* region front vector in global frame of reference */,
              float u[3] /* region up    vector in global frame of reference */,
              float l[3] /* region left  vector in global frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_GLOBAL or region index */);

int
fredGetRegion_FoR(int ireg /* region index */,
              float O[3] /* region Origin in global frame of reference */,
              float f[3] /* region front vector in global frame of reference */,
              float u[3] /* region up    vector in global frame of reference */,
              float l[3] /* region left  vector in global frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_GLOBAL or region index */);


int fredGetRegion_voxels(int ireg /* region index */,
                         int nn[3] /* region subdivisions */);
int fredSetRegion_voxels(int ireg /* region index */,
                         int nn[3] /* region subdivisions */);

int fredGetRegion_material(int ireg /* region index */);
/* returns index of material if region is homogeneous, else -1 */

int fredSetRegion_material(int ireg /* region index */,
                         int imat /* material index */);

int fredLoadRegion_CTscan(int ireg /* region index */,
                         const char *fpath /* path to a file containing a 3D map in HU */);


/* Scorer API */
enum regionScorer {trackScorer,eDepScorer,doseScorer,LETdScorer,countsScorer};

int fredAddScorer(int ireg /* region index */,
                  int type /* scorer type: trackScorer, eDepScorer, ... */);
                  /* returns index of scorer or -1 on error*/ 

int fredScorer_reset(int ireg /* region index */,
                     int iscorer /* scorer index ... */);

int fredScorer_evaluate(int ireg /* region index */,
                        int iscorer /* scorer index ... */);

int fredScorer_save(int ireg /* region index */,
                    int iscorer /* scorer index ... */,
                    const char *fpath /* file path */);

int fredGetScorer_buffer(int ireg /* region index */,
                      int iscorer /* scorer index ... */,
                      float *buffer);
          /* returns a buffer filled with scorer values; user is responsible for allocation and deallocation of the pointed memory */ 

/* Material API */
int fred_NumMaterials(); /* return number of defined materials */
int fredMaterial_index(const char *matID); /* return index of material for given ID; returns -1 if matID is not defined*/
int fredMaterial_info(int imat ); /* print info on material with given index*/

// TO BE IMPLEMENTED
int fredAddMaterial(const char *matID); /* add new material with name matID; returns index of new material or -1 if error*/

// /* Phantom API */
// int 
// fredLoadPhantom_m3d(const char *pathname /* path to a m3d file containing the HU map of the phantom */ );

// int
// fredSetPhantom_offset(float offset[3] /* offset along x,y and z direction */);
// int
// fredGetPhantom_offset(float offset[3] /* offset along x,y and z direction */);

// /* Field API */
// int
// fredAddField(int fid /* field (numeric) ID */);

// int
// fredDeleteField(int fid /* field (numeric) ID */);

// int
// fredSetField_FoR(int fid /* field (numeric) ID */,
//               float O[3] /* field Origin in global frame of reference */,
//               float f[3] /* field front vector in global frame of reference */,
//               float u[3] /* field up    vector in global frame of reference */,
//               float l[3] /* field left  vector in global frame of reference */);
// int
// fredGetField_FoR(int fid /* field (numeric) ID */,
//               float O[3] /* field Origin in global frame of reference */,
//               float f[3] /* field front vector in global frame of reference */,
//               float u[3] /* field up    vector in global frame of reference */,
//               float l[3] /* field left  vector in global frame of reference */);

/* Beam API */
int
fredAddBeam();

//int fredDeleteBeam(int ibeam /* beam index */); <-- discontinued

int
fred_NumBeams(); /* return number of defined beams*/ 


int
fredSetBeam_FoR(int ibeam /* beam index */,
              float O[3] /* beam Origin in selected frame of reference */,
              float f[3] /* beam front vector in selected frame of reference */,
              float u[3] /* beam up    vector in selected frame of reference */,
              float l[3] /* beam left  vector in selected frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_GLOBAL or region index */);

int
fredGetBeam_FoR(int ibeam /* beam index */,
              float O[3] /* beam Origin in selected frame of reference */,
              float f[3] /* beam front vector in selected frame of reference */,
              float u[3] /* beam up    vector in selected frame of reference */,
              float l[3] /* beam left  vector in selected frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_GLOBAL or region index */);

int
fredSetBeam_parentRegion(int ibeam /* beam index */,
                         int ireg /* parent region index */);
int
fredGetBeam_parentRegion(int ibeam /* beam index */,
                         int *ireg /* returns here the parent region index */);

int
fredSetBeam_particle(int ibeam /* beam index */,
                     int particleID /* particle ID (see begin of header file) */);

int
fredGetBeam_particle(int ibeam /* beam index */,
                     int *particleID /* returns here the particle ID */);

int
fredGetBeam_numRays(int ibeam /* beam index */);


/* Ray API */
int
fredCheckRays(int particleID /* particle ID (see begin of header file) */,
              int nrays /* number of rays to be checked for internal consistency */,
              PhaseSpace_xvT *rays /* array of rays */);

int
fredAddRays(int particleID /* particle ID (see begin of header file) */,
            int nrays /* number of rays to add */,
            PhaseSpace_xvT *rays /* array of rays */);

int
fredAddRaysToBeam(int particleID /* particle ID (see begin of header file) */,
            int nrays /* number of rays to add */,
            PhaseSpace_xvT *rays /* array of rays */,
            int ibeam /* beam for this batch of rays*/);

int
fredGetNumRays();


/* Plugin API */
int
fredLoadPlugin(const char *pathname /* path to a shared object containing a Fred plugin */ );

int 
fredPlugin_init();

int 
fredPlugin_close();

// int
// fredSwitchOnPlugin();
// int
// fredSwitchOffPlugin();

int
fredPluginShareBuffer(int BufferID /* buffer ID to be shared with plugin */ ,
                      void * buff /* local (i.e. caller-side) buffer pointer */,
                      size_t count/* num of elements in buffer*/,
                      size_t size /* element size in Bytes */ );

int
fredPluginObtainBuffer(int BufferID /* buffer ID to be shared with plugin */ ,
                      void ** buff /* remote (i.e. plugin-side) buffer handle */,
                      size_t *count/* num of elements in buffer*/,
                      size_t *size /* element size in Bytes */ );

/* Control API */
int
fredCloseSetup();

int fredActivateEloss(int onoff /* set on/off (1/0)  the energy loss module dEdx*/);
int fredActivateFluc(int onoff /* set on/off (1/0) the energy straggling module */);
int fredActivateMCS(int onoff /* set on/off (1/0) the Multiple Coulomb Scattering module */);
int fredActivateNuclear(int onoff /* set on/off (1/0) the whole nuclear interaction module */);
int fredActivateNuclearElastic(int onoff /* set on/off (1/0) the elastic nuclear interaction module */);
int fredActivateNuclearInelastic(int onoff /* set on/off (1/0) the inelastic nuclear interaction module */);

int
fredTrackRays(int iraymin /* minimum ray index: 0 is the first ray */,
              int iraymax /* maximum ray index (inclusive): if -1 then from iraymin to end */ );

int
fredTrackBeams(int ibeammin /* minimum beam index: 0 is the first created beam  */,
               int ibeammax /* maximum beam index (inclusive): if -1 then from ibeammin to end */ );
                /* track all rays belonging to given beams */


#ifdef __cplusplus
}
#endif

#endif

