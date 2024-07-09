// shi.c 史青山
// Modified by snowman 21/05/2000
// Modified by Looklove 2002.1.24
// changed ask 朝廷官位 to  other file, and changed temp short to temp title

inherit NPC;
#include <ansi.h>

void create()
{
	object ob;
	set_name("史青山", ({ "shi qingshan", "shi", "qingshan" }));
	set("title", "正五品守备");
	set("gender", "男性");
	set("age", 33);
	set("str", 25);
	set("dex", 16);
	set("long", "他曾是武当俗家弟子，又跟丐帮有点关联，现在却吃上了朝廷饭。\n");
	set("combat_exp", 200000);
	set("shen_type", 1);
	set("attitude", "heroism");

	set_skill("unarmed", 100);
	set_skill("tiyunzong",100);
	set_skill("yinyun-ziqi",100);
	set_skill("force", 100);
	set_skill("sword", 100);
	set_skill("dodge", 100);
	set_skill("parry", 100);
	set_skill("stick", 100);
	set_skill("dagou-bang", 100);
	map_skill("parry", "dagou-bang");
	map_skill("stick", "dagou-bang");
	map_skill("dodge","tiyunzong");

	set_temp("apply/attack", 70);
	set_temp("apply/defense", 70);
	set_temp("apply/armor", 70);
	set_temp("apply/damage", 70);

	set("inquiry", ([
		"为官" : "你要是有意思投靠朝廷的话，就去找赵知府吧。\n",
		"报效朝廷" : "你要是有意思投靠朝廷的话，就去找赵知府吧。\n",
	]));

	set("max_qi", 1700);
	set("qi", 1700);
	set("neili", 2000);
	set("max_neili", 2000);
	set("eff_jingli", 2000);
	set("jiali", 30);

	setup();

	if (clonep()) {
		ob = unew(BINGQI_D("langya-bang"));
		if(!ob) ob= new(BINGQI_D("stick"));
		ob->move(this_object());
		ob->wield();
		carry_object(ARMOR_D("armor"))->wear();
		carry_object(ARMOR_D("guanxue"))->wear();
        }
}

int accept_fight(object me)
{
       command("say 老夫久未和江湖人动手过招了，今日也不想破例。");
       return 0;
}

void init()
{
	object ob;
	::init();
	if (interactive(ob = this_player()) && living(this_object()) &&
		(int)ob->query_condition("killer")) {
		remove_call_out("kill_ob");
		call_out("kill_ob", 0, ob);
	}
}

void die()
{
	object killer;
	if(objectp(killer = query_temp("last_damage_from")) ){
		killer->add_condition("killer", 30);
		command("say "+killer->name()+"你竟敢谋杀朝廷命官！来人啊。。。");
	}
        ::die();
}
