// NPC wei.c 韦一笑
// Modify By River@SJ 99.06
// been job_npc
#include <ansi.h>
#include <job_mul.h>

inherit NPC;
inherit F_MASTER;
string *drugs = ({
	MEDICINE_D("sanhuang"),
	MEDICINE_D("huiyang"),
	MEDICINE_D("yuling"),
	MEDICINE_D("yuzhen"),
	MEDICINE_D("tianwang"),
});

void destroy(object ob);
void create()
{
	set_name("韦一笑",({"wei yixiao","wei","yixiao"}));
	set("title","明教护教法王");
	set("nickname",GRN"青翼蝠王"NOR);
	set("long", "他是明教的四大护法之一的青翼蝠王。\n"+
		"他身材高大，形如竹杆，生就一身好轻功。\n");
	set("age", 51);
	set("attitude", "peaceful");
	set("shen", -12000);
	set("str", 26);
	set("int", 26);
	set("con", 25);
	set("dex", 45);        

	set("max_qi", 3000);
	set("max_jing", 2400);
	set("eff_jingli", 2000);
	set("jiali", 70);
	set("combat_exp", 1200000);
	set("unique", 1);
	
	set("job_npc",1);

	set_skill("strike",180); 
	set_skill("literate",110);
	set_skill("dodge",190);
	set_skill("force", 150);
	set_skill("piaoyi-shenfa", 190);
	set_skill("hanbing-mianzhang",180);
	set_skill("shenghuo-shengong", 140);
	set_skill("parry", 150);
	map_skill("force", "shenghuo-shengong");
	map_skill("dodge", "piaoyi-shenfa");
	map_skill("strike", "hanbing-mianzhang");
	map_skill("parry", "hanbing-mianzhang");
	prepare_skill("strike","hanbing-mianzhang");

	create_family("明教", 36, "法王");
	set("chat_chance_combat", 40);
	set("chat_msg_combat", ({
		(: perform_action, "strike.xixue":),
	}));
	setup();
	carry_object("/d/mingjiao/obj/yel-cloth")->wear();
}

void init()
{
	object who=this_player();
	mapping fam = who->query("family");
	::init();
	add_action("do_kill", "kill");
	add_action("do_kill", "tk");
	add_action("do_kill", "teamkill");
	add_action("do_kill", "hit");
	if (!who->query("jiuyang/wei")
	&&  fam && fam["family_name"] == "明教"
	&& who->query("combat_exp") > 10000000	)
	{
		command("whisper "+who->query("id")+" 你快去张教主那里一次，他好象有什么传闻要告诉你。");
		who->set("jiuyang/wei", 1);
	}
}

void attempt_apprentice(object ob)
{
        object me  = this_object();
        mapping fam = ob->query("family");
        mapping my_fam = me->query("family");
        if(!fam || fam["family_name"] != "明教"){
                command("say "+RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
                return;
        }
        if(fam["master_name"] == "张无忌"){
                command("haha");
                command("say "+RANK_D->query_respect(ob)+"已是教主亲传弟子，"+RANK_D->query_self(me)+"怎敢再收你为徒？"); 
                return;
        }
        if (fam["generation"] == my_fam["generation"]){
                command("shake");
                command("say "+RANK_D->query_respect(ob)+"已拜光明使为师，"+RANK_D->query_self(me)+"怎敢再收你为徒？");
                return;
        }        
        if ((int)ob->query_skill("shenghuo-shengong", 1) < 100) {
                command("say 要学更为高深的武功，必须有高深的内功修为。"); 
                command("say " + RANK_D->query_respect(ob)+"是否还应该在圣火神功上多下点功夫？");
                return;
        }        
        if ((int)ob->query_skill("dodge",1) < 100) {
                command("say 想学我的轻功身法，必须有一定的轻功基础。");
                command("say 在轻功方面，" + RANK_D->query_respect(ob)+"是否还做得不够？");
                return;
        }
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title", GRN"明教青翼蝠王属下教众"NOR);
}

int accept_object(object who, object ob,object me)
{
	int exp,job,pot,bonus,gold, x, p;

	me = this_object();
	if(ob->query("name") != "巡逻令" || ob->query("id") != "xunluo ling") {
		command("? "+who->query("id"));  
		return 0;
	}
	if(ob->query("owner") != who->query("id") && ! wizardp(who)){
		command("say 哼，你这是从哪偷来的？");
		call_out("destroy", 1, ob);  
		return 1;
	}
	if(!(who->query_temp("mj/over")) && ! wizardp(who)){
		command("say 你的任务完成了吗？");
		command("pat "+who->query("id"));  
		return 0;
	}
	if(!who->query_condition("mj_xunluo") && ! wizardp(who)){
		command("say 这么长时间，你都干吗去了？");
		command("disapp "+who->query("id"));
		call_out("destroy", 1, ob);  
		return 1;
	}
	command("nod");
	command("say 愿我明尊座下所有弟子都如此勤奋向上！");
	bonus = who->query_temp("mj/attacker1");
	if(bonus > 7) bonus = 7;
	exp = 600 + random(100) + bonus * 50;
	job = who->query("job_time/明教");
	if(job< (who->query("job_time/明教巡逻")+who->query("job_time/明教守卫")) ){
		who->set("job_time/明教",(who->query("job_time/明教巡逻")+who->query("job_time/明教守卫")));
		job = who->query("job_time/明教");
	}

	gold = job/50 + random(job/50);

	if( gold < 1) gold = 1;
	if( gold > 15) gold = 14 + random(3);

	if (who->query("combat_exp") > 2000000)	exp = random(100) + bonus * 50+100;//稍微给一点
	
	exp = who->add_exp_combat(exp,this_object(),"明教巡逻");	
	
	if (who->query("combat_exp") > 2000000)
		pot = 100 + random(60);
	else
		pot= exp/5 + random(exp/10);

	who->add("potential", pot);
	p = who->query("mingjiao_job");
	if ( p > 0 ) {
		who->add("job_time/明教", p);
		who->delete("mingjiao_job");
	}
	who->add("job_time/明教", 1);	
	who->delete_temp("mj/attacker1");
	who->delete_temp("mj/over");
	if(who->query("potential") > who->query("max_pot")) 
		who->set("potential", who->query("max_pot"));
	MONEY_D->pay_player(who, gold * 400);
	message_vision("\n$N给了$n一些"HIW"白银"NOR"。\n"NOR,this_object(),who);
	command("whisper "+who->query("id")+" 你辛苦了，这些钱拿去好好快活一番吧。");
	job += 1;
	tell_object(who,"你已经为明教完成"+chinese_number(job)+"次工作了，再好好努力吧。\n");
	log_file("job/xunshan",sprintf("%8s%-10s第%4d次明教任务奖励经验%4d，潜能%3d，现在经验%d。\n",
		who->name(1),"("+who->query("id")+")",job,exp,pot,who->query("combat_exp")), who);
	call_out("destroy", 1, ob);
	if ( who->query("combat_exp",1 ) > 2000000 && job > 2000)
		x = job%20;
	else 
		x = job%50;

	if ( wizardp(who))
		tell_object(who,sprintf("x = %d\n", x ));

	if(who->query("cursed")) return 1;
	
	if( random(job) > 300
	 && x == 0
	 && !who->query("jiuyang/wei")
	 && ( !random(8) || (job > 2000 && !random(3)) )
	 )
	{
		command("whisper "+who->query("id")+" 你快去张教主那里一次，他好象有什么传闻要告诉你。");
		who->set("jiuyang/wei", 1);
		log_file("quest/jiuyang",sprintf("%8s%-10s明教任务第%s次的时候，从韦一笑处，听到有关九阳神功的传闻。\n",
			who->name(1),"("+ capitalize(getuid(who))+")",chinese_number(job)), who);
	}
	return 1;
}

void destroy(object ob)
{
	destruct(ob);
	return;
}

int do_kill(string arg)
{
	object ob,ob2; 
	object me = this_player();
	ob2 = this_object();

	if (!arg) return 0;
	ob = present(arg, environment(me));
	if (ob == this_object() || ob == me) return 0;
	if (!ob || !ob->is_character()) return 0;
        
	if (userp(ob)&& ob->query("family/family_name") == "明教") {
		if (me->query("family/family_name") == "明教") { 
		        message_vision(ob2->name()+"对$N喝道：身为明教弟子，竟然杀自己的同门？我宰了你！\n", me);
                }
                else {
                        message_vision(ob2->name()+"对$N喝道：大胆狂徒，居然敢来明教撒野！\n", me);
                        kill_ob(me);
                }
                me->set("vendetta/明教", 1);
                return 1;
        }
        return 0;
}
