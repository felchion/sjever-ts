// by dubei
// Modify By River@sj 99.5.24
#include <ansi.h>
#include <room.h>

inherit ROOM;
int do_jump(string arg);
int do_hit(string arg);

void create()
{
        set("short", YEL"山洪中"NOR);
        set("long", @LONG
这里是山洪之中，水流激荡，山洪从头顶轰隆轰隆的冲过，隐隐听到轰轰
之声，不绝於耳，声音越响，显是极大的水声。山峰间一条大白龙似的瀑布奔
泻而下，冲入一条溪流，奔胜雷鸣，湍急异常，水中挟著树枝石块，转眼便流
得不知去向。看来功力稍弱之人，难以在此支撑过一柱香的时分。
LONG
    );

        set("outdoors", "襄阳");
        set("no_clean_up", 0);
        setup(); 
}

void init()
{
        add_action("do_jump", ({"jump", "tiao", "yue"}));
        add_action("do_hit", "ji");
}
 
int do_jump(string arg)
{
        object me = this_player();
        if (me->is_busy()) return notify_fail("你正忙着呢！\n");
        if (arg != "anbian" && arg != "shore")
            return notify_fail("你要往哪里跳？\n");
         tell_room(environment(me), me->name()+ "提了一口气，往岸边跃了过去。\n"NOR, ({ me }));
         me->move(__DIR__"pubu");
         message_vision("$N忽然从山洪中跃了出来，足步虚幌的落在了岸边。\n", me);
         return 1;
} 

int do_hit(string arg)
{
	object me = this_player();
        object weapon;        
	int i;
	i = me->query_skill("sword");
	if ( i > 300) i = 300;
	i += random(i);

	weapon = me->query_temp("weapon");

	if (me->is_busy() || me->is_fighting()) 
		return notify_fail("你正忙着呢！\n"); 

	if (!arg ||(arg != "flood" && arg != "hongshui"))
		return notify_fail("你想做什么？\n");

	if( !me->query("ygpass"))
		return notify_fail("你对着山洪拳打脚踢了一阵，感到劲疲力尽。\n");

	if (!weapon || weapon->query("skill_type") != "sword")
		return notify_fail("你对着山洪拳打脚踢了一阵，感到劲疲力尽。\n");

	if (me->query_skill("xuantie-jianfa",1) <= 120
	 && weapon->query("id") != "xuantie jian")
		return notify_fail("凭你现在的玄铁剑法等级，不用玄铁剑怎能领悟呢？\n");

	if (me->query_skill("xuantie-jianfa", 1) > 120 
	 && me->query_skill("xuantie-jianfa", 1) <= 160
	 && weapon->query("id") != "changjian")
		return notify_fail("你想进一步提高玄铁剑法的修为，要换一把剑了。\n");

	if (me->query_skill("xuantie-jianfa", 1) > 160 
	 && weapon->query("id") != "mu jian")
		return notify_fail("你想进一步提高玄铁剑法的修为，要进一步去领悟了。\n");

	if (me->query("neili") < 60) 
		return notify_fail("你的内力不足，无法继续练下去。\n");

	if (me->query("qi") < 60) 
		return notify_fail("你已经不行了，无法继续练下去。\n");

	if (me->query("jingli") < 60) 
		return notify_fail("你精力不足，无法继续练下去。\n");

	if( me->query_skill("force", 1) < 220)
		me->improve_skill("force",i/8+1);

	if ( weapon->query("id") == "xuantie jian")
		weapon->ob_hit();

	if (me->query_skill("xuantie-jianfa",1) <= 30){
		me->improve_skill("xuantie-jianfa",i/5+1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"发出了轻微的声响。\n",me);
		write("你似乎对于玄铁剑法的用力之道有所领悟。\n");
		me->add("neili", -5);
		me->add("jingli",-30);
		return 1;
	}
	else if ((int)me->query_skill("xuantie-jianfa",1) <= 60){
		me->improve_skill("xuantie-jianfa", i/5+1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"发出了嗤嗤的声响。\n",me);
		write("你似乎对于玄铁剑法的运用之道有所领悟。\n");
		me->add("neili", -5);
		me->add("jingli",-30);
		return 1;
	}
	else if((int)me->query_skill("xuantie-jianfa",1) <= 90){
		me->improve_skill("xuantie-jianfa", i/5 +1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name() + "发出了刷刷的声响。\n",me);
		write("你似乎对于玄铁剑法的搏击之道有所领悟。\n");
		me->add("neili", -5);
		me->add("jingli",-30);
		return 1;
	}
	else if((int)me->query_skill("xuantie-jianfa",1) <= 120){
		me->improve_skill("xuantie-jianfa",i/ 4 +1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"发出轰隆隆的巨声。\n",me);
		write("你似乎对于玄铁剑法的内劲运用有所领悟。\n");
		me->add("neili", -10);
		me->add("jingli",-35);
		return 1;
	}
	else if((int)me->query_skill("xuantie-jianfa",1) <= 150){
		me->improve_skill("xuantie-jianfa", i /4+1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"发出轰刷刷的声响。\n",me);
		write("你似乎对于玄铁剑法的精髓有所领悟。\n");
		me->add("neili", -10);
		me->add("jingli",-35);
		return 1;
	}
	else if((int)me->query_skill("xuantie-jianfa",1) <= 180){
		me->improve_skill("xuantie-jianfa", i /3+1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"发出轰嗤嗤的声响。\n",me);
		write("你似乎对于玄铁剑法的御剑之道有所领悟。\n");
		me->add("neili", -10);
		me->add("jingli",-40);
		return 1;
	}
	else if((int)me->query_skill("xuantie-jianfa",1) <= 210){
		me->improve_skill("xuantie-jianfa", i /3 +1);
		message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"寂然无声。\n",me);
		write("你似乎对于玄铁剑法的御剑之气有所领悟。\n");
		me->add("neili", -10);
		me->add("jingli",-40);
		return 1;
	}
	message_vision("$N用"+weapon->name()+"奋力击打着山洪，手中的"+weapon->name()+"寂然无声。\n",me);
	write("你已经无法再在这里加深你的玄铁剑法的修为了！\n");
	if(! me->query("xtjf/gift")){
		me->add("max_neili", i/2);
		me->add("max_jingli", i/3);
		me->add("eff_jingli", i/3);
		me->set("xtjf/gift", 1);
		tell_object(me,HIW"\n你心智一明，只觉得内力和精力有所提高。\n\n"NOR);
	}
	return 1;
}

void reset()
{
	object *ob;
	int i;
        
	ob = all_inventory(this_object());

	for(i=0; i<sizeof(ob); i++) {
		if( userp(ob[i]) ) continue;
		if(ob[i]->query("env/invisibility")) continue;
		ob[i]->reincarnate();
		if( !ob[i]->return_home() ) destruct(ob[i]);
	}
}
