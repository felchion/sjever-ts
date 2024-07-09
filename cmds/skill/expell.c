// expell.c

#include <skill.h>

inherit	F_CLEAN_UP;

void create() {	seteuid(getuid()); }

int main(object	me, string arg)
{
	object ob;
	int i;

	if( !arg || !ob	= present(arg, environment(me))	)
		return notify_fail("你要将谁逐出师门？\n");

	if( !ob->is_character()	)
		return notify_fail("你逐不动他。\n");

	if( !userp(ob) )
		return notify_fail("你只能开除玩家所扮演的人物。\n");

	if( userp(me) )
		return notify_fail("暂时关闭。\n");

	if (me->query("family/privs")==-1
	&& (string)me->query("family/family_name")==(string)ob->query("family/family_name"))
	{
		message_vision("\n$N对着$n说道：从今天起，你再也不是我"
			+ me->query("family/family_name") + "的弟子了，你走吧！\n\n", me, ob
		);
		tell_object(ob, "\n你被" + me->query("family/title") + "开革出师门了！\n\n");
	}
	else if (ob->is_apprentice_of(me))
	{
		message_vision("\n$N对着$n说道：从今天起，你我师徒恩断情绝，你走吧！\n\n", me, ob);
		tell_object(ob,	"\n你被师父开革出师门了！\n\n");
	} else
		return notify_fail("这个人不是你的弟子。\n");

	i = ob->query("combat_exp") / 200;
	if (i > 30000) i = 30000;
	if (i < 150) i = 0;

	ob->set("expell/family_name",ob->query("family/family_name"));
	ob->set("expell/master_id", ob->query("family/master_id"));
	ob->set("expell/master_name", ob->query("family/master_name"));
	ob->delete("family");
	ob->delete("vendetta_mark");
	ob->delete("title");
        ob->add("combat_exp", -i);
	ob->skill_death_penalty();
	tell_object(ob, "你被惩罚了"+chinese_number(i)+"点经验，且所有武功被降了半级。\n");
	return 1;
}

int help(object	me)
{
	write(@HELP
指令格式 : expell|kaichu <某人>

这个指令可以让你开除不成才的弟子，请你在开除一名弟子之前务必审慎考虑。
HELP
	);
	return 1;
}
