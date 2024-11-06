from setuptools import Extension, setup

module = Extension("symnmf", sources=['symnmf.c','symnmfmodule.c', 'fileFuncs.c', 'matrixFuncs.c'])

setup(name='symnmf',
      version='1.0',
      description='Python wrapper for custom C extension',
      ext_modules=[module])