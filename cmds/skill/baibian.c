// 神行百变
// by augx@sj  10/3/2001

#include <ansi.h>
#include <command.h>
inherit F_SSERVER;

//int perform(object me, object target)
int main(object me, string arg)
{
	string msg;
	object target;
	string *exits, *kexits;
	int lvl,lvl2,k,a,b; 
	object room,oroom,lastroom,ob;
	
	if(!me->query_skill("youlong-shenfa",1))
		return 0;
	if (me->is_busy())
		return notify_fail("你现在正忙着呢。\n");
	if( !me->is_fighting() )
		return notify_fail("「神行百变」只能在战斗时用！\n");
	//if( !arg ) 
	target = offensive_target(me);
	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target))
		return notify_fail("「神行百变」只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("youlong-shenfa", 1) < 100 )
		return notify_fail("你的游龙身法还不够熟练，不能使用！\n");

	if(me->query_skill("dulong-dafa",1) < 100 )
		return notify_fail("你的毒龙大法功力不足，不能用「神行百变」。\n");

	if(me->query_skill_mapped("force") != "dulong-dafa" )
		return notify_fail("你没有激发毒龙大法，不能使用「神行百变」！\n");

	if((int)me->query("neili") < 300)
		return notify_fail("你现在内力不足，不能用「神行百变」！\n"); 

	if((int)me->query("jingli") < 100)
		return notify_fail("你现在精力不足，不能用「神行百变」！\n"); 

	if(userp(me) && (int)me->query_temp("sld/baibian") )
		return notify_fail("你已经使用「神行百变」了！\n"); 

	if(!me->query("quest/baibian"))
		return notify_fail("你还没有领会「神行百变」。\n");

	msg = HIY"\n$N身法突然加快，身影缥缈不定，更本无法追踪！\n"NOR;

	message_vision(msg, me);
	me->add("neili",-300);
	me->add("jingli",-100);
	me->set_temp("sld/baibian",me->query("dex"));
	me->add_temp("apply/dexerity",me->query("dex"));
	
	lvl = me->query_skill("dodge") + 1;
	lvl2 = target->query_skill("dodge") + 1;
	if ( target->is_busy() || random(lvl*3/2+lvl2)>lvl2 ) {
		message_vision(HIY"$N飞快的逃了开去，$n更本无法识破$N逃跑的路径，只能呆呆的看着$N逃走！\n"NOR, me, target);
		me->start_perform(3,"「神行百变」");
		foreach (ob in all_inventory(environment(me))) {
			if(!ob || ob==me || !me->is_fighting(ob) || ob->query_leader()!=me) continue;
			ob->set_leader(0);
		}
		me->remove_all_killer();
		
		room = environment(me);
		if( mapp(room->query("exits")) && sizeof(room->query("exits"))>0 ) {
			oroom = room;
			lastroom = room;
			for(k=0;k<9;k++) {
				exits = values(room->query("exits"));
				kexits = keys(room->query("exits"));
				b = sizeof(exits);
				while( (a=random(b))>=0 && b>1 ) {
					if (b==2)
						if ((exits[0]==base_name(lastroom) && exits[1]==base_name(oroom))
						 || (exits[1]==base_name(lastroom) && exits[0]==base_name(oroom)))
							break;
					if (exits[a]==base_name(lastroom) || exits[a]==base_name(oroom)) continue;
					break;
				}
				if( exits[a]!=base_name(oroom) || k<8 )
					GO_CMD->main(me,kexits[a]);
				lastroom = room;
				room = environment(me);
			}
		}
		
		if(room == oroom)
			message_vision(CYN"但是$N却发现无路可逃！\n"NOR, me);			 
        }
        else {
		message_vision(CYN"$N试图逃走，但是$n却识破了$N逃跑的路线，把$N拦了下来！\n"NOR, me, target);
		me->start_perform(2,"「神行百变」");
	}

	call_out("remove_effect", 6, me);	
	return 1;       
}

void remove_effect(object me)
{
	if( !me || !me->query_temp("sld/baibian") ) return;
	me->add_temp("apply/dexerity", -me->query_temp("sld/baibian"));
	me->delete_temp("sld/baibian");
	message_vision(CYN"\n$N急速奔跑了一阵，终于不能维持那样的速度，身形慢了下来！\n"NOR, me);
}

//string perform_name(){ return HIY"神行百变"NOR; }
int help(object me)
{
	write("\n游龙身法「神行百变」：\n\n");
	write(@HELP
        在战斗中快速逃离战场，且使敌人无法追踪，乃保命之不二绝招。
        要求：100级毒龙大法，100级游龙身法
              300内力，100精力

HELP
	);
	return 1;
}
