# Read package version from version.txt
m4_define([meshborn_version], m4_esyscmd([cat version.txt | tr -d '\n']))

AC_INIT([meshborn], [meshborn_version], [meshborn@invalid.invalid])

AC_CONFIG_MACRO_DIRS([m4])

# Enable Automake
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Initialize Libtool
LT_INIT

# Check for C++ compiler
AC_PROG_CXX

# Set include and library paths (use proper variable names)
AC_SUBST(AM_CPPFLAGS, "-I. -I\$(MESHBORN_GLM)")

# Checks for header files
AC_CHECK_HEADERS([stdio.h stdlib.h])

# Output Makefile in root and subdirectories
AC_CONFIG_FILES([Makefile src/Makefile])

# Generate the configuration script
AC_OUTPUT