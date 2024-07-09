#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", "后山");
	set("long", @LONG
这里是玉女峰后山的一个小山谷，长年人迹罕至。谷中古树参天，绿荫森
森。偶尔从远处传来几声悠扬的鸟语，伴随着阵阵花香，简直让人疑心自己到
了世外桃源。
　　你远远的看见谷中有两间小屋，不过看来已经好久没住过人了。你心里不
禁发起愁来，该怎么办呢？
LONG
	);
	set("exits", ([
		"northdown" : __DIR__"midong",
	]));

	set("outdoors", "华山" );

	setup();
}

void init()
{
	add_action("do_shout","han");
}

int do_shout(string arg)
{
	object ob,me;
	int j;
	me = this_player();

	if (!me->query("quest/dgjj/mb")) return 0;

	if ( !arg || arg!= "风老前辈" || me->query("quest/dgjj/fengset"))
	{
		write("你在瞎喊乱叫些什么?\n");
		return 1;
	}

	if (me->is_busy())
	{
		write("你正忙着呢，不能喊叫。\n");
		return 1;
	}

	if (me->is_fighting())
	{
		write("你还是专心打架吧。\n");
		return 1;
	}


	message_vision(HIY"\n$N运用丹田之气，纵声长啸道:“风老前辈。。。”\n"NOR,me);

	if ( me->query("quest/dgjj/han") > 2 ) 
	{
		me->delete("quest/dgjj/han");
		me->delete("quest/dgjj/mb");
		log_file("quest/jiujian",sprintf("%8s%-10s在后山未发生任何情况，次：%d。\n",
			me->name(1), "("+capitalize(me->query("id"))+")",me->query("quest/dgjj/mbtimes")),me);
		me->move(__DIR__"siguoya");
		tell_object(me, "你觉得索然无趣，又回到了思过崖。\n");
		tell_room(environment(me), me->name() + "从后山走了过来。\n", ({ me }));
		return 1;
	}

	if( random(6) && !me->query("blessed") )
	{
		me->add("quest/dgjj/han", 1);
		me->add_busy(1);
		tell_object(me, "四野寂静无声，唯有空谷回音随风飘散。\n");
		return 1;
	}

	log_file("quest/jiujian",sprintf("%s(%s)在后山遇到田伯光，次：%d。\n",
		me->name(1), capitalize(me->query("id")), me->query("quest/dgjj/mbtimes")));

	me->delete("quest/dgjj/han");
	me->delete("quest/dgjj/mb");

	ob=new(__DIR__"npc/tbg");
	ob->move(this_object());
	j = (int)me->query("max_pot") - 80;

	ob->set_skill("blade",j);
	ob->set_skill("parry",j);
	ob->set_skill("cuff",j);
	ob->set_skill("dodge",j);
	ob->set_skill("shandian-daofa",j);
	ob->set_skill("taizu-quan",j);
	ob->set_skill("force",j);
	ob->set_skill("huntian-qigong",j);
	ob->set_skill("shaolin-shenfa",j);

	ob->map_skill("dodge","shaolin-shenfa");
	ob->map_skill("blade","shandian-daofa");
	ob->map_skill("cuff","taizu-quan");
	ob->map_skill("parry","shandian-daofa");
	ob->map_skill("force","huntian-qigong");
	ob->prepare_skill("cuff","taizu-quan");

	ob->set("combat_exp",j/10*j*j);
	ob->set("max_qi", 99999);
	ob->set("eff_qi", 99999);
	ob->set("qi", 99999);
	ob->set("max_neili",99999);
	ob->set("neili",99999); 
	message_vision("一条黑影从山后闪出,刀光霍霍，已经将$N笼罩在狂风快刀刀光之下。\n",me);
	ob->kill_ob(me);
	remove_call_out("confirm");
	call_out("confirm", 60, me);
	return 1;
}

int confirm(object me)
{
	object ob;
	ob=present("tian boguang",this_object());
	if (!me) {
		if(ob) destruct(ob);
		return 1;
	}
	if (me->is_ghost() || !me->is_fighting()){
		if(ob) destruct(ob);
		return 1;
	}
	message_vision(CYN"一道青影飘然而落，淡淡道：“我要指点$N几招，难道收拾不了$n这个笨蛋?\n"NOR, me,ob);
	message_vision(CYN"$N先使一招‘白虹贯日’，跟着便使‘有凤来仪’，再使一招‘金雁横空’。\n"
			+"接下来使‘截剑式’……”一口气滔滔不绝的使了三十招招式。\n"NOR,me);
	message_vision(CYN"$n招架不住，飞身向山下逃去。青衣人轻叹一声消失在山后。\n"NOR,me,ob);
	destruct(ob);
	if (!me->query("quest/dgjj/fengset")){
		write("你得到风清扬的指点，顿感功力大进。\n");
		log_file("quest/jiujian",sprintf("%8s%-10s在打跑田伯光后，得到风清扬的指点，次：%d。\n",
			me->name(1), "("+capitalize(me->query("id"))+")", me->query("quest/dgjj/mbtimes")),me);
		me->add("combat_exp",15000);
		me->set("quest/dgjj/fengset",1);
	}
	me->set_temp("met_feng",1);
	return 1;
}

int valid_leave(object me, string dir)
{
	if (me->query("family/family_name") == "华山派"
	 && me->query("quest/dgjj/fengset") 
	 && dir == "northdown") {
		me->move(__DIR__"siguoya");
		me->delete_temp("mark/steps");
		return notify_fail("你循着风清扬告诉的捷径一下子就来到了思过崖。\n");
	}
	return ::valid_leave(me, dir);
}
