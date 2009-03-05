AUTOMAKE_OPTIONS=foreign

#AM_CFLAGS = -I/usr/include/libxml2 -L/usr/lib -lxml2 -lz -lm
AM_CFLAGS = `xml2-config --cflags` `xml2-config --libs`
lib_LTLIBRARIES=libmuparsexml.la
libmuparsexml_la_SOURCES= Mu_createxml.c Mu_Parsexml.c Mu_Struct.c
