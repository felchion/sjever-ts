// suicide.c

// Modified by mxzhao 2004/02/21
// Modified by felchion 2008/01/28

#include <ansi.h>
#include <command.h>
#include <mudlib.h>
#include <database.h>

private void check_password(string passwd, object me, int forever);
private void confirm_password(string passwd, object me, int forever);

int main(object me, string arg)
{
	if (wiz_level(me) )
	{
		return notify_fail("不可以随便自杀。"
			"如果你暂时有什么想不开的，还是多和大家聊聊吧。\n");
	}

	if( me->is_busy() || me->is_fighting() )
	{
		return notify_fail("你上一个动作还没完成。\n");
	}

	if ( mapp(me->query("Bind_ID")) ) 
		return notify_fail("已经有其它帐号和这个帐号绑定在一起！\n");
	
	if( arg && arg != "-f") 
	{
		return notify_fail("请用 suicide -f 确定自杀。\n");
	}
	
	if (environment(me)->query("no_fight"))
	{
		return notify_fail("这里似乎没有自杀的条件。\n");
	}

	if (!me->query_temp("link_ob"))
	{
		return notify_fail("你不是正常连线进入的，不能自杀。\n");
	}
	
	if(!arg)
	{
		object weapon;
		object zhong;
		int needdest = 0;
		
		if(me->query("age") < 30 ) return notify_fail("你年纪还轻，未来是伟大滴，前途是光明滴！\n");
				
		weapon = me->query_temp("weapon");		
		zhong = find_object(NPC_D("zhong-shentong"));
		if(!zhong)
		{
			zhong = load_object(NPC_D("zhong-shentong"));
			needdest = 1;
		}
				
		if( zhong->query("winner") == me->query("id") )
		{
			if(needdest) destruct(zhong);
			return notify_fail("你身为武林盟主岂可轻言就死？\n");
		}
		
		if(needdest) destruct(zhong);
												
		if(weapon)
		{
			string* weapon_list = ({ "sword","axe","blade","dagger","fork","hook","spear",});
			
			if( member_array(weapon->query("skill_type"),weapon_list)==-1 )
			{
				return notify_fail("你手中"+weapon->name()+"不够锋利。\n");
			}
			//sharpness
		}
		else
		{
			if( me->query_skill("force") < 500 )
			{
				return notify_fail("你的内功修为还不足以自断经脉。\n");				
			}			
			if( me->query("neili") < 4000 )
			{
				return notify_fail("你的内力不够了。");
			}			
		}
		
		write("也许游戏让你心灰意冷，也许你成了独孤求败的知音……\n确定的话请输入您的登陆密码：");
		input_to("check_password_die", 1, me, 1);
		return 1;
	}
	write("选择自杀会删除你的游戏和论坛帐号。\n"
		"确定的话请输入您的登陆密码：");

	input_to("check_password", 1, me, 1);
	return 1;
}

private void check_password_die(string passwd, object me, int forever)
{
	string old_pass = PASSWD_D->get_passwd(geteuid(me));
	object weapon = me->query_temp("weapon");
	
	if (!old_pass)
	{
		tell_object(me, "暂时不能自杀。\n");
		return;
	}

	if( crypt(passwd, old_pass)!=old_pass && passwd!=old_pass)
	{
		tell_object(me, "密码错误！\n");
		return;
	}
	
	tell_object(me,"\n\n");
	
	if(weapon)
	{
		message_vision(HIR"$N昂首仰望天穹深处，心意刚决，力发掌缘，挥起手中利刃，向自己咽喉处发力抹去……\n"NOR,me);
		tell_object(me,"万点珠红刹那间喷溅汹涌，……再也不可回首那红尘往事……\n");
		me->set_temp("last_damage_from","于"+environment(me)->query("short")+"自刎而死。");
	}
	else
	{
		message_vision(HIR"$N惨然一笑，盘膝而坐，五心向天，慢慢将丹田中气逆转，直攻向自己的奇经八脉……\n"NOR,me);
		tell_object(me,"你的面色由红润至惨白，由惨白终至蜡黄，再无半点生机……\n");
		me->set_temp("last_damage_from","于"+environment(me)->query("short")+"自断经脉而死。");
	}
			
	me->delete_temp("faint_by");
	me->die();
}

private void check_password(string passwd, object me, int forever)
{
	string old_pass = PASSWD_D->get_passwd(geteuid(me));

	if (!old_pass)
	{
		tell_object(me, "暂时不能自杀。\n");
		return;
	}

	if( crypt(passwd, old_pass)!=old_pass && passwd!=old_pass )
	{
		tell_object(me, "密码错误！\n");
		return;
	}

	if (!me->query("confirm_passwd"))
	{
		confirm_password(passwd, me, forever);
	}
	else
	{
		write("请输入您的确认密码：");
		input_to("confirm_password", 1, me, forever);
	}
}

private void confirm_password(string passwd, object me, int forever)
{
	string confirm_pass = me->query("confirm_passwd");
	
	if ( confirm_pass && crypt(passwd, confirm_pass) != confirm_pass)
	{
		write("确认密码错误！\n");
		return;
	}

	if (forever)
	{
		tell_object( me, HIR "\n你决定要自杀了。\n\n" NOR);

		if (!wiz_level(me)) 
		{
			CHANNEL_D->do_channel(this_object(), "rumor", 
				me->short(1) + "决定要自杀了。");
		}

		me->set_temp("suicide_countdown", 6);
		me->start_busy( (: call_other, this_object(), "slow_suicide" :),
			(: call_other, this_object(), "halt_suicide" :) );
	}
}

private int slow_suicide(object me)
{
	object link_ob = me->query_temp("link_ob");
	int stage = me->add_temp("suicide_countdown", -1);

	if (!link_ob)
	{
		return 0;
	}

	if (stage > 1)
	{
		if( stage%5 == 0 )
		{
			tell_object(me, HIR "你还有" + chinese_number(stage+stage) 
				+ "秒的生存时间，想停止可以 halt。\n" NOR);
		}

		return 1;
	}

	log_file("static/SUICIDE",	sprintf("%s 自杀\n", geteuid(me)), me);

	seteuid(getuid());

	write(HIY"放松一下心情，好好休息吧！\n"NOR);

	if (me->query("g_title") && me->query("g_level") == 1)
	{
		write_file("/log/group", sprintf("%s %s 解散，%s ( %s ) 自杀。\n", 
			ctime(time())[4..19], me->query("g_title"), 
			me->query("name"), me->query("id")));
	}

	rm(link_ob->query_save_file() + ".o");
	rm(me->query_save_file() + ".o");

	if(!wiz_level(me))
	{
		foreach (object inv in all_inventory(me))
		{
			if (inv->query("money_id"))
			{
				destruct(inv);
			}
			else if ( !inv->query_autoload() )
			{
				DROP_CMD->do_drop(me, inv);
			}

			me->delete("balance");
		}
	}

	if (wiz_level(me))
	{
		link_ob->save();
		me->save();

		cp(link_ob->query_save_file() + SAVE_EXTENSION, 
			link_ob->query_save_file() + ".ooo");
		rename(me->query_save_file() + SAVE_EXTENSION, 
			me->query_save_file() + ".ooo");
	}
	else 
	{
		
		//删除用户信息
		if (me->query("combat_exp") > 50000)	
			dbquery("insert into Users_backup select * from Users where U_Username='" + geteuid(me) + "'");
		dbquery("delete from Users where U_Username='" + geteuid(me) + "'");
		// 删除论坛帐号
		if (me->query("combat_exp") > 50000) 
			dbquery("insert into BBSID_backup_new select * FROM members WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
	//	dbquery("DELETE FROM members WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
	//	dbquery("update members set extcredits1 = extcredits1 WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
		dbquery("update members set extcredits1 = -999999999 WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
		dbquery("update members set credits = -999999999,username = CONCAT('自杀用户',uid),groupid=21  WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
		dbquery("update discuz6.uc_members set username = CONCAT('自杀用户',uid) WHERE username = '"+geteuid(me) +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
		// 删除游戏存档
		me->backup();
		dbquery("DELETE FROM Data WHERE U_ID = '"+link_ob->query_save_file() + SAVE_EXTENSION+"'");
		dbquery("DELETE FROM Data WHERE U_ID = '"+me->query_save_file() + SAVE_EXTENSION+"'");
		rm(link_ob->query_save_file() + SAVE_EXTENSION);
		rm(me->query_save_file() + SAVE_EXTENSION);
	}

	tell_room(environment(me), me->name() +
		"自杀了，以后你再也看不到这个人了。\n", ({me}));

	if (!wiz_level(me))
	{
		CHANNEL_D->do_channel(this_object(),"rumor", 
			me->short(1) + "自杀了，以后你再也见不到这个人了。");
	}

	destruct(link_ob);
	destruct(me);
	return 0;
}

int halt_suicide(object me)
{
	tell_object(me, "你放弃了自杀的念头。\n");

	if (!wiz_level(me))
	{
		CHANNEL_D->do_channel(this_object(), "rumor", 
			me->name(1) + "决定继续勇敢地活下去。");

		me->start_busy(600);
	}

	write("你陷入了长时间的沉思之中。\n");
	return 1;
}


int help (object me)
{
	write(@HELP
指令格式: suicide [-f]

如果因为某种原因你不想活了，你可以选择自杀。
自杀二十四小时以后才能重新登录。

如果使用 -f 这个参数则直接删除玩家档案，此操作不
可逆，请玩家慎用。

HELP);

	return 1;
}
