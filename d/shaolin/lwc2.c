//lwc2.c


inherit ROOM;

void create()
{
	set("short", "练武场");
	set("long", @LONG
这里是少林寺的练武场。由无数块巨大的花岗岩石板铺就。上千年的踩
踏已使得这些石板如同镜子般平整光滑。武场中央竖立着一个制作精巧的木
人，一些年青僧人正自围着它练功。
LONG
	);

	set("exits", ([
		"west" : __DIR__"lwc3",
		"south" : __DIR__"lwc",
	]));
         set("objects",([
		__DIR__"obj/mu-ren" : 1,
	]));


	set("outdoors", "shaolin");
	set("coor/x",20);
  set("coor/y",290);
   set("coor/z",120);
   set("coor/x",20);
 set("coor/y",290);
   set("coor/z",120);
   setup();

}

