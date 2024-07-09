// bing.c 官兵
#include <ansi.h>
inherit NPC;
void create()
{
        set_name("边防官兵", ({ "guan bing", "bing" }));
        set("age", 32);
        set("gender", "男性");
        set("long", "他是个长年驻守这里的边防兵。\n");
        set("attitude", "peaceful");
        set("str", 33);
        set("int", 16);
        set("con", 25);
        set("dex", 20);
        set("combat_exp", 30000);
        set_skill("unarmed", 40);
        set_skill("dodge", 40);
        set_skill("parry", 40);
        set_skill("blade", 40);
        set_skill("force", 40);
        set_temp("apply/attack", 40);
        set_temp("apply/defense", 40);
        set_temp("apply/damage", 20);
        set_temp("apply/armor", 40);        
        setup();
        carry_object(BINGQI_D("blade"))->wield();
        carry_object(ARMOR_D("junfu"))->wear();
}

int accept_fight(object me)
{
	command("say 大爷我守边关累得要死，没空和"+RANK_D->query_respect(me)+"过招玩。\n");
	return 0;
}

void init()
{
	object ob, me, money;
	::init();

	me=this_object();

	if(interactive(ob = this_player()) && living(this_object()))
	if((int)ob->query_condition("killer") || ob->query_temp("xx_rob")) {
		remove_call_out("kill_ob");
		call_out("kill_ob", 0, ob);
	}
}
