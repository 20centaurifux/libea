import os, glob, shutil

# installation folders:
prefix = '/usr/local'
lib_dir  = '%s/lib64' % prefix
include_dir = '%s/include/libea' % prefix

# compiler options:
cxx_flags = '-std=c++11 -Wall -O2'

# target & dependencies:
libea_src = ['src/ARandomNumberGenerator.cpp']
libea_headers = glob.glob('src/*.hpp')
libea_target = 'libea-0.2.0'

test_src = ['src/test.cpp']
test_target = 'libea-test-suite'

doxygen_config = 'doxygen_config'

# create environment:
env = Environment(tools = ['default', 'doxygen'], toolpath = ['scons-deps'])
env.Append(CXXFLAGS = cxx_flags)

# libea:
libea_build = []
libea_build += env.SharedLibrary(libea_target, libea_src)
libea_build += env.StaticLibrary(libea_target, libea_src)
env.Alias('libea', libea_build)

# test suite:
if 'test-suite' in COMMAND_LINE_TARGETS:
	test_env = env.Clone()
	test_env.Append(LIBS = ['cppunit', libea_target])
	test_build = test_env.Program(test_target, test_src)
	test_env.Alias('test-suite', test_build)

# installation:
install_targets = []
install_targets += env.Install(lib_dir, libea_build)
install_targets += env.Install(include_dir, libea_headers)

# ctags:
tags_builder = Builder(action = 'ctags $SOURCES')
env.Append(BUILDERS = { 'Tags': tags_builder })
env.Tags('tags', libea_headers + libea_src)

# doxygen:
doxygen_target = env.Doxygen('doxygen_config')
env.Alias('doc', doxygen_target)

# documentation:
pdf_build = env.PDF('Introduction.pdf', 'tex/introduction.tex')
env.Alias('pdf', pdf_build)

# installation:
env.Alias('install', install_targets)

# read user options:
if GetOption('clean'):
	# make optional targets default to let scons clean everything:
	Default(['libea', 'tags', 'pdf', 'test-suite'])

	# doxygen directory isn't deleted by scons:
	shutil.rmtree('doc', ignore_errors=True)

else:
	Default('libea')
