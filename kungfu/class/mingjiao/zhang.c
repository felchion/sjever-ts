// zhang.c 张中
// Modify By River@sj 99.06

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_me();
string ask_gold();
string ask_xunluo();
int do_yes();
int do_no();

object ob = this_object();

void create()
{
	set_name("张中", ({ "zhang zhong", "zhang", "zhong" }) );
	set("nickname","铁冠道人");
	set("title", "明教五散人");
	set("gender", "男性");
	set("age",51);
	set("long", "他就是明教五散人之一，人称铁冠道人的张中。\n");
	set("rank_info/respect","道长");
	set("str", 25);
	set("int", 25);
	set("con", 25);
	set("dex", 25);
	set("shen_type",0);
	set("combat_exp", 600000);
	set("unique", 1);
	set("no_get","张中对你来说太重了。\n");

	set_skill("sword",80);
	set_skill("literate",80);
	set_skill("force",80);
	set_skill("liehuo-jian",85);
	set_skill("dodge",80);
	set_skill("parry", 80);
	set_skill("cuff",80);
	set_skill("taizuquan",85);
	set_skill("piaoyi-shenfa",85);
	set_skill("shenghuo-shengong",85);
	map_skill("cuff","taizuquan");
	map_skill("force","shenghuo-shengong");
	map_skill("dodge","piaoyi-shenfa");
	map_skill("sword","liehuo-jian");
	map_skill("parry","liehuo-jian");
	prepare_skill("cuff", "taizuquan");
	create_family("明教", 36, "散人");
	set("max_qi",1500);
	set("max_jing",1200);
	set("jiali",50);
	set("eff_jingli",1100);
	set("inquiry", ([
		"宝鼎"  : (: ask_gold :),
		"太极拳经"  : (: ask_me :),
		"秘籍"  : (: ask_me :),
		"画印"  : (: ask_xunluo :),
	]));

	set("xl", 2);
	set("xl2", "zhang-zhong");
	setup();
	carry_object("/d/mingjiao/obj/yel-cloth")->wear();
	carry_object(BINGQI_D("changjian"))->wield();
}

#include "5san_app.h";
#include "xunluo.h";

string ask_me()
{
	mapping fam;
	object ob;

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "明教" )
		return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？";

	if( this_player()->query_temp("marks/秘1") ) {
		ob=unew("/d/mingjiao/obj/force_book");
		if(!clonep(ob)) return "你来晚了，太极拳经已给别人拿去了。";
		if(clonep(ob) && ob->violate_unique()){
			destruct(ob);
			return "你来晚了，太极拳经已给别人拿去了。";
		}
		ob->move(this_player());
		this_player()->delete_temp("marks/秘1");
		return "好吧，既然教主大人已经同意，你就把这本太极拳经拿去吧。\n";
	}
	return "对不起，不经教主同意我不能给你。\n";
}

string ask_gold()
{
	mapping fam;

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "明教" )
		return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？";

	if( this_player()->query_temp("mingjiao_gold") )
		return "我不是已经告诉你了嘛！你还想知道什么啊？";

	if( this_player()->query_skill("shenghuo-shengong", 1) < 60 )
		return RANK_D->query_respect(this_player()) + "来我教不久，整天打听，是何用意啊？";

	if( this_player()->query("mingjiao_given") >= 300 )
		return "我教宝鼎大多已经寻回，多谢" + RANK_D->query_respect(this_player()) + "关心。";

	if( this_player()->query_skill("shenghuo-shengong", 1) > this_player()->query("mingjiao_given") + 20 )
		this_player()->set_temp("mingjiao_gold", 1);

	return "我教大部分宝鼎已经流落江湖不知所踪，近日我派教中朱兄弟四处打听，不知现在可有消息？\n";
}

void init()
{
	object me, ob;
	::init();

	me = this_player();
	ob = this_object();        

	add_action("do_yes","yes");
	add_action("do_no","no");
	if( me->query_temp("mingjiao_gold") == 2 ) {
		command("touch " + me->query("id"));
		command("say " + RANK_D->query_respect(me) + "一定带来好消息了吧！");
		message_vision(HIW"$N将打听到关于宝鼎的消息告诉了$n。\n"NOR,me,ob);
		command("say 这位" + RANK_D->query_respect(me) + "，有心为我教出力，心意可佳！");
		command("say 我这里还有些银两，" + RANK_D->query_respect(me) + "是否愿意收下("HIY"yes"CYN"/"HIY"no"CYN")？\n");
		me->set_temp("mingjiao_gold", 3);
		me->add_busy(2);
	}
}

int do_yes()
{

	object me, ob;
	int gold = 0;
	me = this_player();
	ob = this_object();

	if( me->query_temp("mingjiao_gold") != 3 ) return 0;

	if( (int)me->query_skill("shenghuo-shengong", 1) > 300 ) 
		gold = 300 - (int)me->query("mingjiao_given");
	else gold = (int)me->query_skill("shenghuo-shengong", 1) - (int)me->query("mingjiao_given");

	gold *= 10000;

	if( (int)me->query_skill("literate", 1) < 100 ) gold *= 4; 
	else if( (int)me->query_skill("literate", 1) >= 150 ) gold *= 2; 
	else gold *= 3; 

	gold = gold/2 + random(gold/2);

	me->delete_temp("mingjiao_gold");
	me->add("balance", gold);
	me->set("mingjiao_given", (int)me->query_skill("shenghuo-shengong", 1));
	command("say 这里是" + MONEY_D->money_str(gold) + "，我这就将其存入你的钱庄，你以后一定要好好为我明教出力啊！");
	command("pat " + me->query("id"));
	return 1;
}

int do_no()
{
	object me, ob;
	int neili = 0, jingli = 0;

	me = this_player();
	ob = this_object();

	if( me->query_temp("mingjiao_gold") != 3 ) return 0;

	if( (int)me->query_skill("shenghuo-shengong", 1) > 300 ) 
		neili = 300 - (int)me->query("mingjiao_given");
	else neili = (int)me->query_skill("shenghuo-shengong", 1) - (int)me->query("mingjiao_given");

	if( (int)me->query_skill("force") < 200 ) neili = neili*3/2; 

	jingli = neili + random(neili/3);
	neili = neili + random(neili/5);

	me->delete_temp("mingjiao_gold");
	me->add("max_neili", neili);
	me->add("max_jingli", jingli);
	me->add("eff_jingli", jingli);
	me->set("mingjiao_given", (int)me->query_skill("shenghuo-shengong", 1));
	command("say 既然这位" + RANK_D->query_respect(me) + "不缺银两，为表心意，我就把这颗药丸送你服用吧！");
	message_vision("$N将$n给的药丸吞下，顿觉精力内力大增。\n"NOR,me,ob);
	tell_object(me, HIC"你增加了" + chinese_number(jingli) + "点精力和" + chinese_number(neili) + "点内力。\n"NOR);
	command("thank " + ob->query("id"));
	return 1;
}
