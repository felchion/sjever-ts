// 苏荃
// lane 2004.12.24 add 福利

#include <ansi.h>
inherit NPC;

int ask_zhushao();
void destructing(object ob);
string ask_houshan();
string ask_me();
string ask_skill();
int ask_gold();

void create()
{
	set_name("苏荃", ({ "su quan","su" }));
	set("long", "她看模样不过二十三四岁，是一个美貌少妇，微微一笑，更是媚态横生，\n艳丽无匹，她是神龙教教主的夫人。\n");
	set("gender", "女性");
        set("title", "神龙教教主夫人");
        set("age", 23);
        set("str", 22);
        set("int", 32);
        set("con", 22);
        set("dex", 38);
        set("per", 50);
        set("combat_exp", 3000000);
        set("shen", -1000);
        set("attitude", "peaceful");
        set("max_qi",9000);
        set("max_jing",5000);
        set("neili",9000);
        set("max_neili",9000);
        set("jingli",1800);
        set("eff_jingli",1800);
        set("jiali",150);
        set("score", 15000);
        set("yaoli_count",1);

	set_skill("force", 250);
	set_skill("dodge", 250);
	set_skill("parry",250);
	set_skill("strike",250);
	set_skill("huagu-mianzhang", 250);
	set_skill("youlong-shenfa",250);
	set_skill("dulong-dafa",250);
	set_skill("dagger",300);
	set_skill("tenglong-bifa",300);

	map_skill("force", "dulong-dafa");
	map_skill("dodge", "youlong-shenfa");
	map_skill("strike", "huagu-mianzhang");
	map_skill("dagger","tenglong-bifa");
	map_skill("parry","tenglong-bifa");

	set_temp("apply/damage", 88);
	set_temp("apply/dodge", 188);
	set_temp("apply/attack", 188);
	set_temp("apply/armor", 388);

	set("inquiry", ([
		"资金" : (: ask_gold :),
		"竹哨" : (: ask_zhushao :),
		"zhushao" : (: ask_zhushao :),
		"仙福居": (: ask_houshan() :),
		"神龙药理经" : (: ask_me :),
		"贵妃回眸" :(:ask_skill:),
	]) );

	set("chat_chance", 3);
	set("chat_msg", ({
		"苏荃自言自语道：玄铁匕首虽然不错，但毕竟不够锋利，唉！\n",
	}));

	create_family("神龙教",1, "教主夫人");
	setup();
	carry_object(__DIR__"obj/skirt")->wear();
	carry_object(BINGQI_D("dagger/sld_bishou"))->wield();
}

int accept_object(object me, object ob)
{
	object obj;

	if( ob->query("weapon_prop/damage") >= 80){
		obj=unew(BINGQI_D("xtbs"));
		if(obj) {
			obj->move(me);
			tell_object(me,"你能如此忠心，真是太好了，这把玄铁匕首就赏给你吧！\n");
			tell_room(environment(me), "苏荃给" + me->name() + "一柄玄铁匕首。\n");
		}
		else {
			tell_object(me,"你能如此忠心，真是太好了，教主不会亏待你的！\n");
			me->add("shen", -(3000+random(1000)));
		}
		call_out("destructing", 0, ob);
		return 1;
	}
	return 0;
}

void destructing(object ob)
{
	if(ob)
	destruct(ob);
}

int ask_zhushao()
{
	object me,ob,zs;
	mapping myfam;
	object* inv;
	int i;

	me = this_player();
	ob = this_object();
	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教") return 0;
	if(me->query("combat_exp") < 200000) {
		command("say 我现在可不能给你竹哨哦。");
		return 1;
	}

	inv = all_inventory(me);
	for(i=0; i<sizeof(inv); i++) {
		if(inv[i]->query("id")=="zhushao") {
			tell_object(me,"你不是已经有竹哨了么？\n");
			return 1;
		}
	}

	zs = new(__DIR__"obj/zhushao");
	if (!zs)  return notify_fail("异常错误！\n");
	zs->set("owner",me);

	tell_object(me,"好吧，我给你个竹哨，可不要弄丢了。\n");
	tell_room(environment(me), "苏荃给" + me->name() + "一个竹哨。\n");
	zs->move(me);

	return 1;
}

string ask_me()
{
	object ob;
	mapping fam;
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "神龙教")
		return RANK_D->query_respect(this_player()) + "与本派素无来往，不知此话从何谈起？";

	if ( this_player()->query_temp("ask_slylj") )
		return "刚刚不是给你了嘛。";
	ob=unew("/clone/book/slyaoli-book.c");

	if(!clonep(ob)) return "你来晚了，那本书我已经给别人了。";
	if(clonep(ob) && ob->violate_unique()){
		destruct(ob);
		return "你来晚了，那本书我已经给别人了。";
        }
	ob->move(this_player());
	this_player()->set_temp("ask_slylj", 1);
	message_vision("苏荃从怀中掏出一本神龙药理经递给$N。\n",this_player());
	return "这本书是我和陆先生借来看看的，你拿去可要好好研究啊。";
}

string ask_houshan()
{
	object me=this_player();
	int lvl = (int)me->query_skill("shenlong-yaoli", 1);
	if ( lvl < 120 ) return "仙福居乃我派逍遥胜地,你还不够资格去。";
	if ( file_name(environment(this_object())) != this_object()->query("startroom"))
		return "等我回神龙岛再说吧。";
	tell_object(me, "既然你问到了，那我就带你去吧。\n");
	me->move("/d/sld/xfj");
	return "又一个好苗子。";
}
string ask_skill()
{
	object me=this_player();
	mapping myfam;
			
	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教") return 0;
	if(me->query_skill("tenglong-bifa",1)<100) return 0;
	if(me->query("sld/guifei") || me->query_skill("tenglong-bifa",1)<150)
		return "你不是已经会贵妃回眸了么？";
	command("giggle");
	command("whisper "+getuid(me)+" 其实贵妃回眸这招还有别的精要之处，大致是这样的。。。。");
	me->set("sld/guifei",1);
	tell_object(me,HIW"你听着苏荃的指点，不住点头，渐渐就明白了贵妃回眸这一招的精要。\n"NOR);
	return "大致上就是这样了，本教武功虽不重攻击，但往往越战越强，所向披靡。";
}

int ask_gold()
{
	object me;
	int skill, gold;
	mapping myfam;
	me = this_player();
	skill = me->query_skill("dulong-dafa", 1);
	myfam = (mapping)me->query("family");

	if( !myfam || myfam["family_name"] != "神龙教" ) {
		command("sneer "+(string)me->query("id"));
		command("say 看"+ RANK_D->query_respect(me) +"面生得很呀，是不是考虑下加入我教啊？");
		return 1;
	}

	if( (int)me->query("sld_given") >= (int)me->query("age") ) {
		command("sigh "+(string)me->query("id"));
		command("say 你今年不是已经拿了钱了吗？明年再来吧。");
		return 1;
	}

	if( (int)me->query("age") < 18 || skill < 50 ) {
		command("interest");
		command("say "+ RANK_D->query_respect(me) +"年纪轻轻，来我教不久，还是安心学武吧。\n");
		return 1;
	}

	gold = 300000 + skill * 5000;
	if( 100 >= skill ) gold += gold/2;
	me->add("balance", gold);
	me->set("sld_given", (int)me->query("age"));
	command("say 我教白龙使在朝廷中也是个有身份的人，资金当然不会匮乏！");
	command("say 我将这" + MONEY_D->money_str(gold) + "存入你的钱庄，你离岛后可要好好为我教办事啊！");
	return 1;
}
