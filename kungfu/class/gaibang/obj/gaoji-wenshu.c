#include <ansi.h>
#include <job_mul.h>

inherit ITEM;

void create()
{
	set("unit","封");
	set("no_get", 1);
	set("no_drop", 1);
	set("no_put", 1);
	set("no_give", 1);
	set("material", "paper");
	set_weight(1);
	setup();
}

void init()
{
	object me = this_player();

	set_name ("告急文书", ({ "wenshu" }));
	set("long", "这是一封告急文书(gaoji XXX)。\n");
	if (me && present(this_object(), me)) {
		add_action("do_gaoji", "gaoji");
	}
}

int do_gaoji(string str)
{
	object me = this_player(), ob = this_object();
	object ob1;
	string fam;
	int exp, pot, shen;

	if (!str || str=="")
		return notify_fail ("你要向谁告急？\n");

	if(!objectp(ob1 = present(str, environment(me))) )
		return notify_fail("这里没 " + str + " 这个人。\n");

	if( !living(ob1) )
		return notify_fail("你等" + ob1->name() + "醒来再说吧。\n");

	if( ob1->query("id") != "hou junji" )
		return notify_fail("边关军情可不能随便对什么人都乱说。\n");

	if( file_name (environment(this_player())) != "/d/xingxiu/jyg" )
		return notify_fail("侯君集这次是出来游玩的，不想考虑其它事情。\n");

	fam = (string)me->query("family/family_name");
	message_vision("$N交给$n一张告急文书。\n",me,ob1);
	message_vision(CYN"$n对$N说道：多谢"
		+ RANK_D->query_respect(me)
		+ "前来报讯，我定叫西夏大军有来无回。\n"NOR, me, ob1 );
	me->delete_temp("gb_job1");
	exp = 50 + random( 50 );
		
	exp = me->add_exp_combat(exp,ob1,"告急");
	
	pot = exp/5+random(exp/10);
	shen = exp/2 + pot/2;
	me->add( "potential", pot );
	me->add( "shen", shen );
	write( sprintf(HIW "好，任务完成了，你得到了"
    		+ CHINESE_D->chinese_number( exp ) + "点实战经验，"
	        + CHINESE_D->chinese_number( pot ) + "点潜能和"
	        + CHINESE_D->chinese_number( shen ) + "点正神。\n" NOR));

	log_file( "job/junqing", sprintf("%8s%-10s万里报军情任务，得到%3d点经验，%2d点潜能。\n",
		me->query("name"), "("+me->query("id")+")", exp,pot ),me);	
	if (fam == "丐帮") me->delete("job_name");
	if( me->query("potential") > me->query("max_pot") )
		me->set("potential", me->query("max_pot") );
	
	me->apply_condition("job_busy", 5);
	destruct( ob );
	return 1;
}
