#ifndef __COUNTERPAT__
#define __COUNTERPAT__

#define CP_IP "192.168.1.6"
#define CP_PORT 6667
#define CP_RC_START "#"
#define CP_RC_END "$"
#define CP_RC_CODE	"#"
#define CP_CLOSE CP_RC_START+CP_RC_CODE+"CLOSE"+CP_RC_END
#define CP_REWARD CP_RC_START+CP_RC_CODE+"REWARD"+CP_RC_END
#define CP_DIE		CP_RC_START+CP_RC_CODE+"DIE"+CP_RC_END
#define CP_UPBAG 	CP_RC_START+CP_RC_CODE+"UPDATE_BAG"+CP_RC_END
#define CP_UPWP 	CP_RC_START+CP_RC_CODE+"UPDATE_WEAPON"+CP_RC_END
#define CP_MSG		CP_RC_START+CP_RC_CODE+"MSG:"
#define CP_MSG_SEND(channel,msg) CP_RC_START+CP_RC_CODE+"MSG:"+channel+"=="+msg+CP_RC_END

#endif
