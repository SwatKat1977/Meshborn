# src/Makefile.am

# Compiler and linker flags
AM_CPPFLAGS = -g -std=c++20 -Wall -Wextra -fPIC \
              -I.

# Install header files
nobase_include_HEADERS = Meshborn.h \
                  Logger.h \
                  wavefront/WavefrontObjParser.h

# Install headers into $(prefix)/meshborn
includedir = $(prefix)/include/meshborn

# Specify sources and output shared library
lib_LTLIBRARIES = libMeshborn.la
libMeshborn_la_SOURCES = Meshborn.cpp                         \
                         wavefront/WavefrontObjParser.cpp     \
                         wavefront/BaseWavefrontParser.cpp    \
                         wavefront/MaterialLibraryParser.cpp  \
                         Material.cpp

# Set the libtool versioning
#libWebLoom_la_LDFLAGS = -version-info $(LT_VERSION)

# Specify libraries
libMeshborn_la_LIBADD = $(LDADD)

CLEANFILES = $(lib_LTLIBRARIES)