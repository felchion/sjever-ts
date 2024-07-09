// build.c
// by augx@sj 3/19/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
build [方向]
方向为以下方向之一：
east,west,north,south,southeast,southwest,northeast,northwest,up,down
此命令用来建造新的道路，只有在设置有帮派建造标记的地方才能新建自己的道路。
建造道路需要石料(shi liao)。

build room
此命令用来在建造好的道路上，修建房屋。
建造房屋需要木料(mu liao)。

HELP
	);
	return 1;
}

int main(object me, string arg)
{
	object material, env = environment(me);
	string env_path, dir, sdir, str, str2;
	int i,delay;
	mixed * sv;

	if( !arg ) return help(me);
	if( !env || !(env_path=(base_name(env)+".c")) || env_path[0..2]!="/d/" )
		return notify_fail("在你所处的环境里无法创建房屋或道路。\n");
	if( !me->query("group/class") )
		return notify_fail("你还没有创建自己的帮会，建造道路又有何用？\n");
	if( me->query("group/class") > 2 )
		return notify_fail("你的等级不够，不能修建道路和房屋。\n");
	if( me->is_busy() )
		return notify_fail("你正忙着呢！\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	if(arg!="room") {
		switch (arg) {
			case "east":		sdir = "东";	dir = "west"; 	break;
			case "north":		sdir = "北";	dir = "south";	break;
			case "west":		sdir = "西";	dir = "east";	break;
			case "south":		sdir = "南";	dir = "north";	break;
			case "southeast":	sdir = "东南";	dir = "northwest";break;
			case "southwest":	sdir = "西南";	dir = "northeast";break;
			case "northwest":	sdir = "西北";	dir = "southeast";break;
			case "northeast":	sdir = "东北";	dir = "southwest";break;
			case "up":		sdir = "上";	dir = "down";	break;
			case "down":		sdir = "下";	dir = "up";	break;
			default: return notify_fail("你要往哪个方向修建道路？\n");
		}

		if( !(material=present("shi liao",me)) || !material->query("group") )
			return notify_fail("你没有石料，如何修建道路？\n");
		if( stringp(env->query("exits/"+arg)) )
			return notify_fail("那个方向已经有出口了。\n");

		if( strlen(env_path)<9
		 || env_path[0..8]!="/d/group/"
		 || (strlen(env_path)>15 && env_path[0..14]=="/d/group/entry/") ) {
			if( file_size("/d/group/"+me->query("group/id")) == -2 )
				return notify_fail("你已经在别的地方建立了帮派的总舵。\n");
			if( !env->query("group") )
				return notify_fail("你不能在这里创建帮派。\n");
			if( mapp( env->query("exits") ) ) {
				sv = values( env->query("exits") );
				for(i=0;i<sizeof(sv);i++)
					if( strlen(sv[i])>9 && sv[i][0..8]=="/d/group/" )
						return notify_fail("已经有人在这里创建帮派了。\n");
			}
			if( !GROUP_D->get_entry(env_path[0..<3]) )
				return notify_fail( GROUP_D->get_last_error() );

			if( !mkdir("/d/group/"+me->query("group/id")) )
				return notify_fail("创建目录失败！请报告巫师。\n");
			if( !mkdir("/d/group/"+me->query("group/id")+"/npc") )
				return notify_fail("创建目录失败！请报告巫师。\n");

			GROUP_D->set_entry(me->query("group/id"),env_path[0..<3]);
			//cp(env_path,env_path[0..<3]+".origin");

			env_path = "`" + env_path;
		}
		else {
			env_path = replace_string(env_path,"/d/group/"+me->query("group/id")+"/","__DIR__`");
		}

		if( !GROUP_D->create_room(
			str = sprintf("/d/group/%s/%d.c",me->query("group/id"),time()),
			"石路","这是一条刚刚建造好，还没有粉刷的石路。\n",
			sprintf("`%s`:%s`,",dir,env_path),"",
			me->query("group/id"),GROUP_D->get_group_name(me->query("group/id")),
			0, env->query("have_board") )
		)
			return notify_fail("写文件失败，请报告巫师！\n");

		if( env_path[0] == '`' )
			str2 = "`" + str;
		else
			str2 = replace_string(str,"/d/group/"+me->query("group/id")+"/","__DIR__`");
		if( !GROUP_D->create_room(
			base_name(env)+".c",
			env->query("short"),env->query("long"),
			GROUP_D->get_room_exits(env)+sprintf("`%s`:%s`,",arg,str2),GROUP_D->get_room_objects(env),
			(string)env->query("group1"),env->query("outdoors")?env->query("outdoors"):env->query("indoors"),
			env->query("outdoors")?0:1, env->query("have_board") )
		)
			return notify_fail("写文件失败，请报告巫师！\n");

		message_vision("$N开始用"+material->name()+"，往"+sdir+"方修建道路……\n",me);
		destruct(material);
		delay = 10;
		if( wizardp(me) && me->query("env/test") ) delay = 2;
		me->start_busy(delay);

		GROUP_D->encrypt_file(str);
		GROUP_D->encrypt_file(base_name(env)+".c");
		call_out("make_finish",delay*2-1,me,0,str,base_name(env)+".c");
	}
	else {
		if( !(material=present("mu liao",me)) || !material->query("group") )
			return notify_fail("你没有木料，如何建造房屋啊。\n");
		if( env->query("group1") != me->query("group/id") )
			return notify_fail("你只能在自己的帮派里建造房屋。\n");
		if( env->query("indoors") )
			return notify_fail("这里已经建有房屋了。\n");

		if( !GROUP_D->create_room(
			env_path,
			"房间","这是一座刚刚建造好，还没有粉刷的房屋。\n",
			GROUP_D->get_room_exits(env),GROUP_D->get_room_objects(env),
			me->query("group/id"),GROUP_D->get_group_name(me->query("group/id")),
			1, env->query("have_board") )
		)
			return notify_fail("写文件失败，请报告巫师！\n");

		message_vision("$N开始用"+material->name()+"建造房屋……\n",me);
		destruct(material);
		delay = 10;
		if( wizardp(me) && me->query("env/test") ) delay = 2;
		me->start_busy(delay);

		GROUP_D->encrypt_file(env_path);
		call_out("make_finish",delay*2-1,me,1,env_path,"");
	}

	return 1;
}

void make_finish(object me,int room,string file1,string file2)
{
	if( !me ) return;
	if( !room )
		tell_object(me,"你忙碌了半天，总算把道路修建好了。\n");
	else
		tell_object(me,"你忙碌了半天，总算建造起一座房屋。\n");

	if( file1 && strlen(file1)>3 ) GROUP_D->update_room(file1);
	if( file2 && strlen(file2)>3 ) GROUP_D->update_room(file2);
}
