#include <ansi.h>
#include <job_mul.h>

int update_condition(object me, int duration)
{
	int exp,pot,shen;
	int i;
	object *lineup;
	int lvl,index;
        
	if( duration == me->query_temp("gb_job4/time")){        
		if(!me->query_lineup()){
			lvl = me->query("max_pot");
		}
		else {
			lineup = me->query_lineup();
			lvl = 0;
			for(index=0;index<sizeof(lineup);index++){
				if( lineup[index]->query("max_pot") > lvl )
				lvl = lineup[index]->query("max_pot");
			}
			me->set_temp("gb_job4/lineup",lineup);
		}
		me->set_temp("gb_job4/lvl", lvl-100-random(6) );
		me->set_temp("gb_job4/num",sizeof( lineup ) );
	}               
        
	if( duration <= 1 && !me->query_temp("gb_job4")){
		write(HIY "你时间已过，任务失败。\n" NOR);
		log_file("job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，时间不够失败，经验：%d。",
			me->query("name"), "("+me->query("id")+")", me->query("combat_exp")),me);
		return 0;
	}
    
	if( me->query_temp("gb_job4/enter") 
	 && file_name( environment(me) ) != "/d/xingxiu/silk3" ){
		tell_object(me,HIY "你擅离职守，任务失败。\n" NOR);
		log_file( "job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，离开失败，经验：%d。",
			me->query("name"), "("+me->query("id")+")", me->query("combat_exp")),me);
		me->delete_temp("gb_job4");
		me->apply_condition("gb_job4",-1);
		me->apply_condition("job_busy", 10);
		return 0;
	}
        
	if( duration <=1
	 && me->query_temp("gb_job4/finish") > 3 
	 && file_name(environment(me)) == "/d/xingxiu/silk3" ){
		if( present( "wu shi", environment(me) ) )
		return 1;
        
		lineup = me->query_temp("gb_job4/lineup");
		if( lineup ){
			if( me->query_temp("gb_job4/num") > sizeof( lineup ) )
				i = me->query_temp("gb_job4/num");
			else
				i = sizeof(lineup);
		}
		else
			i = 1;
		if ( i == 0) 
			i=1;
        
		exp=3200/(i+1)+random(200);
		pot=exp/6+random(exp/10);
		shen = exp/2 + pot/2;
		if( exp > 2000 )
		exp = 2000 + random(100);
		
		if( !lineup ){
			       	/*奖励控制开始*/
        if (me->query("registered")==3){
			exp = exp * SMY_JOB_MUL * VIP_MUL /10000;
            		pot = pot * SMY_JOB_MUL * VIP_MUL /10000;
		}
		else
		{
			exp = exp * SMY_JOB_MUL /100;
           		pot = pot * SMY_JOB_MUL /100;
		}
		/*奖励控制结束*/ 
			
			tell_object( me, sprintf(HIW "好，任务完成了，你得到了"
					+ CHINESE_D->chinese_number( exp ) 
					+ "点实战经验，" 
					+ CHINESE_D->chinese_number( pot )
					+ "点潜能和"
					+ CHINESE_D->chinese_number( shen ) 
					+ "点正神。\n" NOR) );
			me->add( "combat_exp", exp );
			me->add( "potential", pot );
			me->apply_condition("gb_job_busy",90);
			if( me->query("potential") > me->query("max_pot"))
				me->set("potential",me->query("max_pot"));
			me->add( "shen", shen );
			me->add("job_time/报效国家",1);
                        me->add("job_time/颂摩崖",1);
			me->set("job_name", "抗敌颂摩崖");
			log_file("job/smy", sprintf(HIR"%8s%-10s颂摩崖任务结束，得到%d点经验，目前经验：%d。"NOR,
				me->query("name"), "("+me->query("id")+")", exp, me->query("combat_exp")),me);

			if( me->query_condition("fx_busy") > 50 )
				me->apply_condition("fx_busy",50);
		}
		else{
			for(i = 0;i<sizeof(lineup);i++){
				if( lineup[i] ){
					
								       	/*奖励控制开始*/
			        if (lineup[i]->query("registered")==3){
						exp = exp * SMY_JOB_MUL * VIP_MUL /10000;
			            		pot = pot * SMY_JOB_MUL * VIP_MUL /10000;
					}
					else
					{
						exp = exp * SMY_JOB_MUL /100;
			           		pot = pot * SMY_JOB_MUL /100;
					}
					/*奖励控制结束*/ 
					
					tell_object( lineup[i], sprintf(HIW "好，任务完成了，你得到了" 
						+ CHINESE_D->chinese_number( exp ) 
						+ "点实战经验，" 
						+ CHINESE_D->chinese_number( pot )
						+ "点潜能和"
						+ CHINESE_D->chinese_number( shen ) 
						+ "点正神。\n" NOR) );

					lineup[i]->add( "combat_exp", exp );
					lineup[i]->add( "potential", pot );
					lineup[i]->apply_condition("gb_job_busy", 60);
					if( lineup[i]->query("potential") > lineup[i]->query("max_pot") )
						lineup[i]->set("potential",lineup[i]->query("max_pot"));
					lineup[i]->add("job_time/报效国家",1);
					lineup[i]->add("job_time/颂摩崖",1);
					lineup[i]->add("shen", shen );
					lineup[i]->set("job_name", "抗敌颂摩崖");

					log_file("job/smy", sprintf("%8s%-10s颂摩崖任务结束，得到%d点经验，目前经验：%d。",
						lineup[i]->query("name"), "("+lineup[i]->query("id")+")",exp, lineup[i]->query("combat_exp")),lineup[i]);

					if( lineup[i]->query_condition("fx_busy") > 50 )
						lineup[i]->apply_condition("fx_busy",30+random(20));
				}
			}
		}
		me->delete_temp("gb_job4");
		return 0;
	}

	if( duration == 21
	 && file_name( environment(me)) == "/d/xingxiu/silk3" ){
		me->set_temp("gb_job4/enter",1);
		me->set_temp("gb_job4/time",random(21));
	}
        
	if( duration < 21 
	 && !me->query_temp("gb_job4/enter")){
		tell_object(me,HIY "你速度太慢，西夏武士已过颂摩崖，任务失败。\n" NOR);
		log_file( "job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，速度太慢失败，经验：%d。",
			me->query("name"), "("+me->query("id")+")", me->query("combat_exp")),me);
		me->delete_temp("gb_job4");
		me->apply_condition("gb_job4",-1);
		me->apply_condition("job_busy",10);
		return 0;
	}
    
	if( file_name(environment(me)) == "/d/xingxiu/silk3" 
	 && me->query_temp("gb_job4")
	 && ( duration == 3 
	   || duration == 9
 	   || duration == 15
	   || duration == 21 )){
		me->add_temp("gb_job4/finish",1);
		new("/d/gb/npc/xixia-wushi")->move(environment(me));
		new("/d/gb/npc/xixia-wushi")->move(environment(me));
	}
	me->apply_condition("gb_job4", duration-1);       
	return 1;
}
