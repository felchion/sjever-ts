// Room: dixiashi.c
//created by campsun 2003/09/22
#include <ansi.h>

inherit ROOM;
int create_fight();
int checkSameIp(object);
string *str_menpai = ({ "wd","hs",
			"ss","gm",
			"gb","tz",
			"dls","mr",
			"sld","xx",
			"em","mj",
			"thd","tls",
			"sl","kl"
			});	
string *str_skill_other = ({"taoism",                                           //武当：道学心法
                            "begging",                                          //丐帮：叫化绝活
                            "bangjue",                                          //丐帮：打狗棒口诀
                            "stealing",                                         //丐帮：妙手空空
                            "checking",                                         //丐帮：
                            "zhenqijue",                                        //华山：正气诀
                            "tiezhang-xinfa",                                   //铁掌：铁掌心法
                            "huanxi-chan",                                      //大轮寺：欢喜禅
                            "shenlong-yaoli",                                   //神龙岛：神龙药理
                            "dacheng-fofa",                                     //峨嵋：大乘佛法
                            "qimen-bagua",                                      //桃花岛：奇门八卦
                            "buddhism",                                         //天龙寺，少林：禅宗心法
                            "art",                                              //昆仑：琴棋书画
                            "daode-jing",                                       //九阴：道德经
                            "poison",						//基本毒计
                            "medicine",						//本草术理
                            "wuxing-zhen",					//公共阵法：五行阵
                            "zhenwu-qijiezhen",					//武当阵法
                            "dagou-zhen",					//丐帮打狗阵
                            "linji-jianzhen",					//娥眉剑阵
                            "shenghuo-fumozhen",				//明教圣火伏魔阵
                            "fumoquan-zhen",					//少林伏魔圈阵
                            "shenlong-wudizhen",				//神龙无敌阵
                            "qimen-baguazhen",					//奇门八卦阵
                                                        });															//随机产生的门派列表

string *str_chuchang_desc =({
		HIC"突然之间，没见到怎么跑，大家就发现$N已经站在了擂台一角。\n"NOR ,
		HIY"只见$N从平地蹭地跃起，使了一个鹞子翻身落在了台上，惹的观众阵阵掌声！\n"NOR ,
		HIM"$N来到台下，仰脸往台上看了看，双脚点地，飞身蹿上擂台。观众一阵掌声：“好功夫！好快的身法！”\n"NOR ,
		HIR"只见$N嘴角轻蔑的一瞥纵身一跃，平平稳稳的站在了台上，惹的观众阵阵掌声！\n"NOR ,
		HIW"只见$N从人群里挤到台下，往上一长腰，双手扒住台板，又一缓劲上了拳台，用手一指：“呔！还不快来送死！”\n"NOR ,
		HIB"只见$N唰“啦”甩掉英雄大氅，丹田提气，“噌”一声蹿上擂台，真是轻如鸿毛，稳如泰山。\n"NOR ,
		});
object red_fighter;
object black_fighter;
object laoban;
object winner;
						
string *strIpAddress;

void create()
{

	set("short", "地下黑拳市");
	set("long", @LONG
这是扬州城内的一家地下赌坊，城里人都知道这里，但外人并不清楚。这里平时
有比武，但是都是生死自负。赢的人往往能获得大量的金钱，输的人则丢掉生命。
左面有个牌子（paizi）,说明了赌博的规则。
LONG
	);
	
	set("exits", ([
	         "up" : __DIR__"dating1",
	]));
	set("coor/x",80);
	set("coor/y",-20);
	set("coor/z",-10);
    	set("no_sleep",1);
    	set("state","");																	//判断是否已经开始

	set("yingyu",0);
    	set("winner","none");

	set("item_desc", ([
                "paizi": @TEXT
地下赌场赌法说明：

一、yazhu <某方> <金量>

某方：　red(红方) black(黑方) both(平局) 或者 none(平局)

金量：　以黄金为单位，最少1两黄金，最多100两，目前赔率是1:1.3 至 1:20
		平局的赔率是 1:1.5
		
例子：
yazhu red 50
赌五十两黄金在红方上

二、dadu <某方> <金量> with <某人>

某方：　red(红方) 或者 black(黑方)
某人：  同一房间的任何一个其他玩家
金量：　以黄金为单位，最少50两黄金，最多500两，目前赔率是1:1
例子：
dadu red 50 with test
与 test 赌 red 赢，赌注为 50 gold。test 同意后双方的赌才有效。

三、其他功能
玩家可以 ask huang fuba about 比赛 询问比赛情况
	 ask huang fuba about 赌金 询问押注情况和当前赔率情况，当前赔率主要和双方的押注有关
	 ask huang fuba about 兑换 可以免费把你身上的银子兑换成黄金
	 list		      可以查看黄富霸出售的食物和饮水
	 					
	 				   如有BUG,请SOS说明
	 				  书剑网络游戏开发小组

TEXT
        ]) );
        

	 
     setup();      
//     laoban=new("/d/city/npc/laoban.c");
//     laoban->move(this_object());
//     laoban->set("startroom","/d/city/dixiashi");     
//     call_out("wait_fight",10);
}


//void init()
//{
//	object me;
//	me = this_player();
//	//add_action("do_bet","yazhu");
//	//add_action("do_bound","bound");
//	//add_action("do_dadu","dadu");
//	if (me->id("linux") ||getuid(me)=="master" )
//	add_action("do_full","damo");															//打赌，玩家之间赌输赢
//	if (userp(this_player()) && !wizardp(this_player()))
//	{
//		add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
//			"kill", "fight", "study", "practice",
//		"emote","perform","ansuan","guard","throw","juedou","bark","steal","beg","sleep"}));
//		add_action("do_yun", ({"exert","yun","fanchang"}));									//add fanchang by campsun 2003.10.18
//
//	}
//	this_player()->delete_temp("dxdc");
//
//	if ((int)me->query_condition("killer"))										//禁止杀人犯在这里躲通缉
//	{
//		write(CYN"你费尽千辛万苦混入了赌场，只想坐下来休息一会。\n");
//		write(CYN"突然之间，一个硕大的身影闪电般出现在你身边，向你连拍八十一掌。\n");
//		write(CYN"黄老板喝到：“杀人了想躲我这里，没那么容易！”\n");
//		write(CYN"你觉得天旋地转，被掌力抛出了门外。\n");
//		write(CYN"你感觉中掌之后，丹田内似乎少了些什么。\n"NOR);
//		me->move("/d/city/dating1");
//		me->add("max_neili",-random(10));														//因为是利用bug进入，所以给予小小惩罚
//	}
//
//	if (userp(me) && !wizardp(me)){
//		if(checkSameIp(me))																			//为了防止玩家gold收入过高，同ip禁止一起玩。
//		{
//			message_vision(HIB"\n目前赌场太拥挤了，$N被挤了出去。\n"NOR, me);
//			me->move("/d/city/dating1");
//		}
//		if (!sizeof(strIpAddress)) strIpAddress=({});
//
//		strIpAddress = strIpAddress + ({query_ip_number(me)});
//	}
//}

void reset()
{
return;																							//禁止系统更新这里，防止中断正在进行的比赛
}
int do_yun()
{
	this_player()->start_busy(2);
	write("\n你刚打了个哈欠，就觉得有人在盯着你看，你只好马上把气咽了回去。\n\n");
	return 1;
}
int do_action(string arg)
{
	this_player()->start_busy(2);
   	write("\n你正要有所动作，突然身旁有人将你一拍：好好看比武，别乱动！\n\n");//禁止玩家对ｎｐｃ动手脚
   	return 1;
}

int do_full(string arg)
{
	object red,black;
	object me = this_player();
	int qi;

	
	if( !arg || sscanf(arg, "%d", qi)!=1 )
		qi = 999999;
	else
		sscanf(arg, "%d", qi);
		
	red = present("red", environment(me));
	black = present("black", environment(me));
	
	if (!red)
		return notify_fail("没有发现红方。\n");
	if (!black)
		return notify_fail("没有发现黑方。\n");	
	if (userp(red))
		return notify_fail("发现叫red账号的玩家。\n");
	if (userp(black))
		return notify_fail("发现叫black账号的玩家。\n");	
			
	red -> set("max_qi",qi);
	red -> set("qi",qi);
	red -> set("eff_qi",qi);
	black -> set("max_qi",qi);
	black -> set("qi",qi);
	black -> set("eff_qi",qi);
   	write("作弊成功！红黑双方气设为"+qi+",请用hp查看红黑双方\n\n");//作弊
   	return 1;
}

int wait_fight()
{
	object huoyan;
	string str_temp="";


	int base_exp=(random(20)+1)*1000000;														//产生随机ｅｘｐ分配给ｎｐｃ
	int base_jiali=random(11);																	//产生随机ｊｉａｌｉ
	remove_call_out("wait_fight");

	if(objectp(red_fighter)) destruct(red_fighter);													//清除红方和黑方
	if(objectp(black_fighter)) destruct(black_fighter);
	black_fighter=new("/d/city/npc/wushi");
	red_fighter=new("/d/city/npc/wushi");
	str_menpai=str_menpai-({"9y","kl","hmg","pxj","jsj"});									//如果不去掉hmg和9y，则这会叠加，最后概率十分
	if(!random(10)) str_menpai=str_menpai+({"9y"});											//有１／１０的概率把９ｙ加入随机门派中。实际概率目前为１／１７０
    if(!random(15)) str_menpai=str_menpai+({"hmg"});
    if(!random(8)) str_menpai=str_menpai+({"pxj"});
    if(!random(6)) str_menpai=str_menpai+({"jsj"});
    if(!random(3)) str_menpai=str_menpai+({"kl"});											//昆仑实在是太弱了，减少出昆仑的概率
    	set("red_party",str_menpai[random(sizeof(str_menpai))]);
    	set("black_party",str_menpai[random(sizeof(str_menpai))]);
    	if((query("red_party")=="xx" || query("black_party")=="xx") && !objectp(huoyan))
    	{
    	huoyan=new("/d/xingxiu/obj/huoyan");
    	huoyan->move(this_object());															//如果有星宿弟子参与战斗，则升起一个火焰
	}	
    set("use_weapon",random(3));																//随机决定是否使用武器
   	set("base_exp",base_exp);
    set("base_jiali",base_jiali);
    set("level",random(3));
   	red_fighter->set("tag","red");																//设定红方选手
	red_fighter->move(this_object());
	black_fighter->set("tag","black");															//设定黑方选手		
	black_fighter->move(this_object());
	message_vision("\n"+str_chuchang_desc[random(sizeof(str_chuchang_desc))],black_fighter);
	message_vision("\n"+str_chuchang_desc[random(sizeof(str_chuchang_desc))],red_fighter);

	if(!objectp(laoban))
		return notify_fail("地下赌场老板哪里去了？大家去找wiz把他请来，他来了才能再比赛！\n");
	
	str_temp=HIM"\n$N说道："+HIB"比武赌博即将开始，欢迎大家踊跃参与！\n"NOR;
	str_temp+=HIM"\n$N说道："+HIG"这次比武的双方分别是：\n"NOR;
	str_temp+=RED"\n              红方："NOR+"来自于"HIR+red_fighter->query("family/family_name")+NOR"的"HIW+red_fighter->query("name")+NOR"";
	str_temp+=BLU"                黑方："NOR+"来自于"HIR+black_fighter->query("family/family_name")+NOR"的"HIW+black_fighter->query("name")+NOR"\n";
	str_temp+="\n                    绝技："HIY+red_fighter->query("good_skills")+NOR"";
	str_temp+="                          绝技："HIY+black_fighter->query("good_skills")+NOR"\n";   
	str_temp+=HIM"\n$N说道："+HIB"这局比武规则如下：\n\n"NOR;
	str_temp+="           ";
	if(!query("use_weapon")) str_temp+="一："+HIY"空手格斗 "NOR;
	else str_temp+="一："+HIY"兵器格斗 "NOR;
	switch(query("base_jiali"))
	{
		case 0:
			str_temp+="　二："+HIY"双方"+HIC"不使用内力比武　"NOR;
		break;
		case 10:
			str_temp+="　二："+HIY"双方使用"+HIC"全力比武　"NOR;
		break;
		default:
			str_temp+="　二："+HIY"双方使用"HIC+chinese_number(query("base_jiali"))+"成"NOR+HIY"力量比武　"NOR;
	}
	str_temp+="　三："+HIY"十分钟内杀死对方的获胜\n\n"NOR;
	str_temp+=HIM"$N说道："+HIC"过五分钟比赛就要开始了！现在大家可以压自己喜欢的选手了！\n\n"NOR;
	
	message_vision(str_temp,laoban);
	
	laoban->set("red_f_skill",red_fighter->query("good_skills"));
	laoban->set("black_f_skill",black_fighter->query("good_skills"));
	

	set("state","wait");																							//初试化状态
	set("winner","none");																								//初试化冠军
	set("red_money",0);																								//压在红方的总金额
	set("black_money",0);																							//压在黑方的总金额
	set("none_money",0);
	set("red_betrate",130);																							//红方与黑方的赔率，目前定为１：１．５
	set("black_betrate",130);
	set("none_betrate",150);
	set("num",4);
	call_out("start_jishu",90);
	
return 1;
}
int start_jishu()
{
	remove_call_out("start_jishu");
	if(query("num"))
	{
		switch(query("num"))
		{
		case 4:
			message_vision(HIC"\n                                     比武即将开始：\n"NOR,red_fighter);	
			add("num",-1);
		break;
		case 3:
			message_vision(HIY"\n               一位穿黄色旗袍的小姐端着牌子在台上走了一圈，上面写着：三\n"NOR,red_fighter);	
			add("num",-1);				
		break;
		case 2:
			message_vision(HIG"\n               一位穿绿色旗袍的小姐端着牌子在台上走了一圈，上面写着：二\n"NOR,red_fighter);	
			add("num",-1);				
		break;
		case 1:
			message_vision(HIW"\n               一位穿白色旗袍的小姐端着牌子在台上走了一圈，上面写着：一\n"NOR,red_fighter);	
			add("num",-1);				
		break;
		}
		call_out("start_jishu",1);
	}
	else

	call_out("start_fight",1);
}
int start_fight()
{
	//赌神程序
	int qi = 9999999;
	int none_flag, red_flag, black_flag;
	int total = query("red_money") + query("black_money") + query("none_money");
	//red win 表示 红方赢所赢的钱，其他类推
	int red_win = total - (query("red_money") * query("red_betrate")/100);
	int black_win = total - (query("black_money") * query("black_betrate")/100);
	int none_win = total - (query("none_money") * query("none_betrate")/100);
	
	set("state","fighting");
	remove_call_out("start_fight");
	
	if (red_win<0 && black_win<0 && none_win<0)
		log_file( "/static/bet",
		sprintf("赔率出错 | 黑方%d:%d| 红方%d:%d | 平局%d:%d \n", query("black_money"),to_int(query("black_betrate")),query("red_money"),to_int(query("red_betrate")),query("none_money"),to_int(query("none_betrate"))));
	


	//在赌注总数大于200 gold的情况下有七成机会执行赌神程序
	if((total > 200) && (random(9)>2))
	{
		if (red_win>none_win) 
		{
			if (red_win>black_win)
				red_flag = 1;
			else 
				black_flag = 1;
		}		
		else
		{
			if (black_win>none_win)
				black_flag = 1; 
			else
				none_flag =1; 
		}
	log_file( "/static/bet",sprintf("赔率 | 黑方%d:%d| 红方%d:%d | 平局%d:%d \n", query("black_money"),to_int(query("black_betrate")),query("red_money"),to_int(query("red_betrate")),query("none_money"),to_int(query("none_betrate"))));	
	}
	
	if (none_flag)
	{
		red_fighter -> set("max_qi",qi);
		red_fighter -> set("qi",qi);
		red_fighter -> set("eff_qi",qi);
		black_fighter -> set("max_qi",qi);
		black_fighter -> set("qi",qi);
		black_fighter -> set("eff_qi",qi);
	}
	if(red_flag)
	{
		red_fighter -> set("max_qi",qi);
		red_fighter -> set("qi",qi);
		red_fighter -> set("eff_qi",qi);
		black_fighter -> set("max_qi",5000);
		black_fighter -> set("qi",5000);
		black_fighter -> set("eff_qi",5000);
	}
	if(black_flag)
	{
		red_fighter -> set("max_qi",5000);
		red_fighter -> set("qi",5000);
		red_fighter -> set("eff_qi",5000);
		black_fighter -> set("max_qi",qi);
		black_fighter -> set("qi",qi);
		black_fighter -> set("eff_qi",qi);
	}
	
	if (!objectp(red_fighter))
	{
		message_vision("红方选手拉肚子不能出席比赛，这场比赛取消了。\n",laoban);
		call_out("wait_fight",10);
		return 0;
	}
	if (!objectp(black_fighter))
	{
		message_vision("黑方选手刚才扭伤了脚，不能参加比赛了。这场比赛取消了。\n",laoban);					//一般不会发生这两种情况
		call_out("wait_fight",10);
		return 0;
	}
	message_vision(HIW"\n                                     比赛开始：\n"NOR,red_fighter);	
		
	message_vision(HIR"看起来$N想杀死$n！\n"NOR,red_fighter,black_fighter);
	message_vision(HIR"看起来$N想杀死$n！\n"NOR,black_fighter,red_fighter);
	

	black_fighter->kill_ob(red_fighter);
	red_fighter->kill_ob(black_fighter);
	
	call_out("get_bound",300);																						//防止比赛莫名其妙停止后不再进行
return 1;
}
void get_bound()
{
	remove_call_out("get_bound");
	
	call_out("do_bound",random(3)+3);																				//防止其中一方选手昏过去死之前就发布奖励
}
int do_bound()
{
	string str_temp="";
	object *ob;
	object huoyan;
	object room;
	object gold_amount;
	object silver_amount;
	mapping skills;
	string* sname;
	int i,j,win_money_gold,win_money_silver,skills_i;
//	int my_dadu_gold,op_dadu_gold;
	int my_dadu_gold;
	float win_rate;
	
	remove_call_out("do_bound");
	set("state","bound");
	call_out("wait",random(3)+3);
	room=this_object();
	ob = all_inventory(room);
	switch(room->query("winner"))
	{
	case "none":
		if(objectp(red_fighter) && objectp(black_fighter))
		{
			red_fighter->remove_enemy(black_fighter);
	       	black_fighter->remove_killer(red_fighter);
		}
		message_vision(HIC"经过这么久的时间，$N和$n还未分胜负，因此决定双方共同为冠军。\n"NOR,red_fighter,black_fighter);		//如果在１０分钟内未决出胜负，则结束
	break;
	case "red":
		if(objectp(red_fighter))
			winner=red_fighter;
	break;
	case "black":
		if(objectp(black_fighter))
			winner=black_fighter;
	break;
	
	}//end switch
	if (objectp(winner)){
	str_temp=sprintf("\n裁判冲上台，举着$N的手说到：本局的冠军是%s的$n。\n"NOR,winner->query("family/family_name"));
	message_vision(HIY+str_temp,winner,winner);
	if(random(2))
		message_vision(CYN"$N兴奋的向观众挥手致意！\n"NOR,winner);
	else
		message_vision(CYN"$N兴奋的对你摇着尾巴！\n"NOR,winner);
	
	}
	win_rate=room->query(room->query("winner")+"_betrate");																		//获得获胜方的赔率

	for(i=0;i<sizeof(ob);i++)
    	{
    		if( !userp(ob[i]) ) continue;
    	if (!living(ob[i])) 
    	{
    		message_vision(HIB"黄富霸皱了皱眉：“怎么昏倒在这里？钱给我留下，人给我扔到街上去！”\n"NOR,ob[i]);
    		
	    	message_vision(HIB"从外面进来两个彪行大汉，把$N身上的钱都搜走了。\n"NOR, ob[i]);	
	    	
	    	gold_amount = present("gold_money", ob[i]);
	    	
	    	if (objectp(gold_amount))
	    		destruct(gold_amount);
	    	
	    	message_vision(HIB"从外面进来两个彪行大汉，把昏倒的$N驾了出去。\n"NOR, ob[i]);
	    	log_file("static/duchangebug",
            	sprintf("%s(%s)在地下黑拳晕倒，可能涉及利用bug牟利。\n",ob[i]->name(),ob[i]->query("id")));
	    	
	    	ob[i]->move("/d/city/dating1");
    		continue;
    	}
    	
    	//if (ob[i]->query_temp("dxdc/dadu/state") == "wait") ob[i]->delete_temp("dxdc/dadu"); //清楚等待者的标记
    	if(ob[i]->query_temp("dxdc/dadu/state")=="dadu")
    	{
    		my_dadu_gold=ob[i]->query_temp("dxdc/dadu/bet");
    	//	op_dadu_gold=ob[i]->query_temp("dxdc/dadu/opbet");												//因为打赌双方压注金额为一样的，所以不用op_dadu_gold,并且发现这样会造成严重bug
    		if(room->query("winner")=="none")
    		{
    			gold_amount = present("gold_money", ob[i]);
    			if (objectp(gold_amount))
    			{
   					gold_amount->add_amount(my_dadu_gold);
   				}
   				else
   				{
   					if(my_dadu_gold)
   					{
   					gold_amount=new("/clone/money/gold");
 					gold_amount->set_amount(my_dadu_gold);
   					gold_amount->move(ob[i]);
   					}
   				}  
				message_vision(sprintf("在私下的打赌中，由于双方战成了平手，所以$N拿回了%s两保证金。\n",chinese_number(my_dadu_gold)), ob[i]);		
    		log_file("static/bet",sprintf("在私下的打赌中，由于双方战成了平手，所以%s拿回了%s两保证金。\n",ob[i]->query("id"),chinese_number(my_dadu_gold)));
    		}
    		else
    		if(ob[i]->query_temp("dxdc/dadu/side")==room->query("winner"))
    		{
    			gold_amount = present("gold_money", ob[i]);
    			if (objectp(gold_amount))
    			{
   					gold_amount->add_amount(my_dadu_gold*2);
   				}
   				else
   				{
   					if(my_dadu_gold*2)
   					{
   					gold_amount=new("/clone/money/gold");
 				//	gold_amount->set_amount(my_dadu_gold+op_dadu_gold);
 					gold_amount->set_amount(my_dadu_gold*2);
   					gold_amount->move(ob[i]);
   					}
   				}    		
			//	message_vision(sprintf("在私下的打赌中，$N慧眼识英雄，拿回来自己的保证金%s两黄金，并且赢得了%s两黄金。\n",chinese_number(my_dadu_gold),chinese_number(op_dadu_gold)), ob[i]);	
				message_vision(sprintf("在私下的打赌中，$N慧眼识英雄，拿回来自己的保证金%s两黄金，并且赢得了%s两黄金。\n",chinese_number(my_dadu_gold),chinese_number(my_dadu_gold)), ob[i]);	
				log_file("static/bet",sprintf("%s赢得了%s两黄金。\n",ob[i]->query("id"),chinese_number(my_dadu_gold)));
				//比武奖励
				if (random(ob[i]->query_kar()) > 22  && my_dadu_gold >= 50){
				skills=ob[i]->query_skills();
				if (mapp(skills)) sname  = keys(skills);
				sname = sname - str_skill_other;
				
				skills_i=random(sizeof(sname));
				ob[i]->improve_skill(sname[skills_i], my_dadu_gold/10 + random(20));
				tell_object(ob[i], YEL "通过对红黑双方的比武观摩，你只觉以前存在的诸多疑问豁然开朗，对"+to_chinese(sname[skills_i])+"有了新的感悟。\n"NOR
				);
				}	
    		}
    		else
    		{
				message_vision(sprintf("在私下的打赌中，$N输掉了%s两黄金。\n",chinese_number(my_dadu_gold)), ob[i]);		
    		log_file("static/bet",sprintf("%s输掉了%s两黄金。\n",ob[i]->query("id"),chinese_number(my_dadu_gold)));
    		}
   		ob[i]->delete_temp("dxdc/dadu");
    		
    	}
    	if(ob[i]->query_temp("dxdc/bet"))
    	{
    	if(ob[i]->query_temp("dxdc/side")==room->query("winner"))
    	{
    		win_money_gold=ob[i]->query_temp("dxdc/bet")*win_rate/100;
    		win_money_silver=to_int(ob[i]->query_temp("dxdc/bet")*win_rate*100) /100 % 100 ;																
    		
    		gold_amount = present("gold_money", ob[i]);
    		if (objectp(gold_amount))
    		{
   				gold_amount->add_amount(win_money_gold);
   			}
   				else
   			{
   				if(win_money_gold)
   				{
   				gold_amount=new("/clone/money/gold");
 				gold_amount->set_amount(win_money_gold);
   				gold_amount->move(ob[i]);
   				}
   			}
    		silver_amount = present("silver_money", ob[i]);
    		if (objectp(silver_amount))
    		{
   				silver_amount->add_amount(win_money_silver);
   			}
   				else
   			{
   				if(win_money_silver)
   				{
   				silver_amount=new("/clone/money/silver");
   				silver_amount->set_amount(win_money_silver);
   				if (win_money_silver)
   					silver_amount->move(ob[i]);
   				}
   			}
   			if (!win_money_silver)
   			{
			message_vision(sprintf(CYN+"$N慧眼识英雄，黄富霸给了$N包括本金在内的%s两黄金。\n"+NOR,chinese_number(win_money_gold)), ob[i]);	
			//比武奖励
				if (random(ob[i]->query_kar()) > 22  && ob[i]->query_temp("dxdc/bet") >= 50){
				skills=ob[i]->query_skills();
				if (mapp(skills)) sname  = keys(skills);
				sname = sname - str_skill_other;
				
				skills_i=random(sizeof(sname));
				ob[i]->improve_skill(sname[skills_i], ob[i]->query_temp("dxdc/bet")/5 + random(20));
				tell_object(ob[i], YEL "通过对红黑双方的比武观摩，你只觉以前存在的诸多疑问豁然开朗，对"+to_chinese(sname[skills_i])+"有了新的感悟。\n"NOR
				);
				}	
			add("yingyu",-win_money_gold*100);
			}	
			else
			{
			message_vision(sprintf(CYN+"$N慧眼识英雄，黄富霸给了$N包括本金在内的%s两黄金%s两银子。\n"+NOR,chinese_number(win_money_gold),chinese_number(win_money_silver)), ob[i]);		
			//比武奖励
				if (random(ob[i]->query_kar()) > 22  && ob[i]->query_temp("dxdc/bet") >= 50){
				skills=ob[i]->query_skills();
				if (mapp(skills)) sname  = keys(skills);
				sname = sname - str_skill_other;
				
				skills_i=random(sizeof(sname));
				ob[i]->improve_skill(sname[skills_i], 10 + random(20));
				tell_object(ob[i], YEL "通过对红黑双方的比武观摩，你只觉以前存在的诸多疑问豁然开朗，对"+to_chinese(sname[skills_i])+"有了新的感悟。\n"NOR
				);
				}
			add("yingyu",-win_money_silver);
			add("yingyu",-win_money_gold*100);
			}
			
    	}
    	else{
    		if(query("winner")=="none")
    		message_vision(sprintf("真可惜,双方打成了平手，所以$N输了%s两黄金。\n",chinese_number(ob[i]->query_temp("dxdc/bet"))),ob[i]);
    		else
    		if(ob[i]->query_temp("dxdc/side")=="none")
				message_vision(sprintf("真可惜，$N压的是平局，但他们决出了胜负，所以$N输了%s两黄金。\n",chinese_number(ob[i]->query_temp("dxdc/bet"))), ob[i]);		
    			else
				message_vision(sprintf("真可惜，$N压的一方已经死了，所以$N输了%s两黄金。\n",chinese_number(ob[i]->query_temp("dxdc/bet"))), ob[i]);		
	    }
   	
	}
	ob[i]->delete_temp("dxdc"); //比赛结束清楚所有人的标记
	}
	

	if (objectp(winner))																						//记录比赛情况
	{
	log_file("skills/"+room->query("red_party"),sprintf("地下比武记录：%10s vs %10s(%3s)，exp：%2dM 双方加力：%3d。获胜的是：%8s %10s\n",	
		 	laoban->query("red_f_skill"),			 		
		 	laoban->query("black_f_skill"),
		 	room->query("black_party"),	
		 	room->query("base_exp")/1000000,
		 	room->query("base_jiali")*20,
			winner->query("family/family_name"),
			winner->query("good_skills")
			));
	}
	else
	{
	log_file("skills/dxdc",sprintf("地下比武记录：%s %s vs %s %s，exp：%dM 双方加力：%d。双方打平手\n",
		 	room->query("red_party"),
		 	laoban->query("red_f_skill"),
		 	room->query("black_party"),
		 	laoban->query("black_f_skill"),
		 	room->query("base_exp")/1000000,
		 	room->query("base_jiali")*20
		 	));						
	 }
	 	ob = all_inventory(this_object());
	for(i=0;i<sizeof(ob);i++)																	//清除场地上的不相关的东西
	{
		if(userp(ob[i]) || ob[i]==red_fighter || ob[i]==black_fighter || ob[i]==huoyan || ob[i]==laoban)
			continue;
		else
		{
			switch(ob[i]->query("id"))
				{
				case "corpse":
					message_vision("\n从台下跳上来几个壮汉，把$N拖了下去。\n", ob[i]);		
				break;
				default:
					message_vision(CYN"\n刮来一阵风把$N"+CYN+"卷走了。\n"NOR, ob[i]);		
				}
			destruct(ob[i]);	
		}
	}
	if(objectp(winner))																				//清除上一次冠军
		{
			message_vision(CYN"\n$N在观众的簇拥下离开了比赛场地。\n"NOR, winner);
			destruct(winner);
		}
	if(objectp(red_fighter))																				
		{
			message_vision(CYN"\n$N在观众的簇拥下离开了比赛场地。\n"NOR, red_fighter);
			destruct(red_fighter);
		}
	if(objectp(black_fighter))																				
		{
			message_vision(CYN"\n$N在观众的簇拥下离开了比赛场地。\n"NOR, black_fighter);
			destruct(black_fighter);
		}				
	
		ob = all_inventory(room);	
		strIpAddress = ({});
		for(i=0;i<sizeof(ob);i++)
    	{
    		if (!living(ob[i])) continue;
    		if( !userp(ob[i]) ) continue;	
    		if( wizardp(ob[i]) ) continue;
    		for (j=0;j<sizeof(strIpAddress);j++)
    		{	
    			if (strIpAddress[j] == query_ip_number(ob[i]))
    			{
    				message_vision(HIB"\n目前赌场太拥挤了，$N被挤了出去。\n"NOR, ob[i]);	
    				ob[i]->move("/d/city/dating1");
    			}    			
    		}
			strIpAddress = strIpAddress + ({query_ip_number(ob[i])});
		}

		call_out("wait_fight",60);																							//一分钟后再进行下一次比赛

return 1;
}

int checkSameIp(object ob)
{
	int j;
	
    for (j=0;j<sizeof(strIpAddress);j++)
    {	
    	if (strIpAddress[j] == query_ip_number(ob))
    	{
    		return 1;	
    	}    			
    }
	
	return 0;
}
int do_dadu(string arg)
{
	string side,target_name,c_side,other_side,c_other_side;
	int bet_gold,gold_in;
	object gold_amount;
	object target;
	object me=this_player();
	
	if(!arg || sscanf(arg, "%s %d with %s",side,bet_gold,target_name)!=3)
    	return notify_fail("dadu <某方> <金量> with <某人>\n");
    	if(!objectp(red_fighter) || !objectp(black_fighter))
    	return notify_fail("选手还没上场，你想赌什么？\n");
 	
 	if(query("state")=="") return notify_fail("选手还没来呢，你急什么？！\n");  
    	if(query("state")=="fighting") return notify_fail("比赛已经在进行中了，现在不接受投注了！\n");
    	if(query("state")=="bound") return notify_fail("正在计算上次比赛结果呢，你别着急啊！\n"); 
    	  	
   	target=present(target_name, environment(me));
   	if(!objectp(target))
   		return notify_fail("你要和谁打赌？\n");
   	if(!userp(target))
   		return notify_fail("你只能和玩家打赌！\n");
    	if(bet_gold>500 || bet_gold<50)
    	return notify_fail("和玩家打赌，至少要赌五十两黄金！最多也只接受五百两黄金！\n");
    	if(target->query_temp("dxdc/dadu/target")==me->query("id") && target->query_temp("dxdc/dadu/state")=="dadu") return notify_fail(target->query("name")+"不是已经和你在打赌了吗？！\n");
   	if(target->query_temp("dxdc/dadu/state")=="dadu") return notify_fail(target->query("name")+"已经在和别人打赌了，你就不要参和了！\n");
    	if(me->query_temp("dxdc/dadu/state")=="dadu") return notify_fail("你不是已经在和"+me->query_temp("dxdc/dadu/target")+"打赌了？！\n");
    	if(side!="red" && side!="black")
    	return notify_fail("你要赌哪一方的生死？\n");
    	if(side=="red") c_side=HIR"红方"NOR; else c_side=HIB"黑方"NOR;
    	if(side=="red") c_other_side=HIB"黑方"NOR; else c_other_side=HIR"红方"NOR;
    	if(side=="red") other_side="black"; else other_side="red";
   	gold_amount = present("gold_money", me);
   	if(!objectp(gold_amount))
   		return notify_fail("你身上什么地方能藏钱的？\n");
	gold_in=gold_amount->query_amount();
	if(gold_in<bet_gold)
		return notify_fail("没有钱充什么大款？\n");   	
   
    	me->set_temp("dxdc/dadu/state","wait");
    	me->set_temp("dxdc/dadu/side",side);
    	me->set_temp("dxdc/dadu/target",target_name);
    	me->set_temp("dxdc/dadu/bet",bet_gold);
   // 	target->set_temp("dxdc/dadu/opbet",bet_gold);                                会造成严重bug
    
    	if(target->query_temp("dxdc/dadu/target")!=me->query("id"))
    	{
		message_vision(sprintf("$N对着$n说道："+RANK_D->query_respect(target)+"，"+RANK_D->query_self(me)+"愿出%s两黄金，赌%s会赢，你敢跟我赌么？\n",chinese_number(bet_gold), c_side), me,target);	
		me->start_busy(random(3));
		tell_object(target, YEL "如果你对"+c_other_side+ YEL + "很有信心的话，可以和" + me->name()+ "("
				+me->query("id")+")打赌，你可以输入（dadu "+other_side+" "+bet_gold+" with "+ me->query("id")+"）的命令。\n"NOR
			);	
    	}
    	else
    	{
    	if(bet_gold < target->query_temp("dxdc/dadu/bet")) return notify_fail("你不能出的比他少吧？\n");
    	if(bet_gold != target->query_temp("dxdc/dadu/bet")) return notify_fail("打赌双方的赌注金额应该是一样的！\n");
    	if(side==target->query_temp("dxdc/dadu/side")) return notify_fail("和对方赌相同一方赢？那还有什么可赌的？\n");
   		gold_amount = present("gold_money", target);
   		if(!objectp(gold_amount))
   			return notify_fail(target->query("name")+"身上已经身无分文了。\n");
		gold_in=gold_amount->query_amount();
		if(gold_in<bet_gold)
			return notify_fail(target->query("name")+"身上的钱已经不够和你赌了。\n");     	
		message_vision(sprintf("$N瞥了$n一眼，说道："+RANK_D->query_self(me)+"我跟你%s两黄金，我赌%s会赢，哈哈。\n",chinese_number(bet_gold), c_side), me,target);		
		message_vision(sprintf("$N把%s两保证金交给了黄老板，黄老板是这次私下赌博的公证人。\n",chinese_number(target->query_temp("dxdc/dadu/bet"))), target);
		message_vision(sprintf("$N把%s两保证金交给了黄老板，黄老板是这次私下赌博的公证人。\n",chinese_number(target->query_temp("dxdc/dadu/bet"))), me);
		gold_amount->add_amount(-target->query_temp("dxdc/dadu/bet"));
   		gold_amount = present("gold_money", me);
		gold_amount->add_amount(-bet_gold);
		me->set_temp("dxdc/dadu/state","dadu");
		target->set_temp("dxdc/dadu/state","dadu");
    	}
  
    return 1;
	
	
	
}


int do_bet(string arg)
{
	object me;																													//下赌注的玩家
	object gold_amount;																											//玩家身上所带的ｇｏｌｄ数量
	string side,c_side;
																													//玩家下注的一方
	int bet_gold;																												//玩家所下注金额	
	me=this_player();
		
	if(!arg || !sscanf(arg, "%s %d",side,bet_gold))
                return notify_fail("yazhu <某方> <金量>\n");
    	if(query("state")=="") return notify_fail("选手还没来呢，你急什么？！\n");  
    	if(query("state")=="fighting") return notify_fail("比赛已经在进行中了，现在不接受投注了！\n");
    	if(query("state")=="bound") return notify_fail("正在计算上次比赛结果呢，你别着急啊！\n");
  	if(side=="both" || side=="none") side="none";
    if(side=="red") c_side=HIR"红方"NOR;
    	else  if (side=="black") c_side=HIB"黑方"NOR;
    		else c_side=HIW"平局"NOR;
    		
	if(side!="red" && side!="black" && side!="none" && side!="both") return notify_fail("你到底要赌哪一方赢，想清楚来再来！\n");
	if(!objectp(gold_amount = present("gold_money", me))) return notify_fail("你身上都没钱了，还赌什么？快去攒点吧！\n");
	if(bet_gold<1) return notify_fail("你压的是多少钱？我怎么看不懂啊？\n");
	if((int)me->query_temp("dxdc/bet")+bet_gold>100) return notify_fail("本赌场虽然是黑市，可输不起这些钱。\n");
	if((int)gold_amount->query_amount()<bet_gold) return notify_fail("嘿嘿，您是不是看花了眼了？就你这点钱还想赌？\n");
	
	if(!objectp(red_fighter) || !objectp(black_fighter))
		return notify_fail("比武双方还没到齐，你就下注了？\n");
	
	if(!me->query_temp("dxdc/bet")) 
	{
		me->set_temp("dxdc/bet",bet_gold);
		me->set_temp("dxdc/side",side);
		message_vision(sprintf("$N从身上拿出%s两黄金，压在%s上．\n",chinese_number(bet_gold), c_side), me);		
	}
	else
	{ 
		if(me->query_temp("dxdc/bet")>=100)
				return notify_fail("这位爷，这局您下注已经达到１００两了，再加我们可承受不起了。\n");
		if(me->query_temp("dxdc/side")!=side)
				return notify_fail("这里可不准一个人同时赌两个人输赢。\n");
		me->add_temp("dxdc/bet",bet_gold);
		message_vision(sprintf("$N从身上又拿出%s两黄金，追压在%s上。\n",chinese_number(bet_gold), c_side), me);		
	}
	gold_amount->add_amount(-bet_gold);
    	me->start_busy(1);
	add(side+"_money",bet_gold);
	add("yingyu",bet_gold*100);
	
	
	if(query("red_money") && query("black_money"))
	{ 
	if(query("red_money") / query("black_money")>=5){													//设定赌场的赔率
		set("black_betrate",(query("red_money") / query("black_money") - 1.5)*100);
		if (query("black_betrate") > 2000)
			set("black_betrate",2000);
	}
	else
		if(query("red_money") / query("black_money")>=2)
			set("black_betrate",140);
		else
			set("black_betrate",130);
	if(query("black_money") / query("red_money")>=5){
		set("red_betrate",(query("black_money") / query("red_money") - 1.5)*100);
		if (query("red_betrate") > 2000)
			set("red_betrate",2000);
	}
	else
		if(query("black_money") / query("red_money")>=2)
			set("red_betrate",140);
		else
			set("red_betrate",130);			
	}
    
	return 1;	
}


//int valid_leave(object me, string dir)
//{
//	if (( me->query_temp("dxdc/bet") || me->query_temp("dxdc/dadu/state")=="dadu") && dir == "up" )
//	return notify_fail("怎么？看到这里死人了就想开溜？给我好好坐着看比赛！\n");
//	if (userp(me) && !wizardp(me))
//	strIpAddress = strIpAddress - ({query_ip_number(me)});
//
//
//	return ::valid_leave(me, dir);
//}

object query_red_fighter()
{
	return red_fighter;
}

object query_black_fighter()
{
	return black_fighter;
}