// huoyan.c 火焰
// By Spiderii xx大福利啊！
#include <ansi.h>

inherit F_CLEAN_UP;

int exert(object me)
{      
       object fire, ob;
       int skill;
       skill = me->query_skill("huagong-dafa");

	if( (int)me->query_skill("huagong-dafa", 1) < 100 )
		return notify_fail("你的化功大法修为不够，无法聚气成焰！\n");

	if (me->query_skill_mapped("force") != "huagong-dafa")
		return notify_fail("其它内功有这个效果吗？\n");

       if(!objectp(fire = present("fire", me)))
               	return notify_fail("你功力再深，也不能凭空点火吧？\n");

       if( (int)me->query_skill("poison", 1) < 100 )
		return notify_fail("你体内的毒素不够，无法使用「化毒」！\n");

	if( (int)me->query("neili") < 500 )
		return notify_fail("你目前的内力太少了，无法使用「化毒」！\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力修为这么差，还想使用「化毒」？\n");
       
       if(present("huo yan", environment(me)))
             	return notify_fail("这里已经有火焰了，还要升什么火？\n");

	message_vision(HIR"$N口中吐出一口青烟，将内力运于手中火折之上，一搓一抛，轰地一声燃起一堆熊熊火焰。\n" NOR, me);
	
       ob = unew("/d/xingxiu/obj/huoyan")->move(environment(me)); 
	return 1;
}
string exert_name(){ return HIR"聚气成焰"NOR; }
int help(object me)
{
        write(BLU"\n化功大法「聚气成焰」："NOR"\n");
        write(@HELP
        星宿派的功夫主要是在<毒>字上下功夫，深的阴损毒辣其中三味
        化功大法吸人内力，阴毒异常，江湖上人提起，无不谈虎色变。
        这聚气成焰一式乃是凭借自身深厚内力，运于手中火折之上，凭
        空既可聚气成焰，攻敌不备！

        要求:   化功大法 100 级；
                毒技     100 级；
                当前内力 500 点；
                最大内力 1000 点；
                手中有火折。
        
HELP
        );
        return 1;
}

