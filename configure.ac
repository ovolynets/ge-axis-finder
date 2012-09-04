#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.50)
AC_INIT([AXIS_FINDER])
AC_CONFIG_SRCDIR([main.cxx])

AC_PROG_CXX

AC_CANONICAL_HOST

case "${host_cpu}" in
  x86_64*)
  CFLAGS="$CFLAGS -fPIC"
  echo "Compiling on an x86_64 chipset, setting -fPIC flag."
  # only on amd64 should one compile static libs with this flag
  # Other tested architectures currently compile PIC libraries
  # and so it is no problem to link a dynamic library using the PIC
  # libs
esac

#Find root
echo ""
echo "${BOLD}Checking for ROOT: ${OFFBOLD} "
m4_include(root.m4)
HAVE_ROOT=""
ROOT_PATH(5.20,HAVE_ROOT="yes", AC_MSG_ERROR(ROOT not found! Please coorectly install ROOT and define ROOTSYS variable))
echo "ROOT found"
#end root

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_CONFIG_FILES([
             Makefile
             ])

AC_OUTPUT
echo
echo "*****************************************************************************************"
echo
echo "Axis finder has been successfully configured. You can now compile it by issuing \"make\" "
echo
echo "*****************************************************************************************"
echo
