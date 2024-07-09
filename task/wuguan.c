//测试
inherit TASK;
#include <task.h>


int task_on() {return 0;}
string *names = ({ "锯木头","打扫马房", "劈柴", "灌溉菜地", "锄草", "挑水"});
string *address = ({ "木房","马厩", "柴房", "菜园", "菜地", "水房"});

mixed query(string arg){
  if(arg=="name") return task["name"];
  if(arg=="id") return __FILE__;
}

string wg_tool_rst(object me){
  if(me->query("gender")=="无性")
  return "公公不能做这个任务。";
  
  return 0;  
}

mapping task = ([
    "name"      : "武馆打杂",
    "cid"       : "武馆",
    "on"        : 1,
    "id"        : "wuguan",
    "start"     : "wg_start",
    "hardness"  : 0,    
    "RPTL"      : 1,
    "offline"   : 0,
]);

mapping events = ([
  "wg_start" : ([
    "name"   : "询问工作",
    "npc"    : "/d/wuguan/npc/dizi1",
    "type"   : TYP_TALK,
    "event"  : "wg_start",    
    "next"   : "wg_tool",
    "rst"    : ([
      "combat_exp'>'3000" : "你功夫已经挺高了，这里没有什么适合你的工作了。",  
  //    "gender'=='男性"    : "这个任务只有女性才能完成。",
    ])    
  ]),
  "wg_tool" : ([
    "prev"   : "wg_start",
    "name"   : "借用工具",
    "npc"    : "/d/wuguan/npc/dizi6",
    "type"   : TYP_TALK,
    "event"  : "wg_tool",
    "hint"   : "到储物间和吴坎交谈取得工具。",
    "rst"    : (: wg_tool_rst :), 
    "next"   : ({"wg_work"})
  ]),
  "wg_work" : ([
    "pre"    : "wg_tool",
    "name"   : "武馆工作",
    "type"   : TYP_ACTION,
    "event"  : "wg_job",
    "cmds"   : ({"sao","ju","pi","jiao","chu","tiao"}),
    "next"   : "wg_re_tool"
  ]),
  "wg_re_tool" : ([
    "pre"    : "wg_work",
    "npc"    : "/d/wuguan/npc/dizi6",
    "name"   : "归还工具",
    "type"   : TYP_ACTION,
    "event"  : "wg_re_tool",
    "cmds"   : ({"give","huan"}),
    "next"   : "wg_finish"
  ]),
  "wg_finish" : ([
    "pre"    : "wg_re_tool",
    "name"   : "领取奖励",
    "npc"    : "/d/wuguan/npc/dizi1",
    "type"   : TYP_TALK,
    "reward"   : ([
      "exp"    : 100,
      "gold"   : 1,
      "pot"    : 10,
      "item"   : ({"/clone/weapon/whip","/clone/weapon/axe"})
    ]),
    "event"  : "wg_reward"
  ])
]);

void create() {
  set_events(events);
  set_task(task);
  setup();
}

varargs int wg_start(mixed arg,object ob,object me){
	int i;
	string target;
	
	if(userp(arg)) {
	  string msg=HIG+ob->name()+"说道：不知道这位"+RANK_D->query_respect(arg)+"可否帮我一个小忙。["HIY"Y|n"HIG"]："NOR;
	  event_choice(msg,ob,arg);
	  return STAT_CNT;
	}
	else if(arg[0]=='y' || arg[0]=='Y') {
  	i = random(sizeof(names));
  	target = names[i];
  	set_flag(me,"job_id",i+1);
	  pre_set_hint(me,"wg_tool","到"HIY"物品房"NOR"和"HIG"吴坎"NOR"交谈。");
	  tell_object(me, CYN+ob->name()+"说道：「最近武馆很忙，你先在物品房领好工具，然后去"+ address[i] +target+ "吧。」\n"NOR);
  	return STAT_RST+STAT_PROG+STAT_END;
	}
	else {
	  tell_object(me,CYN+ob->name()+"说道：那真是太遗憾，希望"+RANK_D->query_respect(me)+"有空时，能为"+RANK_D->query_self(ob)+"分忧。\n");
	  return STAT_ACT;
	}
}

int wg_tool(object ob,object me)
{
	object tools;
	int i;

	if(check_flag(ob,"工具")){
	  tell_object(ob, CYN+me->name()+"说道：你已经领了工具了，还没还呢，怎么又来领了？");
		return 0;
	}
	switch(i=check_flag(ob,"job_id")){
		case 5:	tools = new("/d/wuguan/npc/obj/chutou"); break;
		case 4: tools = new("/d/wuguan/npc/obj/piao"); break;
		case 1:	tools = new("/d/wuguan/npc/obj/juzi"); break;
		case 3:	tools = new("/d/wuguan/npc/obj/chaidao"); break;
		case 6:	tools = new("/d/wuguan/npc/obj/shuitong"); break;
		case 2: tools = new("/d/wuguan/npc/obj/saozhou"); break;
		default: 
		  tell_object(ob,check_flag(me,"job_id")+CYN+me->name()+"说道：「你跟大师兄领的什么活？我这里没这种工具。」\n"NOR);
		  return 0;
	}
	set_flag(tools,"mark_name", ob->query("id"));
	tools->move(ob);
	set_flag(ob,"tools_name", tools->query("id"));
	tell_object(ob, CYN+me->name()+"说道：「既然你已经领了活，我就给你这把工具，小心保管，别弄丢了。」\n"NOR);
	pre_set_hint(ob,"wg_work","到"HIY+address[i-1]+NOR"完成"+HIR+names[i-1]+NOR"工作。");
	message_vision("$N交给$n一"+tools->query("unit") + tools->query("name")+"。\n", me, ob);
	set_flag(ob,"工具");
	return STAT_PROG+STAT_END;
}

int wg_job(object me,string arg){
  object weapon;
	int costj, costq;
  int jid = check_flag(me,"job_id")-1;
  string *place = ({"/d/wuguan/mufang","/d/wuguan/mafang","/d/wuguan/chaifang","/d/wuguan/caiyuan","/d/wuguan/caidi","/d/wuguan/shuifang"}); 
  string *args = ({ "木头","马房", "柴","菜","草","水"});
  string *act = ({"锯","扫","劈","浇","锄","挑"});
  object env = environment(me);
  
  if(jid<0) return 0;
  
  if(query_verb() != events["wg_work"]["cmds"][jid]) return 0;
  
	if(base_name(env)!=place[jid]) return 0;
	
	if(!arg || arg!=args[jid]) 
	return notify_fail("你要"+act[jid]+"什么？\n");
	
	if (me->is_busy())
		return notify_fail("你现在正忙着呢！\n");

	if (me->is_fighting())
		return notify_fail("你正在战斗中,无法专心干活！\n");

	weapon = me->query_temp("weapon");

	if( !weapon || weapon->query("id") != check_flag(me,"tools_name"))
	return notify_fail("你想用什么来"+act[jid]+args[jid]+"，用手吗？("HIY HBCYN"wield "+check_flag(me,"tools_name")+NOR")\n");

	costj = random( me->query("con")/3);
	costq = random( me->query("str")/3);

	if ( me->query("jingli") < costj || me->query("qi") < costq){
		
		switch( jid ){
  		case 2:	message_vision(HIR"$N手一歪，不小心劈在自己的腿上\n"NOR,me);break;
  		case 4:	message_vision(HIR"$N手一松，不小心锄在自己的脚上。\n"NOR,me);break;
  		case 1:	message_vision(HIR"$N脚一滑，不小心滑倒在马粪里。\n"NOR,me);break;
  		case 5:	message_vision(HIR"$N手一抖，不小心水桶砸在自己身上。\n"NOR,me);break;
  		case 0:	message_vision(HIR"木头断了，$N一没留神让掉下来的木头砸在了脚上。\n"NOR,me);break;
  		case 3:	message_vision(HIR"$N弯腰到大缸里舀水，结果失足栽到缸里。\n"NOR,me);break;
	  }
		me->unconcious();
		return STAT_CNT;
	}

	me->receive_damage("jingli", costj);
	me->add("qi", -costq);

	switch(jid){
	  case 2:	write("你摆正一块木头，一刀劈了下去，“哐”的一声，木头被劈为两片。\n");break;
	  case 1:	write ("你找了个地方开始扫了起来，真够臭的，你不由闭紧了呼吸。\n");break;
	  case 3:	write("你用瓢从缸里舀起水，在菜地里浇起水来。 \n");break;
	  case 5:	write("你慢慢将水桶中的水蓄满，然后倒入水缸中。\n");break;
		case 0:	write("你摆正一根木头，慢慢锯了起来。\n");break;
	  case 4:	write("你挥起锄头，对着地上的杂草锄了起来。\n");break;
	}
	
	if(check_flag(me,"do_times") > 15 + random(5)) {
		set_flag(me,"完了");
		write(CYN+address[jid]+"管事说道：「干的不错，好了，你可以到大师兄那里去覆命了！」\n"NOR);
		write(CYN+address[jid]+"管事说道：「记得要把"+weapon->name()+"还给六师兄啊！」\n"NOR);
		pre_set_hint(me,"wg_re_tool","到"HIY"物品房"NOR"找"HIG"吴坎"NOR"归还"HIR+weapon->name()+NOR"。");
		return STAT_END+STAT_PROG;
	}

	me->start_busy(1);
	set_flag(me,"do_times");
  
  return STAT_CNT;
}

int wg_re_tool(object me,string arg)
{
  object ob,obj;
  string nid,tid;
  
  ob = present("wu kan",environment(me));
  
  if(!ob) return 0;
  
  if(sscanf(arg,"%s %s",nid,tid)!=2 && sscanf(arg,"%s to %s",tid,nid)!=2)
  return STAT_CNT;
  
  if(!(obj=present(tid,me))) return 0;
  
  if(!check_flag(me,"工具")){
    command("shake");
    tell_object(me, CYN+ob->name()+"说道：「我没有发给你过东西，你是不是还错了？」\n"NOR);
    return STAT_CNT;
  }
  if(!check_flag(me,"完了")){
    tell_object(me, CYN+ob->name()+"说道：「你还没完成工作，怎么就回来还工具了，偷懒吗？」\n"NOR);
    return STAT_CNT;
  }
  if(check_flag(obj,"mark_name") != me->query("id")){
    command("? "+me->query("id") );
    tell_object(me, CYN+me->name()+"说道：「我给你的好象不是这样东西吧？」\n"NOR);
    return STAT_CNT;
  }
  
  if(check_flag(me,"tools_name") != obj->query("id")){
    command("? "+me->query("id") );
    tell_object(ob, CYN+ob->name()+"说道：「我给你的好象不是这样东西吧？」\n"NOR);
    return STAT_CNT;
  }
  else {
    command("nod");
    tell_object(me,"你还给"+ob->name()+"一"+obj->query("unit")+obj->name()+"。\n");
    tell_object(me, CYN+ob->name()+"说道：「加油，加油，下次再好好干啊！」\n"NOR);
    tell_object(me, CYN+ob->name()+"说道：「快去找大师兄领奖励吧！」\n"NOR);
    set_flag(me,"还了");
    destruct(obj);
    pre_set_hint(me,"wg_finish","到"HIY"武馆大厅"NOR"找"HIG"鲁坤"NOR"交谈。");
    return STAT_END+STAT_PROG;
  }
}

int wg_reward(object ob,object me){
  if(!check_flag(ob,"还了")){ 
		tell_object(ob, CYN+me->name()+"说道：「你先把工具还给六弟，再来覆命吧。」\n"NOR);
		return STAT_CNT;
	}
	if(!check_flag(ob,"完了")){
		tell_object(ob, CYN+me->name()+"说道：「你偷懒啊，叫你干活你不去干，跑来领功！」\n"NOR);
		return STAT_CNT;
	}	
	tell_object(ob, CYN+me->name()+"说道：「好，"+RANK_D->query_respect(ob)+"，做得好，这是给你的奖赏！」\n"NOR);
	
	return STAT_END+STAT_FIN;
}