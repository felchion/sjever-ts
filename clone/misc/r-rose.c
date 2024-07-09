// r-rose.c 红玫瑰
// update by cool
// update by Lklv 2001.9.28

#include <ansi.h>
#include <armor.h>
inherit HEAD;

void create()
{
        set_name(HIR "红玫瑰" NOR, ({ "hong meigui", "meigui", "rose" }));
        set("weight", 100);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "朵");
                set("long", HIR "一朵送给情人的红玫瑰，带给你一丝温情。它香气迷人，让人忍不住想闻(wen)一下。\n" NOR);
		set("value", 1020);
		set("wear_msg", HIM "$N嘻嘻一笑，把花儿别在发髻上，有如天女下凡。\n" NOR);
		set("remove_msg", HIM "$N把花儿从发髻取下，深深闻了闻，揣回怀中。\n" NOR);
		set("armor_prop/armor", 1);
        }
        setup();
}

void init()
{
	add_action("come_life","wen");
	call_out("dest", 18000);
}

int come_life(string arg)
{
	object me, ob;
	me = this_player();
	ob = this_object();

	if( !ob || !me ) return 0;
	if( !arg ) return notify_fail("你闻了下空气，感觉好像少了点什么？\n");
	if(arg == "rose" || arg == "meigui" || arg == "hong meigui" ){
		message_vision("$N用小指轻轻捻住红玫瑰，拿起来深深地闻了闻。\n",me);
        	tell_object(me, HIY "嗯。。。啊。。。好香呀！\n" NOR);
	}
	return 1;
}

void dest()
{
	object me, ob;
	me = this_player();
	ob = this_object();

	if( !ob || !me ) return 0;
	message_vision("$N手中的红玫瑰渐渐枯萎了。\n",me);
	destruct(ob);
}
