#ifndef __MUDLIB__
#define __MUDLIB__

//  The base distribution mudlib name.  

#define MUD_SITE_LIST ([6666:"ts",7777:"ts2",6667:"cp"])
#define MUDLIB_NAME     	"SJever"
#define MUDLIB_VERSION_NUMBER	"2.01"
#define MUDLIB_VERSION		(MUDLIB_NAME + " " + MUDLIB_VERSION_NUMBER)
#define INTERMUD_MUD_NAME	"ts"
#define MUD_SITE_ID	MUD_SITE_LIST[get_config(256)]
#define CHINESE_MUD_NAME	"「书剑·永恒」天枢站"

#endif
