// Room: /d/huashan/jinshe/obj/datiehe.c
// Date: Look 99/03/25

#include <ansi.h>

inherit ITEM;

void create()
{
        set_name("大铁盒" , ({ "tie he", "box" }) );
        set_weight(3000);
        set_max_encumbrance(5000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "个");
                set("long", "这是一个两尺来高的铁盒子，上面满是了泥土。\n");
                set("value", 1000);
                set("material", "steel");
                set("no_get",1);
                set("no_drop",1);
                set("amount",30);
                }
        setup();
}

int is_container() { return 1; }

void init()
{
        add_action("do_open", "open");
        add_action("do_move", "move");
}

int do_open(string arg)
{
        object me = this_player();
        object ob;

       
    if (!arg || (arg != "box" && arg != "tie he"))
       {
        return notify_fail("你想打开什么？\n");
        return 1;
       }
    else
    {
    if (!present("xin jian",me))
       {
        write("你轻开铁匣，突然掉落了一张信笺，你急忙把它捡起。\n");
        ob=new(__DIR__"xinjian1");
        me->set_temp("mark/open",1);
        ob->move(me);
        return 1;
       }
        write("你打开铁盒，发现里面空空的什么也没有。\n");
        return 1;
    }
}
int do_move(string arg)
{
        object me = this_player();
        
        if ( !arg || arg != "anceng" )
        return notify_fail("你要干什麽？\n");
        if (!(int)me->query_temp("mark/open"))
         return notify_fail("你要做什麽？\n");
        message_vision(RED"$N突然看见几道寒光扑面，大叫一声，一阵抽搐。\n"NOR, me);
        me->delete_temp("mark/open"); 
        me->set_temp("last_damage_from", "中毒");
        me->die();
        return 1;
}
