// fang.c 
// By River 98/10

inherit ROOM;

#include <job_mul.h>
#include <ansi.h>
void create()
{
	set("short", HIC"石室"NOR);
	set("long",@long
一间空荡荡的石室，正前方放着一个祭坛，上面呈放着一些瓜果，两边香
烛散发出摇曳的火光，一些古墓弟子正将侵犯古墓的叛逆的首级呈放在祭坛两
侧用来告慰先祖的英灵。
long);

	set("exits",([
		"north" : __DIR__"gmqs",
		"south" : __DIR__"gmcc",
		"east" : __DIR__"yaofang",
	]));

	setup();
}

void init()
{
	add_action("do_jibai","jibai");
	add_action("do_ketou","ketou"); 
}

int do_jibai(string arg)
{
	object me,ob;
	int exp,pot;
	me = this_player();       
	ob = present("head", me);

	if ( arg == "head") {
		if(!ob )
			return notify_fail("你没有人头，祭拜什么？\n");
		if(!me->query_temp("gm/job")) 
			return notify_fail("你还没去巡逻呢，哪里捡来的首级？\n");
		if(ob->query("name") != "蒙面女子的首级")
			return notify_fail("你哪里弄来的人头，这人好象不是古墓叛逆啊？\n");
		if(ob->query("victim_user")) 
			return notify_fail("你哪里弄来的首级，杀错人了吧？\n");
		if(ob->query("kill_by") != me)
			return notify_fail("这好象不是你杀的吧？你是怎么得到这个人头的？\n");

		message_vision(HIY"$N恭恭敬敬地磕了几个响头，然后慢慢将$n呈放在祭坛两侧。\n"NOR, me,ob);
		
		exp = 160 + random(50);
		//me->add("job_time/古墓护墓",1);
		//GIFT_D->check_count(me,"孙婆婆","古墓护墓");
		//me->add("combat_exp", exp);
						
		exp = me->add_exp_combat(exp,"孙婆婆","古墓护墓");
		
		pot = exp/5 + random(exp/10);
		me->add("potential",pot);
		if((int)me->query("potential", 1) > (int)me->query("max_pot", 1))
			me->set("potential", (int)me->query("max_pot", 1));
		me->delete_temp("gm/job");
		me->delete_temp("meet_nuzi");
		call_out("destroying", 1, ob);
		tell_object(me,HIW"你被奖励了："+ chinese_number(exp)+"点实战经验和"+ chinese_number(pot) + "点潜能。\n"NOR);
		return 1;
	}
	return notify_fail("你拿什么祭拜啊？\n");
}

void destroying(object ob)
{
	if (!ob) return;
	destruct(ob);
}

int do_ketou(string arg)
{
	mapping fam;
	object me=this_player();
	if (!(fam = me->query("family")) || fam["family_name"] != "古墓派")
		return notify_fail("你不是古墓传人，在此磕头作甚？\n");
	if ( arg == "jitan"){
		if ( me->query("combat_exp") < 30000)
			return notify_fail("你看着祭坛，心感武艺太差，不由惶恐不安，又慢慢站起了身。\n");
		if ( me->query("combat_exp") > 200000)
			return notify_fail("你还是去江湖上多干些事来告慰祖师吧！\n");
		if ( me->query_temp("bishou") < 2 ) {
			if(objectp(present("bi shou", me)))        
				return notify_fail("你发现盒子里已经空荡荡的，什么也没有了。\n"); 
			me->add_temp("bishou", 1);
			message_vision(HIY"$N恭恭敬敬地对着祭坛磕起头来。\n"NOR, me);
			return 1;
		}
		message_vision(YEL"祭坛缓缓转动，$N从里面的小盒子里拿出一把匕首。\n"NOR, me);
		me->delete_temp("bishou");
		new(__DIR__"obj/bishou")->move(me);
		return 1;
	}
	return notify_fail("你对着什么磕头啊？\n");
}
