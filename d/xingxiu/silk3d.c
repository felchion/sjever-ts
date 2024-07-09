// /d/xingxiu/silk3.c
//tangfeng@SJ 2004

#include <ansi.h>
inherit ROOM;
#include <wanted.h>

void create()
{
	set("short", BLU"绝壁"NOR);
	set("long", @LONG
这里是颂摩崖边最狭之处，山势陡峭，仅有数尺之宽，下面就是黄龙潭，
潭深莫测。可谓一夫当关的险地，正是埋伏截杀的好所在。
LONG);
	
	set("exits", ([
		"down" : __DIR__"silk3",
	]));
	
	set("outdoors", "yili");
	setup();
}

void init()
{
	if (!query("calldest")){
		set("calldest", 1);
		set("player",1);
		call_out("dest", 600);
	}
}

void dest() {
	object *inv;
	int i;

	inv = all_inventory(this_object());

	if (!query("player")) {
		if (sizeof(inv)>0) {
			for(i=0;i<sizeof(inv);i++) {
				if (userp(inv[i]))  {
					set("player",1);
					call_out("dest", 120);
					return;
				}
			}
		}
		destruct(this_object());
		return;
	}
	else {
		call_out("dest", 120);
		if (sizeof(inv)>0) {
			for(i=0;i<sizeof(inv);i++) {
				if (userp(inv[i]))  {
					set("player",1);
					return;
				}
			}
		}
		delete("player");
	}
}