// fei.c 嵩山派第三代弟子 费彬
// Lklv 2001.10.18 Modified

#include <ansi.h>
inherit NPC;

void create()
{
        set_name("费彬", ({ "fei bin", "fei", "bin" }));
        set("nickname", HIY"大嵩阳手"NOR);
        set("long", "这人四十来岁，中等身材，瘦削异常，上唇留了两撇鼠须，是嵩山
派掌门左冷禅的第四师弟费彬、一套大嵩阳手武林中赫赫有名。\n");
        set("gender", "男性");
        set("age", 49);
        set("per", 18);
        set("attitude", "heroism");
        set("shen_type", -1);
        set("str", 25);
        set("int", 23);
        set("con", 26);
        set("dex", 26);
        set("unique", 1);

        set("max_qi", 3000);
        set("max_jing", 2000);
        set("eff_jingli", 2000);
        set("jiali", 50);
        set("combat_exp", 1500000);
        set("shen", -14000);

	set_skill("literate", 100);
	set_skill("force", 165);
	set_skill("parry", 180);
	set_skill("sword", 190);
	set_skill("dodge", 160);
	set_skill("hand", 160);
	set_skill("zhongyuefeng", 160);
	set_skill("songyang-shou", 160);
	set_skill("hanbing-zhenqi", 160);
	set_skill("songshan-jian", 190);

	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-jian");
	map_skill("sword", "songshan-jian");
	prepare_skill("hand", "songyang-shou");
	create_family("嵩山派", 3, "弟子");
	set("chat_chance", 25);
	set("chat_msg", ({
		(: random_move :)
	}) );
	set("chat_chance_combat", 10);
	set("chat_msg_combat", ({
		(: perform_action, "hand.yinyang" :),
                (: exert_function, "jianqi" :),
        }));
	set("inquiry", ([
		"嵩山派" : "我们嵩山派是五岳剑派之首！\n",
		"五岳剑派" : "我们五岳剑派在武林之中势力巨大，可与少林、武当抗衡。\n",
		"左冷禅" : "他就是我们的掌门人，功夫深不可测。\n",
	]) );
        setup();
        carry_object(ARMOR_D("changpao1"))->wear();
}
