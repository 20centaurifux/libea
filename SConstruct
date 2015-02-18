import os, glob, shutil, sys

# 64bit check:
def check_64bit():
	return sys.maxsize > 2**32

# installation folders:
prefix = '/usr/local'

if check_64bit():
	lib_dir  = '%s/lib64' % prefix
else:
	lib_dir  = '%s/lib' % prefix

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

lib_targets = env.Install(lib_dir, libea_build)
install_targets += lib_targets

header_targets = env.Install(include_dir, libea_headers)
install_targets += header_targets

for t in lib_targets:
	env.AddPostAction(t, Chmod(str(t), 0555))

for t in header_targets:
	env.AddPostAction(t, Chmod(str(t), 0444))

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
os.umask(022)

env.Alias('install', install_targets)

# read user options:
if GetOption('clean'):
	# make optional targets default to let scons clean everything:
	Default(['libea', 'tags', 'pdf', 'test-suite'])

	# doxygen directory isn't deleted by scons:
	shutil.rmtree('doc', ignore_errors = True)

	# remove test suite:
	if(os.path.exists(test_target)):
		os.unlink(test_target)
else:
	Default('libea')
