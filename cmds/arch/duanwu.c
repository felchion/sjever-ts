#include <ansi.h>
inherit F_CLEAN_UP;
int main(object me,string str)
{
        object *ob;
        int i;
        object obj;
        if(!me->query_temp("confim_daunwu"))
        {
        	me->set_temp("confim_daunwu",1);
        	return notify_fail("这是端午节专用的哦，再输入一次 确认你没记错 今天其实是国庆日。\n");
        }
        me->delete_temp("confim_daunwu");
	ob = filter_array(children(USER_OB),(:clonep:));
	i = sizeof(ob);		
	while (i--) {
		if(!objectp(obj = new(FOOD_D("super_zongzi")))) continue;
		if(!ob[i]||!objectp(ob[i])) continue;
		obj->move(ob[i]);
		tell_object(ob[i],BLINK HIC"端午快乐！预祝每天都这么快乐！\n"NOR);
		message_vision("突然从天上一个"HIW+obj->name()+NOR"掉在$N的怀里。\n",ob[i]);
		message_vision("$N高兴得唱道：脱去你的外衣，你是那样白嫩，发着淡淡的幽香，舔一舔再咬上一口，我心飞上九霄云外，啊，粽子我的至爱！ \n",ob[i]);
		tell_object(ob[i],HIM"虽然你身体很好，但屈原说：多锻练身体，少打麻将，才能多吃粽子，天天健康！\n"NOR);
	}
	return 1;
}

int help(object me)
{
	write(@HELP
	哈哈，端午节快乐！
HELP
	);
	return 1;
}