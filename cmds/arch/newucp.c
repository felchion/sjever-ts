
//跑得快 v 1.0 by Ciwei@SJ 2004 5 1

#include <ansi.h>
#include <mudlib.h>
#include <database.h>

#define FILED "/adm/daemons/filed"

string* sites = ({"CQ","LN",});

string *allow = ({
	  "linux",
  	"yeju",
	//"mxzhao",
	//"yuj",
	//"mychat",//linux add
	//"tangfeng",//linux add
});

public string query_version(object ob)
{
	if(ob==find_object(BBS_D)) return "1.0";
}

string query(string arg)
{
	if (arg == "channel_id")
        	return "传送精灵";
}

int main(object me, string filename)
{
	//int w;
	string reascon;
	int visi = 0;
	
	me = this_player(1);
	if (member_array(geteuid(me), allow) == -1)
		return notify_fail("超级流氓 "+implode(allow, " ")+" 专用命令。\n");
	
	if (!filename || sscanf(filename,"%s %s",filename,reascon)!=2 )
		return notify_fail("格式不对，请查看帮助。 \n");	
	
	sscanf(reascon,"%s %d",reascon,visi);
					
	if(sizeof(reascon)<8)
		return notify_fail("你的原因必须在10-30个字之间，表达清晰，有可能提供玩家查询。\n");
	seteuid(geteuid(me));
	
	if (member_array(INTERMUD_MUD_NAME, sites ) != -1)
		return notify_fail("该指令只能在开发站使用。\n");
		
	filename = resolve_path(me->query("cwd"), filename);

	if ( file_size(filename) < 1)
		return notify_fail("没有 "+filename+" 这个文件，或者禁止拷贝此文件，或者这是一个目录。\n");

	if( !SECURITY_D->valid_write(filename, me, "write_file")
	|| !SECURITY_D->valid_read(filename, me, "read_file") )
		return notify_fail("Sorry，你没有读写 "+filename+" 这个文件的权力。\n");
	
	if(!read_file(filename))
	{
		return notify_fail("文件太大，或者其他异常，请手动更新。\n");
	}
	FILED->add_file(filename,getuid(me),reascon);	
	//文件名 更新原因 时间 更新人
		
        if( visi ) write("不更新记录。\n");
	else
	{
		CHANNEL_D->do_channel(this_object(),"sys","开始传送"+filename+"到各个分站，请在传送完毕以后，在各分站检查更新。");
		if(!BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,"INSERT INTO updatelog( id , filename , reason , username , dateline ) VALUES ('','"+filename+"','"+reascon+"','"+getuid(me)+"',"+time()+")" ,0,0))
		tell_object(me,"更新论坛文件传送记录失败！！！！！请与论坛管理员联系！\n");
	}
	return 1;
}

int help(object me)
{
	write("
从开发站拷贝文件到运行站
指令格式: ucp <文件名> <原因>

注意：超级流氓 "+implode(allow, " ")+" 专用，
ucp 以后请 update 运行站相应程序，并在线观察至少一小时。
如果对自己将拷贝的文件没有足够的把握，切勿使用本命令！！！
如果没有足够的时间观察，切勿使用本命令！！！
如果发现意外，请立即请所有人吃饭。

简单的说明下更新原因。
"
	);
	return 1;
}
