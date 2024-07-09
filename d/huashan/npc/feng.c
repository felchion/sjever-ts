// NPC: /d/huashan/npc/feng.c
// Date: Look 99/03/25
// Modify by Lklv 2001.10.18

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

void create()
{
	set_name("风清扬", ( {"feng qingyang","feng","qingyang"} ) );
	set("long", "他看起来神色抑郁，面如金纸。\n" );
	create_family("华山派", 12, "弟子");

	set("age", 91);
	set("gender", "男性");
	set("attitude", "friendly");
	set("shen_type", 1);
	set("shen",150000);
	set("no_quest",1);

  set("con", 30);
  set("dex", 30);
  set("int", 40);
  set("str", 30);
	set("per", 35);
  set("max_jing", 6000);
  set("max_neili", 15000);
  set("max_qi", 7500);
  set("eff_jingli", 4000);
  set("neili", 15000);
  set("jiali", 100);
  set("combat_exp", 15000000);

  set_skill("sword",500);
  set_skill("force",500);
  set_skill("dodge",500);
	set_skill("poyu-quan", 520);
	set_skill("cuff", 500);
  set_skill("parry",500);
  set_skill("blade", 500);
  set_skill("fanliangyi-dao", 500);
  set_skill("dugu-jiujian",530);
  set_skill("huashan-shenfa",520);
  set_skill("huashan-jianfa",520);
  set_skill("zixia-gong",530);
  set_skill("zhengqi-jue",280);
  set_skill("literate",540);

  map_skill("force","zixia-gong");
  map_skill("sword","dugu-jiujian");
  map_skill("dodge","huashan-shenfa");
	map_skill("parry","dugu-jiujian");
	map_skill("cuff", "poyu-quan");
	prepare_skill("cuff", "poyu-quan");

	set_temp("apply/damage", 40);
  set_temp("apply/dodge", 40);
  set_temp("apply/attack", 40);
  set_temp("apply/armor", 40);

	setup();

        carry_object(BINGQI_D("sword"))->wield();
        carry_object(ARMOR_D("cloth"))->wear();
}

void attempt_apprentice(object ob)
{
        if (!ob->query("quest/dgjj/fengset")) {
						command("say 你我恐怕今生无缘，这位" + RANK_D->query_respect(ob)+"还是请回吧。");
						command("wave " + ob->query("id"));
						return;
        }
        if ((int)ob->query_skill("zixia-gong",1) < 180){
                command("say 这位" + RANK_D->query_respect(ob)+
                        "，你的紫霞神功等级过低，怕不能领悟我独孤九剑精髓。");
                return;
        }
        if ((int)ob->query_skill("huashan-jianfa",1) < 180){
                command("say 这位" + RANK_D->query_respect(ob) +
                        "，你的华山剑法等级过低，怕不能领悟我独孤九剑精髓。");
                return;
        }
        command("say 好吧，我就收下你了,你可要为我们华山派争口气。");
        command("chat* 脸上浮现出一丝笑容，对着"+ob->query("name")+"笑道：我华山后继有人了。");
        command("chat* nomatch feng");
        command("recruit " + ob->query("id"));
        ob->set("quest/huashan","剑宗");
        ob->set("title", HIW"华山派风清扬传人"NOR);
}
