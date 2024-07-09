#include <ansi.h>
inherit NPC;

string ask_xianhai();

void create()
{
	set_name("巫婆", ({ "wu po", "po", "wu" }) );
	set("gender", "女性" );
	set("shen_type", -1);
	set("age", 80);
	set("attitude", "peaceful");
	set("unique", 1);
	set("combat_exp", 25000);

	set("long",
                "这是个满脸皱纹，法力高强的巫婆，据说已经有上百岁年纪。\n"+
		"她最拿手的是可以给你做「布人」，让你去诅咒你的敌人，\n"+
		"据说给的钱越多，越灵验。\n" );

        set("chat_chance", 3);
        set("chat_msg", ({
                "巫婆嘿嘿一笑: 大仙我法力高强，有什么事尽管说吧。\n",
        }));

        set("inquiry", ([
		"诅咒"   : (: ask_xianhai :),
        ]) );

        setup();
        carry_object(ARMOR_D("cloth"))->wear();
}

string ask_xianhai()
{
	object me;
	me = this_player();
	if ( me->query("shen") > -10000 ){
		command("look "+me->query("id"));
		me->set_temp("about_shen", 1);
		return "我看这位" + RANK_D->query_respect(me) + "心性淳良，为何也来想这害人的法子。\n";
	}
}

void kill_ob(object ob)
{
	ob->remove_killer(this_object());
	remove_killer(ob);
	message_vision("巫婆说道：想杀我？你不要命了？让你看看我的厉害!\n", ob);
	message_vision("只听见巫婆喃喃地念了几句咒语。。。$N的身子一抖，再没有动作了！\n", ob);
	command("hehe");
	ob->add_busy(10);
}

void unconcious()
{
	reincarnate();
	set("eff_qi", query("max_qi"));
	set("qi", query("max_qi"));
	set("eff_jing", query("max_jing"));
	set("jing", query("max_jing"));
	set("jingli", query("eff_jingli"));
	say("巫婆狞笑道：我死不了的,再打要你命。\n");
	command("hehe");
}

void die()
{
	unconcious();
}

int accept_object(object me, object ob)
{
	int nmoney;
	nmoney=ob->value();
	if(!me->query_temp("about_shen"))
		return notify_fail("巫婆说道：这位" + RANK_D->query_respect(me) + "，咱们初次见面，为何送此厚礼？\n");
	if( ob->query("money_id") && nmoney >= 10000 ) {
		me->add("shen",-1 * random(nmoney/2));   // 1 gold random(5k) -shen 
		message_vision("一阵奸笑声中，只见巫婆从背后拿出一个小布人交给$N，那布人的模样酷似$N的仇家。\n$N用一只银针狠狠的扎着布人，口中叫到：我扎，我扎死你。\n", me);
		message_vision("$N自心底涌起一股邪念。\n", me);
		me->delete_temp("about_shen");
		me->add_busy(random(2));
		return 1;
	}
	else {
		message_vision("巫婆怒道：就这么点钱，你打发叫花子么？。\n", me);
		return 0;
	}
}
