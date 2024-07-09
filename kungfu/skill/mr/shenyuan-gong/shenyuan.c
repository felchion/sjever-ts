// shenyuan.c yun shenyuan 可以在战斗中增加相当的防御

#include <ansi.h>
#include <skill.h>
#include <combat.h>

inherit F_CLEAN_UP;


int query_ap() {return 2;}

void remove_effect(object me, int skill);

int exert(object me, object target)
{
	int skill;

	if (!me->is_fighting())
		return notify_fail("你只能在战斗中使用神元功。\n");

	skill = me->query_skill("shenyuan-gong",1);
	
	if( (int)me->query_skill("shenyuan-gong",1) < 100 )
		return notify_fail("你目前尚不能使用神元功。\n");

	if( (int)me->query("neili") < skill  )
		return notify_fail("你的内力不够。\n");
 
	if( (int)me->query_temp("shenyuan") ) 
		return notify_fail("你已经在运用神元功了。\n");

	me->add("neili", -skill);
	me->add_temp("apply/pfm_parry", 120+skill/8);
	me->add_temp("apply/pfm_dodge", 120+skill/8);

	message_vision(HIY "$N运起慕容绝学神元功，似乎熟知武功路数，攻守上轻松多了。\n" NOR, me);

	me->set_temp("shenyuan", 1);
	call_out("remove_effect", 10+skill/10, me, skill);
	return 1;
}

void remove_effect(object me, int skill)
{
	if (!me) return;
	  me->delete_temp("shenyuan");
	  me->add_temp("apply/pfm_parry", -120-skill/8);
	  me->add_temp("apply/pfm_dodge", -120-skill/8);
	if (me->is_fighting())
		tell_object(me, HIW"对手逐渐适应了你的防守招式，渐渐加强了攻势。\n"NOR);	
}

string exert_name() { return HIW"神元功"NOR; }

int help(object me) {
        write(HIW"\n「神元功」："NOR"\n\n");
        write(@HELP        
        可以在战斗中提高相当的防御。
        
        要求：  神元功等级 100 以上；
                当前内力高于内功等级；             

HELP
        );
        return 1;
}