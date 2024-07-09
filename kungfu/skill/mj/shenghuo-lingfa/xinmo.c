// xinmo.c 圣火令法--心魔
// By River@SJ 
#include <ansi.h>
#include <combat.h> 

inherit F_SSERVER;

int query_ap() {return 2;}

string  *msg = ({
HIW"$N直抢而前，脚下一踬，忽然一个筋斗摔了过去，双手$w"HIW"却已向$n胸口拍了过去"NOR,
HIR"$N平平一令击出，$w"HIR"在半途古古怪怪的转了个弯，拍的一响，拍向$n的$l"NOR,
MAG"$N将$w"MAG"插入怀中，右手一探，已抓住$n左脚，硬生生将$n在半空中拉了回来"NOR,
HIY"$N双手$w"HIY"急挥横扫，$n身形一转，向左避开，不意拍的一响，$l已被$w"HIY"击中"NOR,
YEL"$n左手倏地抓住了$N左手的$w"YEL"。岂知那$w"YEL"尾端向上弹起，拍的一响，正好打中$n手腕"NOR,
GRN"$N坐倒在地，双拳猛捶自己胸膛，待$n靠近突然间脚踢飞沙，抽出$w"GRN"向$n的$l疾刺"NOR,
HIG"$N上身微斜，右臂弯过，$w"HIG"从莫名其妙的方位转了过来，拍的一声，向$n的$l重重打去"NOR
});

void remove_effect(object me,object weapon,int count);

int perform(object me, object target)
{
	int lvl;
	object weapon;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target) )
		return notify_fail("你只能在战斗中体验圣火令法的心魔威力。\n");

	if((int)me->query("neili") < 1000 ) 
		return notify_fail("你的内力不够。\n");

	if((int)me->query("jingli") < 800 ) 
		return notify_fail("你的精力不够。\n");

	if((int)me->query_temp("ling")) 
		return notify_fail("你已经在运功中了。\n");

	lvl = (int)me->query_skill("shenghuo-lingfa", 1);

	if(lvl < 120)
		return notify_fail("你的圣火令法等级还不够。\n");

	if(me->query_skill("qiankun-danuoyi", 1) < 120)
		return notify_fail("你的乾坤大挪移等级还不够。\n");

	if(! me->query_skill("jiuyang-shengong", 1))
	if(me->query_skill("shenghuo-shengong", 1) < 120)
		return notify_fail("你的圣火神功等级还不够。\n");

	if( !(weapon=me->query_temp("weapon"))
	|| weapon->query("skill_type") != "dagger" )
		return notify_fail("你不用兵器怎么提高你的圣火令法的心魔威力？\n");

	if ( me->query_skill_mapped("dagger") != "shenghuo-lingfa"
	 || me->query_skill_mapped("parry") != "qiankun-danuoyi"
	 || me->query_skill_mapped("cuff") != "shenghuo-lingfa"
	 || me->query_skill_prepared("cuff") != "shenghuo-lingfa"
	 && userp(me))
		return notify_fail("你必须先将圣火令法功和乾坤大挪移相互配合。\n");

	if(userp(me) && me->query("shen") < - 20000 && !me->query_skill("jiuyang-shengong", 1))
		return notify_fail("你戾气太高了，提高圣火令法太过危险。\n");

	if(userp(me)){
		me->add("jingli", -80);
		me->add("neili", -300);
		me->add("shen", - me->query_skill("shenghuo-lingfa",1) - random(1000));
	}

	message_vision(HIR"\n$N此刻乍逢劲敌，将圣火令法中的精微处尽数发挥出来，心灵渐受感应，突然间哈哈哈仰天三笑，\n"+
			"声音中竟充满了邪恶奸诈之意。\n\n" NOR, me);
	
	lvl = (lvl+me->query_skill("jiuyang-shengong", 1))/2;
	me->add_temp("apply/pfm_attack", 120);
	me->add_temp("apply/pfm_damage", 150);
	me->add_temp("apply/damage", 800);
	me->set_temp("ling", 1);

	call_out("remove_effect", 1, me, weapon, 15);
	return 1;
}

void remove_effect(object me,object weapon,int count)
{
	if ( !me ) return;
	if (objectp(me) && me->query_temp("ling")) {
		if (count-- > 0 
		&& me->is_fighting()
		&& me->query_skill_mapped("cuff") == "shenghuo-lingfa"
		&& me->query_skill_prepared("cuff") == "shenghuo-lingfa"
		&& me->query_temp("weapon")
		&& weapon == me->query_temp("weapon")) {
			me->set_temp("apply/action/pfm",msg[random(7)]);
			call_out("remove_effect", 1, me, weapon, count);
			return;
		}
	  me->add_temp("apply/pfm_attack", -120);
	  me->add_temp("apply/pfm_damage", -150);
	  me->add_temp("apply/damage", -800);
		me->delete_temp("apply/action/");
		me->delete_temp("ling");
		tell_object(me, HIW"\n你心魔慢慢消退，圣火令法立时不能连贯，威力大减。\n"NOR);
		tell_room(environment(me), HIW+ me->name() + "心魔慢慢消退，圣火令法立时不能连贯，威力大减。\n"NOR, ({ me }));
	}
}

string perform_name(){ return HIR"心魔"NOR; }

int help(object me)
{
	write(HIR"\n圣火令法之「心魔」："NOR"\n\n");
	write(@HELP
	要求：	当前内力 1000 以上；
		当前精力 800 以上；
		圣火令法等级 120 以上；
		乾坤大挪移等级 120 以上：
		圣火神功等级 120 以上；
		若无九阳神功，负神不得大于 20000 以上；
		激发令法为圣火令法；
		激发拳法为圣火令法；
		激发招架为乾坤大挪移；
		备拳法为圣火令法；
		且手持兵器。

HELP
        );
        return 1;
}
