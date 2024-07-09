// le.c 嵩山派第三代弟子 乐厚
// Lklv 2001.10.18 Modified

#include <ansi.h>
inherit NPC;

int ask_here();
int ask_idea();
int ask_yy();
int ask_yy2();
void create()
{
        set_name("乐厚", ({ "le hou", "le", "hou" }));
        set("nickname", HIC"大阴阳手"NOR);
        set("long", "这人矮矮胖胖，面皮黄肿，五十来岁年纪，嵩山派的一名好手。\n");
        set("gender", "男性");
        set("age", 46);
        set("per", 17);
        set("attitude", "heroism");
        set("shen_type", -1);
        set("str", 29);
        set("int", 23);
        set("con", 23);
        set("dex", 23);
        set("unique", 1);

        set("max_qi", 3000);
        set("max_jing", 2000);
        set("eff_jingli", 2000);
        set("jiali", 50);
        set("combat_exp", 1000000);
        set("shen", -13000);

	set_skill("force", 150);
	set_skill("hanbing-zhenqi", 150);
	set_skill("dodge", 150);
	set_skill("zhongyuefeng", 150);
	set_skill("hand", 160);
	set_skill("songyang-shou", 160);
	set_skill("parry", 150);
	set_skill("sword", 150);
	set_skill("songshan-jian", 150);
	set_skill("literate", 100);
	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-jian");
	map_skill("sword", "songshan-jian");
	prepare_skill("hand", "songyang-shou");
	create_family("嵩山派", 3, "弟子");
	set("inquiry", ([
                "here": (: ask_here() :),
                "方法": (: ask_idea() :),
                "大阴阳手": (: ask_yy() :),
                "大阴阳手绝技": (: ask_yy2() :),
                "绝技": (: ask_yy2() :),
		"嵩山派" : "我们嵩山派是五岳剑派之首！\n",
		"五岳剑派" : "我们五岳剑派在武林之中势力巨大，可与少林、武当抗衡。\n",
		"左冷禅" : "他就是我们的掌门人，功夫深不可测。\n",
	]) );
        set("chat_chance_combat", 30);
        set("chat_msg_combat", ({
                (: perform_action, "hand.yinyang" :),
        }));
        setup();
        carry_object(ARMOR_D("changpao1"))->wear();
}

int ask_here()
{
	object me = this_player();

	if (!me->query_temp("ss/zsb_3") || me->query_skill("dodge")<250){
		command("say 这里是大铁梁峡，此处地势险恶，"+RANK_D->query_respect(me)+"您最好不要在此久留！");
		return 1;
	}
	me->delete_temp("ss/zsb_3");
	me->set_temp("ss/zsb_4",1);
	command("say 这里是大铁梁峡，此处地势险恶，当年我师伯的一条紫松鞭从这里掉下去再也没能找回来。");
	command("whisper "+me->query("id")+" 不过嘛，我倒有一个方法可以下去，不知"+RANK_D->query_respect(me)+"可愿前往？");
	return 1;
}

int ask_idea()
{
	object me = this_player();

	if (!me->query_temp("ss/zsb_4")){
		command("? " + me->query("id"));
		return 1;
	}
	if (me->query_skill("dodge") < 250){
		command("say "+RANK_D->query_respect(me)+"，你的轻功修为太差，恐怕难以下谷。");
		return 1;
	}
	if (me->query_skill("force") < 250){
		command("say "+RANK_D->query_respect(me)+"，峡谷内寒冷异常，你恐怕难与谷内寒气相抗衡。");
		return 1;
	}
	me->delete_temp("ss/zsb_3");
	me->set_temp("ss/zsb_4",1);
	command("whisper "+me->query("id")+" 我嵩山的松枝极其柔软，只需要找几枝松枝，撮成绳，就可以下谷了。");
	return 1;
}

int ask_yy()
{
	object me = this_player();

	if (!me->query("family") || me->query("family/family_name") != "嵩山派"){
		command("say "+RANK_D->query_respect(me)+"，这是我的绝技，但不传外派的。");
		return 1;
	}
	command("say "+RANK_D->query_respect(me)+"，我的大阴阳手绝技，那可是天下武功数一数二的！");
	me->set_temp("ss/yy1",1);
	return 1;
}

int ask_yy2()
{
	object me = this_player();
	object ob = this_object();

	if (!me->query_temp("ss/yy1")){
		command("say "+RANK_D->query_respect(me)+"，你这是什么意思？");
		return 1;
	}
	if (!me->query("ss/yy")){
		if (me->query_skill("songyang-shou",1) < 120){
			command("say "+RANK_D->query_respect(me)+"，你功力太弱，恐怕难以学会！");
			return 1;
		}
		message_vision("$N在$n耳边悄悄说了几句话。\n",ob,me);
		tell_object(me,HIY"乐厚随即将阴阳手的诀窍悉数传授于你，你终于学会了大阴阳手！\n"NOR);
		me->set("ss/yy",1);
		me->delete_temp("ss/yy1");
		return 1;
	}
	else{
		command("say "+RANK_D->query_respect(me)+"，你已经掌握了大阴阳手绝技的精髓了呀！");
		return 1;
	}
}

#include "app3.h";