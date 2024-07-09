// remove.c

#include <ansi.h>

inherit F_CLEAN_UP;

int do_remove(object me, object ob);

int main(object me, string arg)
{
	object ob, *inv;
	int i;

	if (!arg) return notify_fail("你要脱掉什么？\n");
	if (userp(me) && me->is_busy()) return notify_fail("你正忙着呢。\n");

	if(arg=="all") {
		inv = all_inventory(me);
		for(i=0; i<sizeof(inv); i++)
			do_remove(me, inv[i]);
		write("Ok.\n");
		return 1;
	}

	if( !objectp(ob = present(arg, me)) )
		return notify_fail("你身上没有这样东西。\n");
	return do_remove(me, ob);
}

int do_remove(object me, object ob)
{
	string str;
//add embed for throw stuffs
        if(ob->query("embedded")){
	      message_vision(HIR "$N一咬牙将$n"HIR"从伤口处拔了下来，涌出一大股鲜血！\n" NOR, me,ob);
              me->receive_wound("qi", ob->query("damage")*5, "身中"+ob->name()+"而");
              me->receive_damage("qi", ob->query("damage")*10, "身中"+ob->name()+"而");
              ob->delete("embedded");
              me->delete_temp("embed");
              return 1;
        }
        
// end of throw 
	if( (string)ob->query("equipped")!="worn" )
		return notify_fail("你并没有装备这样东西。\n");

	if( ob->unequip() ) {
		if( !stringp(str = ob->query("remove_msg")) )
			switch(ob->query("armor_type")) {
				case "cloth":
				case "armor":
				case "surcoat":
				case "boots":
					str = YEL "$N将$n"YEL"脱了下来。\n" NOR;
					break;
				case "bandage":
					str = YEL "$N将$n"YEL"从伤口处拆了下来。\n" NOR;
					break;
				default:
					str = YEL "$N卸除$n"YEL"的装备。\n" NOR;
			}
		message_vision(str, me, ob);
		return 1;
	} else
		return 0;
}

int help(object me)
{
  write(@HELP
指令格式 : remove all | <物品名称>
 
这个指令让你脱掉身上某件防具.
 
HELP
    );
    return 1;
}
 
