// Room: /d/emei/baoguosm.c

inherit ROOM;
#include <ansi.h>
#include <wanted.h>

void create()
{
	set("short", "报国寺山门");
	set("long", @LONG
报国寺为峨嵋出入之门户。山门(gate)上有一块大匾。寺外苍楠环卫，翠
柏掩映。由此一路向西上开始登峨嵋山，北面下山是一条弯弯的石阶路。
LONG
	);
	set("outdoors", "峨眉山");
	set("item_desc",([
	"gate" :  HIW "\t\t\t   ≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌
		? 　?    ≌≌　　　　　　　　　　　　　≌≌
　　　　　　　　           ≌≌　　　报　　国　　寺　　　≌≌
		 　　      ≌≌　　　　　　　　　　　　　≌≌
             　　　　      ≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌\n" NOR
]));
	set("exits", ([ 
	  "northdown" : __DIR__"qingshijie",
	  "west" : __DIR__"baoguosi",
]));

	set("coor/x",-180);
  set("coor/y",-120);
   set("coor/z",10);
   setup();
}

int valid_leave(object me, string dir)
{
	if (dir == "west" && is_wanted(me))
		return 0;
	return :: valid_leave(me, dir);
}
