from setuptools import setup, Extension
import os

rayforce_module = Extension(
    'rayforce',
    sources=['rayforce/rayforce_python.c'],
    include_dirs=['../core'],
    library_dirs=['..'],
    libraries=['rayforce'],
    extra_compile_args=['-Wall', '-O3', '-fPIC'],
    extra_link_args=['-L..', '-Wl,-rpath,..']
)

setup(
    name='rayforce',
    version='0.1',
    description='Python bindings for RayforceDB',
    ext_modules=[rayforce_module],
    author='Anton Kundenko',
    author_email='singaraiona@gmail.com',
    url='https://github.com/singaraiona/rayforce',
) 