from distutils.core import setup, Extension
setup(
	ext_modules = [
	Extension("gillessList", sources=["Modules/gllobject.c"], depends = ["Modules/gllobject.h"])],)
