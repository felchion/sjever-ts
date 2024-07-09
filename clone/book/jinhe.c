// jinhe.c 金盒
// By River@SJ For qianzhu-wandushou 2002.11

inherit ITEM;
#include <ansi.h>

string do_look();
void create()
{
	set_name(HIY"金盒"NOR, ({ "jin he", "jin", "he" }));
	set_weight(2500);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "只");
		set("value", 2500);
		set("unique", 1);
		set("no_give", 1);
		set("no_get", 1);
		set("treasure",1);
		set("material", "gold");
	}
	set("xiulian", 50 );
	set("long", (: do_look :));
	setup();
}

void init()
{
	add_action("do_open", "open");
	if (environment() == this_player()) {
		add_action("do_xiulian", "xiulian");
		add_action("do_zhua", ({"zhua", "dai"}));
	}
}

string do_look()
{
	string str;
	int i;
	i = query("xiulian");

	str = "盒中两只拇指大小的蜘蛛蠕蠕而动，蜘蛛背上花纹斑斓，鲜明夺目。\n";
	if ( i <= 32 )
		str = "盒中两只拇指大小的蜘蛛蠕蠕而动，蜘蛛背上花纹已不明显，乌黑发亮。\n";
	if ( i <= 15 )
		str = "盒中两只拇指大小的蜘蛛蠕蠕而动，蜘蛛背上已呈现灰白之色。\n";
	if ( i < 1 )
		str = "盒中蜘蛛已然死去，里面空空如也。\n";
	return str;
}

int do_open(string arg)
{
	object me = this_player();

	if (!id(arg)) 
		return notify_fail("你要打开什么啊？\n");

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if ( query("xiulian") < 1)
		message_vision("$N打开盒盖，只见里面空空如也，又顺手合上了。\n", me);
	else {
		message_vision(HIY"$N打开盒盖，只见盒中两只拇指大小的蜘蛛蠕蠕而动。\n"NOR, me);
		set("open", 1);
	}
	return 1;
}

int do_xiulian()
{
	object me= this_player();

	if (!query("open"))
		return notify_fail("你还没打开盒盖，修炼什么啊？\n");

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if ( me->query("gender") != "女性")
		return notify_fail("你一个大老爷们，练这个干吗啊？\n");

	if ( !me->query("quest/qianzhu/pass"))
		return notify_fail("你是怎么得到金盒的啊？");

	if ( me->query_temp("weapon"))
		return notify_fail("你手持武器，怎么修炼千蛛万毒手啊？\n");

	if ( me->query("neili") < 3000 )
		return notify_fail("你的真气不够，恐难抵挡蜘蛛的毒性。\n");

	if ( me->query_per() < 2 )
		return notify_fail("你已经几乎不能辨认出是人的模样了，还是先适当提高些容貌再来修炼吧。\n");

	if ( me->query_temp("xiulian_qzs"))
		return notify_fail("你刚刚修炼千蛛万毒手完毕，还是先歇息一会吧。\n");

	if (environment(me)->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (environment(me)->query("pending"))
		return notify_fail("此地不宜修练！\n");

	if (environment(me)->query("sleep_room"))
		return notify_fail("卧室不宜修练，会影响别人休息。\n");
 
	if (environment(me)->query("no_fight"))
		return notify_fail("此地不宜修练！\n");

	me->start_busy(15);

	tell_object(me, HIR"\n你盘膝坐下，行了一会儿内功，将双手两根食指伸进盒中，只见盒中的一对花\n"+
			     "蛛慢慢爬近，分别咬住了你两根指头。你深深吸一口气，双臂轻微颤抖，潜运\n"+
			     "内功和蛛毒相抗，花蛛吸取你手指上的血液为食，随着手指上血脉运转，也带\n"+
			     "了花蛛体内毒液，回入自己血中。\n"NOR);

	tell_room(environment(me), HIR + me->name() + "盘膝坐下，深深吸一口气，双臂轻微颤抖，潜运内功和蛛毒相抗。\n"NOR, ({ me }));

	call_out("pass_xiulian", 10 + random(5), me);
	return 1;
}

void pass_xiulian(object me)
{
	if (me) {
		tell_object(me, HIB"\n你满脸庄严肃穆之容，同时眉心和两旁太阳穴上淡淡的罩上了一层黑气，咬紧\n"+
				"牙关，竭力忍受痛楚。\n"NOR);
		call_out("finish_xiulian", 10 + random(5), me);
	}
	return;
}

int finish_xiulian(object me)
{
	int improve, i;
	string str;

	if ( !me) return 0;

	improve = me->query_int(1) * 5;
	improve += random(improve);
	i = query("xiulian");

	i --;
	if ( i < 1) {
		str = "体中的毒液都到了你手指之中已去净毒性而死，你顺手合上了盒盖。\n";
		delete("open");
	}
	else
		str = "直到吸饱了血，肚子胀得和圆球相似，这才跌在盒中，沉沉睡去。\n";

	set("xiulian", i);
	tell_object(me, YEL"\n再过一会，你鼻尖上渗出细细的一粒粒汗珠，这功夫练了几有半个时辰，双蛛\n" + str +NOR);

	tell_room(environment(me), HIB + me->name() + "又运功良久，脸上黑气渐退，重现血色，一口气喷了出来。\n"NOR, ({ me }));

	me->improve_skill("qianzhu-wandushou", improve);
	me->add("shen", - improve);
	me->add("neili", -( 150 + me->query_skill("qianzhu-wandushou", 1)));
	me->set_temp("xiulian_qzs", 1);
	call_out("delete_busy", 5 , me);
	me->start_busy(-1);
	return 1;
}

void delete_busy(object me)
{
	if ( me ) me->delete_temp("xiulian_qzs");
}

int do_zhua(string arg)
{
	object me = this_player();
	object where = environment(me);
	int skill, busy = 5 + random(5);

	if ( !me->query("quest/qianzhu/pass")) return 0;

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if ( ! arg || arg != "zhizhu")
		return notify_fail("你要抓什么啊？\n");

	if ( query("xiulian") > 0 )
		return notify_fail("金盒中的蜘蛛还没死呢，先用着吧。\n");

	if ( where->query("short") != "星宿海")
		return notify_fail("看来这里附近没有合适的蜘蛛可以抓啊。\n");

	if ( ! present("fire", me))
		return notify_fail("你没有火折，如何引火？\n");

	if ( ! present("liu huang", me))
		return notify_fail("你没有引火之物硫磺，如何引火？\n");

	skill = me->query_skill("qianzhu-wandushou", 1);
	skill /= 15;

	me->start_busy(busy+1);
	message_vision(HIY"\n$N从硫磺上捏了一小块，放在地上，用火折点燃，烧了起来。\n"NOR, me);
	call_out("zhua_finish", busy*2-1, me, skill);
	return 1;
}

void zhua_finish(object me, int i)
{
	if ( me ) {
		if ( !random(3)) {
			set("xiulian", i + 40);
			message_vision(HIC"\n忽听得草丛中瑟瑟声响，绿草中有物晃动，$N的眼前一亮，原来是两只花纹斑\n"+
					"斓，鲜明夺目的蜘蛛，$N心下大喜，急忙将蜘蛛抓入盒内。\n"NOR, me);
		}
		else 
			message_vision(HIC"\n但过了良久，草丛中没有任何异状，$N失望极了。\n"NOR, me);
	}
	return;
}
