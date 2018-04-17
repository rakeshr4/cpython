from distutils.core import setup, Extension
setup(
	ext_modules = [
	Extension("gillessList", sources=["gillessList.c"]),],)