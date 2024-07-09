// yaoshi.c
// Modified by Numa 19990917
// Modified by Darken@SJ for taohua quest
// pk river wenmang
// Modified by mxzhao 2004/05/12 for shen restrict to exp/10

#include <ansi.h>
#include "nadir.c"
#include <combat.h>

inherit NPC;
inherit F_MASTER;

int ask_leave();
int ask_jia();
int ask_marry();
int ask_xiao();
int ask_test1();
int ask_test2();
int ask_test3();
int ask_yuxiao();
int ask_dongxiao();
int ask_shenjian();
int ask_kuangfeng();
int ask_lingxi();
int ask_huayu();
int ask_nvxu();
int ask_jiuyin();
int ask_bagua();
int ask_teaching();
int ask_quest();
int perexert();
void cont_per(object me, object weapon);
int ask_reward();
#include "nvxu.h"
#include "perform.h"

void create()
{
	set_name("黄药师", ({ "huang yaoshi", "huang", "yaoshi" }) );
	set("nickname", "桃花岛主");
	set("long",
		"他就是桃花岛的岛主，江湖上人人闻名变色的东邪黄药师。\n"
		"他身穿一件青色长袍，腰带上斜插着一只玉箫。\n"
		"他形相清癯，丰姿隽爽，萧疏轩举，湛然若神。\n");
	set("gender", "男性" );
	set("rank_info/rude", "黄老邪");
	set("age", 50);
	set("attitude", "peaceful");
	set("apprentice",1);
	set("str", 30);
	set("int", 50);
	set("con", 35);
	set("dex", 40);
	set("per", 30);
	set("unique",1);
	
	set("max_qi", 50000);
	set("max_jing", 17500);
	set("eff_jingli", 18500);
	set("neili", 46000);
	set("max_neili", 23000);
	set("jiali", 150);
	set("combat_exp", 15000000);
	set("score", 20000);
	
	set_temp("apply/damage", 100);
	set_temp("apply/dodge", 100);
	set_temp("apply/parry", 50);
	set_temp("apply/defence", 100);
	set_temp("apply/armor", 90);
	
	set("thd/perform", 7);
	set("chat_chance_combat", 100);
	set("chat_msg_combat", ({
		(: perexert :),
	}));
	
	set_skill("force", 500);
	set_skill("bihai-chaosheng", 530);
	set_skill("dodge", 500);
	set_skill("suibo-zhuliu", 520);
	set_skill("parry", 500);
	set_skill("strike", 500);
	set_skill("luoying-zhang", 500);
	set_skill("leg", 500);
	set_skill("xuanfeng-tui", 500);
	set_skill("hand", 500);
	set_skill("lanhua-shou", 520);
	set_skill("finger", 500);
	set_skill("taoism", 300);
	set_skill("throwing", 500);
	set_skill("tanzhi-shentong", 530);
	set_skill("sword", 500);
	set_skill("yuxiao-jian", 520);
	set_skill("literate", 900);
	set_skill("qimen-bagua", 500);
	
	map_skill("force", "bihai-chaosheng");
	map_skill("dodge", "suibo-zhuliu");
	map_skill("strike", "luoying-zhang");
	map_skill("hand", "lanhua-shou");
	map_skill("finger", "tanzhi-shentong");
	map_skill("throwing", "tanzhi-shentong");
	map_skill("leg", "xuanfeng-tui");
	map_skill("parry", "yuxiao-jian");
	map_skill("sword", "yuxiao-jian");
	prepare_skill("strike", "luoying-zhang");
	prepare_skill("leg", "xuanfeng-tui");
	
	set("rank_info/respect", "桃花岛主");
	set("shen_type",0);
	create_family("桃花岛", 1, "岛主");
	set("class", "fighter");
	
	set("inquiry", ([
		"name": "难道天底下就没人知道我东邪黄药师的名字了吗？",
		"rumors": "听说全真教那帮牛鼻子在到处找周伯通，哼！",
		"here": "这里就是桃花岛了。寒雨那个小坏蛋在这里加了无数的ｑｕｅｓｔ，看你能不能找全了。",
		"东邪": "那都是江湖上那班满嘴仁义道德的家伙给我起的绰号。",
		"西毒": "以前的西毒是欧阳锋那只臭蛤蟆，除了会耍两手蛇外，没什么真功夫。",
		"欧阳锋": "那只臭蛤蟆，除了会耍两手蛇外，没什么真功夫。",
		"南帝": "以前的南帝是大理的王爷段智兴，现在出家做和尚，改叫一灯啦。",
		"一灯": "以前是大理的王爷段智兴，现在出家做和尚，改叫一灯啦。",
		"段智兴": "以前是大理的王爷，现在出家做和尚，改叫一灯啦。",
		"北丐": "以前的北丐是洪七公那个老叫化，几手降什么十八掌倒还使得。",
		"洪七公": "那个老叫化是丐帮帮主，几手降什么十八掌倒还使得。",
		"中神通": "以前的中神通是全真教的臭道士王重阳，要不是老夫一时失手，武林第一的称号也不会被他抢走。",
		"王重阳": "是全真教的臭道士，要不是老夫一时失手，武林第一的称号也不会被他抢走。",
		"周伯通": (: ask_jiuyin :),
		"老顽童": "这个大傻瓜，被我骗了都不知道，你如果想找他，嘿嘿嘿....",
		"黄蓉": "唉，从小就对她娇生惯养，现在她调皮任性，我也没办法了。",
		"蓉儿": "唉，从小就对她娇生惯养，现在她调皮任性，我也没办法了。",
		"郭靖": "别提我这个傻女婿了。",
		"女婿": (: ask_nvxu :),
		"寒雨": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
		"jpei": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
		"桃花岛": "这里就是桃花岛了。寒雨那个小坏蛋在这里加了无数的ｑｕｅｓｔ，看你能不能找全了。",
		"乘风": "是我的四弟子，现在住在归云庄。",
		"桃花影落飞神剑" : (: ask_shenjian :),
		"碧海潮生按玉箫" : (: ask_yuxiao :),
		"狂风" : (: ask_kuangfeng :),
		"狂风绝技" : (: ask_kuangfeng :),
		"灵犀指" : (: ask_lingxi :),
		"灵犀一指" : (: ask_lingxi :),
		"漫天花雨" : (: ask_huayu :),
		"陆乘风": "是我的四弟子，现在住在归云庄。",
		"黄药师": "就是老夫我。怎么，不服吗？咱俩练练？",
		"九宫": "九宫之义，法以灵龟。二四为肩，六八为足；左三右七，戴九履一；五居中间。",
		"八卦": "乾三连，坤六段；离中虚，坎中满；震仰盂，艮覆碗；兑上缺，巽下断。",
		"五行": "金生水，水生木，木生火，火生土，土生金。金克木，木克土，土克水，水克火，火克金。",
		"易经": "是一本讲解伏羲六十四卦的奇书，一般人难得看懂。",
		"周易": "是一本讲解伏羲六十四卦的奇书，一般人难得看懂。",
		"九阴真经": "你打听这个干什么？！",
		"和尚": "老夫我对和尚就是瞧不惯！",
		"离开": (: ask_leave :),
		"离岛": (: ask_leave :),
		"回去": (: ask_leave :),
		"leave": (: ask_leave :),
		"求婚": (: ask_marry :),
		"第一关": (: ask_test1 :),
		"test1": (: ask_test1 :),
		"第二关": (: ask_test2 :),
		"软猬甲": (: ask_jia :),
		"test2": (: ask_test2 :),
		"第三关": (: ask_test3 :),
		"test3": (: ask_test3 :),
		// add for jiugong tu
		"奇门八卦": (: ask_bagua :),
		// add for taohua quest
		"梅超风": (: ask_quest :),
		"陈玄风": (: ask_quest :),
		"洞箫" : (: ask_dongxiao :),
		"碧玉箫" : (: ask_xiao :),
		"报酬": (: ask_reward :),
		"教诲": (: ask_teaching :),
        ]) );
	
	setup();
	
	if(clonep())
	{
		object ob = unew(ARMOR_D("rwj"));
		if(!ob) ob=new("/d/thd/obj/grn-cloth");
		ob->move(this_object());
		ob->wear();
	}
	
	if (clonep(this_object()))
	{
		object ob;
		if (ob = carry_object(BINGQI_D("sword/dongxiao")))
			ob->wield();
	}
	
}

void init()
{
	object ob = this_player();
	
	add_action("do_answer","answer");
	add_action("do_answer","huida");
	add_action("do_kill","kill");
	::init();
	
	if ( file_name(environment(this_object())) != this_object()->query("startroom")) return;
	
	if (ob->query("family/family_name") == "桃花岛") 
	{
		if ((int)ob->query("shen") < -ob->query("combat_exp")/2
			&& (int)ob->query("shen") < -200000) 
		{
			message_vision("黄药师对$N喝道：你做恶多端，有辱我名，还敢来见我？！\n", ob);
			message_vision("黄药师对$N喝道：从今天起，你再也不是桃花岛的弟子了！\n", ob);
			command("expell " + ob->query("id"));
			ob->set("title", "桃花岛弃徒");
		}
		else if ((int)ob->query("shen") > ob->query("combat_exp")/2
			&& (int)ob->query("shen") > 200000) 
		{
			message_vision("黄药师对$N喝道：你是大侠，我早就说过，我最恨伪君子。想做我徒弟，就别沽名吊誉。\n", ob);
			message_vision("黄药师对$N喝道：从今天起，你再也不是桃花岛的弟子了！\n", ob);
			command("expell " + ob->query("id"));
			ob->set("title", "桃花岛弃徒");
		}
	}
}

void reset()
{
	object ob, jia;
	
	if (clonep() && (ob = unew(BINGQI_D("sword/dongxiao")))) {
		ob->move(this_object());
		command("wield dongxiao");
	}
	
	if (clonep() && (jia = unew(ARMOR_D("rwj")))) {
		jia->move(this_object());
		command("wear ruanwei jia");
	}
}

int ask_jiuyin()
{
	object me;
	me = this_player();

	if (!me->query("quest/jiuyin1/start")) {
		command("say 你想知道什么？");
		command("hmm");
		return 1;
	}

	if (me->query_temp("jyquest") >0 ) {
		command("say 不是告诉你了么？还想问什么？");
		command("kick "+ me->query("id"));
		return 1;
	}


	command("say 这家伙有九阴真经的上卷，就是死也不肯交出来。");
	command("sneer");
	command("say 我将他困在岛上，看他能撑到几时！");
	me->set_temp("jyquest",1);

	return 1;
}

int ask_xiao()
{
	object me;
	me = this_player();
	
	if (present("biyu xiao", me)){
		command("say 你不是有了么？");
		command("crazy");
		return 1;
	}
	if (me->query("job_time/桃花岛") < 400) {
		command("say 你为桃花做了什么了？");
		command("sneer");
		return 1;
	}
	command("say 好吧，你就自己去找找吧。");
	message_vision(HIY"$N向黄药师行了个礼，走进了一个小房间。\n"NOR, me);
	me->move(__THD__"room");
	return 1;
}

int ask_dongxiao()
{
	object me, xiao;
	me = this_player();
	
	if (present("dongxiao", me)){
		command("say 你不是有了么？");
		command("crazy");
		return 1;
	}
	if (me->query("job_time/桃花岛") < 1000) {
		command("say 你为桃花做了什么了？");
		command("sneer");
		return 1;
	}
	
	xiao = present("dongxiao",this_object());
	
	if (xiao) {
		command("say 好吧，你就拿去吧。");
		xiao->move(me);
		return 1;
	}
	else {
		command("say 你来晚了，已经被人拿走了。");
		return 1;
	}
	return 1;
}

int ask_jia()
{
	object me = this_player();
	object jia;
	if ( me->query("job_time/桃花岛") < 600
		|| me->query("combat_exp") <2000000
		|| me->query("family/family_name") != "桃花岛") {
		command("say 你是什么东西，也敢要我的软猬甲?");
		return 1;
	}
	
	jia = present("ruanwei jia",this_object());
	
	if (jia) {
		command("say 好吧，你就拿去吧。");
		jia->move(me);
		return 1;
	}
	else {
		command("say 你来晚了，已经被人拿走了。");
		return 1;
	}
	return 1;
}

int ask_teaching()
{
	object me = this_player();
	mapping fam;
	
	object ob = this_player();
	if (!(fam = ob->query("family"))
		|| fam["family_name"] != "桃花岛")
		return notify_fail("你不是本派弟子，在这瞎打听什么！\n");
	
	if (me->query("shen") > 10000) {
		command("say 哼，我最讨厌的就是自命大侠之人！");
		command("say 世俗礼节岂是为我辈而设？");
		message_vision(HIY"$N被黄药师一顿教训，当下把想做大侠念头收起。\n"NOR, me);
		me->add_busy(5);
		me->set("shen", 10000);
		return 1;
	}
	if (me->query("shen") < -10000) {
		command("say 好人不可做，更不可做恶人！");
		command("say 想跟这些哑仆一样么？");
		message_vision(HIR"$N被黄药师一顿教训，马上没了恶念。\n"NOR, me);
		me->add_busy(5);
		me->set("shen",-10000);
		return 1;
	}
	return 1;
}

void attempt_apprentice(object ob)
{
	string old_name, new_name;
	
	if (ob->query("gender") != "男性" && ob->query("gender") != "女性") {
		command("say 你不男不女的算什么东西？快滚！");
		return;
	}
	if (ob->query("shen") <= -200000) {
		command("say 我生平最讨厌两种人，你们这种无恶不作的邪魔外道就是其一，哼！");
		return;
	}
	if (ob->query("shen") >= 200000) {
		command("say 我生平最讨厌两种人，你们这种自命大侠的沽名钓誉之徒就是其一，哼！");
		return;
	}
	if (ob->query_int() < 28) {
		command("say 看你这副模样，比我那傻女婿还笨，还想拜我为师？");
		return;
	}
	if (!ob->query_skill("literate")) {
		command("say 你这个文盲也想来学功夫？先去学点文化吧。");
		return;
	}
	if (ob->query("class") == "bonze") {
		if (ob->query("family/family_name") == "少林派")
			command("say 你们自命天下武功出少林，我这里的庙小，可不敢收你。");
		else
			command("say 我东邪最讨厌和尚，看到光头就有气，所以连尼姑、喇嘛一起都不收。");
		return;
	}
	if (ob->query("family/family_name") == "桃花岛") {
		if (ob->query("family/generation") != 3 )
		{
			command("say 我桃花岛武功博大精深，" + RANK_D->query_respect(ob) + "还是先把基础打扎实吧！");
			return;
		}
		if ((int)ob->query_skill("bihai-chaosheng", 1) < 130)
		{
			command("say 我桃花岛一派武功脱胎于道教，很重视内功心法。");
			command("say " + RANK_D->query_respect(ob) +
				"是否还应该在碧海潮生功上多下点功夫？");
			return;
		}
		if (ob->query_int() < 32)
		{
			command("say 看你这副模样，比我那傻女婿还笨，还想拜我为师？");
			return;
		}
		command("chat 哈哈哈哈！！！！");
		command("chat 想不到我东邪误逐弟子之后，还能得此良才美质，真是可喜可贺。");
		command("recruit " + ob->query("id"));
		old_name = ob->query("name");
		new_name = old_name[0..1] + "风";
		ob->set("name", new_name);
		say(old_name + "拜入黄药师门下，位列二代弟子“风”字辈，起名为" + new_name + "。\n");
		return;
	}
	if (ob->query("expell/family_name") == "桃花岛"
		&& ob->query("expell/master_name") =="黄药师") {
		command("recruit " + ob->query("id"));
		old_name = ob->query("name");
		new_name = old_name[0..1] + "风";
		ob->set("name", new_name);
		say(old_name + "拜入黄药师门下，位列二代弟子“风”字辈，起名为" + new_name + "。\n");
		return;
	}
}

int recognize_apprentice(object ob)
{
	object nvxu;
	
	if (ob->query("gender") == "男性") {
		if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
			nvxu = load_object("/clone/npc/huang-son-in-law");
		if (nvxu->query("winner") == ob->query("id") && nvxu->query("death_count") == ob->query("death_count"))
			return 1;
	}
	if (ob->query("thd/quest1")>3)
		return 1;
	return notify_fail("兰花拂穴手乃黄岛主家传绝学，不传外姓。\n");
}

int prevent_learn(object ob, string skill)

{
	if (recognize_apprentice(ob)) {
		if (skill == "lanhua-shou")
			return 0;
		if (ob->is_apprentice_of(this_object()))
			return 0;
		notify_fail("你只能向黄岛主请教兰花拂穴手这门家传功夫。\n");
		return 1;
	}
	return 0;
}

void kill_ob(object me)
{
	if (!is_killing(me->query("id")))
		command("chat* crazy " + me->query("id"));
	::kill_ob(me);
}

void unconcious()
{
	command("chat* 长叹一声：我纵横江湖几十载，人人听到东邪都闻名丧胆，没想到也有今天！");
	::unconcious();
}


int accept_object(object who, object ob)
{
	if (ob->id("art"))
	{
		message_vision(HIY"黄药师拿起"+ob->query("name")+HIY"仔细鉴赏着。\n"NOR, who);
		command("ah");
		command("say 这真是" + ob->query("name") + "！");
		command("thank " +who->query("id"));
		command("consider");
		command("say 我就给你一些回报吧。");
		who->set("potential",who->query("max_pot")*5);
		message_vision(HIY"黄药师向$N指导了一番，$N觉得自己有发挥不完潜能！\n"NOR, who);
		return 1;
	}
	if ((ob->id("skin"))&&(who->query_temp("thd/onquest")>7))
	{
		message_vision(HIY"黄药师拿起人皮细细查看。\n"NOR, who);
		command("ah");
		command("say 这上面刻的正是九阴真经！");
		command("haha");
		command("say 哼，臭道士们，我一定能想出九阴真经上卷的心法！");
		command("say 你能帮我找回这经书，资质也还算不错。");
		command("say 我的兰花拂穴手虽然不传外性，就破破例吧。");
		who->set("thd/finish",1);
		who->delete_temp("thd/onquest");
		destruct(ob);
		return 1;
	}
	
	if (ob->id("qulingfeng tiepai"))
	{
		command("sigh");
		say("黄药师手握铁牌，泪水汩汩而下，嘴里喃喃说道：“灵风，我负你太多！”\n");
		if (!present("zhangfa tujie", who)) {
			if (query_temp("zhangfa_give") >= 3) {
				tell_object(who, "黄药师对你说道：“可惜我的《落英神剑掌图解》都交给了别人，不然倒可以请" + RANK_D->query_respect(who) + "把这本掌法交与灵风之女，也算了却我一个心愿。”\n");
			}
			else {
				tell_object(who, "黄药师对你说道：“听说他还留有一个女儿，就相烦" + RANK_D->query_respect(who) + "把这本掌法交给她，也算了却我一个心愿。\n");
				tell_object(who, "黄药师给你一本《落英神剑掌图解》。\n");
				new_ob("../obj/zhangfa.c")->move(who);
				add_temp("zhangfa_give", 1);
			}
		}
		else
		{
			tell_object(who, "黄药师对你说道：“所幸你身上带有我的《落英神剑掌图解》，就相烦" + RANK_D->query_respect(who) + "把这本掌法交与灵风之女，也算了却我一个心愿。”\n");
		}
		return 1;
	}
	
	// 拜三代
	if (ob->id("recommend letter"))
	{
		if (who->query("thd/receive")==1)
		{
			command("angry " + who->query("id"));
			command("say 我既已叫你跟乘风学功夫，" + RANK_D->query_respect(ob) + "还来这里做甚什么？");
			return 0;
		}
		if (ob->query("owner") == who->query("id"))
			message_vision("黄药师点了点头，对$N说道：“是乘风的亲笔信，你过来，让我瞧瞧。”\n", who);
		else
		{
			message_vision("黄药师双眉一竖，对$N喝道：“乘风推荐的不是你！你如何敢来欺骗老夫？快滚！”\n", who);
			return 0;
		}
		command("nod" + who->query("id"));
		command("say 嗯，既然乘风看中了你，你就跟着他学功夫吧。");
		who->set("thd/receive", 1);
		return 1;
	}
}

int ask_leave()
{
	object nvxu;
	object ob = this_player();
	
	if (ob->query("family/master_name") == "黄药师") {
		message_vision("$N对着黄药师躬身说：“弟子要离岛了，师父有什么教诲？”\n", ob);
		message_vision("黄药师对$N嘱咐道：“你此去中原，要一路小心，莫要坏了我东邪的威名。”\n", ob);
		message_vision("$N点了点头，道：“弟子谨记在心！”\n", ob);
		say("黄药师一招手，喊过一个艄公来：载" + ob->name() + "回陆地。\n");
		write("你对着师父挥了挥手，跳上小船，艄公将船划向陆地。\n");
		write("海上正是顺风，船借风势，数日内便到达了。\n");
		tell_room(__THD__"haibin",
			"海上远处忽然现出一艘小船，借着风势，不一会就到达了，" + ob->name() + "从船上一纵身跳了出来。\n", ({}));
		ob->move(__THD__"haibin");
		return 1;
	}
	if (ob->query("family/family_name") == "桃花岛") {
		if (ob->query("gender") == "男性")
			message_vision("$N对着黄药师躬身说：“徒孙要离岛了，太师父有什么教诲？”\n", ob);
		else
			message_vision("$N对着黄药师躬身说：“徒孙女要离岛了，太师父有什么教诲？”\n", ob);
		message_vision("黄药师对$N嘱咐道：“你此去中原，要一路小心，莫要坏了我东邪的威名。”\n", ob);
		message_vision("$N点了点头，道：“弟子谨记在心！”\n", ob);
		say("黄药师一招手，喊过一个艄公来：载" + ob->name() + "回陆地。\n");
		write("你对着太师父挥了挥手，跳上小船，艄公将船划向陆地。\n");
		write("海上正是顺风，船借风势，数日内便到达了。\n");
		tell_room(__THD__"haibin",
			"海上远处忽然现出一艘小船，借着风势，不一会就到达了，" + ob->name() + "从船上一纵身跳了出来。\n", ({}));
		ob->move(__THD__"haibin");
		return 1;
	}
	if (ob->query("gender") == "男性") {
		if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
			nvxu = load_object("/clone/npc/huang-son-in-law");
		if (nvxu->query("winner") == ob->query("id")
			&& nvxu->query("death_count") == ob->query("death_count")) {
			/*      if (ob->query("age") * 12 + ob->query("month") - ob->query("thd/marry_age") < 12) {*/
			message_vision("$N对着黄药师躬身说：“孩儿要离岛了，岳父有什么教诲？”\n", ob);
			message_vision("黄药师对$N嘱咐道：“你此去中原，要一路小心，莫要坏了我东邪的威名。”\n", ob);
			message_vision("$N点了点头，道：“孩儿谨记在心！”\n", ob);
			say("黄药师一招手，喊过一个艄公来：载" + ob->name() + "回陆地。\n");
			write("你对着黄药师挥了挥手，跳上小船，艄公将船划向陆地。\n");
			write("海上正是顺风，船借风势，数日内便到达了。\n");
			tell_room(__THD__"haibin",
				"海上远处忽然现出一艘小船，借着风势，不一会就到达了，" + ob->name() + "从船上一纵身跳了出来。\n", ({}));
			ob->move(__THD__"haibin");
			return 1;
		}
	}
	if (__NIU__"boat.c"->query_temp("busy"))
		command("say 小船已经送别的客人了，" + RANK_D->query_respect(ob) + "再等一会儿吧。\n");
	else {
		say("黄药师一招手，喊过一个艄公来：载这位" + RANK_D->query_respect(ob) + "回陆地。\n");
		ob->set_temp("thd_target", "bay");
		ob->move(__THD__"haibin");
	}
	return 1;
}

int do_kill(string arg)
{
	object ob, nvxu;
	object me = this_player();
	
	if (!arg) return 0;
	ob = present(arg, environment(me));
	if (ob == this_object() || ob == me) return 0;
	if (!ob || !ob->is_character()) return 0;
	
	if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
		nvxu = load_object("/clone/npc/huang-son-in-law");
	if (nvxu->query("winner") == ob->query("id") && nvxu->query("death_count") == ob->query("death_count")) {
		if (me->query("family/family_name") == "桃花岛") {
			message_vision("黄药师向$N喝道：小奴才，要造反吗？连我的女婿都敢杀？！\n", me);
		}
		else {
			message_vision("黄药师向$N喝道：大胆狂徒，竟敢来桃花岛撒野？！\n", me);
			message_vision("黄药师对$N说道：女婿退后，让我来收拾这个" + RANK_D->query_rude(me) + "！\n", ob);
			kill_ob(me);
		}
		me->set("vendetta/桃花岛", 1);
		return 1;
	}
	if (ob->query("family/family_name") == "桃花岛") {
		if (me->query("family/family_name") == "桃花岛") {
			message_vision("黄药师向$N喝道：小奴才，要造反吗？连自己的同门都杀？！\n", me);
		}
		else {
			message_vision("黄药师向$N喝道：大胆狂徒，竟敢来桃花岛撒野？！\n", me);
			message_vision("黄药师对$N说道：你先让开，让我来收拾这个" + RANK_D->query_rude(me) + "！\n", ob);
			kill_ob(me);
		}
		me->set("vendetta/桃花岛", 1);
		return 1;
	}
	return 0;
}


int ask_bagua()
{
	object ob = this_player();
	mapping fam;
	
	if (!(fam = ob->query("family"))
		|| fam["family_name"] != "桃花岛")
		return notify_fail(RANK_D->query_respect(ob) + "不是本派弟子，在这瞎打听什么！\n");
	
	command( "say 我记得有一本讲解伏羲六十四卦的奇书，或许能对" + RANK_D->query_respect(ob) + "有所帮助。\n");
	command( "say 我把它放在书房了，你自己去找找看吧。\n");
	ob->set_temp("thd/tupu",1);
	return 1;
}

int ask_quest()
{
	object ob = this_player();
	
	if (ob->query_temp("thd/onquest")>0)
		return notify_fail("不是告诉你了么？");
	
	if (ob->query("thd/finish")>0)
		return notify_fail("老夫该多谢你才是。");
	
	command( "heng");
	command( "say 这两个孽徒，居然偷了我的九阴真经跑了。");
	command( "say 你若是找到他们，顺手帮我将他们除去了吧。");
	command( "consider");
	command( "say 他们武功也算不错，找我女儿要点东西防身吧。");
	ob->set_temp("thd/onquest",1);
	return 1;
}

int ask_reward()
{
	object me = this_player();
	mapping fam;
	if (!(fam = me->query("family"))
		|| fam["family_name"] != "桃花岛")
		return notify_fail("你不是本派弟子，在这瞎打听什么！\n");
	
	if ((me->query("thd/finish")==1)&&(me->query("thd/quest1")<4))
	{
		message_vision(HIW"黄药师一怔，随即哈哈大笑起来。\n"NOR, me);
		command("say 好，好！我最恨装模做样的伪君子，你性情直爽，最和我意。\n");
		message_vision(HIY"黄药师对你详解临敌拆解之道，你的身法进步了！\n"NOR, me);
		me->set("thd/quest1",4);
		return 1;
	}
	return 1;
}
int perexert()
{
	object weapon, me;
	me = this_object();
	weapon = me->query_temp("weapon");
	if(me->is_busy()) return 0;
	call_out("cont_per", 2, me, weapon);
	return 1;
}

void cont_per(object me, object weapon)
{
	int i;
	if (!me->is_fighting()) return;
	if(!me->query_temp("thd/is_fast")) {
		command("exert maze");
		command("perform dodge.wuzhuan");
		return;
	} else {
		switch(random(5)) {
        case 0:
			if (!weapon) command("wield xiao");
			else command("perform qimen");
			break;
        case 1:
			if (!weapon) command("wield xiao");
			else command("perform feiying");
			break;
        case 2:
			if (weapon) weapon->unwield();
			me->prepare_skill("finger");
			me->prepare_skill("hand");
			me->prepare_skill("strike", "luoying-zhang");
			me->prepare_skill("leg","xuanfeng-tui");
			i = me->query_temp("thd/is_fast");
			me->delete_temp("thd/is_fast");
			command("perform kuangfeng");
			me->set_temp("thd/is_fast",i);
			if (weapon) weapon->wield();
			break;
        case 3:
			if (weapon) weapon->unwield();
			me->prepare_skill("leg");
			me->prepare_skill("strike");
			me->prepare_skill("finger","tanzhi-shentong");
			command("perfor huayu");
			break;
        case 4:
			if (weapon) weapon->unwield();
			me->prepare_skill("finger");
			me->prepare_skill("hand");
			me->prepare_skill("leg", "xuanfeng-tui");
			me->prepare_skill("strike","luoying-zhang");
			command("perform pikong");
			if (weapon) weapon->wield();
			break;
	}}
	call_out("cont_per", 2, me, weapon);
}
