// eme_job1.c
#include <job_mul.h>
inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	object ob, here, where, bing, me1;
	int i, j, k, temp_number,exp,pot;
	if( duration < 1 ) {
		if(!(here = find_object("/d/emei/houdian"))) return 0;
		i = 4;
		while(i--){
			if (!(ob = me->query("pl"+ i)))continue;
			if (ob == me) continue;
			if (!userp(ob)) continue;
			j = 5;
			while(j--){
				if(!objectp(bing = ob->query_temp("em_job1/bing"+j) )) continue;
				if( bing == ob ) continue;
				bing->dest();
			}
			if ( ob->query_temp("em_job1/killed")){
				j = ob->query_temp("em_job1/killed");
				if (j > 8) j=8;
				k = 20 + random(10);
				
				exp = k*j;
				pot = k*j/5+random(k*j/10);
				exp = ob->add_exp_combat(exp,"灭绝师太","峨嵋元兵",1);
				//ob->add("combat_exp",exp );
				
				ob->add("potential",pot);
				if ( me->query("potential") > me->query("max_pot"))
					me->set("potential", me->query("max_pot"));  // added by snowman
				log_file("job/emei",sprintf("%8s%-10s峨嵋任务杀死%d人，得经验：%d，目前经验：%d。\n",
					ob->name(1),"("+ob->query("id")+")",j,k*j, ob->query("combat_exp")),ob);
				if( me->query("job_ok")){
					temp_number = 100+5*random(j);
					exp = temp_number;
					pot = temp_number/5+random(temp_number/10);
															
					exp = ob->add_exp_combat(exp,"灭绝师太","峨嵋元兵");
					//ob->add("combat_exp",exp );
					//ob->add("job_time/峨嵋元兵",1);
					//GIFT_D->check_count(ob,"灭绝师太","峨嵋元兵");
										
					ob->add("potential",pot);
					if(me->query("potential") > me->query("max_pot"))
						me->set("potential", me->query("max_pot"));  // added by snowman
					ob->add("em_job1", 1 );
					log_file("job/emei",sprintf("%8s%-10s第%d次峨嵋任务杀死%d人，得经验：%d，目前经验：%d。\n",
					 ob->name(1),"("+ob->query("id")+")",ob->query("em_job1"),j,temp_number,ob->query("combat_exp")),ob);
				}
			} 
			ob->delete_temp("em_job1");
		}
		if( me->query("job_ok")){
			CHANNEL_D->do_channel(me, "party", "恭喜诸位弟子，元兵已被赶走了！\n");
			ob = me->query("pl");
			if(ob) {
				temp_number = random(100) + 150;
				//ob->add("combat_exp", temp_number );
				temp_number = ob->add_exp_combat(temp_number,"灭绝师太","峨嵋元兵",1);
												
				ob->add("potential",temp_number/5+random(temp_number/10));
				if(me->query("potential") > me->query("max_pot"))
					me->set("potential", me->query("max_pot"));  // added by snowman
				ob->add("em_job1", 1 );
				log_file("job/emei",sprintf("%8s%-10s第%d次峨嵋任务杀死%d人，得经验：%d，目前经验：%d。\n",
				  ob->name(1),"("+ob->query("id")+")",ob->query("em_job1"),j, temp_number ,ob->query("combat_exp")),ob);
				ob->delete_temp("em_job1");
			}
		}
		else {
			if ((ob = find_object("miejue shitai")) ) {
				CHANNEL_D->do_channel(ob, "party", "元兵鞑子实力强大，现在我派人少兵稀，为保存实力，大家先撤吧！\n");
			}
		}
		me->delay_dest();
		return 0;       
	}
	i = 4;
	while(i--){
		if (!(ob = me->query("pl"+ i)))continue;
		if (ob == me) continue;
		if (!userp(ob)) continue;
		if (!objectp(where = find_object(me->query("st"+ i)))) continue;
		if ( duration == 29 || duration == 30 ){
			ob->set_temp("em_job1/bing1", ob); 
			ob->set_temp("em_job1/bing2", ob); 
			ob->set_temp("em_job1/bing3", ob); 
			ob->set_temp("em_job1/bing4", ob); 
			ob->set_temp("em_job1/bing5", ob); 
		} 
		if ( environment( ob ) != where )
			tell_object(ob, "你怎么还不到"+where->query("short")+"那里去抗击元兵，怕死吗？\n");
		else {
			if ( duration>21 && duration<29 && !ob->query_temp("em_job1/killing") ) {
				bing = new("/d/emei/npc/yuanbing");
				bing->move(where);
				bing->setskill(ob, bing);
                		tell_object(ob,"一个元兵狂叫着向你扑来。\n");
                		bing->set_leader(ob);
                		bing->kill_ob(ob);
               			ob->set_temp("em_job1/bing1", bing); 
                		ob->set_temp("em_job1/killing", 1); 
            		}
			else if ( duration>16 && duration<22 && ob->query_temp("em_job1/bing2") == ob ) {
				bing = new("/d/emei/npc/yuanbing");
				bing->move(where);
				bing->setskill(ob, bing);
				tell_object(ob,"一个元兵狂叫着向你扑来。\n");
				bing->set_leader(ob);
				bing->kill_ob(ob);
				ob->set_temp("em_job1/bing2", bing); 
				ob->set_temp("em_job1/killing", 1); 
			}
			else if ( duration>12 && duration<17 && ob->query_temp("em_job1/bing3") == ob ) {
				bing = new("/d/emei/npc/yuanbing");
				bing->move(where);
				bing->setskill(ob, bing);
				bing->set_leader(ob);
				tell_object(ob,"一个元兵狂叫着向你扑来。\n");
				bing->kill_ob(ob);
				ob->set_temp("em_job1/bing3", bing); 
				ob->set_temp("em_job1/killing", 1); 
			}
			else if ( duration>9 && duration<13 && ob->query_temp("em_job1/bing4") == ob ) {
				bing = new("/d/emei/npc/yuanbing");
				bing->move(where);
				bing->setskill(ob, bing);
				tell_object(ob,"一个元兵狂叫着向你扑来。\n");
				bing->set_leader(ob);
				bing->kill_ob(ob);
				ob->set_temp("em_job1/bing4", bing); 
				ob->set_temp("em_job1/killing", 1); 
			}
			else if ( duration>5 && duration<10 && ob->query_temp("em_job1/bing5") == ob ) {
				bing = new("/d/emei/npc/yuanbing");
				bing->move(where);
				bing->setskill(ob, bing);
				tell_object(ob,"一个元兵狂叫着向你扑来。\n");
				bing->set_leader(ob);
				bing->kill_ob(ob);
				ob->set_temp("em_job1/bing5", bing); 
				ob->set_temp("em_job1/killing", 1); 
			}
		}
	}
	me->apply_condition("em_job1", duration - 1);
	if ( me1 = LOGIN_D->find_body("keepon") )
		tell_object(me1, "Hi, Kick Keepon " + duration);
	return 1;
}

string query_type(object me)
{
    return "job";
}
