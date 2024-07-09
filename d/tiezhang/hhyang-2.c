//hhyang-2.c 花圃
#include <ansi.h>

inherit ROOM;

void create()
{
    set("short", HIR"花圃"NOR);
	set("long", @LONG
只见这里摆满了茉莉、素馨、麝香藤、朱槿、玉桂、红蕉，都是夏日盛开
的香花。院墙上又挂了枷木兰、真腊龙诞等香珠，但觉馨意袭人，清芬满园。
园中桌上放着几盆新藕、甜瓜、枇杷、等鲜果，椅子上丢着一把蒲扇，看来是
帮主休息的地方。
LONG
	);

	set("exits", ([
"west" : __DIR__"hhyuan-1",
]));

	set("no_clean_up", 0);
        set("outdoors", "tiezhang");

	set("coor/x",-320);
  set("coor/y",-230);
   set("coor/z",10);
   setup();
}