// fan.c 范遥
// 4/4/97 -qingyun
// 16.10.98 snowman
// Modify By River@sj 99.6
//modify by looklove@sj 2000.11
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_me();
void create()
{
        set_name("范遥",({"fan yao","fan","yao"}));
        set("title","明教光明右使");
        set("nickname",WHT"逍遥二仙"NOR);
        set("long", "他就是明教的光明右使，和杨逍合称逍遥二仙的范遥。\n"+
                    "他身着一身白衫，虽然已是中年，但看上去还十分英俊潇洒。\n");
        set("age", 56);
        set("attitude", "peaceful");
        set("shen", -10000);
        set("str", 26);
        set("per", 28);
        set("int", 29);
        set("con", 27);
        set("dex", 30);
        set("max_qi", 3500);
        set("max_jing", 2200);
        set("eff_jingli", 2500);
        set("jiali", 80);
        set("combat_exp", 1500000); 
        set("unique", 1);

        set_skill("sword",170); 
        set_skill("dodge",170);
        set_skill("force", 170);
        set_skill("blade",170);
        set_skill("literate",150);
        set_skill("hand",170);
        set_skill("strike",170);
        set_skill("hanbing-mianzhang",170);
        set_skill("yingzhua-shou",170);
        set_skill("piaoyi-shenfa", 170);
        set_skill("shenghuo-shengong", 170);
        set_skill("lieyan-dao",170);
        set_skill("liehuo-jian",170);
        set_skill("parry", 170);

        map_skill("force", "shenghuo-shengong");
        map_skill("dodge", "piaoyi-shenfa");
        map_skill("sword", "liehuo-jian");
        map_skill("parry", "liehuo-jian");
        map_skill("blade", "lieyan-dao");
        map_skill("hand", "yingzhua-shou");
        map_skill("strike", "hanbing-mianzhang");
        prepare_skill("strike", "hanbing-mianzhang");
        prepare_skill("hand","yingzhua-shou");

        create_family("明教",35,"使者");

        set("inquiry", ([
                 "黑玉断续膏" : (: ask_me :),
        ]));
        set("chat_chance_combat", 20);
        set("chat_msg_combat", ({
                (: exert_function, "tougu" :),
	}));
        setup();
        if ( random(10) > 5 )
        carry_object(BINGQI_D("blade"))->wield();
        carry_object("/d/mingjiao/obj/white-cloth")->wear();
}


void init()
{
	::init();
        add_action("do_kill", "kill");
        add_action("do_kill", "tk");
        add_action("do_kill", "teamkill");
        add_action("do_kill", "hit");
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
        if(fam["master_name"] == "张无忌"){
                command("haha");
                command("say "+RANK_D->query_respect(ob)+"已是教主亲传弟子，"+RANK_D->query_self(me)+"怎敢再收你为徒？"); 
                return;
                }
        if(fam["master_name"] == "杨逍"){
                command("say 逍遥二仙不分你我，既然逍兄最近公事繁忙，你就暂时来我这里吧。");
                ob->set("title", this_object()->query("title")+"座下弟子");
                command("recruit " + ob->query("id"));
                return;
                }
        if (fam["generation"] > (my_fam["generation"] + 2)){
                command("say 依我看你应该先在四位法王那里把本事学好再来。");
                return;
                }
        if ((int)ob->query_skill("shenghuo-shengong", 1) < 120) {
                command("say 要学更为高深的武功，必须有高深的内功修为。"); 
                command("say " + RANK_D->query_respect(ob)+"是否还应该在圣火神功上多下点功夫？");
                return;
                }
        if ((int)ob->query_int() < 30) {
                command("say 逍遥二仙的武功博而杂，要学会这些武功，非得有极高的悟性。");
                command("say 在悟性方面，"+RANK_D->query_respect(ob)+"是否还不够？");
                return;
                }
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title", HIW+this_object()->query("title")+"座下弟子"NOR);
}

string ask_me()
{
	mapping fam; 
	object ob, *clone;
	int i, j;

	if(!(fam = this_player()->query("family")) || fam["family_name"] != "明教")
		return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？";

	if (! this_player()->query_skill("jiuyang-shengong"))
	if(this_player()->query_skill("shenghuo-shengong", 1) < 50)
		return "你在明教的时日尚短，这黑玉断续膏你还不够资格使用。";

	if(this_player()->query_temp("marks/膏1"))
		return "你不是刚拿了黑玉断续膏了吗，怎么还要？";

	if(present("heiyu duanxugao", this_player()))
		return "你身上不是带着黑玉断续膏吗，怎么还要？";

	if ((int)this_player()->query("eff_qi") == (int)this_player()->query("max_qi"))
		return "你现在不需要用黑玉断续膏。";

	ob=unew(MEDICINE_D("heiyu"));        
	if(!clonep(ob)) return "你来晚了，黑玉断续膏已给全部发出去了。"; 
	i = ob->query("unique");
	clone = filter_array(children(base_name(ob)), (: clonep :));
	j = sizeof(clone);
	while (j--) if (!clone[j]->query("clone_by_wizard")) i--; 
	if(clonep(ob) && i < 0){
		destruct(ob);
		return "抱歉，你来晚了，黑玉断续膏已经没有了。";
	} 
	ob->move(this_player());
	this_player()->set_temp("marks/膏1",1);
	return "好吧，这盒黑玉断续膏你就拿去用吧。";
}

int do_kill(string arg)
{
	object ob,ob2; 
	object me = this_player();
	ob2 = this_object();

	if (!arg) return 0;
	ob = present(arg, environment(me));
	if (ob == this_object() || ob == me) return 0;
	if (!ob || !ob->is_character()) return 0;
        
	if (userp(ob) && ob->query("family/family_name") == "明教") {
                if (me->query("family/family_name") == "明教") { 
			message_vision(ob2->name()+"对$N喝道：身为明教弟子，竟然杀自己的同门？我宰了你！\n", me);
		}
		else {
			message_vision(ob2->name()+"对$N喝道：大胆狂徒，居然敢来明教撒野！\n", me);
			kill_ob(me);
		}
		me->set("vendetta/明教", 1);
		return 1;
	}
	return 0;
}
