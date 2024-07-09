// steal.c
//by pishou 97/6
// Modify by snowman@SJ 23/02/2000
// modify npc stealer.
// Modified by snowman@SJ 29/12/2000

#include <ansi.h>
#include <pktime.h>

int main(object me, string arg)
{
	string what, who;
	object ob, victim;
	int sp, dp;
	int cost = 2000 / (me->query_skill("stealing", 1) + 40 );

	if( environment(me)->query("no_fight") )
	return notify_fail("这里禁止行窃。\n");

	if(strsrch(file_name(environment(me)),"/d/wuguan/") >= 0)
	return notify_fail("武馆内禁止行窃。\n");

	if( me->query_temp("stealing") )
	return notify_fail("你已经在找机会下手了！\n");

	if( !arg || sscanf(arg, "%s from %s", what, who)!=2 ) return
	notify_fail("指令格式：steal <物品> from <人物>\n");

	victim = present(who, environment(me));
	if( !victim || victim==me)
	return notify_fail("你想行窃的对象不在这里。\n");

	if ( userp(victim) && me->query("age") < 18 && userp(me))
	return notify_fail("小小年纪就要练此身手，还是学点好吧。\n");

	if ( userp(victim) && victim->query("age") < 18 && userp(me))
	return notify_fail("对方身上好象没什麽油水。\n");

	if ( userp(me) && me->query("qi") < me->query("eff_qi")/2 )
	return notify_fail("你摇摇摆摆的还想偷东西？先补满自己的气血吧。\n");

	if( !wizardp(me) && wizardp(victim) )
	return notify_fail("玩家不能偷巫师身上的东西。\n");

	//	if (userp(me) && me->query("registered") < victim->query("registered"))
	//                return notify_fail("试用用户不能偷正式用户身上的东西。\n");

	if ( userp(me) && userp(victim) && me->query("no_pk"))
	return notify_fail("你已经金盆洗手了，就不要偷别人东西了。\n");

	if ( userp(me) && userp(victim)
	&& ( me->query("max_pot") - victim->query("max_pot") > 20
	|| me->query("max_pot") - victim->query("max_pot") < -20) )
	return notify_fail("你不能向人家乞讨了！\n");

	if ( userp(me) && userp(victim) && victim->query("no_pk"))
	return notify_fail(victim->name()+"已经金盆洗手了，就不要偷他的东西了。\n");

	if (pktime_limit(me, victim))
	return 0;

	if ( victim->query("job_npc") || victim->query("no_quest"))
	return notify_fail(victim->name()+"看起来没什么油水，不用偷了。\n");

	if( !victim->is_character() || victim->is_corpse() || victim->is_container() )
	return notify_fail(victim->name()+"里的东西用的着偷吗？随便拿就行了。\n");

	if ((cost > me->query("jing")) || (cost > me->query("jingli")))
	return notify_fail("你太累了, 先歇息一下。\n");

	if( me->is_busy()) return notify_fail("你正忙着呢。\n");

	if( !ob = present(what, victim) ) {
		object *inv;
		inv = all_inventory(victim);
		if( !sizeof(inv) )
		return notify_fail( victim->name() + "身上看起来没有什么值钱的东西好偷。\n");
		ob = inv[random(sizeof(inv))];
	}

	if(ob->query("invisible") && !wizardp(me)) return 0;
	if ( ob->query("unique") && userp(me))
	return notify_fail("哟，"+ob->name()+"这东西对方拿得紧呢，肯定偷不到耶。\n");

	if ( ob->query("no_steal") && userp(me))
	return notify_fail("哟，"+ob->name()+"这东西对方看得很紧，你还是别想了。\n");
	if(userp(me))
	sp = (int)me->query_skill("stealing", 1) * 10 + (int)me->query("kar") * 5
	- (int)me->query("thief") * 20;
	else sp = (int)me->query_skill("stealing", 1) * 10 + (int)me->query("kar") * 10;

	sp = sp * me->query("jing") / me->query("max_jing");
	sp = sp * (1 + me->query("jingli")) / (1 + me->query("eff_jingli"));
	if( sp < 1 ) sp = 1;

	if( me->is_fighting() )
	sp /= 2;

	dp = 300 * victim->query("jing") / (1+victim->query("max_jing"));
	dp = dp * (1 + victim->query("jingli")) / (1 + victim->query("eff_jingli"));
	dp = dp * 2 + (int)ob->weight()/25;

	if( victim->is_fighting() ) dp *= 10;


	me->receive_damage("jing", cost);
	me->receive_damage("jingli", cost);

	write("你不动声色地慢慢靠近" + victim->name() + "，等待机会下手 ...\n\n");

	me->set_temp("stealing", 1);
	if (userp(me) && userp(victim))
	{
		victim->set_temp("other_kill/"+me->query("id"), 1);

		log_file("ANSUAN",
		sprintf("%-18s steal  %-18s",
		me->name(1)+"("+capitalize(getuid(me))+")",
		victim->name(1)+"("+capitalize(getuid(victim))+")"
		),
		this_object(),
		);
	}
	if( userp(me) ){
		me->start_busy(3);
		call_out( "compelete_steal", 3, me, victim, ob, sp, dp);
	}

	else call_out( "compelete_steal", 1, me, victim, ob, sp, dp);

	return 1;
}

private void compelete_steal(object me, object victim, object ob, int sp, int dp)
{
	if (!me) return;

	me->delete_temp("stealing");

	if (!living(me) || !ob) return;

	if ( !victim || environment(victim) != environment(me) ) {
		tell_object(me, "太可惜了，你下手的目标已经走了。\n");
		return;
	}
	if( ob->query("equipped") ){
		tell_object(me, "哎呀，"+victim->name()+"正在使用"+ob->name()+"呢，这可偷不了。\n");
		return;
	}
	if( (!living(victim) || (random(sp+dp) > dp) )&& ob->query("imbued") ) {
		if (!wizardp(me) && ob->query("embedded")){
			tell_object(me," 你也太财迷了,连人家长在肉里的东西也要偷?\n");
			return;
		}
		if( ob->query("no_get") || !ob->move(me) ) {
			tell_object(me, "你摸到一" + ob->query("unit") + ob->name()+"，可是对你而言太重了，不得不放弃。\n");
			return;
		}
		tell_object(me, HIW "得手了！\n\n" NOR);
		tell_object(me, "你成功地偷到一" + ob->query("unit") + ob->name() + "！\n");

		if( living(victim) && !userp(victim) ) {
			me->improve_skill("stealing", random(me->query_int()+me->query_skill("stealing",1)));
			me->add("potential", 1+random(me->query_skill("stealing")/5));
			if (me->query("potential") > me->query("max_pot"))
			me->set("potential", me->query("max_pot"));
			me->add("combat_exp", (int)((victim->query("combat_exp") + 1000) / (me->query("combat_exp") + 1000) * 3));
		}
		if( !userp(me) )
		me->improve_skill("stealing", random(me->query("int"))*3);

		if( random(sp) < dp/2 && userp(me))
		message("vision", "你看到" + me->name() + "鬼鬼祟祟地从"
		+ victim->name() + "身上偷走了一" + ob->query("unit")
		+ ob->name() + "！\n", environment(me), ({ me, victim }) );
		if( !userp(me) ){
			me->set_temp("steal", 1);
			if(me->query("stealer_no_hold")){
				log_file("quest/npc_steal",
				sprintf("%s steal %s from %s。\n",
				me->name()+"("+geteuid(me)+")",
				ob->name(),
				victim->name()+"("+victim->query("id")+")"
				), victim
				);
				destruct(ob);
			}
		}
	}

	else {
		if( random(sp) > dp/2 ) {
			tell_object(me, victim->name() + "不经意地一转头，你急忙将手缩了回去！\n还好，没有被发现。\n");
			return;
		}

		me->add("steal_f", 1);
		tell_object(me, HIR "糟糕！你失手了！\n\n" NOR);
		message_vision("$N一回头，正好发现$n的手正抓着$P身上的" + ob->name() + "！\n\n$N喝道：「干什么！」\n\n", victim, me);
		me->improve_skill("stealing", 1, 1);
		// Lklv add no_pk check
		if( userp(victim) && !me->query("no_pk") ){
			if( me->query("steal_f") > random(me->query_skill("stealing", 1)) ){
				me->kill_ob(victim);
				me->set_skill("stealing", me->query_skill("stealing", 1) -1 );
				if( me->query_skill("stealing", 1) < 1 ) me->delete_skill("stealing");
				me->add("steal_f", -(1+random(5)));
				message_vision("$N发现自己又失手了，不由得恼羞成怒！！\n", me);
			}
			else me->fight_ob(victim);
			victim->fight_ob(me);
		}
		else victim->kill_ob(me);
		if(userp(me)) me->start_busy(3);
		if ( me->query("no_pk") )
		me->add_busy(random(2));
		else me->set_temp("steal", -1);
		me->add("thief", 1);
	}
}

int help(object me)
{
write(@HELP
指令格式 : steal <某物> from <某人>

这个指令让你有机会偷到他人身上的东西。成功了, 当然你就能获得
该样物品。可是, 马有失蹄, 人总有失风的时候, 当你失败时当然就
得付出代价, 至于是什么代价......靠你自己去发掘罗。
HELP
    );
    return 1;
}
