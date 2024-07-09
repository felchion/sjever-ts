// NPC zhangwuji.c 张无忌
// Modify By River@SJ 99.6

#include <ansi.h>
#include "zhangwuji.h";
inherit NPC;
inherit F_MASTER;
string ask_book();
string ask_qishang();
string ask_shu();
string ask_me();
int ask_hdg();
string ask_ling();
string ask_getling();
string ask_fel();
int pfm_zhen();
int pfm_chan();
int pfm_yi();
int pfm_qishang();
int pfm_tougu();
int pfm_tisha();
int pfm_duanhun();
int ask_hu_quest();
int ask_hu();

void create()
{
	object weapon;
	set_name("张无忌",({"zhang wuji","zhang","wuji"}));
	set("title",HIY"明教第三十四代教主"NOR);
	set("long", "他就是明教的第三十四代教主。脸上带着英资勃发的笑容。\n");
	set("age", 25);
	set("per", 32);
	set("attitude", "friendly");
	set("shen", 40000);
	set("str", 35);
	set("int", 30);
	set("con", 50);
	set("dex", 30);

	set("max_qi", 42500);
	set("max_jing", 17500);
	set("neili", 48000);
	set("eff_jingli", 7500);
	set("max_neili", 24000);
	set("jiali", 150);
	set("combat_exp", 15000000);
	set("unique", 1);
	set("no_bark",1);

	set_skill("sword", 500);
	set_skill("dodge", 500);
	set_skill("force", 500);
	set_skill("dagger", 500);
	set_skill("strike", 500);
	set_skill("hanbing-mianzhang", 500);
	set_skill("piaoyi-shenfa", 500);
	set_skill("tiyunzong", 500);
	set_skill("shenghuo-lingfa", 500);
	set_skill("jiuyang-shengong", 500);
	set_skill("lieyan-dao", 500);
	set_skill("blade", 500);
	set_skill("liehuo-jian", 500);
	set_skill("taiji-jian", 500);
	set_skill("taiji-quan", 500);
	set_skill("parry", 500);
	set_skill("cuff", 500);
	set_skill("qiankun-danuoyi", 500);
	set_skill("qishang-quan", 500);
	set_skill("literate", 560);
	set_skill("medicine", 300);
	set_skill("shenghuo-shengong", 500);
	set_skill("shenghuo-fumozhen", 500);
	set_skill("yingzhua-shou", 500);
	set_skill("hand", 500);

	map_skill("force", "jiuyang-shengong");
	map_skill("cuff","qishang-quan");
	map_skill("dagger","shenghuo-lingfa");
	map_skill("sword","taiji-jian");
	map_skill("dodge", "tiyunzong");
	map_skill("blade", "lieyan-dao");
	map_skill("strike", "hanbing-mianzhang");
	map_skill("parry", "qiankun-danuoyi");
	prepare_skill("cuff","qishang-quan");

	set("chat_chance_combat", 90);
	set("chat_msg_combat", ({
//		(: pfm_chan :),
		(: pfm_tisha :),
		(: pfm_qishang :),
		(: pfm_tougu :),
		(: pfm_yi :),
		(: pfm_zhen :),
		(: pfm_duanhun :),
	}));

	create_family("明教",34,"教主");

	set("inquiry", ([
		"波斯明教" : "我中土明教源于波斯明教，已有几百年厉史了，昔日小昭离我而去，\n"+
			"任波斯教主，不知她可好？吾心中常思念她，唉......",
		"圣火令" : "圣火令是我明教的圣物，但已失落多年。",
		"谢逊" : "义父的踪迹，我是不会随便对人讲的。",
		"鲜于通"  : (: ask_hu_quest :),
		"胡青牛"  : (: ask_hu :),
		"蝴蝶谷"  : (: ask_hdg :),
		"秘籍"  : (: ask_book :),
		"七伤拳经"  : (: ask_qishang :),
		"七伤拳谱" : (: ask_shu :),
		"传闻" : (: ask_me :),
		"铸令" : (: ask_ling :),
		"玄铁令" : (: ask_getling :),
		"赏善罚恶令" : (: ask_fel :),
	]));

	set_temp("apply/damage", 90);
	set_temp("apply/dodge", 90);
	set_temp("apply/attack", 90);
	set_temp("apply/armor", 90);
	setup();

	if (clonep()){
		if( sizeof(filter_array(children(BINGQI_D("shangshan_ling")), (: clonep :))) > 0 )
			weapon = new(BINGQI_D("tieling"));
		else if( sizeof(filter_array(children(BINGQI_D("fae_ling")), (: clonep :))) > 0 )
			weapon = new(BINGQI_D("tieling"));
		else weapon = unew(BINGQI_D("sf_ling"));
		if (!weapon) weapon = new(BINGQI_D("tieling"));
		carry_object(BINGQI_D("sword"));
		weapon->move(this_object());
		weapon->wield();
		carry_object("/d/mingjiao/obj/white-cloth")->wear();
	}
}

void init()
{
	::init();
	add_action("do_lingwu","taojiao");
	add_action("do_kill", "kill");
	add_action("do_kill", "teamkill");
	add_action("do_kill", "tk");
	add_action("do_kill", "hit");
}

void reset()
{
	object ob;

	if (clonep()) {
		if (!(ob = unew(BINGQI_D("shangshan_ling"))))
			return;
		destruct(ob);
		if (!(ob = unew(BINGQI_D("fae_ling"))))
			return;
		destruct(ob);
		if (ob = unew(BINGQI_D("sf_ling"))) {
			ob->move(this_object());
			command("wield shangshanfae ling");
		}
	}
}

void attempt_apprentice(object ob)
{
	mapping fam = ob->query("family");
	if (!fam || fam["family_name"] != "明教"){
		command("say "+RANK_D->query_respect(ob)+"与本派素无来往，不知此话从何谈起？");
		return;
	}
	if((int)ob->query_skill("shenghuo-shengong", 1) < 150) {
		command("say 我明教之圣火神功乃极好的内家心法。");
		command("say " + RANK_D->query_respect(ob)+"是否还应该先在内功上多下点功夫？");
		return;
	}
	if((int)ob->query_skill("medicine", 1) < 80) {
		command("say 我所学极杂，所依者乃是熟读医书。");
		command("say "+RANK_D->query_respect(ob)+"是否还应该先在本草术理上多下点功夫？");
		return;
	}
	if((int)ob->query_skill("literate", 1) < 100) {
		command("say 我所学极杂，你要想学好必须先饱览诗书。");
		command("say "+RANK_D->query_respect(ob)+"是否还应该先在读书写字上多下点功夫？");
		return;
	}
	if((int)ob->query("shen") < 100000) {
		command("say 学武之人，德义为先，功夫的高低倒还在其次，未练武，要先学做人。");
		command("say 在德行方面，"+RANK_D->query_respect(ob)+"是否还做得不够？");
		return;
	}
	if(ob->query_int() < 32) {
		command("say 我的武功都要极高的悟性才能学会。");
		command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
		command("say "+RANK_D->query_respect(ob)+"的悟性还大有潜力可挖，还是请回吧。");
		return;
	}
	command("say 嗯，看你还是个学武的料，我就收下你吧！");
	command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
	command("recruit " + ob->query("id"));
	ob->set("title",HIR"明教教主亲传弟子"NOR);
	if( ob->query_skill("dagger", 1) < ob->query_skill("balde", 1) ){
		ob->set_skill("dagger", ob->query_skill("balde", 1));
		tell_object(ob, "你受到教主指点，匕法已经达到了刀法的境界！\n");
	}
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
			message_vision(ob2->name()+"对$N喝道：大胆狂徒，居然不把本教主放在眼里！\n", me);
			kill_ob(me);
		}
		me->set("vendetta/明教", 1);
		return 1;
	}
	return 0;
}
