// reg.c
// created by campsun 2004.2.9
// Modified by Ciwei@SJ


inherit F_CLEAN_UP;

#include <ansi.h>
#include <database.h>
#include <mudlib.h>

object get_player(string arg);

static object author;

int main(object me, string arg)
{
	object ob;
	int vip_day;
	int now_time;
	int vip_time;
	int flag = 0;

	now_time = time();
	
	if( me!=this_player(1) ) return 0;
	
	author=me;
	
	if( !arg || sscanf(arg, "%s %d", arg, vip_day)!=2 )
		return notify_fail("指令格式：regvip <用户ID> <天数>\n");

    	if (!objectp(ob = LOGIN_D->find_body(arg))){
    		flag = 1;
		if(!objectp(ob = get_player(arg))) return notify_fail("没有这个人吧?\n");
	}
        
        //if(!objectp(environment(ob)))
                  //return notify_fail("这个人不知道躲在哪里耶... :-( \n");//有这个必要？

	/*if(ob->query("registered")!=3)
		return notify_fail("对方不是贵宾用户，不能设定贵宾天数。\n");*/
	
	
	if((int)ob->query("vip/vip_time")!=0 && (int)ob->query("vip/vip_time") < now_time)
	{
		ob->delete("vip/vip_time");
		ob->delete("vip/vip_start_time");
		return notify_fail("对方的贵宾已经过期，标记将被删除，请重新充值。\n");
	}
	
	vip_time = vip_day * 24 * 3600;
	
	if(vip_time==0) return notify_fail("你挎起两只脚猛抽自己的脸，高难度哦。\n");
		
	if (!(int)ob->query("vip/vip_start_time"))
	{
		if(vip_time<0)
		{
			return notify_fail("对方并非VIP用户，不需要进行减少VIP有效期的操作。\n");
		}
		ob->set("vip/vip_time",vip_time + now_time);
		ob->set("vip/vip_start_time",now_time);	
		if(!ob->save()) return notify_fail("玩家数据保存失败，注册无效\n");
		//save 失败 无须log
		//message_vision("$N为$n开通了 " +HIY+ vip_day +NOR+ " 天的贵宾，请使用 time 命令进行查看。\n", me, );
		tell_object(me,"你为"+ob->name()+"开通了 " +HIY+ vip_day +NOR+ " 天的贵宾。\n");
		tell_object(ob,me->name()+"为你开通了 " +HIY+ vip_day +NOR+ " 天的贵宾，请使用 time 命令进行查看。\n");
		log_file( "static/VIP", me->query("name")+"("+me->query("id")+") 给 " +
			ob->query("name") + "(" + ob->query("id") + ") 开通了 " + vip_day + " 天贵宾。\n" );
	}
	else
	{
		//vipid  username  regemail  regtimelimit  updatetime  
		//    1 aboy@ln2 xhy009@shujian.com 1087012108 1081396108 
		string dex="增加";
		if(vip_time<0) dex="减少";
				
		//if(vip_time>0) return notify_fail("暂时禁止增加Vip Time的操作。\n");
						
		ob->add("vip/vip_time",vip_time);
		ob->set("vip/vip_start_time",now_time);
		if(!ob->save()) return notify_fail("玩家数据保存失败，注册无效\n");
		//message_vision("$N为$n增加了 " + HIY + vip_day + NOR + " 天的贵宾，请使用 time 命令进行查看。\n", me, ob);
		
		if(!BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,
		"UPDATE mudvips SET regtimelimit = regtimelimit+'"+vip_time+"' WHERE username = '"+getuid(ob)+"@"+lower_case(INTERMUD_MUD_NAME)+"' limit 1",
		this_object(),"back"));
		
		tell_object(me,"你为"+ob->name()+dex+"了 " +HIY+ ABS(vip_day) +NOR+ " 天的贵宾。\n");
		//tell_object(ob,me->name()+"为你增加了 " +HIY+ ABS(vip_day) +NOR+ " 天的贵宾，请使用 time 命令进行查看。\n");
		log_file( "static/VIP", me->query("name")+"("+me->query("id")+") 给 " +
			ob->query("name") + "(" + ob->query("id") + ") "+dex+"了 " + ABS(vip_day) + " 天贵宾。\n" );
	}
	if(flag==1) destruct(ob);
	flag = 0;
	return 1;
}

void back()
{
	tell_object(author,"Web数据库操作完成。\n");
}

object get_player(string arg)
{
	object user;	
	
	user = new(USER_OB);
	if (!user) {
		write("现在可能有人正在修改使用者物件的程序，无法进行复制。\n\n");
		return 0;
	}
	seteuid(arg);
	export_uid(user);
	seteuid(getuid());
	if(!user->restore()) {
	destruct(user);
	return 0;}
	user->set_name(user->query("name"), ({ arg}));
	return user;
}

int help(object me)
{
	write(@HELP
指令格式： regvip 人物<id> 天数<day>

用来给玩家设定vip天数。
HELP );
    return 1;
}
