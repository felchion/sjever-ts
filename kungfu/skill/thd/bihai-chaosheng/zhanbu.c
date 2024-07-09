// Modified by Darken@SJ
#include <ansi.h>
inherit F_CLEAN_UP;

string exert_name() { return HIG"奇门占卜诀"NOR; }

int exert(object me, mixed arg)
{
        object ob;
        int i;
        
        if(!arg)
                return notify_fail("格式：exert zhanbu <id>| help <id> \n"); 
        if( objectp(arg) )
                return notify_fail("你要找的人就在一旁，还「占卜」干什么？\n"); 
        if( !stringp(arg) )
                return notify_fail("你要「占卜」谁？\n"); 
        if( me->query("zhanbu") )
                return notify_fail("你正在占卜呢。\n");
        if( sscanf(arg,"help %s", arg) == 1)
                i = 1;
        if( me->query("id") == arg )
                return notify_fail("你自己好端端的，还是发烧了？\n");
        if( !wizardp(me) && me->query("family/family_name") != "桃花岛" )
                return notify_fail("你不是桃花岛弟子，未得黄药师真传，如何能够占卜？\n");
        if( !wizardp(me) && (int)me->query("family/generation") < 2 )
                return notify_fail("你辈分过低，未得黄药师真传，如何能够占卜？\n");
        if( (int)me->query_skill("qimen-bagua", 1) < 100 )
                return notify_fail("你还不知道怎样占卜呢。\n");
        if( me->query("neili") < 2000 )
                return notify_fail("你内力不够，如何能够占卜？\n");
        if( !present("tie bagua", me) )
                return notify_fail("你需要一枚铁八卦才能占卜。\n");
                
        if(i) {
                if(!objectp(ob = present(arg, environment(me))))
                        return notify_fail("这里没有这个人。\n");
                if( !ob->is_character() || ob->is_corpse() )
                        return notify_fail("看清楚一点，那并不是活物。\n");
                if( !ob->query_temp("zhanbu") )
                        return notify_fail(ob->name()+"没有在占卜，不用你帮忙。\n");
                if( stringp(ob->query_temp("zhanbu")) )
                        return notify_fail("已经有人帮助"+ob->name()+"占卜了。\n");
                ob->set_temp("zhanbu", me->query("id") );
                message_vision(HIG"$N做个兰花手，凝神聚气，点在$n的百汇大穴之上！\n", me, ob);
                tell_object(ob, "你觉得"+me->name()+"的内力不断传来，头脑中的景象一下子清晰了不少！\n");
                me->start_busy(5);
        } else {
                i = ABS(35-(me->query("int")));
                me->set_temp("zhanbu", 1);
                message_vision(HIG"$N捏着手指，按照子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥的方位，仔细的掐算着。。。\n"NOR, me);
                call_out("check_target", i, me, arg);
                me->start_busy(i);
        }
        return 1;
}

int check_target(object me, string arg)
{
        int i, help = 1;
        object ob, where;
        string str;
        
        if( !me ) return 0;
        if( stringp(me->query_temp("zhanbu")) )
                help = 2;
        me->delete_temp("zhanbu");
        if( !living(me) || me->is_fighting() ) return 0;
        me->start_busy(1);
        i = me->query("max_neili") / 2;
        if( i < 2000 ) i = 2000;
        me->receive_damage("neili", i, "内力消耗殆尽而");
        ob = find_player(arg);
        if( !ob ) ob = find_living(arg);
        if( !ob || !me->visible(ob) || ob->query("no_quest") ) 
                tell_object(me, "你发现目前没这个人在线。\n");
        else if ( wizardp(ob) )
                tell_object(me, "你无法通过占卜知道巫师的位置。\n");
        else if( me->query("jingli") < ob->query_int() * 10 )
                tell_object(me, "但你沮丧地发现你现在的精力不够算出对方的位置。\n");
        else if ( !objectp(where = environment(ob)) ) 
                tell_object(me, HIW"可你怎么也算不出这个人在那里耶...\n"NOR);
        else if( random(me->query("kar")) * help < ob->query("kar") && userp(ob)){
                tell_object(me, HIY"可是你费了半天的力气，就是占卜不出"+ob->name(1)+"的位置。\n"NOR);
                me->add("jingli", -ob->query_skill("force"));
                me->start_busy(2);
        } else {
                if( random((int)me->query_int()) * help < (int)ob->query_int()/2 && userp(ob))
                        tell_object(ob, HIY"冥冥中你觉得好象有人正在窥探你所在的方位。\n"NOR);
                str = TASK_D->get_regions(base_name(where));
                if( str == "" )
                        str = "中原";
                if( help == 2 && stringp(where->query("short")) )
                        str += where->query("short");
                tell_object(me, sprintf(HIW"你掐指算出%s(%s)现在好象在%s"HIW"一带活动。\n"NOR,
                        (string)ob->name(1) ,capitalize((string)ob->query("id")) ,str));
                me->add("jingli", -ob->query_skill("force")*2);
                me->start_busy(2);
        }
        return 1;
}

int help(object me)
{
        write(HIG"\n「奇门占卜诀」："NOR"\n");
        write(@HELP

        使用奇门八卦的技巧，占卜一个人的所在位置。可以一人以上同时占卜来增
        加计算速度以及成功机会。

        要求：  碧海潮生等级 100 以上；
                奇门八卦等级 100 以上；
                最大内力 2000 以上；
                必须为黄药师亲传弟子。
HELP
        );
        return 1;
}

