// Room: /u/xdd/gumu/gchang.c
// Modify By River 98/08
inherit ROOM;
#include <ansi.h>
#include "/d/group/entry.h"

void create()
{
	set("short",HIY"终南广场"NOR);
	set("long", @LONG
此处原是乡村小镇村民们打谷晒粮的地方，来了戏团或每年正月庙会，更
是乡镇的集会所，后来由于全真教与古墓派声名日盛，掌教将山下此处场所修
葺整理，于林荫森森中扩开一片平整洁净的广场，成为了武林人物常来拜山论
技的场所，平日里常有老人做清洁打理的活计，也有孩子在此玩耍，东南面有
个小集市。
LONG    
	);

        set("outdoors", "终南山");
	set("group", 1);
        set("exits", ([
        	"north" : "/d/group/tiexue/1020319333",
		"northwest" : "/d/gumu/xiaolu2",
		"southeast" : "/d/gumu/jishi",
        ]));
	setup();
}
