// lagoon.c 礁石
// By Looklove for a Quest
//Last change at 2k/5/27 by Looklove

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short",HIC"礁石"NOR);
	set("long", @LONG
这是岸边向海里突出的一块大礁石。上面大且平整，估计最少可以坐的下
二三十人。淡蓝的海水一次次冲过来，发出和谐的拍打声。几只不知名的海鸟
或盘旋在上空，或扑打着翅膀落在附近的礁石上。一丝淡淡的云彩轻描淡写的
抹在远处的天空中，仿佛一幅水墨画。偶尔有几只嘴里叼着小鱼的水鸟从海水
里钻出来，落在你身边的礁石上。
LONG
	);
	set("outdoors","福州");
	set("exits",([
		"westdown" : __DIR__"island5",
	]));
	setup();
}

void init()
{
	add_action("do_fish","fish");
	add_action("do_fish","diaoyu");
	add_action("do_nosleep","sleep");
	add_action("do_shou","shouxian");
	add_action("do_nosay","say");
}

int do_fish()
{
	object me = this_player();

	if (me->is_busy()) return notify_fail("你现在正忙着呢！\n");
	if (present("yu",  environment(me)))
		return notify_fail("地上已经那么多鱼了，不要暴敛天物啊。\n");
	if (!present("yu gan", me))
		return notify_fail("临渊羡鱼，不如退而结网。没有鱼竿，你想怎么个钓法？\n");
	if (me->query("jingli") <200 )
		return notify_fail("你今天太累了，还是改天再钓吧。\n");
	if (me->query_temp("gb/fish"))
		return notify_fail("先收线吧，有鱼咬着你的钩呢。\n");
	if (present("hai gui", me) && (random(me->query("jingli"))<500)) {
		message_vision(HIG"$N正拿起鱼竿想要放线出去，一只大海龟慢腾腾爬过来，"+
			"在$N的脚上咬了一口。$N疼地跳了起来。\n"NOR, me);
		me->receive_wound("qi", random(100), "被海龟咬");
		me->start_busy(5);
		return 1;
	}

	message_vision("$N将鱼饵穿在钩上，一抖手中的鱼竿，将线甩了出去。\n", me);
	me->receive_damage("jingli", 20);
	me->start_busy(2);

	if (!random(4)) {
		write(HIC"你突然觉得手中鱼竿一颤，有鱼上钩了！快收线(shouxian)！！\n"NOR);
		me->set_temp("gb/fish",1);
	} else write("只见鱼漂一动，一不小心鱼饵被吃掉了。你将空钩拽了上来。\n");
	return 1;
}

int do_shou()
{
	object me = this_player();
	object ob,ob1,ob2;

	ob=present("yu gan",me);
	ob2=present("yu",me);

	if (!me->query_temp("gb/fish")) return notify_fail("不甩竿就想收线？有个性！\n");
	if (me->is_busy()) return notify_fail("别急，慢慢来。\n");
	if (me->query("jingli") <200 ) return notify_fail("你今天太累了，还是改天再钓吧。\n");
	me->delete_temp("gb/fish");
	if (me->query("mud_age") / 4 >= me->query("combat_exp"))
		return notify_fail("你还是多花点时间练功吧，免得武功荒废了。\n");
	if (!ob) return notify_fail("你的鱼竿呢？\n");
	if (random(me->query("str"))<5 && !random(6)) {
		message_vision(HIY"突然$N的竿猛烈抖动起来，看来是条大鱼。\n"
			"$N用力拽着舍不得松手，结果被连人带竿扯进了海里。\n"NOR,me);
		destruct(ob);
		if (random(me->query("kar"))>2 && random(10)>1) {
			message_vision(HIG"$N一顿“狗刨”，好不容易爬上岸来，惊魂未定大口大口喘着粗气。\n"NOR, me);
			me->set("water", me->max_water_capacity()+200);
		} else me->unconcious();
		return 1;
	}
	me->receive_damage("jingli", 100+random(50));
	me->start_busy(1);
	if (ob2 && (random(me->query_str()) <20)) {
		write(HIC"趁你提鱼竿的工夫，一条鱼从你手边又蹦进了水里。\n"NOR);
		destruct(ob2);
		return 1;
	}
	if (random(me->query("combat_exp")) > 200000 && random(me->query_int())> 4)
		if(me->add("potential",10+random(10)) > me->query("max_pot")*2)
			me->set("potential", me->query("max_pot")*2);
	if (random(me->query_str()) > 20 && random(me->query("kar")) < 5)
	{
		message_vision("$N用力过猛，鱼竿啪的一声断了。\n", me);
		destruct(ob);
		new("/d/gb/obj/yugan2")->move(me);
		return 1;
	}
	if (random(me->query("kar")) < 5 && !random(8)) {
		message_vision(HIG"$N慢慢把线拽了上来，发现钩上面挂着一只破草鞋。\n"
			"$N气得破口大骂起来：“哪个王八蛋乱扔草鞋！”。\n"NOR, me);
		return 1;
	}
	message_vision(HIG"$N熟练地将一条鱼慢慢拽了上来。\n"NOR, me);
	ob1=new("/d/gb/obj/fish"+(1+random(7)));
	ob1->move(this_object());
	return 1;
}

int do_nosleep()
{
	object me = this_player();
	if (me->is_busy())
	return notify_fail("你现在正忙着呢！\n");
	write("这里海风很大，小心着凉，还的到别的地方睡吧。\n",me);
	return 1;
}
int do_nosay()
{
	write("嘘……别说话,把鱼都吓跑了。\n");
	return 1;
}
