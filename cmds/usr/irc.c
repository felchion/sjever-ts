// irc.c
// by augx@sj  9/24/2001

#include <ansi.h>
inherit F_DBASE;

int help(object me);

void leave_user(object me,string var)
{
	mapping m = CHANNEL_D->query_ircs(var);
	if(!m) return;
	m["user"] -= ({ me->query("id") });
	m["master"] -= ({ me->query("id") });
}

int leave_all_room(object me,string var)
{
	string str;
	if( stringp(me->query_temp(var)) ) {
		str = me->query_temp(var);
		CHANNEL_D->do_channel(me,"irc*","离开了聊天室。");
		leave_user(me,str); 
		me->delete_temp(var); 
		me->set_temp("irc/room_sayin",str);
		return 1;
	}
	else return 0;
}

void delete_busy(object me)
{
	me->delete_temp("irc_busy");
}

void create()
{
	seteuid(getuid());
	set("channel_id", "聊天精灵");
	set_temp("irc/room",1);
	set_temp("irc/room1","irc_channel");
	call_out("check_irc",5*60);
}

void check_irc()
{
	mapping m;
	mapping * v;
	string * k;
	int i,j;
	
	remove_call_out("check_irc");
	call_out("check_irc",5*60);
	
	if( !(m=CHANNEL_D->query_all_ircs()) ) return;
	if( (j=sizeof(m)) < 1 ) return;
	
	k = keys(m);
	v = values(m);
	for(i=0;i<j;i++) {	
		if( k[i] == "irc_channel" ) continue;
		if( sizeof(v[i]["user"]) < 1 )
			CHANNEL_D->delete_ircs(k[i]);
	}
}

static string * banned_name = ({
"聊天室","书剑","谣言","闲聊",	
// Mud 保留名字
"自己","杀手","闪电","雪人","逍遥","某人","天神","总管","龙神","仙人","巫师","名字待定","祈雨","迎风",
// Mud 地名，门派名
"衡山","华山","嵩山","恒山","泰山","雪山","黄山","天山","西域","苗疆","东北","中原",
"南疆","长江","黄河","中国","中华","昆仑山","长白山","白驼山","武当山","峨嵋山",
"冰火岛","桃花岛","黑木崖",
"少林","丐帮","明教","密宗","慕容","古墓","全真","铁掌","大理","白驼","桃花","星宿",
"天龙寺","大轮寺","五毒教","正义","邪恶","五岳剑派","日月神教","官府","武林",
"少林寺","武当派","铁掌帮","普通百姓","峨嵋派","姑苏慕容","古墓派","华山派","嵩山派",
"明教","神龙教","星宿派",
// 重要人物名字
"金庸","邓小平","江泽民","毛泽东","朱榕基","皇帝","总理", "主席",
// 脏话
"统一","法轮","法轮研究","法轮大","吃人","他妈的","去你的","操你妈","干你娘","废物",
"混蛋","坏蛋","杀人","抢劫","强奸","妓女","台独","领导","中央","屁眼","屁股","裸体",
"放屁","鸡巴","阴道","婊子","你丫","日你","赤佬","瘪三","傻逼","蠢蛋","操他","我操",
"太监","宦官","阉割","阉人","鸡", "奸淫", "淫荡", "轮奸", "奸", "三级", "政治",
});
static string * banned_ename = ({ "irc_channel","sj","irc","shit","damn","fuck","sex" });

int main(object me, string arg)
{
	string cmd,var, rid,rname,invite,passwd, user;
	mapping m;
	int vars, i;
	object ob;
	
	if (!arg || arg[0]!='/') return 0;
	
	if( (vars = sscanf(arg,"/%s %s",cmd,var)) < 2 )
		if( (vars = sscanf(arg,"/%s",cmd)) < 1 )
			cmd = "dunno";
	if((int)me->query_temp("irc_busy") && cmd!="leaveall") {
		write("你正忙着呢！\n");
		return 1;
	}
	switch (cmd) {
		case "?":
		case "help": help(me); break;
		case "list": 
			CHANNEL_D->irc_list(me);
			me->set_temp("irc_busy",1);
			call_out("delete_busy",2,me);
			break;
		case "create": 
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if( !stringp(me->query_temp("user_type")) ) me->query_user_type();
			if( me->query_temp("user_type")=="worker" ) i=100000; else i=200000;
			if( me->query("combat_exp") < i ) {
				write("你的经验不够创建聊天室。\n");
				break;
			}
			if( (string)me->query_temp("irc/create") ) {
				write("你已经创建了一个聊天室了。\n");
				break;
			}

			invite = "no";
			passwd = "nopasswd";
			if( sscanf(var,"%s %s %s %s",rid,rname,invite,passwd)!=4 )
				if( sscanf(var,"%s %s %s",rid,rname,invite)!=3 )
					if( sscanf(var,"%s %s",rid,rname)!=2 ) {
						write("命令格式错误！\n");
						break;
					}
			if( strlen(rid)<3 || strlen(rid)>8 ) {
				write("聊天室英文名必须是3到8个英文字母！\n");
				break;
			}
			if( strlen(rname)<4 || strlen(rname)>16 ) {
				write("聊天室中文名必须是2到8个汉字！\n");
				break;
			}
			if( strlen(passwd)<3 || strlen(passwd)>8 ) {
				write("聊天室密码必须是3到8个字母或数字！\n");
				break;
			}
			if( invite!="invite" && invite!="no" ) {
				write("聊天室是否必须邀请才能加入？\n");
				break;
			}
			if( member_array(rname,banned_name)!=-1 )  {
				write("聊天室的中文名字非法，可能会引起混淆！\n");
				break;
			}
			if( member_array(rid,banned_ename)!=-1 )  {
				write("聊天室的英文名字非法，可能会引起混淆！\n");
				break;
			}
			
			if( CHANNEL_D->query_ircs(rid) ) {
				write("此聊天室已经被创建。\n");
				break;
			}
			if( CHANNEL_D->query_ircs_creator(me) != "" ) {
				me->set_temp("irc/create" , CHANNEL_D->query_ircs_creator(me));
				write("你已经创建了一个聊天室了。\n");
				break;
			}
			
			m = ([ "room_name":rname , "creator":me->query("id") , "invite":invite=="no"?0:1 , "passwd":passwd , "user":({ }) , "master":({ }) ]);
			CHANNEL_D->set_ircs(rid , m);	
			me->set_temp("irc/create",rid);
			CHANNEL_D->do_channel(this_object(),"irc",me->query("name")+"创建了 "+rname+"("+rid+")。");
			var = rid+" "+passwd;
			me->set_temp("invite/"+rid,1);
		case "join":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			passwd = "nopasswd";
			if( sscanf(var,"%s %s",rid,passwd)!=2 )	rid = var;
			
			if( !(m=CHANNEL_D->query_ircs(rid)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( m["passwd"] != passwd ) {
				write("密码错误，不能加入这个聊天室。\n");
				break;
			}
			if( m["invite"] && m["creator"]!=me->query("id") && !(int)me->query_temp("invite/"+rid) ) {
				write("此聊天室需要邀请才能加入。\n");
				break;
			}
			if( member_array(me->query("id"),m["user"]) != -1 ) {
				write("你已经加入此聊天室了。\n");
				break;
			}
			
			if(!stringp(me->query_temp("irc/room1"))) { me->set_temp("irc/room",1); me->set_temp("irc/room1",rid); }
			else if(!stringp(me->query_temp("irc/room2"))) { me->set_temp("irc/room",2); me->set_temp("irc/room2",rid); }
			else if(!stringp(me->query_temp("irc/room3"))) { me->set_temp("irc/room",3); me->set_temp("irc/room3",rid); }
			else if(!stringp(me->query_temp("irc/room4"))) { me->set_temp("irc/room",4); me->set_temp("irc/room4",rid); }
			else if(!stringp(me->query_temp("irc/room5"))) { me->set_temp("irc/room",5); me->set_temp("irc/room5",rid); }
			else { write("你已经加入五个聊天室了！\n"); break; }

			CHANNEL_D->do_channel(me,"irc*","进入聊天室。");
			me->delete_temp("invite/"+rid);
			
			m["user"] += ({ me->query("id") });
			if( m["creator"]==me->query("id") ) m["master"] += ({ me->query("id") });
			
			me->set_temp("irc_busy",1);
			call_out("delete_busy",1,me);
			break;
		case "close": 
			if(vars<2) {
				var = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!var) {
					write("命令格式错误！\n");
					break;
				}
			}
			
			if( !(m=CHANNEL_D->query_ircs(var)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( !wizardp(me) && member_array(me->query("id"),m["master"])==-1 ) {
				write("你没有关闭这个聊天室的权限！\n"); 
				break; 
			}

			me->set_temp("irc/room_sayin",var);
			CHANNEL_D->do_channel(me,"irc*","关闭了聊天室。");
			for(i=0;i<sizeof(m["user"]);i++) {
				if( !(ob=find_player(m["user"][i])) ) continue;
				
				if( ob->query_temp("irc/create")==var ) ob->delete_temp("irc/create");
				if(var==ob->query_temp("irc/room1")) ob->delete_temp("irc/room1");
				else if(var==ob->query_temp("irc/room2")) ob->delete_temp("irc/room2");
				else if(var==ob->query_temp("irc/room3")) ob->delete_temp("irc/room3");
				else if(var==ob->query_temp("irc/room4")) ob->delete_temp("irc/room4");
				else if(var==ob->query_temp("irc/room5")) ob->delete_temp("irc/room5");
			}			
			CHANNEL_D->delete_ircs(var);
			CHANNEL_D->do_channel(this_object(),"irc",me->query("name")+"关闭了 "+m["room_name"]+"("+var+")。");
			
			me->set_temp("irc_busy",1);
			call_out("delete_busy",2,me);
			break;
		case "leave":
			if(vars<2) {
				var = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!var) {
					write("命令格式错误！\n");
					break;
				}
			}
			if(var==me->query_temp("irc/room1")) ;
			else if(var==me->query_temp("irc/room2")) ;
			else if(var==me->query_temp("irc/room3")) ;
			else if(var==me->query_temp("irc/room4")) ;
			else if(var==me->query_temp("irc/room5")) ;
			else { write("你要离开哪个聊天室？\n"); break; }
			
			me->set_temp("irc/room_sayin",var);
			CHANNEL_D->do_channel(me,"irc*","离开了聊天室。");

			if(var==me->query_temp("irc/room1")) me->delete_temp("irc/room1");
			else if(var==me->query_temp("irc/room2")) me->delete_temp("irc/room2");
			else if(var==me->query_temp("irc/room3")) me->delete_temp("irc/room3");
			else if(var==me->query_temp("irc/room4")) me->delete_temp("irc/room4");
			else if(var==me->query_temp("irc/room5")) me->delete_temp("irc/room5");
			else { write("你要离开哪个聊天室？\n"); break; }

			leave_user(me,var);

			me->set_temp("irc_busy",1);
			call_out("delete_busy",1,me);
			break;
		case "leaveall":
			leave_all_room(me,"irc/room1");
			leave_all_room(me,"irc/room2");
			leave_all_room(me,"irc/room3");
			leave_all_room(me,"irc/room4");
			leave_all_room(me,"irc/room5");
			break;
		case "listuser":
			if(vars<2) {
				var = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!var) {
					write("命令格式错误！\n");
					break;
				}
			}
			if( !(m=copy(CHANNEL_D->query_ircs(var))) ) {
				write("没有这个聊天室。\n");
				break;
			}
			write("此聊天室的用户有：");
			for(i=0;i<sizeof(m["master"]);i++)
				write(m["master"][i]+"+ , ");
			m["user"] -= m["master"];
			for(i=0;i<sizeof(m["user"]);i++)
				write(m["user"][i]+" , ");
			write("\n");
			break;
		case "listroom":
			write("你加入的聊天室有：");
			if(stringp(me->query_temp("irc/room1"))) write(me->query_temp("irc/room1")+" ; ");
			if(stringp(me->query_temp("irc/room2"))) write(me->query_temp("irc/room2")+" ; ");
			if(stringp(me->query_temp("irc/room3"))) write(me->query_temp("irc/room3")+" ; ");
			if(stringp(me->query_temp("irc/room4"))) write(me->query_temp("irc/room4")+" ; ");
			if(stringp(me->query_temp("irc/room5"))) write(me->query_temp("irc/room5")+" ; ");
			write("\n");
			break;
		case "setroom":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if(var==me->query_temp("irc/room1")) me->set_temp("irc/room",1);
			else if(var==me->query_temp("irc/room2")) me->set_temp("irc/room",2);
			else if(var==me->query_temp("irc/room3")) me->set_temp("irc/room",3);
			else if(var==me->query_temp("irc/room4")) me->set_temp("irc/room",4);
			else if(var==me->query_temp("irc/room5")) me->set_temp("irc/room",5);
			else { write("你要设定在哪个聊天室中说话？\n"); break; }
			write("设定成功。\n");
			break;
		case "kickout":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if( sscanf(var,"%s %s",user,rid)!=2 ) {
				user = var;
				rid = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!rid) { write("命令格式错误！\n"); break; }
			}
			if( !(m=CHANNEL_D->query_ircs(rid)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( !wizardp(me) && member_array(me->query("id"),m["master"]) == -1 ) {
				write("你没有这个权限。\n");
				break;
			}
			if( member_array(user,m["user"])==-1 || !(ob=find_player(user)) ) {
				write("此用户不在此聊天室中。\n");
				break;
			}
						
			me->set_temp("irc/room_sayin",rid);
			CHANNEL_D->do_channel(me,"irc*","把"+ob->query("name")+"踢出聊天室。");
			
			if(rid==ob->query_temp("irc/room1")) ob->delete_temp("irc/room1");
			else if(rid==ob->query_temp("irc/room2")) ob->delete_temp("irc/room2");
			else if(rid==ob->query_temp("irc/room3")) ob->delete_temp("irc/room3");
			else if(rid==ob->query_temp("irc/room4")) ob->delete_temp("irc/room4");
			else if(rid==ob->query_temp("irc/room5")) ob->delete_temp("irc/room5");	
			leave_user(ob,rid);			
			break;
		case "hat":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if( sscanf(var,"%s %s",user,rid)!=2 ) {
				user = var;
				rid = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!rid) { write("命令格式错误！\n"); break; }
			}
			if( !(m=CHANNEL_D->query_ircs(rid)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( !wizardp(me) && member_array(me->query("id"),m["master"]) == -1 ) {
				write("你没有这个权限。\n");
				break;
			}
			if( member_array(user,m["user"])==-1 || !(ob=find_player(user)) ) {
				write("此用户不在此聊天室中。\n");
				break;
			}
			
			m["master"] += ({ user });
			
			me->set_temp("irc/room_sayin",rid);
			CHANNEL_D->do_channel(me,"irc*","给"+ob->query("name")+"戴上一顶帽子。");
			
			me->set_temp("irc_busy",1);
			call_out("delete_busy",1,me);
			break;
		case "unhat":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if( sscanf(var,"%s %s",user,rid)!=2 ) {
				user = var;
				rid = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!rid) { write("命令格式错误！\n"); break; }
			}
			if( !(m=CHANNEL_D->query_ircs(rid)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( !wizardp(me) && member_array(me->query("id"),m["master"]) == -1 ) {
				write("你没有这个权限。\n");
				break;
			}
			if( member_array(user,m["user"])==-1 || !(ob=find_player(user)) ) {
				write("此用户不在此聊天室中。\n");
				break;
			}
			if( member_array(user,m["master"])==-1 ) {
				write("此用户本来就没有帽子。\n");
				break;
			}
			
			m["master"] -= ({ user });
			
			me->set_temp("irc/room_sayin",rid);
			CHANNEL_D->do_channel(me,"irc*","摘掉"+ob->query("name")+"头上的帽子。");
			break;
		case "invite":
			if(vars<2) {
				write("命令格式错误！\n");
				break;
			}
			
			if( sscanf(var,"%s %s",user,rid)!=2 ) {
				user = var;
				rid = (string)me->query_temp(sprintf("irc/room%d",(int)me->query_temp("irc/room")));
				if(!rid) { write("命令格式错误！\n"); break; }
			}
			if( !(m=CHANNEL_D->query_ircs(rid)) ) {
				write("没有这个聊天室。\n");
				break;
			}
			if( !wizardp(me) && member_array(me->query("id"),m["master"]) == -1 ) {
				write("你没有这个权限。\n");
				break;
			}
			if( member_array(user,m["user"])!=-1 ) {
				write("此用户已经在聊天室中了。\n");
				break;
			}
			if( !(ob=find_player(user)) || !me->visible(ob) ) {
				write("没有这个用户。\n");
				break;
			}
			
			ob->set_temp("invite/"+rid,1);
			tell_object(ob , me->query("name")+"邀请你加入 "+m["room_name"]+"("+rid+")，指令：irc /join "+rid+"。\n");
			write("你邀请"+ob->query("name")+"加入 "+m["room_name"]+"("+rid+")。\n");
			
			me->set_temp("irc_busy",1);
			call_out("delete_busy",1,me);
			break;
		default: write("你要使用什么聊天室命令？\n"); return 1;
	}
	
	return 1;
}

int help (object me)
{
	write(@HELP
指令格式: irc /<聊天室命令> <参数>
说明：<>表示必须的参数 []表示非必须的参数

可以接受的聊天室命令有：
irc /list       ：列出目前所以的聊天室。

irc /create <聊天室英文名> <聊天室的中文名字> [no/invite] [聊天室密码]
                ：创建聊天室，需要200k经验(工匠只需要100k经验)，你可以
                  设定聊天室密码，设定是否需要邀请才能加入改聊天室，
                  每人只能创建一个聊天室。

irc /invite <用户名> [聊天室英文名]
                ：邀请用户加入某个聊天室，只有聊天室的管理员和巫师能使用
                  此命令。如果不指定聊天室英文名，缺省将认为指当前所在的
                  聊天室，下同。

irc /join <聊天室英文名> [密码]
                ：加入某个聊天室，可以同时加入五个聊天室。

irc /leave [聊天室英文名]
                ：离开聊天室。

irc /leaveall   ：离开所有加入的聊天室。

irc /listuser [聊天室英文名] 
                : 显示聊天室中的用户。

irc /listroom   ：列出目前所加入的聊天室。

irc /setroom <聊天室英文名>
                ：设定当前聊天室，你只能在当前聊天室中说话。

irc /close [聊天室英文名]
                ：关闭指定的聊天室，只有巫师和聊天室管理员能使用。

irc /kickout <用户名> [聊天室英文名]
                : 把某人踢出聊天室，只有巫师和聊天室管理员能使用。

irc /hat <用户名> [聊天室英文名]
                : 给予某人聊天室管理员权限，只有巫师和聊天室管理员能使用。

irc /unhat <用户名> [聊天室英文名] 
                : 剥夺某人聊天室管理员权限，只有巫师和聊天室管理员能使用。
 
使用：“irc 聊天内容” 命令就可以在指定的聊天室中聊天，且支持表情(emote)，
      在聊天室中聊天至少需要10k经验。

                                                         迎风  AugX@SJ
                                                         9/25/2001
HELP
	);
	return 1;
}
