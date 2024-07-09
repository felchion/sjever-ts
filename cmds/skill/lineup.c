// lineup.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object *lineup;
	object ob;

    if( !arg ) 
    {
        if( !pointerp(lineup = me->query_lineup()) )
            return notify_fail("你现在并没有加入任何阵法。\n");
        write("你现在阵法中的成员有：\n  " + implode(lineup->name(1), "\n  ") + "。\n");
        return 1;
    }
	if(arg=="refuse")
	{
		if(!me->query_temp("pending/lineup"))
			return notify_fail("现在并没有人邀请你加入阵法。\n");
		ob=me->query_temp("pending/lineup");
		if(objectp(ob))
			tell_object(ob,"对方拒绝了你的邀请。\n");
		me->delete_temp("pending/lineup");
		write("你拒绝了对方加入阵法的邀请。\n");
		return 1;
	}
    if( arg=="dismiss" ) 
    {
        if( !pointerp(lineup = me->query_lineup()) && !me->query_form() )
            return notify_fail("你现在并没有加入任何阵法。\n");
        if( me->is_lineup_leader() ) 
        {
            message_vision( "$N将阵法解散了。\n", me);
        } 
        else 
        {
            message_vision( "$N决定脱离阵法。\n", me);
        }
        me->dismiss_lineup();
        return 1;
    }
    if( sscanf(arg, "with %s", arg)==1 ) 
    {
        if( !(ob = present(arg, environment(me)))
        	|| !living(ob)
        	|| !ob->is_character()
        	|| ob==me )
        	return notify_fail("你想和谁一起布阵？\n");

        if( ob->query_condition("killer") && ! me->query_condition("killer"))
            return notify_fail("对方正在被官府通缉，不能加入你的阵法。\n");

        if( !pointerp(me->query_lineup()) || me->is_lineup_leader() ) 
        {
		if( ob==(object)me->query_temp("pending/lineup"))
		{
		if (!ob->query_form())
			return notify_fail("对方已经把阵法解散了。\n");
		if (!me->query_skill(ob->query_form(), 1))
			return notify_fail("你不会对方的阵法。\n");
            	if( sizeof( ob->query_lineup() ) >= 
            		SKILL_D(ob->query_form())->query_limit() )
            		return notify_fail("对方的阵法已经有足够的人手了。\n");

				if( arrayp(me->query_lineup()) || me->query_form() )
            		return notify_fail("你必须退出当前阵法才能加入新的阵法。\n");
            	
                if( !pointerp(me->query_lineup()) ) 
                {
                    me->set_form(ob->query_form());
			me->set_lineup_leader(ob);
                    ob->add_lineup_member(me);
                    message_vision("$N决定加入$n的阵法。\n", me, ob);
                    if( me->is_fighting() && userp(me) )
                        me->start_busy(1);
                    if( ob->is_fighting() && userp(ob) )
                        ob->start_busy(1);
                }
                else 
                {
			ob->set_form(me->query_form());
			ob->set_lineup_leader(me);
                    me->add_lineup_member(ob);
                    message_vision("$N决定让$n入阵。\n", me, ob);
                    if( ob->is_fighting() && userp(ob) )
                        ob->start_busy(1);
                    if( me->is_fighting() && userp(me) )
                        me->start_busy(1);
                }
                me->delete_temp("pending/lineup");
                return 1;
            } 
            else 
            {
            	if( !me->query_form())
            		return notify_fail("你没有选择要主持哪种阵法。\n");
            	if( !ob->query_skill(me->query_form(),1) )
            		return notify_fail("对方不懂这种阵法，无法加入。\n");
				if( arrayp(ob->query_lineup()) || ob->query_form() )
            		return notify_fail("对方目前也在布阵，无法接受你的邀请。\n");
				if( ob->query_temp("pending/lineup")==me )
					return notify_fail("你已经邀请过对方了。\n");
				if( ob->query_temp("pending/lineup") )
					return notify_fail("已经有别人邀请过对方了。\n");
                message_vision("$N邀请$n加入$P的" + to_chinese(me->query_form()) +"。\n", me, ob);
                tell_object(ob, YEL "如果你愿意加入，请用 lineup with " + me->query("id") + "。\n" NOR);
                ob->set_temp("pending/lineup", me);
                return 1;
            }
        } 
        else
            return notify_fail("只有主持阵法者才可以邀请别人加入。\n");
    }
    if( sscanf(arg, "form %s", arg)==1 ) 
    {
        if( pointerp(lineup=me->query_lineup()) || me->query_form() )
            return notify_fail("你必须首先退出当前的阵法才能组织新的阵法。\n");
        if( !me->query_skill(arg, 1) )
            return notify_fail("这种阵形你没有学过。\n");
		if(SKILL_D(arg)->type()!="array")
            return notify_fail("这种武功不是阵法。\n");
		if( me->query_temp("pending/lineup") )
			return notify_fail("现在正有人邀请你加入，你必须首先拒绝对方才能组织新的阵法。\n");
        me->set_form(arg);
        me->set_temp("lineup/env",environment(me));
        call_out("check_lineup",1,me);
        message_vision(HIY"" 
        	+ "$N鼓足真气，纵声长啸：“布"
        	+ to_chinese(arg)
        	+ "”，声音滚滚地传了出去。\n"NOR, me );
        return 1;
    }
}


int filldata(object obj1)
{
	if(wiz_level(obj1)) return 0;
	if(!environment(obj1)) return 0;
	else return 1;
}

int check_lineup(object me)
{
	object *lineup,*enemy;
	int i,j;
    mixed *ob_list;
    int max;
	
	if( !me )
	{
   		ob_list = users();
   		ob_list = filter_array(ob_list, (: filldata :));
   		max=sizeof(ob_list);
   		for( i = 0;i<max;i++)
   		{
			lineup = ob_list[i]->query_lineup();
   			if( lineup && !lineup[0] )
   			{
   				for( j=0;j<sizeof(lineup);j++)
   				{
   					if( lineup[j] )
   					{
   						lineup[j]->set_lineup(0);
   						lineup[j]->delete_form();
   						lineup[j]->remove_lineup_effect();
   					}
   				}
   			}
   		}
	    return 1;
	}

	if( environment(me)!=me->query_temp("lineup/env") 
		|| me->is_ghost() )
	{
		message_vision(HIY"阵主已经不在主持阵法的位置上，阵法解散。\n"NOR, me );
    	me->dismiss_lineup();
    	return 1;
	}


	if( !me->query_form() )
		return 1;

	lineup=me->query_lineup();

	if( arrayp(lineup) )
	{
		for( i = 0;i<sizeof(lineup);i++ )
		{
			if( !lineup[i] || environment(lineup[i])!=me->query_temp("lineup/env")
				|| lineup[i]->is_ghost() )
			{
				message_vision(HIY"因为有人脱离阵法，阵法的威力受到影响。\n"NOR,me);
				if(lineup[i])
				{
					lineup[i]->set_lineup(0);
					lineup[i]->delete_form();
					lineup[i]->remove_lineup_effect();
				}
				lineup -= ( {lineup[i]} );
				i = 0;
			}
		}

		for( i=0;i<sizeof(lineup);i++)
			lineup -= ( {0} );

		for( i=0;i<sizeof(lineup);i++)
		{
			if( lineup[i] )
			{
				lineup[i]->set_lineup( lineup );
				lineup[i]->remove_lineup_effect();
				lineup[i]->apply_lineup_effect();
			}
		}

		if( me->is_fighting() )
		{
			enemy=me->query_enemy();
			message_vision( ( SKILL_D(me->query_form() ) )->query_action(),
				lineup[random(sizeof(lineup))],
				enemy[random(sizeof(enemy))] );
		}
	}

		
	call_out("check_lineup",1,me);

	return 1;
}


int help(object me)
{
    write(@HELP
阵法指令使用方法:

lineup form <阵法> - 组织某种阵法，组成阵法的状态只要一移动就会解除。
lineup with <某人> - 跟某人组成阵法，自己或对方必须是某个阵法的主持者。
lineup dismiss     - 离开阵法。若下此指令的是阵法的主持者, 则整个阵法会解散。
lineup refuse      - 拒绝加入阵法的邀请。

直接下 lineup 指令时则会显示你目前是否加入阵法及阵法成员名单。

HELP
    );
    return 1;
}
