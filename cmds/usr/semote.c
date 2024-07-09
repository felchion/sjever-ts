// semote.c
// Modified by Numa 1999-12-28 for search emote.

#include "/doc/help.h"

inherit F_CLEAN_UP;

int all_emote(object me); 
int checksemote(object me,string arg);
int femote(object me,string arg);
int show(object me,string str,string arg,int count); 
int flag = 1;

int main(object me, string arg)
{
	if( !arg ) return all_emote(me);
	return checksemote(me,arg);
}

int all_emote(object me)
{
	string *e, out="";
	int i, j = 0;

	e = sort_array(EMOTE_D->query_all_emote(), -1);
	i = sizeof(e);

	while (i--) {
		out += sprintf("%-13s%s", e[i], (j++%6==5)?"\n": "");
	}
	me->start_more(out + (j%6?"\n":""));
	return 1;
}

void remove_busy(object me)
{
	if ( me) me->delete_temp("command_busy");
}

int checksemote(object me, string arg)
{	
	mapping emote;
	string str,*e,str1;
	int i,count;

	if (!wizardp(me)) {
		if (me->query_temp("command_busy"))
			return notify_fail("你正忙着呢。\n");
		me->set_temp("command_busy", 1);
		call_out("remove_busy", 3, me);
		if (me->query("jing") < 100)
			return notify_fail("你的精神欠佳，无法查询。\n");
		me->receive_damage("jing", 100);
	}
	count = 0;
	emote = EMOTE_D->query_emote(arg);
	e = sort_array(EMOTE_D->query_all_emote(), 1);
	str="";

	if ( !arg )
	{
		for(i=0; i<sizeof(e); i++)
		{
			str1=sprintf("%s%s\n%s\n","emote名称：",e[i],EMOTE_D->query_emote(e[i])["others_target"] );
			str+=str1;
		}
	}
	else
	{
		for(i=0; i<sizeof(e); i++)
		{
			if (arg==e[i][0..sizeof(arg)-1]) {
				str1=sprintf("%s%s\n%s%s\n%s%s\n%s%s\n","emote名称：",e[i],
					"对自己：",EMOTE_D->query_emote(e[i])["others_self"],
					"对空气：",EMOTE_D->query_emote(e[i])["others"],
					"对别人：",EMOTE_D->query_emote(e[i])["others_target"] );
				str+=str1;
				count ++;
			}
		}
	} 
	if (str=="") return femote(me,arg);
	flag = 1;
	return show(me,str,arg,count);
}

int femote(object me,string arg)
{
	mapping emote;
	string *e, *loop, *loop1;
	string str="", msg;
	int i,count,k;
	count = 0;

	loop = ({ "others_self", "others", "others_target" });
	loop1 = ({ "自己", "空气", "别人" });

	e = sort_array( EMOTE_D->query_all_emote(), 1);

	for(i=0; i<sizeof(e); i++)
	{
		emote = EMOTE_D->query_emote(e[i]);
		for( int j=0; j<sizeof(loop); j++ ) {
			msg = emote[ loop[j] ];
			if( msg && strsrch(msg, arg) != -1 )
			{
				if (!k++) str += sprintf("emote 名: %s\n", e[i]);
				str += sprintf("对%s：%s\n", loop1[j], msg);
			}
		}
		if( k ) count ++;
		k = 0;
	}

	if(  str == "" ) return notify_fail(MUD_NAME" 中无符合查询条件的 emote.\n");
	else {
		flag = 0;
		return show(me,str,arg,count);
	}
}

int show(object me,string str,string arg,int count)
{
	str =
		"\n查询结果"
		"\n---------------------------------------------------------------\n"
		+ str;
	str = replace_string(str, "0", "无");
	str = replace_string(str, "$n", "(对方)");
	str = replace_string(str, "$N", "(你)");
	str = replace_string(str, "$P", "(你自己)");
	str = replace_string(str, "$p", "(他/她)");
	str = replace_string(str, "$S", "("+RANK_D->query_self(me)+")");
	str = replace_string(str, "$s", "("+RANK_D->query_self_rude(me)+")");
	str = replace_string(str, "$R", "(对方的尊称)");
	str = replace_string(str, "$r", "(对方的辱称)");
	str = replace_string(str, "$C", "("+RANK_D->query_self_close(me)+")");
	str = replace_string(str, "$c", "(对方的呢称)");

	str += "---------------------------------------------------------------\n";
	flag?str += sprintf("在%s中，以“%s”开头的 emote 共有 %d 个。\n",MUD_NAME, arg, count ):str += sprintf("在%s中，包含“%s”的 emote 共有 %d 个。\n",MUD_NAME, arg, count );	
	me->start_more(str);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : semote [参数]

这个指令不加参数时可以列出目前所能使用的emote，加上
参数，则可以搜索emote。
HELP
	);
	return 1;
}
