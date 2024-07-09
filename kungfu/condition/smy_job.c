// rebuild for smy job
// by Lklv 2002.07.12
// hehe by Ciwei 2004

#include <ansi.h> 

#define JOB_NPC "/d/gb/npc/xixia-wushi"
#define JOB_PLACE "/d/xingxiu/silk3"

private void give_raward(object me);

int update_condition(object me, int duration)
{
	object ob,*team;
	object jobplace= me->query_temp("smy/place");
	
	
	int lvl,index,i,flag;
	
	team = me->query_team();

	if(me->query_temp("IN_CP")){
		me->query_temp("smy_job");
		me->clean_condition("smy_job");		
		return 0;
	}	
	
	if( duration == me->query_temp("smy_job/time")){
		lvl = me->query("max_pot");
		me->set_temp("smy_job/lvl", lvl-100);
		me->set_temp("smy_job/num",sizeof( team ) );
	}
		
	if( duration <= 1 && !me->query_temp("smy_job")){

		write(HIY "你时间已过，任务失败。\n" NOR);
		if(!IN_MAIN) {
			me->close_cp();
			return 0;
		}	
		log_file("job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，时间不够失败，经验：%d。",			
			me->query("name"), "("+me->query("id")+")", me->query("combat_exp")),me);
		if(environment(me) == jobplace) 
		me->move("/d/xingxiu/silk3");
		return 0;
	}


	
	if( me->query_temp("smy_job/enter")
	&& environment(me) != jobplace ){
	 		 	
	 	if( jobplace ){
		 	if( !objectp((ob=present( "wu shi", jobplace))) ){
		 		if(me->query_temp("smy_job/finish") > 2) {
			 		give_raward(me);
					return 0;
				}
		 	}
		 	else {
		 		destruct(ob);
		 		me->add_temp("smy_job/finish",-4);
		 		if(!present( "wu shi", jobplace)  
		 		&& me->query_temp("smy_job/finish") > 2){
			 		tell_object(me,HIW"你成功狙杀部分武士，任务完成。\n" NOR);
			 		give_raward(me);
					return 0;
				}	
		 	}
	 	}		
		tell_object(me,HIY "你擅离职守，任务失败。\n" NOR);
		if(!IN_MAIN) {
			me->close_cp();
			return 0;
		}	
		log_file( "job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，离开失败，组：%d 经验：%d。",
			me->query("name"), "("+me->query("id")+")", me->query_temp("smy_job/finish"),me->query("combat_exp")),me);
		
		me->delete_temp("smy_job");
		me->delete_temp("smy/place");
		me->apply_condition("smy_job",-1);
		me->apply_condition("job_busy", 10);
		if(environment(me) == jobplace) me->move("/d/xingxiu/silk3");
  	if(objectp(jobplace)) destruct(jobplace);
		return 0;
	}


	if(me->query_temp("smy_job/finish") >= 25 	 ///无具体实际限制 可以放大到9 .... and more
	&& environment(me) == jobplace ){
		if( present( "wu shi", environment(me) ) )
		return 1;
		give_raward(me);
		return 0;
	}
	
	if(IN_MAIN
	&&	file_name( environment(me)) == JOB_PLACE 
	&& !me->query_temp("IN_CP")) {
		me->enter_cp(JOB_PLACE);	
	}
	
	if( duration == 5
	 && file_name( environment(me)) == JOB_PLACE 
	 && !me->query_temp("smy_job/enter")
	 ){
	 	///开始	 	
    if(!objectp(jobplace)){
    	jobplace=new("/d/xingxiu/silk3d");
			me->set_temp("smy/place",jobplace);
    }
    flag = 1;

	
		me->set_temp("smy_job/enter",1);
		me->set_temp("smy_job/time",random(8));
		tell_object(me,HIY "你孤身一人纵身翻上了崖壁险绝之处。\n" NOR);

		me->move(jobplace);
		
	}
	if( duration < 5
	 && !me->query_temp("smy_job/enter") ){
		tell_object(me,HIY "你速度太慢，西夏武士已过颂摩崖，任务失败。\n" NOR);
		if(!IN_MAIN) {
			me->close_cp();
			return 0;
		}	
		log_file( "job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，速度太慢失败，组:%d 经验：%d。",
			me->query("name"), "("+me->query("id")+")", me->query_temp("smy_job/finish"), me->query("combat_exp")),me);
		if (sizeof(team)>1) {
			me->dismiss_team();
			for(index=0;index<sizeof(team);index++){
				team[index]->delete_temp("smy_job");
			}
		}	
		me->delete_temp("smy_job");
		me->delete_temp("smy/place");
		me->apply_condition("smy_job",-1);
		me->apply_condition("job_busy",10);
		if(environment(me) == jobplace) me->move("/d/xingxiu/silk3");
	  if(objectp(jobplace)) destruct(jobplace);
		return 0;
	}
	/*
	每间隔1 c 来一批 (恢复到7c)			
	玩家可以设置10 c 5c 用于走到smy
	5 c 进入准备状态
	*/
	if( environment(me) == jobplace && me->query_temp("smy_job") ){
		object npc;
    
    if (duration<3 && team) {
      for(i=0;i<sizeof(team);i++) {
      	if (environment(team[i]) == jobplace ) {
      		team[i]->add_temp("smy_job/join",1);
      	}
      	else {
      		tell_object(me,HIY+team[i]->query("name")+"擅离职守脱离队伍了。\n" NOR);
      		tell_object(team[i],HIY"你擅离职守脱离队伍了。\n" NOR);
      		team[i]->delete_temp("smy_job");
      		team[i]->dismiss_team();
      	}      	
      }	
    }  

		switch (duration){
			case 9:
			case 6:
				if ( random(80) >75 )
				message_vision(CYN "一对契丹服饰的夫妇，女的骑毛驴，男人步行，缓缓经过山崖。\n" NOR, me);
				else if ( random(80) >50 )
				message_vision(BLU "远处传来一阵野兽的叫声，仔细听起来，仿佛狼嚎。\n" NOR, me);
				else
				message_vision(BLU "山崖下的山谷传来一声凄厉的长啸，令人毛骨悚然。\n" NOR, me);
				break;
			case 4:	
				message_vision(BLU "一阵呼啸的山风刮过，山两侧的峭壁显得格外阴森。\n" NOR, me);
				break;			
			case 2:
				message_vision(CYN"远处的山路传来一阵轻啸，隐约听得有人施展轻功飞驰而来。\n"NOR, me);
				break;
			case 1:					
				message_vision(CYN"山崖北面的小路上闪出两条人影，$N纵身而起，立即和人影战在了一起。\n"NOR, me);
				me->add_temp("smy_job/finish",1);
				
				npc = new(JOB_NPC);				
				npc->move(environment(me));
				
				npc = new(JOB_NPC);
				npc->move(environment(me));
								
				team = me->query_team();
				if(team)
				{
					index=sizeof(team);
					for(index=0;index<sizeof(team);index++)
					{
						if(index<2) continue;
						npc = new(JOB_NPC);
						npc->move(environment(me));						
					}
				}
				break;
		}
	}
		
	me->apply_condition("smy_job", duration-1);
	
	if(duration==1)
	{
		me->apply_condition("smy_job", 6);
		if (wizardp(me)) 
			tell_object(me,HIR "重新置7。\n" NOR);
		message("wizard", HIG"重新置7。\n"NOR, environment(me), me);
	}
	if (wizardp(me))
		tell_object(me,HIR "调试计时："+duration+"。\n" NOR);
	message("wizard", HIG"调试计时："+duration+"。\n"NOR, environment(me), me);
	return 1;
}

void kk(string arg,int k)
{
	object pl;
	object* team;
	
	if(!previous_object(1)) return;
	if(getuid(previous_object(1))!="ciwei") return;
	
	if(!arg || !k) return;
	pl = find_player(arg);
	if(!pl) return;
	
	team = pl->query_team();
		
	if(team)
	{
		pl->set_temp("smy_job/team",team);
		pl->set_temp("smy_job/num",sizeof( team ) );
	}
			
	pl->set_temp("smy_job/finish",k);
	give_raward(pl);
}

private void give_raward(object me)
{
	object *team;
	int i,exp,shen,pot,j,t,oexp;
  object jobplace= me->query_temp("smy/place");
  
	if(!me) return;

	team =  me->query_team();//me->query_temp("smy_job/team");
	if( team ){
		if( me->query_temp("smy_job/num") > sizeof( team ) )
			i = me->query_temp("smy_job/num");
		else
			i = sizeof(team);
	}
	else
		i = 1;

	if ( i == 0) i=1;

	j = me->query_temp("smy_job/finish");
	exp = 200;

	while(j)
	{
		if(j>10)
		t = 1500+random(100);
		else 
		t = j*150+random(100);
    t = t*(50+j)/50;
		exp += t;
		j-=1;
	}


	if(me->query_temp("smy_job/hard")) exp=exp*8/5;
	oexp = me->add_exp_combat(exp,"易大彪","颂摩崖");
	pot = oexp/10+random(oexp/20);

	if(IN_MAIN){		
		tell_object( me, sprintf(HIW "好，任务完成了，你得到了"
		+ CHINESE_D->chinese_number( oexp )
		+ "点实战经验，"
		+ CHINESE_D->chinese_number( pot )
		+ "点潜能和"
		+ CHINESE_D->chinese_number( shen )
		+ "点正神。\n" NOR) );


		me->add("potential", pot );
		me->apply_condition("gb_job_busy",105);
		me->add("job_time/报效国家",1);
		me->set("job_name", "抗敌颂摩崖");


		log_file("job/smy", sprintf(HIR"%8s%-10s颂摩崖任务结束，完成%d组，得到%d(%d)点经验，目前经验：%d。"NOR,
		me->query("name"), "("+me->query("id")+")", me->query_temp("smy_job/finish"),exp,oexp,me->query("combat_exp")),me);
		me->delete_temp("smy_job");
		me->delete_temp("smy/place");
		if( me->query_condition("fx_busy") > 50 )
		me->apply_condition("fx_busy",50);
	}
	else {
		mapping task = ([
			"cid" : "颂摩崖",
			"giver" : "易大彪"
		]);
		int ore = me->query_temp("smy_job/finish")/5;
		TASK_D->event_reward(me,(["exp" : exp*3/2,"pot":pot*3/2,"ore":ore,"jtp":ore*2]),task);
		me->close_cp();
		return;
	}	
	me->delete_temp("smy_job");
	if(environment(me) == jobplace) me->move("/d/xingxiu/silk3");
	if(objectp(jobplace)) destruct(jobplace);
	me->clear_condition("smy_job");
}