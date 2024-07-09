// hudedi.c 胡德第 tdh

inherit NPC;
#include "tdh_npc.h"

void create()
{
	int x=3;
	set_name("胡德第", ({ "hu dedi", "hu", "dedi" }));
	set("gender", "男性");
	set("age", 40);
	set("shen_type", 1);
	set("天地会/tangkou","参太堂");
	set("天地会/title","香主");
	if ( random(10) >7 ) set("long","他是天地会的参太堂香主。");

        set("str", 35);
        set("int", 25);
        set("con", 25);
        set("dex", 15);
        set("max_qi", 3600);
        set("max_jing", 1800);
        set("max_jingli", 2200);
        set("neili", 6000);
        set("max_neili", 4400);
        set("jiali", 50);
        set("combat_exp", 1800000);

	set_skill("cuff", 200);
	set_skill("strike", 200);
	set_skill("dodge", 200);
	set_skill("parry", 200);
	set_skill("force", 200);
	set_skill("taizu-quan", 200);
	set_skill("weituo-zhang", 200);
	set_skill("yijin-jing", 200);
	set_skill("shaolin-shenfa", 200);

	map_skill("force", "yijin-jing");
	map_skill("cuff","taizu-quan");
	map_skill("strike","weituo-zhang");
	map_skill("dodge","shaolin-shenfa");
	map_skill("parry","weituo-zhang");
        random(x)>1 ? prepare_skill("cuff","taizu-quan"):prepare_skill("strike","weituo-zhang");

	set_temp("apply/attack", 35);

	set("inquiry", ([
		"万云龙": (: ask_join :),
		"天地会": (: ask_tdh :),
		"洪门": (: ask_tdh :),
		"反清复明": (: ask_fq :),
		"陈近南": (: ask_chen :),
		"郑成功": (: ask_zheng :),
		"国姓爷": (: ask_zheng :),
		"国姓爷": (: ask_zheng :),
		"三点革命诗": (: ask_sandian :),
		"切口": (: ask_qk :),
		"暗号": (: ask_qk :),
		"活动": (: ask_job :),
		"job": (: ask_job :),
	]));
	setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
        add_money("silver", 10 + random(20));
}
