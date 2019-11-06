import os
import sys
import numpy as np
if "FredAPI" not in sys.modules:
    import FredAPI
import matplotlib.pyplot as plt


def ensure_dir(dirname):
    if not os.path.exists(dirname):
        os.mkdir(dirname)

# =================================

__initializedFred__ = False

class Simulation(object):
    """Container class to hold the components of a simulation.
    """
    defaultPhysicsSwitches = {"EnergyLoss":True, "EnergyStraggling":True, \
                                "MCS":True,  \
                                "NuclearElastic":True, "NuclearInelastic":True}

    def __init__(self, PosixThreads=1, OutputRedirection=None, datadir="./", physicsSwitches={}):
        global __initializedFred__
        self.regions = []
        self.materials = []
        self.fields = []
        self.rays = None
        self.PosixThreads = PosixThreads
        self.OutputRedirection = None
        if OutputRedirection:
            self.SetOutputRedirection(OutputRedirection)
        self.physicsSwitches = {}
        self.SetPhysics(physicsSwitches)
        if not __initializedFred__:
            FredAPI.Init(datadir)
            __initializedFred__ = True
        else:
            raise Exception("You can only create one simulation per runtime. ")

    def SetPhysics(self, physicsSwitches):
        for k in physicsSwitches.keys():
            if not k in self.defaultPhysicsSwitches.keys():
                raise Exception("The key {0} provided is not known".format(k))

        for k in self.defaultPhysicsSwitches.keys():
            try:
                self.physicsSwitches[k] = physicsSwitches[k]
            except KeyError:
                self.physicsSwitches[k] = self.defaultPhysicsSwitches[k]

        FredAPI.ActivateEloss(self.physicsSwitches["EnergyLoss"])
        FredAPI.ActivateFluc(self.physicsSwitches["EnergyStraggling"])
        FredAPI.ActivateMCS(self.physicsSwitches["MCS"])
        FredAPI.ActivateNuclearElastic(self.physicsSwitches["NuclearElastic"])
        FredAPI.ActivateNuclearInelastic(self.physicsSwitches["NuclearInelastic"])

    def Run(self, firstRay=0, lastRay=-1):
        FredAPI.TrackRays(firstRay, lastRay)

    def SetOutputRedirection(self, OutputRedirection):
        if isinstance(OutputRedirection, tuple):
            if len(OutputRedirection) == 2:
                FredAPI.RedirectOutput(OutputRedirection[0], OutputRedirection[1])
                self.OutputRedirection = OutputRedirection
            else:
                raise Exception("OutputRedirection must be a tuple of TWO elements, such as ('cout', 'myout.out')")
        else:
            raise Exception("OutputRedirection must be a tuple of TWO elements, such as ('cout', 'myout.out')")

    def GetOutputRedirection(self, OutputRedirection):
        return self.OutputRedirection

    def SetErrorRedirection(self, ErrorRedirection):
        if isinstance(ErrorRedirection, tuple):
            if len(ErrorRedirection) == 2:
                FredAPI.RedirectError(ErrorRedirection[0], ErrorRedirection[1])
                self.ErrorRedirection = ErrorRedirection
            else:
                raise Exception("ErrorRedirection must be a tuple of TWO elements, such as ('cout', 'myout.out')")
        else:
            raise Exception("ErrorRedirection must be a tuple of TWO elements, such as ('cout', 'myout.out')")

    def GetErrorRedirection(self, ErrorRedirection):
        return self.ErrorRedirection

    def AddRegion(self, region):
        if region.IsReadyToAdd():
            FredAPI.AddRegion(region.regionID)
            region.regionIndex = FredAPI.Region_index(region.regionID)
            print("Region index is {0:d}.".format(region.regionIndex))
            FredAPI.SetRegion_extent(region.regionIndex, region.extent)
            FredAPI.SetRegion_origin(region.regionIndex, region.origin)
            FredAPI.SetRegion_pivot(region.regionIndex, region.pivot)
            FredAPI.SetRegion_voxels(region.regionIndex, region.voxels)
            if region.material is not None:
                materialIndex = FredAPI.Material_index(region.material)
                FredAPI.SetRegion_material(region.regionIndex, materialIndex)
            for k in region.scorers.keys():
                if region.scorers[k] is None:
                    scorerCode = getattr(FredAPI.RegionScorers, k)
                    region.scorers[k] = FredAPI.AddScorer(region.regionIndex, scorerCode)
            region.hasBeenAdded = True
            region.simulation = self
            self.regions.append(region)
        else:
            raise Exception("Region {0} is not ready to be added to simulation.\
                            Check if all parameters have been \
                            specified.".format(region.regionID))

    def RemoveRegion(self, region):
        raise Exception("This function is not yet implemented in the library")
        # FredAPI.DeleteRegion(region.regionID)
        # self.regions.remove(region)

    def GetNumberOfRegions(self):
        numberOfRegions = FredAPI.NumRegions()
        if not numberOfRegions == len(self.regions):
            raise Exception("Something is wrong: the number of regions stored \
                             in the simulation object and the number known to \
                             the Fred library do not match. ")
        else:
            return numberOfRegions

    def AddBeam(self, beam):
        FredAPI.AddRays(np.asarray(beam.rays).astype(np.float32), beam.particle)



class Region(object):
    """Container class to hold a region.
    Provides methods for setting and getting its properties.
    Is aware of being part of a simulation or not.
    """

    extent = None
    origin = None
    pivot = [0.5, 0.5, 0.5]
    voxels = None
    material = None
    scorers = {}

    def __init__(self, regionID, extent=None, origin=None, pivot=None, voxels=None, material=None):
        if not isinstance(regionID, str):
            raise Exception("The ID must be a string type object.")
        else:
            self.regionID = regionID
            self.regionIndex = None
            self.hasBeenAdded = False
            self.simulation = None
        if extent is not None:
            self.SetExtent(extent)
        if origin is not None:
            self.SetOrigin(origin)
        if pivot is not None:
            self.SetPivot(pivot)
        if voxels is not None:
            self.SetVoxels(voxels)
        if material is not None:
            self.SetMaterial(material)

    # def __setattr__(self, name, value):
    #     """The __setattr__ method is implemented to redirect to the
    #     Set-methods for certain attributes, e.g. to check whether the attribute
    #     may actually be changed or not, and to enforce further checks,
    #     like size, dimension etc.
    #     """
    #     if name=='regionID':
    #         self.SetRegionID(value)
    #     elif name=='extent':
    #         self.SetExtent(value)
    #     elif name=='origin':
    #         self.SetOrigin(value)
    #     elif name=='pivot':
    #         self.SetPivot(value)
    #     elif name=='voxels':
    #         self.SetVoxels(value)
    #     else:
    #         self.__dict__[name] = value

    def IsAdded(self):
        """Check whether this region is currently added to the simulation.
        """
        if (self.hasBeenAdded is True) and (self.simulation is not None):
            return True
        elif (self.hasBeenAdded is False) and (self.simulation is not None):
            raise Exception("Region is flagged as not being added, but simulation attribute is not None.")
        elif (self.hasBeenAdded is True) and (self.simulation is None):
            raise Exception("Region is flagged as being added, but simulation attribute is not set.")
        else:
            return False

    def IsReadyToAdd(self):
        """Check if the region object has all the necessary parameters set to be added to a simulation.
        """
        if (self.extent is not None) and (self.origin is not None) and (self.voxels is not None):
            return True
        else:
            return False

    def SetRegionID(self, regionID):
        """Set the regionID attribute if the region is not already added to the simulation.
        Raise exception otherwise to avoid inconsistencies.
        """
        if self.IsAdded() is True:
            raise Exception("You are not allowed to change the regionID because the region has been added to the simulation.")
        else:
            self.regionID = regionID

    def SetExtent(self, extent):
        """Set the region's extent attribute.
        Make the change known to the simulation if the region has already been added to the simulation.
        """
        if self.IsAdded() is True:
            FredAPI.SetRegion_extent(self.regionIndex, extent)
        self.extent = extent

    def GetExtent(self):
        return self.extent

    def SetOrigin(self, origin):
        if self.IsAdded() is True:
            FredAPI.SetRegion_origin(self.regionIndex, origin)
        self.origin = origin

    def GetOrigin(self):
        return self.origin

    def SetPivot(self, pivot):
        if self.IsAdded() is True:
            FredAPI.SetRegion_pivot(self.regionIndex, pivot)
        self.pivot = pivot

    def GetPivot(self):
        return self.pivot

    def SetVoxels(self, voxels):
        if self.IsAdded() is True:
            FredAPI.SetRegion_voxels(self.regionIndex, voxels)
        self.voxels = voxels

    def GetVoxels(self):
        return self.voxels

    def SetMaterial(self, material):
        materialIndex = FredAPI.Material_index(material)
        if self.IsAdded() is True:
            FredAPI.SetRegion_material(self.regionIndex, materialIndex)
        self.material = material
        # else:
        #     raise Exception("Currently, a material can be set only \
        #                      after it has been added to the simulation. ")

    def GetMaterial(self):
        return self.material

    def AddScorer(self, scorerName):
        try:
            scorerCode = getattr(FredAPI.RegionScorers, scorerName)
        except AttributeError:
            raise Exception("The scorerName is unknown.")

        if scorerName in self.scorers.keys():
            raise Exception("This scorer has already been added to the region.")

        if self.IsAdded() is True:
            scorerIndex = FredAPI.AddScorer(self.regionIndex, scorerCode)
        else:
            scorerIndex = None
        self.scorers[scorerName] = scorerIndex
        # else:
        #     raise Exception("Currently, a scorer can be added to a region only \
        #                      after it has been added to the simulation. ")

    def GetScorerBuffer(self, scorerName):
        buffer1D = FredAPI.Scorer_buffer(self.regionIndex, self.scorers[scorerName], np.prod(self.voxels))
        return buffer1D.reshape(self.voxels[::-1])

    def SaveScorerBuffer(self, scorerName, pathname):
        FredAPI.Scorer_save(self.regionIndex, self.scorers[scorerName], pathname);


# class Beam(object):
#     """Container class to hold rays which form one beam.
#     Specific beams such as pencil beams are created by inheritance.
#     """



# class PencilBeam(Beam):
class PencilBeam(object):
    """Convenience class to construct and manipulate pencil beams with Fred."""

    def __init__(self, numberOfRays, particle, rayGenerator=None, generatorArguments=None):
        self.numberOfRays = int(numberOfRays)
        self.particle = particle
        self.hasBeenAdded = False
        if rayGenerator is None:
            self.GenerateRays = self.ParallelPencilBeam
        else:
            self.GenerateRays = rayGenerator
        self.rays = self.GenerateRays(self.numberOfRays, generatorArguments)

    def CheckRays():
        FredAPI.CheckRays(self.rays, self.particle)

    def ParallelPencilBeam(self, numberOfRays, *args):
        """Generate a simple parallel pencil beam.
        Everything is hardcoded and serves to illustrate the concept.
        """

        xyPosition = np.random.randn(2, numberOfRays) * 0.4
        zPosition = np.ones(numberOfRays) * (-20)
        xDirection = np.zeros(numberOfRays)
        yDirection = np.zeros(numberOfRays)
        zDirection = np.ones(numberOfRays)
        energy = np.ones(numberOfRays) * 100.
        rays = np.vstack((xyPosition, zPosition, xDirection, yDirection, zDirection, energy)).T
        return rays
