// yunu-xinjing.c 玉女心经
// Modify By River 10/98

inherit FORCE;
#include <ansi.h>

int valid_enable(string usage) { return usage == "force"; }

#include "../force.h"

int valid_learn(object me)
{
        int lvl = (int)me->query_skill("yunu-xinjing", 1);

	if ( me->query("gender") == "无性") 
		return notify_fail("你无根无性，阴阳不调，难以领会高深的玉女心经神功。\n");

        if ( lvl < 120)
                return notify_fail("你现在还只能通过合修来领会高深的玉女心经神功。\n");

        if ((int)me->query_skill("yunu-xinjing", 1) > me->query_skill("force", 1) + 10
          && me->query_skill("yunu-xinjing", 1) >= 200 )
               return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

        return valid_public(me);
}

int practice_skill(object me)
{
	if((int)me->query_skill("yunu-xinjing", 1) >= 200 ){
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200){
			me->improve_skill("yunu-xinjing", (int)me->query_int());
			me->add("neili", -100); 
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高玉女心经了。\n");
	}
	else return notify_fail("你现在的玉女心经修为只能用学(learn)的来增加熟练度。\n");

}

string exert_function_file(string func)
{
	return __DIR__"gm/yunu-xinjing/" + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIB + me->name()+"姿态悠雅，神态清冷。"NOR,
		"start_my_msg" : "你轻轻的吸一口气，闭上眼睛，运起玉女心经，内息在脉络中开始运转。\n",
		"start_other_msg" : me->name()+"神态突然开始变的清冷无比，可是姿势又变的悠雅异常。\n",
		"halt_msg" : "$N内息一转，迅速收气，停止了内息的运转。\n",
		"end_my_msg" : "你慢慢收气，归入丹田，睁开眼睛，轻轻的吐了一口气。\n",
		"end_other_msg" : "只见"+me->name()+"睁开眼睛，轻轻的吐出了一口气，神态恢复自然。\n"
	]);
}

int hit_ob(object me, object ob, int damage)
{
	object weapon;	
	if( me->query_int(1) > 40
	 && me->query_dex(1) > 40
	 && me->query_skill_mapped("hand") == "tianluo-diwang"
	 && me->query_skill_mapped("cuff") == "meinu-quanfa"
	 && me->query_skill("hand") > 300
	 && me->query_skill("cuff") > 300
	 && me->query("neili") > 500
	 && me->query("jingli") > 500
	 && me->query("family/family_name") == "古墓派"
	 && me->query("env/玉女心经") == "融合"
	 && !me->query_temp("anran")
	 &&!ob->is_busy()
	 &&!ob->is_lost()
	 &&!random(4)){
		if (weapon=me->query_temp("weapon"))
			message_vision(HIC"$N将手中的"+weapon->query("name")+HIC"一收，",me);
		else
			message_vision(HIC"$N招式一变，",me);

		if(ob->query("class") != "bonze"
		 && ob->query("class") != "huanxi" 
		 && me->query("gender") != ob->query("gender")){
			message_vision(HIC"施展出古墓派嫡传美女拳法！\n", me);
			if (random(me->query_per()) > ob->query_per()/2){
				message_vision(HIR"$n心神一乱，险些被$N击中要害，一阵手忙脚乱！\n"NOR, me, ob);
				ob->add_busy(2+random(3));
				me->add("jingli",-50);
				me->add("neili",-100);
			}
			else
				message_vision(HIC"$n心神一乱，连忙自行镇慑心神！\n"NOR, me, ob);
		}
		else {
			message_vision(HIC"施展出古墓派嫡传天罗地网势！\n", me);
			if (random(me->query_dex()) > ob->query_dex()/2){
				message_vision(HIR"$n连连受挫，自顾不暇，无法准确的向$N攻击！\n"NOR, me, ob);
				ob->start_lost(2+random(3));
				me->add("neili",-50);
				me->add("jingli",-100);
			}
		else
			message_vision(HIC"$n以快打快，破解了$N的奇袭！\n"NOR, me, ob);
		}
	}
}
