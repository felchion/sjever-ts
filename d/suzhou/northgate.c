// 苏州北门
inherit ROOM;
string look_gaoshi();

void create()
{
	set("short", "北门");
	set("long", @LONG
这是北城门，因为曾经失火，到现在城墙还是黑乎乎的，因此白纸黑字
的官府告示(gaoshi)就显得特别现眼。北方是一片崇山峻岭，一条黄土小径
在山里蜿蜒而上。
LONG
	);
	set("item_desc", ([
		"gaoshi" : (:look_gaoshi:),
	]));

	set("exits",([
		"north" : __DIR__"qsgdao6",
		"west" : __DIR__"bingying.c",
		"south" : __DIR__"beidajie2",
	]));
	set("objects", ([
		NPC_D("guanfu/wujiang") : 1,
		NPC_D("guanfu/bing") : 2,
	]));
	set("outdoors", "苏州");
	set("incity",1);
	setup();
}

string look_gaoshi()
{
	return FINGER_D->get_killer() + "\n苏州知府\n赵知仁\n";
}
