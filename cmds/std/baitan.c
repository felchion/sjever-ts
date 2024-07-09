#include <ansi.h>
inherit F_CLEAN_UP;

int baitan(object me);
int halt_baitan(object me);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
        object where;

        where = environment(me);

        if (! me->query("is_vendor"))
                return notify_fail("只有从事商业的人才能摆摊。\n");

        if (! present("shang ling", me))
                return notify_fail("你的商令不在身上，如今世道乱，小心为妙。\n");

        if (me->is_fighting())
                return notify_fail("你还是好好打你的架吧。\n");

        if (me->query_temp("on_baitan"))
                return notify_fail("你现在已经占了一个摊位，还是适可而止吧。\n");

        if (me->is_busy())
                return notify_fail("你现在正忙着呢，没功夫摆摊设点。\n");

        if (me->query_condition("killer"))
                return notify_fail("现在官府正到处查你呢，难得还有心情摆摊？\n");

        if (where->query("no_trade"))
                return notify_fail("这里常有官兵来巡逻，最好换个地方做生意。\n");

        if (where->query("no_fight") && ! where->query("can_trade"))
                return notify_fail("这个地方过于嘈杂，还是换个地方做生意吧。\n");

        message_vision(WHT "$N" WHT "寻了块空地，一屁股坐了下来，随后掏"
                       "出一块布摊开在地。\n" NOR, me);
        tell_object(me, HIC "现在你可以摆上(" HIW "stock" HIC ")货物或是"
                        "收起(" HIW "unstock" HIC ")某种货物。\n" NOR);

        me->set_temp("on_baitan", 1);
        me->delete("vendor_goods");
        me->set("disable_type"," <摆摊中> ");;
	me->start_busy(1);

        return 1;
}


int help(object me)
{
        write(@HELP
指令格式 : baitan

摆摊设点，兜售货物。

HELP);
        return 1;
}
