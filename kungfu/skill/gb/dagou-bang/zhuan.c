// zhuan.c 打狗棒法--转字诀
// Creat by looklove@SJ 2001.4.10
// Modify by looklove 2001.5.2
// Modify by looklove 2001.5.4 青年节咯……
// Looklove modified at 2001.5.15

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name(){ return HBYEL"转字诀"NOR; }

string *zhuan_msg = ({
HIM"$n无法可施，只得向前急纵，却是避开一棒，后一棒又连环打来。\n"NOR,
HIY"眼看$n无法可施，只得向前急纵，却是避开前棒，后棒又至。\n"NOR,
HIC"大惊中$n脚下加劲，欲待得机转身，但$n纵跃愈快，$W"HIC"端来得愈急。\n"NOR,
HIG"$n绕着$N飞奔跳跃，大转圈子。$N手中$W"HIG"却一直在$n后心晃动。\n"NOR,
HIB"$N神色自若地看着$n飞奔跳跃，大转圈子。$N站在中心，举棒不离$n后心。\n"NOR,
HIR"$n绕着$N上蹿下跳，$N神色自若，$W"HIR"始终不离$n后心。\n"NOR,
HIW"$N将$W"HIW"自左手交到右手，又自右手交到左手，始终不离$n后心大穴。\n"NOR,
RED"$N手中$W"RED"划个圆圈，直点$n后心大穴，$n只得继续向前蹿跃。\n"NOR,
YEL"$n心中暗暗叫苦，却无计可施，脚下一步也不敢停顿。\n"NOR,
WHT"只见$n跑的圈子越转越大，$N手中的$W"WHT"却一直在$n后心打转。\n"NOR,
BLU"$n圈子越转越大，汗流夹背，$N手中的$W"BLU"依旧不断往$n后心招呼。\n"NOR,
MAG"$n一脸骇然，满头大汗，足下可丝毫不敢停步，一直绕着圈子躲避。\n"NOR,
CYN"一时间$n脸色大变，满头大汗，$W"CYN"不断点来，使得$n应接不暇。\n"NOR,
});

int check_fight(object me);
int halt_fight(object me);

int perform(object me, object target)
{
	string msg;
	object tweapon, weapon;
	int lost, mexp, texp, mlvl, tlvl, x, y, b, n;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target))
		return notify_fail("「转字诀」只能对战斗中的对手使用。\n");

	weapon = me->query_temp("weapon");
	if( !weapon
	 || weapon->query("skill_type") != "stick"
	 || me->query_skill_mapped("stick") != "dagou-bang" 
	 || me->query_skill_mapped("parry") != "dagou-bang" )
		return notify_fail("你现在无法使用「转字诀」。\n");

	if( me->query_temp("dgb/zhuan") )
		return notify_fail("你正在使用「转字诀」。\n");

	if( me->query_temp("dgb/chan") )
		return notify_fail("你正在使用「缠字诀」。\n");

	if( target->is_busy() || target->query_temp("dgb/chan") )
		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧。\n");

	mexp=me->query("combat_exp");
	texp=target->query("combat_exp");

	mlvl = (int)me->query_skill("dagou-bang",1);
	tlvl = (int)target->query_skill("dodge", 1);

	x = me->query_int(1);
	y = target->query_int(1);
	b = me->query_skill("bangjue",1);

	tweapon = target->query_temp("weapon");

	if( mlvl < 180 )
		return notify_fail("你的打狗棒法还不够娴熟，不会使用「转字诀」。\n");
	if( b < 180 )
		return notify_fail("你的打狗棒诀不够娴熟，无法使出「转字诀」。\n");
	if( me->query_skill("huntian-qigong",1) < 180 )
		return notify_fail("你的混天气功根基不足，无法使出「转字诀」。\n");
	if( me->query("neili") < 800 )
		return notify_fail("你的内力不够了。\n");
	if( me->query("jingli") <  800 )
		return notify_fail("你的精力不足以支持你使出「转字诀」。\n");
	if( me->query("max_neili") < 2000 )
		return notify_fail("你的内力不够支持你使用「转字诀」。\n");

	lost = b /50 + random(b/50)+x/5+mlvl/50;
	if( mlvl > tlvl ) lost += (mlvl-tlvl)/5;
	if( mlvl > 350 ) lost += (mlvl-300)/10;
	n = lost*5;
	if(lost > 15) lost = 15 + random(lost-15)/5;

	if( wizardp(me) )
        	write(sprintf(HIR"lost=%d n = %d\n"NOR,lost, n));

	msg = HBYEL"$N使出打狗棒法「转字诀」，只见"+weapon->name()+HBYEL"化成了一团碧影，\n"NOR;
	msg += HBYEL"猛点$n后心「强间」、「风府」、「大椎」、「灵台」、「悬枢」各大要穴。\n"NOR;

	if(COMBAT_D->do_attack(me, target, 0, 0, 1)) {
		msg += HIM "$N的点打连绵不断，一点不中，又点一穴，棒影只在$n背后各穴上晃来晃去。\n" NOR;
		me->set_temp("dgb/zhuan", lost);
		me->set_temp("dgb/zhuan_n", n);
		me->set_temp("dgb/zhuan_target",target);
		target->start_busy(2);
		me->add("neili", -600);
		if(wizardp(me))	write("starting busy2 now\n");
		me->start_busy((:check_fight:),(:halt_fight:));
	}
	else {
		msg += HIW "$n吃了一惊，慌忙在"+weapon->name()+HIW"上一挡，阻住了$N的攻势。\n" NOR;
		me->start_busy(random(2));
		me->add("neili", -200);
	}
	me->start_perform(2, "转字诀");
	message_vision(msg, me, target);
	return 1;
}


int remove_effect(object me)
{
        if(!me) return 0;

        me->delete_temp("dgb/zhuan");
        me->delete_temp("dgb/zhuan_n");
        me->delete_temp("dgb/zhuan_target");
        if(!living(me)) return 0;
        me->start_perform(2, "转字诀");
        me->start_busy(random(2));
        return 0;
}

int check_fight(object me)
{
	object wp,target;
	string msg;
	int n, lost;
	n = me->query_temp("dgb/zhuan_n");
	lost = me->add_temp("dgb/zhuan",-1);
	if(wizardp(me))
		tell_object(me,sprintf("lost = %d  n = %d \n",lost,n ));
	if(!objectp(target = me->query_temp("dgb/zhuan_target")))
		lost = 0;
	message("wizard", "转字诀战斗信息：精力消耗 "+lost+", 回合数"+n+"。\n", environment(me), me);
	if(!objectp(wp = me->query_temp("weapon"))){
		msg = HIG"$N手中武器既失，转字诀也随之停了下来。\n"NOR;
		me->delete_temp("dgb/zhuan");
		target->start_busy(1);
		message_vision(msg, me, target);
		message_vision(HBYEL"\n$N的「转字诀」运行完毕，气回丹田，缓缓收功。\n"NOR, me);
		return remove_effect(me);
	}
	if(lost){
		if(!living(target))
			msg = HIW"$N不屑地看了$n一眼，神色自若地将"+wp->name()+HIW"收了回来。\n"NOR;
		else if(!me->is_fighting(target) || !living(me))
			msg = HBYEL"$N将手中"+wp->name()+HBYEL"一收，脚步停了下来。\n"NOR;
		else if( me->query_skill_mapped("stick") != "dagou-bang")
			msg = HBYEL"$N招数一变，使出另一路棒法来。\n"NOR;
		else if( me->query("jingli") <= 300 )
			msg = HBYEL"$N棒招渐缓，原来是精力不足了。\n"NOR;
		else if( random((int)me->query_int()) < target->query_int()/3 && lost<5)
			msg = HIW"$n奋力跳起，回身在"+wp->name()+HIW"上一拨，$N始料不及，急忙收招。\n"NOR;
		else if( target->is_ghost())
			msg = HBYEL"$N将"+wp->name()+HBYEL"向后一摆，收回了招式。\n"NOR;
		if(stringp(msg)){
			message_vision(msg, me,target);
			return remove_effect(me);
		}
		msg = zhuan_msg[random(sizeof(zhuan_msg))];
		msg = replace_string(msg, "$W", wp->name());
		msg += "\n"NOR;
		if((int)target->query("jingli")< 0) target->set("jingli",-1);
		me->start_perform(1, "转字诀");
		me->receive_damage("jingli",n/3,"精力耗尽");
		target->receive_damage("jingli", n, me);
		target->add_busy(1);
		message_vision(msg, me, target);
		return 1;
	}
	msg = HBBLU"$N将"+wp->name()+HBBLU"轻轻向斜上方一挑，棒影不见了。\n"NOR;
	message_vision(msg, me);
	return remove_effect(me);
}

int halt_fight(object me)
{
	if(!objectp(me))
		return 1;
        message_vision(HBYEL"\n$N的「转字诀」运行完毕，气回丹田，缓缓收功。\n"NOR, me);
	if(!remove_effect(me))
		return 1;
}
int help(object me)
{
	write(HBYEL"\n打狗棒「转字诀」："NOR"\n");
	write(@HELP
        「转字诀」将打狗棒“轻”“灵”“奇”“巧”的特点发挥得淋
        漓尽致，以专攻对手的后心几大要穴为主，一旦施展开来，将会
        迫使对手不停地象螺砣般旋转，以趋避点打。结果是愈逃棒越急，
        对手纵使暗暗叫苦却也无计可施，最后因耗尽体力而败下阵去。

	要求：	最大内力 2000 以上；
		当前内力 800 以上；
		当前精力 800 以上；
		打狗棒等级 180 以上；
		混天气功等级 180 以上；
		打狗棒诀等级 180 以上：
		激发招架为打狗棒。
HELP
	);
	return 1;
}
