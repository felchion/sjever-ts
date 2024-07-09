// xinjing.c 
// By River
// Modified by darken@SJ

#include <ansi.h>
inherit F_CLEAN_UP;
string *pictures=({
  HIY"少思、"YEL"少念、"HIC"少欲、"HIR"少事、"HIB"少语、"MAG"少笑"HIW,
  YEL"少愁、"CYN"少乐、"HIG"少喜、"HIR"少怒、"HIY"少好、"RED"少恶"HIW,
  HIC"多思"HIG"则"HIW"神怠，"HIB"多念"MAG"则"HIY"精散"HIW,
  HIR"多欲"YEL"则"BLU"智损，"MAG"多事"RED"则"HIG"形疲"HIW,
  HIB"多语"CYN"则"RED"气促，"HIR"多笑"HIC"则"BLU"肝伤"HIW,
  MAG"多愁"YEL"则"HIR"心慑，"RED"多乐"BLU"则"HIY"意溢"HIW,
  HIW"多喜"HIG"则"HIY"忘错"MAG"昏乱，"HIC"多怒"HIR"则"HIB"百脉"HIG"不定"HIW,
  HIG"多好"RED"则"BLU"专迷"HIG"不治，"HIW"多恶"HIB"则"MAG"焦煎"CYN"无宁"HIW
});

string exert_name(){ return HIW"玉女心经"NOR; }
int query_ap() {return 2;}

int exert(object me)
{
	int skill;
	string picture;

	if((int)me->query("neili") < 500 )
	return notify_fail("你的内力不够。\n");

	if((int)me->query_temp("gm/xinjing"))
	return notify_fail("你已经在运功中了。\n");

	if((int)me->query_skill("yunu-xinjing",1 ) < 100)
	return notify_fail("你的玉女心经等级不够，还不能体会十二少和十二多之妙处。\n");

	if( me->query_skill_mapped("force") != "yunu-xinjing")
	return notify_fail("你现在不能使用「心经」字诀。\n");

	if( me->query("gender") == "无性" )
	return notify_fail("你无根无性，如何能领悟玉女心经的「心经」字诀。\n");

	skill = (int)me->query_skill("yunu-xinjing", 1);
	if (skill > 250) skill = 250+ (skill-250)/5;
	me->add("neili", -(200+ random(200)));
	picture = pictures[random(sizeof(pictures))];
	message_vision(HIY"$N屏气凝神，口中默念"HIW"「"+picture+"」"HIY"的玉女心经正反要诀。\n" NOR, me);

	me->add_temp("apply/damage", skill);
	me->add_temp("apply/ep_armor", skill/2);

	if( me->query("gender") == "女性" )
	{
		me->add_temp("apply/pfm_dodge", skill/3);
		me->add_temp("apply/pfm_attack", skill/3);
		
	}
	else if( me->query("gender") == "男性" )
	{
		me->add_temp("apply/pfm_damage", skill/3);
		me->add_temp("apply/pfm_parry", skill/3);
	}

	me->set_temp("gm/xinjing", skill);
	me->start_call_out((:call_other,__FILE__,"remove_effect",me, me->query_skill("yunu-xinjing", 1)/3:),1);

  me->start_busy(random(2));
	return 1;
}

void remove_effect(object me, int count)
{
	int skill;
	if(!objectp(me)) return;
	if(objectp(me)
	&& me->query_temp("gm/xinjing")
	&& me->query_skill_mapped("force") == "yunu-xinjing"
	&& count){
		call_out("remove_effect", 1, me, --count);
		return;
	}
	skill = me->query_temp("gm/xinjing");
	me->add_temp("apply/damage", -skill);
  me->add_temp("apply/ep_armor", -skill/2);
	if( me->query("gender") == "女性" )
	{
		me->add_temp("apply/pfm_attack", -skill/3);
		me->add_temp("apply/pfm_dodge", -skill/3);
	}
	else if( me->query("gender") == "男性" )
	{
		me->add_temp("apply/pfm_damage", -skill/3);
		me->add_temp("apply/pfm_parry", -skill/3);
	}
	me->delete_temp("gm/xinjing");
	tell_object(me, HIW"你的玉女心经「心经」字诀运功完毕，将内力收回丹田。\n"NOR);
}
