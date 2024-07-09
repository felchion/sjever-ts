// tdhjob 第一步流程 npc by Lklv

inherit NPC;
inherit F_UNIQUE;
inherit F_SAVE;

#include <ansi.h>
#include <get_place.h>
#include "skill.h"
#include "place.h"
#include "man1.h"

string *first_title = ({
	"莲花堂","洪顺堂","家后堂","参太堂","宏化堂",
	"青木堂","赤火堂","西金堂","玄水堂","黄土堂"
});
string *second_title = ({
	"会众","眼线","卧底","探马","弟子","护卫","护法"
});

void create()
{
	string title, title2;
	mapping name;

	name = RNAME_D->get_random_name(1);
	title = first_title[random(sizeof(first_title))];
	title2 = second_title[random(sizeof(second_title))];

	set_name(name["name"], name["id"]);
	set("gender", "男性");
	set("shen", DE_LVL*10);
	set("age", 20 + random(40));
	set("meitude", "peaceful");
	set("天地会/tangkou",title);
	set("天地会/title",title2);
	set("tdh_job_target1", 1);
	set("no_get", 1);
	set("no_quest", 1);
	set("no_bark", 1);

	set("max_qi", DE_LVL*10-DE_GIFT*5);
	set("max_jing", DE_LVL*10-DE_GIFT*10);
	set("eff_jingli", DE_LVL*10);
	set("combat_exp", 500000);

	set("str", DE_GIFT+random(DE_GIFT/4));
	set("dex", DE_GIFT+random(DE_GIFT/4));
	set("int", DE_GIFT+random(DE_GIFT/4));
	set("con", DE_GIFT+random(DE_GIFT/4));

	set("no_ansuan", 1);
	set("no_bark", 1);
	set("no_quest", 1);

	set_skill("cuff", DE_LVL);
	set_skill("dodge", DE_LVL);
	set_skill("force", DE_LVL);
	set_skill("parry", DE_LVL);
	set_skill("xinyuegong", DE_LVL);
	set_skill("taizuquan", DE_LVL);
	set_skill("taiji-shengong", DE_LVL);
	set_skill("literate", query("int") * 10);
	map_skill("force", "taiji-shengong");
	map_skill("dodge", "tiyunzong");
	map_skill("cuff", "taizuquan");
	map_skill("parry", "taizuquan");
	prepare_skill("cuff", "taizuquan");

	set("inquiry", ([
		"天地会": (: ask_tdh :),
		"洪门": (: ask_tdh :),
		"反清复明": (: ask_fq :),
		"陈近南": (: ask_chen :),
		"郑成功": (: ask_zheng :),
		"国姓爷": (: ask_zheng :),
		"明复清反母地父天": (: ask_qk :),
		"切口": (: ask_qk :),
		"暗号": (: ask_qk :),
		"联络": (: ask_target :),
		"丐帮": "据说天地会会里面也有很多丐帮的兄弟。\n",
		"凝血神爪": "据说天地会总舵主陈近南练有这么一门功夫，很是厉害。\n",
	]));
	setup();

	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(ARMOR_D("shoes"))->wear();
	add_money("silver", 20 + random(20));
	if (clonep())
		call_out("dest", 1200);
}

void kill_ob(object ob)
{
	object me = this_player();

	if ( !me || !ob ) return;

	if (userp(me) && query("have_killer") != me->name()){
		command("say 好你个鞑子的走狗！今天我来为民除害！");
		//command("chat "+me->name()+"奸诈险恶，竟然投靠清廷做了鞑子的走狗，大家小心...");
		set("have_killer", me->name());
		set("job_npc",1);///added by Ciwei
	}
	
	::kill_ob(ob);
}

void die()
{
	object ob, jober, killer;
	ob = this_object();

	if ( objectp(killer = query_temp("last_damage_from")) ){
		killer->add("shen", -DE_LVL* 20);
		killer->set_temp("kill_tdhnpc", ob->name());
	}
	if ( query("天地会jober")
	&& objectp( jober = find_player(query("天地会jober")) )
	&& jober->query_temp("tdhjob")
	&& jober->query_temp("天地会job/target_1id") == query("id")){
		log_file("job/tiandi", sprintf("%8s%-10s第%4d次任务失败(3)，时间：%4d，现经验：%d。",
			jober->name(), "("+jober->query("id")+")", jober->query("job_time/天地会")+1, uptime()-jober->query_temp("天地会job/time"), jober->query("combat_exp")), jober);
		jober->delete_temp("天地会job");
		jober->set_temp("天地会job/lost", query("name"));
		message_vision(HIY"\n糟了！"+ob->name()+"死亡，任务失败！\n"NOR, jober);
	}

	command("say 做了鬼我也还是要反清复明！");
	::die();
}
