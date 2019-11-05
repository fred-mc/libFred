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


#define FRED_FILE_NOT_FOUND                       -10
#define FRED_IO_ERROR                             -11


#define FRED_ILLDEFINED_VECTOR                    -50
#define FRED_ILLDEFINED_DIMENSIONS                -51

#define FRED_REGION_NOT_FOUND                     -100
#define FRED_REGION_ALREADY_DEFINED               -101

#define FRED_MATERIAL_NOT_FOUND                   -200

#define FRED_SCORER_NOT_FOUND                     -300

/********************************************************************************************************/

/* fred_reference_frames */
#define FRED_FRAME_GLOBAL                            0
#define FRED_FRAME_ROOM                              0
#define FRED_FRAME_REGION                            1
#define FRED_FRAME_FIELD                             2
#define FRED_FRAME_PB                                3

/* fred_geometryclose_status */
#define FRED_GEOMETRYCLOSE_SUCCESS                            0
#define FRED_GEOMETRYCLOSE_NONE                               -1
#define FRED_GEOMETRYCLOSE_ERROR                              -2
#define FRED_GEOMETRYCLOSE_IN_PROGRESS                        -3

/* fred_tracking status */
#define FRED_TRACKING_COMPLETE                                 0x0
#define FRED_TRACKING_RUNNING                                  0x1
// #define FRED_TRACKING_SUBMITTED                                0x2
// #define FRED_TRACKING_QUEUED                                   0x3


/********************************************************************************************************/
struct PhaseSpace_xvT_s {
    float x[3]; /* ray position  in global frame of reference */
    float v[3]; /* ray direction (versor) in global frame of reference */
    float T; /* ray kinetic energy */
};
typedef PhaseSpace_xvT_s PhaseSpace_xvT;
/********************************************************************************************************/

/* Library API */

int fredInit(const char* datadir/* path to directory containing data repository libFred.data */);
int fredGetPThreads(); /* returns number of maximum POSIX threads for parallel execution */
int fredSetPThreads(int numThreads); /* set number of maximum POSIX threads for parallel execution */

int fredRedirectOutput(const char *out1,const char *out2); /* redirect std output to stream or filname, eg. "cout", "myout.txt" or "/dev/null" */
int fredRedirectError (const char *err1,const char *err2); /* redirect std error  to stream or filname, eg. "cerr", "myerr.txt" or "/dev/null" */

int fredResetAll();
int fredResetFields();
int fredResetPencilBeams();
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

float* fredScorer_buffer(int ireg /* region index */,
                    int iscorer /* scorer index ... */,
                    int nn[3] /* dimensions */,
          int ierr /* error code */);
          /* returns a buffer filled with scorer values; user is responsible for deallocation of the pointed memory */ 

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

/* Field API */
int
fredAddField(int FID /* field (numeric) ID */);

int
fredDeleteField(int FID /* field (numeric) ID */);

int
fredSetField_FoR(int FID /* field (numeric) ID */,
              float O[3] /* field Origin in global frame of reference */,
              float f[3] /* field front vector in global frame of reference */,
              float u[3] /* field up    vector in global frame of reference */,
              float l[3] /* field left  vector in global frame of reference */);
int
fredGetField_FoR(int FID /* field (numeric) ID */,
              float O[3] /* field Origin in global frame of reference */,
              float f[3] /* field front vector in global frame of reference */,
              float u[3] /* field up    vector in global frame of reference */,
              float l[3] /* field left  vector in global frame of reference */);

/* Pencil Beam API */
int
fredAddPB(int FID  /* referenced field (numeric) ID */,
          int PBID /* pencil beam (numeric) ID */);

int
fredDeletePB(int FID  /* referenced field (numeric) ID */,
             int PBID /* pencil beam (numeric) ID */);

int
fredSetPB_FoR(int FID  /* referenced field (numeric) ID */,
              int PBID /* pencil beam (numeric) ID */,
              float O[3] /* pencil beam Origin in selected frame of reference */,
              float f[3] /* pencil beam front vector in selected frame of reference */,
              float u[3] /* pencil beam up    vector in selected frame of reference */,
              float l[3] /* pencil beam left  vector in selected frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_FIELD or FRED_FRAME_GLOBAL */);

int
fredGetPB_FoR(int FID  /* referenced field (numeric) ID */,
              int PBID /* pencil beam (numeric) ID */,
              float O[3] /* pencil beam Origin in selected frame of reference */,
              float f[3] /* pencil beam front vector in selected frame of reference */,
              float u[3] /* pencil beam up    vector in selected frame of reference */,
              float l[3] /* pencil beam left  vector in selected frame of reference */,
              int   frame/* selected frame of reference: FRED_FRAME_FIELD or FRED_FRAME_GLOBAL */);


int
fredSetPB_particle(int FID  /* referenced field (numeric) ID */,
                   int PBID /* pencil beam (numeric) ID */,
                   char *particle /* particle name, e.g. PROTON,C12,... */);

/* Ray API */
int
fredCheckRays(const char *particle /* particle name, e.g. PROTON,GEORAY,C12,... */,
              int nrays /* number of rays to be checked for internal consistency */,
              PhaseSpace_xvT *rays /* array of rays */);

int
fredAddRays(const char *particle /* particle name, e.g. PROTON,GEORAY,C12,... */,
            int nrays /* number of rays to add */,
            PhaseSpace_xvT *rays /* array of rays */);

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
fredPluginShareBuffer(int BID /* buffer ID to be shared with plugin */ ,
                      void * buff /* local (i.e. caller-side) buffer pointer */,
                      size_t count/* num of elements in buffer*/,
                      size_t size /* element size in Bytes */ );

int
fredPluginObtainBuffer(int BID /* buffer ID to be shared with plugin */ ,
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

#ifdef __cplusplus
}
#endif

#endif