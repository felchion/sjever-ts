//add by tangfeng
//2005.3
#include <ansi.h>
inherit ROOM;
string look_ziji();
string look_paizi();
int do_dream(string arg);
void must_dream(object me);
void check(object me,object ob);
int sort_user(object ob1, object ob2);
#define Score_Limit 1000
#include "/d/city/diemenglou_poison.h";
#include "/d/city/diemenglou.h";
void create()
{
	set("short",HIW"蝶梦楼一楼"NOR);
	set("long","一座小小的阁楼，以竹为魂，篱为魄，珠玉为阁，凝香为路，地上几方看
上去有点凌乱的草席，还有一扇黑洞洞的小草门，倒是墙壁上两个大字格外醒
目「"+HIR"書劍"NOR+"」，下面还有一行字迹（"+HIC"ziji"NOR+"）。几只蝴蝶五彩斑斓，在楼宇之间飞
来飞去。旁边树着一个木刻的牌子（"+HIY"paizi"NOR+"），似乎记载着什么。
");
	set("exits",([
		"up" : __DIR__"diemenglou2",
	]));
	set("item_desc",([            
            "ziji" : (: look_ziji :),
            "paizi" : (: look_paizi :),
        ]));
	set("quest",1);
	set("no_death",1);
	set("no_get_from", 1); 
	set("no_sleep_room", 1);
	set("outdoors", "扬州");
	set("coor/x",80);
	set("coor/y",40);
	set("coor/z",0);
	setup();
}
void init()
{
	object me;
	me = this_player();
	if(userp(me) && !wizardp(me)&& (me->query_condition("killer")||me->query_condition("job_busy")|| !me->query_temp("quest/special/up")))
	{
		me->remove_all_killer();
		me->revive();
		me->reincarnate();
		message_vision(HIC"\n突然$N眼前一花，周边的一切如梦如幻。$N不禁痴呆半响。\n"NOR,me); 
		me->move("/d/city/lxs");
		me->start_busy(2);
		return;
	}
	add_action("do_dream", ({"dream"}));
	add_action("do_liaoshang", ({"liaoshang"}));
	add_action("do_chakan", ({"chakan"}));
	add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
                                  "kill", "fight", "study", "practice", "exercise", "halt", "ask","whisper",
	                          "quit", "save", "throw", "suicide"}));
	if(me->query_temp("quest/special/up"))
	{
		remove_call_out("must_dream");
		call_out("must_dream",120,me);
	}
}
void must_dream(object me)
{
	if(!me) return;
	if(!me->query_temp("quest/special/up") || !me->is_fighting())
	{
		me->remove_all_killer();
		me->reincarnate();
		message_vision(HIC"\n突然$N眼前一花，周边的一切如梦如幻。$N不禁痴呆半响。\n"NOR,me); 
		message_vision(HIY"$N只觉得一阵朦胧，竟然似乎是飘飘然的离开了那个小楼。\n"NOR, me);
		me->move("/d/city/lxs");
		message_vision("$N急急忙忙地来到了"+environment(me)->query("short")+"。\n", me);
		tell_object(me,HIY"你怀疑自己也许从来就没有到过一个叫做梦蝶楼的地方。\n梦蝶楼在世外，而你，还在茫茫的书剑江湖中。。\n"NOR);
        me->start_busy(2);
		me->delete_temp("quest/special");
		remove_call_out("again_check");
		call_out("again_check",3,me);//为了防止dazhuan 类似的pfm 出现兵器留在上面的bug
		return;
	}
}
int do_action(string arg)
{
	if(!wizardp(this_player())){
		write("梦蝶楼上只能比武，请不要做不相关的事！\n");
		return 1;
	}
	else return 0;
}

string look_ziji()
{
    	object me;
		string str;
    	me = this_player();
		if(me->is_fighting()) 
			return "你正忙着呢。\n";
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";
    	me->start_busy(2);
		    str  = HIR"\n               且化蝴蝶梦北溟，更携书剑到天涯。\n"NOR;

			str += HIC"\n逍遥子化此小筑，可叹此生江湖潦倒，如痴似幻。大梦觉时，有泪如倾，\n"NOR;
			str += HIC"留此小筑，蝶舞一生，供后人演梦，但不知道小子辈可晓其中识、知、味。\n"NOR;
			str += HIW"\n你可以在这里使用dream指令与虚构的人物交手，演化尝试体验书剑世界中\n"NOR;
			str += HIW"的武学。目前开放的指令格式有：\n"NOR;
			str += HIY"\ndream player |<id>|"NOR+"："+HIM"与游戏中指定id玩家开始战斗\n"NOR;
			str += HIY"dream npc |<门派编号>|<难度级数>|<空手是否>|"NOR+"："+HIM"与指定门派NPC开始战斗\n"NOR;
			str += RED"\n门派编号为：\n"NOR;
		    str += WHT"1--武当 2--华山 3--嵩山 4--古墓 5--丐帮 6--铁掌 7--大轮寺 8--慕容\n"NOR;
			str += WHT"9--神龙岛 10--星宿 11--峨嵋 12--明教 13--桃花岛 14--天龙 15--少林\n"NOR;
			str += WHT"16--昆仑 17--九阴大全 18--蛤蟆功 19--辟邪剑 20--金蛇剑\n"NOR;
			str += HBRED"\n难度级数设定从1到9。空手设定为1，兵器为2，随机为3，空手却互搏为4。\n"NOR;
        tell_object(me,str);
    	return "";
}
string look_paizi()
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
int do_dream(string arg)
{
	string *str_menpai = ({ "wd","hs","ss","gm","gb","tz","dls","mr",
							"sld","xx",	"em","mj","thd","tls","sl","kl","9y","hmg","pxj","jsj"});
	object me = this_player();
	object obj,player,*inv;
	string player_id,player_party;
	int i,p,j,k;
	int a=1,b=0,c=100,d=9,e=0,f=0;
	// a  高级/低级武功 b 空手/兵器 c pfm几率 d 经验加成 e hubo f lbwb
	if(me->query_temp("killer")) return 0;
	if(!me->query_temp("quest/special/up")) return 0;//tjf不可以
    if(me->is_fighting()) 
        return notify_fail(RED"你正忙着呢。\n"NOR);
    if(me->is_busy()) 
        return notify_fail(RED"你正忙着呢。\n"NOR);
	if(!arg) return notify_fail(HIR"指令格式：dream npc |数字|数字|数字|或者dream player |id|！\n"NOR);

	if(sscanf(arg, "npc |%d|%d|%d|", i, p,j) == 3
		|| sscanf(arg, "player |%s|", player_id) == 1);
	else return notify_fail(HIR"指令格式：dream npc |数字|数字|数字|或者dream player |id|！\n"NOR);

	remove_call_out("must_dream");
	if (sscanf(arg, "npc |%d|%d|%d|", i, p,j) == 3)
	{
		if(i>20 || i<1 || p>9 || p<1 ||j<1 || j>4) 
			return notify_fail(HIY"指令中数字取值范围请详细阅读字迹上的提示。\n"NOR);
		
		if(j==2) b=1;
		else if(j==3) b=random(2);//兵器的概率
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
			d=9+p-2;//增加11%exp
			e=random(2);//辅助hubo
		}
		else if(p==4)
		{
			c=(p-2)*30+random((p-2)*30);
			d=9+p-3+random(p);//增加11%~44%exp
			e=random(2);//辅助hubo
		}
		else if(p==5)
		{
			d=9+p-1+random(p-4);//增加44%~66%exp
			e=1;//辅助hubo
			f=random(2);
		}
		else if(p==6)
		{
			d=9+p+random(p-3);//增加66%~88%exp
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

		player_party=str_menpai[i-1];
		obj=new(__DIR__"npc/mengdie_npc1");
		obj->set("quest/level",p);//
		obj->copy_menpai(({player_party}),a,b,c);
		obj->set_skills_level((me->query("max_pot")-100)*d/9);	
		obj->copy_state();	
		obj->get_npc_name();	
		if(i<=16)
			obj->set("title",HIW+obj->query("family/family_name")+HIW"弟子"NOR);
		else obj->set("title",HIY+obj->query("family/family_name")+NOR);
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
		obj->set("combat_exp",me->query("combat_exp")*(d+random(2))/9);	
if((obj->query("combat_exp")-me->query("combat_exp"))>30000000)		obj->set("combat_exp",me->query("combat_exp")+30000000);
		//防止数据溢出，最多增加30M;
		obj->set("party_id",i);//门派号码
	}
	else
	{
        player=LOGIN_D->find_body(player_id);
		if(!player) player = find_living(player_id);
		if(!player)
			return notify_fail(HIY"没有发现一个id是"+player_id+"的玩家。请注意在线玩家列表！\n"NOR);
		if(player==me)
			return notify_fail(HIY"不提供自残者样本！\n"NOR);
		if( wiz_level(me) < wiz_level(player) )
			return notify_fail("你没有直接呼叫这个物件的复制权力。\n");
		if(wizardp(player))
			return notify_fail(HIY"巫师无法寻找！\n"NOR);
		obj=new(__DIR__"npc/mengdie_npc1");
		obj->do_copy(player);
		obj->set("title",HIY"复制品"NOR);
		if(!random(4))
		{
			obj->set("double_attack",1);
			obj->set_temp("double_attack",1);
			obj->set("title",HIR"基因突变复制品"NOR);
			obj->add("combat_exp",random(obj->query("combat_exp")));
			obj->add("max_qi",random(obj->query("max_qi")*3/2));
			obj->add("max_neili",random(obj->query("max_neili")/2));
			obj->reincarnate();
			obj->set("neili",obj->query("max_neili")*2);
		}
		i=obj->query("combat_exp")/me->query("combat_exp");
		if(i>0)
			obj->set("quest/level",i*i);//
	}
    if(!obj)
		return notify_fail(RED"你命令似乎失效。\n"NOR);
	obj->set("jiali",200);
	obj->set("kill_object",me);
	message_vision(HIG"\n一阵花香鸟语，周边景色全变，$N疑惑中似乎满天叠彩飞舞。\n突然间，一个"+obj->short(1)+HIG"跳了进来。\n"NOR,me,obj);
	obj->move(environment(me)); 
	obj->kill_ob(me);
	me->kill_ob(obj);
	inv = all_inventory(obj);
	if(sizeof(inv))
	{
		for (k=0;k<sizeof(inv);k++)
		{
			if( !inv[k]->query("unique") )
				inv[k]->set("copy_object",1);          
		} 
	}
	remove_call_out("check");
    call_out("check",1,me,obj);
	return 1;

}

void check(object me,object ob)
{
	object *obj,*obs;
	int i,x=0;

	if(!me || !living(me)||!present(me,this_object())||me->query("qi")<=1||me->query("jing")<=1||!me->query_temp("quest/special"))  
	{
		remove_call_out("check");
		remove_call_out("must_dream");
		if(ob)
		{
			obs = deep_inventory(ob);       
			obs = filter_array(obs,(:$1->query("imbued"):));
			i = sizeof(obs);
			message_vision(HIY"\n突然一阵风散，$N原来站着的地方竟然一点痕迹也没有留下。\n"HIM"刚才的一切如梦幻般，全然消逝在风中。\n"NOR,ob);
			while(i--)
			{
				if(me) 
				{
					tell_object(me,HIY"突然你觉得行囊里似乎多了点什么，低头一看竟然是刚才失去的那一"+obs[i]->query("unit")+obs[i]->name()+HIY"。\n"NOR);
					obs[i]->move(me);
				}
				else 
				{
					message_vision(YEL"$N丢下一"+obs[i]->query("unit")+obs[i]->name()+NOR+YEL"。\n"NOR,ob);
					obs[i]->move(environment(ob));
				}
			}
			destruct(ob);
		}
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
	if (!ob || !present(ob,this_object()))  
	{
		remove_call_out("check");
		if(me)
		{
			me->remove_all_killer();
			destruct_poison(me);//去毒
			message_vision(HIW"\n接着一道神光笼罩着$N，$N的内力有部分恢复！\n"NOR, me);
			me->add("neili",me->query("max_neili")/2+random(me->query("max_neili")/2));
			if(me->query("neili")>me->query("max_neili")*2) me->set("neili",me->query("max_neili")*2);
			remove_call_out("must_dream");
			call_out("must_dream",60,me);
		}
    	return;
	}
	call_out("check",1,me,ob);
}

int valid_leave(object me, string dir)
{
	int i=me->query("quest/special/score");
	int p=me->query("quest/special/second_floor");
	if (me->is_fighting())
		return notify_fail(RED"\n战斗中，你竟然无法脱身。\n"NOR);
	if (present("mengdie npc",this_object()))
		return notify_fail(RED"\n你还在梦中。\n"NOR);
	if(i<Score_Limit)
		return notify_fail(CYN"突然久违不见的老仆伸手拦在你面前道：这位"+RANK_D->query_respect(me) +"，如此战绩怎能上楼？还是多多努力吧！\n"NOR);
	if (i<(p+1)*Score_Limit)
		return notify_fail(CYN"突然久违不见的老仆伸手拦在你面前道：这位"+RANK_D->query_respect(me) +"，虽然你已经上次过"+chinese_number(p)+"次，但是还是需要努力才行的！\n"NOR);
	 message_vision(HIY"\n突然久违不见的老仆慢慢地走了过来。\n"NOR, me);
	 message_vision(CYN"老仆冲着$N微笑地点了点头！\n"NOR, me);
	 message_vision("老仆说道：「"NOR+HIR"这位"+RANK_D->query_respect(me) +"，想不到有如此骄人的战绩，欢迎来到二楼！"NOR+"」\n"NOR, me);
	 message_vision(WHT"接着，老仆将手一指，突然一道神光笼罩着$N。\n$N的精、气、神、内力均有大幅度恢复！\n"NOR, me);
	 message_vision(CYN"老仆向$N挥了挥手！\n"NOR, me);
	 remove_call_out("must_dream");
	 me->clear_condition();//
	 me->reincarnate();
	 me->add("neili",me->query("max_neili")/2+random(me->query("max_neili")/2));
	 me->add("quest/special/second_floor",1);
	 me->set_temp("quest/special/second_floor",1);
     return ::valid_leave(me, dir);
}

