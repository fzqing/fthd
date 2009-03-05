AUTOMAKE_OPTIONS=foreign

AM_CFLAGS = `xml2-config --cflags` `xml2-config --libs`

bin_PROGRAMS=muftad
muftad_SOURCES= MuFTAD_App.c MuFTAD_Init.c MuFTAD_main.c MuFTAD_Proc.c MuFTAD_Util.c
muftad_LDADD=../network/libmunetwork.la ../parsexml/libmuparsexml.la ../crcmd5/libmucrc.la
