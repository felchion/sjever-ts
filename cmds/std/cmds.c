// cmds.c 查看玩家可使用的命令   等级为 wiz(命令没必要判断了)   ls cmds/*** 都能知道
// by Lane@LN 24/05/2004

#include <ansi.h>
inherit F_CLEAN_UP;

int sort_file(string file)
{
	int i;
	if( file[<2..<1] != ".c" ) return 0;
	if( !sscanf(file, "%*s.c") )  return 0;
	if( sscanf(file, "%*s_noshow.c") )  return 0;
	i = strlen(file);
	while(i--){
		if( file[i] == '.' ) continue;
		if( file[i] < 'a' || file[i] > 'z' )
			return 0;
	}

	return 1;
}

int main(object me)
{

	string str, *file;
	int c, i, j = 0;

	seteuid(getuid());

	str = "\n────────────────────────────────────\n";
	str += HBBLU"                            "HIW"【书剑命令检查】"NOR+HBBLU"                            \n"NOR;
	str += "────────────────────────────────────\n";
	str += HIW"附：绝大部分的指令，您都可以通过 help "RED"(显示的命令) "HIW"来查询其功能用法。\n"NOR;
	file = get_dir("cmds/skill/");
	if( sizeof(file) > 0){
		file  = filter_array(file, (: sort_file :) );
		j = sizeof(file);
	}

	if(j && j > 0){
		str += "\n────────────────────────────────────\n";
		str += HBBLU"                            "HIW"【常用武功命令】"NOR+HBBLU"                            \n"NOR;
		str += "────────────────────────────────────\n";
		for( i=0; i < j; i++ ) {
			str += sprintf("\t%-15s",HIY + replace_string(file[i], ".c", "")+NOR);
			c = c + 1;
			if( c > 3 ) {
				str += "\n";
				c = 0;
			}
		}
		str += "\n";
		j = 0;
		c = 0;
	}

	file = get_dir("cmds/std/");
	if( sizeof(file) > 0){
		file  = filter_array(file, (: sort_file :) );
		j = sizeof(file);
	}

	if(j && j > 0){
		str += "\n────────────────────────────────────\n";
		str += HBBLU"                            "HIW"【常用冒险命令】"NOR+HBBLU"                            \n"NOR;
		str += "────────────────────────────────────\n";
		for( i=0; i < j; i++ ) {
			str += sprintf("\t%-15s",HIY + replace_string(file[i], ".c", "")+NOR);
			c = c + 1;
			if( c > 3 ) {
				str += "\n";
				c = 0;
			}
		}
		str += "\n";
		j = 0;
		c = 0;
	}
	file = get_dir("cmds/group/");
	if( sizeof(file) > 0){
		file  = filter_array(file, (: sort_file :) );
		j = sizeof(file);
	}
	if(j && j > 0){
		str += "\n────────────────────────────────────\n";
		str += HBBLU"                            "HIW"【常用帮派命令】"NOR+HBBLU"                            \n"NOR;
		str += "────────────────────────────────────\n";
		for( i=0; i < j; i++ ) {
			str += sprintf("\t%-15s",HIY + replace_string(file[i], ".c", "")+NOR);
			c = c + 1;
			if( c > 3 ) {
				str += "\n";
				c = 0;
			}
		}
		str += "\n";
		j = 0;
		c = 0;
	}
	
	file = get_dir("cmds/usr/");
	if( sizeof(file) > 0){
		file  = filter_array(file, (: sort_file :) );
		j = sizeof(file);
	}

	if(j && j > 0){
		str += "\n────────────────────────────────────\n";
		str += HBBLU"                            "HIW"【常用系统命令】"NOR+HBBLU"                            \n"NOR;
		str += "────────────────────────────────────\n";
		for( i=0; i < j; i++ ) {
			str += sprintf("\t%-15s",HIY + replace_string(file[i], ".c", "")+NOR);
			c = c + 1;
			if( c > 3 ) {
				str += "\n";
				c = 0;
			}
		}
		str += "\n";
		j = 0;
		c = 0;
	}
	

	str += "────────────────────────────────────\n";
	me->start_more(str);
	return 1;
}

int help(object me)
{
	"/cmds/std/cmds"->main(me);
	return 1;
}


