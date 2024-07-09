#ifndef NPCDATA
#define NPCDATA "/data/npc/"
#endif
#define MENGZHU NPCDATA + "zhong-shentong"

#include <ansi.h>
inherit NPC;
inherit F_SAVE;

void do_copy(object);
void do_clone(object);

string query_save_file()
{
	return MENGZHU;
}

void create()
{
	object tmp;
	seteuid(getuid());
	set_temp("pob", previous_object(-1));
	if (!restore()) {
		set_name("王重阳", ({ "zhong shentong", "zhong", "shentong" }) );
		create_family("武林", 0, "盟主");
		set("title", "中神通" );
		set("gender", "男性" );
		set("age", 60);
		set("long","他就是在第一次华山论剑中技压群雄的重阳真人。\n");
		set("attitude", "heroism");
		set("no_quest", 1);
		set("winner","");
		set("str", 25);
		set("con", 25);
		set("int", 25);
		set("dex", 25);
		set("max_qi", 2500);
		set("eff_qi", 2500);
		set("qi", 3000);
		set("max_jing", 2500);
		set("jing", 2500);
		set("jiali", 100);
		set("combat_exp", 800000);
		set_skill("unarmed", 180);
		set_skill("force", 180);
		set_skill("dodge", 180);
		set_skill("parry", 180);
		set("weapon", BINGQI_D("changjian"));
		set("armor", ARMOR_D("armor"));
		setup();
		carry_object(ARMOR_D("armor"))->wear();
	}
	else {
		set_name(query("name"), ({ "zhong shentong", "zhong", "shentong" }));
		set("no_quest", 1);
		setup();
		if( query("weapon") ) {
			tmp = carry_object(query("weapon"));
			tmp->wield();
			tmp->set("no_get", 1);
		}
		if( query("armor") ) {
			tmp = carry_object(query("armor"));
			tmp->wear();
			tmp->set("no_get", 1);
		}
	}
}

void heart_beat()
{
	if (!living(this_object()) || is_fighting() || is_busy()) ::heart_beat();
}

void init()
{
	::init();
	add_action("do_recover", "recover");
	add_action("do_recopy", "recopy");
}

void kill_ob(object me)
{
	object ob;
	int i;

	command("say 你想谋害本座，当真是吃了熊心豹子胆了！！");
	reincarnate();
	me->set("neili", 0);
	me->set("jingli", 10);
	me->remove_killer(this_object());
	remove_killer(me);
	for (i=1; i<=2; i++)
		if (objectp(ob = present("wei shi " + i)) && living(ob))
			 ob->kill_ob(me);
		else {
			command("say 可本座根本就懒得理你！");
			break;
		}
}

int accept_fight(object ob)
{
	if (query("winner") == ob->query("id"))
		return notify_fail("你跟你自己打什么架？！\n");

	if (userp(ob) && NATURE_D->query_daytime() != "event_noon")
		return notify_fail("正午时分才能挑战盟主！\n");

	if (ob->query_temp("no_zhong"))
		return notify_fail("你现在不能抢盟主之位！\n");

	if (wizardp(ob))
		return notify_fail("巫师不能抢盟主之位！\n");

	if (ob->query_condition("killer"))
		return notify_fail("通缉犯不能抢盟主之位！\n");

	if (mapp(ob->query_conditions_by_type("poison")) || mapp(ob->query_conditions_by_type("hurt")))
		return notify_fail("你身中巨毒，无法凝神运气抢夺盟主！\n");

	if (is_fighting()) return notify_fail("已经有人正在挑战武林盟主！\n");

	if (is_busy()) return notify_fail("盟主刚刚更新，等一段时间再试吧！\n");

	restore();
	setup();
	clear_condition(0);
	start_busy(-1);
	interrupt_me(0, 0);
	reincarnate();
	command("enable all");
	reincarnate();
	remove_call_out("checking");
	call_out("checking", 1, ob);
	return 1;
}

void checking(object ob)
{
	int my_max_qi, his_max_qi;

	if (!ob) return;
	my_max_qi = query("max_qi");
	his_max_qi = ob->query("max_qi");

	if (is_fighting(ob)) {
		call_out("checking", 1, ob);
		return;
	}

	if (!present(ob)) return;

	if (query("qi")*100 / my_max_qi <= 50) {
		command("say 哈哈！果然厉害，恭喜你成为新一代的「中神通」！\n");
		command("chat "+HIY+ob->query("name")+"技高一筹，获得「中神通」的称号！本人输的心服口服！"NOR);
		add("generation", 1);
		do_copy(ob);
		do_clone(ob);
		start_busy(200);
		return;
	}
	if (ob->query("qi")*100 / his_max_qi <= 50)
		command("say 看来" + RANK_D->query_respect(ob) + "还得多加练习，方能在当今武林中出人头地！\n");
}

void do_copy(object ob)
{
	object obj;
	seteuid(getuid());

	set("name", ob->name(1));
	set("title", "第" + chinese_number(query("generation")) + "代「中神通」");
	set("short", query("title") + query("name") + "(" + capitalize(ob->query("id")) + ")");
	set("long", ob->long(1));

	//remove last winner's apply/short.
	if (objectp(obj = LOGIN_D->find_body(query("winner"))))
		obj->delete_temp("apply/short");

	ob->set_temp("apply/short", ({query("short")}));
	set("short", query("title") + query("name") + "(Zhong shentong)");
	set("no_get",1);
	delete("title");
}

int do_recopy()
{
	object ob = this_player();

	if (query("winner") != ob->query("id"))
		return notify_fail("你不是现任「中神通」！\n");

	do_copy(ob);
	write("称号恢复完毕！\n");
	return 1;
}

void do_clone(object ob)
{
	object *inv;
	mapping hp_status, skill_status, map_status, prepare_status;
	string *sname, *mname, *pname;
	int i;

	seteuid(getuid());
	set("winner", ob->query("id"));

/* delete and copy skills */

	if (mapp(skill_status = query_skills())) {
		sname = keys(skill_status);

		i = sizeof(sname);
		while (i--) delete_skill(sname[i]);
	}

	if (mapp(skill_status = ob->query_skills()) ) {
		sname = keys(skill_status);

		i = sizeof(sname);
		while (i--) set_skill(sname[i], skill_status[sname[i]]);
	}

/* delete and copy skill maps */

	if ( mapp(map_status = query_skill_map()) ) {
		mname = keys(map_status);

		i = sizeof(mname);
		while (i--) map_skill(mname[i]);
	}

	if ( mapp(map_status = ob->query_skill_map()) ) {
		mname = keys(map_status);

		i = sizeof(mname);
		while (i--) map_skill(mname[i], map_status[mname[i]]);
	}

/* delete and copy skill prepares */

	if ( mapp(prepare_status = query_skill_prepare()) ) {
		pname = keys(prepare_status);

		i = sizeof(pname);
		while (i--) prepare_skill(pname[i]);
	}

	if ( mapp(prepare_status = ob->query_skill_prepare()) ) {
		pname = keys(prepare_status);

		i = sizeof(pname);
		while (i--) prepare_skill(pname[i], prepare_status[pname[i]]);
	}

/* unwield and remove weapon & armor */

	inv = all_inventory();
	i = sizeof(inv);
	while (i--) destruct(inv[i]);
	delete("weapon");
	delete("armor");
	delete("recovered");
/* wield and wear weapon & armor */

	inv = all_inventory(ob);
	i = sizeof(inv);
	while (i--) {
		if( inv[i]->query("weapon_prop/damage") > 100
		|| inv[i]->query("unique")
		|| inv[i]->query("imbued")
		|| inv[i]->query("treasure")
		|| inv[i]->query("armor_prop/armor") > 100 ) continue;

		if( inv[i]->query("weapon_prop") && inv[i]->query("equipped") ) {
			carry_object(base_name(inv[i]))->wield();
			set("weapon", base_name(inv[i]));
		}
		if( inv[i]->query("armor_prop") && inv[i]->query("armor_type") == "cloth" && inv[i]->query("equipped") ) {
			carry_object(base_name(inv[i]))->wear();
			set("armor", base_name(inv[i]));
		}
	}

/* copy entire dbase values */

	hp_status = ob->query_entire_dbase();

	set("str", hp_status["str"]+5);
	set("int", hp_status["int"]+5);
	set("con", hp_status["con"]+5);
	set("dex", hp_status["dex"]+5);
	set("age", hp_status["age"]);
	set("per", hp_status["per"]);

	set("max_qi", hp_status["max_qi"]/4*5);
	set("max_jing", hp_status["max_jing"]);
	set("max_neili", hp_status["max_neili"]/2*3);
	set("eff_jingli", hp_status["eff_jingli"]);
	set("gender", hp_status["gender"]);
	// let the copy have 1.5 times exp.
	set("combat_exp", hp_status["combat_exp"]);
	set("death_count", hp_status["death_count"]);
	set("death_times", hp_status["death_times"]);
	set("PKS", hp_status["PKS"]);
	set("family/family_name", ob->query("family/family_name"));
	set("no_get", query("name")+"对你来说太重了。\n");
	clear_condition(0);
	revive(1);
	reincarnate();
	save();
	destruct(load_object(__FILE__));
	tell_object(ob, "状态储存完毕。\n");
}

int do_recover()
{
	object ob = this_player();
	mapping skill_status, map_status, prepare_status;
	string *sname, *mname, *pname;
	int i;

	if (is_busy() || is_fighting())
		return notify_fail("中神通正忙着呢。\n");
	if (NATURE_D->query_daytime() != "event_midnight")
		return notify_fail("午夜时分才能恢复！\n");
	restore();
	if (query("winner") != ob->query("id")) return notify_fail("你不是现任「中神通」！\n");
	if ( ob->query("family/family_name") != query("family/family_name") )
		return notify_fail("你已经改变了门派，不能恢复了。\n");
	if (ob->query("death_count") <= query("death_count"))
		return notify_fail("你活得好好的，恢复什么？\n");
	if( ob->query("combat_exp") > query("combat_exp"))
		return notify_fail("你想越恢复越少？算了吧。\n");
	
	if (strsrch(ob->query("last_death"),"就地正法了。",1) > 0
	|| strsrch(ob->query("last_death"),"正当防卫杀死了。",1) > 0 
	|| strsrch(ob->query("last_death"),"复仇杀死了。",1) > 0)
	  return notify_fail("江湖仇杀还是认命吧。\n");
	
	if( ob->query("combat_exp") / 199 < query("combat_exp") / 200)
		return notify_fail("你的经验都哪里去了？\n");
	if (  query("combat_exp") - ob->query("combat_exp") > 50000 )
	 	return notify_fail("你的经验都哪里去了？\n");
	if (ob->query("PKS") > query("PKS")+1)
		return notify_fail("杀害玩家被报复了吧？认命吧。\n");
	if (query("recovered"))
		return notify_fail("你已经恢复过一次，不能再恢复了。\n");
	ob->set("death_count", query("death_count"));
	ob->set("death_times", query("death_times"));

/* delete and copy skills */

	if ( mapp(skill_status = ob->query_skills()) ) {
		sname = keys(skill_status);

		i = sizeof(sname);
		while (i--) ob->delete_skill(sname[i]);
	}

	if ( mapp(skill_status = query_skills()) ) {
		sname = keys(skill_status);

		i = sizeof(sname);
		while (i--) ob->set_skill(sname[i], skill_status[sname[i]]);
	}

/* delete and copy skill maps */

	if ( mapp(map_status = ob->query_skill_map()) ) {
		mname = keys(map_status);

		i = sizeof(mname);
		while (i--) ob->map_skill(mname[i]);
	}

	if ( mapp(map_status = query_skill_map()) ) {
		mname = keys(map_status);

		i = sizeof(mname);
		while (i--) ob->map_skill(mname[i], map_status[mname[i]]);
	}

/* delete and copy skill prepares */

	if ( mapp(prepare_status = ob->query_skill_prepare()) ) {
		pname = keys(prepare_status);

		i = sizeof(pname);
		while (i--) ob->prepare_skill(pname[i]);
	}

	if ( mapp(prepare_status = query_skill_prepare()) ) {
		pname = keys(prepare_status);

		i = sizeof(pname);
		while (i--) ob->prepare_skill(pname[i], prepare_status[pname[i]]);
	}

/* copy combat exp values */
	// the copy's exp was 1.5 times of this player
	ob->set("combat_exp", query("combat_exp"));
	set("recovered", 1);
	clear_condition(0);
	reincarnate();
	save();
	write("你的状态恢复完毕。\n");
	return 1;
}
