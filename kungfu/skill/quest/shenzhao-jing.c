// shenzhao-jing.c
// written by Xiaoyao 98.7.25
// hehehe by Ciwei@SJ
// 正式quest skill by tangfeng@SJ 2004/11/1
// （不好意思 *_* ciwei）

inherit FORCE;
#include <ansi.h>
#include "/kungfu/skill/force.h"
#include <combat_msg.h>

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{
	return 1;
}

int practice_skill(object me)
{
        if((int)me->query_skill("shenzhao-jing", 1) >= 120 )
       {
	   if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
	   {
		   me->improve_skill("shenzhao-jing", (int)me->query_int());
		   me->add("neili", -100); 
		   me->add("potential", -1*(1+random(3)));
		   return 1;
	   }
	   else return notify_fail("你现在的修为不足以提高神照经了。\n");
       }
	else return notify_fail("神照经只能通过学习来增加熟练度。\n");
}

string exert_function_file(string func)
{
        return SKILL_D("quest/shenzhao-jing/") + func;
}

mapping exercise_msg(object me)
{
  return ([
    "status_msg" : HIW + me->name()+"双手摆于胸前，头顶白气越来越浓。"NOR,
    "start_my_msg" : "你慢慢盘膝而坐，双手摆于胸前，体内一股暖流随经脉缓缓流转。\n",
    "start_other_msg" :HIW + me->name()+"缓缓坐下，弯曲双手摆于胸前，头上白雾缓缓升起。\n"NOR,
    "halt_msg" : HIW "$N猛的睁开双眼，双手迅速回复体侧，仔细打量四周。\n"NOR,
    "end_my_msg" : "你一个周天行将下来，精神抖擞的站了起来。\n",
    "end_other_msg" : HIW "只见盘旋在"+me->name()+"头顶的白气慢慢消散，"+me->name()+"睁开双眼，站了起来。\n"NOR
     ]);
}

mixed hit_ob(object me, object ob, int damage)
{
	//if(!wizardp(me) || !me->query("env/shenzhao_test")) return;
	int busy,p;
	string msg;

	if(!me->query("quest/连城诀/武功/shenzhao")) return;
	if(me->query_temp("szj_power"))
	{
		me->add_temp("szj_power",-1);
		return;
	}
     //tell_object(me,"回合："+me->query_temp("combat_time")+"。\n");
	 if(me->query_temp("combat_time")>10 && random(3)  )
	 {
		me->set_temp("szj_power",1);
	 	me->delete_temp("combat_time");
	 	message_vision(HIW"$N运气神照经，越战斗越勇，毫无败退之像。\n"NOR,me);
	 	me->set_temp("action_flag",1);
	 	me->add_temp("apply/attack",me->query_temp("combat_time")*10);
	 	me->add_temp("apply/damage",me->query_temp("combat_time")*10);
	 	me->add_temp("apply/parry",me->query_temp("combat_time")*10);
	 	COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(2)?1:3);
	 	me->add_temp("apply/attack",-me->query_temp("combat_time")*10);
	 	me->add_temp("apply/damage",-me->query_temp("combat_time")*10);
	 	me->add_temp("apply/parry",-me->query_temp("combat_time")*10);
		me->delete_temp("action_flag");
	 	return;
	 }
	 else
		 if( !ob->busy()
	 && me->query_str()*10/2+random(me->query_str()*10/2) > random(ob->query_dex()*10) 
	 && random(me->query("neili"))>1000
	 && me->query("jingli")>1000
	 && random(3)==1
	 ){
		me->set_temp("szj_power",5);
	 	msg = HIW"$N一言不发，一伸手便抓住$n的胸口。\n$n还想挣扎，但一被$P抓住，竟立即软垂垂的毫不动弹。\n"NOR;
	 	busy = me->query_str()-ob->query_dex();
	 	if(busy<=0) busy = 1+random(2);
	 	if(busy>5) busy=5;
	 	ob->start_busy(busy);
	 	//message("wizard",sprintf("神照经：%s附加攻击%s busy:%d\n",me->name(),ob->name(),busy),environment(me));
	 	return msg;
	 }
	 else 
		 if(random(me->query("neili"))>1500
	 && me->query("jingli")>500
	 && me->query("jiali") > 30
	 && !me->query_temp("weapon")
	 && random(6)==1 
	 ){
		me->set_temp("szj_power",5);
	 	msg = HIW"不料$N双手力道并不使足，半途中回将过来，砰的一声，重重砸在$n腰间。\n"NOR;
	 	damage = (me->query_str()*10+me->query_skill("force")-ob->query_skill("parry",1))*5;
	 	if(damage<=0) damage = 1;
	 	if(damage>4000) damage = 4000;
	 	//message("wizard",sprintf("神照经：%s附加攻击%s damage 攻击上限:%d\n",me->name(),ob->name(),damage),environment(me));
	 	ob->receive_wound("qi", damage - ob->query_temp("apply/armor"), me);
		p=(int)ob->query("qi")*100 / (int)ob->query("max_qi");
		msg += damage_msg(damage, "内伤");
		msg += "( $n"+eff_status_msg(p)+" )\n";
	 	return msg;
	 }
}

mixed ob_hit(object ob,object me,int damage)
{
	//if(!wizardp(me) || !me->query("env/shenzhao_test")) return;
	int p;
	string msg;
	if(!me->query("quest/连城诀/武功/shenzhao")) return;
	if(me->query_skill("shenzhao-jing",1)<180
	 ||me->query_str()<40
	 ||me->query_con()<40
	 ||random(6)
	 ) return;
	 
	if(me->query("neili")>500
	&& me->query("jingli")>500
	&& me->query("jiali") > 30
	&& random(10)==1
	) {
		if(ob->query_temp("weapon"))
			msg = HIW"$n不闪不避，让那"+ob->query_temp("weapon")->query("name")+HIW"将及喉头数寸之处，突然一矮身，欺向$N左侧，手肘撞处，正中$P上腹。\n"NOR;
		else 
			msg = HIW"$n不闪不避，$N劲力及喉头数寸之处，突然一矮身，欺向$N左侧，手肘撞处，正中$P上腹。\n"NOR;
		damage = (me->query_str()*10+me->query_skill("force")-ob->query_skill("parry",1))*5;
		if(damage<=0) damage = 1;
	 	if(damage>4000) damage = 4000;
	 	ob->receive_damage("qi", damage - ob->query_temp("apply/armor"), me);
	 	//message("wizard",sprintf("神照经：%s附加攻击%s damage:%d\n",me->name(),ob->name(),damage),environment(me));
		p=(int)ob->query("qi")*100 / (int)ob->query("max_qi");
		msg += damage_msg(damage, "内伤");
		msg += "( $N"+eff_status_msg(p)+" )\n";
	 	return msg;
	}
	else
	{
		if(ob->query_temp("weapon"))
			msg = HIW"$n不闪不避，让那"+ob->query_temp("weapon")->query("name")+HIW"将及喉头数寸之处，突然身形一变，一矮身竟然恰好躲了过去。\n"NOR;
		else 
			msg = HIW"$n不闪不避，$N劲力及喉头数寸之处，突然身形一变，一矮身竟然恰好躲了过去。\n"NOR;
		message_vision(msg,ob,me);
	 	return -(8000-damage);
	}
}
