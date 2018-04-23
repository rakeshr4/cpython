from distutils.core import setup, Extension
setup(
	ext_modules = [
	Extension("gilfullList", sources=["Modules/gflobject.c"], 
							 depends = ["Modules/gflobject.h"]),
	Extension("gillessList", sources=["Modules/gllobject.c"], 
							 depends = ["Modules/gllobject.h"])])
