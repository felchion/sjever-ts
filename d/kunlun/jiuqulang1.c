// jiuqulang1.c (kunlun)

#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
	  set("short", HIG"九曲廊"NOR);
	  set("long",@long
九曲廊前后共有九进，曲曲折折地盘旋在水面。一路穿花度柳，抚石依泉，
落花浮荡，水色溶溶。两边垂柳杂着桃杏，藤蔓绕柱。白石为栏，飞楼插空，
转折处皆由小亭相接。
long);
	  set("exits",([
             "southwest" : __DIR__"tieqinju",
             "east" : __DIR__"shiqiao",
          ]));
          set("outdoors", "昆仑");
          setup();
}
