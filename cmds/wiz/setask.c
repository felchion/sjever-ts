//setask.c 
//for set ask
// /cmds/wiz/setask.c
// 希望cp 到imm下 不然偶就没得用了 

#define TONG "/clone/npc/tong"
object get_player(string arg);
object get_tong();
int help(object me);
void update_tong();

int main(object me, string arg)
{
	mapping ask;
	mapping* answer;
	string* top;
	string same = "";
	object tong;
	int i;
	
	if(!arg) return help(me);
	if (wiz_level(me) < 3 && getuid(me)!="ciwei" )
		return notify_fail("只有 wizard 等级以上巫师才可以使用此指令。\n");
	if(!objectp(tong = get_tong())) return notify_fail("万事通尚未复制。无法通讯。\n");
	ask = tong->query_ask();
	if(sscanf( arg,"delete %s",arg) == 1  )
	{
		if(mapp(ask) && !mapp(ask[arg])) return notify_fail("没有此ask。\n");
		if( tong->delete_ask(arg))
		{
			write("删除OK。\n");
			update_tong();
			return 1;
		}
		return notify_fail("修改失败。\n");
	}	
	if(mapp(ask) && mapp(ask[arg]) && stringp(ask[arg]["word"]))
	{
		write(ask[arg]["word"]+"\n");
		answer = values(ask);
		top = keys(ask);
		for(i=0;i<sizeof(answer);i++)
		{
			if( answer[i]["word"]==ask[arg]["word"]) same +=","+top[i];
		}
		if(same != "" && same != (","+arg)) write("存在与该ask相同回答"+same+"  请最好一并修改。\n");
		write("ask "+arg+" 的内容已经存在，要替换么？[Y\\N]\n");
		input_to("config_set",me,arg,tong);
		return 1;
	}
	write("输入回答的内容(输入q 取消)：\n");
	input_to("get_msg_select", me,arg,tong);
	return 1;
}
void config_set(string get,object me,string arg,object tong)
{
	if( get == "Y" || get =="y" || get[0]=='y' || get[0]=='Y' ) 
	{
		write("输入回答的内容：\n");
		input_to("get_msg_select", me,arg,tong);
		return;
	}
	else {
	write("放弃修改。\n");
	return;
	}
}
void get_msg_select(string get,object me,string arg,object tong)
{
	mapping def = allocate_mapping(3);
	if(!stringp(get) || get == "Q" || get =="q" || get =="" ) {
	write("放弃修改。\n");
	return;
	}
	def["word"]=get;
	def["semote"]="";
	def["updated"]="";
	write("输入回答后执行的semote(直接回车 表示不做任何semote):\n");
	input_to("get_semote_select", me,arg,def,tong);
	return;	
}
void get_semote_select(string get,object me,string arg,mapping def,object tong)
{
	def["semote"]=get;
	def["updated"]=getuid(me);
	if(!stringp(get)) get = "";
	if (!tong->set_ask(arg,def))
	{
	write("修改失败。\n");
	return;
	}
	write("修改完成。\n");
	update_tong();
	return;	
}
object get_player(string arg)
{
	object user;
	string msg;
	
	user = new(USER_OB);
	seteuid(arg);
	export_uid(user);
	user->set_name("", ({ arg }));
	if (!user->restore())
        { destruct(user); return 0;};
	msg = user->name();
	user->create();
	seteuid("MudOS");
	export_uid(user);
	user->set_name(msg, ({ arg }));
	user->setup();
	return user;
}
object get_tong()
{
	object* obs;
		
	obs = children(TONG);
	if(sizeof(obs)<=0) return 0;
	return obs[0];
}
void update_tong()
{
	object* obs;
	int i;
		
	obs = children(TONG);
	if(sizeof(obs)<=0) return;
	for(i=0;i>sizeof(obs);i++)
	{
		if(objectp(obs[i])) obs[i]->restore();
	}
	return;
}

int help(object me)
{
	write(@HELP
指令格式：setask xxx

这条命令是用来设置万事通自动回答的。

如果需要换行 用 textn
...
HELP
	);
	return 1;
}


