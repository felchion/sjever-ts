//by cool 98.6 for dalijob
// dali_songxin.c

#include <ansi.h>
#include <login.h>

int update_condition(object me, int duration)
{
	if (duration < 2) {
		if (me->query_temp("dali_songxin")) {
			tell_object(me, HIY "你时间已过，任务失效！\n" NOR);
		}
		me->delete_temp("dali_songxin");
		me->delete_temp("songxin/name");
		me->delete_temp("songxin_zuji");
		me->delete_temp("songxin/target_name");
		me->delete_temp("letter_steal");
		me->delete_temp("songxin");
		me->delete_temp("zuji");
		me->set_temp("dali_songxin_late",1);
		if(present("secret letter", me)) destruct(present("secret letter", me));
		return 0;
	}
	
	//message("wizard:ciwei",sprintf("time : %d cond : %d \n",time,duration),users());
	
	if( time()-me->query_temp("last_go")>20+random(40) 
	&& present("secret letter", me) && living(me)
	&& !me->query_temp("songxin_zuji")
	&& interactive(me) && !environment(me)->query("no_fight")
	&& !me->query_temp("letter_steal")) {
		tell_object(me, HIY "你觉得有些不妙，似乎被人跟踪上了！\n" NOR);
		me->add_busy(1);
		call_out("zuji", 1, me);
	}

	me->apply_condition("dali_songxin", duration - 1);
	return 1;
}

void zuji(object me)
{
	object obj;
	object obj2;

	if (!me) return;
	obj2 = present("secret letter", me);
	
        
	if (obj2) {
		tell_object(me,"你隐约感觉到有人围了过来。。!!! \n");
		me->set_temp("songxin_zuji", 1);
		if (me->query_temp("songxin/twice")){		
			obj = new("/d/dali/job/robber");
			obj->set("can_guard_ob",1);//added by Ciwei@SJ 路见不平 拔刀相助
			obj->set("target", me);
			obj->set_temp("target", me->query("id") );
	        	obj->setparty(me->query("bonus/送信"),me->query("max_pot")-100,me->query("combat_exp"));
	        	tell_object(me, RED "你紧张起来，不由一摸衣袋，不好！信被偷了!\n" NOR);
	        	tell_object(me, "你定睛一看，原来是"+obj->query("name")+"，而且此人武功极高，似乎用的是"+obj->query("family/family_name")+"的"+to_chinese(obj->query("f_w_skill"))+"！\n"NOR);
			obj->move(environment(me));
			obj2->move(obj);
			return;
        	}
                if (random(10)> 8) {
			//this is killer kill npc
			tell_object(me, RED "糟糕！有人要抢信!\n" NOR);
			obj = new("/d/dali/job/npc1");
			obj->set("target", me);
			obj->move(environment(me));
			if (random(me->query("combat_exp")) >= 300000) call_out("killer_clone", 1, me);
		} else {
			//this is stealer steal letter
			//destruct(obj);
			tell_object(me, RED "你紧张起来，不由一摸衣袋，不好！信被偷了!\n" NOR);
			obj = new("/d/dali/job/snpc1");
			obj->set("target", me);
			obj->move(environment(me));
			obj2->move(obj);
			if (me->query("combat_exp") >= 300000) call_out("stealer_clone", 1, me);
		}
	} else tell_object(me, "信已送出，你庆幸的躲过了一次追杀!\n");   
}

void killer_clone(object me)
{
	object obj;

	if (!me) return;
	tell_object(me, YEL"糟糕，又冲上来了个人！\n");
	
		obj = new("/d/dali/job/npc2");
		obj->set("target", me);
		obj->move(environment(me));
}

void stealer_clone(object me)
{
	object obj;

	if (!me) return;
	tell_object(me, YEL"糟糕，又冲上来了个人！\n");
		obj = new("/d/dali/job/snpc2");
		obj->set("target", me);
		obj->move(environment(me));
}

string query_type(object me)
{
	return "job";
}
