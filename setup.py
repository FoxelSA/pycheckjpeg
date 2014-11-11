from distutils.core import setup, Extension

setup(
    name="pycheckjpeg", 
    version="0.1",
    ext_modules = [Extension("pycheckjpeg", ["src/pycheckjpeg.c"], libraries=['jpeg'])],
    scripts=['scripts/pycheckjpeg']
    )
