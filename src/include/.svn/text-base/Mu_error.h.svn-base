#ifndef __MU_ERROR_H__
#define __MU_ERROR_H__

#include <errno.h>

#define MUOK 0
#define MUESSL (-1)
#define MUETIMEOUT (-2)
#define MUEERO (-3)
#define MUNPRT (-4)
#define MUNHST (-5)
#define MUNBUF (-6)
#define MUNSKT (-7)
#define MUECNNT (-8)
#define MUEREFUSED (-9)

#define MUEXDOC ( -10)
#define MUEXNOD (-11)
#define MUEXCNT (-12)
#define MUNNOD (-13)               //no node
#define MUEXSIN (-14)		
#define MUEXADP (-15)
#define MUEX2XX (-16)		//2xx error
#define MUEX4XX (-17)		//4XX error

#define MUEFSY (-18)
#define MUENSP (-19)
#define MUNXML (-20)

#define MU_DNCOMP 0
#define MUNPAT (-103)
#define MUNLEN (-104)
#define MUNLVR (-105)
#define MUNCOK (-106)
#define MUNPOS (-107)

#define MUEBUL (-108)
#define MUEAUH (-109)
#define MUESND (-110)
#define MUERLC (-111)
#define MUEDEL (-112)
#define MUNCMP (-113)
#define MUERED (-114)
#define MUEREC (-115)
#define MUEURL (-116)
#define MUEMEH (-117)
#define MUEEOF (-118)
#define MUEPRO (-119)
#define MUNFIL (-120)


#define MUNPRO (-200)
#define MUEPST (-201)
#define MUECON (-202)
#define MUEQUE (-203)

#define MUFLEN (-300)
#define MUCNOM (-301)
#define MUFRAD (-302)

#define MUEBLD (-400)
#define MUEDNC (-401)
#define MUETYP (-402)

#define MUNAPI (-404)
#define MUEDWN (-405)
#define MUEXML (-406)
#define MUECKS (-407)

#define Mu_ErrorPrint() \
	do{\
		fprintf(stdout, "In [%s] the [%d] line error, include in file [%s]\n", \
			__func__, __LINE__, __FILE__);\
	}while(0)
#endif
