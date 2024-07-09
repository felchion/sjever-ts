// 补偿 NPC
// Modify By Lane@sj 2005.01


#include <ansi.h>
inherit NPC;
#include <mudlib.h>

string ask_gold();
void create()
{
	set_name("钱爷", ({ "qian ye","qian","ye" }) );
	set("title", "风流倜傥");
	set("nickname", HIY"钱太多"NOR);
	set("gender", "男性");
	set("age",36);
	set("long","他就是蛮力的后身，听说后来做了皇帝，娶了1800个老婆。\n辽宁站的玩家可以通过'ask qian ye about 补偿'来获得一些好处。");
	set("str", 30);
	set("int", 20);
	set("con", 20);
	set("dex", 20);
	set("per", 23);
	set("shen_type",1);
	set("combat_exp", 4500000);
	set("unique", 1);
	set("quest/天龙八部/武功/quanli",1);

	set("inquiry", ([
		"补偿"  : (: ask_gold :),
	]));

	set("chat_chance_combat", 50);
	set("chat_msg_combat", ({
                (: exert_function, "longxiang" :),
                (: exert_function, "shield" :),
                (: perform_action, "cuff.qianjun" :),
                (: perform_action, "dazhuan" :),
	}));

	set_skill("cuff",380);
	set_skill("dodge",380);
	set_skill("force",380);
	set_skill("parry",380);
	set_skill("hammer",380);
	set_skill("strike",380);

	set_skill("huoyan-dao",380);
	set_skill("longxiang-boruo",380);
	set_skill("taizu-quan",380);
	set_skill("xiangfu-lun",380);
	set_skill("yuxue-dunxing",380);

	set_skill("huanxi-chan",200);
	set_skill("literate",200);

	set("max_qi",5974*3);
	set("max_jing",3600*3);
	set("eff_jingli",3821*3);
	set("neili",7908*6);
	set("max_neili",7908*3);
	set("jiali",200);

	map_skill("parry","taizu-quan");
	map_skill("cuff","taizu-quan");
	map_skill("force","longxiang-boruo");
	map_skill("dodge","yuxue-dunxing");
	map_skill("hammer","xiangfu-lun");
	map_skill("strike","huoyan-dao");
	prepare_skill("cuff","taizu-quan"); 
	setup();
	carry_object("/clone/weapon/hammer")->wield();
}

string ask_gold()
{
	object me, ob;
	int end_time = 1106452800; //January 23, 2005, 12:00 pm
	string site;
	
	site = lower_case(INTERMUD_MUD_NAME);

	me = this_player();
	ob = this_object();
	
	if (me->query("vip/redeem_gold"))
		return "有道是：人为财死，鸟为食亡，"+me->query("name")+"这个"+RANK_D->query_rude(me)+"，\n我虽然是个NPC，可是眼睛不瞎，你已经补偿过了，再问我要，不怕我把你存款冻结？\n";
	if( me->query("combat_exp") < 100000 && me->query("birthday") < 1104508800 && !me->query("vip/redeem_gold"))
		return RANK_D->query_respect(me)+"，我看你还是先练好武功吧！";
		
	if( end_time - time() > 0 && !me->query("vip/redeem_gold") && me->query("birthday") < 1104508800 && site=="ln") //1104508800 January 1, 2005, 12:00 am
	{
		me->add("balance", 1000000);
		me->set("vip/redeem_gold",1);//标记
		log_file("static/REDEEM",
				sprintf("%s 获得黄金补偿。\n",me->query("id")));
		return "听说这里前段时间老是当机，我就先给你一百两黄金做补偿，马上存入你银行！\n";
	}

	return "要钱没有，命到是有一条！\n";
}
