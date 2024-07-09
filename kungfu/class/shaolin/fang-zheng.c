// Modify by YUJ@SJ 2000-11-12
// Modify by LOOKLOVE@SJ 2001-1-8
// 反映难度过大 加长生存时间  顺便fix几个bug Ciwei@SJ
#include <ansi.h>
#include <job_mul.h>

inherit NPC;

int ask_job();

void create()
{
	set_name("方正大师", ({ "da shi", }));
	set("long", "他是一位身材矮小的老年僧人，容颜瘦削，神色慈和，\n"
		"也瞧不出有多少年纪。\n");
	set("nickname", "少林长老");
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");
	set("muren_winner",1);
	set("no_get","你膂力不够，背不起来。");
	set("no_ansuan",1);
	
	set("age", 60+random(20));
	set("shen", 1);
	set("str", 20);
	set("int", 21);
	set("con", 20);
	set("dex", 20);
	set("max_qi", 6500);
	set("max_jing", 3000);
	set("eff_jingli",5000);
	set("max_neili", 8000);
	set("jiali", 40);
	set("combat_exp", 2000000);

	set_skill("force", 180);
	set_skill("yijin-jing", 180);
	set_skill("dodge", 220);
	set_skill("shaolin-shenfa", 180);
	set_skill("parry", 180);
	set_skill("finger",180);
	set_skill("yizhi-chan",190);
	set_skill("buddhism", 180);
	set_skill("literate", 100);
	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("finger", "yizhi-chan");
	map_skill("parry", "yizhi-chan");
	set_temp("sljob", 1);

	prepare_skill("finger", "yizhi-chan");
	create_family("少林派", 35, "弟子");

	set("inquiry", ([
		"救援" : (: ask_job :),
	]));

	setup();

	carry_object("/d/shaolin/obj/xuan-cloth")->wear();

	if (clonep()) call_out("remove_ob",2000);
}

void remove_ob()
{
	//可能有些扔下找不到的dashi就再去askjob 防止玩家损失 这里就不delete_temp了
	/*
	object me;
	int teams;
	teams=query("teams");
	for(int i=1;i<=teams;i++) {
			me = query_temp("team" + i);
			if(!me) continue;
			me->delete_temp("sljob");
	}
	*/
	message_vision(HIR "$N不耐烦地说：怎么这么久还不到恒山，老衲自己先去了。\n" NOR, this_object());
	destruct(this_object());
}

void init()
{
	object me = this_player();

	::init();
	if (me==query("jobleader")) {
		me->add_busy(1+random(2));
		call_out("init2", 2, me);
	}
}

void init2(object me)
{
	object ob=this_object();
	object ob1,ob2;
	int i,teams;
	int exp,pot;
	int killed;
	mapping exits;
	string *dirs;
	object env;
	int j;
	int k,m,n;
	object *envs;
	object *envs2;

	if (!me) return;
	if (me==query("jobleader")
	&& file_name(environment()) == "/d/hengshan/baiyunan") {
		if( !me->query_temp("sljob") ) {
			log_file( "job/husong", sprintf("%8s%-10s做少林“救援恒山”任务出现BUG。\n",
				me->query("name"), "("+me->query("id")+")" ),me);
			call_out("remove_obj",1);
			return;
		}
		command("say 好了，历尽艰险，终于到了恒山。");
		command("say 你们先回去告诉方丈，我和定闲师太一起抵抗魔教的进攻。");
		CHANNEL_D->do_channel(ob, "rumor", sprintf("%s"HIR"护送大师"NOR"顺利到达恒山了！", me->query("name")));
		teams=query("teams");
		for(i=1;i<=teams;i++) {
			ob1 = query_temp("team" + i);
			if(ob1 && present(ob1)) {
				exp = 1000/teams + 500;
				killed = ob1->query_temp("sljob/killed");
				if (killed > 6) killed = 6;
				exp += killed * 100+random(killed*50);
				if (query_temp("sljob/total") - ob1->query_temp("sljob/killed") > 0) {
					exp -= (query_temp("sljob/total")-ob1->query_temp("sljob/killed"))*100;
					if (exp < 100) exp=100+random(100);
				}
				if (exp > 1500) exp = 1400 + random(100);
				if (!ob1->query_temp("sljob/killed")) exp = 100 + random(50);
				if ( ob1->query("sl_job") > 1000 && teams < 2 ) exp /= 5;
								
				exp = ob1->add_exp_combat(exp,this_object(),"少林救援");
				//ob1->add("combat_exp", exp);
				//GIFT_D->check_count(ob1,this_object(),"少林救援");
				//ob1->add("job_time/少林救援",1);
				pot = exp/6+random(exp/10);
				
				ob1->add("potential", pot);
				if (ob1->query("potential") > ob1->query("max_pot"))
					ob1->set("potential", ob1->query("max_pot"));
				ob1->add("job_time/少林",1);				
				
				ob1->apply_condition("sljob", exp/60 + random(7));
                                ob1->set("job_name", "恒山救援");
                                ob1->apply_condition("sl_job_busy", 60);
				ob1->apply_condition("job_busy",2);
				tell_object(ob1,HIW"好，任务完成，你得到了" 
					+ CHINESE_D->chinese_number( exp ) 
					+ "点实战经验和" 
					+ CHINESE_D->chinese_number( pot )
					+ "点潜能。\n" NOR);
				
				log_file( "job/husong", sprintf("%8s%-10s"
					+"，第%5d次，%2d个杀手，死%2d个，"
					+ "奖励%4d点，目前经验：%d\n", ob1->query("name"),"("+ob1->query("id")+")",
					ob1->query("job_time/少林"),query_temp("sljob/total"),
					ob1->query_temp("sljob/killed"),exp, ob1->query("combat_exp")),ob1);
				ob1->delete_temp("sljob");
				// add for sl wuxiang-zhi's quest
				if (ob1->query("family/family_name")=="少林派" 
				&& ob1->query("job_time/少林救援") > 1000
				&& ob1->query("combat_exp") > 2000000
				&& random(ob1->query("combat_exp")) > 1500000
				&& random(ob1->query_skill("shaolin-shenfa",1)) > 200
				&& ob1->query_skill("buddhism",1) > 180
				&& random(ob1->query_skill("buddhism",1)) < 20
				&& !ob1->query_temp("wxz/quest")
				&& ob1->query("wxz/quest") < 2) {
					command("whisper "+ob1->query("id")+" 刚才我碰到方生师弟，他似乎有一些"
						+"关于少林武技的想法要告诉你，请你马上去他那里一趟。");
					ob1->set_temp("wxz/quest",1);
					ob1->set("wxz/quest",1);
					log_file("quest/wuxiang", sprintf("%8s%-18s第%5d次护送任务时，获得学习无相劫指的机会。\n",
						ob1->query("name"), "("+ob1->query("id")+")", ob1->query("job_time/少林")), ob1);
				}
			}
		}
		call_out("remove_obj",1);

		i=random(teams+1);
		ob1=query_temp("team"+i);
		if (ob1 && present(ob1) && random(10)> 8 ) {
			tell_object(ob1,HIG"你从"+query("name")
				+"那里得到了一颗六阳正气丹。\n" NOR);
			ob2=new("/clone/medicine/neili/liuyang-dan.c");
			if( ob2 ) {
				ob2->set("owner",ob1->query("id"));
				ob2->move(ob1);
			}
			log_file("quest/neili", sprintf("%-18s护送任务结束，得到六阳正气丹。\n",
				 ob1->query("name")+"("+ob1->query("id")+")" ), ob1);
		}
		return;
	}

	if(me==query("jobleader") && me->query_temp("sljob/asked")>=2
	&& !environment(me)->query("no_fight")) {
		if (add("neili",-(query("max_qi")-query("eff_qi"))/2) >= 0) {
			set("eff_qi",query("max_qi"));
			set("qi",query("max_qi"));
		}
		if ((add_temp("sljob/go", 1) > 5 && !random(5))
		|| query_temp("sljob/go") > 8) {
			set_temp("sljob/go",0);
			set_heart_beat(0);
			set_heart_beat(1);
			ob1=new("/d/shaolin/npc/killer");
			message_vision(HIR "突然从路边冲出一个魔教教徒，一声不吭的向"
				+query("name")+"冲去。\n"NOR, ob);
			ob1->set("lvl",query("lvl"));
			ob1->set("dashi",ob);
			ob1->set("int", me->query_int());
			ob1->set_name(ob1->query("name"), ({ me->query("id") + "'s jiaotu" }));
			ob1->move(environment(me));
			ob1->set_leader(ob);
			add_temp("sljob/total",1);
			if (!random(20)) {
				message_vision(HIY"" + query("name")+"高喊一声：情况紧急，我撤退，你掩护。\n"NOR,me);
				envs=({file_name(environment(me))});
				for(m=0;m<2;m++) {
					envs2=envs;
					for(n=0;n<sizeof(envs);n++) {
						exits=envs[n]->query("exits");
						dirs=keys(exits);
						for(k=0;k<sizeof(dirs);k++) {
							env=envs[n]->query("exits/"+dirs[k]);
							if(env&&!env->query("no_fight")&&!env->query("sleep_room")) {
								for(j=0;j<sizeof(envs2);j++) if(envs2[j]==env) break; 
								if(j==sizeof(envs2)) envs2=envs2+({env});
							}
						}
					}
					envs=envs2;
				}
				me->set_temp("env",envs);
				env=envs[random(sizeof(envs))];
				move(env);
				ob1->move(env);
			}
		}
	}
}

void remove_obj()
{
	destruct(this_object());
}

int ask_job()
{
	object me=this_player();
	object ob=this_object();

	if( query("jobleader")!=me) return 0;

	if( me->query_temp("sljob/asked")==0 ) return 0;

	if( me->query_temp("sljob/asked")>1) {
		command( "say 你刚才不是问过了吗？快去吧。");
		return 1;
	}
	command("say 好的，就请这位" + RANK_D->query_respect(me) + "在前面带路，前去恒山。" );
	message_vision("$N决定跟随$n一起行动。\n",ob,me);
	set_leader(me);
	set("sljob/leader",me);
	me->set_temp("sljob/asked",2);
	return 1;
}

void attempt_apprentice(object ob)
{
	command("say 对不起，老衲现在不收徒。");
}

void unconcious()
{
	object me;
	
	me = query_temp("last_damage_from");

	if (!objectp(me)) {
		::unconcious();
		return;
	}

	if (userp(me)) {
		reincarnate(); 
		return;
	}
	::unconcious();
}

void die()
{
	object me;
	int teams;
	
	teams=query("teams");
	for(int i=1;i<=teams;i++) {
			me = query_temp("team" + i);
			if(!me) continue;
			me->delete_temp("sljob");
	}
	me=query("jobleader");
	if( !me ) {
		::die();
		return;
	}
	tell_object( me, HIY""+query("name")+"不幸阵亡，你任务失败。\n"NOR);
	log_file("job/husong", sprintf("%8s%-10s任务失败。\n", me->query("name"),"("+me->query("id")+")"), me);
	me->apply_condition("sljob",10);
	me->apply_condition("job_busy", 10);
	me->delete_temp("sljob");
	::die();
}

void kill_ob(object ob)
{
	if (userp(ob))
	{
		ob->remove_killer(this_object());
		remove_killer(ob);
		message_vision("$N心中忽然觉得：“自己怎么这么无耻，居然对这样德高望重的大师下此毒手？”\n", ob);
	}
}