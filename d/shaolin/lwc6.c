//lwc6.c

inherit ROOM;

void create()
{
	set("short", "练武场");
	set("long", @LONG
这里是少林寺的练武场。由无数块巨大的花岗岩石板铺就。上千年的踩
踏已使得这些石板如同镜子般平整光滑。十来个僧人正在练习武技。南北东
三面各有一个练武场。西面是研习武功用的罗汉九部。
LONG
	);

	set("exits", ([
		"north" : __DIR__"lwc4",
		"east" : __DIR__"lwc5",
		"south" : __DIR__"lwc7",
                "west" : __DIR__"luohan5",
	]));

	set("outdoors", "shaolin");
	set("coor/x",0);
  set("coor/y",280);
   set("coor/z",120);
   set("coor/x",0);
 set("coor/y",280);
   set("coor/z",120);
   setup();

}

