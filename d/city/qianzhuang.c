// Room: /city/qianzhuang.c
#include <ansi.h>
inherit ROOM;



mapping *special;

void getspec()
{
	mixed *file;
	int i;
	object ob;

	file = get_dir("/clone/stmall/");

	if( !sizeof(file) ) return;
	special=({});
	for( i = 0; i < sizeof(file); i++ )	{	
		file[i] = "/clone/stmall/" + file[i];
		if( file_size(file[i]) < 0 ) continue;
		ob = new(file[i]);

		if(!objectp(ob)) continue;
		if(!ob->query("credit")) continue;

		special+=({ ([
			"value"		:	ob->query("credit"),
			"name"		:	ob->query("name"),
			"id"			:	ob->query("id"),
			"desc"		:	ob->query("desc"),
			"file"		:	file[i]
		]) });
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

	if (arg != "paizi" ) return 0;
	
	write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	write(sprintf("┃ %-28s  %-38s %-10s┃\n", "道具名称","道具作用","永恒币" ));
	write("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
	for (i=0;i<sizeof(special);i++) {
		if (!special[i] || !special[i]["value"]) continue;
		write(sprintf("┃ %-28s %-40s  "+HIY+"%-8d"+NOR+"┃\n", special[i]["name"]+"("+capitalize(special[i]["id"])+")",special[i]["desc"],special[i]["value"] ));
	}
	write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	write(" 使用命令“"HIY"duihuan <道具id>"NOR"”可兑换对应道具。\n");
	return 1;
}

int do_exchange(string arg) 
{
	int i,j,k;
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
        
	ob = new(special[j-1]["file"]); 
	
	if(!objectp(ob))
		{return notify_fail("系统错误请联系WIZ。\n");}

	if (!ob->restrict()) {
		destruct(ob);
		return notify_fail("你暂时无法使用这个物品。\n");
	}

	if (present(ob->query("id"), me)){
		destruct(ob);
		return notify_fail("你先要用完现有的物品才能再次兑换。\n");	
	}	

 	if (special[j-1]["value"] > me->query("SJ_TC"))
		return notify_fail("对不起你现有的永恒币不够兑换"+special[j-1]["name"]+"。\n");
	me->add("SJ_TC",-special[j-1]["value"]);
	me->add("SJ_TC_Used",special[j-1]["value"]);
	tell_object(me,sprintf("你兑换%s，花费永恒币%s个。\n",special[j-1]["name"],chinese_number(special[j-1]["value"])));		

	log_db(me,ob,special[j-1]["value"],me->query("SJ_TC"),"永恒道具");
  
	ob->set("owner",me->query("id"));
	ob->move(me,1);
  me->start_busy(1);
	me->save();	
	
	return 1; 
}




void create()
{
        set("short", "天阁斋");
        set("long", @LONG
这是一家老字号的钱庄，已有几百年的历史，在全国各地都有分店。它发
行的银票信誉非常好，通行全国。听说这里正在大量收纳永恒币，在柜台
上摆着一个牌子(paizi)。
LONG
        );
        
        set("no_fight",1);
        set("exits", ([
          "east" : __DIR__"beidajie2",
        ]));
        
        set("objects", ([
          __DIR__"npc/qian" : 1,
          __DIR__"npc/dashou" : 1,
        
        ]));
                
        set("coor/x",110);
        set("coor/y",10);
        set("coor/z",0);
        set("coor/x",110);
        set("coor/y",10);
        set("coor/z",0);
        set("coor/x",110);
        set("coor/y",10);
        set("coor/z",0);
        set("incity",1);
        getspec();
        setup();

}


void init()
{
	add_action( "do_exchange", "duihuan" );
	add_action( "do_look", "look" );
}