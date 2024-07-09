// write by snowman@SJ

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string perform_name() {return HIR"火球"NOR;}
int query_ap() {return 4;}

int check_pfm(object me, object target, object ob);

int perform(object me, object target)
{
      	object ob;
        if( !target ) target = offensive_target(me);

        if( !objectp(target)
        || !target->is_character()
        || !me->is_fighting(target)
        || !necessary(target) )
                return notify_fail("「火球」攻击只能对战斗中的对手使用。\n");
      
      	if( (int)me->query("max_neili") < 2500 ) 
                return notify_fail("你发现自己内力太弱，无法驱动「火球」进行攻击。\n");
        if( (int)me->query_skill("huagong-dafa",1) < 180 )
                return notify_fail("你发现自己的化功大法修为不够，无法使用「火球」进行攻击。\n");
        if( (int)me->query_skill("poison",1) < 150 )
                return notify_fail("你发现自己体内聚毒不够，无法使用「火球」进行攻击。\n");
        if( (int)me->query_skill("chousui-zhang", 1) < 180 )
                return notify_fail("你发现自己的星宿毒掌还不够娴熟，无法使用「火球」进行攻击。\n");
        if( objectp(me->query_temp("weapon")) )
                return notify_fail("你必须空手才能使用「火球」！\n");        
        if( !objectp(ob = present("huo yan", environment(me))) )
                return notify_fail("没有火堆怎么驱动「火球」进行攻击？\n");
        if( me->query_temp("pfm_chousui") )
                return notify_fail("你正在驱动火堆进行攻击！\n");

       message_vision(HIR"\n$N厉声大喝，掌力加盛，火焰中突然生出一个斗大的"RED"火球"HIR"，在空中骨碌碌的迅速转动！\n"NOR,me);

       me->set_temp("pfm_chousui", 1);
       check_pfm(me, target, ob);
       return 1;
}

int check_pfm(object me, object target, object ob)
{
	int i,bonus,neili, lvl;
	object *inv;

	if( !me ) return 0;
	me->delete_temp("pfm_chousui");

	if( !living(me) || me->is_ghost() ) return 0;

	if(!target
	|| !living(target)
	|| !me->is_fighting(target)
	|| environment(me)!=environment(target)){
		message_vision(HIY"$N衣袖再拂，将升起的"RED"火球"HIY"压回火焰之中。\n"NOR,me);
		return 1;
	}

	lvl = (int)me->query_skill("chousui-zhang", 1);
	neili = (int)me->query("neili");

	if(!objectp(present(ob, environment(me)))){
		message_vision(HIY"$N眼看火堆熄灭，散开内息，放下举起的双手。\n"NOR,me);
		return 1;
	}
	if (me->query_skill_prepared("strike") != "chousui-zhang"
	|| me->query_skill_mapped("strike") != "chousui-zhang"
	|| me->query_skill_mapped("parry") != "chousui-zhang"){
		tell_object(me,"你必须用抽髓掌才能使用「火球」进行攻击。\n");
		return 1;
	}
	if( me->query_skill_mapped("force") != "huagong-dafa"){
		tell_object(me,"你发现自己所用的内功无法进行「火球」攻击。\n");
		return 1;
	}
	if(neili < 1000){
		tell_object(me,"你发现自己现在真气太弱，使不出「火球」进行攻击。\n");
		return 1;
	}

  message_vision(WHT"\n此时$N脸上已无半点血色，一口口鲜血不住向"RED"火球"WHT"中吐去!\n"HIR"刹那间,火势一盛，嗤嗤两声轻响，"
	"爆出几朵火花，"RED"火球"HIR"陡然变大向$n疾冲过来！\n"NOR,me,target);
	
	me->receive_damage("neili", 500+lvl);
	
	me->add_temp("apply/pfm_attack",240+lvl/5);
	if(COMBAT_D->do_attack(me, target, 0, 0, 1)){
		target->add_condition("xx_poison", 10);
		inv = filter_array(all_inventory(target),(:$1->query("equipped")=="worn":));
		inv -= ({0});
		bonus = target->query_temp("apply/armor")/2;
		if( sizeof(inv) >= 1 ){
			if( me->query_str()*2/3 > random(target->query_str())){
				message_vision( HIY"猛烈间火球温度飙升，火花迸射，炸开了$p的护具！\n" NOR,me,target);					
				for(i=0; i<sizeof(inv); i++){
					inv[i]->unequip();
					inv[i]->move(environment(target));
					inv[i]->set("name", "粉碎的" + inv[i]->query("name"));
					inv[i]->delete("value");
					inv[i]->set("armor_prop", 0);
				}				
			}
	
			me->add_temp("apply/force",500+lvl);
			me->add_temp("apply/pfm_force",230+lvl/5-bonus);
			target->set_temp("must_be_hit",1);
			COMBAT_D->do_attack(me, target,0,random(3)?1:3);
			target->delete_temp("must_be_hit");
			me->add_temp("apply/force",-500-lvl);
			me->add_temp("apply/pfm_force",-230-lvl/5+bonus);
		}

		else{
			//target has no armor, so, hehe...
			me->add_temp("apply/force",500+lvl);
			me->add_temp("apply/pfm_force",230+lvl/5);
			target->set_temp("must_be_hit",1);
			COMBAT_D->do_attack(me, target,0,3);
			target->delete_temp("must_be_hit");
			me->add_temp("apply/force",-500-lvl);
			me->add_temp("apply/pfm_force",-230-lvl/5);
		}
	}
	else{
		if(!target->is_busy())	target->start_busy(2);
		message_vision(HIY"\n$p眼看就要被打中，冒死一个“铁板桥”，全身自膝盖处向后倒去，堪堪避过火球"HIY"！\n" NOR,me,target);
	
	}
  me->add_temp("apply/pfm_attack",-240-lvl/5);
	me->start_busy(1);
	return 1;
}

int help(object me)
{
        write(RED"\n抽髓掌之「火球」："NOR"\n\n");
        write(@HELP
        要求：  最大内力 2500 以上；
                毒技等级 150 以上；
                抽髓掌等级 180 以上；
                化功大法等级 180 以上；                
                且手中无兵器。

        
HELP
        );
        return 1;
}