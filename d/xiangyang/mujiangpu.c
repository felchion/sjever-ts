// Room: /d/xiangyang/mujiangpu.c

inherit ROOM;

void create()
{
	set("short", "木匠铺");
	set("long", @LONG
这是一家简陋的木匠铺，一个中年木匠正蹲在地上钉东西。值此乱世，木
匠们也只好制作些刀枪棍棒什么的以养家糊口。地上和墙角凌乱地堆放着一些
木剑，木刀，木棍什么的，还有几件象锯子，刨子一类的木匠工具。
LONG
	);

	set("exits", ([
		"west" : __DIR__"ncx4",
	]));

	set("objects", ([
		__DIR__"npc/mujiang" : 1,
	]));
	set("no_clean_up", 0);

	set("incity",1);
	setup();
}