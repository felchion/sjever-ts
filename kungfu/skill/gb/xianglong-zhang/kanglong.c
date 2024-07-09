// 亢龙有悔
// by snowman@SJ 16/10/99
// modify by looklove 2001.10.04
// Modify by lsxk@hsbbs 2007/7/2

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HIY"亢龙有悔"NOR; }
int kanglong_attack(object me, int flag);

int perform(object me, object target)
{
   int tmp,lvl,flag;

   if( !target ) target = offensive_target(me);

   if( !objectp(target)
       || !me->is_fighting(target) 
       || !living(target)
       || environment(target)!=environment(me))
    return notify_fail("「亢龙有悔」只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「亢龙有悔」！\n");

	if( (int)me->query_skill("xianglong-zhang", 1) < 200 )
		return notify_fail("你的降龙十八掌还不够娴熟，使不出「亢龙有悔」绝技。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 180 )
		return notify_fail("你的混天气功等级还不够，使不出「亢龙有悔」绝技。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你所用的内功不对，使不出「亢龙有悔」绝技。\n");

	if( (int)me->query_skill("force") < 270 )
		return notify_fail("你的内功等级不够，不能使用「亢龙有悔」。\n");

	if( (int)me->query_str(1) < 40 )
		return notify_fail("你的膂力还不够强劲，使不出「亢龙有悔」绝技。\n");

	if( me->query_skill_prepared("strike") != "xianglong-zhang"
	 || me->query_skill_mapped("strike") != "xianglong-zhang"
	 || me->query_skill_mapped("parry") != "xianglong-zhang")
		return notify_fail("你现在无法使用「亢龙有悔」！\n");

	if( (int)me->query("max_neili") < 4000 )
		return notify_fail("你现在内力太弱，使不出「亢龙有悔」。\n");

	if( (int)me->query("neili") < 1500 )
		return notify_fail("你现在真气太弱，使不出「亢龙有悔」。\n");

	if( (int)me->query("jingli") < 1000 )
		return notify_fail("你的精力不够，无法使用「亢龙有悔」绝技。\n");

	if( !me->query("xlz/hang") )
		return notify_fail("你还没领悟到「亢龙有悔」的精髓，无法使用此招数攻击。\n");

  lvl = (int)me->query_skill("xianglong-zhang",1);
  
  tmp = 1 + (lvl-200)/10;
  if(tmp>18) tmp = 18;

  if(random(tmp)>8) flag = 1;
  
  me->set_temp("apply/action/pfm", HIY"$N叫道：「看招！」左腿微屈，右掌划了个圆圈，平推出去，正是降龙十八掌中的「亢龙有悔」。\n"+
   "初推出去看似轻描淡写，但一遇阻力，刹时之间连加"+ chinese_number(tmp)+ "道後劲，一道强似一道，重重叠叠。\n"+
   "直至无坚不摧，无强不破"NOR);
  if(flag)
  	me->set_temp("apply/action/hit", HIW"\n这「亢龙有悔」之精要不在「亢」字而在「悔」字！「亢龙有悔，盈而有余」十分力道之打出之后，更是余有二十分力道。\n"+
               "$n只道$N奋力一击，便催动全身内劲与之抗衡，岂料劲道将老之际，$N的二十分后劲以排山倒海之势狂卷而来。\n"+
               "$n顿时便被的这后股劲冲得眼前一黑，五脏六腑犹如炸开一般难受，再也支持不住了！\n\n"NOR);       
	else
	  me->set_temp("apply/action/hit", HIR"$n直被这一掌逼得退无可退，让无可让，重重地被击中胸口，口中鲜血狂喷！\n"NOR);   
	
	target->set_temp("apply/action/parry", HIC"$n哪里见过如此刚猛的掌法？连忙就地一滚，远远的躲了开去，只吓地连冒冷汗！\n"NOR);
	target->set_temp("apply/action/dodge", HIC"$n哪里见过如此刚猛的掌法？连忙就地一滚，远远的躲了开去，只吓地连冒冷汗！\n"NOR);	
   
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",500+lvl+(flag?200:0));
	me->add_temp("apply/pfm_damage",150+lvl/4+(flag?100:0));
	me->add_temp("apply/pfm_attack",180+lvl/10+(flag?120:0));
	COMBAT_D->do_attack(me, target,0,(flag||!random(3))?3:0);
	me->add_temp("apply/damage",-500-lvl-(flag?200:0));
	me->add_temp("apply/pfm_damage",-150-lvl/4-(flag?100:0));
	me->add_temp("apply/pfm_attack",-180-lvl/10-(flag?120:0));	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

	me->start_busy(random(2));
  return 1;
}



int help(object me)
{
	write(YEL"\n降龙十八掌之「亢龙有悔」："NOR"\n");
	write(@HELP
	最能体现降龙十八掌无匹威力，所向披靡。
	当年萧峰用三掌逼走丁春秋，正是此绝招。

	要求：	当前内力 1500 以上；
		最大内力 4000 以上：
		当前精力 1000以上；
		降龙掌等级 200 以上；
		混天气功等级 180 以上；
		内功有效等级 270 以上：
		膂力 40 以上：
		激发招架为降龙掌：
		经洪七公指点后方可使用。
HELP
	);
	return 1;
}
