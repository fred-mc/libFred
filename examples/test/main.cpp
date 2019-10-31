#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "libFred.h"

int main(int argc, char *argv[]){
cout<<"Hello from libFred"<<endl;

// redirecting output and error streams
// available choices: cout e cerr (standard output and error streams)
//                   null o /dev/null to discard completely the output/error
//                   pathname to pipe output/error to a file
fredRedirectOutput("cout","out/fred.out");
fredRedirectError("cout","out/fred.err");


// Fred Library initialization
fredInit("./");
// set number of POSIX threads for parallel execution (default = 1, i.e. serial execution)
fredSetPThreads(12);
// cout<<fredGetPThreads()<<endl;



// default setup is just an empty room => add the phantom region
int iphantom = fredAddRegion("Phantom");

// customize phantom: extent, origin, material, etc
float L[3]={4,4,10};
fredSetRegion_extent(iphantom,L);

float O[3]={0,0,0};
fredGetRegion_origin(iphantom,O);

float pivot[3]={0.5,0.5,0};
fredSetRegion_pivot(iphantom,pivot);

int nn[3]={41,41,400};
fredSetRegion_voxels(iphantom,nn);

// get material index for standard liquid water
int iwater = fredMaterial_index("water");
//fredMaterial_info(iwater);

// set phantom to be a homogeneous water box
fredSetRegion_material(iphantom,iwater);

// add a scorer to phantom region
int iScorer = fredAddScorer(iphantom,eDepScorer);

// output phantom information
fredRegion_info(iphantom);



// generate primary particles

int nprim = 1e4;

vector<PhaseSpace_xvT> rays(nprim);
for(int ir=0;ir<rays.size();ir++){
	// position x,y,z in cm
	rays[ir].x[0]=0; 
	rays[ir].x[1]=0.;
	rays[ir].x[2]=-20;
	
	// direction vx,vy,vz (it is internally normalized to a unit vector)
	rays[ir].v[0]=0;
	rays[ir].v[1]=0;
	rays[ir].v[2]=1;
	
	// kinetic energy of the particle in MeV
	rays[ir].T=100;
}

// check that primary particles are well defined 
if(fredCheckRays("proton",rays.size(),rays.data())==0){
	// OK: add particles to the tracking stack
	fredAddRays("proton",rays.size(),rays.data());
}


cout<<"num rays to track: "<<fredGetNumRays()<<endl;

// switch on/off the transport packages (default = everything on)
// fredActivateEloss(0);
// fredActivateMCS(0);
// fredActivateFluc(0);
// fredActivateNuclearElastic(0);
// fredActivateNuclearInelastic(0);


// track particles 
fredTrackRays(0,-1); // 0 : -1 = all particles
//fredTrackRays(5,13); // 5 : 13 = particles from 5 to 13 inclusive


// save scorer map to file
fredScorer_save(iphantom,iScorer,"out/myscorer");

// end of then game
cout<<"Aufwiedersehen!"<<endl;
return 0;
}}}}
