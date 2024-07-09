// by 无名英雄

#include <ansi.h>
#include <dbase.h>
inherit ITEM;

void create()
{
	string name="", id="";

	name = YEL"太极谱"NOR;
	id = "taiji";

	set_name(HIW"『"+name+HIW"』乐谱"NOR, ({ id+"'s yuepu", id, "yue pu", "yuepu" }));
	set_weight(500);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "本");
		set("value", 2500);
		set("material", "paper");
		set("check", 1);
		set("music_name", name);
		set("music_id", id);
		set("music_add", 5);
		set("long", HIY"一本乐谱，记载了『"+name+HIY"』的弹奏方法，能提高些许熟练度。\n"+
			"你可以研读（Read）它来增加乐曲的熟练度。\n"NOR);
	}
}

void init()
{
	int i=0;
	string str="";

	if( environment() == this_player() ) {
		add_action("do_du", "read");
		if( query("check") > 0 ) {
			switch( random(4) ) {
				case  0: { i = random(3) + 2;str = HIW"些许"NOR;break; }
				case  1: { i = random(3) + 4;str = HIG"一些"NOR;break; }
				case  2: { i = random(3) + 6;str = HIR"许多"NOR;break; }
				case  3: { i = random(3) + 8;str = RED"满额"NOR;break; }
				default: { i = random(4) + 4;str = HIG"一些"NOR;       }
			}
			delete("check");
			set("music_add", i);
			set("long", HIY"一本乐谱，记载了『"+query("music_name")+HIY"』的弹奏方法，能提高"+str+HIY"熟练度。\n"+
				"你可以研读（Read）它来增加乐曲的熟练度。\n"NOR);
		}
	}
}

int do_du(string arg)
{
	object me,ob;
	int i, j=0;

	me = this_player();
	ob = this_object(); 

	i = me->query_skill("string", 1);
	if( me->query_skill("flute", 1) > i ) i = me->query_skill("flute", 1);

	if( !(arg == ob->query("id") || arg == "yuepu") ) return 0;

	if( !me->query_skill("literate", 1) )
		return notify_fail("你是个文盲，先学点文化(literate)吧。\n");	   

	if( me->is_busy() )
		return notify_fail("你现在正忙着呢。\n");

	if( me->is_fighting() ) 
		return notify_fail("你无法在战斗中专心下来研读新知！\n");

	if( !id(arg) )
		return notify_fail("你要读什么？\n");

	if( me->query_skill("literate", 1) < 100 )
		return notify_fail("你的读书写字等级不够，无法理解乐谱中记载的内容。\n"); 

	if( i < ( j = call_other("/d/music/book/" + ob->query("music_id") + ".c", "query_level") ) )
		return notify_fail("以你的乐理修为，去研读这样的乐谱，实在是勉为其难了。\n");

	if( i < ob->query("music_add")*10 )
		return notify_fail("你研读了一会儿，但是发现上面所说的对你而言都太深了，没有学到任何东西。\n");

// 加熟练度
	me->add("music/"+ob->query("music_id"), ob->query("music_add")); 
// 统计时间
	me->set("music_time/"+ob->query("music_id"), time() );

	message("vision", me->name() + "拿出一本" + ob->name() + "认真的研读着。\n", environment(me), me);
	write(HIC"你研读着" + ob->name() + HIC"，觉得自己对这一乐章的领悟熟练了一层。\n"YEL+ob->name()+YEL
		"化为长空中的几缕渺音消逝了。\n");

	if( wizardp(me) ) tell_object(me, "乐谱"+ob->query("music_id")+"增加："+ob->query("music_add")+"\n");
   	destruct(ob);
	return 1;
}
