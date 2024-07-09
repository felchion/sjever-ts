// peng.c 彭莹玉
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
void create()
{
        set_name("彭莹玉", ({ "peng yingyu","peng","yingyu" }) );
        set("title", "明教五散人");
        set("gender", "男性");
        create_family("明教",36,"散人");
        set("age",51);
        set("class", "bonze");
        set("long","他就是明教五散人之一，人称彭和尚。\n");
        set("rank_info/respect","大师");
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 25);
        set("per", 12);
        set("combat_exp", 600000);
        set("unique", 1);

        set_skill("sword",80);
        set_skill("cuff",80);
        set_skill("literate",130);
        set_skill("force",80);
	set_skill("parry", 80);
        set_skill("dodge",80);
        set_skill("liehuo-jian",85);
        set_skill("piaoyi-shenfa",85);
        set_skill("shenghuo-shengong",80);
        set_skill("hand",80);
        set_skill("qianye-shou",85);
        map_skill("force","shenghuo-shengong");
        map_skill("hand","qianye-shou");
        map_skill("dodge","piaoyi-shenfa");
        map_skill("sword","liehuo-jian");
        map_skill("parry","liehuo-jian");
        prepare_skill("hand", "qianye-shou");

        set("max_qi",1500);
        set("max_jing",1200);
        set("jiali",50);
        set("eff_jingli",1100);

        setup();
        carry_object("/d/mingjiao/obj/yel-cloth")->wear();
}

void attempt_apprentice(object ob)
{     
        command("say 阿弥陀佛，善哉，善哉，贫僧已许久不收弟子了。");
        command("say 这位"+RANK_D->query_respect(ob)+"还是请回吧。");
        return;
}
