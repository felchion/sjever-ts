// NPC: /d/huashan/npc/puren.c
// Date: Look 99/03/25
// Modify By Looklove@SJ 2000/1/7 for add job
// Lklv Modify at 2001.10.18

#include <ansi.h>
inherit NPC;
string ask_over();

void create()
{
	set_name("仆人", ({ "pu ren", "puren",}));
	set("long","一个烧饭扫地的仆人\n");
	set("gender", "男性");
	set("age", 26);
	set("attitude", "peaceful");
	set("shen_type", 1);
	set("str", 23);
	set("int", 23);
	set("con", 24);
	set("dex", 28);

	set("max_qi", 300);
	set("max_jing", 180);
	set("neili", 300);
	set("max_neili", 300);
	set("eff_jingli", 180);

	set("jiali", 5);
	set("combat_exp", 5000);
	set("score", 50);

	set_skill("unarmed", 20);
	set("inquiry", ([
	      "柴禾" : (: ask_over :),
	      "砍材" : (: ask_over :),
	      "caihe" : (: ask_over :),
	]));
	setup();
	carry_object(ARMOR_D("armor/cloth"))->wear();
}

string ask_over()
{
	int exp,pot;
	object me;
	me = this_player();

	if ((int)me->query("combat_exp") >= 100000)
		return "虽然厨房缺柴禾，可是这些事还是让我们下人来做吧。\n";
	if (me->query("shen") < 1000)
		return "你眼露凶光, 最近是不是做了什么坏事？ ";
	if (!me->query_temp("hs/kc_job")||!me->query_temp("kc_job/ok"))
		return "你六师兄刚才来说，要帮我找几个弟子去砍柴，你去不去？去就告诉他一声。\n";

	me->delete_temp("hs/kc_job");
	me->delete_temp("kc_job");

	exp = me->query("max_pot")/2+random(20);
	
	exp = me->add_exp_combat(exp,this_object(),"华山砍柴");
	
	pot = exp/5 + random(exp/10);

	//me->add("combat_exp",exp);
	me->add("shen",exp*3/4);
	me->add("potential",pot);
	if(me->query("potential") > me->query("max_pot"))
		me->set("potential", me->query("max_pot"));
		me->improve_skill("zixia-gong",me->query("int")*2);
		
	me->add("job_time/华山砍柴",1);

	message_vision("$N大声对仆人说道：“你要的柴禾我都砍了，堆在朝阳峰，需要的话你找人去取。”\n",me);
	tell_object(me,HIM" 你得到了：\n" +
	chinese_number(exp) + "点实战经验\n"+
	chinese_number(pot) + "点潜能。\n"NOR);
	command("ok "+me->query("id"));
	return "先堆着吧，等我要用的时候我就去取。\n";
}
