// /u/beyond/mr/xiaozhou2.c
// this is made by beyond
// update 1997.6.20
inherit ROOM;
void create()
{
	set("short", "渡船");
	set("long", @LONG
一叶小舟，最多也就能载七、八个人。一名六十多岁的老艄公手持长竹篙，
正在船尾吃力地撑着船。
LONG
	);
	set("outdoors", "慕容");
        set("no_fight",1);
	setup();
}

void run(object ob)
{
	if (!ob) return;
	if (environment(ob) == this_object()){
		tell_object(ob, "\n小船在荷花溏中缓缓而行,不一会你变看到曼佗罗山庄的轮廓了。\n\n");
		ob->move(__DIR__"xiaozhou3", 1);
	}
}

void init()
{
	object ob = this_player();
	call_out("run", 5, ob);
}
