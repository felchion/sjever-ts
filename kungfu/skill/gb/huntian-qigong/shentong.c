// shentong.c 神通
// Creat by Looklove 2001.10.11
// 写完以后才听说跟hs yun zixia很象，不过我原来是不知道zixia的。
// 而且这个原来有double_atteck 被river 去掉了，我可没照抄zixia。嘿嘿。
// 此注释请勿去掉。谢谢！

#include <ansi.h>
inherit F_CLEAN_UP;

int query_ap() {return 2;}

string exert_name(){ return HIY"神通"NOR; }

int exert(object me, object target)
{
	int skill;
	skill = me->query_skill("huntian-qigong",1);



	if( me->query_skill("medicine", 1) < 100 )
		return notify_fail("你的本草术理级别不够。\n");

	if( skill < 150 )
		return notify_fail("你的混天气功级别不够。\n");

	if( me->query("neili") < 500 )
		return notify_fail("你的内力不够。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你的内功不对。\n");

	if( !me->is_fighting() )
		return notify_fail("神通只能在战斗中使用。\n");

	if( me->query_temp("htqg/shentong") )
		return notify_fail("你已经在运神通中了。\n");

	message_vision(HIW"\n$N神色悲怆，纵声向天长啸，顿时全身真气鼓荡，衣袍飞舞。\n" NOR, me);

	me->add("neili", skill*20);
	if( me->query("neili") > me->query("max_neili") * 2 )
		me->set("neili", me->query("max_neili")*2-1);

	me->add("max_neili", -1);
	me->set_temp("htqg/shentong", 1);
	call_out("remove_effect", 1, me, skill);
	me->start_busy(random(2));
	return 1;
}

void remove_effect(object me, int skill)
{
	if( ! me) return;
	if( me->query_temp("htqg/shentong") ) {
		if( me->is_fighting() ) {
			call_out("remove_effect", 1, me, skill);
			return;
		}
		if( me->query("neili") > me->query("max_neili") * 2 )
			me->set("neili", me->query("max_neili")*2-1);
		me->delete_temp("htqg/shentong");
		message_vision(HIW"$N混天气功运行完毕，鼓荡在衣袍下的真气消失了。\n"NOR, me);
	}
}

int help(object me)
{
        write(HIY"\n混天气功之「神通」："NOR"\n");
        write(@HELP
        神通为混天气功重要技能之一。战斗中通过运神通可以大幅度提升当前内力。
        注意：运功结束会降低1点最大内力。
        要求：
                当前内力  500 以上；
                混天气功等级 150 级以上；
                本草术理等级 100 级以上；
                战斗中使用。
HELP
        );
        return 1;
}
