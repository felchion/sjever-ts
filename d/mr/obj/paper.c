// /u/beyond/mr/npc/obj/paper.c
// this is made by beyond
// update beyond

inherit ITEM;

#include <ansi.h>
string do_look();
void create()
{
	set_name(HIW"卷帛"NOR, ({ "murong paper", "paper" }) );
	set_weight(100);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "张");
		set("long", (: do_look :));
		set("value", 0);
		set("no_get",1);
		set("no_give",1);
		set("no_drop",1);
		set("material", "paper");
	}
	setup();
}

void init()
{
	object ob = this_object();
	object me = this_player();
	
	ob->set_temp("skill_lvl", me->query_temp("skill_lvl"));	//需要偷学的级数 
	ob->set_temp("learned_skill",5);			       //已经偷学的级数
	ob->set_temp("learned_amount",0);
	add_action("do_touxue", "touxue");
}

string do_look()
{
	string msg;
	object me = this_player();

	msg = "这是一张发黄的卷帛";
	if ( me->query_temp("skill_target"))
		msg += "，上面写着：偷学"+ HIW+chinese_number(me->query_temp("skill_lvl"))+NOR+"级 "+HIY+me->query_temp("skill_target")+HIR+" ("+me->query_temp("skill_id")+")。\n"NOR;
	else 
		msg += "。\n";
	return msg;
}

int do_touxue(string arg)
{
	object ob, me, obj, weapon;
	string skill, skill_name, teacher;
	string *v_skill;
	int master_skill, gin_cost, lvl;
	mapping prepare;

	me = this_player();
	obj = this_object();
       
	if (me->query_temp("job_done"))
		return notify_fail(HIW"你感觉已经渐渐领悟了"+me->query_temp("skill_target")+"的精髓，可以回去复命了。\n"NOR);
	       
	if (me->is_busy())
		return notify_fail("你现在正忙着呢。\n");

	if( me->query("family/family_name") != "姑苏慕容")
		return notify_fail("你不是姑苏慕容弟子，根本就不会偷学。\n");

	if( me->query_skill("douzhuan-xingyi", 1) < 100 )
		return notify_fail("你的斗转星移级别不够，无法运用偷学这个高级技能。\n");

	if(me->query_skill_mapped("parry") != "douzhuan-xingyi")
		return notify_fail("你现在没有用斗转星移，怎么能偷学到别人的武功？\n");

	if(!arg || (sscanf(arg, "%s from %s", skill, teacher)!=2 ))
		return notify_fail("指令格式：touxue <技能> from <某人>\n");

	if (!(ob = present(teacher, environment(me)))
	|| !ob->is_character()
	|| query_ip_number(ob) == query_ip_number(me))
		return notify_fail("你要向谁偷学？\n");

	if (!ob->is_fighting(me) )
		return notify_fail("不在战斗中怎么能偷学呢？\n");

	if( !living(ob) )
		return notify_fail("嗯....你得先把" + ob->name() + "弄醒再说。\n");

	if( skill != me->query_temp("skill_id") )
		return notify_fail("这一次你是要偷学这种武功么？\n");

	if( !master_skill = ob->query_skill(skill, 1) )
		return notify_fail("这项技能你恐怕必须找别人偷学了。\n");

	if( !me->query_condition("mr_job"))
		return notify_fail("你还没有领任务呢。\n");

	if (userp(ob)) {
		if (objectp(weapon = ob->query_temp("weapon"))) {
			skill_name = weapon->query("skill_type");
			v_skill = ({ ob->query_skill_mapped(skill_name) });
		} else {
			prepare = ob->query_skill_prepare();
			if (!prepare) prepare = ([]);
			v_skill = ({});
			switch (sizeof(prepare)) {
				case 2: v_skill += ({ ob->query_skill_mapped(keys(prepare)[1]) });
				case 1: v_skill += ({ ob->query_skill_mapped(keys(prepare)[0]) });
			}
		}
		if (!arrayp(v_skill) || member_array(skill, v_skill) < 0) {
			return notify_fail("对方现在并没有使用这项武功。\n");
		}
	}

	me->set_temp("master_exp", ob->query("combat_exp")/(userp(ob)?2:1));
	lvl = obj->query_temp("learned_skill");
	if( lvl >= me->query_temp("skill_lvl"))	{
		write(HIW"你感觉已经渐渐领悟了"+me->query_temp("skill_target")+"的精髓，可以回去复命了。\n"NOR);
		me->set_temp("job_done", 1);
		return 1;
	}

	gin_cost = 500 / me->query("int");

	if( (ob->query("jing") > gin_cost/5 + 1 )
	&&  (ob->query("jingli") > gin_cost/5 + 1 )) {
		if( userp(ob) )
		 ob->receive_damage("jing", gin_cost/5 + 1);
		 ob->receive_damage("jingli", gin_cost/5 + 1);
	}
	else {
		write("但是" + ob->name() + "显然太累了，你已经没有办法偷学到什么了。\n");
		return 1;
	}

	if( me->query("jing") > gin_cost && me->query("jingli") > gin_cost ){
	       if(skill_name = SKILL_D(skill)->query_skill_name(master_skill))
			write(sprintf("你根据斗转星移的精髓，仔细琢磨着%s的每个招数，尤其对「%s」这一招似乎有些心得。\n",
				ob->name(), skill_name));
		else
			write("你把所偷学的"+to_chinese(skill)+"的精华暗暗记在心中。\n");
		obj->add_skill(skill, me->query("int"));
	}
	else {
		write("你太累了，还是先休息一下吧。\n");
		return 1;
	}
// 取消NPC kill
//	if (random(10) >= 7 && !userp(ob)) {
//		message_vision("$N突然觉得有什么事情不大对了，双眼顿起杀意。\n", ob);
//		ob->kill_ob(me);
//	}

	me->receive_damage("jing", gin_cost);
	me->receive_damage("jingli", gin_cost);
	me->start_busy(2);
	return 1;
}

void add_skill(string skill, int amount)
{
	object ob = this_object();
	int lvl,sum;
	if( !find_object(SKILL_D(skill))
	  && file_size(SKILL_D(skill)+".c") < 0 )
		error("F_SKILL: No such skill (" + skill + ")\n");
	ob->add_temp("learned_amount",amount);
	lvl = (int)ob->query_temp("learned_skill");
	sum = (int)ob->query_temp("learned_amount");
	if(sum>(lvl*lvl)) {
		ob->add_temp("learned_skill",1);
		ob->set_temp("learned_amount",sum-lvl*lvl);
	}
	return;
}
