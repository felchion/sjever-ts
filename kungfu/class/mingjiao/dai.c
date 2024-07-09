// NPC dai.c 黛绮丝
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
void create()
{
	object weapon;
        set_name("黛绮丝",({"dai qisi","dai","qisi"}));
        set("title","明教护教法王");
        set("nickname",MAG"紫衫龙王"NOR);
        set("gender","女性");
        set("long",  "她是明教的四大护法之一的紫衫龙王。\n"+
                     "她身材苗条，是当年明教的第一美女，\n"+
                     "虽年事已高，仍不资色仍不减当年。\n");
        set("age", 45);
        set("attitude", "peaceful");
        set("shen", -10000);
        set("str", 24);
        set("int", 40);
        set("con", 26);
        set("dex", 30);        
        set("max_qi", 2800);
        set("max_jing", 2200);
        set("eff_jingli", 2000);
        set("jiali", 10);
        set("combat_exp", 1100000);
        set("unique", 1);

        set_skill("throwing",180); 
        set_skill("dodge",140);
        set_skill("force", 140);
        set_skill("duoming-jinhua",180);
        set_skill("literate",130);
        set_skill("piaoyi-shenfa", 140);
        set_skill("shenghuo-shengong", 140);
        set_skill("parry", 140);
        set_skill("hand", 140);
        set_skill("yingzhua-shou",120);
        map_skill("force", "shenghuo-shengong");
        map_skill("dodge", "piaoyi-shenfa");
        map_skill("throwing", "duoming-jinhua");
        map_skill("parry", "duoming-jinhua");
        map_skill("hand", "yingzhua-shou");
        prepare_skill("hand","yingzhua-shou");

	set("chat_chance_combat", 40);
	set("chat_msg_combat", ({
		(: perform_action, "throwing.huayu":),
	}));

        create_family("明教", 36, "弟子");
        setup();
        if(clonep()){
        	weapon = unew(BINGQI_D("shanhu-jin"));
        	if(!weapon) weapon = new(BINGQI_D("jinhua"));
        	weapon->move(this_object());
		carry_object("/d/mingjiao/obj/yel-cloth")->wear();
		carry_object("/d/mingjiao/obj/luopan");
		carry_object(BINGQI_D("jinhua"))->wield();
	}
}

void attempt_apprentice(object ob)
{
        object me  = this_object();
        mapping fam = ob->query("family");
        mapping my_fam = me->query("family");
        if(!fam || fam["family_name"] != "明教"){
                command("say "+RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
                return;
        }
        if(ob->query("gender")!="女性"){
                command("say "+RANK_D->query_respect(ob)+"还是回去吧，老身只收女弟子。");
                return;
        }
        if(fam["master_name"] == "张无忌"){
                command("haha");
                command("say "+RANK_D->query_respect(ob)+"已是教主亲传弟子，"+RANK_D->query_self(me)+"怎敢再收你为徒？"); 
                return;
        }
        if (fam["generation"] == my_fam["generation"]){
                command("shake");
                command("say "+RANK_D->query_respect(ob)+"已拜光明使为师，"+RANK_D->query_self(me)+"怎敢再收你为徒？");
                return;
        }
        if ((int)ob->query_skill("shenghuo-shengong", 1) < 100) {
                command("say 要学更为高深的武功，必须有高深的内功修为。"); 
                command("say " + RANK_D->query_respect(ob)+"是否还应该在圣火神功上多下点功夫？");
                return;
        }
        if ((int)ob->query_int() < 28) {
                command("say 想学我的金花暗器，必须能够眼观四方，耳听八方。");
                command("say 在身体悟性方面，"+RANK_D->query_respect(ob)+"是否还嫌差了点？");
                return;
        }
        command("say 嗯，看你还是个学武的料，老身我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title", MAG"明教紫衫龙王属下教众"NOR);
}
