// destory.c
// by augx@sj 4/11/2002

#include <ansi.h>
inherit F_CLEAN_UP;
#include <group.h>

int help(object me)
{
	write(@HELP
指令格式 :
destory [物品id]
摧毁自己帮派里的某样物品。

destory room
摧毁已经建造好的房屋。

destory to [方向]
摧毁某个方向的房间，要摧毁的房间必须符合以下条件：
1. 有且只有一个出口；
2. 里面没有物品或NPC；
3. 里面没有其他玩家。

HELP
	);
	return 1;
}


int main(object me, string arg)
{
	object weapon, ob, env = environment(me);
	object * inv;
	string str,gid;
	int i,delay;

	if( !arg ) return help(me);
	if( !env || !base_name(env) )
		return notify_fail("你无法在这里摧毁物品或房间。\n");
	if( !(gid=me->query("group/id")) )
		return notify_fail("你还没有参加帮会，摧毁什么？\n");
	if( me->is_busy() )
		return notify_fail("你正忙着呢。\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );
	// Added by mxzhao 2004/02/24
	if( !me->query("group/class") > 2 )
	{
		return notify_fail("你的等级不够。\n");
	}
	// end
	if( !(weapon=me->query_temp("weapon")) || weapon->query("skill_type")!="hammer" )
		return notify_fail("你没有锤子，如何摧毁房间和物品啊？\n");

	if( arg == "room" ) {
		if( env->query("group1") != gid )
			return notify_fail("你只能在自己的帮派里摧毁房屋。\n");
		if( !env->query("indoors") )
			return notify_fail("这里并没有房屋啊。\n");
		if( !me->query("group/class") > 2 )
			return notify_fail("你的等级不够。\n");

		if( !GROUP_D->create_room(
			base_name(env)+".c",
			"石路","这是一条刚刚建造好，还没有粉刷的石路。\n",
			GROUP_D->get_room_exits(env),GROUP_D->get_room_objects(env),
			(string)env->query("group1"),(string)env->query("group2"),
			0, env->query("have_board") )
		)
			return notify_fail("写文件失败，请报告巫师！\n");

		GROUP_D->encrypt_file(base_name(env)+".c");
		message_vision("$N手持"+weapon->name()+"砸向房屋......\n",me);
		str = "你把这间房屋完全的摧毁了。\n";
	}
	else if( sscanf(arg,"to %s",arg) == 1 ) {
		if( !me->query("group/class") > 1 )
			return notify_fail("你的等级不够。\n");
		if( !(str=env->query("exits/"+arg)) )
			return notify_fail("没有这个方向！\n");
		if( !(ob=find_object(str)) )
			if( !(ob=load_object(str)) )
				return notify_fail("这个方向的出口有问题，请报告巫师。\n");
		if( !mapp(ob->query("exits")) || sizeof(ob->query("exits"))>1 )
			return notify_fail("要摧毁的房间，必须有且只有一个出口。\n");
		if( mapp(ob->query("objects")) && sizeof(ob->query("objects"))>0 )
			return notify_fail("你必选先摧毁房间里的人和物品，然后才能摧毁此房间。\n");
		if( ob->query("group1") != gid )
			return notify_fail("你只能摧毁自己帮派的房间。\n");
		inv = all_inventory(ob);
		for(i=0;i<sizeof(inv);i++)
			if( inv[i] && userp(inv[i]) )
				return notify_fail("你要摧毁的房间里还有别的玩家，无法摧毁房间。\n");

		message_vision("$N手持"+weapon->name()+"砸向"+ob->query("short")+"......\n",me);

		if( env->query("group") ) {
			str = "你把"+ob->query("short")+"完全的摧毁了。\n";
			log_file("group/destory", 
				sprintf("%8s%-10s 摧毁 %-8s的%s。", 
					me->name(1), 
					"("+me->query("id")+")", 
					gid,
					ob->query("short"),
				), me
			);
			GROUP_D->rm_dir(GROUP_DIR+gid+"/npc/");
			rmdir(GROUP_DIR+gid+"/npc/");
			GROUP_D->mirror_file( GROUP_DIR+gid+"/npc/","","delete" );

			GROUP_D->rm_dir(GROUP_DIR+gid+"/");
			rmdir(GROUP_DIR+gid+"/");
			GROUP_D->mirror_file( GROUP_DIR+gid+"/","","delete" );

			GROUP_D->set_entry(gid,"");

			cp(base_name(env)+".origin",base_name(env)+".c");
		}
		else {
			if( sscanf(base_name(ob),GROUP_DIR+gid+"/%s",str) < 1 )
				return notify_fail("识别文件名错误，请报告巫师！\n");
			if( !GROUP_D->create_room(
				base_name(env)+".c",
				env->query("short"),env->query("long"),
				replace_string(GROUP_D->get_room_exits(env),"`"+arg+"`:__DIR__`"+str+".c`,",""),GROUP_D->get_room_objects(env),
				(string)env->query("group1"),env->query("outdoors")?env->query("outdoors"):env->query("indoors"),
				env->query("outdoors")?0:1, env->query("have_board") )
			)
				return notify_fail("写文件失败，请报告巫师！\n");

			str = "你把"+ob->query("short")+"完全的摧毁了。\n";
			log_file("group/destory", 
				sprintf("%8s%-10s 摧毁 %-8s的%s。", 
					me->name(1), 
					"("+me->query("id")+")", 
					gid,
					ob->query("short"),
				), me
			);
			rm(base_name(ob)+".c");
			GROUP_D->mirror_file( base_name(ob)+".c","","defile" );
			GROUP_D->encrypt_file(base_name(env)+".c");
			destruct(ob);
		}
	}
	else if( objectp(ob=present(arg,env)) ){
		if( env->query("group1") != me->query("group/id") )
			return notify_fail("你只能在自己的帮派里摧毁物品。\n");
		if( !me->query("group/class") > 3 )
			return notify_fail("你的等级不够。\n");

		if( !GROUP_D->create_room(
			base_name(env)+".c",
			env->query("short"),env->query("long"),
			GROUP_D->get_room_exits(env),replace_string(GROUP_D->get_room_objects(env),"`"+base_name(ob)+".c`:1,",""),
			(string)env->query("group1"),env->query("outdoors")?env->query("outdoors"):env->query("indoors"),
			env->query("outdoors")?0:1, env->query("have_board") )
		)
			return notify_fail("写文件失败，请报告巫师！\n");

		GROUP_D->encrypt_file(base_name(env)+".c");
		message_vision("$N手持"+weapon->name()+"砸向"+ob->name()+"......\n",me);
		str = "你把"+ob->name()+"完全的摧毁了。\n";
		log_file("group/destory", 
			sprintf("%8s%-10s 摧毁 %-8s的%s。", 
				me->name(1), 
				"("+me->query("id")+")", 
				gid,
				ob->name()
			), me
		);
		destruct(ob);
	}
	else
		return notify_fail("没有这样东西。\n");

	delay = 10 + random(10);
	if( wizardp(me) && me->query("env/test") ) delay = 2;
	me->start_busy(delay);
	call_out("destory_finish",delay*2-1,me,base_name(env)+".c",str);

	return 1;
}

void destory_finish(object me,string file,string str)
{
	if( !me || !str ) return;
	if( file ) GROUP_D->update_room(file);
	tell_object(me,str);
}
