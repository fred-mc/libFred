import os
import numpy as np
import matplotlib.pyplot as plt
import FredAPI


def ensure_dir(dirname):
    if not os.path.exists(dirname):
        os.mkdir(dirname)

# =================================

FredAPI.Init("./")

PThreads = FredAPI.GetPThreads()
print("Found {0:d} avaiable posix threads.".format(PThreads))

FredAPI.SetPThreads(1)
FredAPI.RedirectOutput("cout", "out/FredAPI.out")
FredAPI.RedirectError("cout", "out/FredAPI.err")


regionID = "Phantom"
FredAPI.AddRegion(regionID)

regionIndex = FredAPI.Region_index(regionID)
print("Region index is {0:d}.".format(regionIndex))

FredAPI.Region_info(regionIndex)

numberOfRegions = FredAPI.NumRegions()
print("Found {0:d} regions.".format(numberOfRegions))


FredAPI.SetRegion_extent(regionIndex, [4., 4., 10.])
FredAPI.SetRegion_origin(regionIndex, [0., 0., 0.])
FredAPI.SetRegion_pivot(regionIndex, [0.5, 0.5, 0.])
FredAPI.SetRegion_voxels(regionIndex, [41, 41, 400])

materialIndexWater = FredAPI.Material_index("water")
FredAPI.Material_info(materialIndexWater)

FredAPI.SetRegion_material(regionIndex,materialIndexWater)

scorerIndex = FredAPI.AddScorer(regionIndex,FredAPI.RegionScorers.eDepScorer)

FredAPI.Region_info(regionIndex)



nprim = 1e4
ray = [0., 0., -20., 0., 0., 1., 100.]
PhaseSpace = np.repeat(np.atleast_2d(np.array(ray, dtype=np.float32)), nprim, axis=0)
if not FredAPI.CheckRays(PhaseSpace, "proton"):
    print("Rays checked: OK. ")
FredAPI.AddRays(PhaseSpace, "proton")


numberOfRays = FredAPI.GetNumRays()
print("The simulation currently contains {0:d} rays.".format(numberOfRays))

FredAPI.ActivateEloss(True)
FredAPI.ActivateFluc(True)
FredAPI.ActivateMCS(True)
FredAPI.ActivateNuclearElastic(True)
FredAPI.ActivateNuclearInelastic(True)
FredAPI.ActivateNuclear(True)

FredAPI.TrackRays(0, -1)

nVoxels = FredAPI.GetRegion_voxels(regionIndex)
buffer1D = FredAPI.Scorer_buffer(regionIndex, scorerIndex, np.prod(nVoxels))
buffer3D = buffer1D.reshape(nVoxels[::-1])

ensure_dir("out")
FredAPI.Scorer_save(regionIndex,scorerIndex,"out/myscorer");


# FredAPI.Close()  # not implemented in library yet.

plt.figure()
plt.imshow(np.sum(buffer3D, axis=1))
plt.show()
