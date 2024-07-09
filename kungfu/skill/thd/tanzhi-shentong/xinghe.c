// xinghe Modify By River@SJ 2001.12.13
inherit F_SSERVER;
#include <ansi.h>
#include <combat.h>

mapping valid_types = 
([
	"finger":	"指法",
	"hand":		"手法",
	"cuff":		"拳法",
	"claw":		"爪法",
	"strike":	"掌法",
	"leg":		"腿法",
]);

int query_ap() {return 3;}

int perform(object me, object target)
{
	string *skill, basic1, msg, sk;
	int i,lvl;
	mapping smap, pmap;

	if( !target ) target = offensive_target(me);

        if( !objectp(target)
	 ||  !target->is_character()
	 ||  !me->is_fighting(target) )
		return notify_fail("「星河千帆舞」只能对战斗中的对手使用。\n");

	if ( me->query_skill("bihai-chaosheng", 1) < 250 )
		return notify_fail("你的碧海潮生功不够，无法使出星河千帆舞。\n");

	if ( me->query_skill("suibo-zhuliu", 1) < 250 )
		return notify_fail("你的随波逐流修为未到，无法使出星河千帆舞。\n");

	if ( (lvl = me->query_skill("tanzhi-shentong", 1)) < 250 )
		return notify_fail("你的弹指神通修为未到，无法使出星河千帆舞。\n");

	if ( me->query_skill_mapped("finger") != "tanzhi-shentong"
	 || me->query_skill_prepared("finger") != "tanzhi-shentong")
	 	return notify_fail("你现在无法使用星河千帆舞。\n");

	if ( me->query_temp("weapon"))
		return notify_fail("你现在无法使用星河千帆舞。\n");

	if ( me->query_skill("finger",1) < 250 )
		return notify_fail("你的指法修为不足，使不出星河千帆舞。\n");

	if( me->query("max_neili") < 3500 )
		return notify_fail("你内力太弱，不能出星河千帆舞。\n");

	if( me->query("neili") < 1200 )
		return notify_fail("你真气太弱，不能出星河千帆舞。\n");

	if( me->query("jingli") < 1000 )
		return notify_fail("你精力太弱，不能出星河千帆舞。\n");

	if ( me->query_temp("thd/xinghe"))
		return notify_fail("你正在使用星河千帆舞。\n");

	sk = me->query("env/xinghe");

	if (!sk || sk == "tanzhi-shentong") 
		return notify_fail("你必须选择一个技巧来配合！\n");

	if (me->query_skill(sk,1) < 250)
		return notify_fail("你的组合技巧还没纯熟。\n");

	smap = me->query_skill_map();
	pmap = me->query_skill_prepare();

	skill = keys(valid_types);
	for (i=0; i<sizeof(skill); i++) {
		if( smap[skill[i]] == sk ) basic1 = skill[i];
	}
	if ( !basic1 )
		return notify_fail("这种技能尚未激发或目前不能准备。\n");

	msg = HIW"$N长啸一声，身形加快，施展出桃花绝技「星河千帆舞」，转而使用"+to_chinese(sk)+"的招数，且不时发出弹指神通指风！\n"NOR;
	me->add("neili",-400-lvl/2);
  
  me->add_temp("apply/pfm_damage",100);
  me->add_temp("apply/pfm_attack",60);
  me->set_temp("apply/action/skill",sk);
  me->add_temp("apply/action/color",HIC);
	me->add("jingli", - 200);
	me->set_temp("thd/xinghe",1);
	message_vision(msg, me, target);
	call_out("check", 1, me, target, 6,lvl,sk);
	return 1;
}

string *xue_name = ({ 
"劳宫穴","膻中穴","曲池穴","关元穴","曲骨穴","中极穴","承浆穴","天突穴","百会穴",
"幽门穴","章门穴","大横穴","紫宫穴","冷渊穴","天井穴","极泉穴","清灵穴","至阳穴",
"涌泉穴","大锥穴","风府穴",
}); 

void check(object me, object target, int count,int lvl,string sk)
{

	string  name;
	mapping pmap;
  
	if ( !me ) return;

	pmap = me->query_skill_prepare();

	if (count > 0 && target && me->is_fighting(target)) {
		name = xue_name[random(sizeof(xue_name))];
		me->set_temp("apply/action/pfm",HIW"$N将弹指神通挟在"+to_chinese(sk)+"的招数之中，弹出一道指风，直刺$n的「"+name+"」");	  

	  me->add_temp("apply/damage",250+lvl/2);
	  me->add_temp("apply/pfm_damage",150+lvl/4);
    me->add_temp("apply/pfm_attack",180+lvl/10);
    me->set_temp("apply/action/damage_type","刺伤");
	  COMBAT_D->do_attack(me, target,me->query("weapon"),1);
	  me->add_temp("apply/damage",-250-lvl/2);
    me->add_temp("apply/pfm_damage",-150-lvl/4);
    me->add_temp("apply/pfm_attack",-180-lvl/10);

    me->delete_temp("apply/action/pfm");
		
		call_out("check", 4, me, target, count--,lvl,sk);
	}
	
	else {
		me->delete_temp("thd/xinghe");
		me->delete_temp("xinghe");
    me->add_temp("apply/pfm_damage",-100);
    me->add_temp("apply/pfm_attack",-60);
    me->delete_temp("apply/action");
		if (living(me) && ! me->is_ghost())
			message_vision(HIW"$N招数一变，又转回了弹指神通。\n"NOR, me);
	}
}

string perform_name(){ return HIW"星河千帆舞"NOR; }

int help(object me)
{
        write(HIW"\n弹指神通之「星河千帆舞」："NOR"\n\n");
        write(@HELP
	要求：	最大内力 3500 以上；
		当前内力 1200 以上；
		当前精力 1000 以上；
		碧海潮生功等级 250 以上；
		随波逐流等级 250 以上；
		弹指神通等级 250 以上；
		基本指法等级 250 以上；
		激发指法为弹指神通；
		激发招架为弹指神通；
		空手且备指法为弹指神通。
	
	使用说明：
		set xinghe <空手特殊武功>
		例：set xinghe luoying-zhang

HELP
        );
        return 1;
}
