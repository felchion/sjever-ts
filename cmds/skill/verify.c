// verify.c
// Modified by snowman@SJ 16/03/2000

/* 闲来无聊，写了个查询武功的命令。个人比较喜欢有框架的图形，各位多包涵。
   PS: 在没有优化过 ansi 显示的OS里，带颜色的框架会乱序排列，去掉code里的个ansi color就行了。
   
	用 verify xxx.yyy 是 call yyy 文件里的 int help(object me)，可以按照其它
	command 的help()来写。
   本想再加点其它功能的，但觉得过于花哨了反而不好，就这样吧。
   附太极拳的help()以做参考：

int help(object me)
{
	write(WHT"\n太极拳："NOR"\n");
	write(@HELP
	太极拳，为宋末元初武当张三丰真人所创，合道家之说，冲虚自然，主以慢
	打快，以静制动，後发制人，无为至虚之念的上乘武学。拳出时双手圆转，
	每一招都含着太极式的阴阳变化，精微奥妙，实是开辟了武学中从所未有的
	新天地。诀窍为「虚灵顶劲、涵胸拔背、松腰垂臀、沉肩坠肘」十六个字，
	纯以意行，最忌用力。形神合一，是这路拳法的要旨。
	太极拳劲首要在似松非松，将展未展，劲断意不断，其接力打力为武林一绝。
	
	要求：  氤氲紫气等级 100 以上；
		内力 500 以上；      
		有张三丰亲传拳招。
HELP
	);
	return 1;
}

*/

#include <ansi.h>
#include <level.h>
inherit F_CLEAN_UP;

string *skill_level_desc = ({ LEVEL });

string *knowledge_level_desc = ({
	BLU "新学乍用" NOR,HIB "初窥门径" NOR,MAG "略知一二" NOR,HIM "半生不熟" NOR,
	CYN "马马虎虎" NOR,YEL "已有小成" NOR,YEL "融会贯通" NOR,HIY "心领神会" NOR,
	HIY "了然於胸" NOR,HIG "已有大成" NOR,HIG "非同凡响" NOR,HIC "举世无双" NOR,
	HIC "震古铄今" NOR,HIW "无与伦比" NOR,HIW "超凡入圣" NOR,HIR "空前绝后" NOR
});

mixed skill_class_map = ({
// 无   正   奇   刚   柔   快   慢   妙   险   空
({100, 100, 100, 100, 100, 100, 100, 100, 100, 150}), //无
({100, 100, 100,  80, 125,  67, 150,  50, 200,  67}), //正
({100, 100, 100, 125,  80, 150,  67, 200,  50,  67}), //奇
({100, 125,  80, 100, 100,  50, 200,  67, 150,  67}), //刚 
({100,  80, 125, 100, 100, 200,  50, 150,  67,  67}), //柔
({100, 150,  67, 200,  50, 100, 100,  80, 125,  67}), //快
({100,  67, 150,  50, 200, 100, 100, 125,  80,  67}), //慢
({100, 200,  50, 150,  67, 125,  80, 100, 100,  67}), //妙
({100,  50, 200,  67, 150,  80, 125, 100, 100,  67}), //险
({ 67, 150, 150, 150, 150, 150, 150, 150, 150, 100})  //空
});

mapping valid_type = ([
"unarmed":      "拳脚",
"claw":	 "爪法","cuff":	 "拳法",
"finger":       "指法","strike":       "掌法",
"hand":	 "手法","leg":	  "腿法",
"sword":	"剑法","blade":	"刀法",  
"dagger":       "匕法","brush":	"笔法",	  
"spear":	"枪法","hook":	 "钩法",	  
"stick":	"棒法","staff":	"杖法",	  
"club" :	"棍法","throwing":     "暗器",	  
"whip":	 "鞭法","axe":	  "斧法",
"hammer":       "锤法","force":	"内功",  
"dodge":	"轻功","parry":	"招架",  
]);

string skill_level(string, int);
string skill_enables(object sk);
string skill_class_list(string skill_class);
string get_color(int p);

int sort_skill(string file)
{
	int i;
	if ( file[<2..<1] != ".c" ) return 0;
	if ( !sscanf(file, "%*s.c") )  return 0;
	if ( sscanf(file, "%*s_noshow.c") )  return 0;
	i = strlen(file);
	while(i--){
		if( file[i] == '.' ) continue;
		if( file[i] < 'a' || file[i] > 'z' )
			return 0;
	}

	return 1;
}

string skill_class_list(string skill_class) {
	string result="",list = "无正奇刚柔快慢妙险空";
  int i,j;
  
  i = strsrch(list,skill_class)/2;
  if (i==-1) i=0;
  for (j=0;j<10;j++) {
  	result += get_color(skill_class_map[i][j]);  	
  	result += list[j..j];
  	result += "    "NOR;
  }
  return result;
}

string get_color(int p) {
  if (p == 50)  return CYN;
  if (p == 67)  return HIB;
  if (p == 80)  return HIC;
  if (p == 125) return RED;
  if (p == 150) return HIM;
  if (p == 200) return HIR;
  return WHT;
}

int main(object me, string arg)
{
	mapping lrn;
	string exert, str, *file, pfm;
	string sk_class;
	object sk,ob;
	int ap,i, j = 0;

	seteuid(getuid());

	if(!arg) return notify_fail("指令格式 : verify [武功名称].[?|特殊技能]\n");
	
	sscanf(arg, "%s.%s", arg, str);
		
	if(!wizardp(me) && !me->query_skill(arg, 1)) 
		return notify_fail("你要查对什么技能？\n");
	
	if(file_size(SKILL_D(arg)+".c") < 1)
		return notify_fail("错误！你要查对技能文件已经消失，请通知巫师。\n");
	
	if( stringp(str) ){
		// 格式 verify xxx.? 是查询此skill的帮助。
		// verify xxx.yyy 是查询 yyy 的帮助。
		if( str == "?" ){
			notify_fail("对不起，「"+to_chinese(arg)+"」目前还没有详细的帮助说明。\n");
			return SKILL_D(arg)->help(me);
		}
		else if( stringp(exert = SKILL_D(arg)->exert_function_file("")) && file_size(exert + str +".c") > 0){
			notify_fail("对不起，"+to_chinese(arg)+"内功方面的「"+str+"」功能没有详细的说明。\n");
			return (exert + str)->help(me);
		}
		else if( stringp(exert = SKILL_D(arg)->perform_action_file("")) && file_size(exert + str +".c") > 0){
			notify_fail("对不起，"+to_chinese(arg)+"外功方面的「"+str+"」功能没有详细的说明。\n");
			return (exert + str)->help(me);
		}
		return notify_fail("对不起，"+to_chinese(arg)+"没有「"+str+"」这项功能。\n");
	}
	
	lrn = me->query_learned();
	if( !mapp(lrn) ) lrn = ([]);
	sk_class = SKILL_D(arg)->query_class();
	str = sprintf("\n┌%-24s─────────────────────┐\n", HIW"【"+to_chinese(arg)+"功能表】"NOR);
	
	str = replace_string(str, "  ", "─");
	str += sprintf("│"HIY"等级"NOR"：%s %-7d        "HIY"类别"NOR"：%-17s"HIY"特性"NOR"：%5s%2s\n",skill_level(SKILL_D(arg)->type(), 
			me->query_skill(arg, 1)), me->query_skill(arg, 1),to_chinese(SKILL_D(arg)->type()),"「"+(sk_class?sk_class:"无")+"」", "│"); 
	
	if(SKILL_D(arg)->type() != "martial"
	|| member_array(arg, keys(valid_type))!=-1) 
		return notify_fail(str+"└─────────────────────────────────┘\n");

	str += "│                                                                  │\n";
	sk = load_object(SKILL_D(arg));
	
	exert = skill_enables(sk);
	if(exert != ""){
		str += exert;
		i++;
	}
	
	if( stringp(exert = SKILL_D(arg)->exert_function_file("")) ){
		file = get_dir(exert);
		if( sizeof(file) > 0){
			file  = filter_array(file, (: sort_skill :) );
			j = sizeof(file);
		}
	}
	
	if(j && j > 0){
		if( i )
			str = str + "├─────────────────────────────────┤\n";
		str += "│"HIY"内功方面(exert + )：                                       合气度 "NOR"│\n";
		for(i=0; i < j; i++){
			ap = call_other(exert + file[i], "query_ap");
			if (objectp(ob=new(exert + file[i]))) {
			  if (!function_exists("exert",ob)) {
			  	destruct(ob);
			  	continue;
			  }
			  else destruct(ob);
			}
			if (!ap) ap = 1;
			pfm = call_other(exert + file[i], "exert_name");
			str += sprintf("│      %-14s %-38s "HIC"%4d"NOR"  │\n",YEL+replace_string(file[i], ".c", "")+NOR, 
				pfm?WHT"「"NOR + pfm + WHT"」"NOR:"",ap<40?ap*40:ap);
		}
		j = 0;
	}
	
	if( stringp(exert = SKILL_D(arg)->perform_action_file(""))){
		file = get_dir(exert);
		if( sizeof(file) > 0){
			file  = filter_array(file, (: sort_skill :) );
			j = sizeof(file);
		}
	}

	if(j && j > 0){
		str = str + "├─────────────────────────────────┤\n";
		str += "│     "+skill_class_list(sk_class?sk_class:"无")+" │\n";
		if( i )
			str = str + "├─────────────────────────────────┤\n";
		str += "│"HIY"外功方面(perform + )：                                     合气度 "NOR"│\n";
		for(i=0; i < j; i++){
			ap = call_other(exert + file[i], "query_ap");
			if (objectp(ob=new(exert + file[i]))) {
			  if (!function_exists("perform",ob)) {
			  	destruct(ob);
			  	continue;
			  }
			  else destruct(ob);
			}
			if (!ap) ap = 2;
			pfm = call_other(exert + file[i], "perform_name");
			str += sprintf("│      %-14s %-38s "HIC"%4d"NOR"  │\n",WHT+replace_string(file[i], ".c", "")+NOR, 
				pfm?WHT"「"NOR + pfm + WHT"」"NOR:"",ap<40?ap*40:ap);		  
		}
	}
	str += "└─────────────────────────────────┘\n";
	write(str);
	return 1;
}

string skill_enables(object sk)
{
	int i, j;
	string str, *skills;
	skills = keys(valid_type);
	
	for(i=0; i < sizeof(skills); i++) {
		if (sk->valid_enable(skills[i])) continue;
		skills[i] = 0;
	}
	
	skills -= ({ 0 });
	j = sizeof(skills);
	if( !j || j < 1)
		return "";
	str = "│"HIY"特殊技能"NOR"：                                                        ";
		
	for(i=0; i < j; i++) {
		if (i % 4 == 0) str += "│\n│        ";
		str += sprintf("%-14s ", HIC+valid_type[skills[i]]+"("+skills[i]+")"NOR);
		if ( i > j-2 ){
			switch(i%4){
				case 0: str +="                                           "; break;
				case 1: str +="                            "; break;
				case 2: str +="             "; break;
				default: break;
			}
		}
	}
	str += "│\n";
	return str;
}

string skill_level(string type, int level)
{
	int grade;
	
	switch(type) {
		case "knowledge":
			grade = level / 20;
			if( grade >= sizeof(knowledge_level_desc) )
				grade = sizeof(knowledge_level_desc)-1;
			return knowledge_level_desc[grade];
		default:
			//grade = level / 7;
			//but now system the player combat speed is very fast.//by yeju.
			grade = level / 11;
			if( grade >= sizeof(skill_level_desc) )
				grade = sizeof(skill_level_desc)-1;
			return skill_level_desc[grade];
	}
}

int help(object me)
{
	write("\n指令格式 : verify [武功名称].[绝招名称 | ?]\n");
	write(@HELP
这个指令可以让你查询此项技能的功能和数据。
特殊技能表示可以 enable 的技能，如：
(轻功、招架、棍法、杖法) 表示：此武功可以用作轻功，招架，棍法，杖法等。

内功方面显示可运用(exert)的内功。
外功方面显示可使用(perform)的外功，绝招等。
如果内功或外功的绝招有帮助介绍，可用 verify skill.pfm 的形式来查看。
如：
	verify taiji-quan.dongjing  查看动静决
	verify yinyun-ziqi.yinyun   查看氤氲内聚
	verify taiji-quan.?	    查看太极拳

HELP);

    return 1;
}
