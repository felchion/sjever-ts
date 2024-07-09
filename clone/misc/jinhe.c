// jinhe.c 金纸鹤
// By River@SJ 

inherit ITEM;
#include <ansi.h> 

void create()
{
	set_name(HIY"金纸鹤"NOR, ({ "jin zhihe", "jin", "zhihe" }));
	set("weight", 50);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一只栩栩如生的金色纸鹤，仔细看去，原来是用巧手精心折叠(zhedie)而成的。\n");
                set("value", 500);
		set("unit", "只");
		set("no_drop", 1);
		set("no_give", 1);
		set("no_get", 1);
		set("material", "paper");
	}
	setup();
}

void init()
{
	add_action("do_zhedie", "zhedie");
}

void he_done(object me,object ob,string msg,int i)
{
	object he;

	if(! me || !ob ) return;

	message_vision(msg, me);
	if (! wizardp(me))
		me->add_busy(random(i/5)+1);
	he = new(MISC_D("zhihe"));
        he->set("name", HIW+me->query("name")+"叠的纸鹤"NOR);
	he->set("id", me->query("id")+"'s zhihe");
	he->set("long", he->query("long") +"这是由"+me->query("name")+"("+capitalize(me->query("id"))+")亲手折叠而成的。\n");
	he->set_amount(i);
	he->move(me);
	destruct(ob);
}

int do_zhedie(string arg)
{
	object ob, me = this_player();
	string msg;
	int i =1;

	if (! arg || arg != "zhihe")
		return notify_fail("你想折叠什么？\n");

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if ( ! objectp( ob =present("bai zhi", me)))
		return notify_fail("你没有一张干净的白纸，用什么来折叠啊？\n");
	
	if ( !wizardp(me) && query("owner") != me->query("id"))
		return notify_fail("你从那里得到的金纸鹤啊？\n");

	if ( me->query_temp("make_zhihe/done"))
		return notify_fail("你已经有一千只纸鹤了。\n");

	if (me->query_temp("zhihe") < 50) {
		me->add_temp("zhihe", i);
		msg = "$N拿起一张白纸，照着金纸鹤的样子开始叠，终于叠好了一只，突然发现自己已是大汗淋漓。\n"; 
		he_done(me, ob, msg, i);
		return 1;
	}
	if (me->query_temp("zhihe") >= 50 && me->query_temp("zhihe") < 300 ) {
		i = 3 + random(5);
		me->add_temp("zhihe", i);
		msg = "$N拿着白纸开始叠鹤，渐渐开始熟练起来，不一会就叠好了"+chinese_number(i)+"只。\n"; 
		he_done(me, ob, msg, i);
		return 1;
	}
	if (me->query_temp("zhihe") >= 300 && me->query_temp("zhihe") < 550) {
		i = 5 + random(10);
		me->add_temp("zhihe", i);
		msg = "$N拿着白纸，非常熟练地折叠起来，很快就折好了"+chinese_number(i)+"只纸鹤。\n"; 
		he_done(me, ob, msg, i);
		return 1;
	}
	if (me->query_temp("zhihe") >= 550 && me->query_temp("zhihe") < 800) {
		i = 10 + random(20);
		me->add_temp("zhihe", i);
		msg = "$N拿起白纸，熟练地折叠着，心里想着某个人，叠成的鹤也渐渐显得有灵气了。\n"; 
		he_done(me, ob, msg, i);
		return 1;
	}
	if (me->query_temp("zhihe") >= 800 && me->query_temp("zhihe") < 1000) {
		i = 20 + random(30);
		if ( i + me->query_temp("zhihe") > 1000)
			i = 1000 - me->query_temp("zhihe");
		me->add_temp("zhihe", i);
		msg = "$N一边叠着白纸，一边幸福地微笑着，叠成的纸鹤也甚是灵动，仿佛马上就会飞到另一个人的身边。\n"; 
		he_done(me, ob, msg, i);
		return 1;
	}
	if (me->query_temp("zhihe") >= 1000 ) {
		msg = HIW"\n$N刚叠完这只鹤，突然觉得阳光好象也变得温柔了一些，原来身上已经叠好了一千只纸鹤。\n"NOR;
		message_vision(msg, me);
		me->delete_temp("zhihe");
		me->set_temp("make_zhihe/done", 1);
		call_out("dest", 1, this_object());
		return 1;
	}
	return 1;
}

void dest(object obj)
{
	if (!obj) return;
	destruct(obj);
}
