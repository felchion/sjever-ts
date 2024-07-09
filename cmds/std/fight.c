// fight.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object obj, old_target;
        string msg;

	if( environment(me)->query("no_fight") )
		return notify_fail("这里禁止战斗。\n");

	if(!arg || !objectp(obj = present(arg, environment(me))))
		return notify_fail("你想攻击谁？\n");

	if( !obj->is_character() )
		return notify_fail("看清楚一点，那并不是生物。\n");

	if( obj->is_fighting(me) )
		return notify_fail("加油！加油！加油！\n");

	if( !living(obj) )
		return notify_fail(obj->name() + "已经无法战斗了。\n"); 

	if ( obj->query("job_npc"))
		return notify_fail(obj->name()+"看起来没什么兴趣和你战斗。\n");

	if(obj==me) return notify_fail("你不能攻击自己。\n");

	if( userp(obj) && (object)obj->query_temp("pending/fight")!=me ) {
		if(me->query("combat_exp") > obj->query("combat_exp")*2){
			if(me->query("shen") < -10000)
		                msg = "\n$N对着$n一瞥："+RANK_D->query_self_rude(me)+"看你还不赖，怎么样，来和我耍两手试试？\n\n";
		        else msg = "\n$N对着$n看了看："+RANK_D->query_respect(obj)+"看起来身手不错啊。来，让我指点指点你几招怎么样？\n\n";
		        }
		else if(obj->query("combat_exp") > me->query("combat_exp")*2){
			if(me->query("shen") < -10000)
		                msg = "\n$N对着$n一抱拳："+RANK_D->query_respect(obj)+"，"+RANK_D->query_self(me)+"这不如眼的庄稼把式请您指点几招如何？\n\n";
		        else msg = "\n$N对着$n深深一鞠道："+RANK_D->query_self(me)+me->name()+"，在此恳请"+obj->name()+RANK_D->query_close(obj, me)+"赐教！\n\n";
		        }
		else {
			if(me->query("shen") < -10000)
		                msg = "\n$N对着$n哈哈一笑：早听人说"+RANK_D->query_respect(obj)+"武艺高强，"+RANK_D->query_self_rude(me)+"想在此领教几式高招，如何？\n\n";
		        else msg = "\n$N对着$n说道：" + RANK_D->query_self(me) + me->name() + "，领教"+ RANK_D->query_respect(obj) + "的高招！\n\n";
		        }
		message_vision(msg, me, obj);
		if( objectp(old_target = me->query_temp("pending/fight")) )
			tell_object(old_target, YEL + me->name() + "取消了和你比试的念头。\n" NOR);
		me->set_temp("pending/fight", obj);
		tell_object(obj, YEL "如果你愿意和对方比试武艺，请你也对" + me->name() + "下一次 fight 指令。\n" NOR);
		write(YEL "由于对方是由玩家控制的人物，你必须等对方同意才能进行比试。\n" NOR);
		return 1;
	}

	({ me, obj })->delete_temp("guarding");
	if( obj->query("can_speak") ) {
		if(me->query("combat_exp") > obj->query("combat_exp")*2 && userp(obj)){
			if(me->query("shen") < -10000)
		                msg = "\n$N一声冷笑：好吧，看你这"+RANK_D->query_rude(obj)+"还不赖，就让我教训教训你吧！\n\n";
		        else msg = "\n$N“呵呵”一笑：既然"+RANK_D->query_respect(obj)+"请教，就让我来指点指点你几招吧！\n\n";
		        }
		else if(obj->query("combat_exp") > me->query("combat_exp")*2 && userp(obj)){
			if(me->query("shen") < -10000)
		                msg = "\n$N干笑一声：哎哟，"+RANK_D->query_respect(obj)+"您这么看得起"+RANK_D->query_self(me)+"，我只好在此献献丑了！\n\n";
		        else msg = "\n$N肃然起敬：承蒙"+RANK_D->query_respect(obj)+"看得起"+RANK_D->query_self(me)+"，我只好恭敬而不如从命了！\n\n";
		        }
		else {
			if(me->query("shen") < -10000)
		                msg = "\n$N对着$n嘿嘿一笑：来来来！就让"+RANK_D->query_self_rude(me)+"来领教领教"+RANK_D->query_respect(obj)+"的高招吧！\n\n";
		        else msg = "\n$N对着$n说道：" + RANK_D->query_self(me) + me->name() + "，领教"+ RANK_D->query_respect(obj) + "的高招！\n\n";
		}
		obj->delete_temp("pending/fight");
        message_vision(CYN + msg + NOR, me, obj);

		notify_fail("看起来" + obj->name() + "并不想跟你较量。\n");
		if( !userp(obj) && !obj->accept_fight(me) ) return 0;

		me->fight_ob(obj);
		obj->fight_ob(me);
	} else {
		message_vision("\n$N大喝一声，开始对$n发动攻击！\n\n", me, obj);
		me->fight_ob(obj);
		obj->kill_ob(me);
	}

	return 1;
}
int help(object me)
{
  write(@HELP
指令格式 : fight <人物>
 
这个指令让你向一个人物「讨教」或者是「切磋武艺」，这种形式的战斗纯粹是
点到为止，因此只会消耗体力，不会真的受伤，但是并不是所有的  NPC 都喜欢
打架，因此有需多状况你的比武要求会被拒绝。
 
其他相关指令: kill

PS. 如果对方不愿意接受你的挑战，你仍然可以迳行用 kill 指令开始战斗，有
    关 fight 跟 kill 的区分请看 'help combat'.
HELP
    );
    return 1;
}
 
