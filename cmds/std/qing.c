#include <ansi.h>

int main(object me, string arg)
{
	object ob;
	mapping quest;
	string msg;
	int i,j;

	if (!mapp(quest = me->query_temp("quest")) || quest["type"] != "请")
		return notify_fail("你的任务好象不是要邀请谁吧？\n");

	if( me->is_busy())
		return notify_fail("你正忙着呢，先歇口气吧。\n");

	if( me->is_fighting())
		return notify_fail("你正在打架呢，等会儿再说吧。\n");

	if( me->query_temp("yaoqing") )
		return notify_fail("你正在极力邀请人家呢！\n");

	if( !arg || !objectp(ob=present(arg, environment(me))))
		return notify_fail("这里没有这个人耶。\n");

	if( !ob || ob == me)
		return notify_fail("你想邀请的对象不在这里。\n");

	if( !living(ob) )
		return notify_fail("你要先等他醒过来再说。\n");

	if( !wizardp(me) && wizardp(ob) )
		return notify_fail("想邀请巫师吃饭？好主意，不过用 chat 效果会更好。\n");

	if( ob->is_fighting() || ob->is_busy())
		return notify_fail("没看见"+ob->name() + "正忙着吗？没空理你！\n");

	if( ob->query_temp("songshan_job") == me)
		return notify_fail(ob->name() + "已经接受了邀请，你不用再费劲啦。\n");

	if (quest["id"] != ob->query("id"))
		return notify_fail("你的任务好象不是要邀请这人吧？\n");

	i = (int)me->query_skill("force") + (int)me->query_kar() * 5;
	j = (int)ob->query_skill("force") + (int)ob->query_kar() * 5;

	if(me->query("shen") < ob->query("shen")*2) i += 100;
	else if(ob->query("shen") < me->query("shen")*2) j += 100;
	else if(ob->query("shen") < me->query("shen")) i += 10;
	else j += 10;

	if(me->query("jing") < me->query("eff_jing")/2
	|| me->query("jingli") < me->query("eff_jingli")/2)
		return notify_fail("你精神不振，先休息一下吧！\n");

	me->set_temp("last_damage_from", "脱力而");
	me->receive_damage("jing", me->query("jing")/3, me);
	me->receive_damage("jingli", me->query("jingli")/3, me);

	switch(random(4)){
		case 0: msg = CYN"\n$N一脸的恭敬，对$n抱拳鞠躬道：奉嵩山派左掌门之命，前来请"+RANK_D->query_respect(ob)+"出山相助。\n"NOR;break;
		case 1: msg = CYN"\n$N对着$n深深一躬：晚辈"+me->name()+"给"+RANK_D->query_respect(ob)+"请安了，奉嵩山派掌门命令请"+RANK_D->query_respect(ob)+"出山相助。\n"NOR;break;
		case 2: msg = CYN"\n$N面带笑容，对$n道：奉嵩山派左掌门之命，前来邀请前辈出山相助。久闻"+RANK_D->query_respect(ob)+"大名，今日总算能一睹金面，果然名不虚传！\n"NOR;break;
		case 3: msg = CYN"\n$N对$n鞠躬道：嵩山派掌门人遣"+RANK_D->query_self(me)+"前来恭请"+RANK_D->query_respect(ob)+"出山相助，顺道一叙故人之情。\n"NOR;break;
	}

	if(ob->is_killing(me->query("id"))){
		ob->remove_all_enemy();
		msg += "\n$n上下打量了$N一眼，身形向后一跃，跳出战圈不打了。\n";
	}

	message_vision(msg, me, ob);
	me->set_temp("yaoqing", 1);
	me->start_busy(2);
	call_out( "compelete_yaoqing", 3, me,ob, i, j);

	return 1;
}

private void compelete_yaoqing(object me, object ob, int i, int j)
{
	object where;

	if (!me) return;
	where = environment(me);
	me->delete_temp("yaoqing");

	if (!living(me))
		return;
	if( !ob || environment(ob) != where ) {
		tell_object(me, "太可惜了，你要邀请的人已经走了。\n");
		return;
	}

	if(ob->is_killing(me->query("id"))) ob->remove_killer(me);

	if( living(ob) && (random(i+j) > j) ) {
		if (ob->query_temp("last_lost_to") == me && ob->query_temp("yaoqing") == me){
			message_vision(
				CYN"\n$N哈哈一笑：“"
				+RANK_D->query_respect(me)
				+"的功夫果然不错，承蒙左掌门看得起，你就给我带路吧！”\n"NOR, ob
			);
			ob->set_temp("songshan_job", me);
			ob->delete_temp("yaoqing");
			ob->set("chat_c", ob->query("chat_chance"));
			if (ob->query("location"))
				ob->set_temp("location", 1);
			ob->set("location", 1);
			ob->delete("chat_chance");
			ob->apply_condition("songshan_busy", 20+random(30));
			ob->set_leader(me);
			ob->reincarnate();
			message_vision("$N决定跟随$n一起行动。\n", ob, me);
			return;
		}
		if( ob->query("jing") * 100 / ob->query("max_jing") < 90
		|| ob->query("qi") * 100 / ob->query("max_qi") < 90
		|| ob->query("jingli") * 100 / ob->query("eff_jingli") < 70
		|| ob->query("neili") * 100 / ob->query("max_neili") < 70 ) {
			message_vision("$N脸色苍白，只看了$n一眼。看来是身体不适。\n", ob, me);
			return;
		}
		message_vision(
			HIY"\n$N扫了$n一眼道：“"+RANK_D->query_rude(me)
			+"，左兄既然派你来，想必有几分真才实学，就让"
			+RANK_D->query_self_rude(ob)+"来掂量掂量你吧！”\n"NOR, ob, me
		);
		ob->delete_temp("last_lost_to");
		ob->set_temp("yaoqing", me);
		ob->fight_ob(me);
		me->fight_ob(ob);
		return;
	}
	switch(random(4)){
		case 0:
			message_vision("$N转过头来，上上下下看了$n一遍，又转过头去了。\n", ob, me);
			break;
		case 1:
			message_vision("$N轻蔑地扫了$n一眼，脸上露出不屑的表情。\n", ob, me);
			break;
		case 2:
			message_vision(
				"$N嘿嘿一声："+RANK_D->query_rude(me)+"，想请动"
				+RANK_D->query_self_rude(ob)+"，可得花不少工夫啊。\n", ob, me
			);
			break;
		case 3:
			message_vision(
				"$N一脸不耐烦的表情：去！去！去！别来打扰"
				+RANK_D->query_self_rude(ob)+"。\n", ob
			);
			break;
	}
	me->start_busy(3);
}

int help(object me)
{
write(@HELP
邀请某人。

指令格式 : qing <某人>

HELP
    );
    return 1;
}
