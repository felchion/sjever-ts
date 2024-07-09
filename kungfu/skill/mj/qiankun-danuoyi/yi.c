// yi.c 乾坤大挪移「移」字诀
#include <ansi.h>
inherit F_SSERVER;
string perform_name(){ return HIB"移字诀"NOR; }

int perform(object me, string target)
{
	object ob;

	if(!target)
		return notify_fail("你想把对方的劲力挪移至谁？\n");

	if(!objectp(ob = present(target, environment(me))))
		return notify_fail("这里没有你想挪移的目标这个人。\n");

	if(!me->is_fighting())
		return notify_fail("你没在打架，移什么移！\n");

        if(!ob->is_character()) 
		return notify_fail("看清楚一点，那并不是生物。\n");

	if(!living(ob))
		return notify_fail(ob->name()+"已经无法战斗了。\n"); 

	if(ob==me) return notify_fail("你不能攻击自己。\n");               
	
	if(userp(ob) && !me->is_killing(ob))
		return notify_fail("除非跟对方有深仇大恨，你不能陷害玩家。\n");

	if(me->is_fighting(ob))
		return notify_fail("你正和他打着呢。\n");

	if(me->query_skill("qiankun-danuoyi", 1) < 120 )
		return notify_fail("你的乾坤大挪移还不够娴熟，不会使用「移」字诀。\n");

	if (!me->query_skill("jiuyang-shengong", 1))
	if( me->query_skill("shenghuo-shengong", 1) < 120 )
		return notify_fail("你的圣火神功等级不够。\n");

	if(me->query_skill_mapped("parry") != "qiankun-danuoyi")
		return notify_fail("你没有使用乾坤大挪移作为招降，无法使用「移」字诀！\n"); 
	
	if((int)me->query("neili") < 800 )
		return notify_fail("你的内力不够！\n");
	
	if((int)me->query_skill("force") < 200 )
		return notify_fail("「移」字诀需要深厚的内功基础，你的条件好象还不够。\n");

	me->add("neili", -50);
	tell_object(me, BLU"你暗运乾坤大挪移「移」字诀，企图要把对方下一招的力道转移到"+ob->name()+"身上！\n"NOR); 
	me->start_perform(2, "移字诀");
	me->set_temp("qkdny/yi", ob);
	return 1;
}

int help(object me)
{
        write(HIB"\n乾坤大挪移「移字诀」："NOR"\n");
        write(@HELP
        要求：  当前内力 800 以上；
                乾坤大挪移等级 120 以上；
                有效内功 200 以上；
                激发招架为乾坤大挪移。
HELP
        );
        return 1;
}
