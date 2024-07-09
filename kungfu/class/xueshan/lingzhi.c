// lingzhi.c 灵智上人
// by iceland

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

int ask_me_1();
int ask_me_2();

void create()
{
        set_name("灵智上人", ({ "lingzhi shangren", "lingzhishangren" ,"lingzhi","shangren" }));
        set("long",@LONG
他长得白白净净，身体肥胖，颈项中的肥肉叠了至少三四层。慈眉善目，老是
笑眯眯的模样，张嘴一乐就露出两颗虎牙。
LONG
        );
        set("title", HIY "大轮寺第十二代弟子" NOR);
        set("gender", "男性");
        set("age", 41);
        set("attitude", "friendly");
        set("shen_type", -1);
        set("str", 28);
        set("int", 20);
        set("con", 25);
        set("dex", 27);
        set("max_qi", 1800);
        set("eff_jing", 900);
	set("max_jing",900);
        set("neili", 2500);
        set("max_neili", 2500);
        set("jiali", 100);
        set("combat_exp", 600000);
        set("score", 25000);
	set("unique", 1);

        set_skill("huanxi-chan", 120);
        set_skill("literate", 100);
        set_skill("force", 120);
        set_skill("longxiang-boruo", 120);
        set_skill("dodge", 120);
        set_skill("yuxue-dunxing", 120);
        set_skill("hand", 120);
	set_skill("poison",120);
        set_skill("dashou-yin", 120);
        set_skill("parry", 120 );
        set_skill("staff",200);
        set_skill("wushang-dali",200);

        map_skill("force", "longxiang-boruo");
        map_skill("dodge", "yuxue-dunxing");
        map_skill("hand", "dashou-yin");
        map_skill("parry", "dashou-yin");
        map_skill("staff", "wushang-dali");

        prepare_skill("hand","dashou-yin");

        create_family("大轮寺", 12, "弟子");
        set("class", "huanxi");
        set("inquiry", ([
                "俸禄" : (: ask_me_1 :),
                "供奉" : "善哉，善哉，请把酥油倒在长明灯里吧。\n",
        ]));

        set("chat_chance_combat", 30);
        set("chat_msg_combat", ({
                (: exert_function, "shield lingzhi shangren" :),
                (: perform_action, "hand.tianyin" :), }));

        setup();
        carry_object("/d/xueshan/npc/obj/qinxiu-jiasha")->wear();

        add_money("silver",50);
}

void attempt_apprentice(object ob)
{
       	if( (string)ob->query("gender") == "女性" ) {
               command("shake");
               command("say 我佛法还不够深湛，恐怕一个把持不住...你还是去找别人吧。");
               return;
	}
        if ((string)ob->query("family/family_name") != "大轮寺") {
                command("say"+ RANK_D->query_respect(ob) +
                        "不是本寺弟子，在这里捣什么乱？！");
                return;
	}
        if ((int)ob->query_skill("huanxi-chan", 1) < 80) {
                command("say 欢喜禅是各项武功之本，你再勤加修炼吧。");
                command("say"+ RANK_D->query_respect(ob) +
                        "再熬几个通宵，多多钻研钻研本门的心法吧。");
                return;
        }

        command("say 好吧，以后你就是我的弟子了。");
        command("recruit " + ob->query("id"));
        ob->set("title", HIY "大轮寺第十三代弟子" NOR);
}

int ask_me_1()
{
	object me;
	int skill,i,j;
	mapping myfam;
	me=this_player();

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	skill = me->query_skill("huanxi-chan",1);
	myfam = (mapping)me->query("family");
        i = (int)me->query("join_age");
        j = (int)me->query("gf_ok");

	if (!myfam || myfam["family_name"] != "大轮寺") {
		command("sneer "+(string)me->query("id"));
		command("say 看"+ RANK_D->query_respect(me) +"贼眉鼠眼，面生得很呀，不知道问这个做什么？\n");
		return 1;
	}
	if((int)me->query("xs_given")>=(int)me->query("age")) {
		command("sigh "+(string)me->query("id"));
		command("say 你今年的俸禄不是已经领走了么？明年再来吧。\n");
		return 1;
	}
	if( (( i + j ) < (int)me->query("age")) && ( skill < 90 ) ) {
		if((int)me->query("xs_given")>=(int)me->query("age")) {
			command("shake "+(string)me->query("id"));
			command("say "+ RANK_D->query_respect(me) +"历年供奉有所短缺，今年不能领取俸禄，明年再来吧。\n");
			return 1;
		}
		command("shake "+(string)me->query("id"));
		command("say "+ RANK_D->query_respect(me) +"历年供奉有所短缺，今年不能领取俸禄，明年再来吧。\n");
    		me->add("gf_ok",1);
        	me->set("xs_given",(int)me->query("age"));
		return 1;
	 }

	if((int)me->query("xs_given")<(int)me->query("age")) {
		if (skill < 60){
			command("pat "+(string)me->query("id"));
			command("say 你佛法尚未小成，还是勤加修炼为是。\n");
			return 1;
		}
		if (skill >= 60 && skill < 90) {
			me->set_temp("give_gold", 30);
			remove_call_out("give_money");
			call_out("give_money", 1, me);
			return 1;
		}
		if (skill >= 90 && skill < 140)	{
			me->set_temp("give_gold", 60);
			remove_call_out("give_money");
			call_out("give_money", 1, me);
			return 1;
		}
		if (skill >= 140 && skill < 180) {
			me->set_temp("give_gold", 100);
			remove_call_out("give_money");
			call_out("give_money", 1, me);
			return 1;
		}
		if (skill >= 180){
			me->set_temp("give_gold", 150);
			remove_call_out("give_money");
			call_out("give_money", 1, me);
			return 1;
		}
	}
}

int give_money(object me)
{
	int amount;
	object n_money;
        command("buddhi lingzhi shangren");
        command("say"+ RANK_D->query_respect(me) +"请拿好俸禄，尽可去精研佛法，不必再为衣食些许俗务分心了。");
        message_vision("灵智上人拿出几锭黄金，递给了$N。\n",me);
        n_money = new(GOLD_OB);
        n_money->move(me);
        amount = me->query_temp("give_gold");
        n_money->add_amount(amount);
        me->delete_temp("give_gold");
        me->set("xs_given",(int)me->query("age"));
}
