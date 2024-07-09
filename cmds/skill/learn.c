// learn.c
// LinuX@SJ 2004/02/28

#include <ansi.h>
#include <job_mul.h>
#include <skill.h>

inherit F_CLEAN_UP;

string *reject_msg = ({
	"说道：您太客气了，这怎么敢当？\n",
	"打量了你几下，摇了摇头。\n",
	"象是受宠若惊一样，说道：请教？这怎么敢当？\n",
	"笑着说道：您见笑了，我这点雕虫小技怎够资格「指点」您什么？\n",
});

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string skill, teacher, skill_name;
	object ob;
	int i = 1, j, k, l, m;
	int improve;
	int master_skill, my_skill, gin_cost;

	if (me->is_busy())
		return notify_fail("你现在正忙着呢。\n");

	if (!arg ||
		(
			sscanf(arg, "%s %s %d", teacher, skill, i) != 3
			&& sscanf(arg, "%s from %s", skill, teacher) != 2
			&& sscanf(arg, "%s %s", teacher, skill) != 2
		)
	)
		return notify_fail("指令格式：learn|xue <某人> <技能> [次数]\n");

	if (me->is_fighting())
		return notify_fail("临阵磨枪？来不及啦。\n");

	if (!(ob = present(teacher, environment(me))) || !ob->is_character() || ob->query("race") != "人类")
		return notify_fail("你要向谁求教？\n");

	if (!living(ob))
		return notify_fail("嗯....你得先把" + ob->name() + "弄醒再说。\n");

	if (!master_skill = ob->query_skill(skill, 1))
		return notify_fail("这项技能你恐怕必须找别人学了。\n");

	if (me->query("potential") < i || i < 1)
		return notify_fail("你没有这么多潜能来学习，没有办法再成长了。\n");

	if( i > 50 )
		return notify_fail("一次学习这么多，是没法进一步理解的，少点吧。\n");

	notify_fail(ob ->name() + reject_msg[random(sizeof(reject_msg))]);
	if (!me->is_apprentice_of(ob)
	&& !ob->recognize_apprentice(me))
		return 0;

	if (me->query_skill(skill, 1) >= 450 && SKILL_D(skill)->type() == "martial")
		return notify_fail("这项技能你已经无法通过学习来提高了。\n");

	if (me->query("id") == ob->query("marry/id")) {
		notify_fail(ob->name()+"的脸色有点难堪，似乎是不想传授你这样功夫。\n");
		if (me->query_skill(skill, 1) > ob->query_skill(skill, 1) - me->query("divorce") * 20)
			return 0;
		if (SKILL_D(skill)->type2() == "worker")
			return 0;
		if (SKILL_D(skill)->valid_enable("force")
		&& me->query("family/family_name") != ob->query("family/family_name"))
			return 0;
	}

	notify_fail(ob->name() + "不愿意教你这项技能。\n");

	if (ob->prevent_learn(me, skill))
		return 0;

	my_skill = me->query_skill(skill, 1);
	if (my_skill >= master_skill)
		return notify_fail("这项技能你的程度已经不输你师父了。\n");

	notify_fail("依你目前的能力，没有办法学习这种技能。\n");
	if (!SKILL_D(skill)->valid_learn(me))
		return 0;

	m = me->query_int();
	gin_cost = (90 + random(10)) / m;

	if (!my_skill) {
		gin_cost *= 2;
		me->set_skill(skill, 0);
	}

	gin_cost += my_skill / 10;


	teacher = ob->name();
	if (i > 1)
		teacher += "反复";

	write("你向" + teacher + "请教有关「" + to_chinese(skill) + "」的疑问。\n");

	if (ob->query("env/no_teach"))
		return notify_fail("但是" + ob->name() + "现在并不准备回答你的问题。\n");

	gin_cost *= i;
	if (userp(ob)) {
		tell_object(ob, me->name() + "向你请教有关「" + to_chinese(skill) + "」的疑问。\n");
		if (ob->query("jing") < gin_cost / 5 + 10) {
			write("但是" + ob->name() + "显然太累了，没有办法教你什么。\n");
			tell_object(ob, "但是你太累了，没有办法教" + me->name() + "。\n");
			return 1;
		}
		ob->receive_damage("jing", gin_cost/5 + 1);
	}

	if (!random(10))
		tell_room(environment(me), me->name() + "正在向" + ob->name() + "请教问题。\n", ({ me, ob }));

	if (me->query("jing") > gin_cost + 10) {
		if ((SKILL_D(skill)->type()=="martial"
		||   SKILL_D(skill)->type2() == "worker")
		&& my_skill * my_skill * my_skill / 10 > me->query("combat_exp"))
			return notify_fail("也许是缺乏实战经验，你对"+ob->name()+"的回答总是无法领会。\n");

		if(skill_name = SKILL_D(skill)->query_skill_name(my_skill))
			write("你开始听取"+teacher+"的指导，对「"+skill_name+"」这一招进行苦思。\n");
		else write("你听了"+teacher+"的指导，似乎有些心得。\n");
		
		if (me->query("registered") > 2 && me->query_temp("ggs/started") && me->query("ggs/started") ){
			if (!random(5))
			write("你参照自己在"HIG"鬼谷算术"NOR"上的修为，运神通数，觉得自己学习起来事半功倍。\n");
			if (!random(10))
			tell_room(environment(me), me->name() + "正在参照"HIG"鬼谷算术"NOR"上的修为，运神通数，向" + ob->name() + "请教问题。\n", ({ me }));
		}
		if (me->query("age") < 30) {
			l = (35 - me->query("age")) / 3;
			if (me->query("age") <= 20)
				l += l;
		}
		//my_skill /= 10;
		for (k = 0;;) {
			j = m * 3 / 8 + my_skill/2 + m / 4;
			me->add("potential", -1);
			improve = j+l+random(++k);
		
			/*奖励控制开始*/
			if (me->query("registered")>2) improve = improve * LEARN_MUL * VIP_LEARN_MUL /10000;
		  else improve = improve * LEARN_MUL /100;
		  /*奖励控制结束*/ 
		  if(me->worker_skill("highest")>350) improve = improve*7/10;
			else if(me->worker_skill("highest")>220) improve = improve*8/10;
			if(me->worker_skill("limit")>600) improve = improve*8/10;
		  improve = to_int(improve*me->query("int")/15.0);      
			if (me->query("registered") >2 && me->query_temp("ggs/started") && me->query("ggs/started") || skill=="literate")
				me->improve_skill(skill, improve*2); // 2 倍
			else
				me->improve_skill(skill, improve);
			if (wizardp(me) && me->query("env/test"))
			write (sprintf("测试信息：鬼谷术增加 %d 点，其他增加 %d 点\n",improve*2,improve));
			
			if (k >= i)
				break;
			if (!me->is_apprentice_of(ob)
			&& !ob->recognize_apprentice(me))
				break;
			if (!SKILL_D(skill)->valid_learn(me))
				break;
			my_skill = me->query_skill(skill, 1);
			if (my_skill >= master_skill)
				break;
		}
	} else {
		gin_cost = me->query("jing");
		write("你今天太累了，结果什么也没有学到。\n");
	}

	me->receive_damage("jing", gin_cost);
	if (i > 1)
		me->add_busy(1);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : learn|xue <某人> <技能> [次数]

这个指令可以让你向别人请教有关某一种技能的疑难问题，当然，你请教的对象在这
项技能上的造诣必须比你高，而你经由这种方式学习得来的技能也不可能高於你所请
教的人，然而因为这种学习方式相当於一种「经验的传承」，因此学习可以说是熟悉
一种新技能最快的方法。

例：xue master force 10  -> 从 master 那里学 10 次 force

其中，次数最多为 50；对每回学习而言，次数多的比次数少的进步速度有额外的增加。

此外学习也需要消耗一些精力，而消耗的精力跟你自己、与你学习对象的悟性有关。

其他相关指令 : apprentice, practice, skills, study
HELP
	);
	return 1;
}
