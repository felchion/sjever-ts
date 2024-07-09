#include <ansi.h>

string gb_job1()
{
	object me = this_player();
	
	if(me->query_temp("gb_job1"))
	    return "你上次的任务还没有完成，我怎么放心再让你办事。";
	command("say 刚才我接到本帮弟子易大彪飞鸽传书，西夏国主起大军十万，欲侵我大宋。");
	new(__DIR__"obj/gaoji-wenshu")->move(me); 
	message_vision("$N给$n一张告急文书。\n", this_object(), me);
	me->set_temp("gb_job1",1);
	return "你速将这份告急文书交给边关守将侯君集，请他早做防备。";
}

string gb_job2()
{
	object me = this_player(), ob = this_object();
	mixed *ob_list;
	int max,i;
	object obj1;
	
	if( me->query_temp("gb_job2") )
	     return "你不是已经接过任务了吗？";
		
	ob_list = filter_array(children(USER_OB), (: clonep($1) && !wizardp($1) :));
	max=sizeof(ob_list);
	for (i=0;i<max;i++){
	   obj1 = ob_list[i];
	   if (obj1->query_temp("gb_job2") )
	    return "这个任务我已经交给" + obj1->query("name")
		+"("+capitalize(obj1->query("id"))+")"+"去做了。";
	} 

	if( ob->query_temp("gb_job2") )
	    return "我这里现在没有什么任务可以给你。";
		
	set_temp("gb_job2",1);
	me->set_temp("gb_job2",1);
	me->set("job_name","火烧草料场");
	me->apply_condition("gb_job2",40);
	me->apply_condition("job_busy", 10);

	command("say 蒙古大军侵我大宋襄阳城，每次都以南阳为囤粮之所。\n");
	command("say 郭靖郭大侠多次派人偷袭，均因守将防守严密而失败。\n");
	command("say 前几天本帮弟子打探到南阳城北有一处断崖，地势险峻，蒙古兵防守空虚。\n");
	return "你从该处爬上去，伺机烧掉蒙古粮仓，以解我襄阳之围。";
}

string gb_job3()
{
	object me = this_player(), ob = this_object();
	mixed *ob_list;
	int max,i;
	object obj1;
	
	if( me->query_temp("gb_job3") )
	      return "你不是已经接过任务了吗？";

	if( me->query("job_name") == "刺杀敌元帅") 
	      return "您上次任务辛苦了，还是先休息一下再说吧。";

	if(me->query_condition("job_busy"))
	      return "您上次任务辛苦了，还是先休息一下再说吧。";
			    
	ob_list = filter_array(children(USER_OB), (: clonep($1) && !wizardp($1) :));
	max=sizeof(ob_list);
	for (i=0;i<max;i++){
	    obj1 = ob_list[i];
	    if (obj1->query_temp("gb_job3") )
		return "这个任务我已经交给" + obj1->query("name")
		+"("+capitalize(obj1->query("id"))+")"+"去做了。";
	}

	if( ob->query_temp("gb_job3") )
		return "我这里现在没有什么任务可以给你。";
			
	command("say 蒙古大汉蒙哥令大将粘而帖率精兵数十万，兵分两路，企图犯我襄阳。\n");
	command("say 为今之计，只有寻机刺杀蒙古大将，才能迫使蒙古大军撤围襄阳。\n");
	
	set_temp("gb_job3",1);
	me->set_temp("gb_job3",1);
	me->apply_condition("gb_job3", 50);
	me->apply_condition("job_busy", 10);
	me->set("job_name","刺杀敌元帅");
	command("say 我丐帮弟子易大彪已经潜入蒙古军中，你可去襄阳北门等他。\n");
	return  "由他带你混进蒙古大营，伺机刺杀蒙古大汉。";
}

string gb_job4()
{
	object me = this_player(), ob = this_object();
	int max,i;
	object obj1;
	object *ob_list;
	object ob2;
	
	if( me->query_temp("gb_job4") )
	      return "你不是已经接过任务了吗？";
		
	if(me->query_condition("job_busy") || me->query_condition("gb_job_busy"))
	      return "您上次任务辛苦了，还是先休息一下再说吧。";

	if( me->query("job_name") == "抗敌颂摩崖") 
	      return "您上次任务辛苦了，还是先休息一下再说吧。";
	    
	ob_list = filter_array(children(USER_OB), (: clonep($1) && !wizardp($1) :));
	max=sizeof(ob_list);
	for (i=0;i<max;i++){
	    obj1 = ob_list[i];
	    if (obj1->query_temp("gb_job4") )
	    return "这个任务我已经交给" + obj1->query("name")
		+"("+capitalize(obj1->query("id"))+")"+"去做了。";
	}

       if( ob->query_temp("gb_job4") )
	    return "我这里现在没有什么任务可以给你。";
			
       ob2 = new("/clone/npc/zhong-shentong");
       if( ob2 ){
//       if( ob2->query("winner") == me->query("id"))
//	   return "您老是当今的武林盟主，在下怎敢差遣您做事。\n";
	 destruct(ob2);
       }
    
       command("say 我刚才接到本帮弟子易大彪飞鸽传书，西夏一品堂派遣大批高手东来。\n" );
	
       set_temp("gb_job4",1);
       me->set_temp("gb_job4/asked",1);
       me->apply_condition("gb_job4",30);
       me->apply_condition("job_busy", 30);
       me->set("job_name", "抗敌颂摩崖");     
       command("say 颂摩崖是西夏武士东来的必经之地，你速带几名弟子埋伏在那里截杀。\n");
	return "这批武士中不乏高手，你多带弟子，布下阵法，以策万全，切记切记。";
}
