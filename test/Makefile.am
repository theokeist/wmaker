## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = no-dependencies

AM_LDFLAGS = $(WMAKER_RUNPATH_FLAGS)

EXTRA_DIST = notest.c

noinst_PROGRAMS = wtest

wtest_SOURCES = wtest.c

wtest_LDADD = $(top_builddir)/wmlib/libWMaker.la @XLFLAGS@ @XLIBS@

AM_CPPFLAGS = -g -D_BSD_SOURCE @XCFLAGS@ -I$(top_srcdir)/wmlib
