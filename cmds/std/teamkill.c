// teamkill.c

#include <ansi.h>
#include <pktime.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object obj;
	string callname;
	object *t;
	int i;

	if( environment(me)->query("no_fight") )
		return notify_fail("这里不准战斗。\n");

	if( !arg )
		return notify_fail("你想杀谁？\n");

	if(!objectp(obj = present(arg, environment(me))))
		return notify_fail("这里没有这个人。\n");

//	if (userp(obj) && me->query("转让id")) // add by Looklove for share id
//		return notify_fail(HIR"你的id属于接手别人的id，未cut转让时的exp不能杀害其他玩家！\n"NOR);

	if(strsrch(file_name(environment(me)),"/d/wuguan/") >= 0 && !obj->query_condition("killer"))
		return notify_fail("武馆内禁止杀人。\n");

	if( !obj->is_character() || obj->is_corpse() )
		return notify_fail("看清楚一点，那并不是活物。\n");

	if (wiz_level(me) < wiz_level(obj))
		return notify_fail("你不能攻击巫师等级比你高的对手。\n");

	if( me->query_condition("killer") > 180 && userp(obj))
		return notify_fail("你想到自己还被通缉着，手不由软了下来。\n");

	if (obj == me)
		return notify_fail("用 suicide 指令会比较快:P。\n");

	if (!me->is_team_leader())
		return notify_fail("只有队伍领袖才能命令大家。\n");

	if ( obj->query("job_npc") || obj->query("no_ansuan"))
		return notify_fail("要杀就杀，参加什么队伍啊！\n");

// Add By River@SJ 2003.6.24
	if ( pktime_limit(me, obj))
		return 0;

	if(meisdummy(me,obj))
	{
		return notify_fail("死大米，一边凉快去。\n");
	}
/*
	if (userp(obj) && userp(me)
	&& (!living(obj) && me->query("mud_age") < (time() - me->query("birthday"))/24
	|| me->query("mud_age") < (time() - me->query("birthday"))/48))
		return notify_fail("死大米，一边凉快去。\n");
*/
	if (me->query_temp("combat_yield"))
		return notify_fail("你正打不还手呢，怎么杀？\n");

	t = me->query_team();
	if (arrayp(t))
		t -= ({ 0 });
	if(sizeof(t) < 2)
		return notify_fail("你现在并没有参加任何队伍或队伍只你一个人。(use kill)\n");

	for(i=0;i<sizeof(t);i++){
		if(environment(me) != environment(t[i]))
			return notify_fail("队员不都在场。(use kill)\n");

		if (obj == t[i])
			return notify_fail("想内讧么？\n");

		if (t[i]->query("combat_exp") > obj->query("combat_exp")/2*sizeof(t) && userp(obj))
			return notify_fail("唉！你可不可以改掉以众欺寡的恶习？\n");
	}

	callname = RANK_D->query_rude(obj);
	message_vision("\n$N对着$n喝道：「"
		+ callname + "！今日不是你死就是我们活！」\n\n", me, obj);

	for(i=0;i<sizeof(t);i++) {
		if (userp(obj) && t[i]->query_condition("killer") > 180)
			continue;
		if (userp(obj) && userp(t[i])
		&& (!living(obj) && t[i]->query("mud_age") < (time() - t[i]->query("birthday"))/24
		|| t[i]->query("mud_age") < (time() - t[i]->query("birthday"))/48))
			continue;
		if (t[i]->query_temp("combat_yield"))
			continue;
		if (t[i]->is_killing(obj->query("id")) && t[i]->is_fighting(obj))
			continue;
		t[i]->set_temp("ttarget",obj->query("id"));
		t[i]->kill_ob(obj);
		if (userp(t[i]) && userp(obj) && strsrch(file_name(environment(t[i])), "/cmds/leitai/bwdh") != 0 )
			t[i]->add_busy(2);
		obj->kill_ob(t[i]);
	}
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : teamkill <人物>
 
这个指令让你的队伍主动攻击一个人物。

其他相关指令: team,fight,kill

HELP
    );
    return 1;
}
