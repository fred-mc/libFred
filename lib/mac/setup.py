

"""
setup.py file for libFred SWIG wrapper
"""

from distutils.core import setup, Extension
import numpy

libFred_module = Extension('_FredAPI',
                           sources=['libFred_wrap.cxx'],
                           libraries=['Fred'],
                           library_dirs=['./'],
                           extra_compile_args=["-stdlib=libc++"],
                           extra_link_args=["-stdlib=libc++"],
                           include_dirs=[numpy.get_include()],
                           )

setup (name = 'FredAPI',
       version = '0.1',
       author      = "N. Krah",
       description = """Taken from simple swig example from docs""",
       # ext_modules=[Extension('_FredAPI', ['libFred.i'],
       #                       swig_opts=['-c++'])],
       ext_modules = [libFred_module],
       py_modules = ["FredAPI"],
       )
