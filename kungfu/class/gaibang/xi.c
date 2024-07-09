// NPC: CLASS_D gaibang/xi.c 奚长老
// Modify by Looklove@sj 2000/9/19

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

string ask_yao();
string ask_dan();
#include "begger.h";
int ask_gold(object who);

void create()
{
        set_name("奚长老", ({"xi zhanglao", "xi", "zhanglao"}));
        set("title", HIW"丐帮九袋长老"NOR);
        set("gb/bags",9);
        set("gender", "男性");
        set("age", 60);
        set("long","他乃丐帮六大长老之一,一根钢杖使得出神入化。\n");
        set("attitude", "peaceful");
        set("unique", 1);
        set("str", 25);
        set("int", 18);
        set("con", 20);
        set("dex", 20);

        set("qi", 1000);
        set("max_qi", 1000);
        set("jing", 600);
        set("max_jing", 600);
        set("eff_jingli", 860);
        set("neili", 2400);
        set("max_neili", 1400);
        set("jiali", 100);

        set("yao_count",20);
        set("dan_count",20);
        set("combat_exp", 500000);

        set_skill("force", 120); // 基本内功
        set_skill("huntian-qigong", 120); // 混天气功
        set_skill("dodge", 120); // 基本躲闪
        set_skill("xiaoyaoyou", 120); // 千里行乞
        set_skill("parry", 120); // 基本招架
        set_skill("staff", 140);
        set_skill("fengmo-zhang",150);
        set_skill("lianhua-zhang",120);
        set_skill("strike",120);
        set_skill("begging", 90);

        map_skill("force", "huntian-qigong");
        map_skill("parry","fengmo-zhang");
        map_skill("staff","fengmo-zhang");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("strike", "lianhua-zhang");
        prepare_skill("strike", "lianhua-zhang");

        create_family("丐帮", 18, "九袋长老");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "唉。。。乔。。。他以前还跟过我学掌法呢。\n",
                "萧峰" : "他不就是乔峰，我们的前任帮主。\n",
                "学掌法" : "那都是很多年前的事了，他那时常一个小岛上埋头苦练。\n",
                "小岛" : "我老了，以前的事也不记得太清楚了，好象是在福州附近吧。\n",
                "伤药" : (: ask_yao :),
                "解毒" : (: ask_dan :),
                "百草丹" : (: ask_dan :),
                "立功" : (: ask_gold :),

        ]));
        setup();
        carry_object(__DIR__"obj/gb_budai9")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("gangzhang"))->wield();
}

void attempt_apprentice(object ob)
{
        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
        }

	if ((int)ob->query_str() <=20	) {
		command("say 你膂力不足，恐怕学不了我的功夫。\n");
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
	command("recruit " + ob->query("id"));

        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大勇分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大勇分舵");
        ob->set("class","beggar");
}

string ask_yao()
{
        object me = this_player();

        if (me->query("family/family_name") != "丐帮")
		return RANK_D->query_respect(me) + "非我丐帮好汉，不知此话从何谈起？";
        if (me->query("max_neili" ) < 100)
		return RANK_D->query_respect(me) + "目前武功还没有根基，伤药留给更需要它的帮中兄弟吧。";
        if (me->query("eff_qi",1) == me->query("max_qi",1))
		return RANK_D->query_respect(me) + "并没有受伤，伤药留给受了伤的帮中兄弟吧。";
        if (present("gaibang shangyao", me))
                return RANK_D->query_respect(me) + "伤药很珍贵，你身上还有，还是留一些给其他帮中兄弟吧！";

        if (query("yao_count") < 1) return "目前伤药没有了，等会吧";

        new(MEDICINE_D("gb_shangyao"))->move(me);
        add("yao_count", -1);
        message_vision("$N获得一包丐帮伤药。\n",me);
        return "我这里有一包丐帮伤药，功效显著，你拿去护身吧。";
}

string ask_dan()
{
        object ob, me = this_player();

	if (me->query("family/family_name") != "丐帮")
                return "快去找医生吧，我可没办法！";
        if (me->query("eff_jing",1) == me->query("max_jing",1))
                return "你好象没有中毒嘛，来找我做什么？";
        if (query("dan_count") < 1)
                return "你来晚了，百草丹全部发完了，快去自己找大夫吧。";
        if (present("bai caodan", me))
                return RANK_D->query_respect(this_player()) +
                "百草丹很珍贵，你身上还有，还是留一些给其他帮中兄弟吧，丐帮弟子最讲义气。！";

        add("dan_count", -1);
        ob = new(MEDICINE_D("bcd"));
        ob->move(me);
        return "我这里有一包百草丹，善解蛇虫杂毒，不知是否对你症况。";
}

int ask_gold(object who)
{
	object me = this_player();
	int gold = 0;

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	if( me->query("family/family_name") != "丐帮" ) {
		command("say " + RANK_D->query_respect(me) + "非我丐帮好汉，不知此话从何谈起？");
		return 1;
	}

	if( (int)me->query_skill("huntian-qigong", 1) < 50 ) {
		command("say " + RANK_D->query_respect(me) + "你不好好学武，怎么能为我丐帮立功呢？");
		return 1;
	}

	if( me->query("gb_given") >= 300 ) {
		command("say " + RANK_D->query_respect(me) + "武功已有见数，应该为我丐帮更出些力啊！");
		return 1;
	}

	if( (int)me->query("gb_given") + 20 < (int)me->query_skill("huntian-qigong", 1) ) {
		if( (int)me->query_skill("huntian-qigong", 1) > 300 ) 
			gold = 300 - (int)me->query("gb_given");
		else gold = (int)me->query_skill("huntian-qigong", 1) - (int)me->query("gb_given");
		gold *= 10000;

		if( (int)me->query_skill("literate", 1) < 100 ) gold *= 5; 
		else if( (int)me->query_skill("literate", 1) >= 150 ) gold *= 3; 
		else gold *= 4;

		command("interest");
		me->add("balance", gold);
		me->set("gb_given", (int)me->query_skill("huntian-qigong", 1));
		command("say 既然" + RANK_D->query_respect(me) + "有这想法，我这就将" + MONEY_D->money_str(gold) + "存入你的钱庄，你定要好好为我丐帮立功啊！");
	} else command("say " + RANK_D->query_respect(me) + "，我丐帮以武功群领武林，在武学方面马虎不得啊！");
	return 1;
}
