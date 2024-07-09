//guigushu.c created by campsun 2003.12.4
//LinuX@SJ 2004.2.28


#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	int sNow;
	int sLeftTime;
	int sStartTime;
	int this_week = time()/86400/7;
	int user_time;
	//int an_hour = 60*60;
	
	if(!me->query("ggs/started")) return 0;
	
	if(me->query("registered")<3)
	{
		tell_object(me, HIG"\n天空突然打了几个响雷，原来你的贵宾时间到期了，你的鬼谷算术自动中止掉。\n"NOR);	
		
		if (me->query("ggs/left_time") < 0 ) //防止bug
			me->set("ggs/left_time",0);
			
		me->set_temp("ggs/started",0);//不要delete，否则玩家重新激活贵宾，就有2次10个小时了。
		me->set("ggs/started",0);
		me->set("ggs/start_time","");
		me->set("ggs/over_time","");
					
		me->clear_condition("guigushu"); //清除了	
			
		return 0;
	}	
	
	if(me->query("ggs/started") && !me->query_temp("ggs/started"))
	{
		tell_object(me, YEL"\n你感觉瑛姑用鬼谷算术术对你进行控制，原来是你上次下线前没有结束鬼谷算术状态。\n"NOR);
		tell_object(me, YEL"瑛姑运用神功把你鬼谷算术的结束时间变成你的上次下线时间。\n"NOR);
		me->set("ggs/over_time",FINGER_D->get_last_on(me->query("id")));
		user_time=(int)me->query("ggs/over_time")-(int)me->query("ggs/start_time");
		me->set("ggs/left_time",me->query("ggs/left_time") - user_time);
		
		if (me->query("ggs/left_time") < 0 ) //防止bug
			me->set("ggs/left_time",0);
			
		if (me->query("ggs/left_time")) 
			tell_object(me, YEL"你本周还可以使用鬼谷算术"+CHINESE_D->chinese_time(me->query("ggs/left_time"))+"。\n"NOR);
		else
			tell_object(me, YEL"你本周的鬼谷算术时间用完了。\n"NOR);
		
		me->set_temp("ggs/started",0);
		me->set("ggs/started",0);
		me->set("ggs/start_time","");
		me->set("ggs/over_time","");	
		
		me->clear_condition("guigushu"); //清除了
		
		
		return 0;
	}
	
	sNow = time();
	
	sStartTime =(int) me->query("ggs/start_time");
	
	
	if ( (sNow - sStartTime) == 3600 )
	{
		tell_object(me, HIG"时间过的很快，不知不觉，你用鬼谷算术已经一个小时过去了。\n"NOR);
		//me->add("ggs/left_hours",-1);
		return 0;
	
	}
	
	if (me->query("ggs/left_time") < 0 ) //防止bug
			me->set("ggs/left_time",0);
		
	sLeftTime =(int) me->query("ggs/left_time");
			
	if (sLeftTime < (sNow - sStartTime))
	{
		tell_object(me, HIR"你脑子里嗡的一下，感觉精神疲惫之极，你这个星期里已无法再支撑鬼谷算术状态了。\n"NOR);
		message_vision(BLU"$N一阵头晕目眩，无法再使用鬼谷算术加强学习能力了！\n"NOR, me);
		log_file( "quest/ggs",sprintf("%s(%s)在第%d周没有询问over自动结束.\n",
			me->name(),
			me->query("id"),
			this_week
			));
		
		me->set_temp("ggs/started",0);
		me->set("ggs/started",0);
		me->set("ggs/start_time","");
		me->set("ggs/over_time","");
		me->set("ggs/left_time",0);
		
		me->clear_condition("guigushu"); //结束了就清除
		return 0;
	}
	
	if (this_week > me->query("ggs/last_week"))
	{
		me->set("ggs/left_time",me->query("ggs/left_time") - sNow + sStartTime);
		tell_object(me, HIR"\n你脑子里嗡的一下，感觉精神疲惫之极，你这个星期里已无法再支撑鬼谷算术状态了。\n"NOR);
		tell_object(me, HIR"你为自己浪费了鬼谷算术时间而懊悔不已，你大概浪费了"+	CHINESE_D->chinese_time(me->query("ggs/left_time"))+"。\n"NOR);
		message_vision(BLU"$N一阵头晕目眩，无法再使用鬼谷算术加强学习能力了！\n"NOR, me);
		
		log_file( "quest/ggs",sprintf("%s(%s)在第%d周自动结束,浪费:%s\n",
			me->name(),
			me->query("id"),
			this_week,
			CHINESE_D->chinese_time(me->query("ggs/left_time"))
			));
		
		me->set_temp("ggs/started",0);
		me->set("ggs/started",0);
		me->set("ggs/start_time","");
		me->set("ggs/over_time","");
		me->set("ggs/left_time",0);
		
		me->clear_condition("guigushu"); //结束了就清除
		return 0;
	}
	
	if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "看起来面目清朗，神思敏捷。\n", ({ me }));
		tell_object(me, HIG"在鬼谷算术的作用下，你感觉天地之间没有你不能理解的事。\n"NOR);
	}
    	return CND_CONTINUE;
}

string query_type(object me)
{
        return "guigushu";
}