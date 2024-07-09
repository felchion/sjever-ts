// liang.c 梁长老
// Lklv 2001.9.28

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "bags_up.h"
int ask_xiaofeng();

#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"

void create()
{
	set_name("梁长老", ({"liang zhanglao", "liang", "zhanglao"}));
	set("title", HIW"丐帮首席长老"NOR);
	set("gb/bags", 9);
	set("gender", "男性");
	set("age", 55);
	set("long",
		"梁长老是丐帮出道最久，武功最高的长老，在武林中享名已久。\n"
		"丐帮武功向来较强，近来梁长老一力整顿，更是蒸蒸日上。\n");

	set("attitude", "peaceful");

	set("str", 28);
	set("int", 24);
	set("con", 24);
	set("dex", 25);

	set("qi", 4500);
	set("max_qi", 4500);
	set("jing", 4000);
	set("max_jing", 4000);
	set("eff_jingli", 3500);
	set("neili", 6000);
	set("max_neili", 5000);
	set("jiali", 120);

	set("combat_exp", 2500000);

	set_skill("force", 190);
	set_skill("huntian-qigong", 195);
	set_skill("strike", 185);
	set_skill("xianglong-zhang", 190);
	set_skill("dodge", 193);
	set_skill("xiaoyaoyou", 190);
	set_skill("parry", 180);
	set_skill("stick", 180);
	set_skill("dagou-bang", 180);
	set_skill("begging", 180);
	set_skill("checking", 190);

	map_skill("force", "huntian-qigong");
	map_skill("strike", "xianglong-zhang");
	map_skill("parry", "xianglong-zhang");
	map_skill("stick", "dagou-bang");
	map_skill("dodge", "xiaoyaoyou");
	prepare_skill("strike", "xianglong-zhang");

    	set("chat_msg_combat", ({
        	(: perform_action, "strike.xiao" :),
        	(: perform_action, "strike.paiyun" :),
        	(: exert_function, "huntian" :),
        }));

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "他不就是萧峰，我们的前任帮主。\n",
		            "萧峰" : (: ask_xiaofeng :),
		            "竹林惊变" : (: ask_xiaofeng :),
		            "丐帮内乱" : (: ask_xiaofeng :),
		            "竹林剧变" : (: ask_xiaofeng :),
                "升袋" : (: ask_me :),
                "功劳" : (: ask_bag :),

        ]));

	create_family("丐帮", 18, "首席长老");
	setup();

        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
	carry_object(__DIR__"obj/gb_budai9")->wear();
}

void attempt_apprentice(object ob)
{
        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
	}
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！滚！");
                command("kill "+ob->query("id"));
                return;
        }

        if((int)ob->query_skill("huntian-qigong",1) < 120){
           command("say 混天气功是丐帮祖上留传的功夫，你对此钻研不够啊。\n");
           return;
	}
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");

        command("recruit " + ob->query("id"));
        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮总舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","总舵");
        ob->set("class","beggar");
}

void init()
{
        add_action("do_kill", "kill");
        add_action("do_kill", "tk");
        add_action("do_kill", "teamkill");
        add_action("do_kill", "hit");
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


        if (userp(ob) && ob->query("family/family_name") == "丐帮") {
                if (me->query("family/family_name") == "丐帮") {
	                message_vision(ob2->name()+"对$N叫道：你居然杀自己的同门？我宰了你！"+ob->name()+"快走！\n", me);
	                kill_ob(me);
                }
                else {
                        message_vision(ob2->name()+"对$N喝道：大胆，居然敢在我面前撒野！"+ob->name()+"快走，让我来！\n", me);
                        kill_ob(me);
                }
                return 1;
        }
        return 0;
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
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，宋长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_liang"))
		{
			command("sigh");
      me->set_temp(QUESTDIR3+"ask_liang",1);
      message_vision(HIC"$N摇了摇头道：“宋长老不明白，我有何尝晓得。这萧峰，竟然是契丹人，可是这些年来怎么一点迹象也没有啊。”\n"NOR,this_object());
      message_vision(HIC"$N担心道：“这萧峰才略过人，英雄了得，当年谁不佩服？现在变成契丹人，反而这本事越大，大伙儿越是危险。”\n"NOR,this_object());
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
