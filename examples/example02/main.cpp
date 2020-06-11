#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

#include "libFred.h"

void printFoR(float O[3],float f[3],float u[3],float l[3]){
	cout<<"O=["<<O[0]<<' '<<O[1]<<' '<<O[2]<<"] "<<endl;
	cout<<"f=["<<f[0]<<' '<<f[1]<<' '<<f[2]<<"] "<<endl;
	cout<<"u=["<<u[0]<<' '<<u[1]<<' '<<u[2]<<"] "<<endl;
	cout<<"l=["<<l[0]<<' '<<l[1]<<' '<<l[2]<<"] "<<endl;
	cout<<endl;
}

void setVec(float v[3],float a,float b,float c){v[0]=a;v[1]=b;v[2]=c;}
void setVec(int v[3],int a,int b,int c){v[0]=a;v[1]=b;v[2]=c;}

int main(int argc, char *argv[]){
cout<<"Hello from libFred"<<endl;

// redirecting output and error streams
// available choices: cout e cerr (standard output and error streams)
//                   null o /dev/null to discard completely the output/error
//                   pathname to pipe output/error to a file
fredRedirectOutput("cout","out/fred.out");
fredRedirectError("cout","out/fred.err");


// check that env var LIBFREDDIR is set
if(getenv("LIBFREDDIR")==nullptr) {cerr<<"Error: LIBFREDDIR is not set"<<endl;exit(1);}

// Fred Library initialization
if(fredInit(getenv("LIBFREDDIR"))) return -1;

// exit(0);
// set number of POSIX threads for parallel execution (default = 1, i.e. serial execution)
fredSetPThreads(12);
// cout<<fredGetPThreads()<<endl;



// default setup is just an empty room => add the phantom region
int iphantom = fredAddRegion("Phantom");

// customize phantom: extent, origin, material, etc
float L[3]={20,20,20};
fredSetRegion_extent(iphantom,L);

float O[3]={0,0,0};
fredGetRegion_origin(iphantom,O);

float pivot[3]={0.5,0.5,0.5};
fredSetRegion_pivot(iphantom,pivot);

int nn[3]={41,41,41};
fredSetRegion_voxels(iphantom,nn); 

// get material index for standard liquid water
int iwater = fredMaterial_index("water");
//fredMaterial_info(iwater);

// set phantom to be a homogeneous water box
fredSetRegion_material(iphantom,iwater);

// add a scorer to phantom region
int iScorer = fredAddScorer(iphantom,eDepScorer);


int icube = fredAddRegion("Cube");
setVec(L,5,5,5);
setVec(pivot,0.5,0.5,0.5);
setVec(nn,30,30,30);
fredSetRegion_extent(icube,L);
fredSetRegion_pivot(icube,pivot);
fredSetRegion_voxels(icube,nn); 
int imat = fredMaterial_index("pmma");
fredSetRegion_material(icube,imat);
int iCubeScorer = fredAddScorer(icube,eDepScorer);
fredSetRegion_parent(icube,iphantom);

float alpha = 0.0175*30;
float f[3],l[3],u[3];

fredGetRegion_FoR(icube,O,f,u,l,0);
setVec(f,0,sin(alpha),cos(alpha));
setVec(l,1,0,0);
// setVec(O,0,0,200);
fredSetRegion_FoR(icube,O,f,NULL,l,0);

// cout<<icube<<endl;
// exit(0);
// int ireg = fredAddRegion("pip");
// fredSetRegion_parent(ireg,icube);
// ireg = fredAddRegion("pipa");
// ireg = fredAddRegion("pipaaa");
// fredSetRegion_parent(ireg,iphantom);

// cout<<"empty scoring"<<endl;
// fredScorer_save(iphantom,iScorer,"out/myscorer");
// exit(0);

// output phantom information
fredRegion_info(iphantom);
fredRegion_info(icube);

// fredSetRegion_active(iphantom,1);
// exit(0);


int ibeam = fredAddBeam();
// ibeam = fredAddBeam();



// cout<<ibeam<<endl;
// cout<<fredAddBeam()<<endl;
// cout<<fredAddBeam()<<endl;
// cout<<fredAddBeam()<<endl;
// // printFoR(O,f,u,l);
// cout<<"fredGetBeam_FoR "<<fredGetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
// printFoR(O,f,u,l);
// // setVec(O,1,2,5);
// setVec(f,3,3,3);
// setVec(u,3,3,-6);
// cout<<"fredSetBeam_FoR "<<fredSetBeam_FoR(ibeam,O,f,u,NULL,0)<<endl;
// cout<<"fredGetBeam_FoR "<<fredGetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
// printFoR(O,f,u,l);
// cout<<"fredSetBeam_FoR "<<fredSetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
// cout<<"fredGetBeam_FoR "<<fredGetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
// printFoR(O,f,u,l);

// setVec(O,5,5,5);
// setVec(f, 1,1,0);
// setVec(u,-1,1,0);
// cout<<fredSetRegion_FoR(iphantom,O,f,u,NULL,0)<<endl;
// fredRegion_info(iphantom);
// // cout<<"fredSetBeam_FoR "<<fredSetBeam_FoR(ibeam,O,f,u,l,iphantom)<<endl;
// // cout<<"fredGetBeam_FoR "<<fredGetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
// // printFoR(O,f,u,l);
// cout<<"fredGetRegion_FoR "<<fredGetRegion_FoR(iphantom,O,f,u,l,0)<<endl;
// printFoR(O,f,u,l);
// exit(0);
setVec(O,0,0,0);
setVec(f,0,0,1);
// setVec(u,0,1,0);
setVec(l,1,0,0);
cout<<"fredSetBeam_FoR "<<fredSetBeam_FoR(ibeam,O,f,NULL,l,0)<<endl;
cout<<"fredGetBeam_FoR "<<fredGetBeam_FoR(ibeam,O,f,u,l,0)<<endl;
printFoR(O,f,u,l);

// exit(0);

// generate primary particles

int nprim = 1e3;

vector<PhaseSpace_xvT> rays(nprim);
for(int ir=0;ir<rays.size();ir++){
	// position x,y,z in cm
	rays[ir].x[0]=0; 
	rays[ir].x[1]=0.;
	rays[ir].x[2]=0;
	
	// direction vx,vy,vz (it is internally normalized to a unit vector)
	rays[ir].v[0]=0;
	rays[ir].v[1]=0;
	rays[ir].v[2]=1;
	
	// kinetic energy of the particle in MeV
	rays[ir].T=110;
}

// check that primary particles are well defined 
if(fredCheckRays(PROTON_ID,rays.size(),rays.data())==0){
	// OK: add particles to the tracking stack
	// fredAddRays(PROTON_ID,rays.size(),rays.data());
	fredAddBeam_rays(ibeam,PROTON_ID,rays.size(),rays.data());

}


cout<<"num rays to track: "<<fredGetNumRays()<<endl;
cout<<"num rays to track beam : "<<fredGetBeam_numRays(ibeam)<<endl;

// switch on/off the transport packages (default = everything on)
// fredActivateEloss(0);
// fredActivateMCS(0);
// fredActivateFluc(0);
// fredActivateNuclearElastic(0);
// fredActivateNuclearInelastic(0);


// track particles 
// fredTrackRays(0,-1); // 0 : -1 = all particles
// fredTrackRays(0,-1); // 0 : -1 = all particles
// fredTrackRays(0,-1); // 0 : -1 = all particles
// fredTrackRays(0,-1); // 0 : -1 = all particles
// fredTrackRays(0,-1); // 0 : -1 = all particles
// fredTrackRays(0,-1); // 0 : -1 = all particles
//fredTrackRays(5,13); // 5 : 13 = particles from 5 to 13 inclusive
fredGetBeam_FoR(ibeam,O,f,u,l,0);
int Npos = 12;
for(int n=0;n<Npos;n++){
	alpha = 2*M_PI/Npos*n;
	float r=15;
	// setVec(O,0,2.0/Npos*n,-10);
	cout<<"alpha "<<alpha<<endl;
	setVec(f,0,sin(alpha),cos(alpha));
	setVec(O,0,-20*sin(alpha),-20*cos(alpha));
	// setVec(u,0,1,0);
	setVec(l,1,0,0);
	cout<<fredSetBeam_FoR(ibeam,O,f,NULL,l,0)<<endl;
	fredGetBeam_FoR(ibeam,O,f,u,l,0);
	printFoR(O,f,u,l);
	// fredSetRegion_active(iphantom,n%2);
	// setVec(O,2.*n/(Npos-1),0,0);
	// fredSetRegion_origin(iphantom,O);
	fredSetRegion_active(icube,n%2);

	fredTrackBeams(0,-1); // 0 : -1 = all particles
	// break;
}

// exit(0);
// fredTrackBeams(0,-1); // 0 : -1 = all particles
// fredTrackBeams(0,-1); // 0 : -1 = all particles
// fredTrackBeams(0,-1); // 0 : -1 = all particles
// fredTrackBeams(0,-1); // 0 : -1 = all particles
// fredTrackBeams(0,-1); // 0 : -1 = all particles

fredScorer_evaluate(iphantom,iScorer);

// save scorer map to file
fredScorer_save(iphantom,iScorer,"phantom");

fredScorer_evaluate(icube,iCubeScorer);
fredScorer_save(icube,iCubeScorer,"cube");


fredGetRegion_voxels(iphantom,nn); 

size_t N = 1UL*nn[0]*nn[1]*nn[2];
float buff[N];
int ierr = fredGetScorer_buffer(iphantom,iScorer,buff);

double sum=0;
for(int i=0;i<1UL*nn[0]*nn[1]*nn[2];i++) sum+=buff[i];

cout<<"Scorer buffer sum: "<<sum<<endl;

// end of then game
cout<<"Aufwiedersehen!"<<endl;
return 0;
}