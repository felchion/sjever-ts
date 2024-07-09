// fengling.c 风陵师太
// Lklv modify at 2001.10.18
// Add quest by Lane@SJ 2005.5.13 黑色星期五

#include <ansi.h>
inherit F_MASTER;
inherit NPC;

mixed em_jianjue();

void create()
{
	set_name("风陵师太", ({ "fengling shitai", "fengling", "shitai"}));
	set("long","她就是峨嵋派第二代掌门人风陵师太了。\n"
		"她面目慈祥，便如寻常老尼一般，浑不带一丝尘俗之气。\n"
	);

	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("class", "bonze");
	set("age", 80);
	set("shen", 20000);
	set("str", 26);
	set("int", 30);
	set("con", 29);
	set("dex", 30);
	set("per", 30);

	set("max_qi", 16700);
	set("max_jing", 16000);
	set("eff_jingli", 13500);
	set("neili", 45000);
	set("max_neili", 24000);
	set("jiali", 150);
	set("emei/jianjue", 1);
	set("combat_exp", 14000000); 

	set_skill("dacheng-fofa", 300);
	set_skill("linji-zhuang", 490);	// 增加 20 level
	set_skill("jieshou-jiushi", 480);
	set_skill("huifeng-jian", 480);
	set_skill("sixiang-zhang", 480);
	set_skill("yanxing-daofa", 480);
	set_skill("hand", 500);
	set_skill("literate", 520);
	set_skill("strike", 500);
	set_skill("sword", 500);
	set_skill("blade", 500);
	set_skill("parry", 500);
	set_skill("dodge", 500);
	set_skill("force", 500);	// 增加 20 level
	set_skill("anying-fuxiang", 480);

	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("blade", "yanxing-daofa");
	map_skill("parry", "jieshou-jiushi");
	map_skill("dodge", "anying-fuxiang");
	map_skill("strike", "sixiang-zhang");
	map_skill("hand", "jieshou-jiushi");
	prepare_skill("hand", "jieshou-jiushi");

	set("chat_chance_combat", 40);
	set("chat_msg_combat", ({
		(: perform_action, "sword.mie" :),
		(: perform_action, "sword.liaoyuan" :),
		(: perform_action, "hand.foguang" :)
	}));
	create_family("峨嵋派", 2, "前掌门");

	set("inquiry", ([
		"峨嵋剑诀" : (: em_jianjue :),
		"剑诀" : (: em_jianjue :),
	]));

	setup();
	set_temp("apply/armor", 60);
	set_temp("apply/damage", 40);
	set_temp("apply/attack", 50);
	set_temp("apply/dodge", 40);
	if(random(2)==1)
		carry_object(BINGQI_D("gangjian"))->wield();
	UPDATE_D->get_cloth(this_object());
}

void init()
{
	object me, ob;
	::init();
	ob = this_player();
	me = this_object();
	if( interactive(ob = this_player()) && !is_fighting() ) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
	add_action("do_qingjiao", "qingjiao");

}

void greeting(object me)
{
	int shen;
	if(!me) return;
	shen = me->query("shen");
	if(shen <= -200000 && me->query("family/family_name") == "峨嵋派"){
		command("chat* smash "+me->query("id"));
		command("say 你满眼瘴气，身带邪气，再和邪魔外道来往我开除你出峨嵋派！");
	}
	if(shen <0 && shen > -200000 && me->query("family/family_name") == "峨嵋派"){
		command("look "+me->query("id"));
		command("say 我看你满眼瘴气，身带邪气，最近是不是在外面做了什么坏事？");
	}
	else command("say 阿弥陀佛。");
}

int accept_object(object me, object ob)
{
	if( me->query("family/family_name") != "峨嵋派" ) return 0;

	if( ob->query("id") != "zhi huan" || !me->query_temp("marks/pass_yitian") ) return 0;

	if( !(me->query_temp("marks/get_ring")) ) {
		command("say 这枚指环是从何而来？哼，想骗老尼，没那么容易！");
		command("consider " + me->query("id"));
		command("say 拿命来吧！");
		kill_ob(me);
		return 1;
	} else {
		command("say 好吧，既然把掌门指环交给了我，你就有资格做老尼的衣钵传人了。\n");
		me->delete_temp("marks/pass_yitian");
		me->delete_temp("marks/get_ring");
		me->set_temp("bai_feng",1);
		remove_call_out("destroying");
		call_out("destroying", 1, me, ob);
		return 1;
	}
	return 0;
}

void attempt_apprentice(object ob)
{
	object me;
	mapping ob_fam, my_fam;
	string name, new_name;

	me = this_object();
	my_fam  = me->query("family");
	ob_fam = ob->query("family");
	name = ob->query("name");

        if (ob->query("family/family_name") != "峨嵋派" || !ob->query("family")){
		command("say " + RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
		return;
        }
        if ((string)ob->query("class")!="bonze" && ob->query("family/family_name") == "峨嵋派"){
                command("say " + RANK_D->query_respect(ob) + "是俗家弟子，不能拜在老尼门下学艺。");
                return;
        }
	if ((int)ob->query_skill("linji-zhuang", 1) < 150 ){
		command("say 老尼我不收练功不勤之人，你还是多练练你的临济十二庄吧。");
		return;
	}
        if (!ob->query_temp("bai_feng")){
		command("say " + RANK_D->query_respect(ob) + "，你没有拿到掌门指环，不能拜老尼为师。");
		return;
	}
	if (ob_fam["generation"] == (my_fam["generation"] + 1)){
		command("say 徒儿，你功夫练的如何了?\n");
		ob->delete_temp("bai_feng");
		return;
	}
	if ( ob_fam["generation"] == (my_fam["generation"] + 2) ){
		if ( ob->query_temp("bai_feng") ){
			ob->delete_temp("bai_feng");
			command("chat 老尼垂死之年，终得到一个衣钵传人，真是可喜可贺 !");
			name = ob->query("name");
			new_name = "灭" + name[1..1];
			ob->set("name", new_name);
			command("say 从今以后你的法名叫做" + new_name + "，恭喜你成为灭字辈长老之一 !");
			command("recruit " + ob->query("id"));
			return;
		}
		command("say 你没有找来指环，我不能把衣钵传你。\n");
		return;
	}
	command("say 你班辈不合，不可越级拜师。\n");
	return;
}

void destroying(object me, object obj)
{
	if (!obj) return;
	destruct(obj);
	return;
}

mixed em_jianjue()
{
	object me = this_player();
	object ob = this_object();
	int exp, time;

	if( me->query("family/family_name") != "峨嵋派" )
		return RANK_D->query_respect(me) + "与本派素无来往，不知此话从何谈起？";

	if( me->query_temp("marks/失败") ) {
		message_vision(CYN"$N面目慈祥，对着$n慢慢说道：“练武之道在于不断求新，你安心练功，终有一天能领悟到武学的最高境界！”\n"NOR,ob, me);
		return 1;
	}

	if( me->query("emei/jianjue") )
		return RANK_D->query_respect(me) + "，你的剑诀造诣已经不在贫尼之下了，又何故开此玩笑呢？";

	if( me->query_skill("huifeng-jian", 1) < 200 )
		return RANK_D->query_respect(me) + "，你的剑法造诣不够，还无法领悟我派剑法的精要所在！";

	if( me->query_skill("huifeng-jian", 1) > 400 )
		return RANK_D->query_respect(me) + "的峨嵋剑法精湛无比，应该不难自我领悟吧！";

	time = time() - me->query("em_jianjue/time");
	if( me->query("em_jianjue/time") && time < 86400 && !wizardp(me) )
		return "嗯？你不是刚来请教过我剑诀吗？还是再努力段时间吧！";

	exp = me->query("combat_exp") - me->query("em_jianjue/exp");
	if( me->query("em_jianjue/exp") && exp < 100000 && !wizardp(me) )
		return "嗯？你不是刚来请教过我剑诀吗？还是再努力段时间吧！";

	if( me->query_temp("marks/qingjiao") )
		return RANK_D->query_respect(me) + "，我刚才不是已经回答你了吗？";

	message_vision(CYN"$N严肃的看着$n，慢慢说道：“我派的回风拂柳剑法讲究的是佛家圆通之道，临战时千变万籷，无穷无尽，\n"
	"如不是亲身体会估计很难领悟，近来你的武功已有所小成，如果你有信心，可以带着倚天剑来向我请教一下。("HIY"qingjiao"CYN")”\n"NOR, ob, me);

	me->set_temp("marks/qingjiao", 1);
	return 1;
}

int do_qingjiao()
{
	object me, ob, weapon, weapon1;

	me = this_player();
	ob = this_object();

	weapon1 = ob->query_temp("weapon");

	if( !me->query_temp("marks/qingjiao") ) return notify_fail("什么？\n");

	if( me->is_busy() || me->is_fighting() ) return notify_fail("你正忙着呢。\n");

	if( ob->is_busy() || ob->is_fighting() ) return notify_fail(ob->name()+"正忙着呢。\n");

	if( me->query("qi") < me->query("max_qi")*3/10 )
		return notify_fail("你现在状态不佳，还是等会再请教吧！\n");

	if( me->query("eff_qi") < me->query("max_qi")/2 )
		return notify_fail("你现在重伤在身，还是等会再请教吧！\n");

	if( !weapon = present("yitian jian", me) ) {
		message_vision(CYN"$N对$n说：“恳请师太指点"+RANK_D->query_self(me)+"的峨嵋剑法吧！”\n"NOR, me, ob);
		message_vision(CYN"$n说：“" + RANK_D->query_respect(me) + "还是先去把倚天剑拿来吧！”\n"NOR, me, ob);
		me->start_busy(1);
		return 1;
	} else {
		me->start_busy(1);
		message_vision(CYN"$N对着$n恭恭谨谨的递上倚天剑说道：“恳请师太指点"+RANK_D->query_self(me)+"的峨嵋剑法吧！”\n"NOR, me, ob);
		me->delete_temp("marks/qingjiao");
		me->set("em_jianjue/time", time());
		me->set("em_jianjue/exp", me->query("combat_exp"));

		if( weapon1 ) weapon1->unequip(ob); 
		weapon->move(ob);
		weapon->wield(ob);
		ob->set("jiali", 150);
		ob->set("neili", query("max_neili")*2 );
		ob->set("jingli", query("eff_jingli")*2 );
		message_vision(HIW"$n默念峨嵋剑诀，“"HBRED+HIW"前后左右，全无定向，后发制人，先发制于人。"NOR+HIW"”顿时剑气纵横，将$N攻个措手不及！"NOR, me, ob);
		ob->set_temp("jianjue", 1);
		COMBAT_D->do_attack(ob, me, weapon, 1);
		if( me->query("qi") >= me->query("max_qi")*3/10 ) COMBAT_D->do_attack(ob, me, weapon, 1);
		if( me->query("qi") >= me->query("max_qi")*3/10 ) COMBAT_D->do_attack(ob, me, weapon, 1);
		if( me->query("qi") >= me->query("max_qi")*3/10 ) {
			message_vision(HIW"$n默念峨嵋剑诀，“"CYN"先以心使身，从人不从己，由己则滞，从人则活。"HIW"”右手猛的运剑回转，左手截手九式随劲而起！"NOR, me, ob);
			weapon->unequip(ob); 
			COMBAT_D->do_attack(ob, me, 0, 1);
			weapon->wield(ob);
		}
		if( me->query("qi") >= me->query("max_qi")*3/10 ) COMBAT_D->do_attack(ob, me, weapon, 1);
		if( me->query("qi") >= me->query("max_qi")*3/10 ) {
			message_vision(HIW"$n默念峨嵋剑诀，“"HIY"从身能从心，由己仍从人。前进后退，处处恰合。"HIW"”紧接着"+weapon->name()+HIW"猛的回转再次刺出！"NOR, me, ob);
			COMBAT_D->do_attack(ob, me, weapon, 1);
		}
		ob->delete_temp("jianjue");

		if( me->query("qi") < me->query("max_qi")*3/10 ) {
			message_vision(CYN"$n非常失望的对$N说到：“"+me->name()+"，你武功未成，还是等你打好基础再说，倚天剑暂时就由我帮你保管吧！”\n"NOR, me, ob);
			weapon->unequip(ob); 
			destruct(weapon);
			if( weapon1 ) weapon1->wield(ob);
			me->start_busy(1);
			me->set_temp("marks/失败", 1);
			return 1;
		}
		weapon->unequip(ob); 
		weapon->move(me);
		message_vision(CYN"\n$n将倚天剑交还给$N，愉快的说着：“"+me->name()+"，你自己慢慢琢磨下我所使用的剑诀吧！”\n"NOR, me, ob);
		if( weapon1 ) weapon1->wield(ob);
		message_vision(HIY"\n$N拼命地回忆着刚才的招数，开始认真思索着.........\n", me);
		if( random(2) && ( random(me->query_int()) > 30 || random(me->query_con()) > 30 ) ) {	// 随机 + 跟骨或者悟性 高
			message_vision(HIW"$N突然捕捉住交手瞬间的一幕，自言自语：“前后左右，全无定向，后发制人，先发制于人。”渐渐领悟出峨嵋剑法的真谛。\n", me);
			tell_object(me, BYEL"恭喜你，你于" + NATURE_D->game_time() + "领悟了峨嵋剑诀。\n"NOR);
			me->set("emei/jianjue", 1);
		} else {
			message_vision(HIC"$N摇了摇头，道：“好像还是很困难的。”说罢，深深地叹了口气。\n", me);
			me->set_temp("marks/失败", 1);
		}
	}
	return 1;
}

