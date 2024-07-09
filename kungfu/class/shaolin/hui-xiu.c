// Npc: /kungfu/class/shaolin/hui-xiu.c
#include <job_mul.h>
#include <ansi.h>
inherit NPC;
string ask_me();

void create()
{
	set_name("慧修尊者", ({
		"huixiu zunzhe",
		"huixiu",
		"zunzhe",
	}));
	set("long",
		"他是一位两鬓斑白的老僧，身穿一袭青布镶边袈裟。他身材略高，\n"
		"太阳穴微凸，双目炯炯有神。\n"
	);

	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");

        set("age", 50);
        set("shen_type", 1);
        set("str", 20);
        set("int", 20);
        set("con", 20);
        set("dex", 20);
        set("max_qi", 450);
        set("max_jing", 300);
        set("neili", 600);
        set("max_neili", 600);
 
	set("jiali", 50);
	set("combat_exp", 120000);
	set("score", 100);

        set_skill("force", 70);
        set_skill("yijin-jing", 70);
        set_skill("dodge", 70);
        set_skill("shaolin-shenfa", 70);
        set_skill("cuff", 75);
        set_skill("jingang-quan", 75);
        set_skill("staff", 76);
        set_skill("pudu-zhang", 76);
        set_skill("parry", 70);
        set_skill("buddhism", 70);
        set_skill("literate", 70);
   	map_skill("force", "yijin-jing");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("cuff", "jingang-quan");
        map_skill("staff", "pudu-zhang");
        map_skill("parry", "pudu-zhang");

        prepare_skill("cuff", "jingang-quan");

	create_family("少林派", 38, "弟子");
        set("inquiry",  ([
		"轮值" : (: ask_me :),
	]));
	setup();

        carry_object("/d/shaolin/obj/chanzhang")->wield();
        carry_object("/d/shaolin/obj/hui-cloth")->wear();
}

#include "/kungfu/class/shaolin/hui.h"

void init()
{
	::init();
	add_action("do_task","task");
}
string ask_me()
{
        mapping fam; 
        object me, ob;
        ob=this_player();
        me=this_object();
        
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "少林派")
                return RANK_D->query_respect(ob) +"，你不是本寺弟子，此话从何说起？";
        if (!wizardp(ob) && ob->query("class")!="bonze")
            	return ("俗家弟子不能轮值。");
	if (!wizardp(ob) && ob->query_condition("job_busy"))
		return "现在不需要轮值，你还是去干别的吧。";
	if (!wizardp(ob) && ob->query("combat_exp") < 60000)
		return ("你的武功太差，我怕来了坏人你对付不了，丢了我少林的面子。");
        if (!wizardp(ob) && ob->query("combat_exp") >= 90000)
		return ("我看你的武功已有相当的功底了，就把轮值的机会让给师弟们吧。\n");

	me->set("lunzhi_name",ob->query("name"));
	ob->set_temp("lunzhi",1);
	ob=new("/d/shaolin/npc/obj/lunzhi-ling");
	ob->move(this_player());
	write("慧修大师给你一块轮值令。\n");
	return "好吧，你就到迎客亭把令交给虚通吧，轮值时应当小心防范，防止外敌偷入寺中。";
}

int do_task(string arg)
{
	object ob,me;
	int exp,pot;
	ob=this_player();
	me=this_object();

	if(!(arg)) return 0;
	if(!(arg=="ok")) return 0;
	if(!ob->query_temp("lunzhi")) return 0;
	if(!ob->query_temp("lunzhied")) return 0;
	if(ob->query_temp("lunzhi_ok")) {
		if (interactive(ob) && (int)ob->query_condition("sl_lunzhi")) {
			command("kick "+ob->query("id"));
			return notify_fail(RED"想偷懒，快给我滚回去。\n"NOR);
		}
		ob->add("job_time/少林",1);
		
		command("pat "+ob->query("id"));
		command("say 哈哈，真难为你了，"+RANK_D->query_respect(ob)+"，干得好！");
		
		pot =25 + random(10);
		exp =(int)(ob->query_skill("buddhism",1)/4)+70;
		
		exp = ob->add_exp_combat(exp,me,"少林轮值");
		//ob->add("combat_exp",exp);
		//ob->add("job_time/少林轮值",1);
		//GIFT_D->check_count(ob,me,"少林轮值");
						
		ob->add("potential",pot);
		if (ob->query("potential") > ob->query("max_pot"))
			ob->set("potential", ob->query("max_pot"));   
		
		me->delete("lunzhi_name");
		me->delete_temp("lunzhi");
		ob->delete_temp("lunzhi");
		ob->delete_temp("lunzhied");
		return 1;
	}
	return notify_fail("你这个东西是从哪里来的？\n");
}
