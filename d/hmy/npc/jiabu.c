// jiabu.c
#include <ansi.h>
#include <wanted.h>
inherit NPC;

string give_pass();

string *unknow = ({
	"你是何人？如何上得黑木崖？",
	"切口？这等机密尔等无权得知。",
	"不知。",
});

void create()
{
        set_name("贾布", ({ "jia bu", "jia", "bu"}));
	set("nickname", "黄面尊者");
        set("long",
"他那一张瘦脸蜡也似黄，两边太阳穴高高鼓起，便如藏了一杖核桃相似。\n");
        set("title",HIY"日月神教  "HIC"青龙堂长老"NOR);
        set("gender", "男性");
	set("age", 40);
	set("attitude", "peaceful");
	set("shen_type", -1);
	set("str", 23);
	set("int", 23);
	set("con", 24);
	set("dex", 28);

        set("max_qi", 2500);
        set("max_jing", 1500);
        set("eff_jingli", 1500);
        set("jiali", 50);
        set("combat_exp", 1200000);

        set_skill("parry",170);
        set_skill("dodge",170);
        set_skill("force",170);
        set_skill("strike",170);
        set_skill("literate", 120);
        set_skill("ding-dodge",170);
        set_skill("huntian-qigong",170);
        set_skill("tiangang-zhang",170);

        map_skill("dodge", "ding-dodge");
        map_skill("force", "huntian-qigong");
        map_skill("parry", "tiangang-zhang");
        map_skill("strike", "tiangang-zhang");
        prepare_skill("strike", "tiangang-zhang");

	set("inquiry", ([
		"切口" : (: give_pass :)
	]));
        setup();

	carry_object(MISC_D("cloth"))->wear();
}

string give_pass()
{
	object me = this_object();
	object ob;

	ob = environment(me);
	if (ob == load_object("/d/hmy/shimen.c"))
	if (!is_wanted(this_player()) && random(3) == 1)
			return "今日的切口是："+ob->query("pass");
	return unknow[random(sizeof(unknow))];
}
