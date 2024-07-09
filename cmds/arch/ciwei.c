//ciwei.c

#include <combat.h>

inherit F_CLEAN_UP;

int help(object me);


varargs int skill_power(object ob, string skill, int usage)
{
	int level, power;

	if (!living(ob)) return 0;

	level = ob->query_skill(skill);

	switch(usage) {
		case SKILL_USAGE_ATTACK:
			level += ob->query_temp("apply/attack");
			break;
		case SKILL_USAGE_DEFENSE:
			level += ob->query_temp("apply/defense");
			if (ob->is_fighting())
				level += level * ob->query_temp("fight/"+skill) / 100;
	}

	if( level < 1 )
		power = ob->query("combat_exp") / 120;
	else
		power = (to_int(sqrt(level))*level*level + ob->query("combat_exp")/2) / 30;

	if (usage == SKILL_USAGE_ATTACK) return power * ob->query_str();
	return power * (skill == "dodge"?ob->query_dex():ob->query_int());
}

int main(object me,string arg)
{
	//BALANCE_D
	/*
	ciwei -set skill party
	ciwei -skill skill
	ciwei -party party
	*/
	string skill,party;
	if(!arg) return help(me);
	if(sscanf(arg,"-set %s %s",skill,party)==2)
	{
		if((string)SECURITY_D->get_status(me) != "(admin)" && getuid(me)!="ciwei")
			return notify_fail("只要天神才可以进行酱紫的设置。\n");
		BALANCE_D->set_party(skill,party);
		write("设置完成。\n");
		return 1;
	}
	if(sscanf(arg,"-remove %s %s",skill,party)==2)
	{
		if((string)SECURITY_D->get_status(me) != "(admin)" && getuid(me)!="ciwei")
			return notify_fail("只要天神才可以进行酱紫的设置。\n");
		BALANCE_D->remove_party(skill,party);
		write("设置完成。\n");
		return 1;		
	}
	else if(sscanf(arg,"-skill %s",skill)==1)
	{
		write(BALANCE_D->query_skill(skill));
		return 1;
	}
	else if(sscanf(arg,"-party %s",party)==1)
	{
		write(BALANCE_D->query_party(party));
		return 1;		
	}
	else if(arg=="-query")
	{
		mixed files = get_dir("/kungfu/skill/",-1);
		int i=sizeof(files);
		while(i--)
		{
			if(files[i][1]==-2) continue;
			if(files[i][0][<2..<1]!=".c") continue;
			if(("/kungfu/skill/"+files[i][0])->type() != "martial" ) continue;
			write("A "+files[i][0][0..<3]+" B\n");
		}
		//write(sprintf("%O\n",files[0]));
		//write(files[0]);
		return 1;
	}
	else if(arg=="-save")
	{
		return BALANCE_D->save();		
	}
	else if(sscanf(arg,"-player %s",party)==1)
	{
		object ob;
		ob = find_player(party);
		if(!ob) return notify_fail("No this Body!\n");
		write(sprintf("%s的轻功效果为：%d\n",party,skill_power(ob,"dodge",SKILL_USAGE_DEFENSE) ));
		return 1;
	}
	else return help(me);
}

int help(object me)
{
	write(
@HELP
？？？？？？？？？？？？？？？？？？？？？
	ciwei -set [skill] [party]
	将这个技能指派给该门派；
	ciwei -remove [skill] [party]
	取消
	ciwei -skill [xxxx]
	查询有关这个技能的使用情况
	ciwei -party [xxx]
	查询这个门派的情况
	ciwei -save
	保存当前状态
？？？？？？？？？？？？？？？？？？？？？
HELP
);
	return 1;
}