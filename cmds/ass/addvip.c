// addvip.c 
// by LinuX@SJ 2004/02/18

#include <ansi.h>

#define VIP_FILE       "/data/vip"
#define NULL 0

inherit F_SAVE;

mapping *notes;
string query_save_file();
void list_viplist(object me, int num);
void search_viplist(object me, string arg, int flag);
void do_read(object me, int i);
int search_mail(string mail);
int search_player(string player,string site);


void create()
{
	seteuid(getuid());
	restore();
}

int main(object me, string arg)
{
	mapping note;
	string key;
	string player,mail;
	string site;
	string s;
	int num,renew,i,day;
	
	if (!arg) {
		list_viplist(me, 10);
		return 1;
	}
		
	if (sscanf(arg, "%d", num) == 1) {
		do_read(me,num);
		return 1;
	}
	
	if (sscanf(arg, "%s -p", key) == 1) {
		search_viplist(me,key,1);
		return 1;
	}
	
	if (sscanf(arg, "%s -m", key) == 1) {
		if (!stringp(key) || sscanf(key, "%*s@%*s.%*s") != 3) 
			return notify_fail("您要搜索的电子邮件地址错误。 \n");
		search_viplist(me,key,0);
		return 1;
	}
	
    	if (sscanf(arg, "%s", s) == 1) {
		if ( s == "all" ) {
			list_viplist(me,0);
			return 1;
		}
	}

	if (sscanf(arg, "%s %s %s %d", site, player, mail, day) == 4) {
		if (wiz_level(me)<4)
			return notify_fail("只有arch才可以将玩家列入VIP总管理名单。 \n");
			
	   site = lower_case(site);
	   player = lower_case(player);
	    if ( site!="ln"&& site!="ln2" && site!="bx" && site!="ty" && site!="cq") 
           	 	return notify_fail("必须是LN,LN2,BX,TY,CQ这几个站的玩家!\n");
	    if (!stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3) 
			return notify_fail("电子邮件地址错误。 \n");
		site = upper_case(site);
		i = search_mail(mail);
		if (i > 0) {
			write(" "+HIY+mail+NOR+" 这个信箱地址已经有人使用了，编号为：" + HIW + i + NOR + " 。");
			search_viplist(me,mail,0);
			return 1;
		
		}
		i = search_player(player,site);
		if (i > 0) {
			write(" "+HIY+player+"@"+site+NOR+" 这个玩家已经登记过了，编号为：" + HIW + i + NOR + " 。");
			search_viplist(me,player,1);
			return 1;
		
		}
	
		note = allocate_mapping(6);
		note["time"] = time(); 	//时间
		note["wiz"] = me->query("id"); //处理巫师
		note["player"] = player+"@"+site; //贵宾ID?		note["mail"] = mail; 	//贵宾信箱
		note["day"] = sprintf("%d",day); 	//贵宾时间
		note["renew"] = NULL;	//是否续费
		
        if( !pointerp(notes) || !sizeof(notes) )
                notes = ({ note });
        else
                notes += ({ note });
		save();
		write("你把 " + HIY + player + "@" + site + NOR + " 成功地列入VIP总管理名单，该玩家注册信箱为 "+HIY+mail+NOR"，贵宾有效期为 "+HIY+day+NOR+" 天。\n");
		return 1;
	}

	if (sscanf(arg, "-d %d", num)== 1) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以删除VIP总管理名单上的内容。 \n");
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("VIP总管理名单上没有这个编号的内容。\n");
		num--;
		notes = notes[0..num-1] + notes[num+1..<1];
		save();
		write("第 "+HIY+ (num+1) +NOR+ " 号VIP总管理名单的内容已经删除。\n");
		return 1;
	}

	if (sscanf(arg, "-r %d %d", num, renew)== 2) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以修改VIP总管理名单上的内容。 \n");
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("VIP总管理名单上没有这个编号的内容。\n");
		notes[num-1]["renewwiz"]=me->query("id");
		notes[num-1]["renew"]=renew;
		save();
		write("第 "+HIY+ (num) + NOR+" 号VIP总管理名单的续费已经完毕。\n");
		return 1;
	}


	write("语法错误，请help addvip。\n");
	return 1;
}

string query_save_file()
{
	return VIP_FILE ;
}


void list_viplist(object me, int num)
{
	int i;
	string str;

	i = sizeof(notes);
	if( i < 1 ) write("书剑VIP总管理名单目前没有任何内容。\n");
	else {
		str="书剑VIP总管理名单：\n";
		str += repeat_string("━", 55) + "\n";
		while (i-- && (i>=sizeof(notes)-num || num==0)) {
			str += sprintf("%3d %s %-12s信箱：%-=20s 日期：%s 天   处理巫师：%s，%s\n",
				i+1,
				ctime(notes[i]["time"])[4..15],
				capitalize(notes[i]["player"]),
				notes[i]["mail"],
				notes[i]["day"],
				capitalize(notes[i]["wiz"]),
				notes[i]["renew"]?
					("续费："+notes[i]["renew"]+
						" 天 续费巫师："+capitalize(notes[i]["renewwiz"]))
					:"该玩家没有续费"
			);
			str += repeat_string("━", 55) + "\n";
		}
	}
	me->start_more( str );
}

int search_mail(string mail)
{
    int i;
    if( !(i = sizeof(notes)) ) return 0;
    for( ; i > 0; i-- ) 
	if( notes[i-1]["mail"] == mail ) break;
    return i;
}

int search_player(string player,string site)
{
    int i;
    if( !(i = sizeof(notes)) ) return 0;
    for( ; i > 0; i-- ) 
	if ((strsrch(notes[i-1]["player"],player) > -1) && (strsrch(notes[i-1]["player"],site) > -1 ))break;
    return i;
}


void do_read(object me, int i) 
{
    string s;
    if( i > sizeof(notes) || i < 1) {
	write("书剑VIP总管理名单中无此编号!!\n");
	return;
    }
    s="书剑VIP总管理名单编号："+HIG+i+NOR+"\n";
    s += repeat_string("━", 55) + "\n";
    s += sprintf("%3d %s %-12s信箱：%-=20s 日期：%s 天   处理巫师：%s，%s\n",
				i,
				ctime(notes[i-1]["time"])[4..15],
				capitalize(notes[i-1]["player"]),
				notes[i-1]["mail"],
				notes[i-1]["day"],
				capitalize(notes[i-1]["wiz"]),
				notes[i-1]["renew"]?
					("续费："+notes[i-1]["renew"]+
						" 天 续费巫师："+capitalize(notes[i-1]["renewwiz"]))
					:"该玩家没有续费"
			);
    s += repeat_string("━", 55) + "\n";
			
    me->start_more(s);
}
void search_viplist(object me, string arg, int flag)
{
	int i;
	string str,str2;
	str2 = "";

	i = sizeof(notes) -1;
	if( i < 0 ) write("书剑VIP总管理名单目前没有任何内容，无法进行搜索。\n");
	else {
		str="书剑VIP总管理名单，您当前搜索的关键字是：" + HIY + arg + NOR"\n";
		str += repeat_string("━", 55) + "\n";
		while (i>=0) {
			
			if (notes[i]["mail"] == arg){
			str2 += sprintf("%3d %s %-12s信箱："HBRED+HIY"%-=20s "+NOR+"日期：%s 天   处理巫师：%s，%s\n",
				i+1,
				ctime(notes[i]["time"])[4..15],
				capitalize(notes[i]["player"]),
				notes[i]["mail"],
				notes[i]["day"],
				capitalize(notes[i]["wiz"]),
				notes[i]["renew"]?
					("续费："+notes[i]["renew"]+
						" 天 续费巫师："+capitalize(notes[i]["renewwiz"]))
					:"该玩家没有续费"
			);
			str2 += repeat_string("━", 55) + "\n";
		}
		
			if (strsrch(notes[i]["player"],arg) > -1 && flag){
			str2 += sprintf("%3d %s %-12s信箱：%-=20s 日期：%s 天   处理巫师：%s，%s\n",
				i+1,
				ctime(notes[i]["time"])[4..15],
				HBRED+HIY+capitalize(notes[i]["player"])+NOR,
				notes[i]["mail"],
				notes[i]["day"],
				capitalize(notes[i]["wiz"]),
				notes[i]["renew"]?
					("续费："+notes[i]["renew"]+
						" 天 续费巫师："+capitalize(notes[i]["renewwiz"]))
					:"该玩家没有续费"
			);
			str2 += repeat_string("━", 55) + "\n";			
		}
		i--;
		}
		if (str2==""){
			str += "没有找到用 " + arg + " 为信箱的贵宾玩家。\n";
			str += "请确认该信箱是否为正式缴费信箱，如果是，请先登记，然后在游戏中加玩家的贵宾时间。\n";
			str += repeat_string("━", 55) + "\n";	
		}
		else
			str = str + str2;	
	}
		me->start_more( str );
}

int help(object me)
{
        write(@HELP
指令格式 :
addvip [site] [player] [mail] [day] 增加新玩家到 书剑VIP管理 总列表
site 	是所在书剑站点，当前只允许 LN,LN2,BX,TY,CQ这几个站的玩家 
player 	是玩家ID
mail	是玩家申请贵宾时候的有效百姓Email
day	是玩家的贵宾有效期

addvip [num] 查看具体某个编号的具体
num	是列表中的编号

addvip [key] -m 按照key为关键字，查找有没有使用[key]为注册信箱的玩家。
addvip [key] -p 按照key为关键字，查找有没有使用[key]的玩家。

addvip -d [num] 删除一条记录
num	是列表中的编号

addvip -r [num] [day] 续费过程
num	是列表中的编号
day	是续费的具体时间

HELP);
 
        return 1;
}
