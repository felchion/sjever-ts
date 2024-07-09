// group teach
// by augx@sj 3/25/2002

#include <ansi.h>
inherit F_CLEAN_UP;

#define	WORK_TIME	60
#define	REST_TIME	60
#define MAX_SKILLS	35

int help(object me)
{
	write(@HELP
指令格式： gteach [技能] to [人物id]
这个命令用来传授帮派保镖技能，保镖的技能最多比传授者的此项技能高100级。
HELP );
	return 1;
}

int main(object me, string arg)
{
	string skill;
	object ob;
	int delay;

	if( !arg || !sscanf(arg, "%s to %s",skill,arg) ) return help(me);
	if( !(ob = present(arg, environment(me))) )
		return notify_fail("没有这个人。\n");
	if( userp(ob) )
		return notify_fail("你不能教玩家武功。\n");
	if( !me->query("group/id")
	  || me->query("group/id") != ob->query("group/id")
	  || me->query("group/class") >= ob->query("group/class") )
		return notify_fail("你只能教授同帮派，且等级比你低的人武功。\n");
	if( environment(me)->query("group1") != me->query("group/id") )
		return notify_fail("你只在自己的帮会中教人武功。\n");
	if( !me->query_skill(skill,1) )
		return notify_fail("你根本不会这项技能啊。\n");
	if( me->is_busy() || me->is_fighting() )
		return notify_fail("你正忙着呢。\n");
	if( ob->is_busy() || ob->is_fighting() )
		return notify_fail(ob->name()+"正忙着呢。\n");
	if( (me->query_skill(skill,1)+100) < ob->query_skill(skill,1) )
		return notify_fail(ob->name()+"此项技能已经大大高与你，你无法再教授"+ob->name()+"什么了。\n");
	if( ob->query_temp("gteach")>time() && !(wizardp(me) && me->query("env/test")) )
		return notify_fail(ob->name()+"刚刚学习过，现在感觉过于劳累。\n");
	if( (me->query("group/gteach")+WORK_TIME*60+REST_TIME*60) < time() )
		me->set("group/gteach",time());
	if( (me->query("group/gteach")+WORK_TIME*60) < time() )
		return notify_fail("你教授别人武功的时间过长，只感到头昏脑胀，精力不济。\n");
	if( sizeof(ob->query_skills()) > MAX_SKILLS )
		return notify_fail(ob->name()+"所学技能过多，灵性不足，再也无法学习新的知识！\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	message_vision("$N开始反复教授$n「"+to_chinese(skill)+"」......\n", me,ob);
	delay = 10 + random(10);
	if( wizardp(me) && me->query("env/test") ) delay = 2;
	me->start_busy(delay);
	ob->start_busy(delay);
	call_out("teach_finish",delay*2-1,me,ob,skill);

	return 1;
}

void teach_finish(object me,object ob,string skill)
{
	int i,improve;

	if(!me || !ob) return;
	i = ob->query_skill(skill,1) - me->query_skill(skill,1);
	improve = random(ob->query_int(1) + me->query_int(1)) * 150;

	if( i <= 0 ) {
		message_vision("$n反复练习$N所教授的技能，技能精进了不少。\n", me,ob);
		ob->improve_skill(skill,improve);
	}
	else {
		message_vision("$n与$N反复切磋武功，似乎有所收获。\n", me,ob);
		ob->improve_skill(skill,improve * (80-i*7/10) / 100 );
	}

	ob->set_temp("gteach", time()+random(30) );
}
