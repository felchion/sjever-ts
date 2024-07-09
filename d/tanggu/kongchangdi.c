#include <ansi.h>
inherit ROOM;
string look_leitai();
string look_qi();
string look_board();

void create()
{
        set("short", HIY"广场空地"NOR);
        set("long", "此处内是塘沽繁华之所，只见红楼画阁，绣户朱门，雕车竞驻，骏马争驰。一
片开阔的场地上，但听得前面人声喧哗，喝彩之声不绝于耳，远远望去，围着好大
一堆人，只见中间老大一块空地，立着一方"NOR+HIR"擂台"NOR+"，地下插了一面锦旗（Qi），白底
红花，绣着『"NOR+HIY"比武 招亲"NOR+"』四个金字，旗下一块白板（Board）。旗下两人正自拳来
脚去的打得热闹，一个是红衣少女，一个是长大汉子。\n");

        set("no_fight", 1);
        set("no_sleep_room", 1);
        set("leitai", __DIR__"biwuleitai");
        set("exits", ([ 
			             "northwest" : "/d/tanggu/center",
        ])); 
        set("objects", ([    
             	__DIR__"npc/mu-nianci" : 1,
				__DIR__"npc/manghan" : 1,
        ]));
        set("item_desc",([            
            "qi" : (: look_qi :),
				"leitai" : (: look_leitai :),
				"board" : (: look_board :),
        ]));
        set("outdoors", "长安");  
        setup(); 
}

void init()
{
	object *ob;
	object me=this_player();
	int i,p=0;
    if(me->query_condition("killer"))
	{
		message_vision(HIC"\n突然，穆念慈拍拍$N的脑袋：我们和不想官府作对。\n"NOR,me); 
		tell_object(me,HIR"跟着，不知道哪里来的一只神脚飞起一脚，将你一脚踢飞出去！\n"NOR);
		me->move("/d/xiangyang/damen");
		tell_room(environment(me), HIR"\n只见"+ me->name()+ "大叫一声飞过来，摔了个狗吃屎！好帅哦~~~~~~~~\n"NOR, ({ me }));
		me->start_busy(1);
		return;
	}
    
	ob = all_inventory(this_object());
	i = sizeof(ob);
	while (i--) {
		if(userp(ob[i])) p++;
	}
	if( p > 15 && !wizardp(me))
	{
		me->start_busy(1);
		me->move("/d/xiangyang/damen");
		tell_object(me,query("short")+"已经人满为患，你挤不进去！\n");
		return;
	}
	add_action("do_chihe",({"chihe"}) );
	add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
                             "kill", "fight", "study", "practice","exercise", "exert",}));
}
int do_chihe(string arg)
{
     object obj,me = this_player();
	 int money = 50000;		 
	 if(arg) return 0;
	 obj=present("mu nianci",this_object());
	 if(!obj) return 0;
	 if(!me->query_temp("quest/bwzh/hoster") 
		 && !me->query_temp("quest/bwzh/biwu_host")
		 && !me->query_temp("quest/bwzh/biwu_challenge"))
	   return notify_fail(HIR"穆念慈对你笑笑道：这位"+RANK_D->query_respect(me) +"这里不提供伙食！\n"NOR);
	 
	 if (me->query("food") > (me->query("str")*3+50) && me->query("water") >(me->query("str")*3+50))
	   return notify_fail(HIR"穆念慈对你笑笑道：这位"+RANK_D->query_respect(me) +"肚子不饿专心比武吧！\n"NOR);

	 if(me->query("balance")<money)
	   return notify_fail(HIR"穆念慈对你笑笑道：这位"+RANK_D->query_respect(me) +"银行存款....也忒少点吧？算了，还是专心比武吧！\n"NOR);

	 message_vision(HIC"\n$N正觉得有些肚子饥，只听身边的穆念慈把手一招，走过一个小生，穆念慈小声吩咐了几句。\n"NOR, me);
	 message_vision(HIG"一会儿，有人给$N送了一些饭菜。好阔气的款待，$N爽快地大吃一顿。\n"NOR, me);
	 message_vision(HIY"穆念慈凑在$N耳边轻轻地道：恩，结账"+MONEY_D->money_str(money)+HIY"，我已经委托银行自动转扣了。\n"NOR, me);
	 
	 me->add("balance",-50000);
	 me->add("food", me->query("str")*10+100);
     me->add("water", me->query("str")*10+100);
	 return 1;
 }

string look_leitai()
{
    	object me, room;
    	me = this_player();
    
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";
    	me->start_busy(2);
    	if (!(room = find_object(query("leitai"))))
          	room = load_object(query("leitai"));
        if( !room ) 
        	return "比武擂台没有准备好，请通知巫师。\n";  
    	message_vision("$N挤到擂台边，伸长了脖子向上望去。\n", me);
    	"/cmds/std/look.c"->look_room(this_player(), room); 
    	return "";
}
string look_qi()
{
    	object me,obj,who;
		string str;
		int i,p;
    	me = this_player();
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";
    	me->start_busy(2);
		str  = "一面锦旗，白底红花，绣着『"+HIY"比武招亲"NOR+"』，底下几行小字：\n\n";
		str += "        『"+HIC"卧虎藏龙地，良辰美景天"NOR+"』\n\n";
		obj=present("mu nianci",this_object());
		if(obj && obj->query("station") && obj->query("bwzh/host_player"))
		{
			who=obj->query("bwzh/host_player/object");
			if(!who) return "";
			i=who->query("age")-obj->query("bwzh/age");
			if(i<18) i=18;
			p=who->query("age")+obj->query("bwzh/age");
			if(p>80) p=80;
			str += HIC"在锦旗的侧面一角，写着一个名字："+obj->query("bwzh/host_player/name")+"。\n"NOR;
			str += HIC"名字下面注明几个数字："+obj->query("bwzh/exp")+"|"+obj->query("bwzh/per")+"|"+i+"|"+p+"。\n"NOR;
		}
        tell_object(me,str);
    	return "";
}
string look_board()
{
    	object me,obj;
		string str;
    	me = this_player();
    	if (me->is_busy()) 
          	return "你正忙着呢。\n";

    	me->start_busy(2);
		obj=present("mu nianci",this_object());
		if(!obj || !obj->query("bwzh/winner_player"))
		{
			str  = "一块白板，正上方写着『"+HIY"比武招亲"NOR+"』标题，底下却什么也没写。\n";
		}
		else
		{
			if(obj->query("bwzh/challenge_player"))
				str  = "一块白板，正上方写着『"+HIY"比武招亲"NOR+"』标题，底下也写有两行字：\n";
			else 
				str  = "一块白板，正上方写着『"+HIY"比武招亲"NOR+"』标题，底下也写有一行字：\n";

			str += "\n             『"+HIC"擂  主"NOR+"』："+obj->query("bwzh/winner_player/name")+"\n";
			if(obj->query("bwzh/challenge_player"))
				str += "             『"+HIC"挑战者"NOR+"』："+obj->query("bwzh/challenge_player/name")+"\n";
		}
        tell_object(me,str);
    	return "";
}
int do_action(string arg)
{
	this_player()->start_busy(2);
   	write("\n你正要有所动作，突然身旁有人将你一拍：好好看比武，别乱动！\n\n");
   	return 1;
}
int valid_leave(object me, string dir)
{

   if (me->query_temp("quest/bwzh/hoster") ||me->query_temp("quest/bwzh/biwu_host")||me->query_temp("quest/bwzh/biwu_challenge"))
	   return notify_fail(HIR"穆念慈对你笑笑道：这位"+RANK_D->query_respect(me) +"还是专心点比武招亲吧！\n"NOR);
   me->delete_temp("quest/bwzh");
   return ::valid_leave(me, dir);
}





