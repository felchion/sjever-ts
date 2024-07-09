// yinyang.c   冷泉神功之「阴阳无常」
// Create By lsxk@hsbbs 2007/5/12
// By Spiderii@ty 降低效果.免busy.免no_exert.no_perform.近乎无敌了。
#include <ansi.h>
string exert_name() {return HIY"阴阳无常"NOR;}
int check(object me);
int exert(object me)
{


   if( (int)me->query_skill("lengquan-shengong", 1) < 200 )
           return notify_fail("你的冷泉神功修为还不够。\n");

   if( (int)me->query_skill("force", 1) < 200 )
           return notify_fail("你的基本内功修为还不够。\n");

   if( (int)me->query_skill("medicine", 1) < 120 )
		return notify_fail("你的本草术理等级不够。\n");

   if( (int)me->query("max_neili") < 2000 )
           return notify_fail("你的内力修为还不够。\n");

   if( (int)me->query("max_jingli") < 1600 )
           return notify_fail("你的精力修为还不够。\n");

   if( (int)me->query("neili") < 600 )
		return notify_fail("你的真气不够。\n");

   if( (int)me->query("jingli") < 400 ) 
		return notify_fail("你的精力不够。\n");

   if( me->query_temp("lqsg/yy") )
           return notify_fail("你正在运用冷泉神功之「阴阳无常」绝技！\n");


   message_vision(HIY"冷泉神功当真厉害非常，只见$N深深吸了一口气，冷泉内劲便布满全身，或攻、或守全随意而行！\n"NOR, me);
   me->set_temp("lqsg/yy", 1);  
   me->add("neili", - 400);
	check(me);
   me->start_exert(random(3),"「阴阳无常」");
	me->start_busy(random(2));
	return 1;
}

int check(object me)
{
   if( !me || !me->query_temp("lqsg/yy") ) return 0;

	if( me->is_ghost() || !living(me) )
           me->delete_temp("lqsg/yy");

   if( me->query("jingli") < 400 
   || me->query("neili") < 600
	|| me->query("qi") < 1		// 避免 BUG，气（-1）还恢复
	|| !me->is_fighting()
   || (userp(me) && me->query_skill_mapped("force") != "lengquan-shengong") ) {
		me->start_call_out( (: call_other, __FILE__, "remove_effect", me :), 1);
	}
	else {
           if( me->query("qi") < (int)me->query("max_qi") && !me->is_busy() && !random(6)) {
                   message_vision(HIM"$N一皱眉，催动冷泉神功为自己疗伤，就一会功夫，$N看上去竟如毫无受伤一般！\n"NOR,me);
                   me->add("neili", -80 );
			me->add("eff_qi", me->query_skill("medicine", 1));
                   me->add("eff_qi", (int)me->query_skill("lengquan-shengong", 1)/8);
			if( me->query("eff_qi") > me->query("max_qi") )
				me->set("eff_qi", me->query("max_qi"));
                        me->add("qi", me->query_skill("lengquan-shengong")*3/2);
                        if( me->query("qi") > me->query("eff_qi") )
				me->set("qi", me->query("eff_qi") );
		}
           else if( me->query("jing") < (int)me->query("max_jing") && !me->is_busy() && !random(6) ) {
                   message_vision(HIC"$N一皱眉，正催动冷泉神功疗伤自己精神疲劳，就一会功夫，$N看上去就容光焕发，神采奕奕！\n"NOR,me);
                   me->add("neili", -80 );
                   me->add("eff_jing", (int)me->query_skill("lengquan-shengong", 1)/8);
                   if( me->query("eff_jing") > me->query("max_jing") )
                          me->set("eff_jing", me->query("max_jing"));
           }


        if((int)me->query_skill("lengquan-shengong",1) > 300 && !random(6)){
           if( me->query_condition("no_perform") ){
               message_vision(HIR"$N发现自己已被封招，便连连催动冷泉神功，望能助自己行动恢复正常！\n"NOR,me);
               me->apply_condition("no_perform",random((int)me->query_condition("no_perform")));
           }
        if( me->is_busy() && !random(6) ){
               message_vision(HIW"$N行动一受制，立即潜运冷泉内劲，慢慢使自己脱离险境！\n"NOR,me);
               me->start_busy(random((int)me->query_busy()));
           }

           if( me->query_condition("no_exert") && !random(6)){
               message_vision(HIY"$N发现自己已被闭气，急忙暗运冷泉神功，望能助自己立即恢复受损经络穴道！\n"NOR,me);
               me->apply_condition("no_exert",random((int)me->query_condition("no_exert")));
           }

           if( me->query_condition("no_force") && !random(6)){
               message_vision(HIC"$N发现自己已内息不匀，急忙催动冷泉神功，调理内息！\n"NOR,me);
               me->apply_condition("no_force",random((int)me->query_condition("no_force")));
           }
        }

           call_out("check", 1, me); 
	}
	return 1;
}       

void remove_effect(object me)
{
        if( !me || !me->query_temp("lqsg/yy") ) return;
        me->start_exert(0);
        me->delete_temp("lqsg/yy");
        message_vision(HIY"$N「阴阳无常」绝技用毕，长长地呼出了一口浊气！\n"NOR, me);
}

int help(object me)
{
        write(WHT"\n冷泉神功「"HIY"阴阳无常"WHT"」："NOR"\n\n");
        write(@HELP
   冷泉神功乃当世内功一绝,非常人能道也!此「阴阳无常」绝技
   乃是冷泉神功最大特色，在战斗中，不但有机会治疗自己气的
   损伤，也有机会治疗精的受伤，但这个还不是全部，冷泉神功
   到达一定境界后，甚至可以在战斗中调理内息，解除自身的各
   种不良忙乱状态。

   要求：  当前内力 600 以上；
           当前精力 400 以上；
           最大内力 2000 以上；
           最大精力 1600 以上；
           冷泉神功等级 200 以上；
           基本内功等级 200 以上；
           本草术理等级 120 以上；
HELP
        );
        return 1;
}
