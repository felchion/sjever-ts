
//使用数据库的自动更新文件 by Ciwei@SJ 2004 4 27
//本文件只能手动更新

#include <ansi.h>
#include <mudlib.h>

#define JIAMI_C "/cmds/common/jiami.c"
#define EV_DATA 1000
/*每次上传下载的最大字符*/
#define FILE_DB_NAME "discuz"
/*Web服务器中存放文件数据表的数据库名称*/
#define TICK_TIME 60
/*定时更新时间*/
#define DB_DATA "mfile_data"
#define DB_FLAG "mfile_flag"

string* for_bid = ({
	"а","б","в","г","д","е","ж","з","и","й",
	"к","л","м","н","о","п","р","с","т","у",
	"ф","х","ц","ч","ш","щ","ъ","ы","э","ь",
	"ю","я","ё",});
///////////////////////////
int stas = 0;
string name="";
string file_data;
string reascon="";
string id="";
int u_part=0;

//"LN","CQ",
string* sites = ({"CQ","LN",});
/*运行站*/
int now_site = 0;

string *allow = ({
	"linux",
	"ciwei",
	"mxzhao",
});

void create()
{
	seteuid(getuid());
	// 减轻网络负担，只需要开发站负责清理数据库 & 运行站才会下载更新的文件
	if (member_array(INTERMUD_MUD_NAME, sites ) != -1)
		call_out("down_file",TICK_TIME);
	if (member_array(INTERMUD_MUD_NAME, sites ) == -1)
		call_out("delete_file",TICK_TIME/2);
}

string query(string arg)
{
	if (arg == "channel_id")
        	return "传送精灵";
}

void debug(string arg)
{
	object ob = find_player("ciwei");
	arg = HIW+arg+NOR;	
	if(ob) tell_object(ob,arg);
}

int main(object me, string filename)
{
	//int w;
	me = this_player(1);
		
	if (member_array(geteuid(me), allow) == -1)
		return notify_fail("超级流氓 "+implode(allow, " ")+" 专用命令。\n");
	
	if(getuid(me)!="ciwei")
		return notify_fail("建议使用newucp 查看help newucp。\n");
	
	if (!filename || sscanf(filename,"%s %s",filename,reascon)!=2 )
		return notify_fail("格式不对，请查看帮助。 \n");
	//必须<30
	
	if(sizeof(reascon)>30 || sizeof(reascon)<10)
		return notify_fail("你的原因必须在10-30个字之间，表达清晰，有可能提供玩家查询。\n");
	seteuid(geteuid(me));
	
	if (member_array(INTERMUD_MUD_NAME, sites ) != -1)
		return notify_fail("该指令只能在开发站使用。\n");
	
	if(stas!=0) return notify_fail("对不起，该指令还没有使用完毕，请稍等候。\n");
	
	filename = resolve_path(me->query("cwd"), filename);

	if ( file_size(filename) < 1)
		return notify_fail("没有 "+filename+" 这个文件，或者禁止拷贝此文件，或者这是一个目录。\n");

	if( !SECURITY_D->valid_write(filename, me, "write_file")
	|| !SECURITY_D->valid_read(filename, me, "read_file") )
		return notify_fail("Sorry，你没有读写 "+filename+" 这个文件的权力。\n");
	
	name = filename;
	
	//这里应该先进行加密处理
	//或者传送到了以后再加密也行 :D		
	//加密	
	/*
	w =  strsrch(name,"/",-1);
	filename = name[0..w]+"temp_"+ name[w+1..<1];
	if(!cp(name,filename)) return notify_fail("暂储文件复制失败。\n");
	
	
	JIAMI_C->main(this_object(),filename);
	
	file_data = read_file(filename);
	
	if(!rm(filename)) write("文件"+filename+"删除失败。\n");
	
	*/
	file_data = read_file(filename);
	
	if(!file_data)
	{
		return notify_fail("文件太大，或者其他异常，请手动更新。\n");
	}	
					
	notify_fail("特殊字符限制，该文件只能手动传送。\n");	
	if(strsrch(file_data,"")!=-1) return 0;
	if(strsrch(file_data,"")!=-1) return 0;
	if(strsrch(file_data,"")!=-1) return 0;
	if(strsrch(file_data,"")!=-1) return 0;
	if(strsrch(file_data,"ё")!=-1) return 0;
	if(strsrch(file_data,"я")!=-1) return 0;
	if(strsrch(file_data,"ю")!=-1) return 0;
	if(strsrch(file_data,"ь")!=-1) return 0;
	if(strsrch(file_data,"э")!=-1) return 0;
	
	file_data = replace_string(file_data, "", "");
	file_data = replace_string(file_data, "\\", "");
	file_data = replace_string(file_data, "\"", "");
	file_data = replace_string(file_data, "'", "");
	file_data = replace_string(file_data, ";", "ё");
	file_data = replace_string(file_data, ",", "я");
	file_data = replace_string(file_data, "&", "ю");
	file_data = replace_string(file_data, "+", "ь");
	file_data = replace_string(file_data, "*", "э");
	stas = 1;
	u_part=0;
	id = getuid(me);
	CHANNEL_D->do_channel(this_object(),"sys","开始传送"+name+"到各个分站，请在传送完毕以后，在各分站检查更新。");
	call_out("update_file",0);
	return 1;
}

void update_file()
{
	string data;
	string sql;
		
	if(file_data==""){
		stas = 3;
		sql = "UPDATE "+DB_DATA+" SET "
			"U_Time='"+time()+"',"
			"Wizard='"+id+"',"
			"Code='"+reascon+"' "
			"WHERE U_Name='" + name+"'";
	}
	else {
		if(sizeof(file_data)>EV_DATA)
		{
			data = file_data[0..EV_DATA-1];
			file_data = file_data[EV_DATA..<1];
		}
		else
		{
			data = file_data;
			file_data = "";	
		}		
		sql = "REPLACE "+DB_DATA+" SET "
			"U_Name='" + name + "',"
			"U_Part='" + u_part + "',"
			"U_Data='"+data+"'";
		u_part++;		
	}
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,sql,this_object(),"readback");
	call_out("failed",300);
}

void readback(mixed ret)
{
	remove_call_out("failed");
	remove_call_out("update_file");
	remove_call_out("update_flag");
	if(stas == 3)
	{
		now_site = 0;
		call_out("update_flag",0);
		return;
	}
	call_out("update_file",0);
}

void update_flag()
{
	string sql;
	
	if(now_site>=sizeof(sites))
	{		
		CHANNEL_D->do_channel(this_object(),"sys","传送"+name+"成功，请到各分站等待检查更新。");
		stas=0;
		return;
	}
	sql = "REPLACE "+DB_FLAG+" SET "
		"U_Name='" + name + "',"
		"U_Site='" + sites[now_site] + "'";		
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,sql,this_object(),"readback2");
	now_site++;
	call_out("failed",300);			
}

void readback2(mixed ret)
{
	remove_call_out("failed");
	remove_call_out("update_file");
	remove_call_out("update_flag");	
	call_out("update_flag",0);
}

void failed()
{
	CHANNEL_D->do_channel(this_object(),"sys","传送"+name+"失败，请检查网络是否可用。");
	//log
	stas = 0;
	name="";
	file_data="";
	remove_call_out("update_file");	
}
/////////////////////////////////////////上传部分结束///////////////////////////////////////
int d_stas = 0;
string d_file="";
string d_data;
int d_part;

void down_file()
{
	if(d_stas!=0){
		//call_out("down_file",TICK_TIME);
		return;
	}
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"SELECT U_Name,U_Data FROM "+DB_FLAG+" WHERE U_Site='"+INTERMUD_MUD_NAME+"'",this_object(),"readback3");
}

void readback3(mixed ret)
{
	if(ret && sizeof(ret) )
	{
		int i;
		for(i=0;i<sizeof(ret);i++)		
			if(ret[i][1]=="0") break;		
		if(i==sizeof(ret))
		{
			call_out("down_file",TICK_TIME);
			return;	
		}
		d_stas = 1;
		d_file = ret[i][0];
		d_part = 0;
		CHANNEL_D->do_channel(this_object(),"sys","正准备从网络中下载"+d_file+"。");		
		call_out("down_the_file",0);
	}
	else call_out("down_file",TICK_TIME);
}

void down_the_file()
{
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"SELECT U_Data,Wizard,Code FROM "+DB_DATA+" WHERE U_Name='"+d_file+"' AND U_Part='"+d_part+"'",this_object(),"readback4");
	call_out("d_failed",300);
}

void readback4(mixed ret)
{	
	remove_call_out("d_failed");
	if(!ret)
	{
		d_stas=0;
		call_out("down_file",TICK_TIME);
		error("数据库错误！！");
		return;
	}	
	if(!sizeof(ret))
	{		
		d_data = replace_string(read_file(d_file), "", "\n");
		//d_data = replace_string(d_data, "\n\n", "\n");
		d_data = replace_string(d_data, "", "");
		d_data = replace_string(d_data, "", "\\");
		d_data = replace_string(d_data, "", "\"");
		d_data = replace_string(d_data, "", "'");
						
		d_data = replace_string(d_data, "ё",";");
		d_data = replace_string(d_data, "я",",");
		d_data = replace_string(d_data, "ю","&");
		d_data = replace_string(d_data, "ь","+");
		d_data = replace_string(d_data, "э","*");
				
		write_file(d_file,d_data,1);
		
		//改在这里加密
		JIAMI_C->main(this_object(),d_file);
		CHANNEL_D->do_channel(this_object(),"sys","下载"+d_file+"完成，请在线工作人员更新相关应用程序。");
		BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"REPLACE "+DB_FLAG+" SET U_Name='" + d_file + "',U_Site='" + INTERMUD_MUD_NAME + "',U_Data='1'",this_object(),"over_back");	
		call_out("d_failed",300);
		return;
	}
	
	if(sizeof(ret)>1)
	{
		d_stas=0;
		call_out("down_file",TICK_TIME);
		error("数据库错误！！");
		return;
	}
			
	if(sizeof(ret)==1)
	{
		string wizard = ret[0][1];
		string be = ret[0][2];
		d_data = ret[0][0];
				
		//add log here	
		if(!d_part) log_file("static/super_ftp",sprintf("由于%-30s,%-8s更新%s",be,wizard,d_file),this_object());
		//add test
		if(!write_file(d_file,d_data,!d_part?1:0))
		{
			CHANNEL_D->do_channel(this_object(),"sys","下载"+d_file+"失败，请在线工作人员安全文件。");
			//change db
			d_stas=0;
			call_out("down_file",TICK_TIME);
			return;
		}
		d_part++;
		call_out("down_the_file",0);
	}
}

void over_back(mixed ret)
{
	d_stas=0;
	remove_call_out("d_failed");
	call_out("down_file",1);
	return;
}

void d_failed()
{
	d_stas=0;
	call_out("down_file",TICK_TIME);
	CHANNEL_D->do_channel(this_object(),"sys","下载"+d_file+"失败，请检查网络是否可用。");
	return;
}
/////////////////////////////////////下载部分结束///////////////////////////////////////////

int delete_busy = 0;
mixed hash;
int delete_num = 0;
string to_delete;

void delete_file()
{
	if(delete_busy!=0)
	{
		//call_out("delete_file",TICK_TIME);
		return;
	}
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"SELECT U_Name,U_Data FROM "+DB_FLAG,this_object(),"delete_back");
}

void check_hash()
{
	mixed tt;
	if(delete_num >= sizeof(hash))
	{
		delete_busy = 0;
		call_out("delete_file",1);
		return;
	}
	tt = hash[delete_num];
	for(int i=0;i<sizeof(tt);i++)	
		if(tt[i][1]!="1")
		{
			delete_num++;
			call_out("check_hash",1);
			return;
		}
	to_delete = copy(tt[0][0]);
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"DELETE FROM "+DB_FLAG+" WHERE U_Name='"+to_delete+"'",this_object(),"delete1");
	call_out("delete_failed",300);
}

void delete1(mixed ret)
{	
	remove_call_out("delete_failed");
	BBS_D->add_Bbs_Up_Map(FILE_DB_NAME,"DELETE FROM "+DB_DATA+" WHERE U_Name='"+to_delete+"'",this_object(),"delete2");
	call_out("delete_failed",300);
}

void delete2(mixed ret)
{	
	remove_call_out("delete_failed");
	delete_num++;
	call_out("check_hash",0);	
}

void delete_failed()
{
	remove_call_out("delete_file");
	remove_call_out("check_hash");
	remove_call_out("delete_back");
	delete_busy = 0;
	call_out("delete_file",TICK_TIME);		
}

void delete_back(mixed ret)
{		
	if(!ret || !sizeof(ret))
	{
		call_out("delete_file",TICK_TIME);
		return;
	}
	hash = unique_array(ret,(:$1[0]:));
	delete_num = 0;
	delete_busy = 1;
	call_out("check_hash",0);
}
//////////////////////清理部分结束//////////////////////////////////////////

void remove(string id)
{
	if( id!="ciwei" && (delete_busy!=0 || stas!=0 || d_stas!=0) )
	{
		log_file("static/super_ftp","文件异常报告：正在上传或者下载或者检查更新的过程中程序被清除，可能涉及到的文件："+name+" and "+d_file,this_object());
		error("你皮在痒啊？");
	}
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