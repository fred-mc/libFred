# Fred library
Fred is available as dynamic library (*libFred.so*) which can be called from other codes. The data needed by the physics models in bundled into the binary file *libFred.data*.  

# Python wrapper
* We have begun implementing a python wrapper of the Fred library. It is intended to call the Fred library functions from python.
* All *int* functions which return error codes are wrapped into *void* functions which do not return anything in python, but rather raise a python exception informing the user about the error.
* Array type inputs are wrapped as numpy arrays, but also lists are acceptable in most cases. The *numpy.i* file contains the necessary typemaps - it is part of every numpy distribution. Checks of correct input size are rudimentary for now, but this will improve in the future.
* The library is imported in python by *import FredAPI* and functions are then available as *FredAPI.foo()*.

## Compile instructions for Fred python wrapper

1. All the following steps are performed within the lib/mac directory.

2. , run *swig -c++ -python libFred.i*. This will generate the wrapper code contained in libFred_wrap.cpp.

3. Run *python setup.py build_ext --inplace*. This will compile the wrapper code and link it to the Fred library (libFred.so). It generates \_FredAPI.so, which is the dynamic python library.

4. Run *python example1.py*.

## Notes and known issues
* So far, the module is linked in place, so that *import FredAPI* only works from within the directory where the python library is stored. This will be modified in the future to install the module in a standard location.
* The compilation using the setup.py file might complain about wrong deployment target on MacOS. As a workaround, type *export MACOSX_DEPLOYMENT_TARGET=10.14* (or whatever MacOS version you are using) before running *python setup.py*.
* There is no docstring available for now, so typing *help(foo)* will not provide you with any information (yet).
