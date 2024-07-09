// NPC: song.c 宋长老
// Modify By Looklove 2000/10/18

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";
int ask_xiaofeng();

#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"
void create()
{
	set_name("宋长老", ({"song zhanglao", "song", "zhanglao"}));
	set("title", HIW"丐帮九袋长老"NOR);
    	set("gb/bags",9);
    	set("nickname", "白须老者");
	set("gender", "男性");
    	set("age", 65);
	set("long", "他乃丐帮六大长老之一。\n");
	set("attitude", "peaceful");

	set("str", 28);
	set("int", 20);
	set("con", 25);
	set("dex", 28);

	set("qi", 2800);
	set("max_qi", 2800);
	set("jing", 1600);
	set("max_jing", 1600);
	set("eff_jing", 2600);
	set("neili", 3800);
	set("max_neili", 3800);
	set("jiali", 30);

    	set("combat_exp", 550000);

	set_skill("force", 140); // 基本内功
	set_skill("huntian-qigong", 140); // 混天气功
	set_skill("hand", 140); // 基本拳脚
        set_skill("suohou-shou", 140);
	set_skill("dodge", 135); // 基本躲闪
        set_skill("xiaoyaoyou", 140); // 千里行乞
	set_skill("parry", 140); // 基本招架
        set_skill("stick", 100);
        set_skill("dagou-bang", 100);
	set_skill("begging", 100);

	map_skill("force", "huntian-qigong");
        map_skill("hand", "suohou-shou");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("parry", "suohou-shou");
	map_skill("stick", "dagou-bang");
	prepare_skill("hand", "suohou-shou");

	create_family("丐帮", 18, "九袋长老");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "升袋" : "找你的舵主去。\n",
                "乔峰" : "他不就是萧峰，我们的前任帮主。\n",
		            "萧峰" : (: ask_xiaofeng :),
		            "竹林惊变" : (: ask_xiaofeng :),
		            "丐帮内乱" : (: ask_xiaofeng :),
		            "竹林剧变" : (: ask_xiaofeng :),
		            "打狗棒法" : "打狗棒乃本帮镇帮之绝技，往往非帮主不传。\n"
			     "可惜乔帮主一去，帮中就再也没人会了，唉。。。\n"
			     "虽说三个臭皮匠凑成一个诸葛亮，我们几个长老，\n"
			     "聚在一块，揣摩良久，还只领悟了一点点。\n",
		"揣摩" : "我们只不过凭着一幅乔帮主昔年大战聚贤庄的图画，\n"
			 "希望能从中领悟打狗棒法的神勇，说来惭愧，不但 \n"
			 "领悟不出什么打狗棒法，连基本棒法都只领悟到一点点。\n",
		"图画" : "唉，不知哪个狗贼，竟把它偷去了！\n"
        ]));
        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
        	(: perform_action, "stick.chan" :),
        	(: exert_function, "huntian" :),
	}));

	setup();
	carry_object(__DIR__"obj/gb_budai9")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("zhubang"))->wield();
}

void attempt_apprentice(object ob)
{
	if ((int)ob->query_str() <=25	) {
		command("say 我的功夫刚猛强劲" +
		RANK_D->query_respect(ob) +
		"，臂力太弱，似乎不适合跟我学武功。\n");
		return;
	}

        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
	}
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

	if( ob->query_skill("huntian-qigong", 1) < 50){
		command("say "+RANK_D->query_respect(ob) +"的混天气功是不是还不够？");
		return;
	}
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        command("recruit " + ob->query("id"));
        ob->set("title",sprintf("丐帮总舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("class","beggar");
}
int ask_xiaofeng()
{
	object me = this_player();
	if(me->query(QUESTDIR2+"over") && !me->query(QUESTDIR3+"start"))
	{
		if(!me->query_temp(QUESTDIR3+"ask"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，丐帮其他长老都在场，你可以找其他人问问吧。”\n"NOR,this_object());
	    me->set_temp(QUESTDIR3+"ask",1);
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_chen"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，陈长老也在，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_song"))
		{
			command("sigh");
      me->set_temp(QUESTDIR3+"ask_song",1);
      message_vision(HIC"$N叹了口气，道：“这萧峰，或者是个装腔作势的大奸雄，或者是个直肠直肚的好汉子，我宋某没本事分辨。你找梁长老吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_liang"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，梁长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		command("sigh");
    message_vision(HIC"$N顿了一顿道：“这件事说起来牵连太多，传了出去，丐帮在江湖上再也抬不起头来，人人要瞧我们不起。去问别人吧。”\n"NOR,this_object(),me);
		return 1;
	}	
	if(me->query(QUESTDIR3+"start")&&!me->query(QUESTDIR3+"over"))
	{
		message_vision(HIC"$N脸上露出一幅咬牙切齿的样子。\n"NOR,this_object());
    command("say 萧峰是契丹狗种，还是堂堂汉人，此时还未分明。倘若他真是契丹胡虏，我宋某第一个跟他拚了。。"NOR);
		return 1;
	}	
	if(me->query(QUESTDIR3+"over"))
	{
		command("sigh");
		command("say 那萧峰要是还是我们帮主该多好......"NOR);
		return 1;
	}	
	message_vision(HIC"$N沉默了一会了，道：“萧峰正是我丐帮中人。”\n"NOR,this_object());
	return 1;
}
