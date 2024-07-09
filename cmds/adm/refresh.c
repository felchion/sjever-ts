#include <ansi.h>

inherit F_CLEAN_UP;
//inherit F_FROZEN;

int main(object me,string arg)
{	
	if(wizardp(me)){
	if(arg=="qi")
	{
		F_FROZEN->refresh_qi(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="neili")
	{
		F_FROZEN->refresh_neili(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="cloth")
	{
		F_FROZEN->refresh_cloth(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}	
	if (arg=="jing")
	{
		F_FROZEN->refresh_jing(me);
		return 1;
	}
	if (arg=="jingli")
	{
		F_FROZEN->refresh_jingli(me);
		return 1;
	}
	if (arg=="qi")
	{
		F_FROZEN->refresh_qi(me);
		return 1;
	}
	
	if(arg=="head")
	{
		F_FROZEN->refresh_head(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}	
	if(arg=="weapon")
	{
		F_FROZEN->refresh_weapon(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="tail")
	{
		F_FROZEN->refresh_tail(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="water")
	{
		F_FROZEN->refresh_water(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="exp")
	{
		F_FROZEN->refresh_exp(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="surcoat")
	{
		F_FROZEN->refresh_surcoat(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="food")
	{
		F_FROZEN->refresh_food(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="pot")
	{
		F_FROZEN->refresh_pot(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="armor")
	{
		F_FROZEN->refresh_armor(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="shen")
	{
		F_FROZEN->refresh_shen(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="maxneili")
	{
		F_FROZEN->refresh_maxneili(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	if(arg=="boots")
	{
		F_FROZEN->refresh_boots(me);
		tell_object(me,"刷新ing\n");
		return 1;
	}
	}
	if(!wizardp(me) && me->query("registered")<3) return notify_fail("该指令只想贵宾玩家开放。\n");
	if (arg=="none")
	{		
		F_FROZEN->refresh_clear_all(me);
		//me->delete("env/hp_mode");
		tell_object(me,"状态已经清空。\n");
		return 1;
	}
	//me->set("env/hp_mode",1);
	F_FROZEN->show_two_screen(me);
	tell_object(me,"状态栏刷新完毕！\n");
	return 1;
}

int help(object me)
{
     write(@HELP
指令格式: refresh | none
该命令让玩家启用 HP 模式，更新你的状态栏。
参数 none 表示取消 HP 模式。
HP 模式：就是在您ZMUD客户端的顶端显示一个 HP 和基本装备框，类似
武侠RPG的状态。
HELP
    );
    return 1;
}
