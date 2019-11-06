# Fred library
Fred is available as dynamic library (*libFred.so*) which can be called from other codes. The data needed by the physics models in bundled into the binary file *libFred.data*.  

# Python wrapper: FredAPI
* We have begun implementing a python wrapper of the Fred library. It is intended to call the Fred library functions from python.
* All *int* functions which return error codes are wrapped into *void* functions which do not return anything in python, but rather raise a python exception informing the user about the error.
* Array type inputs are wrapped as numpy arrays, but also lists are acceptable in most cases. The *numpy.i* file contains the necessary typemaps - it is provided as part of every numpy distribution. Checks of correct input size are rudimentary for now, but this will improve in the future.
* The library is imported in python by *import FredAPI* and functions are then available as *FredAPI.foo()*.

## Compile instructions for Fred python wrapper

1. All the following steps are performed within the lib/mac directory.
2. Run *swig -c++ -python libFred.i*. This will generate the wrapper code contained in libFred_wrap.cpp.
3. Run *python setup.py build_ext --inplace*. This will compile the wrapper code and link it to the Fred library (libFred.so). It generates \_FredAPI.so, which is the dynamic python library.
4. Run *python example1.py*.

## Notes and known issues

* So far, the module is linked in place, so that *import FredAPI* only works from within the directory where the python library is stored. This will be modified in the future to install the module in a standard location.
* The compilation using the setup.py file might complain about wrong deployment target on MacOS. As a workaround, type *export MACOSX_DEPLOYMENT_TARGET=10.14* (or whatever MacOS version you are using) before running *python setup.py*.
* There is no docstring available for now, so typing *help(foo)* will not provide you with any information (yet).

# User interface: FredUI.py
* This is an initial idea how to provide a higher level user interface written in python directly.
* Rationale: For a python user, it is much more intuitive to work with objects. For example, a region would be an object with attributes such as offset, origin etc.
* Methods call the FredAPI, e.g. to get the regions index, offset and so on.
* This also allows us to implement beams as classes. The current *FredUI.py* contains an example of a pencil beam class. One can either implement a fancy pencil beam model inside the class with a lot of options or let the user define a function which generates rays and pass the function (which is an object in python) to the class. To be discussed.
* To try the FredUI.py, look at and run *example2.py*.
