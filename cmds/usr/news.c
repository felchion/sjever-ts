//BBB 09/25/1999
//让玩家不会漏过任何重要新闻，也不要长途跋涉去看巫师公告版。
// Modify by snowman@SJ for new_list when login.
// Modified By Numa@SJ 2000.3.31 For write all to player
// Modified By bbb@SJ 2000.11.03 For Mysql
// Modified by snowman@SJ 19/12/2000 for faster speed.

// Modified by mxzhao 2004/02/18

inherit F_SAVE;

#include <ansi.h>
#include <net/dns.h>
#include <mudlib.h>
#include <database.h>

#define Data_Base_Online

#define NEWS_DIR       "/data/"
#define NEWS_FILENAME  "news"
#define NEWS_TITLE  "\n━━━━━━━━━━━━━━ "HIY"书剑网络游戏新闻列表"NOR" ━━━━━━━━━━━━━━\n"
#define NEWS_END  "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
#define NEWS_P HIY"新闻系统提示："NOR
#define BBS_URL(x) sprintf(CYN"关于本新闻的讨论请浏览：%s/viewthread.php?tid=%d。\n"NOR,BBS_ADDR,x)

static object ob = 0;
static int status=0;
static string msg,subject;

mapping *notes;

public string query_version(object ob)
{
	if(ob==find_object(BBS_D)) return "1.0";
}

string query_save_file()
{
	return NEWS_DIR + NEWS_FILENAME ;
}

void tell_me(string msg)
{
	if (objectp(ob)) tell_object(ob, msg);
}
//这个函数用在和新闻系统连接那里，感觉很乱
string c_time(int t)
{
	string ts = ctime(t);
	string str = ts[8..9];
	if (str == " 1" || str == " 2" || str == " 3" || str == " 4"
	|| str == " 5" || str == " 6" || str == " 7" || str == " 8" || str == " 9")

	str = "0" + ts[8..9];
	ts = ts[0..2]+ts[19..23]+"-"+ts[4..6]+"-"+str+ts[10..18];

	ts = replace_string(ts, ":", "");
	ts = replace_string(ts, " ", "");
	ts = replace_string(ts, "Sun", "");
	ts = replace_string(ts, "Mon", "");
	ts = replace_string(ts, "Tue", "");
	ts = replace_string(ts, "Wed", "");
	ts = replace_string(ts, "Thu", "");
	ts = replace_string(ts, "Fri", "");
	ts = replace_string(ts, "Sat", "");
	ts = replace_string(ts, "Jan", "01");
	ts = replace_string(ts, "Feb", "02");
	ts = replace_string(ts, "Mar", "03");
	ts = replace_string(ts, "Apr", "04");
	ts = replace_string(ts, "May", "05");
	ts = replace_string(ts, "Jun", "06");
	ts = replace_string(ts, "Jul", "07");
	ts = replace_string(ts, "Aug", "08");
	ts = replace_string(ts, "Sep", "09");
	ts = replace_string(ts, "Oct", "10");
	ts = replace_string(ts, "Nov", "11");
	ts = replace_string(ts, "Dec", "12");
	return ts;
}

static int op;

void post_ok(mixed ret)
{	
	if(!ret || !sizeof(ret)) return;
	if(!notes || !sizeof(notes)) return;
	if(op != sizeof(notes)-1) return;//have somebody change it
	notes[op]["tid"] = ret[0][0];
	save();
}

void done_post(object me,mapping note, string text)
{
	int i;
	note["msg"] = text;
	i = sizeof(notes);
	if( !pointerp(notes) || !sizeof(notes) )
		notes = ({ note });
	else
		notes += ({ note });
	note["time"] = time();
	if(userp(me)) tell_object(me, NEWS_P + HIM "新闻发布完毕。\n"NOR);
	message("system",HIW"\n\n【新闻】发布书剑游戏新闻，请用 news 指令查看，或访问书剑·永恒主页 www.sjever.net 。\n\n"NOR, users());
	save();
  dbquery(sprintf("replace news set title='%s',msg='%s',time=%d,poster='%s',author='%s'",note["title"],note["msg"],note["time"],note["poster"],note["author"]));

//	if(me&&userp(me))
//	{
//#ifdef Data_Base_Online
//		string msg = ( sprintf("[%d] %-=31s %18s (%s)\n" + NEWS_END,sizeof(notes),note["title"], " ",ctime(note["time"])[0..15]) + note["msg"] );
//		string subject = ( sprintf("%s",note["title"]) );
//		op = sizeof(notes)-1;
//		if(BBS_D->bbs_post(10,"system","【書劍新聞】"+note["title"],text,0,0,0,this_object(),"post_ok" ) )
//			tell_object(me,NEWS_P +"news  发送到论坛帖子完毕。\n");
//		else tell_object(me,NEWS_P +"news  发送到论坛帖子失败。\n");
//				
//		if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "INSERT INTO announcements (author, subject, starttime, endtime, message) VALUES ('master', '"+subject+"', '"+time()+"', '0', '"+msg+"')"))
//			tell_object(me,NEWS_P +"news  发送到论坛公告完毕。\n");
//		else
//			tell_object(me,NEWS_P +"news  发送到论坛公告失败。\n");
//		if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,"INSERT INTO izz_text_soft (soft_name, soft_keyword, soft_download_url, soft_is_commend,soft_grade, soft_type, soft_note, soft_author, soft_relation_url,soft_category_id, soft_class_id, soft_img, soft_byuser, soft_user_id,soft_size, soft_date, soft_hits_day, soft_display) VALUES ( '"+subject+"', '新闻', 'Wizard', '0', '3', '原创', '"+msg+"', '书剑官方站', 'http://www.mymud.com', '10','51', 'N/A', 'Linux', '17', 'N/A', '"+Ctime(note["time"])+"', '20040127', '1' )"))
//			tell_object(me,NEWS_P +"news  发送到主页完毕。\n");
//		else tell_object(me,NEWS_P +"news  发送到主页失败。\n");		
//#endif
//		//send to others
//		"/adm/daemons/network/services/newsmsg"->send_msg(note["title"],note["author"],note["poster"],text);
//	}
	return;
}

void list_news(object me, int login)
{
	int i = 0, ulogin, j = 0;
	string str;

	if (sizeof(notes) < 1)
		write("没有任何新闻。\n");
	else {
		ulogin = me->query("last_news");
		str = NEWS_END;
		foreach (mapping item in notes) {
			i++;
			if (ulogin < item["time"]) {
				if (j++ < 15)
					str = sprintf("%3d) %-=40s %18s (%s)\n%s",
						i, item["title"],
						wizardp(me)?item["poster"]+"("+capitalize(item["author"])+")":"",
						c_time(item["time"])[0..9], str
					);
			}
		}
		if (!j)
			write(NEWS_P+"目前没有新发布的新闻。\n");
		else {
			if (login && j > 5)
				;
			else
				write(NEWS_TITLE + str);
			write(NEWS_P+"目前共有 "+ i +" 条新闻、"+ j +" 条未读。\n");
			write("请用 news 阅读标题，或者 news new|next|编号 阅读详细内容。\n");
		}
	}
}

void up_news()
{
	int i = 0;
	if (sizeof(notes) < 1)
		write("没有任何新闻。\n");
	else {
		foreach (mapping item in notes) {
      i++;
      dbquery(sprintf("replace news set title='%s',msg='%s',time=%d,poster='%s',author='%s'",item["title"],item["msg"],item["time"],item["poster"],item["author"]));
		}
	}
}

void down_news()
{
	mixed ret;
	mapping note;
	int i,last_time;

	if(sizeof(notes)) {
	  last_time=notes[sizeof(notes)-1]["time"];
	  ret = dbquery(sprintf("select * from news where time>%d",last_time));
  }
  else {
  	ret = dbquery("select * from news");
  }	
	if (!sizeof(ret))
		write("没有任何新闻。\n");
	else { 
	  for(i=0;i<sizeof(ret);i++){
			if(arrayp(ret[i])) {
				note = allocate_mapping(5);
				note["time"]    = ret[i][0];
				note["title"]   = ret[i][1];
				note["msg"]     = ret[i][2];
				note["poster"]  = ret[i][3];
				note["author"]  = ret[i][4];
		  	notes += ({ note });
	    }
		}
		message("system",HIW"\n\n【新闻】发布书剑游戏新闻，请用 news 指令查看，或访问书剑·永恒主页 www.sjever.net 。\n\n"NOR, users());
		save();
	}	
}

void list_allnews(object me)
{
	int i;
	string str;

	i = sizeof(notes);
	if ( i < 1) {
		write(NEWS_P +"没有任何新闻。\n");
		return;
	}
	str = NEWS_TITLE;
	while( i-- )
		str += sprintf("%3d) %-=40s %18s (%s) \n",
			i+1, notes[i]["title"], wizardp(me)?notes[i]["poster"]+"("+capitalize(notes[i]["author"])+")":"",
			c_time(notes[i]["time"])[0..9] );
	str += NEWS_END;
	me->start_more(str);
}

void create()
{
	seteuid(getuid());
	restore();
	down_news();
	call_out("create", 900);
}
/*这是供Ask调用的 LinuX@SJ*/
string read_last_news()
{
	int size = sizeof(notes);
	int num;
	string str;
	if (size < 10) return "最近没有听到过什么新闻。";
	num = size - 1 - random(10);
	if (strlen(notes[num]["msg"]) > 400) return "最近没有听到过什么大新闻。";
	str = "\n┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n"
	+ HIY+notes[num]["title"] + NOR+"\n"+ notes[num]["msg"] + "┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n";			
	return str;
	
}

int main(object me, string arg)
{
	mapping note;
	string s,arg1;
	int num = -1;

/*#ifdef Data_Base_Online
	//int mysql,rows;
	string forum;
#endif
*/
	if (!arg) {
		list_news(me, 0);
		return 1;
	}

	if (sscanf(arg, "%s", s) == 1) {
		if ( s == "all" ) {
			list_allnews(me);
			return 1;
		}
		if ( s == "up" ) {
			if( wiz_level(me)<3 )
				return notify_fail(NEWS_P +"只有巫师进行此操作。 \n");
			up_news();
			return 1;
		}
		if ( s == "down" ) {
			if( wiz_level(me)<3 )
				return notify_fail(NEWS_P +"只有巫师进行此操作。 \n");
			down_news();
			return 1;
		}
		if ( s == "post" ){
			if( wiz_level(me)<3 )
				return notify_fail(NEWS_P +"只有巫师才可以发布消息。 \n");
			else 	return notify_fail("使用方法： news post <主题> \n");
		}
		if ( s == "delete" ){
			if( wiz_level(me)<3 )
				return notify_fail(NEWS_P +"只有巫师才可以删除消息。 \n");
			else	return notify_fail("使用方法： news delete <编号> \n");
		}
	}

	if (sscanf(arg, "post %s", arg1) == 1) {
		if ( wiz_level(me)<3 )
			return notify_fail(NEWS_P +"只有巫师才可以发布消息。 \n");
		if ( strwidth(arg1)< 8 || strwidth(arg1)>40 )
			return notify_fail(NEWS_P +"请简明扼要地说明主题，主题字数必须在4-20之间，禁止使用口头语言。 \n");
		else note = allocate_mapping(5);
		note["title"] = arg1;
		note["author"] = getuid(me);
		note["poster"] = me->query("name");
		me->edit( (: done_post, me, note :) );
		return 1;
	}

	if (sscanf(arg, "delete %d", num)== 1) {
		if ( wiz_level(me)<3 )
			return notify_fail(NEWS_P +"只有巫师才可以删除消息。 \n");
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail(NEWS_P +"没有这个编号的新闻。\n");
		else num--;
		notes = notes[0..num-1] + notes[num+1..<1];
		save();
		write(NEWS_P +"删除第 " + (num+1) + " 号新闻....Ok。如果此新闻被发布过主页，请去主页删除。\n");
		return 1;
	}

	if (arg == "new" || arg == "next") {
		int ulogin = me->query("last_news");

		num = sizeof(notes);
		while (num--)
			if (ulogin >= notes[num]["time"])
				break;
		num+=2;
	}

	if (num+1 || sscanf(arg, "%d", num) == 1) {
		int size = sizeof(notes);
		string bbs_msg = "";
		
		
				
		if (!size)
			return notify_fail(NEWS_P +"没有任何新闻。\n");
		if (num < 1 || num > size)
			return notify_fail(NEWS_P +"没有这个编号的新闻。\n");
		num--;
		
		if(notes[num]["tid"]) bbs_msg = BBS_URL(notes[num]["tid"]);

		me->start_more(sprintf("%d) %-=43s %8s(%s)(%s)\n"+NEWS_END,
			num + 1, notes[num]["title"],
			wizardp(me)?notes[num]["poster"]:"",
			wizardp(me)?notes[num]["author"]:"News",
			ctime(notes[num]["time"])[0..15]) + notes[num]["msg"] + bbs_msg +"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━[ 本 篇 完 ]"
		);
		if (me->query("last_news") < notes[num]["time"])
			me->set("last_news", notes[num]["time"]);
		return 1;
	}

	if ( arg == "write all" ) {
		if (!wizardp(me))
			return notify_fail("这是巫师命令。\n");
		if( !pointerp(notes) || !sizeof(notes) )
			return notify_fail(NEWS_P +"没有任何新闻。\n");
		msg = NEWS_TITLE;
		for (num=0;num<sizeof(notes);num++) {
			msg += sprintf(HIW"%d"NOR") %-=43s %8s(%s)(%s)\n"+NEWS_END,num + 1, notes[num]["title"],
				wizardp(me)?notes[num]["poster"]:"",wizardp(me)?notes[num]["author"]:"",
				ctime(notes[num]["time"])[0..15]) + notes[num]["msg"] + "\n\n";
		}
		me->start_more(msg);
		return 1;
	}
}

int help(object me)
{
	if (!wiz_level(me) )
	write(@HELP
指令格式 :
news		查看最近十五条未读的新闻列表。
news all	查看全部新闻列表。
news new|next	查看最近一条未读新闻的详细内容。
news <编号>	查看此编号新闻的详细内容。

发布内容主要为：系统公告、程序改动、武林大事。
HELP);

	else
	write(@HELP
指令格式 :
news				查看最近十五条未读的新闻列表。
news all			查看全部新闻列表。
news new|next			查看最近一条未读新闻的详细内容。
news <编号>			查看此编号新闻的详细内容。
news post <主题>		发布新闻，wizard 级别以上巫师专用
news delete <编号>		删除新闻，wizard 级别以上巫师专用
news write all			列出全部 news 的详细内容
news send <编号> <论坛ID>	发送新闻到主页，可以选择不同的讨论区发送

论坛ID如下：系统公告(info) 少林(shaolin)  武当(wudang)
华山(huashan) 桃花(taohua) 铁掌(tiezhang) 神龙(shenlong)
星宿(xingxiu) 峨嵋(emei)   嵩山(songshan) 雪山(xueshan)
古墓(gumu)    慕容(murong) 明教(mingjiao) 丐帮(gaibang) 天龙(tianlong)
分站论坛分别为：ln sh qhd zj hb sd yt dd gz cdedu

发送举例：news send 122 info

注意：  1、发布内容主要为：系统公告、程序改动、武林大事。
	2、未经确实的消息严禁发布，虚假新闻立即删除。
	3、重大程序变动，如可能涉及玩家，必须用此发布通告，且以此为准。
	4、新闻发布涉及全局的，必须发布在info，针对门派的发两次到info和门派区,针对分站的发两次到info和分站区
	5、注意发布时主题明确，不要含糊不清，不要使用口头语言。
HELP);
	return 1;
}
