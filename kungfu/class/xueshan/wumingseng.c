// Npc: wuming-laoseng

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "wuming.h"

void create()
{
	set_name("无名老僧", ({	"wuming laoseng", "wuming", "laoseng",}));
	set("long",
		"他是一位身穿青袍的枯瘦僧人，身材不高。\n"
		"年纪已在七旬开外，稀稀疏疏的几根长须已然全白。\n"
	);

	set("nickname", "无名神僧");
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");
	set("no_bark",1);
	set("no_quest", 1);

	set("age", 70);
	set("shen", 32000);
	set("str", 40);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("max_qi", 12000);
	set("max_jing", 10000);
	set("eff_jingli", 9000);
	set("neili", 22000);
	set("max_neili", 22000);
	set("jiali", 150);
	set("combat_exp", 15000000);

	set_skill("force", 500);
	set_skill("yijin-jing", 500);
	set_skill("dodge", 500);
	set_skill("shaolin-shenfa", 500);
	set_skill("fumoquan-zhen", 200);
	set_skill("claw", 500);
	set_skill("parry", 500);
	set_skill("cuff", 500);
	set_skill("club", 500);
	set_skill("blade", 500);
	set_skill("ranmu-daofa", 500);
	set_skill("finger", 500);
	set_skill("strike", 500);
	set_skill("buddhism", 200);
	set_skill("riyue-bian", 500);
	set_skill("whip", 500);
	set_skill("yizhi-chan", 500);
	set_skill("literate", 250);
	set_skill("jingang-quan", 500);
	set_skill("hand", 500);
	set_skill("leg", 500);
	set_skill("ruying-suixingtui", 500);
	set_skill("banruo-zhang", 500);
	set_skill("qianye-shou", 500);
	set_skill("longzhua-gong", 500);
	set_skill("weituo-chu", 500);
	set_skill("zui-gun", 500);
	set_skill("club", 500);
	set_skill("nianhua-zhi",500);
	set_skill("xiuluo-dao",500);

	map_skill("blade","ranmu-daofa");
	map_skill("club","weituo-chu");
	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("finger", "yizhi-chan");
	map_skill("cuff","jingang-quan");
	map_skill("leg","ruying-suixingtui");
	map_skill("hand","qianye-shou");
	map_skill("claw","longzhua-gong");
	map_skill("whip","riyue-bian");
	map_skill("strike","banruo-zhang");
	prepare_skill("finger", "yizhi-chan");
	set("chat_chance_combat", 80);
	set("chat_msg_combat", ({
		(: exert_function, "jingang" :),
		(: perform_action, "finger.qiankun" :),
		(: perform_action, "finger.wofo" :),
	}));
	create_family("少林派", 34, "弟子");
	set("inquiry", ([
		"佛法": (: ask_fofa :),
		"重新修业": (: ask_reset_skill :),
		"韦陀杵": (: ask_wtc :),
		"散功": (: ask_sangong :),
		"萧峰":"老僧对萧施主好生相敬，唯大英雄能本色，萧施主当之无愧。",
		"降龙十八掌":"降龙十八掌，果然天下第一。",
	]));

	setup();

	set_temp("apply/damage", 80);
	set_temp("apply/dodge", 100);
	set_temp("apply/attack", 80);
	set_temp("apply/armor", 90);
	UPDATE_D->get_cloth(this_object());
}

void kill_ob(object me)
{
	command("yun jingang");
	::kill_ob(me);
}

void attempt_apprentice(object ob)
{
	string new_name;
		if (!ob->query_temp("laoseng_pass",)){
		command("say 对不起，老衲现在不收徒。");
		return;
	}
	command("recruit " + ob->query("id"));
	new_name=ob->query("name");
	new_name[0..0] = "渡";
	ob->set("name", new_name);
	command("say 从今以后你的法名叫做" + new_name + "，就随老衲研修少林佛法吧！\n");
}

int accept_object(object who, object ob)
{
	mapping my_fam  = who->query("family");
	if ((string)ob->query("id") != "fanwen jing"){
		command(":)");
		return 0;
	}

	if (!(my_fam) || my_fam["family_name"] !="少林派"
	 || my_fam["generation"] != 36 || who->query_skill("yijin-jing",1) <180
	 || who->query_skill("buddhism",1) <160
	 || who->query_skill("literate",1) <150
	 || !who->query_temp("sl_bookget")) {
		command(":)");
		command("say "+RANK_D->query_respect(who)+"不辞辛苦把经书送给还少林，老衲代方丈仅此谢过。\n");
		command("say 空既是幻，幻既是空。\n");
		who->delete_temp("sl_bookget");
		call_out("destroying", 1, ob);
		return 1;
	}
	command("say "+RANK_D->query_respect(who)+"不辞辛苦把经书找回，了却老衲心愿，善哉，善哉。\n");
	command("look "+(string)who->query("id"));
	command("say "+RANK_D->query_respect(who)+"可愿拜老衲为师？\n");
	who->delete_temp("sl_bookget");
	who->set_temp("laoseng_pass",1);
	call_out("destroying", 1, ob);
	return 1;
}

void destroying(object obj)
{
        destruct(obj);
        return;
}
