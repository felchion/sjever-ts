// Room: /d/city/chmiao.c
// Modify by Lklv 2001.10.16
#include <mudlib.h>
#include <ansi.h>
inherit ROOM;

int DB_last_query;

void create()
{
        set("short", "城隍庙");
        set("long", @LONG
这是扬州城北的城隍庙。庙里平日香客稀少，赶上中秋节、端午节或者庙
会的时候，才会有些香火。堂中放着一个城隍的塑像，上面落满了灰尘，墙角
里有个破牌子(paizi)上面结着蜘蛛网，两只蜘蛛无聊地蹲在上面。
LONG    );
        set("exits", ([
                "east" : __DIR__"beidajie1",
//                "north" : __DIR__"mine_room",
                "northwest" : "/d/wizard/guest_room",
        ]));
        set("no_fight", 1);
        set("no_sleep_room", 1);
        set("objects", ([
                __DIR__"npc/ymonk" : 1,
        	__DIR__"obj/box" : 1
        ]));
	setup();
        "/clone/board/wizto_b"->foo();
}

void init()
{
	object me = this_player();
	if (me->query("qiankun/qi") && me->query("qiankun/neili")) {
		me->set("max_qi",me->query("qiankun/qi"));
		me->set("max_neili",me->query("qiankun/neili"));
		me->delete("qiankun/qi");
		me->delete("qiankun/neili");
	}

	me->delete_temp("die_record");
	add_action("action", ({
		"dazuo",
		"du",
		"exercise",
		"lian",
		"practice",
		"respirate",
		"study",
		"tuna"
	}));
	add_action("do_repent","repent");
	add_action("do_donate",({ "juan", "donate" }));
	add_action("do_exchange","exchange");
	add_action("do_look","look");
	add_action("do_trade","convert");
}

int action()
{
        write("你无法静下心来修炼。\n");
        return 1;
}


int do_look(string arg)
{
	object me = this_player();

	if(me->is_busy())
	return notify_fail("你正忙着呢。\n");

	if (arg == "paizi" ){
    write("这是一个莫名其妙形状怪异的破牌子，好象不是这个世\n");
    write("界的东西。你隐隐约约能认出上面写着：\n\n");
    write(HIY"         捐赠：   donate\n\n");
    if(GIFT_D->query("fudai"))
    write(HIY"         抽奖：   draw\n\n");
  //  write(HIY"         偿还：   payback\n\n");
  //  write(HIY"         悔过：   repent\n\n");
    write(HIY"         转换：   exchange [通宝数量]\n\n"NOR);
    write(HIY"         兑换：   convert 永恒币数量\n\n"NOR);
    return 1;
  }
  return 0;
}


int do_trade(string arg) 
{
	int i,j;
	object me = this_player();
	mixed ret; 
		
	if(me->is_busy()) 
		return notify_fail("你正忙着呢。\n");
  		
	if(!arg || !sscanf(arg, "%d", i))
		return notify_fail("命令格式：convert 永恒币数量。\n"); 
  
  if(i>1000)
  	return notify_fail("一次最多只能兑换一千个永恒币。\n"); 
  			
	if(i<10)
		return notify_fail("一次最少要兑换十个永恒币。\n"); 
		
	if (time()-me->query("last_dbquery")<3)
  	return notify_fail("你操作太频繁了，请稍后再试。\n");
  	
	ret = dbquery("SELECT extcredits3 FROM members WHERE username='"
			+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'");
	
	if (!ret) 
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	if (sizeof(ret)<1)
	  return notify_fail(HIR"您的论坛账号太长时间没有登录，无法同步数据。\n请登录论坛账号后再试。\n"NOR) ; 
	  
	j = ret[0][0];	
	j = j - i*10;
	if ( j < 0)
		return notify_fail("你的通宝不够！\n");
	
	if(!dbquery("update members set extcredits3="+sprintf("%d",j)+" WHERE username='"	+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'"))	
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	me->set("last_dbquery",time());
	me->add("SJ_TC",i);
	me->add("BBS_trade",i*10);

       
	tell_object(me,sprintf(HIW"你将%s个书剑币"HIW"兑换成%s个"HIR"永恒币"HIW"。\n"NOR,chinese_number(i*10),chinese_number(i)),me);
	tell_object(me,"您目前还剩" + chinese_number(j)+"个书剑币。\n");
	
	log_db(me,0,i,j,"永恒币");
  
  me->start_busy(1);			
	
	return 1; 
}


void do_recover(object ob, int j)
{
	int i,tmp;

//	if (j < 1 || j > 5) return;
	while(j--) {
		if ((tmp=ob->query("combat_exp")/199+1)<50000)
		  ob->add("combat_exp",tmp);
		else 
		  ob->add("combat_exp",50000);
		i = ob->query("potential");
		i += i/3;
		if (i > ob->query("max_pot")) i = ob->query("max_pot");
		ob->set("potential", i);
		i = ob->query("death_count") - 1;
		if (i < 0) i = 0;
		ob->set("death_count", i);
		if (i < ob->query("death_times")) ob->set("death_times", i);
	}
	ob->reincarnate();
}

int do_exchange(string arg) {
	object ob=this_player();
	mixed ret,res;
	int i,j;

	if (time() - ob->query("last_db_query") < 3)
	  return notify_fail("系统忙稍后再试。\n");
	
	if (time() - DB_last_query < 3)
	  return notify_fail("系统忙稍后再试。\n");
			
	if(!arg || !sscanf(arg, "%d", i)) {
		DB_last_query = time();
		ob->set("last_db_query",time());
		ret = dbquery("SELECT extcredits3,extcredits4 FROM members WHERE username='"
		+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
		if (sizeof(ret)==1) {
			if(!ret[0][1]){ 
			  tell_object(ob,YEL"你没有需要转换的社区通宝。\n"NOR);
			  return 1;
			}  
			if (ret[0][1]>0) {
				dbquery("UPDATE members set extcredits4 = 0 WHERE username='"
				+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
				ob->add("SJ_Credit",ret[0][1]);
				tell_object(ob,HIW+sprintf("你在社区获得的%s个"HIG"通宝"HIW"已经转入帐户。\n"NOR,chinese_number(ret[0][1])));
				log_file( "static/BBSCREDIT",sprintf("%s(%s)转换%d个游戏通宝\n", ob->name(),ob->query("id"),ret[0][1]));
			}
			return 1;
		}
		else 
		  return notify_fail("你因太长时间没有登录社区造成数据无法同步，请登录社区后重试。\n");	  
	}
	
	if(i>10000)
	return notify_fail("一次最多只能转换一千个通宝。\n");

	if(i<100)
	return notify_fail("一次最少要转换一百个通宝。\n");


	DB_last_query = time();
	ob->set("last_dbquery",time());

	ret = dbquery("SELECT extcredits3 FROM members WHERE username='"
	+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");

	if (sizeof(ret)==1) {
		if ((j=ret[0][0]-i)>0) {
			res=dbquery("UPDATE members set extcredits3 ="+j+"  WHERE username='"
			+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
			ob->add("SJ_Credit",i);
			tell_object(ob,HIW+sprintf("你在社区获得的%s个书剑币已转换为"HIG"通宝"HIW"存入你的个人帐户。\n"NOR,chinese_number(i)));
			log_db(ob,0,i,ob->query("SJ_Credit")+"/"+j,"转换通宝");
		}
		else tell_object(ob,YEL"你没有足够的社区书剑币用于转换通宝。\n"NOR);
		return 1;
	}
	else
	return notify_fail("你因太长时间没有登录社区造成数据无法同步，请登录社区后重试。\n");
	
}

int do_donate() 
{
	object me = this_player();
	int x;


	if (me->query("balance")<10000000)
		return notify_fail("你存款太少无法捐赠。\n"); 
	
	me->add("balance",-10000000);
	x=me->add("honor_point",100);
  CHANNEL_D->do_channel(present("xiao heshang", environment(me)), "chat", me->name()+"施主真是个大好人啊!");
  me->save();
   
	tell_object(me,HIY"你捐赠黄金千锭获得一百点江湖声望。\n"NOR);
	log_file( "static/donate",sprintf("%s(%s)捐赠一次(%d)。\n", me->name(),me->query("id"),x));
  me->start_busy(1);		
	return 1; 
}

int do_repent() 
{
	int i,j;
	object me = this_player();
	mixed ret; 
	
	if(me->is_busy()) 
		return notify_fail("你正忙着呢。\n");		
 	
	ret = dbquery("SELECT extcredits2 FROM members WHERE username='"
			+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'");
	
	if (sizeof(ret)!=1) 
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	i = ret[0][0];
	
	if(i > -1)	
		return notify_fail("你现在不需要悔罪。\n"); 
		
	j = i * 5;

	if (me->query("max_neili")<-j)
		return notify_fail("你内力修为不够不能忏悔。\n"); 
	
	if(!dbquery("update members set extcredits2=0 WHERE username='"	+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'"))	
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	me->add("max_neili",j);
  me->save();
   
	tell_object(me,"你因恶意灌水被扣除了"+ chinese_number(-j)+"点内力，你的社区负资产已清零。\n");
	log_file( "static/repent",sprintf("%s(%s)悔过，扣除内力%d点。\n", me->name(),me->query("id"),j));
  me->start_busy(5);		
	return 1; 
}