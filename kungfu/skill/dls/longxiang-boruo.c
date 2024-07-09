// longxiang-boruo.c 龙相般若功
// by iceland
// modefied by emnil@sj	2/2/2000

inherit FORCE;
#include <ansi.h>
#include "/kungfu/skill/force.h"

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{
	int i = (int)me->query_skill("longxiang-boruo", 1);

	if (((string)me->query("class") != "huanxi") && i > 20)
		return notify_fail("你没有出家，不能领会更高深的龙象般若功，只能修炼到第一层。\n");     

	if ((int)me->query_skill("force", 1) < 10)
		return notify_fail("要学龙象般若功先打打基本内功根基吧。\n");

	if ( me->query("gender") == "无性" && i > 40)
		return notify_fail("龙象般若功深奥复杂，你达不到至阳或至阴，还是放弃修炼吧。\n");

	if ((int)me->query_con() < 30 && i > 120 )
		return notify_fail("你的根骨不够，再修炼龙象般若功恐怕会走火入魔。\n");

	if ((int)me->query_skill("huanxi-chan",1)<120 && (int)me->query_skill("huanxi-chan",1)<i)
		return notify_fail("你对欢喜禅的理解不够，阻碍了你的龙象般若功的修炼。\n");

	return valid_public(me);        
}

int practice_skill(object me)
{
	if((int)me->query_skill("longxiang-boruo", 1) >= 200 ) {
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200) {
			me->improve_skill("longxiang-boruo", (int)me->query_int());
			me->add("neili", -100); 
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高龙象般若功了。\n");   
	}
	else return notify_fail("龙象般若功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	return __DIR__"dls/longxiang-boruo/" + func;
}

void skill_improved(object me)
{
	int i = (int)me->query_skill("longxiang-boruo", 1);   

	if (i == 20)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第一层了。\n");
	if (i == 40)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第二层了。\n");
	if (i == 60) 
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第三层了。\n");
	if (i == 80)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第四层了。\n");
	if (i == 100)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第五层了。\n");
	if (i == 120)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第六层了。\n");
	if (i == 140)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第七层了。\n");
	if (i == 160)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第八层了。\n");
	if (i == 180)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第九层了。\n");
	if (i == 200)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第十层了，已经具备了十龙十象的大力。\n");
	if (i == 220)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第十一层了。\n");
	if (i == 240)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了第十二层了。\n");
	if (i == 260)
		tell_object(me,HIW"龙象般若功十三层你已经修炼到了融会贯通的境界，只怕天下已无人是你的对手了。\n");
}

mapping exercise_msg(object me)
{
	if (me->query("gender")=="男性") {
		return ([
			"status_msg" : CYN + me->name()+"脸上神色平和，身形凝重，稳如山岳。"NOR,
			"start_my_msg" : CYN"你抉弃杂念盘膝坐定，手捏气诀，脑中一片空明，渐入无我境界，一道炽热的内息在任督二脉之间游走。\n"NOR,
			"start_other_msg" : CYN + me->name() +"盘膝坐定，左手阴，右手阳虚拢胸前，脸上神色平和，一丝红潮若隐若现，身形凝重，稳如山岳。\n"NOR,
			"halt_msg" : CYN"$N感到烦躁难耐，只得懈了内息，轻吁口气，身上涔涔透出层冷汗。\n"NOR,
			"end_my_msg" : CYN"你只觉神元归一，全身精力弥漫，无以复加，忍不住长啸一声，徐徐站了起来。\n"NOR,
			"end_other_msg" : CYN + me->name()+"一声清啸，双手两下一分，徐徐站了起来，眼中神光闪烁，令人不可逼视。\n"NOR
		]);
	}
	else {
		return ([
			"status_msg" : MAG + me->name()+"脸上神色娇艳，安定祥和，静若止水。"NOR,
			"start_my_msg" : MAG"你收敛心神闭目打坐，手搭气诀，调匀呼吸，感受天地之深邃，自然之精华，渐入无我境界。\n"NOR,
			"start_other_msg" : MAG + me->name() +"闭目盘膝打坐，双手十指如兰搭于胸前，呼吸渐渐悠长，脸上涌起一层红晕，安定祥和，静若止水。\n"NOR,
			"halt_msg" : MAG"$N感到呼吸紊乱，全身燥热，只好收功站了起来。\n"NOR,
			"end_my_msg" : MAG"你感到自己和天地融为一体，全身清爽如浴春风，忍不住舒畅的呻吟了一声，缓缓睁开了眼睛。\n"NOR,
			"end_other_msg" : MAG + me->name()+"鼻中发出一阵舒畅的呻吟，睁眼缓缓站了起来，伸衣袖擦了擦额头的汗水。\n"NOR
		]);
	}
}

int ob_hit(object ob, object me, int damage)
{
	int skill = me->query_skill("force");
	int skill2= ob->query_skill("force");
	int neili = me->query("neili");
	int neili2= ob->query("neili");
	int i = 1;
	mixed p, j;
	string msg;

	if(!me->query_temp("xs/shield"))
		return 1;

	p = me->is_busy();

	if ( random(skill) > (skill2*14/10/2-1) ) {
		if( (intp(p) && p < 2) || random(3)==1 ) {
			msg = MAG"$N"+MAG+"默运龙象般若护体大法，化去了$n"MAG;
			if(neili > neili2*2) {
				msg += "大部分的攻击！\n"NOR;
				i = -(damage/2 + random(damage));
			}
			else {
				if(neili < neili2/4) {
					msg += "很少的一点攻击！\n"NOR;
					i = -random(damage/4);
				}
				else {
					msg += "一部分的攻击！\n"NOR;
					i = -(damage/3+random(damage/2));
				}
			}
			if(wizardp(me) && me->query("env/test")) tell_object(me,sprintf("ob_hit: %d  ",i));
			if( objectp(me) && objectp(ob) )
				message_vision(msg, me, ob);		

		}
		else {
			j = me->is_busy();
			if ( intp(j))
				j = j/4 + random(j/2);
			else 
				j = 3;
			if(j < 3) j = 3;
			if(j > 10) j = 10;
			if(wizardp(me) && me->query("env/test")) tell_object(me,sprintf("busy:%d , dec busy:%d  ",me->is_busy(),j));
			me->add_busy(-j);
			msg = MAG"$N"MAG"试图摆脱困境，虽然没有成功，但手脚似乎灵活了不少！\n"NOR;
			
			if( objectp(me) && objectp(ob) )
				message_vision(msg, me, ob);		
		}
	}

	if(random(4)!=1) {
		if( ((damage+i) > 500) && (me->query("jing")>200) ) {
			j = (damage+i)/2;
			if( j > me->query("jing")) 
				j = me->query("jing") - 100;
			i = i -j ;
			me->add("jing", -j);
			if(wizardp(me) && me->query("env/test")) tell_object(me,sprintf("jing:%d ",j));
			message_vision(MAG"$N"MAG"眉头紧锁，努力以自己的精神念力化解$n"MAG"的攻势！\n"NOR, me, ob);		
		}
	}

	return i;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor, string attack_skill)
{
	int skill;
	int i = 1,j;	

	if(!me->query_temp("xs/longxiang"))
		return 1;
	
	skill = me->query_skill("force");

	i = damage_bonus * (skill+1) / 1000;
	i = i/2+ random(i);
	if(me->query_temp("weapon"))
		i = i*3;
	if(me->query_temp("apply/damage")>=100)
		i = i/3;

	if(wizardp(me) && me->query("env/test")) tell_object(me,sprintf("hit_ob: %d  ",i));
	j = me->query_skill("longxiang-boruo",1)/20;
	if ( j > 13) j = 13;
	message_vision(MAG"$N"+MAG+"陡然大喝一声，以"+CHINESE_D->chinese_number(j)+"龙"
		+CHINESE_D->chinese_number(j)+"象之力攻向$n！\n" NOR, me,victim);

	//i = i + ::force_hit(me,victim,damage_bonus,factor,attack_skill);

	return i;
} 
