// lineupkill.c

#include <ansi.h>
#include <pktime.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object obj;
	string callname;
	object *lineup;
	int i;

	if( environment(me)->query("no_fight") )
		return notify_fail("这里不准战斗。\n");

	if( !arg )
		return notify_fail("你想杀谁？\n");

	if(!objectp(obj = present(arg, environment(me))))
		return notify_fail("这里没有这个人。\n");

	if( !obj->is_character() || obj->is_corpse() )
		return notify_fail("看清楚一点，那并不是活物。\n");

	if (wiz_level(me) < wiz_level(obj))
		return notify_fail("你不能攻击巫师等级比你高的对手。\n");

	if(strsrch(file_name(environment(me)),"/d/wuguan/") >= 0 && !obj->query_condition("killer"))
		return notify_fail("武馆内禁止杀人。\n");

	if( me->query_condition("killer") && userp(obj))
		return notify_fail("你想到自己还被通缉着，手不由软了下来。\n");

	if(obj==me)
		return notify_fail("用 suicide 指令会比较快:P。\n");

	if(! me->is_lineup_leader())
		return notify_fail("只有阵法的主持者才能命令大家。\n");

	if ( pktime_limit(me, obj))
		return 0;

	if ( obj->query("job_npc") || obj->query("no_ansuan"))
		return notify_fail("要杀就杀，组什么阵啊！\n");

	callname = RANK_D->query_rude(obj);
	lineup = me->query_lineup();
	if(sizeof(lineup) <=1)
		return notify_fail("你现在并没有参加任何阵法或阵法只你一个人。(use kill)\n");
	    
	if( obj->query_lineup_leader() == me )
		return notify_fail("连自己阵里面的人也杀？有没有搞错？\n");

	if( userp(me) && obj->query("no_pk") && !obj->query_condition("killer") )
		return notify_fail(obj->name()+"已经金盆洗手，还是不要介入江湖争斗吧！\n");

	message_vision("\n$N对著$n喝道：「"+ callname + "！今日不是你死就是我们活！」\n\n", me, obj);

	for(i=0;i<sizeof(lineup);i++){
		if(lineup[i]) {
			lineup[i]->set_temp("lineuptarget",obj->query("id"));
			lineup[i]->kill_ob(obj);
			obj->kill_ob(lineup[i]);
			lineup[i]->add_busy(1);
		}
	}
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : lineupkill <人物>
 
这个指令让你的阵法主动攻击一个人物。

HELP
    );
    return 1;
}
