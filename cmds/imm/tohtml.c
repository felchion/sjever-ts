// /u/look/workroom.c include Exp sort and online player
// And write to /log/html/top10.html. Link to /home/httpd/html/top10.html
// And write to /log/html/onlineuser.html. Link to /home/httpd/html/onlineuser.html
// And write to /log/html/party.html. Link to /home/httpd/html/party.html
// And write to /log/html/count24.html. Link to /home/httpd/html/count24.html
// Write By Look@SJ 19990906
// Modify by LinuX@SJ 2004-4-1

inherit F_DBASE;

#include <database.h>
#include <ansi.h>
#include <mudlib.h>
#include <localtime.h>

int sort_user(object,object);
int sort(object obj1, object obj2);
int filldata(object obj1);
int writefile();
string remove_ansi(string str);

static object* list;
static int now_num;
static int user_num;
static int count_user;
static string fnameb;
static string fnamea;
static int ppl_cnt;

void create()
{
	seteuid(getuid());
	set("channel_id", "传送精灵");
}

int main(object me, string arg)
{
	CHANNEL_D->do_channel( this_object(), "sys","在线玩家数据更新到WEB启动。");
	remove_call_out("writefile");
	call_out("writefile", 0);
        return 1;

}

int clean_up()
{
    return 0;
}

int writefile()
{
        int totalexp = 0, max, i, j = 1;
        string show="",fname="";
        mixed *ob_list, *ob_list_old;
        object *ob, obj1;
        string top10 = "";
        string onlineuser = "";
        string party = "";
        string title="";
                
        ob_list_old = users();
        ob_list_old = filter_array(ob_list_old, (: filldata :));
        max=sizeof(ob_list_old);
        if (max > 10) max=10;
        ob_list = sort_array(ob_list_old, (: sort :));
        top10 += "<table width='97%' border='0' align='center' bgcolor='#000000'>
  <tr>
    <td height='317'><div align='center'><BR>
        <strong><font color='#FFFF00' face='Fixedsys'>" + CHINESE_MUD_NAME + "在线十大高手("+CHINESE_D->chinese_date(time(),1)+")</font></strong><font face='Fixedsys'><br>
        </font></div>
      <table width='70%' border='0' align='center' cellpadding='4' cellspacing='3' bgcolor='#000000'>
        <tr align=center> 
          <td width='10%' height='30'><font color='#00FF00' face='Fixedsys'><strong>名次 
            </strong></font></td>
          <td width='50%' height='30'><font color='#00FF00' face='Fixedsys'><strong>昵称</strong></font></td>
          <td width='25%' height='30'><font color='#00FF00' face='Fixedsys'><strong> 
            姓名</strong></font></td>
          <td width='15%' height='30'><font color='#00FF00' face='Fixedsys'><strong>门派</strong></font></td>
        </tr>";
        for (i=0;i<max;i++)
          {
            obj1 = ob_list[i];
            show = obj1->query("name")+"("+capitalize(obj1->query("id"))+")";
            top10 += sprintf("<tr align=center> 
          <td><font color='#008000' face='Fixedsys'>No.%d</font></td>
          <td><font color='#008000' face='Fixedsys'>「%s」</font></td>
          <td><font color='#008000' face='Fixedsys'>%s</font></td>
          <td><font color='#008000' face='Fixedsys'>%s</font></td>
          </tr>",
          i+1,obj1->query("nickname")?obj1->query("nickname"):"暂无",show,
          obj1->query("family/family_name"));
          }
        top10 += "</table>
      <br>
    </td>
  </tr>
</table>";
 
//在线玩家及门派实力文件生成
        onlineuser += "<table width='97%' border='0' align='center' bgcolor='#000000'>
  <tr>
    <td height='317'><div align='center'><BR>
        <strong><font color='#FFFF00' face='Fixedsys'>" + CHINESE_MUD_NAME + "在线玩家清单("+CHINESE_D->chinese_date(time(),1)+")</font></strong><font face='Fixedsys'><br>
        </font></div>
      <table border=0 cellpadding=0 cellspacing=5 width='97%'>
        <tr align=center> 
          <td width='12%' ><font color='#FFFF00' face='Fixedsys'>门派</font></td>
          <td colspan=4 ><font color='#FFFF00' face='Fixedsys'>中文姓名(ID)</font></td>
        </tr>";

	party += "<table width='97%' border='0' align='center' bgcolor='#000000'>
  <tr>
    <td height='317'><div align='center'><BR>
        <strong><font color='#FFFF00' face='Fixedsys'>" + CHINESE_MUD_NAME + "在线玩家门派实力图示("+CHINESE_D->chinese_date(time(),1)+")</font></strong><font face='Fixedsys'><br>
        </font></div>
      <table border=0 cellpadding=0 cellspacing=0 width='97%'>
        <tr align=center> 
          <td width='12%'><font color='#00FF00' face='Fixedsys'>门派</font></td>
          <td width='88%'><font color='#00FF00' face='Fixedsys'>综合实力</font></td>
        </tr>";

        ob = filter_array(objects(), (: userp :));
        list = sort_array(ob, (: sort_user :));
        ppl_cnt = 0;
        i = sizeof(list);
        while( i-- ) {
                if( !environment(list[i]) ) continue;
                if (wizardp(list[i]) && list[i]->query("env/invisibility")) continue;
                if (!wiz_level(list[i]) && fname != list[i]->query("family/family_name")) {
                  if (totalexp > 7000000) totalexp = 7000000;
		    if ((fname?totalexp:0) != 0)
		    {
		    	party += sprintf("%s%d%s\n","        <tr> 
          <td align=center><font color='#008000' face='Fixedsys'>"+fname+"</font></td>
          <td><hr align=left noshade size='10' color='#FF0000' width='",(totalexp/70000),"%'>");
		    	party += sprintf("%s%d%s\n",
		    	"<hr align=left noshade size='10' color='#0000FF' width='",(totalexp/70000)/j,"%'></td></tr>");
		    }
                    fname = list[i]->query("family/family_name");
                    
                    totalexp = 1;
                    j = 0;
                }
                if (!wiz_level(list[i])){
                	totalexp += (int)list[i]->query("combat_exp");
                	j ++;
                }
                if (interactive(list[i])) ppl_cnt++;
        }
   	party += sprintf("%s%d%s\n","<tr> 
          <td align=center><font color='#008000' face='Fixedsys'>"+fname+"</font></td>
          <td><hr align=left noshade size='10' color='#FF0000' width='",(totalexp/70000),"%'>");
   	party += sprintf("%s%d%s\n","<hr align=left noshade size='10' color='#0000FF' width='",(totalexp/70000)/j,"%'></td></tr>");
        party += "</table>
      <div align='center'> 
        <p><font color=#FF0000 face='Fixedsys'>红色</font><font color=#008000 face='Fixedsys'>表示门派EXP总和，<font color='#0000FF'>蓝色</font>表示EXP平均值。</font><br>
        </p>
      </div></td>
  </tr>
</table>";
       
        title += "<table width='97%' border='0' align='center' bgcolor='#000000'>
  <tr>
    <td height='317'><div align='center'><BR>
        <strong><font color='#FFFF00' face='Fixedsys'>" + CHINESE_MUD_NAME + "在线玩家详细资料("+CHINESE_D->chinese_date(time(),1)+")</font></strong><font face='Fixedsys'><br>
        </font></div>
      <table border=0 cellpadding=0 cellspacing=5 width='97%'>
        <tr align=left> 
          <td width='12%' ><font color='#FFFF00' face='Fixedsys'>门派</font></td>          
        </tr>";
        
	top10 = replace_string(top10,"'","\"");
	onlineuser = replace_string(onlineuser,"'","\"");
	party = replace_string(party,"'","\"");	
	title = replace_string(title,"'","\"");
//文件生成结束
	now_num = 0;
	fnamea ="";
	if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "REPLACE INTO mud_info (subject, utime, content, type, site)
			VALUES ('"+CHINESE_MUD_NAME+"在线玩家详细资料"+"', '"+time()+"', 
			'"+title+"', 'title', '"+lower_case(INTERMUD_MUD_NAME)+"')",this_object(),"upload_title"))
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家详细资料自动更新到主页.........开始更新...。");
			else
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家详细资料自动更新到主页.........失败。");
				
	if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "REPLACE INTO mud_info (subject, utime, content, type, site)
			VALUES ('"+CHINESE_MUD_NAME+"在线十大高手"+"', '"+time()+"', 
			'"+top10+"', 'top', '"+lower_case(INTERMUD_MUD_NAME)+"')"))
				CHANNEL_D->do_channel( this_object(), "sys", "在线十大高手数据自动更新到主页.........成功。");
			else
				CHANNEL_D->do_channel( this_object(), "sys", "在线十大高手数据自动更新到主页.........失败。");

	user_num = 0;
	count_user =0;
	fnameb ="";
	if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "REPLACE INTO mud_info (subject, utime, content, type, site)
			VALUES ('"+CHINESE_MUD_NAME+"在线玩家"+"', '"+time()+"', 
			'"+onlineuser+"', 'online', '"+lower_case(INTERMUD_MUD_NAME)+"')",this_object(),"upload_user"))
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家数据自动更新到主页.........开始更新...。");
			else
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家数据自动更新到主页.........失败。");
	
	if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "REPLACE INTO mud_info (subject, utime, content, type, site)
			VALUES ('"+CHINESE_MUD_NAME+"在线玩家门派实力图示"+"', '"+time()+"', 
			'"+party+"', 'party', '"+lower_case(INTERMUD_MUD_NAME)+"')"))			
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家门派实力图示数据自动更新到主页.........成功。");
			else
				CHANNEL_D->do_channel( this_object(), "sys", "在线玩家门派实力图示数据自动更新到主页.........失败。");
				
	remove_call_out("writefile");
	call_out("writefile", 3600);
	return 1;
}
string get_frame(object ob);

void upload_title(mixed ret)
{	
	int i = 0;
	string title ="";
	string sql;
	int max = now_num+10;
	if(max>sizeof(list)) max=sizeof(list);
	for(i=now_num+0;i<max;i++){
		if(!objectp(list[i])) continue;
		if( !environment(list[i]) ) continue;
                if (wizardp(list[i]) && list[i]->query("env/invisibility")) continue;
                if(fnamea!=get_frame(list[i]))
                {
                	fnamea=get_frame(list[i]);
          	    	title += "<td width='12%' align=left ><font color='#00FF00' face='Fixedsys'>"+fnamea+ "</font></td>";
          	    	title += "</tr><tr> ";
                }
                     
		title += sprintf("%s%12s%s%s%s\n",
			"<td width='22%'><font color='#008000' face='Fixedsys'>",
			RANK_D->query_rank(list[i]),
			interactive(list[i])?
			(query_idle(list[i]) > 120?HIM"+"NOR:" ")
			:HIG"*"NOR,
			list[i]->short(1),
			"</font></td>"
		);
		title += "</tr><tr> ";
	}
	title = replace_string(title,"'","\"");
	now_num += 10;	
	if(title!="") 
	{
	sql = sprintf("UPDATE mud_info SET content = CONCAT(content,'%s') WHERE subject='%s' AND site = '%s'",
	title,CHINESE_MUD_NAME+"在线玩家详细资料",lower_case(INTERMUD_MUD_NAME));
	BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,sql,this_object(),"upload_title");	
	//message("wizard", sql,users());
	CHANNEL_D->do_channel( this_object(), "sys", "在线玩家详细资料自动更新到主页.........继续更新...。");
	}
	else
	{
		title = "</table>
      			<div align='center'>
        		<p><font color='#008000' face='Fixedsys'>\n";
        	title += sprintf("有 <font color=yellow>%d</font> 位玩家连线中。</font></p>
        			<p><br>
        			</p>
      				</div></td>
  				</tr>
				</table>", ppl_cnt);
		title = replace_string(title,"'","\"");
		sql = sprintf("UPDATE mud_info SET content = CONCAT(content,'%s') WHERE subject='%s' AND site = '%s'",
		title,CHINESE_MUD_NAME+"在线玩家详细资料",lower_case(INTERMUD_MUD_NAME));
		BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,sql);	
		CHANNEL_D->do_channel( this_object(), "sys", "在线玩家详细资料自动更新到主页.........成功。");
	}
}
void upload_user(mixed ret)
{
	int i = 0;
	string str="";
	string sql;	
	int max = user_num+100;
	if(max>sizeof(list)) max=sizeof(list);	
	for(i=user_num+0;i<max;i++){
		if(!objectp(list[i])) continue;
		if( !environment(list[i]) ) continue;
                if (wizardp(list[i]) && list[i]->query("env/invisibility")) continue;
                if(fnameb!=get_frame(list[i]))
                {
                	fnameb=get_frame(list[i]);
                	if (count_user % 5) str += "</tr><tr> ";
          	    	str += "<td width='12%' align=center ><font color='#00FF00' face='Fixedsys'>"+fnameb+ "</font></td>";
                    	count_user = 1;
                }
		if (!(count_user % 5)) {
                	count_user++;
                  	str += "<td>　</td>";
                }
                str += sprintf("%s%s%s",
                      "<td width='22%'><font color='#008000' face='Fixedsys'>"+
                      (interactive(list[i])?(query_idle(list[i]) > 120?"<font color=BLUE>":""):"<font color=RED>"),
                      list[i]->name(1) + "("+capitalize(list[i]->query("id"))+")"+(interactive(list[i])?"":"</font>")+"</td>",
                      (++count_user % 5) ? "" : "</tr>\n<tr>");
	}
	str = replace_string(str,"'","\"");
	user_num += 100;
	if(str!="") 
	{
	sql = sprintf("UPDATE mud_info SET content = CONCAT(content,'%s') WHERE subject='%s' AND site = '%s'",
	str,CHINESE_MUD_NAME+"在线玩家",lower_case(INTERMUD_MUD_NAME));
	BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,sql,this_object(),"upload_user");
	CHANNEL_D->do_channel( this_object(), "sys", "在线玩家数据自动更新到主页.........继续更新...。");
	//message("wizard", sql,users());
	}
	else {
		str = "</table>
      			<div align='center'>
        		<p><font color='#008000' face='Fixedsys'>\n";
        	str += sprintf("有 <font color=yellow>%d</font> 位玩家连线中，<font color='#FF0000'>红色</font>表示断线，<font color=BLUE>蓝色</font>表示发呆。</font></p>
        			<p><br>
        			</p>
      				</div></td>
  				</tr>
				</table>", ppl_cnt);
		str = replace_string(str,"'","\"");
		sql = sprintf("UPDATE mud_info SET content = CONCAT(content,'%s') WHERE subject='%s' AND site = '%s'",
		str,CHINESE_MUD_NAME+"在线玩家",lower_case(INTERMUD_MUD_NAME));
		BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,sql);
		CHANNEL_D->do_channel( this_object(), "sys", "在线玩家数据自动更新到主页.........成功。");
	}
}
string get_frame(object ob)
{
	if(wiz_level(ob)) return "游戏管理员";
	if(!mapp(ob->query("family"))) return "普通百姓";
	return ob->query("family/family_name");	
}

int sort(object obj1, object obj2)
{
  int temp1,temp2;
  temp1=obj1->query("combat_exp");
  temp2=obj2->query("combat_exp");
  if (temp1>temp2) return -1;
  else if (temp1<temp2) return 1;
  else return 0;
}

int filldata(object obj1)
{
  if(wiz_level(obj1)) return 0;
  if(!environment(obj1)) return 0;
  else return 1;
}

string remove_ansi(string str)
{
        int i;
        string *color = ({ BLK, RED, GRN, YEL, BLU, MAG, CYN, WHT,
                           HIR, HIG, HIY, HIB, HIM, HIC, HIW,
                           HBRED, HBGRN, HBYEL, HBBLU, HBMAG, HBCYN, HBWHT,
                           BBLK, BRED, BYEL, BBLU, BMAG, BCYN,
                           NOR, BOLD });
        if( !str || !stringp(str) ) return 0;
        i = sizeof(color);
        while( i-- ) {
                str = replace_string(str, color[i], "");
                }
        return str;
}

int sort_user(object ob1, object ob2)
{
        string name1, name2;

        reset_eval_cost();
        if( wiz_level(ob1) != wiz_level(ob2) )
                return wiz_level(ob1)- wiz_level(ob2);

        name1 = get_frame(ob1);
        name2 = get_frame(ob2);
        //name1 = ob1->query("family/family_name");
        //name2 = ob2->query("family/family_name");
        
        if (intp(name1)) name1 = "";
        if (intp(name2)) name2 = "";
        if (name1 > name2) return -1;
        if (name1 < name2) return 1;

        name1 = ob1->query("id");
        name2 = ob2->query("id");

        if (intp(name1)) name1 = "";
        if (intp(name2)) name2 = "";
        if (name1 > name2) return -1;
        if (name1 < name2) return 1;
//      return 0;
	return (int)ob2->query("combat_exp") - (int)ob1->query("combat_exp");
}
