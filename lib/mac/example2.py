import os
import numpy as np
import matplotlib.pyplot as plt
import FredUI as Fred

def ensure_dir(dirname):
    if not os.path.exists(dirname):
        os.mkdir(dirname)

# =================================

# Create a simulation
simulation = Fred.Simulation()

# Redirect output if desired
simulation.SetOutputRedirection(("cout", "out/Fred.out"))
simulation.SetErrorRedirection(("cout", "out/Fred.err"))

# Create region object with geometry parameters and material specification
# Can also be left None and set later
region_phantom = Fred.Region("phantom", extent=[4., 4., 10.], \
                        origin=[0., 0., 0.], pivot=[0.5, 0.5, 0.], \
                        voxels=[41, 41, 400], material="water")
# Add a scorer to the region
region_phantom.AddScorer("eDepScorer")
# Add the region to the simulation
simulation.AddRegion(region_phantom)
# can also add another scorer after the region has been added
region_phantom.AddScorer("countsScorer")

# generate a simple pencil beam.
# Implementation is currently hard-coded and will be refined.
nprim = 1e3
PB1 = Fred.PencilBeam(nprim, "proton")
simulation.AddBeam(PB1)

# Run the simulation
simulation.Run()

# Save the region's buffers to disk
region_phantom.SaveScorerBuffer("eDepScorer", "out/example2_eDepScorer")
region_phantom.SaveScorerBuffer("countsScorer", "out/example2_countsScorer")

# Get the region's buffers as numpy arrays
buffer_edep = region_phantom.GetScorerBuffer("eDepScorer")
buffer_counts = region_phantom.GetScorerBuffer("countsScorer")

# Plot the result
fig, axarr = plt.subplots(ncols=2, nrows=1)
axarr[0].imshow(np.sum(buffer_edep, axis=1))
axarr[0].set_title("Energy deposit")
axarr[1].imshow(np.sum(buffer_counts, axis=1))
axarr[1].set_title("Counts")
plt.show()
