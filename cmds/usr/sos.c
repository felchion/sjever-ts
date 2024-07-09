// This Program is for command "sos"
// Create by keepon at 1999.4.20
// Optimize by Looklove@SJ 2003.10.21
// Modify By LinuX@SJ 2004.3.22

#define SOS_DIR       "/data/sos/"
#define SOS_FILENAME  "sos"
#define WIZ_MAIL "garrison.h@gmail.com"
#define BBS_TNUM 40
#ifndef BBS_ADDR
#define BBS_ADDR "http://bbs.mymud.com"
#endif
#define BBS_OK(x) sprintf(WHT"%s发表到论坛完成。\n"NOR,x)
#define BBS_URL(x) sprintf(CYN"详情请浏览：%s/viewthread.php?tid=%d。\n"NOR,BBS_ADDR,x)

inherit F_SAVE;

#include <command.h>
#include <ansi.h>
#include <mudlib.h>
#include <database.h>

mapping *notes;
int confirm(string yn, object me);
int help(object me);

void create()
{
        seteuid(getuid());
}

string query_save_file()
{
	return SOS_DIR + SOS_FILENAME ;
}

mixed query(string arg)
{
	if (arg && arg=="channel_id")
		return  "求救精灵";
	return 0;
}

static int op;

void post_ok(mixed ret)
{
	//mapping *notes;	
	//if(environment()) tell_room(environment(),BBS_OK("留言"));
	//notes = query("notes");
	if(!notes || !sizeof(notes)) return;	
	if(op != sizeof(notes)-1) return;//have somebody change it
	if(!ret || !sizeof(ret)) return;
	notes[op]["tid"] = ret[0][0];
	//if(environment()) tell_room(environment(),BBS_URL(ret[0][0]));
	save();
}
/*
void re_ok(mixed ret)
{
	//if(environment()) tell_room(environment(),BBS_OK("回复"));
	//if(ret && sizeof(ret)) if(environment()) tell_room(environment(),BBS_URL(ret[0][0]));
	message("wizard",sprintf("回复OK\n"),users());
}
*/

void done_post(object me, mapping note, string text)
{
	int i;
	object where;
	string msg;
	object ob;
	string mail;
	note["msg"] = text;
	i = sizeof(notes);
	where = environment(me);

	for( ; i > 0; i-- )
	if( notes[i-1]["author"] == me->query("id") ) break;
	if( !pointerp(notes) || i > 0 )
		if(!sizeof(notes)) notes = ({ note });
		else
		{
			notes[i-1]["title"] = note["title"];
			notes[i-1]["author"] = note["author"];
			notes[i-1]["msg"] = note["msg"];
			notes[i-1]["time"] = note["time"];
			notes[i-1]["solve"] = "";
			notes[i-1]["solve_id"] = "";
		}
	else notes += ({ note });
	
	op = sizeof(notes)-1;
	
	message("wizard", HIY "【求救】：玩家 "+note["author"]+" 使用"BLINK" SOS "NOR HIY"紧急求救，请在线巫师使用 sos 命令查看。\n" NOR,users());
	
	ob = new(LOGIN_OB);
        ob->set("id", me->query("id"));
        ob->restore();
        mail = ob->query("email");
        destruct(ob);

      	msg = ""+note["title"]+"\n"+
      	note["msg"]+"\n"+
      	"玩家信箱:"+mail+"\n"+
      	ctime(time()) +"\n";
      	
	tell_object(me, WHT "\n求救完毕，你的信息已经被加到巫师通知消息里，并发邮件到主管巫师那里。\n");
	tell_object(me, WHT "你可以在论坛看到你的求助信息，巫师有可能会在论坛帮你解决问题，请留意："+BBS_ADDR+"/forumdisplay.php?fid="+BBS_TNUM+"。\n");
	tell_object(me, WHT "巫师正在处理其他玩家，请等待......\n");
	tell_object(me, WHT "你可以随时用sos read，或者查看您的注册信箱新邮件判断是否被解决。\n" NOR);
	SMTP_D->send_mail(me, WIZ_MAIL,CHINESE_MUD_NAME+"玩家" + note["author"] + "求救信号", msg);
	/*BBS_D->bbs_post(
		BBS_TNUM,
		wizardp(me)?me->query("id"):me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME),
		strip("【玩家问题求解】："+note["title"]),
		"\n\n" + text,0,0,0,0,0 );*/
	BBS_D->bbs_post(
	BBS_TNUM,wizardp(me)?"master":me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME),
	strip("【玩家问题求解】："+note["title"]),
	"\n\n" + text,0,0,0,this_object(),"post_ok");
			
	save();
	return;
}

void list_bug(object me, string arg)
{
	int i;
	string str,s;

	if (sizeof(notes)<1) {
		write("没人发出 sos 求救信号。\n");
		return;
	}
	arg == "all"?s="全部":s="未解决";
	s = sprintf("当前共有("+sizeof(notes)+")条SOS，"+s+"信息列表如下:\n"
	"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	for( i = 0; i<sizeof(notes); i++){
		if( arg=="all" ){
			str = sprintf("[%2d] %-40s%10s [%s] \n",
			i+1, notes[i]["title"], notes[i]["solve_id"]==""?"尚未回复":"By "+notes[i]["solve_id"],
			ctime(notes[i]["time"])[0..15] );
			s = s + str;
		}
		else{
			if(notes[i]["solve_id"]=="") {
				str = sprintf("[%2d] %-40s%10s [%s] \n",
				i+1, notes[i]["title"], "尚未回复",
				ctime(notes[i]["time"])[0..15] );
				s = s + str;
			}
		}
	}
	me->start_more(s);
}

void do_read(object me, int i)
{
	string s;
	if( i > sizeof(notes) || i < 1) {
		wizardp(me)?write("无此编号！\n"):write("目前没有你发出的SOS信息。\n");
		return;
	}

	s = sprintf("(%d) %-46s "HIW"时间："NOR"%s\n"
	"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
	"%s\n"
	"巫师处理回复:（%s）\n"NOR
	HIG"%s\n"NOR,
	i, notes[i-1]["title"], ctime(notes[i-1]["time"])[0..15],
	notes[i-1]["msg"], notes[i-1]["solve_id"]==""?"尚未回复":"By "+notes[i-1]["solve_id"],
	notes[i-1]["solve"] );
	
	if(notes[i-1]["tid"]) s+= sprintf("论坛可能会有更多的解答，%s",BBS_URL(notes[i-1]["tid"]) );
		
	me->start_more(s);
}

int search_id(string id)
{
	int i;
	if( !(i = sizeof(notes)) ) return 0;
	for( ; i > 0; i-- )
	if( notes[i-1]["author"] == id ) break;
	return i;
}

void done_re(object me, int i, string text)
{
	object obj, ob;
	string mail;
	string msg;
	notes[i-1]["solve"] = text;
	notes[i-1]["solve_id"] = me->query("id");
	save();
	if ( obj = LOGIN_D->find_body(notes[i-1]["author"])){
		tell_object(obj, HIG "在线巫师已经回复你的sos，请使用sos read查看！\n");
	}
	ob = new(LOGIN_OB);
        ob->set("id", notes[i-1]["author"]);
        ob->restore();
        mail = ob->query("email");
        destruct(ob);
	
	msg = ""+notes[i-1]["title"]+"\n"+
      	notes[i-1]["msg"]+"\n"+
      	"巫师回复：\n"+
      	text+
      	ctime(time()) +"\n";
      	
        SMTP_D->send_mail(me, mail,CHINESE_MUD_NAME+"管理员对您SOS的回复", msg);
	write("该 sos 信息已标志为解决。\n");
	
	if(notes[i-1]["tid"])
	{
		BBS_D->bbs_post(BBS_TNUM,"master","管理员在游戏中的的回复",text,notes[i-1]["tid"],0,0,0,0);
		BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,sprintf("UPDATE %s SET solve='1' WHERE tid='%d' AND fid='%d'",BBS_THREADS,notes[i-1]["tid"],BBS_TNUM),0,0);		
	}		
	return;
}

int do_re(object me, int i)
{
	if( i > sizeof(notes) || i < 1) {
		write("无此编号。\n");
		return 0;
	}

	if ( notes[i-1]["solve_id"]!="" ) {
		write(notes[i-1]["solve_id"]+"已经标志解决了。\n");
		return 1;
	}
	write("请输入你的处理回复：\n");
	me->edit( (: done_re, me, i :) );
	return 1;
}

int main(object me, string arg)
{
	string s, str,arg1;
	int i,arg2;
	restore();

	if (!arg) {
		if(wizardp(me) && !me->query("env/test")) {
			list_bug(me,"test");
		} else {
			write(HIG"\n阅读自己的sos请使用sos read，每个人只能填写一份SOS信息\n"NOR);
			write(HIG"\n如果已经填写过信息，选择 "HIR"Y"HIG" 将覆盖你以前写过的SOS信息\n"NOR);
			write(HIR "这份求助信息将被认为是公开的，在论坛上可见的，请大家保护自己的隐私。\n对于一些更隐私的问题，可以发送到巫师信箱。\n"NOR);
			write(HIG"\n你确定要写一份SOS信息吗？（Y/N）\n"NOR);
			input_to((: confirm :), me);
		}
		return 1;
	}
	if(wizardp(me)){
		if( sscanf(arg, "re %s", arg1) == 1 ) {
			i = search_id(arg1);
			if( i > 0 ){
				do_re( me, i );
				return 1;
			}
		}
		if( sscanf(arg, "re %d", arg2) == 1 ) {
			do_re( me, arg2 );
			return 1;
		}
		if( sscanf(arg, "del %s", arg1) == 1 ) {
			i = search_id(arg1);
			if( !(i = sizeof(notes)) ) return notify_fail("没人发出 sos 求救信号。\n");
			printf("%s的 sos 求助已被删除！！！\n", notes[i-1]["author"]);
			if( sizeof(notes) == 1 ) notes = ({});
			else if(i==1) notes = notes[1..sizeof(notes)-1];
			else if( i == sizeof(notes) ) notes = notes[0..sizeof(notes)-2];
			else notes = notes = notes[0..i-2] + notes[i..sizeof(notes)-1];
			save();
			return 1;
		}
	}

	if ( sscanf(arg, "%s", s) == 1 ){
	if (!wizardp(me) && arg !="read")
		return help(me);
	switch(arg){
	case "read":
		if( !(i = sizeof(notes)) )
			return notify_fail("还没有人做过 sos 呼叫！\n");
		else {
			i = search_id( me->query("id") );
			do_read(me,i);
		}
		break;

	case "all":
	case "list":
		list_bug(me,"all");
		break;
	case "bug":
		list_bug(me,"test");
		break;
	case "clear":
		if( !(i = sizeof(notes)) ) return notify_fail("没人发出 sos 求救信号。\n");
		for( ; i > 0; i-- )
		if( "" != notes[i-1]["solve_id"] ) {
			printf("%s的 sos 求助已被删除！！！\n", notes[i-1]["author"]);
			if( sizeof(notes) == 1 ) notes = ({});
			else if(i==1) notes = notes[1..sizeof(notes)-1];
			else if( i == sizeof(notes) ) notes = notes[0..sizeof(notes)-2];
			else notes = notes = notes[0..i-2] + notes[i..sizeof(notes)-1];
		}
		save();
		break;
	case "re":
		return notify_fail("格式: sos re <编号或玩家id>\n");
		break;

	default:
		if( sscanf(arg, "%d", i) == 1 )
			do_read( me, i );
		else if( sscanf( arg, "%s", str ) == 1 ) {
			i = search_id(str);
			if( i == 0 ) return notify_fail("无此玩家发出的 sos 求助信息！\n");
			do_read( me, i );
		}
	}
	}
	else return notify_fail("格式不对，具体使用方法请参阅 help sos \n");
	return 1;
}

int confirm(string yn, object me)
{
	mapping note;
	object where;
	string s, mid;
	restore();
	mid = me->query("id");
	wizardp(me)? mid= sprintf("%s%d",mid,random(10)):mid=mid;
	if( upper_case(yn) == "Y" ) {
		tell_object(me, HIM "你正在" HIW MUD_NAME NOR HIM "里辛苦地挖着泥巴，突然发现一只大个的臭虫向你爬来。\n" NOR);
		tell_object(me, HIM "你赶紧向巫师们发出紧急求助信号：ＳＯＳ！！！\n" NOR);
		tell_object(me, HIM "请将出现的情况和系统的提示尽量详细地进行说明，以便巫师修改。\n"NOR);
		tell_object(me, HIR "这份求助信息将被认为是公开的，在论坛上可见的，请大家保护自己的隐私。\n对于一些更隐私的问题，可以发送到巫师信箱。\n"NOR);
		tell_object(me, HIW "请输入此份SOS的标题，请尽可能简扼的说明(长度请保持在五个汉字或者十个字母以上)："NOR);
		where = environment(me);
		if (!where )
			s=sprintf("%s(%s)求助",me->query("name"), mid);
		else
			s=sprintf("%s(%s)在 %s 求助", me->query("name"), mid, (string)where->query("short"));

		note = allocate_mapping(6);
		note["title"] = s;
		note["author"] = mid;
		note["time"] = time();
		note["solve"] = "";
		note["solve_id"] = "";
		input_to("this_title", me,note);
		//me->edit( (: done_post, me, note :) );
	} else {
		write(HIW"您放弃了填写SOS信息。。。。\n"NOR);
		return 1;
	}
}

void this_title(string title,object me,mapping note)
{
	if(!title || !strlen(title) )
	{
		write(HIW"您放弃了填写SOS信息。。。。\n"NOR);
		return;
	}
	if(strlen(title)<10)
	{
		write(HIW"长度不符合规范，请重新输入标题："NOR);
		input_to("this_title", me,note);		
		return;
	}
	note["title"]=title;
	tell_object(me, HIW "现在请您输入此SOS求助的详细内容：\n"NOR);
	me->edit( (: done_post, me, note :) );
}

int help(object me)
{
	if (!wiz_level(me) )
	write(@HELP

指令格式 :
           sos
           sos read
这个指令可以在系统出现bug时来向巫师求助。
输入该命令后，系统将提示你输入对 bug 的描述。玩家在这里应该将 bug
出现的情况和系统的提示尽量详细地进行说明，以便巫师修改该 bug。
每个玩家只能贴一条 sos，新的 sos 将覆盖以前的输入。
可以用 sos read 阅读自己贴出的 sos。
HELP
 );
	else
	write(@HELP
指令格式 :
           sos
           sos bug             显示所有未解决的sos。
	   sos all/list        将所有的 sos 信息进行列表。
           sos <数字或ID>      将显示对应号码或对应玩家的 sos 的详细信息?           sos clear           删除所有标志为已解决的 sos 信息。
           sos del <数字或ID>  删除指定的 sos 信息。
           sos re <数字或ID>   对 sos 信息进行回复。

这个指令可以辅助巫师处理玩家提出的 sos 列表。
HELP
 );
    return 1;
}