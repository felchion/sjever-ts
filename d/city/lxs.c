// /d/city/lxs.c
// this is made by beyond
// update beyond
#include <ansi.h>

inherit ROOM;

void create()
{
	set("short",HIW"莲性寺白塔"NOR);
	set("long",@long
塔下筑有长方形高台，四周围以栏板，前有小台，小台北面和两侧均筑阶
梯，可以登人。塔位于高台中央，底为砖雕须弥座，上为塔肚，塔肚上是塔脖
子，再上是十三层瘦长的圆圈，称“十三天”，顶上盖圆盘，最上是铜制的葫
芦塔顶。此塔用砖砌成，是瘦西湖名景之一。
long);
	set("exits",([
		"east" : __DIR__"hubian1",
	]));
//	set("objects", ([    
//"/d/city/npc/laopu" : 1,
//	]));

	set("outdoors", "扬州");
	set("coor/x",80);
  set("coor/y",40);
   set("coor/z",0);
   set("coor/x",80);
  set("coor/y",40);
   set("coor/z",0);
   set("coor/x",80);
  set("coor/y",40);
   set("coor/z",0);
   set("coor/x",80);
  set("coor/y",40);
   set("coor/z",0);
   setup();
}
