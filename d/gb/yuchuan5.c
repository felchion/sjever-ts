// yuchuan5.c 丐帮渔船

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","渔船");
	set("long", @LONG
突然你眼前一亮，你来到了一艘大船上，甲板上躺着几个乞丐，一边在捉
虱子， 一边用警惕的双眼不停地打量着你。正中有一个舱门，里面黑乎乎的，
你什么也瞧不见。
LONG
	);
	set("exits",([
	"enter" : __DIR__"fenduo1",
	"west"  : __DIR__"yugang"
	]));
	set("coor/x",130);
  set("coor/y",-470);
   set("coor/z",10);
   setup();
}

