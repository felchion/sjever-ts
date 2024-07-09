//value.c

#include <ansi.h>
inherit F_CLEAN_UP;


int main(object me, string arg)
{
    	float value;
        object ob;

	if (!arg || !(ob = present(arg, this_player())))
		return notify_fail("你要估什么价？\n");

	if(!clonep(ob)
	||ob->query("unique")
	||ob->query("no_get")
	||ob->query("no_give")
	||ob->query("no_drop")
	||ob->query("quest")///////////
	||ob->is_corpse()
	||ob->query("food_remaining")
	||ob->query("liquid")
	||ob->query("material")=="meat"//肉？
	||ob->query("money_id")
	||strsrch(file_name(ob),"/d/city/weapon/")==0
	||ob->is_character()
	//||obj->query("")
	//||obj->query("")
	//||obj->query("")
	//||obj->query("")
	) return notify_fail(ob->name()+"不可以被保存，也不好估价。\n");
	
	value = ob->query("value");
	if (value < 30)
		return notify_fail(ob->name()+"一文不值！\n");
	else
	
	if (me->query("registered") > 2)
	tell_object(me,ob->name()+"值"+ MONEY_D->price_str(value) + "，个人储物箱可以保存，你是贵宾用户，大概费用需要"
		+MONEY_D->money_str("/inherit/room/miscroom.c"->get_value2(86400,to_float(value/2)))+"/天。\n" );
	else
	tell_object(me,ob->name()+"值"+ MONEY_D->price_str(value) + "，个人储物箱可以保存，大概费用需要"
		+MONEY_D->money_str("/inherit/room/miscroom.c"->get_value2(86400,to_float(value)))+"/天。\n" );

	return 1;
}


int help(object me)
{
    write(@HELP
指令格式 : value <物品名称>
 
这个指令可以让你对物品进行估价，并且提示个人储物箱的保管费用。
 
HELP
    );
    return 1;
}
 
