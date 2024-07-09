// Room: /d/taishan/.c

inherit ROOM;
#include <ansi.h>
void create()
{
	set("short", HIY"封禅台"NOR);
	set("long", @LONG
这里是临时推选天下武功第一人之处，各路英雄如能战胜「中神通」，便
可成为统领中原武林的领袖。高台上摆放着汉白玉宝座，台上高高立着一竿旗，
上书：替天行道。
LONG
	);

	set("exits", ([
		"down" : __DIR__"yuhuang",
	]));

	set("objects",([
		"/clone/npc/zhong-shentong" :1,
		__DIR__"npc/wei-shi1": 2,
	]));

        set("no_clean_up", 1);
	set("outdoors", "泰山");
	setup();
}
