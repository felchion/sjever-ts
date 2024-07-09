// nick.c

#include <database.h>
#include <ansi.h>

public string query_version(object ob)
{
	if(ob==find_object(BBS_D)) return "1.0";
}

string delansi(string str)
{
   int is_ansi,ansi_len,i;
   //string ruslt;
   is_ansi=0;
   ansi_len=0;
   
   for (i=0;i<strlen(str);i++){     
     if (str[i]==27)
    	 is_ansi=1;
     if (str[i]==109 && is_ansi==1){ 
     	 str[i]=37;
     	 is_ansi=0;
     }
     if (is_ansi==1){
     	 ansi_len++;
       str[i]=37;
     }     
   }
	 return replace_string(str,"%","");	
}

int main(object me, string arg)
{
	string nickname,new_name;
	int i;
	
	if( !arg )
		return notify_fail(me->query("nickname")?"你目前的绰号是：「"+me->query("nickname")+"」\n":"你目前没有任何绰号。\n");
	
	if (arg == "none") {
      		me->delete("nickname");
      		write("您的绰号被删除了。\n");
//      		if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "UPDATE members SET nickname = 
//		'' WHERE username = '"+
//		me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"'"))
//		
//      		write("您在论坛的绰号自动删除！\n");
      		return 1;
    	}
    	
    	arg = explode(arg+"\n", "\n")[0];
	arg = replace_string(arg, "$RED$", RED);
	arg = replace_string(arg, "$GRN$", GRN);
	arg = replace_string(arg, "$YEL$", YEL);
	arg = replace_string(arg, "$BLU$", BLU);
	arg = replace_string(arg, "$MAG$", MAG);
	arg = replace_string(arg, "$CYN$", CYN);
	arg = replace_string(arg, "$WHT$", WHT);
	arg = replace_string(arg, "$HIR$", HIR);
	arg = replace_string(arg, "$HIG$", HIG);
	arg = replace_string(arg, "$HIY$", HIY);
	arg = replace_string(arg, "$HIB$", HIB);
	arg = replace_string(arg, "$HIM$", HIM);
	arg = replace_string(arg, "$HIC$", HIC);
	arg = replace_string(arg, "$HIW$", HIW);
	arg = replace_string(arg, "$NOR$", NOR);
	
	new_name = delansi(arg);
	i = strlen(new_name);




	if(!is_chinese(new_name) ) {
		return notify_fail("你的绰号只能用中文！\n");
	}
	
	if (!strlen(strip(arg))) return notify_fail("你到底输入的是什么内容啊？\n");
	if (strwidth(strip(arg)) > 12 && !wizardp(me)) return notify_fail("你的绰号太长了，想一个短一点的、响亮一点的。\n");

	me->set("nickname", arg + NOR);
	nickname = specialchars(strip(me->query("nickname")));
	nickname = strip(me->query("nickname"));
	write("恭喜！您现在的新绰号是：「"+me->query("nickname")+"」\n");
//	if (BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "UPDATE members SET nickname = 
//	'"+nickname+"' WHERE username = '"+
//	me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"'"))
	/*
	write("UPDATE members SET nickname = 
	'"+strip(me->query("nickname"))+"' WHERE username = '"+
	me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"'\n");
	*/		
	/*BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "
	INSERT INTO mud_nick (utime, content, name, userid, type, site) 
	values (
	'" + time() +"',
	'" + me->query("nickname") + "',
	'" + me->query("color") + me->query("name") +"',
	'" + me->query("id") + "',
	'nick',
	'"+ lower_case(INTERMUD_MUD_NAME) + "')");*/
	
//	write("您在论坛的绰号自动更新！\n");
	return 1;
}
int help(object me)
{
        write(@HELP
指令格式 : nick <外号>|none
 
这个指令可以让你为自己取一个响亮的名号或头衔，nick none 为删除。
你如果希望在绰号中使用 ANSI 的控制字元改变颜色，可以用以下的控制字串：

HELP
"                        $NOR$ - 恢复正常颜色\n"
"$RED$ - "RED"红色"NOR"		$HIR$ - "HIR"亮红色\n"NOR
"$GRN$ - "GRN"绿色"NOR"		$HIG$ - "HIG"亮绿色\n"NOR
"$YEL$ - "YEL"土黄色"NOR"		$HIY$ - "HIY"黄色\n"NOR
"$BLU$ - "BLU"深蓝色"NOR"		$HIB$ - "HIB"蓝色\n"NOR
"$MAG$ - "MAG"浅紫色"NOR"		$HIM$ - "HIM"粉红色\n"NOR
"$CYN$ - "CYN"蓝绿色"NOR"		$HIC$ - "HIC"天青色\n"NOR
"$WHT$ - 浅灰色		$HIW$ - "HIW"白色\n"NOR
@HELP 

其中系统自动会在字串尾端加一个 $NOR$。
HELP
);
        return 1;
}
