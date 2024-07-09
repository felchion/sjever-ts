// yang.c 
// Modified by Numa 19991024

#include <ansi.h>
inherit NPC;
#include "hmy_def.c"

void create()
{
        set_name("杨莲亭", ({ "yang lianting", "yang",}));
        set("long", "此人身形魁梧，满脸胡须，形貌极为雄健
威武，可下盘虚浮，看来无甚内功。\n");
        set("title",HIY"日月神教  总管"NOR);
        set("gender", "男性");
        set("age", 20);
        set("attitude", "peaceful");
        set("shen_type", -1);
        set("str", 22);
        set("int", 22);
        set("con", 21);
        set("dex", 19);
        
        set("max_qi", 1800);
	set("qi", 1800);
	set("jing", 1300);
        set("max_jing", 1300);
        set("neili", 2000);
        set("max_neili", 2000);
	set("eff_jingli", 1200);
	set("jingli", 1200);

        set("jiali", 50);
        set("combat_exp", 600000);
        set("shen", -5000);

	set_temp("apply/attack", 40);
	set_temp("apply/strength", 40);
	set_temp("apply/defence", 40);
	set_temp("apply/dexerity", 40);

        set_skill("tianmo-jian",170);
	set_skill("parry", 170);
	set_skill("sword", 170);
        set_skill("dodge",170);
	set_skill("ding-dodge", 170);
	set_skill("force", 170);
	set_skill("tianmogong", 170);

        map_skill("sword", "tianmo-jian");
        map_skill("dodge", "ding-dodge");
        map_skill("force", "tianmogong");
	map_skill("parry", "tianmo-jian");
        setup();
 
	carry_object("/clone/weapon/changjian")->wield();
	carry_object("/clone/misc/cloth")->wear();        

}

void init()
{
        object ob;
        
        ::init();
        
        if( interactive(ob = this_player()) )
        {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}

void greeting(object ob)
{
        object me = this_object();
        
        if (!ob || !present(ob, environment())) return;
// 判断 player        
        if (ob->query_temp("hostage/job_2") == 2
        &&  me->query_temp("target") == ob->query("id")) {
                message_vision(CYN"$n对着$N大喊：“"+ RANK_D->query_rude(ob) +"，你在这里干什么？”\n"NOR,ob,me);
                command("look " + ob->query("id"));
                ob->start_busy(random(2));
                remove_call_out("do_kill");
                call_out("do_kill", 0, me, ob);
        }
        else if (ob->query_temp("hostage/job_3") == 1
        &&  me->query_temp("target2") == ob->query("id"))
        {
                command("look " + ob->query("id"));
                command("shout " + ob->query("id"));
                ob->start_busy(random(2));
                remove_call_out("do_kill2");
                call_out("do_kill2", 0, me, ob);
        }
}

int do_kill(object me,object ob)
{
// kill player        
        remove_call_out("kill_ob");
        call_out("kill_ob", 0, ob);
        return 1;
}

int do_kill2(object me,object ob)
{
// kill player        
        remove_call_out("kill_ob");
        call_out("checking",1,me,ob);
        call_out("kill_ob", 0, ob);
        return 1;
}

int checking(object me, object ob)
{
	object room,npc;
        int my_max_qi,his_max_qi;
        my_max_qi  = (int)me->query("max_qi");
        his_max_qi = (int)ob->query("max_qi");
        if (me->is_fighting())
        {
                call_out("checking",1, me, ob);
                return 1;
        }
        if ( !present(ob, environment()) )
        	return 1; 
        if (( (int)me->query("qi")*100 / my_max_qi) <= 90 )
        {
		tell_room(environment(me),"杨莲亭一看势头不对，转身向着西边的小舍夺路逃去。\n");
		message_vision("$N也立刻追赶上去。\n",ob);
            	if (me->query_temp("block_msg/all")>0)
            	{
            		me->remove_call_out("revive");
            		me->revive();
            	}
            	me->reincarnate();
         	room = load_object(__HMY__"xiaoshe");
         	if (!objectp(npc=present("dongfang bubai",room)))
         	{
         		npc=new(__DIR__"dongfang");
         		npc->move(room);
         	}
         	me->move(room);
         	ob->move(room);
         	tell_room(room,CYN"东方不败尖声道：“莲弟，你……你……怎么了？是他把你打伤了吗？”\n"NOR);
         	tell_room(room,"东方不败从身边摸出一块绿绸手帕，缓缓替杨莲亭拭去额头的汗水和泥污。\n");
         	tell_room(room,CYN"杨莲亭怒道：“大敌当前，你跟我这般婆婆妈妈干甚么？你能打发得了敌人，再跟我亲热不迟。”\n"NOR);
         	tell_room(room,CYN"东方不败微笑道：“是，是！你别生气，腿上痛得厉害，是不是？真叫人心疼。”\n"NOR);
         	tell_room(room,CYN"杨莲亭一转身，躲了起来。\n");
         	destruct(me);
         	ob->apply_condition("hostage_3",60);
         	npc->do_kill(ob);
            	return 1;
        }
        if (( (int)ob->query("qi")*100 / his_max_qi) < 50 )
        {
                message_vision(CYN"$N看了看$n，说道：你的功夫稀松平常，居然还敢到黑木崖来撒野？\n"NOR, me,ob);
                return 1;
        }
        return 1;
}

int accept_fight(object ob)
{
        command("say " + RANK_D->query_rude(ob) + "，你别妨碍我办事。");
        return 0;
}

void die()
{
        object ob = this_object(),me;
        object obj;
//        int i,j;
        
        me = query_temp("last_damage_from");

        if (me->query("id") != ob->query_temp("target")) {
                ob->revive(1);
                command("say 好，居然还有" + me->name(1) + "帮你，杀了我，你也拿不到钥匙了！");
                destruct(ob);
                return;
        }
        else {
        	ob->revive(1);
        	command("say 你不杀我，我这把钥匙可以给你，算是交换。");
		obj = new(__HMY__"obj/key");
		obj->set_temp("owner",me->query("id"));
		obj->move(ob);
		command("give key to " + me->query("id"));
		"/d/hmy/npc/yingying.c"->give_reward_2(ob);
        	command("escape");
       		destruct(ob);
       		return;
	}
	        
        ::die();
}
