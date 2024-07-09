// Write by Yanyang@SJ 2000.9.23
// jintan.c  大酒坛

inherit ITEM;
inherit F_LIQUID;
#include <ansi.h>

void create()
{
        set_name(YEL"大酒坛"NOR, ({"da jiutan", "jiutan", "tan"}));
        set_weight(700);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "一个大酒坛，可以用它来向小金杯里倒酒(daojiu)。\n");
                set("unit", "只");
                set("value", 300);
                set("max_liquid",400);
        }

        set("liquid", ([
                "type": "alcohol",
                "name": "上等的女儿红",
                "remaining": 400,
                "drunk_apply": 200,
        ]));
}

void init()
{
	add_action("do_daojiu","daojiu");
	add_action("do_drink","drink");
}

int do_drink(string arg)
{
	object me=this_player();
	object ob=this_object();
        if(!objectp(present(ob, me)) ) return 0;
        if(ob->query("liquid/name")!="上等的女儿红") return 0;
	if (arg=="da jiutan" || arg=="jiutan" || arg=="tan"){
		message_vision("$N似乎觉得用小金杯不过瘾，于是拿起"YEL"大酒坛"NOR"一阵海喝！\n",me);
		me->add("water",-50);
		}
}        

int do_daojiu()
{
	object me=this_player();
	object ob=this_object();
	object obj=(present("xiao jinbei",me));
        if (ob->query("liquid/name")!="上等的女儿红") return notify_fail("你这坛也不是酒啊！\n");
	if (!obj) return notify_fail("你身上没有小金杯，准备用什么东西来盛装这上等的女儿红？\n");
	if (obj->query("liquid/remaining")>0) return notify_fail("小金杯里已经是满的了！\n");
	message_vision("$N用"YEL"大酒坛"NOR"将"HIY"小金杯"NOR"倒满了上等的女儿红！\n",me);
        ob->add("liquid/remaining", -1);
        obj->add("liquid/remaining",1);
        return 1;
}


