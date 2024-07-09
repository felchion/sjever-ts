// moshenggu.c
// by ydh
// Modified by Lklv for raozhi Quest @@
// 2002.12.12 lane add gold

inherit NPC;
inherit F_MASTER;

int ask_me_1(object who);
int ask_raozhi();

void create()
{
        set_name("莫声谷", ({ "mo shenggu", "mo" }));
        set("nickname", "武当七侠");
        set("long",
                "他就是张三丰的七弟子莫声谷。\n"
                "他年纪最轻，但身材魁梧，精明能干。\n");
        set("gender", "男性");
        set("age", 22);
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("unique", 1);
        set("str", 26);
        set("int", 30);
        set("con", 26);
        set("dex", 28);

        set("max_qi", 1800);
        set("eff_jingli",1000);
        set("max_jing",1300);
        set("neili", 3500);
        set("max_neili", 2500);
        set("jiali", 50);
        set("combat_exp", 900000);
        set("score", 5000);

        set_skill("force", 150);
        set_skill("yinyun-ziqi", 160);
        set_skill("dodge", 150);
        set_skill("tiyunzong", 160);
        set_skill("cuff", 180);
        set_skill("taiji-quan", 160);
        set_skill("parry", 180);
        set_skill("sword", 180);
        set_skill("taiji-jian", 170);
        set_skill("taoism", 120);
        set_skill("literate", 130);

        map_skill("force", "yinyun-ziqi");
        map_skill("dodge", "tiyunzong");
        map_skill("cuff", "taiji-quan");
        map_skill("parry", "taiji-jian");
        map_skill("sword", "taiji-jian");
        prepare_skill("cuff", "taiji-quan");

        create_family("武当派", 2, "弟子");

        set("inquiry", ([
                "下山" : (: ask_me_1 :),
                "绕指柔剑" : (: ask_raozhi :),
                "绕指" : (: ask_raozhi :),
        ]));

        setup();
        carry_object(BINGQI_D("changjian"))->wield();
        carry_object("/d/wudang/obj/white-robe")->wear();
        carry_object("/d/wudang/obj/daolv")->wear();
}

int ask_raozhi()
{
        object me = this_player();

        if (me->query("raozhi")){
        	command("knock "+me->query("id"));
		return 1;
	}
        if (me->query_temp("raozhi")) {
            command("say 不是告诉你了么？");
            return 1;
        }
        if (me->query("family/family_name") != "武当派") {
		command("mimi "+me->query("id"));
		return 1;
        }
        if (random(3)>1 && me->query("family/family_name") == "武当派") {
		command("say 那是恩师传给我的一门特殊剑招。");
		command("say 我新学不久，无法精通，你还是亲自去请教他吧。");
		me->set_temp("raozhi",1);
		return 1;
        }
        return 0;
}

int ask_me_1(object who)
{
	object me = this_player();
	int gold = 0;

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	if( me->query("family/family_name") != "武当派" ) {
		command("hmm "+(string)me->query("id"));
		command("say 你非我派弟子，也敢来滥竽充数！\n");
		return 1;
	}

	if( me->query("shen") < 10000 ) {
		command("pat "+(string)me->query("id"));
		command("say 你的侠义正气不够，还是别下山的好！\n");
		return 1;
	}
	if( (int)me->query_skill("taoism", 1) < 30 ) {
		command("pat "+(string)me->query("id"));
		command("say 我武当派所属于道家，最重视道学修养，你似乎还是差了些啊！\n");
		return 1;
	}

	if( (int)me->query("wudang_given") < (int)me->query_skill("taoism", 1) ) {
		gold = (int)me->query_skill("taoism", 1) - (int)me->query("wudang_given");
		if( (int)me->query_skill("taoism", 1) >= 180 ) gold *= 3; 
		else if( (int)me->query_skill("taoism", 1) >= 120 ) gold *= 2; 
		gold *= 30000;
		command("nod "+me->query("id"));
		me->add("balance", gold);
		me->set("wudang_given", (int)me->query_skill("taoism", 1));
		command("say " + RANK_D->query_respect(me) + "的道学修为又更进一层，真是可喜啊！");
		command("say 我武当派侠义名满天下，你就拿些钱去锄强扶弱吧，" + MONEY_D->money_str(gold) + "已经存入钱庄了！");
	} else command("say 你下山后要多行侠仗义，要是为非做歹，我必清理门户！");
	return 1;
}

void attempt_apprentice(object ob)
{
       if( (int)ob->query("shen") < 10000 ) {
	       command("say 我武当乃是堂堂名门正派，对弟子要求极严。");
	       command("say 在德行方面，" + RANK_D->query_respect(ob) + "是否还做得不够？");
	       return;
	}
	if( (int)ob->query_skill("yinyun-ziqi", 1) < 30 ) {
		command("say 我武当派乃内家武功，最重视内功心法。");
		command("say " + RANK_D->query_respect(ob) + "是否还应该在氤氲紫气上多下点功夫？");
                return;
       }
       if (ob->query_int() < 28) {
                command("say 我武当派武功全从道藏悟出。");
                command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
                command("say " + RANK_D->query_respect(ob) + "的悟性还大有潜力可挖，还是请回吧。");
                return;
       }
       command("say 好吧，贫道就收下你了。");
       command("recruit " + ob->query("id"));
}
