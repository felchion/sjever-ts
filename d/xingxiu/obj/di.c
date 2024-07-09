// xx job.
// by snowman@SJ 02/04/99.
// 使用 TASK_D。

#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(WHT"白玉短笛"NOR, ({ "duandi", "di",}));
	set("weight", 100);
	if(clonep()) set_default_object(__FILE__);
	else{
		set("unit", "只");
          	set("long", WHT"这玉笛短得出奇，只不来七寸来长、通体洁白，晶莹可爱。\n" NOR);
	  	set("value", 50000); 
          	set("no_get", 1);
          	set("di_number", 0);
          	set("no_drop", 1);
	}
	setup();
}

void init()
{
        add_action("mark_success", "sign");
        add_action("set_task", "blow");
        add_action("report_task", "report");
}

int set_task(string arg)
{
	object me, where; 
	mapping quest;
	me = this_player();  
	where = environment(me);

	if( !arg ) return 0;
	if( !this_object()->id(arg)) return 0;

	if( me->is_busy() || me->is_fighting()) 
		return notify_fail("你正忙着呢。\n");    

	if(this_object()->query("xx_user") != getuid(me)) 
		return notify_fail("拿着只别人的笛子吹？\n");    

	if(!(where)->query("outdoors")) 
		return notify_fail("你在屋里吹笛子，笛声怎么可能传得远呢？\n");

	if(me->query_temp("quest/family") == "丐帮")
		return notify_fail("老仙最讨厌臭叫化子，先扔掉你的丐帮帮务再说！\n");

	if(me->query_temp("quest/id")) 
		return notify_fail("你现在正在执行任务中。\n");  

	if(me->query("job_name") == "老仙分忧")
		return notify_fail("你刚要完任务，先等等吧。\n");

	if(me->query_condition("job_busy")) 
		return notify_fail("你刚要完任务，先等等吧。\n");           

	if(me->query_condition("wait_xx_task")) 
		return notify_fail("上次搞砸了，这次你就等等吧。\n");      

	tell_room(environment(me),HIY + me->name()+"拿起一只"+this_object()->query("name")+HIY"放到口边，轻轻一吹，发出一股尘锐的哨声。本来笛声清扬激越，\n"+
					"但这根玉笛中发出来的声音却十分凄厉，全非乐调。\n"NOR, ({ me }));
	tell_object(me,HIY"你拿起"+this_object()->query("name")+HIY"放到口边，轻轻吹了起来。\n"NOR);

	if((where)->query("outdoors") != "星宿") 
		return notify_fail("此地和星宿海相离太远，信鸽恐怕听不到笛声。\n");

	if(random(2)==1) {
		if(!mapp(quest = TASK_D->get_quest(me, 0, "杀", "星宿派"))) 
		quest = TASK_D->get_quest(me, 0, "找", "星宿派");
	}
	else quest = TASK_D->get_quest(me, 0, "找", "星宿派");
   	   
	if ( quest) {
		me->set_temp("quest", quest);
		me->add_busy(2);
		me->apply_condition("job_busy", 30+random(20));
		call_out("report", 3, me);
		return 1;
	}
	else {
		tell_object(me,"你现在还没有接收到任何任务。\n");
		return 1;
	}
}

int report(object me)
{
	if(!me) return 0;

	if(!me->query_temp("quest/id")) 
		return notify_fail("你现在还没有接收到任何任务。\n");
   
	message_vision(HIW"\n只见天空中飞下一只白色信鸽，降在$N的肩头。\n\n"NOR, me);
	tell_object(me, "你将书信从白鸽身上解下，瞟了一眼，信中写道：\n\t\t老仙最近的意思可能是想要");
	if(me->query_temp("quest/type") == "杀") 
		tell_object(me,"杀掉「 "+me->query_temp("quest/name")+"("+me->query_temp("quest/id")+") 」。\n\n");
	else
		tell_object(me,"得到「 "+me->query_temp("quest/name")+"("+me->query_temp("quest/id")+") 」。\n\n");
	tell_object(me,"目前情况可以通过查看任务(report)来了解。\n");
	return 1;       
}

int report_task()
{
	object me = this_player();

	if(me->is_busy() || me->is_fighting()) 
		return notify_fail("你正忙着呢。\n");
	if (!me->query_temp("quest/time") || !me->query_temp("quest/id")) 
		return notify_fail("你现在还没有接收到任何任务。\n");

	tell_object(me, "你目前的任务目标是：\n\n");

	if(me->query_temp("quest/type") == "杀"){
		tell_object(me,"\t找到曾经出现在"+me->query_temp("quest/place")+"一带的"+
		"\t\t「 "+me->query_temp("quest/name")+"("+me->query_temp("quest/id")+") 」。\n\n"+
		"杀掉他并在尸体上留下(sign corpse)我们星宿派的标志。\n");
	}
	else
		tell_object(me,"\t找到「 "+me->query_temp("quest/name")+"("+me->query_temp("quest/id")+") 」，献(xian)给老仙。\n\n");

	tell_object(me, "你必须在"+TASK_D->get_time(me->query_temp("quest/time"))+"之前完成任务！\n\n");
	return 1;
}

int mark_success(string arg)
{
	object me, target;
	string last;
	me = this_player();        
  
	if(!arg) return notify_fail("你要干什么？\n");
	if(!objectp(target = present(arg, environment(me))))
		return notify_fail("找不到这个东西。\n");
	if( me->is_busy() || me->is_fighting()) 
		return notify_fail("你正忙着呢。\n");    
	switch (TASK_D->is_target(target, me)){
		case 0:  return 0;
		case -1: tell_object(me, "耶？有让你去杀人了吗？\n");	return 0;
		case -2: tell_object(me,"笨蛋，你杀错人了！\n");	return 0;
		case -3: tell_object(me,"糟了，时间已经过了！\n");
			last = me->query_temp("quest/last_id");
			me->delete_temp("quest");
			me->set_temp("quest/last_id", last);
			me->apply_condition("job_busy", 40); 
			return 0;
		case -4: tell_object(me,"哦，这人和NPC同名？算他倒霉！\n");	return 0;
		default: break;
	}
	if(me->query_temp("quest/family") != "星宿派") 
		return notify_fail("嘿嘿，想作弊是吧？那你可要等到没有巫师的时候才行。\n");
	if(me->query("family/family_name") == "星宿派"){
		message_vision(HIR"\n$N左手一扬，衣袖中飞出一点"BLU"蓝印印"HIR"的火花，嗤嗤做响，射向$n"HIR"，着体便燃！\n\n"NOR, me, target);
		target->set("name", HIB"烧焦的尸体"NOR);
		target->set("long", HIB"一具被炼心弹烧焦的尸体，焦臭四溢，情状可怖。\n"NOR);		
	}
	else {
		message_vision("$N哼了一声，用墨笔在$n的衣袖上写下了“星宿”二字。\n", me, target);
		target->set("long", target->query("long")+"\n上面有人写了大大的 "HIB"星宿"NOR" 二字。\n");
	}
	call_out("done", 1, me);   
	me->start_busy(1);
	return 1;
}

void done(object me)
{
	if(!me) return;
	tell_object(me, "好！任务完成，你马上做了一封飞鸽传书送回星宿海。\n");
	
	TASK_D->give_reward(me, me->query_temp("quest"), -1, 0, "丁春秋" ,"星宿");
	//me->add("job_time/星宿", 1);

	me->set("job_name","老仙分忧");
	log_file("job/fenyou",sprintf("%-18s 做星宿JOB，得经验 %d，潜能 %d，目前经验：%d。\n",
		me->query("name")+"("+getuid(me)+")", me->query_temp("quest/exp_reward"),
		me->query_temp("quest/pot_reward"), me->query("combat_exp")),me);
	me->clear_condition("job_busy");
	return;
}
