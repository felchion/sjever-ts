//by cool 98.6.1
// Modify By River@SJ 8/30/2001
// Validate By YUJ@SJ 2002-01-17

#include <ansi.h>
inherit NPC;

#include "skill.h";

string *color_title = ({"天杀门", "地煞门"});

void create()
{
	mapping name;
	int i;

	i = random(4);

	name = RNAME_D->get_random_name(i);
	set_name(name["name"], name["id"] + ({ "killer" }));
	set("title", color_title[random(sizeof(color_title))]+(i?"":"女")+"杀手");
	set("gender", i?"男性":"女性");
	set("long", "这人身穿黑色紧装，面上蒙着一块黑布，只露出一双眼睛，精光四射。\n");
	set("age", random(20)+25);
	set("str",18+random(5));
	set("int",18+random(5));
	set("con",18+random(5));
	set("dex",18+random(5));
	set("combat_exp", 100000);
	set("no_quest", 1);
	set("shen_type", -1);
	set("attitude", "friendly");
	set("max_qi",1200);
	set("max_jing",1200);
	set("neili",1000);
	set("max_neili",1000);
	set("eff_jingli", 1200);
	set("score", 100);


	set_skill("force", 100);
	set_skill("dodge", 100);
	set_skill("parry", 100);
	set_skill("literate", query("int")* 10);

	setup();


	carry_object("/d/dali/obj/blackcloth")->wear();
}

#include "stealer1.h"
