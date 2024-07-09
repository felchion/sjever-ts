// yang.c 杨过
// By River 98.8

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_sword();
int pfm_haichao();
int pfm_anran();
string ask_skill();
string ask_skill1();
string ask_skill2();

void create()
{
	set_name("杨过",({"yang guo","yang","guo"}));
	set("title",HIW"古墓侠侣"NOR);
	set("nickname",HIY"神雕大侠"NOR);
	set("long", "他就是名满天下的杨过。虽断一臂但仍遮不去一脸的英雄豪气。\n");
	set("age", 39);
	set("attitude", "friendly");
	set("gender", "男性");
	set("shen", 10000);
	set("per", 36);
	set("str", 40);
	set("int", 35);
	set("con", 40);
	set("dex", 35);

        set("max_qi", 20500);
        set("max_jing", 17000);
        set("neili", 28500);
        set("eff_jingli", 17500);
        set("max_neili", 18500);
        set("jiali", 150);
        set("combat_exp", 14800000);
        set("unique", 1);

        set_skill("sword",450);
        set_skill("dodge",450);
        set_skill("strike",450);
        set_skill("force", 450);
        set_skill("hand", 450);
        set_skill("cuff", 450);
        set_skill("parry", 450);
        set_skill("finger",240);
        set_skill("literate", 520);
        set_skill("xiantian-gong", 180);
        set_skill("hamagong",450);
        set_skill("medicine",120);
        set_skill("jingmai-xue", 280);
        set_skill("meinu-quanfa", 450);
        set_skill("yunu-shenfa", 450);
        set_skill("xuantie-jianfa", 470);
        set_skill("anran-zhang", 480);
        set_skill("quanzhen-jianfa",460);
        set_skill("tianluo-diwang", 440);
        set_skill("yunu-xinjing",450);
        set_skill("jiuyin-zhengong",360);

        map_skill("force", "yunu-xinjing");        
        map_skill("sword","xuantie-jianfa");
        map_skill("dodge", "yunu-shenfa");
        map_skill("parry", "anran-zhang");        
        map_skill("hand", "tianluo-diwang");
        map_skill("strike", "anran-zhang");        
        prepare_skill("strike","anran-zhang");

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
		(: exert_function, "xinjing" :),
		(: pfm_haichao :),
		(: perform_action, "strike.xiaohun":),
		(: pfm_anran :),
	}));

	create_family("古墓派",3,"男主人");

	set_temp("apply/damage", 70);
	set_temp("apply/dodge", 60);
	set_temp("apply/attack", 60);
	set_temp("apply/armor", 70);

	if (random(3))
		set("env/玄铁剑法", "汹涌");

        set("inquiry", ([
		"古墓" : "那日华山与众英雄诀别后和龙儿隐居于此已有数年，不闻世事，能与\n"+
			"龙儿在此长伴一世，此身足矣！",
		"小龙女" : "龙儿乃我爱妻，你问她做甚？",
		"玄铁剑" : (: ask_sword :),
		"玄铁剑法" : (: ask_sword :),
                "武功"  : (: ask_skill :),
                "剑法"  : (: ask_skill1 :),
                "内功"  : (: ask_skill2 :),

	]));

        setup();
	carry_object(BINGQI_D("sword/gangjian"));
	carry_object("/d/gumu/obj/buxue1")->wear();
	carry_object("/d/gumu/obj/pao1")->wear();
}

void attempt_apprentice(object ob)
{
	mapping fam = ob->query("family");
	if(!fam || fam["family_name"] != "古墓派"){
		command("say "+RANK_D->query_respect(ob)+"与本派素无来往，不知此话从何谈起？");
		return;
	}
	if(ob->query("gender") != "男性") {
		command("bow "+ ob->query("id"));
		command("say 我可只收男徒，你去找吾妻龙儿试试吧。");
		return;
	}       
	if((int)ob->query_skill("yunu-xinjing", 1) < 120) {
		command("say 内功是古墓武功之基础。"); 
		command("say " + RANK_D->query_respect(ob)+"是否还应该先在内功上多下点功夫？");
		return;
	}   
	if((int)ob->query_int() < 22 ) {
		command("say 我的武功都要极高的悟性才能学会。");
		command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
		command("say "+RANK_D->query_respect(ob)+"的悟性还大有潜力可挖，还是请回吧。");
		return;
	}
	if((int)ob->query("shen") < 10000){
		command("say 我古墓弟子行走江湖中，一向行侠仗义。");
		command("say 这位" + RANK_D->query_respect(ob)+"正气不够，去做几件侠义的事后再来吧？");
		return;
	}
	command("say 嗯，看你还是个学武的料，我就收下你吧！");         
	command("chat 古墓武功重现江湖，"+ ob->name() +"可别让为师看走眼了！");
	command("chat* pat " + ob->query("id"));
	command("recruit " + ob->query("id"));
	ob->set("title",HIW"古墓派神雕侠侣传人"NOR);
}

string ask_sword()
{
	object ob=this_player();
	mapping fam = ob->query("family");

	if ( !fam || fam["family_name"] != "古墓派")
		return RANK_D->query_respect(ob) +"非我古墓传人，不知道此话怎讲？";

	if ( fam["generation"] != 4 )
		return RANK_D->query_respect(ob) +"非我神雕侠侣传人，还是等以后再说吧。";

	if ( ob->query("shen") < -100000)
		return RANK_D->query_respect(ob) +"邪气太重，想必独孤前辈也不愿他的剑法被用来做恶。";

	if ( ob->query("ygpass"))
		return "我不是已经告诉你玄铁剑法的运气之道了么？";

	if ( ob->query("gender")=="男性") {
		command("pat "+ ob->query("id"));
		command("say 当年我在襄阳城南，得遇神雕的相助，于独孤大侠墓前练成玄铁剑法。");
		ob->set("ygpass", 1);
		return "屏气具息，凝气守中，意守丹田，这就是在洪水中运气的诀窍，你好好去领悟一下吧。";
	}
	if ( ob->query("gender") == "女性"){
		if ((int)ob->query_str() < 35 )
			return "这位"+RANK_D->query_respect(ob) +"，玄铁剑法需要极高的膂力，你还是再努力一翻吧。";
		if ((int)ob->query_dex() < 35 )
			return "这位"+RANK_D->query_respect(ob) +"，在洪水中练习玄铁剑法需要极高的身法，你还是再努力一翻吧。";
		if ((int)ob->query_skill("sword", 1) < 200 )
			return "这位"+RANK_D->query_respect(ob) +"的基本剑法还需继续修炼，否则难以抵挡洪水的冲击。";
		if ((int)ob->query_int() < 35 )
			return "这位"+RANK_D->query_respect(ob) +"的悟性还不够高，难以理解我所要讲的口诀。";
		command("pat "+ ob->query("id"));
		command("say 当年我在襄阳城南，得遇神雕的相助，于独孤大侠墓前练成玄铁剑法。");
		ob->set("ygpass", 1);
		return "屏气具息，凝气守中，意守丹田，这就是在洪水中运气的诀窍，你好好去领悟一下吧。";
	}
}

int pfm_haichao()
{
	object me,weapon;
	me = this_object();
	weapon = present("gangjian",me);
	if( me->is_busy()) return 0;
	if( objectp(weapon)){
		set("jiali", 50);
		command("wield jian");
		command("perform haichao");
		if(random(10) > 5){
			set("jiali", 150);
			command("unwield jian");
		}
		return 1;
	}
	else {
		new(BINGQI_D("sword/gangjian"))->move(me);
		set("jiali", 50);
		command("wield jian");
		command("perform haichao");
		if(random(10) > 5){
			set("jiali", 150);
			command("unwield jian");
		}
		return 1;
	}
	return 1;
}

int pfm_anran()
{
	object me,weapon;
	me = this_object();
	weapon = me->query_temp("weapon");

	if(me->is_busy()) return 0;
	if( me->query("eff_qi") < 1 ) return 0;
	if( me->query("max_qi") / me->query("eff_qi") < 2 ) return 0;
	if(objectp(weapon)) weapon->unequip();
	this_object()->set("jiali", 150);
	command("perform anran");
	return 1;
}

string ask_skill()
{
	mapping fam;

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "古墓派" )
		return "我与"+RANK_D->query_respect(this_player())+"素无瓜葛，不知此话从何谈起？";

	if( this_player()->query_temp("gumu_skill") )
		return RANK_D->query_respect(this_player()) + "，你可以向我请教‘剑法’或‘内功’方面的问题！";

	if( this_player()->query_skill("sword") < 150 )
		return RANK_D->query_respect(this_player()) + "的剑法似乎还不够精进啊！";

	if( this_player()->query("gumu_given") >= 300 )
		return RANK_D->query_respect(this_player()) + "，你的武功已经不弱了，我已经没什么可以指教的了。";

	if( this_player()->query_skill("yunu-xinjing", 1) > this_player()->query("gumu_given") + 30 ) {
		this_player()->set_temp("gumu_skill", 1);
		return RANK_D->query_respect(this_player()) + "的内功大有精进，我就指点你几下吧，你可以请教我关于‘"HIY"剑法"CYN"’或‘"HIY"内功"CYN"’的疑问！";
	} else {
		return "我近期不是刚指教过你吗？武学的关键靠的还是自己努力练习啊！\n";
	}
}

string ask_skill1()
{
	object me, ob;
	int skill, pot;
	mapping fam;

	me = this_player();
	ob = this_object();

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "古墓派" )
		return "我与"+RANK_D->query_respect(this_player())+"素无瓜葛，不知此话从何谈起？";

	if( this_player()->query_skill("sword") < 150 )
		return RANK_D->query_respect(this_player()) + "的剑法似乎还不够精进啊！";

	if( !me->query_temp("gumu_skill") )
		return "当年我在襄阳城南，得遇神雕的相助，于独孤大侠墓前练成玄铁剑法！";

	if( (int)me->query_skill("yunu-xinjing", 1) > 300 ) 
		skill = 300 - (int)me->query("gumu_given");
	else skill = (int)me->query_skill("yunu-xinjing", 1) - (int)me->query("gumu_given");

	pot = skill * 10;
	pot += random(pot/2);

	skill /= 30;
	if( skill < 1 ) skill = 1;

	message_vision(HIB"$n微笑不语，将手中的剑从轻而响，从响而轻，忽寂然无声，忽轰轰之声，反复演练着。\n"NOR,me,ob);
	message_vision(HIW"$N照着$n所展示的方法练习，果然武功大有长进。\n"NOR,me,ob);

	me->delete_temp("gumu_skill");
	me->set("gumu_given", (int)me->query_skill("yunu-xinjing", 1));

	me->set_skill("xuantie-jianfa", me->query_skill("xuantie-jianfa", 1) + skill );
	me->set_skill("quanzhen-jianfa", me->query_skill("quanzhen-jianfa", 1) + skill );
	me->set_skill("yunu-jianfa", me->query_skill("yunu-jianfa", 1) + skill );
	me->set_skill("sword", me->query_skill("sword", 1) + skill );
	me->add("potential", pot );

	tell_object(me, HIC"你的「玄铁剑法」进步了！\n"NOR);
	tell_object(me, HIC"你的「全真剑法」进步了！\n"NOR);
	tell_object(me, HIC"你的「玉女素心剑」进步了！\n"NOR);
	tell_object(me, WHT"你的增加了" + chinese_number(pot) + "点潜能。\n"NOR);

	return RANK_D->query_respect(me) + "天资过人，真是个可造之才啊！哈哈哈！";
}

string ask_skill2()
{
	mapping fam;
	object me, ob;
	int skill, neili;

	me = this_player();
	ob = this_object();

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "古墓派" )
		return "我与"+RANK_D->query_respect(this_player())+"素无瓜葛，不知此话从何谈起？";

	if( this_player()->query_skill("sword") < 150 )
		return RANK_D->query_respect(this_player()) + "的剑法似乎还不够精进啊！";

	if( !me->query_temp("gumu_skill") )
		return "屏气具息，凝气守中，意守丹田，这就是在洪水中运气的诀窍，你好好去领悟一下吧。";

	if( (int)me->query_skill("yunu-xinjing", 1) > 300 ) 
		skill = 300 - (int)me->query("gumu_given");
	else skill = (int)me->query_skill("yunu-xinjing", 1) - (int)me->query("gumu_given");

	neili = skill/2;
	neili += random(neili/4);

	skill = random(skill)/30;
	if( skill < 1 ) skill = 1;

	message_vision(HBYEL"$n屏气具息，凝气守中，意守丹田，周身真气聚集，正反运行玉女心经。\n"NOR,me,ob);
	message_vision(HIW"$N受了$n的启悟，心中疑团顿时解开，对内功的领悟又更进了一层。\n"NOR,me,ob);

	me->delete_temp("gumu_skill");
	me->set("gumu_given", (int)me->query_skill("yunu-xinjing", 1));

	me->set_skill("yunu-xinjing", me->query_skill("yunu-xinjing", 1) + skill );
	me->set_skill("force", me->query_skill("force", 1) + skill );
	tell_object(me, HIC"你的「玉女心经」进步了！\n"NOR);
	tell_object(me, HIC"你的「基本内功」进步了！\n"NOR);

	me->add("max_neili", neili );
	tell_object(me, WHT"你的内力修为增加了" + chinese_number(neili) + "点。\n"NOR);

	return RANK_D->query_respect(me) + "天资过人，真是个可造之才啊！哈哈哈！";
}