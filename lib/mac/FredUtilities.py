import os
import numpy as np
import FredAPI
import matplotlib.pyplot as plt


def ensure_dir(dirname):
    if not os.path.exists(dirname):
        os.mkdir(dirname)

# =================================

class PencilBeam():
    """Convenience class to construct and manipulate pencil beams with Fred."""

    def __init__(self, numberOfRays, particle, rayGenerator=None, generatorArguments=None):
        self.numberOfRays = int(numberOfRays)
        self.particle = particle
        self.hasBeenAdded = False
        if rayGenerator is None:
            self.generateRays = self.ParallelPencilBeam
        self.rays = self.generateRays(self.numberOfRays, generatorArguments).astype(np.float32)

    def ParallelPencilBeam(self, numberOfRays, *args):
        """Generate a simple parallel pencil beam.
        Everything is hardcoded and serves to illustrate the concept.
        """
        print(numberOfRays)
        xyPosition = np.random.randn(2, numberOfRays) * 0.4
        zPosition = np.ones(numberOfRays) * (-20)
        xDirection = np.zeros(numberOfRays)
        yDirection = np.zeros(numberOfRays)
        zDirection = np.ones(numberOfRays)
        energy = np.ones(numberOfRays) * 100.
        rays = np.vstack((xyPosition, zPosition, xDirection, yDirection, zDirection, energy)).T
        return rays

    def AddToSimulation(self):
        FredAPI.AddRays(self.rays, self.particle)

    def Check():
        FredAPI.CheckRays(self.rays, self.particle)
