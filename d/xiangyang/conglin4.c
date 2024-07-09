// Room: /d/xiangyang/conglin4.c

inherit ROOM;
void create()
{
	set("short", "山间空地");
	set("long", @LONG
这里是山麓上的一块空地，路旁的杂草和树木被人休整过了。路旁有一个
用树木搭起的小屋，小屋的门口有一些猎人在休息。一些弓箭和绳索挂在路旁
的树上。西北方向有一片树林，史家五兄弟正在这里好象要抓什么动物。
LONG	);
	set("exits", ([
        	"eastdown" : __DIR__"conglin3",
        	"northwest" : "/d/gumu/xuantie/xiaolu4",

        ]));
        set("objects",([
              "/clone/npc/shijiqiang" : 1,
              "/clone/npc/shibowei" : 1,
              "/clone/npc/shimengjie" : 1,
              "/clone/npc/shishugang" : 1,
              "/clone/npc/shizhongmeng" : 1,
        ]));

        set("outdoors", "襄阳");
	setup();
}
