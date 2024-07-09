// Npc: wuming-laoseng
inherit NPC;
inherit F_MASTER;
#include <ansi.h>

string ask_wx();
string ask_wj();
mixed ask_ff();

void create()
{
        set_name("无相禅师", ({"wuxiang chanshi", "wuxiang", "chanshi",
        }));
        set("long",
                "他是一位身穿青袍的枯瘦僧人，身材精瘦。\n"
                "须发已然全白，也看不出多少年纪。\n"
        );


        set("gender", "男性");
        set("attitude", "friendly");
        set("class", "bonze");
        set("no_bark",1);

        set("age", 80);
        set("shen", 32000);
        set("str", 40);
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("max_qi", 8000);
        set("max_jing", 7000);
        set("eff_jingli", 4000);
        set("neili", 25000);
        set("max_neili", 25000);
        set("jiali", 100);
        set("combat_exp", 6000000);

        set_skill("force", 300);
        set_skill("yijin-jing", 300);
        set_skill("dodge", 300);
        set_skill("shaolin-shenfa", 300);
        set_skill("parry", 300);
        set_skill("finger", 300);
        set_skill("buddhism", 200);
        set_skill("literate", 200);
	set_skill("wuxiang-zhi",300);
	set_skill("cuff",300);
	set_skill("luohan-quan",300);

        map_skill("force", "yijin-jing");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("finger", "wuxiang-zhi");
	map_skill("cuff","luohan-quan");

        prepare_skill("finger", "wuxiang-zhi");
	prepare_skill("cuff", "luohan-quan");

        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
                (: perform_action, "finger.qiankun" :),
        }));

        create_family("少林派", 34, "弟子");
	set("inquiry", ([
		"少林武技": (: ask_wj :),
		"无相劫指": (: ask_wx :),
		"佛法": (: ask_ff :),
	]));

        setup();
        set_temp("apply/damage", 50);
        set_temp("apply/dodge", 100);
        set_temp("apply/attack", 80);
        set_temp("apply/armor", 60);
	UPDATE_D->get_cloth(this_object());
	set("no_quest", 1);
}

void attempt_apprentice(object ob)
{
	command("say 对不起，老衲现在不收徒。");
	return;
}

string ask_wj()
{
	object me=this_player();

	if(me->query("wxz/quest") < 4) return 0;
	if( me->query("wxz/quest")== 4 )
		me->set("wxz/quest",5);
	return RANK_D->query_respect(me)+"何苦贪恋武学小技，须知慈悲为怀，方能修成正果。\n"
		+"不过既然你能来到这里，也算我俩有缘，我这套无相劫指，便传了给你吧。\n";
}

string ask_wx()
{
	object me=this_player();

	if(me->query("wxz/done"))
		return RANK_D->query_respect(me)+"不是已经学到了么？\n";

	if(me->query("wxz/quest")<5)
		return "";
	if( me->query("wxz/quest")== 5 )
		me->set("wxz/done",1);
	log_file("quest/wuxiang", sprintf("%8s%-10s 在无相禅师处学到无相劫指。\n",
		me->query("name"), "("+me->query("id")+")" ), me);
	command("say 你可以尝试以指力鼓劲，拨动地上的木屑，或可有所心得。");
	return "只是木片跃动，便是有相。当真要名副其实，练至无形无相，纵穷毕生之功，也不易有成。";
}

mixed ask_ff()
{
	object me=this_player();

	if (!me->query("potential")) return "你的潜能不够了。";
	if (!me->query("wxz/poison")) return RANK_D->query_respect(me)+"佛法精深，心怀慈悲，我目前实在没有什么可以教你的。";

	do if (me->add("wxz/poison",-2-random(2)) < 0) {
		me->set("wxz/poison",0);
		break;
	}
	while (me->add("potential",-4-random(2)) > 0);
	if (me->query("potential") < 0) me->set("potential",0);
	command("say 即心即佛，即佛即心，心明识佛，识佛明心，离心非佛，离佛非心……");
	command("buddhi wuxiang chanshi");
	tell_object(me,"你端坐良久，若有所悟。\n");
	return 1;
}
