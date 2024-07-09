// Room: /d/city/dangpu.c
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

//	=({
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//		([]),
//	});


void getspec();
int do_look(string arg);
int do_exchange(string arg);
int do_trade(string arg);

mapping *special;
	

void create()
{
	set("short", "当铺");
	set("long", 
"这是一家素以买卖公平著称的老字号当铺，一个四尺高的柜台摆在你的面\n"+
"前，柜台上摆着一个牌子(paizi)， 柜台后坐着当铺的老板，一双鬼溜溜的眼\n"+
"睛上上下下打量着你，听说这里正在大量收纳『"+HBGRN+HIY+"社区元宝"+NOR+"』和『"+HBGRN+HIG+"书剑通宝"+NOR+"』。\n"
);
	set("no_fight",1);
	set("no_sleep_room",1);
	set("objects", ([
		__DIR__"npc/huang" : 1,
	]));
	set("exits", ([
         "north" : __DIR__"guangchangnan",
	]));

	set("coor/x",110);
  set("coor/y",-30);
  set("coor/z",0);
	getspec();
	setup();
}

void init()
{
	add_action( "do_exchange", "duihuan" );
	add_action( "do_trade", "jiaoyi" );
	add_action( "do_look", "look" );
}

void getspec()
{
	mixed *file;
	int i;
	object ob;
	string tpw;

	file = get_dir("/clone/spec/");

	if( !sizeof(file) ) return;
	special=allocate(sizeof(file));
	for( i = 0; i < sizeof(file); i++ )	{	
		tpw = "";
		file[i] = "/clone/spec/" + file[i];
		if( file_size(file[i]) < 0 ) continue;
		ob = new(file[i]);

		if(!objectp(ob)) continue;
		if(!ob->query("credit")) continue;

		if (!ob->query("rest"))
		tpw = "无";
		else
		tpw = "每周"+chinese_number(ob->query("rest"))+"次";
		special[i]=([
			"value"		:	ob->query("credit"),
			"name"		:	ob->query("name"),
			"id"			:	ob->query("id"),
			"desc"		:	ob->query("desc"),
			"degree"	:	ob->query("degree"),
			"tpw"			:	tpw,
			"file"		:	file[i]
		]);
		destruct(ob);
	}
	return;
}


int do_look(string arg)
{
	object me = this_player();
	int i;

	if(me->is_busy())
	return notify_fail("你正忙着呢。\n");


	if (arg == "paizi" ){
		write(query("special"));
		write(" 本店现在收纳社区元宝和书剑通宝，可以兑换的限制级宝物如下：\n");
		write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		write(sprintf("┃ %-20s  %-45s  %-10s %-12s %-6s┃\n", "宝物名称","宝物作用","可用次数","使用限制","通宝" ));
		write("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
		for (i=0;i<sizeof(special);i++) {
			if (!special[i] || !special[i]["value"]) continue;
			write(sprintf("┃ %-20s  %-46s %-10s %-12s "+HIY+"%-6d"+NOR+"┃\n", special[i]["name"]+"("+capitalize(special[i]["id"])+")",special[i]["desc"],chinese_number(special[i]["degree"])+"次",special[i]["tpw"],special[i]["value"] ));
		}
		write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
		write(" 使用“"HIG"duihuan <宝物id>"NOR"”兑换宝物，使用“"HIG"jiaoyi <通宝数量>"NOR"”进行通宝交易。\n");
		return 1;
	}
	return 0;
}

int do_exchange(string arg) 
{
	int i,j;
	object ob,me = this_player();
	
	if(me->is_busy()) 
		return notify_fail("你正忙着呢。\n");
  		
	if(!arg)
		return notify_fail("你想兑换什么。\n"); 
  		
	for (i=0;i<sizeof(special);i++)
	if (special[i] && arg == special[i]["id"])  
		j=i+1;	
	if(!j)
		return notify_fail("这里没有你想兑换东西。\n");
        
  if (special[j-1]["value"]>me->query("SJ_Credit"))
		return notify_fail("对不起你现有的通宝不够兑换"+special[j-1]["name"]+"。\n");
	
	ob = new(special[j-1]["file"]); 
	if(!objectp(ob))
		{return notify_fail("系统错误请联系WIZ。\n");}
	if (!ob->restrict()) {
		destruct(ob);
		return 0;	
	}
	if (present(ob->query("id"), me)){
		destruct(ob);
		return notify_fail("你先要用完现有的物品才能再次兑换。\n");	
	}	
	me->add("SJ_Credit",-special[j-1]["value"]);
	me->add("SJ_Credit_Used",special[j-1]["value"]);
	ob->move(me,1);    
	message_vision(sprintf("当铺老板吆喝一声：%s兑换限制级宝物%s，收讫书剑通宝%s个。\n",me->query("name"),special[j-1]["name"],chinese_number(special[j-1]["value"])),me);
	tell_object(me,"您目前还剩"
		+ chinese_number(me->query("SJ_Credit"))+"个书剑通宝，您已经累计消费了" 
		+ chinese_number(me->query("SJ_Credit_Used")) +"个书剑通宝。\n");
	log_file("static/CREDIT",sprintf("%s(%s) | 购买%s | 剩:%d | 用:%d | 累计:%d.\n", me->name(), me->query("id"), special[j-1]["name"],me->query("SJ_Credit"),special[j-1]["value"],me->query("SJ_Credit_Used")));
  log_file("static/spec"+special[j-1]["file"][11..strlen(special[j-1]["file"])],sprintf("%s(%s)购买%s",me->name(), me->query("id"), special[j-1]["name"]));
  me->start_busy(1);
	me->save();	
	return 1; 
}

int do_trade(string arg) 
{
	int i,j;
	object me = this_player();
	mixed ret; 
		
	if(me->is_busy()) 
		return notify_fail("你正忙着呢。\n");
  		
	if(!arg || !sscanf(arg, "%d", i))
		return notify_fail("你想交易多少通宝。\n"); 
  
  if(i>10000)
  	return notify_fail("一次最多只能交易一万个通宝。\n"); 
  			
	if(i<10)
		return notify_fail("一次最少要交易十个通宝。\n"); 
		
	if (time()-me->query("last_dbquery")<10)
  	return notify_fail("你操作太频繁了，请等十秒后再试。\n");
  	
	ret = dbquery("SELECT extcredits2 FROM members WHERE username='"
			+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'");
	
	if (!ret) 
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	if (sizeof(ret)<1)
	  return notify_fail(HIR"您的论坛账号太长时间没有登录，无法同步数据。\n请登录论坛账号后再试。\n"NOR) ; 
	  
	j = ret[0][0];	
	j = j - i*10;
	if ( j < 0)
		return notify_fail("当铺老板说道：你的元宝不够！\n");
	
	if(!dbquery("update members set extcredits2="+sprintf("%d",j)+" WHERE username='"	+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'"))	
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
	
	me->set("last_dbquery",time());
	me->add("SJ_Credit",i);
	me->add("BBS_Credit_Used",i*10);
  me->set("BBS_Credit",j);
   
	message_vision(sprintf("当铺老板吆喝一声：%s兑换书剑通宝%s个，收讫社区元宝%s个。\n",me->query("name"),chinese_number(i),chinese_number(i*10)),me);
	tell_object(me,"您目前还剩"
		+ chinese_number(j)+"个社区元宝，您已经累计交易了" 
		+ chinese_number(me->query("BBS_Credit_Used")) +"个社区元宝。\n");
	log_file( "static/BBSCREDIT",sprintf("%s(%s) | 兑换%d通宝 | 剩:%d元宝 | 累计:%d\n", me->name(),me->query("id"),i, j,me->query("BBS_Credit_Used")));
  me->start_busy(1);		
	return 1; 
}