// /kungfu/class/emei/mie-jue.c
// 死尼姑灭绝
// Lklv 2001.10.18 update

#include <ansi.h>
inherit F_MASTER;
inherit NPC;
#include "miejue.h"
int ask_sword();
int ask_zhou();
int ask_guo();
int ask_jiuyin();
int ask_dragon();

void create()
{
	set_name("灭绝师太", ({"miejue shitai", "miejue", "shitai"}));
	set("long","她就是峨嵋派第三代掌门人灭绝师太了。\n"
"她容貌算得极美，但两条眉毛斜斜下垂，一副面相便变得极是诡异，几乎\n"
"有点儿戏台上的吊死鬼味道。\n"
	);
	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("class", "bonze");

	set("age", 50);
	set("shen_type", 1);
	set("str", 42);
        set("int", 42);
        set("con", 42);
        set("dex", 42);
        set("max_qi", 3500);
        set("max_jing", 3500);
        set("neili", 4500);
        set("max_neili", 4500);
        set("jiali", 100);
	set("combat_exp", 1200000);
	set("chat_chance_combat", 40);
	set("chat_msg_combat", ({
                (: perform_action, "sword.mie" :)
	}));

        set_skill("dacheng-fofa", 180);
        set_skill("linji-jianzhen", 180);
        set_skill("linji-zhuang", 170);
        set_skill("jieshou-jiushi", 170);
        set_skill("huifeng-jian", 170);
        set_skill("sixiang-zhang", 170);
        set_skill("yanxing-daofa", 170);
        set_skill("hand", 170);
        set_skill("literate", 180);
        set_skill("strike", 170);
        set_skill("sword", 170);
        set_skill("blade", 170);
        set_skill("parry", 170);
        set_skill("dodge", 170);
        set_skill("force", 170);
        set_skill("anying-fuxiang", 170);
        set("inquiry",([
		"倚天剑" : (: ask_sword :),
		"九阴真经" : (: ask_jiuyin :),
		"周芷若" : (: ask_zhou :),
		"郭靖" : (: ask_guo :),
		"黄蓉" : (: ask_guo :),
		"倚天屠龙" : (: ask_dragon :),
		"武林至尊" : (: ask_dragon :),
        ]) );

        map_skill("force", "linji-zhuang");
        map_skill("sword", "huifeng-jian");
        map_skill("blade", "yanxing-daofa");
        map_skill("parry", "huifeng-jian");
        map_skill("dodge", "anying-fuxiang");
        map_skill("strike", "sixiang-zhang");
        map_skill("hand", "jieshou-jiushi");

        prepare_skill("hand", "jieshou-jiushi");

        create_family("峨嵋派", 3, "掌门");

        setup();
        carry_object(BINGQI_D("changjian"))->wield();
        carry_object("/d/emei/obj/shoes")->wear();
        carry_object("/d/emei/obj/cloth")->wear();
}

void init()
{
        object me, ob;
        ::init();
        ob = this_player();
        me = this_object();
        if (interactive(ob = this_player()) && !wizardp(ob) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
        }
}

void greeting(object me)
{
        int shen;
        if(!me) return;
        shen = me->query("shen");

        if ( file_name(environment(this_object())) != this_object()->query("startroom")) return;

        if(shen <= -200000 && me->query("family/family_name") == "峨嵋派"){
        	command("chat* kick2 "+me->query("id"));
        	command("say 你满眼瘴气，身带邪气，再和邪魔外道来往我开除你出峨嵋派！");
	}
	if(shen <0 && shen > -200000 && me->query("family/family_name") == "峨嵋派"){
		command("look "+me->query("id"));
		command("say 我看你满眼瘴气，身带邪气，最近是不是在外面做了什么坏事？");
	}
	else command("buddhi miejue shitai");
}
