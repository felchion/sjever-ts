
//add 查看quest skill by tangfeng 2005

#include <ansi.h>
inherit F_CLEAN_UP;
#define BUFFER_SIZE     1024
void searchstr(object me,string file,string str,int location,string buf);
void donext(string arg,object me,string file,string str,int location,string buf);
string sprint_list(object ob_list,string str);
int sort_user(object ob1, object ob2);
int clear_user(object ob);
int help(object me);

//查看skill
int chakan_skill(object me,int k);

mapping *quest_skill = ({
([      "location" : "quest/天龙八部/武功/pass",
        "name" : HIY"凌波微步"NOR,
        "level" : 1,
		"logfile":"/log/quest/TLBB",
		"string":"解开凌波微步",
]),
([      "location" : "quest/连城诀/武功/shenzhao",
        "name" : HIB"神照经"NOR,
        "level" : 2,
		"logfile":"/log/quest/LCHJ",
		"string":"学到神照经绝技",
]),
([      "location" : "quest/连城诀/武功/sword",
        "name" : HIC"躺尸剑法"NOR,
        "level" : 3,
		"logfile":"/log/quest/LCHJ",
		"string":"学到躺尸剑法",
]),
([      "location" : "quest/雪山飞狐/武功/hujiadaofa",
        "name" : HIW"胡家刀法"NOR,
        "level" : 2,
		"logfile":"/log/quest/FEIHU",
		"string":"揭开胡家刀法",
]),
([      "location" : "quest/雪山飞狐/武功/miaojiajianfa",
        "name" : HIM"苗家剑法"NOR,
        "level" : 2,
		"logfile":"/log/quest/FEIHU",
		"string":"揭开苗家剑法",
]),
([      "location" : "quest/雪山飞狐/武功/lengquanshengong",
        "name" : HIB"冷泉神功"NOR,
        "level" : 2,
		"logfile":"/log/quest/FEIHU",
		"string":"解开冷泉神功",
]),
([      "location" : "double_attack",
        "name" : HIR"双手互搏"NOR,
        "level" : 1,
		"logfile":"/log/quest/jiebai",
		"string":"从周伯通处领悟到双手互搏",
]),
([      "location" : "quest/jiuyin2/pass",
        "name" : HIY"九阴下篇"NOR,
        "level" : 2,
		"logfile":"/log/quest/jiuyin",
		"string":"次后，从陈玄风处得到九阴真经下卷",
]),
([      "location" : "quest/jiuyin1/pass",
        "name" : HIR"九阴全篇"NOR,
        "level" : 1,
		"logfile":"/log/quest/jiuyin",
		"string":"得到九阴真经上册",
]),
([      "location" : "mao18/pass",
        "name" : HIY"五虎断门刀"NOR,
        "level" : 3,
		"logfile":"/log/quest/wuhu",
		"string":"从茅十八处学得五虎断门刀",
]),
([      "location" : "oyf/hamagong",
        "name" : HIR"蛤蟆功"NOR,
        "level" : 1,
		"logfile":"/log/quest/hamagong",
		"string":"成为欧阳峰义子",
]),
([      "location" : "quest/qianzhu/pass",
        "name" : HIR"千珠万毒手"NOR,
        "level" : 2,
		"logfile":"/log/quest/qianzhu",
		"string":"后，解开千蛛万毒手",
]),
([      "location" : "jinshe/jianfa",
        "name" : HIY"金蛇剑法"NOR,
        "level" : 2,
		"logfile":"",
		"string":"",
]),
([      "location" : "jinshe/zhangfa",
        "name" : HIY"金蛇掌法"NOR,
        "level" : 3,
		"logfile":"",
		"string":"",
]),
([      "location" : "quest/pixie/pass",
        "name" : HIW"辟邪剑法"NOR,
        "level" : 2,
		"logfile":"/log/quest/pixie",
		"string":"后，获得「葵花宝典」",
]),
([      "location" : "xmsz",
        "name" : HIW"玄冥神掌"NOR,
        "level" : 2,
		"logfile":"",
		"string":"",
]),
});


int main(object me, string arg)
{
	object ob;
	string str;

	str="\n";
	if (!wizardp(me))
		return notify_fail("只有巫师能察看。\n");
	if (!arg)
		chakan_skill(me,0);
	else 
	{

			ob = present(arg, environment(me));
			if (!ob) ob = LOGIN_D->find_body(arg);
			if (!ob) ob = find_living(arg);
			if (!ob || !me->visible(ob)) return notify_fail("你要察看谁的状态？\n");
			if(wizardp(ob)) return notify_fail("你不可以查询巫师状态\n");
			me->set_temp("quest/chakanobject",ob);
			chakan_skill(me,1);
	}
	return 1;
}

int chakan_skill(object me,int k)
{
	string str;
	object *ob_list,obj;
	int i,j,offset,x=1;
	string file;

    if(k==0)
	{
		str =  "\n      ┏ "+HIY"在线解密武功查询榜"NOR+" ┓\n";
		str += "┏━━┻━━━━━━┯━━━┻┯━━━━━━┓\n";
		str += "┃     名     字    │ 门  派 │  武功名称  ┃\n";
		ob_list = filter_array(children(USER_OB), (: userp :));
		ob_list = filter_array(ob_list, (: !wizardp($1) :));
		ob_list = filter_array(ob_list, (: clear_user :));
		//ob_list = filter_array(ob_list, (: sort_user :));

		for(i=0; i <sizeof(ob_list); i++)
		{

			str += "┠─────────┴────┴──────┨\n";
			for(j = sizeof(quest_skill)-1; j >= 0; j--)
			{
				if(ob_list[i]->query(quest_skill[j]["location"] ))
					str += sprint_list(ob_list[i],quest_skill[j]["name"]);
			}

		}
		if(!sizeof(ob_list)) 
		{
			str += "┠─────────┴────┴──────┨\n";
			str+= "┠────没有发现任何解密武功的存在────┨\n";
		}
		str += "┗━━━━━━━━━━━━━━━━━━━━━┛\n";  
		tell_object(me,str);
	}
	else
	{
        obj=me->query_temp("quest/chakanobject");
		if(!obj) return 1;
		str =  "\n"+HIY+obj->query("name")+"武功查询榜"NOR+"\n";
		str += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		str += "门派："+obj->query("family/family_name");	
		str += "\n────────────────────────────────────────\n";
        x=0;
		for(i = sizeof(quest_skill)-1; i >= 0; i--)
		{
			if(obj->query(quest_skill[i]["location"]))
			{
				str +=""+quest_skill[i]["name"]+"\n";	
				x=1;
			}
			
		}
		if(x==0)
			str+= "没有发现任何解密武功的存在───────────────────────────\n";
		str += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"NOR;
		tell_object(me,str);
		if(x==1)
		{
			for(i = sizeof(quest_skill)-1; i >= 0; i--)
			{
				if(obj->query(quest_skill[i]["location"] ))
				{
					if(quest_skill[i]["logfile"]=="")
						tell_object(me,"可惜没有"+quest_skill[i]["name"]+"的Log纪录。\n");
					else
					{
						seteuid(geteuid(me));
						file = quest_skill[i]["logfile"];
						if( (offset=file_size(file))>=0 ) 
							searchstr(me,file,quest_skill[i]["string"],file_size(file),"");
					}
				}
			}
			str = NOR"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"NOR;
			tell_object(me,str);
		}
		me->delete_temp("quest/chakanobject");
	}
    return 1;
}
int clear_user(object ob)
{
	int i;
	for( i = 0; i < (sizeof(quest_skill)-1); i++)
	{
        if(ob->query(quest_skill[i]["location"]))
			return 1;
	}
	return 0;	
}
int sort_user(object ob1, object ob2)
{
	string name1, name2;
	int l1 = wiz_level(ob1), l2 = wiz_level(ob2);

	if (l1 != l2) return l2 - l1;

	if (l1) {
		name1 = ob1->query("id");
		name2 = ob2->query("id");

		if (intp(name1)) name1 = "";
		if (intp(name2)) name2 = "";
		if (name1 > name2) return -1;
		if (name1 < name2) return 1;
		return 0;
	}

	reset_eval_cost();

	name1 = ob1->query("family/family_name");
	name2 = ob2->query("family/family_name");
	if (intp(name1)) name1 = "";
	if (intp(name2)) name2 = "";
	if (name1 > name2) return -1;
	if (name1 < name2) return 1;

	l1 = ob1->query("family/generation");
	l2 = ob2->query("family/generation");
	if (l1 != l2) return l1 - l2;

	return (int)ob2->query("combat_exp") - (int)ob1->query("combat_exp");
}

string sprint_list(object ob_list,string str)
{
	string msg="";
	if(!str)
		str="无名神功";
	msg = sprintf("┃  %8s%-10s  %|8s  %|8s  " NOR "┃\n",
			ob_list->query("name") ,
			"("+capitalize(ob_list->query("id"))+")",
			ob_list->query("family/family_name"),
			str);
	return msg;
}
void searchstr(object me,string file,string str,int location,string buf)
{
        string * s;
        int i,j,count;
		object obj;
		obj=me->query_temp("quest/chakanobject");
		if(!obj) return;

        count = 0;
        while (1) {
                i = location;
                location -= BUFFER_SIZE;
                if (location<0)
                        location=0;
                else
                        i = BUFFER_SIZE;
                s = explode(read_bytes(file,location,i)+buf,"\n");
                buf = s[0]+"\n";
                        
                if (location<=0) j=0; else j=1;
                for(i=sizeof(s)-1;i>=j;i--) {
                        if( strsrch(s[i],str,1)>=0 && strsrch(s[i],obj->query("name"),1)>=0) {
                                tell_object(me,s[i]+"\n");
                                count++;        
                        }
                }

                if (location<=0) 
                        return;

				if (count>20) {
                        count = 0;
                        tell_object(me,"== 未完继续 == (ENTER 继续下一页，q 离开)");
                        input_to("donext",me,file,str,location,buf);
                        return;
                }
        }
        return;
} 
void donext(string arg,object me,string file,string str,int location,string buf)
{
        if( arg[0]!='q' )
                searchstr(me,file,str,location,buf);
        else
                tell_object(me,"查询终止。\n");
}

int help(object me)
{
	write("
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
          指令格式 : "+YEL"checkquestskill <......>"NOR"
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"YEL"
       checkquestskill  巫师查看所有玩家quest skill情况
checkquestskill <玩家>  巫师查看某玩家quest skill纪录\n"NOR"
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"NOR);
return 1;
}
