// bonze_jail.c

#include <ansi.h>
#include <login.h>

int update_condition(object me, int duration)
{
      object where;
	if (duration < 1) {
		me->move("/d/shaolin/guangchang");
		message("vision",
			HIY "只听乒地一声，你吓了一跳，定睛一看，\n"
			"原来是一个昏昏沉沉的家伙从大门里被扔了出来！\n" NOR, environment(me), me);
		tell_object(me, HIY "只觉一阵腾云驾雾般，你昏昏沉沉地被扔出了少林寺！\n" NOR);
		me->set("startroom", START_ROOM);
		return 0;
	}
	if (!duration) return 0;
        where = environment(me);
        if (duration >= 1 && base_name(where)!="/d/shaolin/jianyu") {		
		message_vision(HIR "\n突然冲出来一大群僧兵，指着$N喊道：“看你这逃犯还能逃到哪儿去！”
接着便是乱棒齐下，顿时将$N打得昏死过去......\n" NOR, me);
                me->unconcious();
                message_vision("僧兵们扛着晕死过去的$N，打道回少林寺去了。\n", me);
                me->set("eff_qi", 1);
                me->set("qi", 1);
                me->apply_condition("bonze_jail", 120);
                me->move("/d/shaolin/jianyu");
                message("vision",
			HIY "只听乒地一声，你吓了一跳，定睛一看，\n"
			"原来是一个昏昏沉沉的家伙被扔了进来！\n" NOR, environment(me), me);
	}
	me->apply_condition("bonze_jail", duration - 1);
	return 1;
}

string query_type(object me)
{
	return "jail";
}
