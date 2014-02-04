AUTOMAKE_OPTIONS        = foreign
ACLOCAL_AMFLAGS         = -I autotools
AM_MAKEFLAGS            = -s

pkglib_LTLIBRARIES	= logserv.la
logserv_la_SOURCES	= logserv.c logprocessing.c help.c logdefault.c logeggdrop.c logmirc.c logxchat.c
noinst_HEADERS		= logdefault.h logmirc.h logxchat.h logeggdrop.h logserv.h              

logserv_la_LDFLAGS	= -module -avoid-version

EXTRA_DIST		= autotools/ccdv.c autotools/shtool RELNOTES 

dist_doc_DATA		= README.LogServ OnisDoLogs.sh onisconfig


include $(top_srcdir)/autotools/rules.mk
