//add by tangfeng
//2005.3
#include <ansi.h>
#define Description "一座小小的阁楼，以竹为魂，篱为魄，珠玉为阁，凝香为路，地上几方\n看上去有点凌乱的草席，还有一扇黑洞洞的小草门。几只蝴蝶五彩斑斓，在\n楼宇之间飞来飞去。墙上挂着一个草八卦（"+HIR"cao bagua"NOR+"）。角落里放着一个\n石坛（"+HIW"shitan"NOR+"），上面模糊着有些什么似的。\n"
#define BOOK    "/clone/gift/book"      //书
#define MIC     "/clone/gift/danwan"    //药
#define MON     "/clone/gift/money"     //钱
#define JADE    "/d/zhiye/obj/othermaterial" //玉
#define DAZAO   "/clone/gift/imbue"     //打造机会

inherit ROOM;
string look_bagua();
string look_shitan();
int do_qidong(string arg);
void destruct_person();
void copy_npc(object me,int x);
void random_gift(object me);
int sort_user(object ob1, object ob2);
#define Score_Limit 3000
#define Gift_Limit 800

#include "/d/city/diemenglou_poison.h";
#include "/d/city/diemenglou_party.h";
#include "/d/city/diemenglou.h";

void create()
{
	set("short",HIW"蝶梦楼二楼"NOR);
	set("long",Description);
	set("exits",([
		//"up" : __DIR__"diemenglou3",
		"down" : __DIR__"diemenglou1",
	]));
	set("item_desc",([            
            "cao bagua" : (: look_bagua :),
            "shitan" : (: look_shitan :),
        ]));
	set("quest",1);
	set("no_death",1);
	set("no_get_from", 1); 
	set("no_sleep_room", 1);
	set("coor/x",80);
	set("coor/y",40);
	set("coor/z",0);
	setup();
}
void init()
{
	object me;
	me = this_player();
	if(userp(me) && !wizardp(me)
		&& (me->query_condition("killer")
		    ||me->query_condition("job_busy")
			|| !me->query_temp("quest/special/second_floor")
			|| !me->query_temp("quest/special/up")))
	{
		me->remove_all_killer();
		me->revive();
		me->reincarnate();
		message_vision(HIC"\n突然$N眼前一花，周边的一切如梦如幻。$N不禁痴呆半响。\n"NOR,me); 
		me->move("/d/city/lxs");
		message_vision(HIY"$N只觉得一阵朦胧，竟然似乎是飘飘然的离开了那个小楼。\n"NOR, me);
		me->delete_temp("quest/special");
		me->start_busy(2);
		return;
	}
	add_action("do_qidong", ({"qidong"}));
	add_action("do_liaoshang", ({"liaoshang"}));
	add_action("do_chakan", ({"chakan"}));
	add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
                              "kill", "fight", "study", "practice", "exercise", "halt", "ask","whisper",
	                          "quit", "save", "throw", "suicide"}));
	if(!wizardp(this_player()) && userp(this_player()))
	{
		if(me->query_temp("quest/special/second_floor"))
			call_out("must_dream",360,me);
		else
			call_out("must_dream",1,me);
	}
}
void must_dream(object me)
{
	if(!me) return;
	if(!userp(me)) return;
	if(!me->query_temp("quest/special/second_floor") || !me->is_fighting())
	{
		me->remove_all_killer();
		me->reincarnate();
		message_vision(HIC"\n突然$N眼前一乱，似乎无意中$N启动了什么阵法，不禁痴呆半响。\n"NOR,me); 
		message_vision(HIY"$N只觉得一阵朦胧，竟然似乎是飘飘然的离开了梦蝶二楼。\n"NOR, me);
		me->move(__DIR__"diemenglou1");
		message_vision("$N急急忙忙地来到了"+environment(me)->query("short")+"。\n", me);
        me->start_busy(1);
		me->delete_temp("quest/special/second_floor");
		set("long",Description);
		remove_call_out("again_check");
		call_out("again_check",3,me);//为了防止dazhuan 类似的pfm 出现兵器留在上面的bug
		return;
	}
}
int do_action(string arg)
{
	if(!wizardp(this_player())){
		write("梦蝶二楼只能比武，请不要做不相关的事！\n");
		return 1;
	}
	else return 0;
}
string look_bagua()
{
    	object me;
		string str;
    	me = this_player();
		if(me->is_fighting()) 
			return "你正忙着呢。\n";
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";
    	me->start_busy(2);
		    str  = HIR"\n               五湖四海任逍遥，阴阳八卦藏玄机。\n"NOR;

			str += HIW"\n你可以在这里使用 qidong指令启动八卦阵法，尝试书剑中武学的搭配与阵\n"NOR;
			str += HIW"法的妙用。目前开放的指令格式有：\n"NOR;
			str += HIY"\nqidong |<门派>|<人数>|<难度等级>|<空手是否>|"NOR+"："+HIM"与指定门派组阵战斗\n"NOR;
			str += RED"\n门派编号为：\n"NOR;
		    str += WHT"1--武当 2--华山 3--嵩山 4--古墓 5--丐帮 6--铁掌 7--大轮寺 8--慕容\n"NOR;
			str += WHT"9--神龙岛 10--星宿 11--峨嵋 12--明教 13--桃花岛 14--天龙 15--少林\n"NOR;
			str += WHT"16--昆仑 17--九阴大全 18--蛤蟆功 19--辟邪剑 20--金蛇剑\n"NOR;
			str += HBRED"\n难度等级：1-9。空手设定为1，兵器为2，随机为3。阵法人数上限10人。\n"NOR;
        tell_object(me,str);
    	return "";
}
string look_shitan()
{
    	object me;
		string str;
    	me = this_player();
		if(me->is_fighting()) 
			return "你正忙着呢。\n";
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";
    	me->start_busy(2);
			str = HBYEL"似乎有好事者曾于此地穷其一生研究，意外发现几个秘密，特刻如此：\n"NOR;
			str += CYN"如果气或精血低于1/3，可以使用"+HIC"liaoshang"NOR+CYN"命令就地疗伤给予一定恢复。\n";
			str += "如果使用"+HIC"chakan"NOR+CYN"命令，可以让你了解你蝶梦积分、当前积分和奖励情况。\n";
			str += ""+HIC"chakan top"NOR+CYN"可以查看前十名蝶梦高手。巫师可以使用"+HIC"chakan <player>"NOR+CYN"\n"NOR;
        tell_object(me,str);
    	return "";
}
int do_qidong(string arg)
{
	object me = this_player();
	int i,p,j,k;
	int a=1,b=0,c=100,d=9,e=0,f=0;
	// a  高级/低级武功 b 空手/兵器 c pfm几率 d 经验加成 e hubo f lbwb
	if(me->query_temp("killer")) 
		return notify_fail(RED"你似乎和官府正有些过节。\n"NOR);
	if(!me->query_temp("quest/special/up")) 
        return notify_fail(RED"你无法启动阵法。\n"NOR);
	if(!me->query_temp("quest/special/second_floor")) 
        return notify_fail(RED"你无法启动阵法。\n"NOR);

    if(me->is_fighting()) 
        return notify_fail(RED"你正忙着呢。\n"NOR);
    if(me->is_busy()) 
        return notify_fail(RED"你正忙着呢。\n"NOR);
	if(!arg) return notify_fail(HIR"指令格式：qidong |<门派编号>|<阵法人数>|<难度级数>|<空手是否>|！\n"NOR);
	if(this_object()->query("busy")) 
		return notify_fail(HIR"稍微休息一下，八卦需要时间复位！\n"NOR);
	if(sscanf(arg,"|%d|%d|%d|%d|",i,j,p,k) == 4)
	{   
		if(i>20 || i<1 ) 
			return notify_fail(HIY"注意指令中门派编号的取值范围。\n"NOR);
		if(j<2) 
			return notify_fail(HIY"注意指令中阵法人数不能低于2人。\n"NOR);
		if(j>10 ) 
			return notify_fail(HIY"注意指令中阵法人数的取值范围。\n"NOR);
		if(p<1 || p>10) 
			return notify_fail(HIY"注意指令中难度系数的取值范围。\n"NOR);
		if(k<1 || k>3) 
			return notify_fail(HIY"注意指令中空手系数的取值范围。\n"NOR);
	
		remove_call_out("must_dream");
        
		if(k==2) b=1;
		else if(k==3) b=random(2);//兵器的概率
		else if(j==4) e=1;

		if(p<=2)
		{
			a=0;//低级武功；
			c=p*30+random(p*30);
			d=9+random(p*3/2);//相同经验exp，随难度增加一点
		}
		else if(p==3)
		{
			c=(p-2)*30+random((p-2)*30);
			d=7+random(p);//增加-11%~11%exp
			e=random(2);//辅助hubo
		}
		else if(p==4)
		{
			c=(p-2)*30+random((p-2)*30);
			d=9+p-3+random(p-1);//增加11%~33%exp
			e=random(2);//辅助hubo
		}
		else if(p==5)
		{
			d=9+p-2+random(p-4);//增加33%~55%exp
			e=1;//辅助hubo
			f=random(2);
		}
		else if(p==6)
		{
			d=9+p-1+random(p-2);//增加66%~88%exp
			e=1;//辅助hubo
			f=random(2);
		}
		else if(p==7)
		{
			d=9+p+1+random(p-3);//增加88%~122%exp
			e=1;//辅助hubo
			f=1;
		}
		else if(p==8)
		{
			d=9+p+3+random(p-3);//增加122%~155%exp
			e=1;//辅助hubo
			f=1;
		}
		else if(p==9)
		{
			d=9+p+5+random(p-3);//增加155%~211%exp
			e=1;//辅助hubo
			f=1;
		}

		if(c>100) c=100;
		remove_call_out("start");
		remove_call_out("do_start");
		remove_call_out("busy");
		call_out("busy",120);
		this_object()->set("busy",1);
		if(random(2)) message_vision(HIC"$N调节八卦罗盘。只见霎那间狂风大作。\n"NOR, this_player());
		else message_vision(HIC"$N调节八卦罗盘。只见周边一阵恍惚。\n"NOR, this_player());
		me->set_temp("quest/special/a",a);//高级武功
		me->set_temp("quest/special/b",b);//兵器空手
		me->set_temp("quest/special/c",c);//pfm 几率
		me->set_temp("quest/special/d",d);//经验加成
		me->set_temp("quest/special/e",e);//hubo
		me->set_temp("quest/special/f",f);//lbwb
		me->set_temp("quest/special/i",i);//门派编号
		me->set_temp("quest/special/j",j);//阵法人数
		me->set_temp("quest/special/p",p);//难度系数
		call_out("do_start",2,me);
		return 1;
	}
	else return notify_fail(HIR"指令格式：qidong |<门派编号>|<阵法人数>|<难度级数>|<空手是否>|！\n"NOR);
}
void busy()
{
  this_object()->delete("busy");
  return;
}
void do_start(object me)
{
	object obj;
	string obj_party;
	mapping my_party;
	if(!me->query_temp("quest/special/up")) 
        return notify_fail(RED"你阵法启动失败。\n"NOR);
	if(!me->query_temp("quest/special/second_floor")) 
        return notify_fail(RED"你阵法启动失败。\n"NOR);

    my_party = get_party_int(me->query_temp("quest/special/i"));
	if(me->query_temp("quest/special/i")<=16)
		obj_party=my_party["party-name"]+"弟子";
	else obj_party=my_party["party-name"]+"传人";
	message_vision(HIG"\n一阵花香鸟语，周边景色全变，$N疑惑中似乎满天叠彩飞舞。"HIY"\n突然间，"+chinese_number(me->query_temp("quest/special/j"))+"个"NOR+my_party["col"]+obj_party+HIY"依次走了出来。\n"NOR,me,obj);
    copy_npc(me,1);
	for (int k = 1; k <= (me->query_temp("quest/special/j")-1); k++) 
			copy_npc(me,0);
	remove_call_out("check");
	call_out("check",1,me);
}

void check(object me)
{
 	string obj_party;
	object *obj,*player;
	int i,x=0,p=0;
	mapping my_party;


	if(!me || !living(me)||!present(me,this_object())||me->query("qi")<=1||me->query("jing")<=1||!me->query_temp("quest/special"))  
	{
		remove_call_out("check");
		remove_call_out("must_dream");
		destruct_person();//所有npc消失
		obj = all_inventory(this_object());
		for (i = 0; i < sizeof(obj); i++) {
			if (living(obj[i]) || obj[i]->is_character() ) continue;
			if(me && !obj[i]->query("copy_object")) 
			{
				obj[i]->move(me);
				message_vision(HIG"\n$N眼看着地上的"NOR+obj[i]->query("name")+HIG"飘散开去，犹如梦中，似乎也要消逝一般。\n"NOR,me);
				x=1;
			}
			else
				destruct(obj[i]);
		}         	
		//失败后的去所
		if(userp(me))
		{ 
			if(present(me,this_object()))
			{
				me->remove_all_killer();
				me->revive();
				me->reincarnate();
				destruct_poison(me);//去毒
				remove_call_out("busy");
				call_out("busy",5);
				message_vision(HIW"\n突然一道神光笼罩着$N，$N的精气神竟然全部恢复了！\n"NOR, me);
				message_vision(HIY"$N只觉得一阵朦胧，竟然似乎是飘飘然的离开了那个小楼。\n"NOR, me);
				me->move("/d/city/lxs");
				message_vision("$N急急忙忙地来到了"+environment(me)->query("short")+"。\n", me);
				tell_object(me,HIY"你怀疑自己也许从来就没有到过一个叫做梦蝶楼的地方。\n梦蝶楼在世外，而你，还在茫茫的书剑江湖中。。\n"NOR);
				if(x==1) tell_object(me,"你突然发现原来在蝶梦楼失落的物品竟然还在身上。\n");
				remove_call_out("again_check");
			    call_out("again_check",3,me);//为了防止dazhuan 类似的pfm 出现兵器留在上面的bug
			}
		   	me->delete_temp("quest/special");
    	}
        return;
	}

	player = all_inventory(environment(me));
	for (i = 0; i < sizeof(player); i++) {
		if (!living(player[i]) || player[i] == me) continue;
		if( player[i]->query("id")=="mengdie npc"|| player[i]->query("quest/level")) p++;
	}

	if (p==0)  //胜利
	{
		remove_call_out("check");
		if(me)
		{
			my_party = get_party_int(me->query_temp("quest/special/i"));
			me->remove_all_killer();
			destruct_poison(me);//去毒
			if(me->query_temp("quest/special/i")<=16)
				obj_party=my_party["party-name"]+"弟子";
			else obj_party=my_party["party-name"]+"传人";
			if(me->query_temp("quest/special/j")) 
				message_vision(HIW"\n经过艰苦搏斗，$N终于成功击退"+chinese_number(me->query_temp("quest/special/j"))+"人的"+my_party["party-name"]+my_party["col"]+my_party["lineup-name"]+HIW"！\n"NOR, me);
			else 
				message_vision(HIW"\n经过艰苦搏斗，$N终于成功击退"+my_party["party-name"]+my_party["col"]+my_party["lineup-name"]+HIW"！\n"NOR, me);
			message_vision(HIY"接着一道神光笼罩着$N，$N的内力有部分恢复！\n"NOR, me);
			me->add("neili",me->query("max_neili")*2/3+random(me->query("max_neili")/3));
			if(me->query("neili")>me->query("max_neili")*2) me->set("neili",me->query("max_neili")*2);
			remove_call_out("busy");
			call_out("busy",1);
			remove_call_out("must_dream");
			call_out("must_dream",360,me);
			random_gift(me);//随机奖励
		}
    	return;
	}
	call_out("check",1,me);
}
int valid_leave(object me, string dir)
{
	int i=me->query("quest/special/score");
	int p=me->query("quest/special/third_floor");
	if (me->is_fighting())
		return notify_fail(RED"\n战斗中，你竟然无法脱身。\n"NOR);
	if (present("mengdie npc",this_object()))
		return notify_fail(RED"\n你还在梦中。\n"NOR);
	if ( dir == "up" )
	{
		if(i<Score_Limit)
			return notify_fail(HIR"突然久违不见的老仆伸手拦在你面前道：这位"+RANK_D->query_respect(me) +"，如此战绩怎能上楼？还是多多努力吧！\n"NOR);
		if (i<(p+1)*Score_Limit)
			return notify_fail(HIR"突然久违不见的老仆伸手拦在你面前道：这位"+RANK_D->query_respect(me) +"，虽然你已经上次过"+chinese_number(p)+"次，但是还是需要努力才行的！\n"NOR);
		remove_call_out("must_dream");
		message_vision(HIY"\n突然久违不见的老仆慢慢地走了过来。\n"NOR, me);
		message_vision(CYN"老仆冲着$N微笑地点了点头！\n"NOR, me);
		message_vision("老仆说道：「"NOR+RED"这位"+RANK_D->query_respect(me) +"，想不到有如此骄人的战绩，欢迎来到三楼！"NOR+"」\n"NOR, me);
		message_vision(WHT"接着，老仆将手一指，突然一道神光笼罩着$N，\n$N的精、气、神、内力均有大幅度恢复！\n"NOR, me);
		message_vision(CYN"老仆向$N挥了挥手！\n"NOR, me);
		me->clear_condition();//
		me->reincarnate();
		remove_call_out("busy");
		call_out("busy",1);
		me->add("neili",me->query("max_neili")/2+random(me->query("max_neili")/2));
		if(me->query("neili")>me->query("max_neili")*2) me->set("neili",me->query("max_neili")*2);
		me->add("quest/special/third_floor",1);
		me->set_temp("quest/special/third_floor",1);
	}
	else if ( dir == "down" )
	{
		message_vision(HIY"\n突然久违不见的老仆慢慢地走了过来。\n"NOR, me);
		message_vision(CYN"老仆不解地看着$N，疑惑地点了点头！\n"NOR, me);
		message_vision("老仆说道：「"NOR+RED"这位"+RANK_D->query_respect(me) +"，想不到竟然放弃机会！不要怪我没提醒啊！"NOR+"」\n"NOR, me);
		message_vision(CYN"老仆叹了口气，向$N挥了挥手！\n"NOR, me);
		remove_call_out("must_dream");
		me->clear_condition();//
		remove_call_out("busy");
		call_out("busy",1);
		me->set_temp("quest/special/up",1);
		me->delete_temp("quest/special/second_floor");
	}
	return ::valid_leave(me, dir);
}

void copy_npc(object me,int x)
{
	int a,b,c,d,e,f,i,j,p,k;
    object obj,*inv;
	mapping my_party;

	if(!me) return;
	if(!me->query_temp("quest/special/up")) return;
	if(!me->query_temp("quest/special/second_floor")) return;
    a=me->query_temp("quest/special/a");
    b=me->query_temp("quest/special/b");
    c=me->query_temp("quest/special/c");
    d=me->query_temp("quest/special/d");
    e=me->query_temp("quest/special/e");
    f=me->query_temp("quest/special/f");
    i=me->query_temp("quest/special/i");
    j=me->query_temp("quest/special/j");
    p=me->query_temp("quest/special/p");

	my_party = get_party_int(i);

	obj=new(__DIR__"npc/mengdie_npc2");
	obj->set("quest/level",p);//
	obj->copy_menpai(({my_party["party"]}),a,b,c);
	obj->set_skills_level((me->query("max_pot")-100)*d/9);
	obj->set_skill(my_party["lineup"],200);//阵法
	obj->set_skill("literate",200);//读书写字
	obj->copy_state();	
	obj->get_npc_name();	

	if(i<=16)
		obj->set("title",my_party["col"]+obj->query("family/family_name")+"弟子"NOR);
	else obj->set("title",my_party["col"]+obj->query("family/family_name")+NOR);
	if(e==1) obj->set("double_attack",1);
	if(f==1 && i!=13 && i!=17 && i!=19 && i!=4)
	{
		obj->set_skill("lingbo-weibu",(me->query("max_pot")-100)*d/9);
		obj->set("quest/天龙八部/武功/yuxiang",1);
		obj->set("quest/天龙八部/武功/pass",1);
		obj->map_skill("dodge","lingbo-weibu");		
	}
	obj->set("max_qi",me->query("max_qi")*(d+random(3))/9);
	obj->set("eff_qi",obj->query("max_qi"));
	obj->set("qi",obj->query("max_qi"));
	obj->set("max_jing",obj->query("max_qi"));
	obj->set("eff_jing",obj->query("max_qi"));
	obj->set("jing",obj->query("max_qi"));
	obj->set("max_jingli",obj->query("max_qi"));
	obj->set("jingli",obj->query("max_qi"));
	obj->set("max_neili",me->query("max_neili")*(d+random(3))/9);
	obj->set("neili",me->query("max_neili")*(d+random(3))*3/18);	
	obj->set("combat_exp",me->query("combat_exp")*(d+random(3))/9);	
if((obj->query("combat_exp")-me->query("combat_exp"))>30000000)         obj->set("combat_exp",me->query("combat_exp")+30000000);
                //防止数据溢出，最多增加30M;
	obj->set("kill_object",me);
	obj->set("jiali",200);
	inv = all_inventory(obj);
	if(sizeof(inv))
	{
		for (k=0;k<sizeof(inv);k++) {
			if( !inv[k]->query("unique") )
				inv[k]->set("copy_object",1);
		}
	} 
	if(x>0)  obj->set("teamleader",1);//头头
	obj->set_name(obj->query("name"), ({ obj->query("id"),"mengdie npc"}));//为了方便组阵
    obj->set("party",my_party["party"]);//为了方便计算
	obj->move(environment(me)); 
}
void random_gift(object me)
{
        object obj;
        string str = "";
        int i = random(3);
	    int tt=me->query("quest/special/time");
		int x=me->query("quest/special/score");
		int y=me->query("quest/special/gift");
		int p=me->query_temp("quest/special/p");//难易程度
		int j=me->query_temp("quest/special/j");//阵法人数
		int a=me->query_temp("quest/special/a");//高级 低级武功
		int gg=0;
		int level=1;
		if(!p) p=1;
		if(!j) j=2;
		if(!a) a=0;
		if(!tt) tt=1;

		if(x<Gift_Limit) return;
		if(x<(y+1)*Gift_Limit) return;
		if(a==0 && !random(3)) return; //低级武功获得奖励几率低
		if(random(j+2)<3) return;//人数越少几率越低
		if(random(p+2)<2) return;//难度控制几率

        switch(i) {
                        case 0:         
                                obj = new (BOOK);
                                str = "书";  
                                break;
                        case 1: 
                                obj = new (MIC);
                                if (random(me->query_kar()) > 29) {
                                        obj->set("secret_obj",1);
                                        str = "极品";
                                }
                                str += "药";     
                                break;
                        case 2:         
                                obj = new (MON);
                                str = "钱"; 
                                break;
                        default:return;
		}
		if((random(150)<5+(p+j)/2||random(p+j)>17) && time()-tt>(60*60*12+random(60*60*6)))
			                     //难度控制高级奖励,p最大9
			                     //人数控制高级奖励,j最大10
								 //时间间隔至少12个小时~18个小时
								 //另外给难度9 人数10的天才 一点额外概率
		{
			i = random(4);
			if(!random(4)) level+=1;
			if(!random(8)) level+=1;
			if(!random(12)) level+=1;
			if(!random(24-p-j)) level+=1;//难度和人数增加奖励等级
			destruct(obj);
			switch(i) {
		          case 0:
					  obj = new(DAZAO);
					  str = "天外陨石";
					  break;
				  case 1:
				  case 2:
				  case 3:
					  obj = new(JADE);
					  str = "玉";
					  obj->set_level(level);
					  break;
				  default:return;              			
			}
            me->set("quest/special/time",time());
			gg=1;
		}	
        if (!objectp(obj))
        {
                CHANNEL_D->do_channel( this_object(), "sys",sprintf("%s出错。",str));
                return;
        }
        obj->set("gift_target",me->query("id"));
        obj->move(me);
        message_vision("$N突然觉得行囊里似乎重了很多，低头一看竟然是一"+ obj->query("unit")+obj->name()+"。\n",me);    
		me->add("quest/special/gift",1);
      	CHANNEL_D->do_channel( this_object(), "rumor",sprintf("听说%s在蝶梦楼处获得了一%s！",me->query("name"),obj->query("unit") + obj->name()));
        if(gg==1) 
			log_file("quest/SPECIAL",sprintf("%s(%s)第%d次获得%s奖励。等级：%d", me->name(1),me->query("id"),me->query("quest/special/gift"),obj->name(),level));
		else 
			log_file("quest/SPECIAL",sprintf("%s(%s)第%d次获得%s奖励。", me->name(1),me->query("id"),me->query("quest/special/gift"),obj->name()));
}
void destruct_person()
{  
	object *player,*obs,me;
	int i,k;
	player = all_inventory(this_object());
	for (i = 0; i < sizeof(player); i++) {
		if (!living(player[i])) continue;
		if (userp(player[i])) continue;
		if( player[i]->query("id")=="mengdie npc"|| player[i]->query("quest/level")) 
		{
			if(random(2)) command("wait");
			else if(random(2)) message_vision(HIG"\n$N哼了一声，缓缓消失在角落里。\n\n" NOR, player[i]);
			else message_vision(HIG"\n$N慢慢地退了出去。\n\n" NOR, player[i]);
			obs = deep_inventory(player[i]);       
			obs = filter_array(obs,(:$1->query("imbued"):));
			k = sizeof(obs);
			if(player[i]->query("kill_object") && player[i]->query("quest/level"))
			{
				me=player[i]->query("kill_object");
			}
			while(k--)
			{
				if(me) 
				{
					tell_object(me,HIY"突然你觉得行囊里似乎多了点什么，低头一看竟然是刚才失去的那一"+obs[k]->query("unit")+obs[k]->name()+HIY"。\n"NOR);
					obs[k]->move(me);
				}
				else 
				{
					message_vision(YEL"$N丢下一"+obs[k]->query("unit")+obs[k]->name()+NOR+YEL"。\n"NOR,player[i]);
					obs[k]->move(environment(player[i]));
				}
			}
			destruct(player[i]);
		}
	}
}
