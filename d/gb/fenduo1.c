// fenduo1.c 丐帮大信分舵
// Modify by Looklove@SJ at 2000/9/26

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","大信分舵");
	set("long", @LONG
这就是丐帮的大信分舵，负责管理福州的事务。舱四周的窗都关得严严的，里
面只点了一盏油灯，你依稀看到地上满是鱼骨，鱼腥味，霉味混在一块，你觉得难
受死了。正中桌子围了几个乞丐，似乎正在商量着什么，一数他们背上的布袋就知
道他们是帮里很有地位的人物。
LONG
	);
	set("exits",([
	"out" : __DIR__"yuchuan5",
	]));
	set("objects", ([
                CLASS_D("gaibang") + "/jiang" : 1,
                CLASS_D("gaibang") + "/wu" : 1,
	]));

	set("valid_startroom",1);
	set("coor/x",120);
  set("coor/y",-470);
   set("coor/z",20);
   setup();
       call_other("/clone/board/gb_board", "???");
}

