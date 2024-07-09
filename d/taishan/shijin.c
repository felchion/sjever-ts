// Room: /d/taishan/.c

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "石经峪");
        set("long", @LONG
这里是一块大石坪，石面刻有篆文「金刚经」，字体其大如斗，但却没有
记载刻石者的名字和刻石的年代。过了此处，渐入空山，山路越来越不好走了。
LONG
        );
        set("exits", ([
                "northup" : __DIR__"huima",
                "westdown" : __DIR__"doumo",
        ]));
        set("objects",([
                __DIR__"npc/seng-ren" : 1,
        ]));
        set("outdoors", "泰山");
	setup();
}

void init()
{
        add_action("do_mosong", "mosong");
}

int do_mosong(string arg)
{
        object me;
        me = this_player();
	if (!living(me)) return notify_fail("");
        if ( !arg || ( arg != "stone" ) )
		return notify_fail("什么？\n");
        if (me->query_skill("literate",1)<150)
		return notify_fail("你的文化太差，不认识石壁上的梵文。\n");
        if (me->query_skill("buddhism",1)<160)
		return notify_fail("你的禅宗心法太低，不能领悟金刚经意。\n");
        if (me->query_skill("yijin-jing",1)<140)
		return notify_fail("你读着金刚经，觉得自己的易筋经修为太差，不觉面红过耳。\n");
        if (me->query_skill("shaolin-shenfa",1)<140) 
		return notify_fail("你妙识金刚经意，但不久便腰酸背痛，原来少林身法太差，不能久站。\n");
	if (!me->query_temp("sl_wm4"))
		return notify_fail("你对着石壁一筹莫展。\n");
	if (random(30)==1) {
		write (HIY"你对着石坪上的「金刚经」琢磨了一回儿，似乎对一些禅理仍有些疑问。\n"NOR);
		me->delete_temp("sl_wm4");
		me->set_temp("sl_findjing",1);
	}
	message_vision("$N正专心研读石坪上的「金刚经」。\n", me);
	me->add("jingli", -20);
	return 1;
}
