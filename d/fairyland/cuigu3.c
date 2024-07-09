// Room: cuigu3.c
// By River@SJ
// Update by Lklv , nip river

#include <ansi.h>
inherit ROOM;
string look_zi();
void create()
{
        set("short", HIG"翠谷"NOR);
        set("long", @LONG
你面前是个花团锦簇的翠谷，红花绿树，交相掩映。脚下踏着的是柔软细
草，鼻中闻到的是清幽花香，鸣禽间关，鲜果悬枝，不由的心旷神怡。左壁山
壁上刻着一行小字(zi)，距离太远，你看不真切。
LONG    );

        set("exits", ([
                "north" : __DIR__"cuigu",
        ]));
        set("item_desc", ([
           "zi" : (: look_zi :)
        ]));

        set("outdoors", "昆仑翠谷");
        set("no_clean_up", 0);

        setup();
}

void init()
{
        add_action("do_mai",({"mai","cang"}));
        add_action("do_wa", ({"qu", "jue"}));
}

string look_zi()
{
	object me=this_player();
	if (me->query("jiuyang/mai"))
		return HIW"\t\t"+me->name()+"埋经处\n"NOR;
	else
		return "时间久远，风雨腐蚀，你已经看不清楚上面所写的字迹了。\n";
}

int do_mai(string arg)
{
	object me=this_player();
	object ob;
	if(!me->query("jiuyang/baiyuan")) return 0;
	if(me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
	if( arg =="book" || arg =="jiuyang zhenjing"){
		if( !objectp(ob = present("jiuyang zhenjing", me)))
			return notify_fail("你身上好象没有啊，怎么埋啊？\n");
		message_vision(HIW"\n$N在山洞左壁挖了个三尺来深的洞孔，将四卷九阳真经埋在洞内，填上了泥土。\n"+
                          "然后拾起一块尖石，在山壁上划下几个大字："+me->name()+"埋经处。\n\n"NOR,me);
		if(!me->query("jiuyang/mai"))
			me->set("jiuyang/mai", 1);
		me->delete_temp("get_9yjing");
		destruct(ob);
		return 1;
        }
	return notify_fail("你要埋什么东西啊？\n");
}

int do_wa(string arg)
{
	object me = this_player();
	object ob;
	if(!me->query("jiuyang/mai")) return 0;
	if(me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
	if( arg =="book" || arg =="jiuyang zhenjing"){
		if(me->query_temp("get_9yjing"))
			return notify_fail("你大吃一惊，藏在山壁中的九阳真经已然不见，不知被谁拿走了！！\n");
		ob = unew("/d/fairyland/obj/jiuyang");
		if ( !clonep(ob))
			return notify_fail("你大吃一惊，藏在山壁中的九阳真经已然不见，不知被谁拿走了！\n");
		message_vision(HIY"\n$N将泥土挖开，将藏在里面的九阳真经取了出来。\n\n"NOR,me);
		ob->set_temp("jiuyang",me->query("id"));
		me->set_temp("get_9yjing", 1);
		ob->move(me);
		return 1;
        }
        return notify_fail("你要挖掘什么？\n");
}
