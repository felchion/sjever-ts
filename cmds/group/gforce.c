// gforce.c 强迫执行
// by augx@sj 3/25/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式：
gforce [玩家id] do [命令]
使用此命令可以在自己的帮派中强迫等级比自己低的玩家和NPC执行命令。

gforce [玩家id] do here
使用此命令可以在自己的帮派中命令等级比自己低的玩家到命令人所在的地方。

gforce [保镖id] do save
把保镖的状态存盘，保镖当时所在地点也会成为其重生地点。

gforce [保镖id] do setname [保镖中文名(二到四个汉字)]
  [保镖新英文ID(必须是两个词，每个词长度为2到6个字母)]
例如：gforce guard do setname 保镖 body guard
设置保镖的新名字，每个保镖只能设置一次名字，设置后请存盘。

强制别人执行命令时，可能不能得到完整的返回信息，比如出错信息等，请注意。
HELP );
	return 1;
}


string * exclude_commands = ({
	"passwd","quit","fangqi","abandon","chat","rumor","say","reply","party","sj","irc","to","tell",
	"lian","practice","chat*","rumor*","alias","sj*","vote","team","group","group*","party*","alias","sr","sr*"
});

int main(object me, string arg)
{
	string cmd,cmds,str,ob_file,id1,id2;
	object ob,env;
	int flag;

	return notify_fail("暂时关闭。\n");

	if (!arg || !sscanf(arg, "%s do %s", arg, cmds)) return help(me);
	if ( !(ob = present(arg, environment(me))) ) {
		if( cmds != "here" )
			return notify_fail("没有这个人。\n");
		else if ( !(ob=find_player(arg)) || environment(ob)->query("group1")!=me->query("group/id") )
			return notify_fail("没有这个人或者此人不在帮派区域之内。\n");
	}
	if (ob == me)
		return notify_fail("直接打命令比较快些。\n");
	if( !me->query("group/id")
	  || me->query("group/id") != ob->query("group/id")
	  || me->query("group/class") >= ob->query("group/class") )
		return notify_fail("你只能命令同帮派，且等级比你低的人。\n");
	if( environment(me)->query("group1") != me->query("group/id") )
		return notify_fail("你只在自己的帮会里命令他人。\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	seteuid(getuid());
	if( cmds != "here" ) message_vision("$N命令$n执行命令：「"HIR+cmds+NOR"」。\n", me,ob);

	if( sscanf(cmds,"%s %*s",cmd) < 1 ) cmd = cmds;

	flag = 0;
	switch ( cmd ) {
		case "fangqi":
		case "abandon":
			if( !userp(ob) ) {
				if( sscanf(cmds,"%*s %s",cmd) < 1 )
					return notify_fail("你要"+ob->name()+"放弃什么技能？\n");
				ob->delete_skill(cmd);
				return 1;
			}
			else flag = 1;
			break;
		case "here":
			if( ob->is_busy() || ob->is_fighting() )
				return notify_fail(ob->name()+"正忙着呢。\n");

			tell_room(environment(ob),"$N似乎突然听到了什么声音，急急忙忙的跑了过去。\n",({ ob }));
			tell_object(ob,"你似乎突然听到"+me->name()+"在呼唤你，赶忙跑了过去。\n");
			ob->move(environment(me));
			message_vision("$N大声呼唤$n的名字，$n听到后赶忙跑了过来。\n",me,ob);
			return 1;
		case "save":
			if( !userp(ob) ) {
				if( (ob->query_temp("last_save")+15*60)>time() && !wizardp(me) )
					return notify_fail(ob->name()+"的状态不是刚刚储存过吗？\n");

				log_file( "group/save" , sprintf("save:%s(%s)[%d]:%s(%s)\n",ob->name(),ob->query("id"),ob->skill_count(),me->name(),me->query("id")) );

				if( ob->query("startroom") == base_name(environment(ob)) ) {
					ob->set_temp("last_save",time());
					if( !ob->save() )
						log_file("group/save",sprintf("fail:%s(%s):%s(%s)\n",ob->name(),ob->query("id"),me->name(),me->query("id")));
					tell_object(me,ob->name()+"存储完毕。\n");
					return 1;
				}

				ob_file = replace_string( base_name(ob)+".c" , "/d/group/"+me->query("group/id")+"/", "" );

				if( ob->query("startroom") ) {
					if( !(env=find_object(ob->query("startroom"))) )
						env = load_object(ob->query("startroom"));
					if( env ) {
						if( !GROUP_D->create_room(
							base_name(env)+".c",
							env->query("short"),env->query("long"),
							GROUP_D->get_room_exits(env),replace_string(GROUP_D->get_room_objects(env),"__DIR__`"+ob_file+"`:1,",""),
							env->query("group1"),env->query("group2"),
							env->query("outdoors")?0:1, env->query("have_board") )
						)
							return notify_fail("写文件错误，请报告巫师！\n");

						GROUP_D->encrypt_file(base_name(env)+".c");
						GROUP_D->update_room(base_name(env)+".c");
					}
				}

				env = environment(ob);

				str = GROUP_D->get_room_objects(env);
				if( strsrch( str , "__DIR__`"+ob_file+"`:1," ) == -1 )
					str +=  "__DIR__`"+ob_file+"`:1,";
				if( !GROUP_D->create_room(
					base_name(env)+".c",
					env->query("short"),env->query("long"),
					GROUP_D->get_room_exits(env),str,
					env->query("group1"),env->query("group2"),
					env->query("outdoors")?0:1, env->query("have_board") )
				)
					return notify_fail("写文件错误，请报告巫师！\n");

				tell_object(me,ob->name()+"存储完毕。\n");

				ob->set("startroom","");
				ob_file = base_name(env);

				GROUP_D->encrypt_file(base_name(env)+".c");
				GROUP_D->update_room(base_name(env)+".c");

				ob->set("startroom",ob_file);
				ob->set_temp("last_save",time());
				ob->save();

				return 1;
			}
			break;
		case "setname":
			if( userp(ob) ) break;
			if( sscanf(cmds,"%*s %s %s %s",cmd,id1,id2) < 3 )
				return notify_fail("你要把"+ob->name()+"的名字改成什么？\n");
			if( strlen(cmd)<4 || strlen(cmd)>8 )
				return notify_fail("新名字必须是二到四个汉字。\n");
			if( strlen(id1)<2 || strlen(id1)>6 || strlen(id2)<2 || strlen(id2)>6 )
				return notify_fail("新英文ID长度不符合规定。\n");
			if( ob->query("setname") )
				return notify_fail(ob->name()+"已经设置过名字了。\n");

			ob->set("name",cmd);
			ob->set("id",id1+" "+id2);
			ob->set("id1",id1);
			ob->set("id2",id2);
			ob->set("setname",1);
			ob->set_name( ob->query("name"), ({ ob->query("id"),ob->query("id1"),ob->query("id2"),"group guard","guard" }));
			break;
		default: flag = 1;
	}
	if( flag && member_array(cmd,exclude_commands)>=0 )
		return notify_fail("但是你输入的命令被禁止强制执行！\n");

	if( !ob->force_me(cmds) )
		return notify_fail("你命令"+ob->name()+"执行命令失败。\n");
	return 1;
}
