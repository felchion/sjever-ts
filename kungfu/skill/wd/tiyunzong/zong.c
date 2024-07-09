#include <ansi.h>
//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

void remove_effect(object me,int count);

string perform_name() {return HIY"梯云纵"NOR;}

int perform(object me, object target)
{

	if( !me->is_fighting() && (int)me->query_skill("tiyunzong",1)<350 )
	return notify_fail("你现在只能在战斗中使用「梯云纵」！\n");
	if( (int)me->query_skill("tiyunzong",1) < 120 )
	return notify_fail("你的梯云纵不够娴熟，不会使用「梯云纵」！\n");
	if( (int)me->query_skill("yinyun-ziqi",1) < 120 )
	return notify_fail("你的氤氲紫气功等级不够，不能使用「梯云纵」！\n");
	if (me->query_skill_mapped("force") != "yinyun-ziqi")
	return notify_fail("你的内功不对，不能使用「梯云纵」。\n");
	if( (int)me->query_dex() < 28 )
	return notify_fail("你的身法太低，不能使用「梯云纵」！\n");

	if( (int)me->query_skill("dodge",1) < 120 )
	return notify_fail("你的基本轻功太差，身体笨拙，不能使用「梯云纵」！\n");

	if( (int)me->query("neili") < 600 )
	return notify_fail("你的内力太少了，无法使用出「梯云纵」！\n");

	if (me->query_skill_mapped("dodge") != "tiyunzong")
	return notify_fail("你现在激发的轻身数使用「梯云纵」绝技。\n");

	if( me->query_temp("zong") )
	return notify_fail("你正在使用梯云纵的特殊防御「梯云纵」！\n");

	message_vision(HIY"$N突然深吸一口气，一抬腿猛的拔高数丈，升势刚尽，双腿连续踢出，" +
	"身体又上升丈许，才有如大鸟般盘旋落下！\n" NOR, me);

	me->set_temp("zong",1);
	me->add_temp("apply/pfm_dodge", 150);
	me->add("neili", -200);
	call_out("remove_effect", 1, me,me->query_skill("tiyunzong",1)/10);
	return 1;
}

void remove_effect(object me,int count)
{
	if( !me ) return;
	if( count < 1 || !me->is_fighting()){
		me->add_temp("apply/pfm_dodge", -150);
		me->delete_temp("zong");
		tell_object(me, HIY"你散去丹田凝聚的内力，混身经脉真气流动，气定神闲，精神弈弈。\n"NOR);
		tell_room(environment(me), HIY + me->name()+"全身骨头一阵轻响，散去了混身的功力。\n"NOR,  ({ me }));
		return;
	}
	call_out("remove_effect", 1 , me ,count -1);
}

int help(object me)
{
        write(WHT"\n梯云纵「纵」字诀："NOR"\n");
        write(@HELP
        武当轻功，当世可谓独步武林，天下轻功无出其右，临敌之时使出「纵」字
        诀，可以用来大幅度提升自己的轻功有效等级，增加胜算或逃脱机率。
        
        要求：  梯云纵等级 120 以上；
                氤氲紫气等级 120 以上；
                基本轻功等级 120 以上；
                内力 600 以上；
                后天身法 28 以上。
HELP
        );
        return 1;
}
