# libea v0.2 - README

## Introduction

***libea*** is a template based library written in C++14. The purpose
of this software is to provide an extensible and reliable framework
for writing evolutionary algorithms.


## Building libea

To build ***libea*** scons is required. The library can be compiled and
installed with the following commands:

	# scons libea
	# sudo scons install

After installation you may want to build the test suite. Please note
that this step requires the CppUnit framework:

	# scons test-suite

If you want to generate the source code documentation please ensure
Doxygen is installed on your system and type in the following command:

	# scons doc


## Documentation

The *Introduction.pdf* document offers a brief overview and explains
how you can write evolutionary algorithms with ***libea***.
