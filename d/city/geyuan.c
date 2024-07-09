// /d/city/geyuan.c

#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", HIY"个园"NOR);
	set("long", @long
据传这里是盐商黄应泰修建。园内种竹千杆，因竹叶形如“个”字，故以
是名。入园门左转至复道廊，迎面花坛种竹，竹间立石笋。北面园门上有“个
园”题字石额。园门向北为桂花厅，前植桂树，后凿水池，六角攒尖式小亭隔
水相望。延北墙是湖石假山，山上古松，山下趋桥流水。夏日晴雨，水中倒影
多变，有下山之称。
long);
	set("exits",([
		"west" : __DIR__"jiashan",
		"east" : __DIR__"nandajie1",
	]));
	set("objects", ([
		__DIR__"obj/flower" : 1,
		BINGQI_D("shizi") :1,
	]));
	set("incity",1);
	setup();
}
