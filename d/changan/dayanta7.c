// Write By Yanyang@SJ 2001.10.2
// Room: /d/changan/dayanta7.c

inherit ROOM;

void create()
{
        set("short", "大雁塔七层");
        set("long", @LONG
大雁塔内结构精洁宽敞，塔内布置精美，给人以清静肃穆之感。四周开有
龛窗，塔内有楼梯直通上下。这里似乎是非同寻常的场所。
LONG
        );
        set("exits", ([
                 "down"  : __DIR__"dayanta6",
        ]));

        set("incity",1);
	setup();
}