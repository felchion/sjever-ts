// xln.c 小龙女
// By River@SJ 99.05
// 增加 乐音绝技Quest By River@SJ 9.25.2001

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

string ask_suo(); 
string ask_zhen();
string ask_shoutao();
string ask_yueyin();
int pfm_yufeng();
int pfm_hebi();
int ask_yrw();

void create()
{
	object ob;
	set_name("小龙女", ({ "xiao longnv", "xiao", "longnv" }));
	set("title",HIW"古墓侠侣"NOR);
	set("nickname",HIC"神雕侠侣"NOR);
	set("long","盈盈而站着一位秀美绝俗的女子，肌肤间少了一层血色，显得苍白异常。\n"+
		"披著一袭轻纱般的白衣，犹似身在烟中雾里。\n");
	set("gender", "女性");
	set("age", 39);
	set("attitude", "friendly");
	set("no_bark",1);

	set("per", 48);
	set("str", 24);
	set("dex", 40);
	set("con", 25);
	set("int", 40);

	set("gmsanwu", 1);
	set("yueyin", 1);
	set("double_attack", 1);
	set_temp("double_attack", 1);
	set("shen", 10000);

	set("combat_exp", 14000000);
	set("max_qi", 19500);
	set("max_jing", 17600);
	set("eff_jingli", 6000);
	set("max_neili", 20000);
	set("neili", 40000);
	set("jiali", 100);
	set("unique", 1);

	set_skill("parry", 460);
	set_skill("force", 460);
	set_skill("dodge", 450);
	set_skill("sword", 480);
	set_skill("hand", 480);
	set_skill("tianluo-diwang",470);
	set_skill("throwing", 450);
	set_skill("whip", 480);
	set_skill("meinu-quanfa", 470);
	set_skill("cuff", 480);
	set_skill("yunu-shenfa", 470);
	set_skill("xuantie-jianfa", 440);
	set_skill("quanzhen-jianfa",440);
	set_skill("yinsuo-jinling", 470);
	set_skill("yunu-jianfa", 460);
	set_skill("quanzhen-jianfa", 480);
	set_skill("yunu-xinjing", 470);
	set_skill("literate", 500);

	map_skill("force", "yunu-xinjing");
	map_skill("dodge", "yunu-shenfa");
	map_skill("cuff", "meinu-quanfa");
	map_skill("sword", "yunu-jianfa");
	map_skill("whip", "yinsuo-jinling");
	map_skill("parry","yinsuo-jinling");
	prepare_skill("cuff", "meinu-quanfa");

	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: exert_function, "xinjing" :),
		(: perform_action, "whip.yueyin" :),
		(: perform_action, "whip.sanwu" :),
		(: pfm_yufeng :),
	}));

	create_family("古墓派", 3, "女主人");

	set("inquiry", ([
		"杨过" : "过儿是我夫君，我知道他很爱我，可一直呆在这真怕他会闷。",
		"钥匙" : "嗯....这我可记不清楚，好象是放在古墓里面了吧。",
		"金铃索" : (: ask_suo :),
		"玉蜂针" : (: ask_zhen :),
		"手套" : (: ask_shoutao :),
		"白金手套" : (: ask_shoutao :),
		"乐音" : (: ask_yueyin :),
		"乐音绝技" : (: ask_yueyin :),
		"yueyin" : (: ask_yueyin :),
	  "美容养颜" : (: ask_yrw :),
	  "beautiful" : (: ask_yrw :),
	]));

	set_temp("apply/damage", 80);
	set_temp("apply/dodge", 100);
	set_temp("apply/attack", 100);
	set_temp("apply/armor", 80);
        
	setup();
	if (clonep()){
		ob = unew("/d/gumu/obj/shoutao");
		if (ob) {
			ob->move(this_object());
			ob->wear();        
		}
	}
	carry_object(BINGQI_D("changbian"))->wield();
	carry_object(BINGQI_D("changjian"));
	carry_object("/d/gumu/obj/yfzhen");
	carry_object("/d/gumu/obj/buxue1")->wear();
	carry_object("/d/gumu/obj/qun1")->wear(); 
}

void reset()
{
	object ob;

	if (clonep() && (ob = unew("/d/gumu/obj/shoutao"))) {
		ob->move(this_object());
		command("wear baijin shoutao");
	}
}

int ask_yrw()
{
	object ob, me = this_player();
	mapping fam = me->query("family");

	if (present("yangrong wan", me))	{
		command("say 你不是已经有了么？");
		return 1;
	}
	
	if (!fam || fam["family_name"] != "古墓派") {
		command("say 你不是古墓弟子，这在瞎问什么？\n");
	  return 1;
  }
	if(me->query("per")>24) {
		command("say 你天生丽质何须用药？\n");
	  return 1;
  }
		
	if(me->query("age")<16) {
    command("say 你年龄太小，无法承受人参养容丸的药性。\n");
	  return 1;
  }
   
  if(me->query("max_neili")<800 || me->query("combat_exp")<100000){
    command("say 你修为太低，无法承受人参养容丸的药性。\n");
	  return 1;
  }
  

	ob = new(MEDICINE_D("yrw"));
	if(ob){
		ob->move(me);
		command("say 这有一颗人参养容丸，可以美容养颜，你就拿去吧!");
		message_vision("$N递给$n一颗"+HIW+"人参养容丸"+NOR+"。\n", this_object(), me);
		return 1;
	}
	else
	{
		command("say 药已经用完了，新的还没练出来呢!");
		return 1;
	}
}

void attempt_apprentice(object ob)
{
	mapping fam = ob->query("family");
	if (!fam || fam["family_name"] != "古墓派"){
		command("say "+RANK_D->query_respect(ob)+"与本派素无来往，不知此话从何谈起？");
		return;
	}
	if( ob->query("gender") != "女性") {
		command("wanfu " + ob->query("id"));
		command("say 我可只收女徒，你去找我夫君试试吧。");
		return;
	}
	if((int)ob->query_skill("yunu-xinjing", 1) < 120) {
		command("say 我古墓武功内功是基础。"); 
		command("say " + RANK_D->query_respect(ob)+"是否还应该先在内功上多下点功夫？");
		return;
	}
	if((int)ob->query_skill("sword", 1) < 100) {
		command("say 你剑法这么差，我教的你不一定学得会。"); 
		command("say "+RANK_D->query_respect(ob)+"是否还应该在剑法上多下点功夫？");
		return;
	}
	if((int)ob->query_int() < 22) {
		command("say 我的武功都要极高的悟性才能学会。");
		command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
		command("say 这位"+RANK_D->query_respect(ob)+"的悟性还大有潜力可挖，还是请回吧。");
		return;
	}
	if((int)ob->query("shen") < 10000){
		command("say 我古墓弟子行走江湖中，一向行侠仗义。");
		command("say 这位" + RANK_D->query_respect(ob)+"正气不够，去做几件侠义的事后再来吧？");
		return;
	}
	command("say 嗯，看你还是个学武的料，我就收下你吧！");
	command("recruit " + ob->query("id"));
	ob->set("title",HIW"古墓派神雕侠侣传人"NOR);
}

string ask_suo()
{
	object ob = this_player();
	mapping fam = ob->query("family");
	if (!fam || fam["family_name"] != "古墓派" )
		return RANK_D->query_respect(ob)+"，我已经多年未用金铃索了，也不知道放在何处了。";
	if (ob->query_skill("yinsuo-jinling", 1) < 100)
		return "这位"+ RANK_D->query_respect(ob) +"，金铃索灵活异常，你是否应该把武功练高些？";
	ob->set_temp("marks/银索", 1);
	command("nod " + ob->query("id"));
	return "不错，金铃索就放在此屋中，你自个找找吧。";       
}

int pfm_yufeng()
{
	object me,weapon;
	me = this_object();
	weapon = present("changbian",me);
	if( me->is_busy()) return 0;
	if( me->query_temp("hebi")) return 0;
	if( objectp(weapon)){
		set("jiali", 1);
		command("wield bian");
		command("perform yufeng");
		if(random(10) > 5){
			set("jiali", 50);
			command("unwield bian");
		}
		return 1;
	}
	else {
		if(present("changbian", environment(me)))
		command("get bian");
		else new(BINGQI_D("whip"))->move(me);
		command("wield bian");
		set("jiali", 1);
		command("wield bian");
		command("perform yufeng");
		if(random(10) > 5){
			set("jiali", 50);
			command("unwield bian");
		}
		return 1;
	}
	return 1;
}

string ask_zhen()
{
	object me = this_player();
	object *list;
	int i, j=0;

	if (me->query("family/family_name") != "古墓派")
		return "你不是我古墓派传人，你要这玉蜂针何用？";

	if( me->query("family/generation") > 4 )
		return "你现在还用不到玉蜂针。";

	if (present("yufeng zhen",me))
		return "你身上还有玉蜂针，怎么还来要？";

	if ( me->query_temp("yfzhen"))
		return "你不是刚要过了玉蜂针了么？不要这么贪心啊。";

	list = filter_array(objects(), (: $1->query("id") == "yufeng zhen":));

	i =  sizeof(list);
	while (i--)
		j += list[i]->query_amount();

	if ( j > 61)
		return "哎哟，玉蜂针已经发完了，你等会来要吧。";

	new("/d/gumu/obj/yfzhen")->move(this_object());
	command("give 10 yufeng zhen to " + me->query("id"));
	me->set_temp("yfzhen", 1);
	return "这些玉蜂针你先拿去用吧！";
}

string ask_shoutao()
{
	object me = this_player();

	if (me->query("family/master_name")!="小龙女")
		return ("这手套是我古墓派的宝物，你并非我的传人，不能给你！");

	if (present("baijin shoutao",this_object())) {
		command("give baijin shoutao to " + me->query("id"));
		return("这手套就给你了，你可要妥善保管好！");
	}
	else return ("你来晚了，手套已经让我送与他人了。");
}

string ask_yueyin()
{
	object me = this_player();
	mapping fam = me->query("family");
	object *dest;
	int i, j = 0;

	if (!fam || fam["family_name"] != "古墓派")
		return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";

	if(me->query_skill("yunu-xinjing", 1) < 150 )
		return "你的玉女心经等级不够，无法运力自如，练不了乐音! ";

	if(me->query_skill("yinsuo-jinling", 1) < 150 )
		return "你的银索金玲等级不够，还是先练练招式再说吧。 ";

	if(me->query("yueyin")) {
		command("? "+me->query("id"));
		return RANK_D->query_respect(me)+"不是已经掌握了乐音绝技了么？";
	}

	if ( me->query_temp("yueyin/ask"))
		return "你还不快去修炼？";

	dest = users();
	i = sizeof(dest);
	while(i--) {
		if (userp(dest[i]) && !wizardp(dest[i]) && dest[i]->query_temp("yueyin/ask"))
		j++;
	}
	if(j >= 1)
		return "嗯，我现在比较忙，你等会来打听吧。";

	command("say 嗯，乐音绝技就是将乐理融入武技中，银索金铃正可使用此技巧，不过可需要多多练习。");
	me->set_temp("yueyin/ask", 1);
	return "出大屋有片花丛，比较适合修炼，你专心去修炼吧，可不要忘记带一根鞭子哦。";
}

void kill_ob(object me)
{
	command("heng");
	command("throw yufeng zhen at " + me->query("id"));
	::kill_ob(me);
}
