// yingying.c
// Modified by Numa 19991102
// Finish by Numa@SJ 2000.8

#include <ansi.h>
#include "hmy_def.c"
inherit NPC;
#include "job_npc.h"
#include "job.h"

int ask_renzhi();
int ask_jiuren();
int go_where();
int do_finish();
int do_letout();

void create()
{
        
        set_name("任盈盈", ({ "ren yingying","ren","yingying" }));
        set("long", "她容貌秀丽绝伦，不过十七八岁年纪，娇羞之态，娇美不可方物。\n");
        set("nickname", HIR"小魔女"NOR);
        set("title", HIY"日月神教  圣姑"NOR);
        set("gender", "女性");
        set("attitude", "peaceful");
        set("age", 18);
        set("str", 30);
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("per", 30);
        set("unique", 1);
        set("location", 1);
        set_temp("no_return", 1);
        set("startroom",__HMY__"pingding/kefang");

        set("max_qi", 1500);
        set("qi", 1500);
        set("jing", 1300);
        set("max_jing", 1300);
        set("neili", 2400);
        set("max_neili", 2400);
        set("eff_jingli", 1700);
        set("jingli", 1700);
        set("jiali", 50);
        set("combat_exp", 180000);
        set("shen", -1200);
/*
        set_temp("apply/attack", 40);
        set_temp("apply/defence", 40);

        set_skill("tianmo-zhang",100);
        set_skill("strike", 100);
        set_skill("parry", 100);
        set_skill("dodge",100);
        set_skill("ding-dodge", 100);
        set_skill("force", 100);
        set_skill("tianmogong", 100);

        map_skill("strike", "tianmo-zhang");
        map_skill("dodge", "ding-dodge");
        map_skill("force", "tianmogong");
        map_skill("parry", "tianmo-zhang");
        prepare_skill("strike", "tianmo-zhang");*/
        set("inquiry", ([
                "人质" : (: ask_renzhi :),
                "救人" : (: ask_jiuren :),
                "脱险" : (: do_finish :),
                "letout": (: do_letout :),
        ]));

        set("hostage/gived",0);

        setup();

        carry_object("/clone/misc/cloth")->wear();        
        if (clonep())
        	call_out("do_ready",100 + random(50));
}

void init()
{       
        object ob;

        ::init();

        if (base_name(environment(this_object())) != query("startroom")) return;
        if( interactive(ob = this_player())
        &&  userp(ob)
        &&  visible(ob)) {
            	if (ob->query_temp("hostage/job_1")) {
                        remove_call_out("greeting_1");
                        call_out("greeting_1", 1, ob);
                        return;
                }
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}

void greeting(object ob)
{
        object me = this_object();
        
        if (!ob
        ||  !present(ob, environment(me)))
                return;
        if (userp(ob)) {
                switch( random(2) ) {
                        case 0:
                                command("hi " + ob->query("id"));
                                break;
                        case 1:
                                command("wanfu " + ob->query("id"));
                                break;
                }
        }
}

void player_fail(object ob, object me)
{
	if (ob->query_temp("hostage/job_1")) {
        	ob->delete_temp("hostage");
        	ob->clear_condition("hostage_1");
        	ob->apply_condition("hostage_busy",50 + random(10));
        }

	else if (ob->query_temp("hostage/job_2")) {
        	ob->delete_temp("hostage");
        	ob->clear_condition("hostage_2");
        	ob->apply_condition("hostage_busy",40 + random(10));
        }

	else if (ob->query_temp("hostage/job_3")) {
        	ob->delete_temp("hostage");
        	ob->clear_condition("hostage_3");
        	ob->apply_condition("hostage_busy",30 + random(10));
        }

	me->set_temp("hostage/gived", 0);
}

int greeting_1(object ob)
{
        object me = this_object();
        object obj;
        int exp, pot;
        
	if (ob->query_temp("hostage/job_1") == 1) {
        	if (!(obj=present("tong baixiong",environment(me)))) {
                	command("say 你找的童百雄，他人呢？");
                	return 1;
        	}
        	if (!ob->query_temp("hostage/passed_x")) {
                	command("say 你究竟有没有向他询问有关东方不败的事情呀！");
                	return 1;
        	}
        	command("touch tong baixiong");
        	tell_room(environment(me),"童百雄在任盈盈耳边小声地说了些话。\n");
        	tell_room(environment(me),CYN"任盈盈对着童百雄关心的说道：你一路上辛苦了，先回厢房休息吧。\n"NOR);
        	destruct(obj);
	}
	else if (ob->query_temp("hostage/job_1") == 2) {
            if (!(obj=present("linghu chong",environment(me)))) {
                	command("say 你找的令狐冲，他人呢？");
                	return 1;
        	}
        	if (!ob->query_temp("hostage/passed_l")) {
                	command("say 你究竟有没有问他要黑木令呀！");
                	return 1;
        	}
        	
            command("touch linghu chong");
        	tell_room(environment(me),"令狐冲在任盈盈耳边小声地说了些话。\n");
        	tell_room(environment(me),"令狐冲给任盈盈一块黑木令。\n");
        	tell_room(environment(me),CYN"任盈盈对着令狐冲关心的说道：令狐哥哥你一路上辛苦了，先回厢房休息吧。\n"NOR);
	        destruct(obj);
	}

// Rewards
        exp = ob->query_temp("hostage/job1_killed")*50+ ob->query_condition("hostage1")*5;
	pot = exp/4 + random(exp/4);
    if( ob->query("family/family_name")!="桃花岛")
        ob->add("shen",exp*(5+random(6))+pot*10);
	ob->delete_temp("hostage/job_1");
//	ob->add("combat_exp", exp);
//	ob->add("potential", pot);
	me->set("hostage/gived", 3);
	ob->apply_condition("hostage_busy",30 + random(10));
    tell_object(ob, HIY"\n由于任务的出色完成，你得到了"+CHINESE_D->chinese_number(exp)+"点经验和"+CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"HIC"你的正气增加了。\n"NOR);

//make a log
        log_file("job/HMY_1",sprintf("%s %s(%s)[exp %d]完成任务，得到奖励经验 %d ，潜能 %d。\n", ctime(time())[4..19], ob->name(1), ob->query("id"), ob->query("combat_exp"), 
       				exp, pot));
	call_out("do_wait_ask", 200, me);
        return 1;
}

void do_wait_ask(object me)
{
	me->set("hostage/gived", 0);
	me->delete("pl1");
	me->delete("pl2");
	command("sigh");
        command("say "+me->query("pl1/name")+"("+me->query("pl1/id")+")和"
        	+me->query("pl1/name")+"("+me->query("pl1/id")+")怎么还不见回来？看来是凶多吉少了！");

}

void give_reward_2(object ob)
{
	int exp, pot;
	
	if (ob->query_temp("hostage/job_2") == 2) {
		tell_object(ob, HIY"\n由于出色的取到了钥匙，你得到了"+CHINESE_D->chinese_number(exp)+"点经验和"+CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR);
        	tell_object(ob, "你现在需要尽快赶到监狱门口去救人！\n");
        }
        else {

		tell_object(ob, HIY"\n由于出色的到达了监狱门口，你得到了"+CHINESE_D->chinese_number(exp)+"点经验和"+CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR);
        	tell_object(ob, "你现在只要在这里等待队友的到来，然后用钥匙救人(kai men)！\n");
        }

// Rewards
    exp = ob->query_temp("hostage/job2_killed")*40 + ob->query_condition("hostage2");
	pot = exp/4+random(exp/4);
//	ob->add("combat_exp", exp);
//	ob->add("potential", pot);
       	ob->delete_temp("hostage/job_2");
       	ob->set_temp("hostage/ready_3",1);
	ob->apply_condition("hostage_busy",80 + random(10));
//make a log
        log_file("job/HMY_2",sprintf("%s %s(%s)[exp %d]完成任务，得到奖励经验 %d ，潜能 %d。\n", ctime(time())[4..19], ob->name(1), ob->query("id"), ob->query("combat_exp"), 
     		exp, pot));

}

int do_finish()
{
        object ob = this_player();
        object me = this_object();
	object ob1, ob2;
        int exp1 = 0, exp2 = 0, pot1, pot2;
        
        if (me->query("hostage/gived") != 4) {
        	command("say 哎，不知道他们究竟脱险了没有！");
        	return 1;
        }
        if (!ob->query_temp("hostage/job_3")) {
                command("hmm");
                command("say 你干什么脱险？我可听不懂！");
                return 1;
        }
	if (me->query("pl1/id") == ob->query("id")) {
		ob1 = ob;
		if (!(ob2 == present(me->query("pl2/id"),environment(me)))) {
			command("咦，"+me->query("pl2/name")+"("+me->query("pl2/id")+")怎么不见了？");
			return 1;
		}
	}
	else if (me->query("pl2/id") == ob->query("id")) {
		ob2 = ob;
		if (!(ob1 == present(me->query("pl1/id"),environment(me)))) {
			command("咦，"+me->query("pl1/name")+"("+me->query("pl1/id")+")怎么不见了？");
			return 1;
		}
	}
	else {
		command("say 我正在等"+me->query("pl1/name")+"("+me->query("pl1/id")+")和"
       			+me->query("pl1/name")+"("+me->query("pl1/id")+")他们的消息！");
		return 1;
        }
        
        exp1 = ob1->query("hmy_point")*2;
        exp2 = ob2->query("hmy_point")*2;
        if (ob1->query_temp("hostage/win3")) {
        	if (ob2->query_temp("hostage/kill3"))
        		exp2 += (int)ob2->query_temp("hostage/kill3")*200;
        	exp1 += (int)ob1->query_temp("hostage/win3");
        }
        else if (ob1->query_temp("hostage/kill3")) {
        	if (ob2->query_temp("hostage/win3"))
        		exp2 += (int)ob2->query_temp("hostage/win3");
        	exp1 += (int)ob1->query_temp("hostage/kill3")*200;

	}

	pot1 = exp1/5 + random(exp1/10);
	pot2 = exp2/5 + random(exp2/10);

//	ob1->add("combat_exp", exp1);
//	ob1->add("potential", pot1);
	tell_object(ob1, HIY"\n你顺利的使人质脱险，从而获得了"+CHINESE_D->chinese_number(exp1)+"点经验和"+CHINESE_D->chinese_number(pot1)+"点潜能的奖励。\n"NOR);
//	ob2->add("combat_exp", exp2);
//	ob2->add("potential", pot2);
	tell_object(ob2, HIY"\n你顺利的使人质脱险，从而获得了"+CHINESE_D->chinese_number(exp2)+"点经验和"+CHINESE_D->chinese_number(pot2)+"点潜能的奖励。\n"NOR);
//make a log
        log_file("job/HMY_3",sprintf("%s(%s)[exp %d]完成任务，得到奖励经验 %d ，潜能 %d。\n", ob1->name(1), ob1->query("id"), ob1->query("combat_exp"), 
     		exp1, pot1));
        log_file("job/HMY_3",sprintf("%s(%s)[exp %d]完成任务，得到奖励经验 %d ，潜能 %d。\n", ob2->name(1), ob2->query("id"), ob2->query("combat_exp"), 
     		exp2, pot2));

        ob1->delete_temp("hostage");
        ob2->delete_temp("hostage");
        ob1->apply_condition("hostage_busy",60 + random(40));
        ob2->apply_condition("hostage_busy",60 + random(40));
        
        me->delete("hostage");
        me->set("hostage/gived",0);
	remove_call_out("do_ready");
	call_out("do_ready", 100 + random(50));
        return 1;
}
