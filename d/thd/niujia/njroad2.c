// Room: /u/jpei/thd/njroad2.c

inherit ROOM;

void create()
{
	set("short", "村中小路");
	set("long", @LONG
一条村中的小路。路边是一家民房，好象很久没有人住的样子了。
LONG
	);
	set("no_clean_up", 0);
	set("outdoors","牛家村");

	set("exits", ([
		"west": __DIR__"njroad1.c",
		"east": __DIR__"njroad3.c",
		"north": __DIR__"guojia.c",
	]) );
	set("objects", ([
	"/clone/npc/girl" : 1,
	]) );

	setup();
}

