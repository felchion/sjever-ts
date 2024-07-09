// taskd.C for tasks.
// 15.03.99 by snowman@SJ
// Modified by snowman@SJ 03/09/2000

///kungfu/class/gaibang/wu.c -> /tmp/ciwei/job/wu.c 成功。
///d/xingxiu/obj/di.c -> /tmp/ciwei/job/di.c 成功。
///d/xingxiu/npc/ding.c -> /tmp/ciwei/job/ding.c 成功。
// zuo.c zuo.h

/*
用法: get_quest(this_player(), shen type(int), job type(string), family name(string))
shen type is either 1 for +shen npcs or -1 for -shen npcs,
传回是 mapping：
["id"] = target 的 ID,
["name"] = taget 的名字,
["last_id"] = 上个target的id，以免重复,
["time"] = 最后时间限制,
["exp_reward"] = EXP 奖励,
["pot_reward"] = POT 奖励,
["type"] = 种类，比如 "杀", "请", "找"...,
["family"] = 门派名，如果不给family，将等于player自己的门派名,
["place"] = 地点，target目前的所在地.

PS: get_time(quest["time"]) 将传回中文的时间限制.
*/

#include <job_mul.h>
#include <get_place.h>
#include <ansi.h>
#include <task.h>
#include <mudlib.h>

inherit F_SAVE;

#include <regions.h>

#define STUFFS	 	DATA_DIR + "task_stuffs"


private int checkx(object ob, object me, string fam, string type, int shen, string gender);
private int quest_reward(int exp, int texp);
string get_regions(string str);
varargs int add_exp_combat(object me,int iexp,mixed giver,string reason,int no_job,int gift_flag,int silent);
mapping stuffs = ([ ]);
static string *st;

static mapping NPC_Event;
static mapping Place_Event;

static mapping Reward_List =
([
  "jtp"	  : ({ HIW"精铁魄"NOR, "collection/l1/jingtie po","块" }),
  "exp"	  :	({ "实战经验", "combat_exp","点" }),
  "pot"	  :	({ "潜能", "potential","点" }),
  "gold"  :	({ HIY"黄金"NOR, "gold","锭" }),
  "cash"  :	({ HIW"银票"NOR, "cash","张" }),
  "tb"		: ({ HIG"书剑通宝"NOR, "SJ_Credit","个" }),
  "sw"	  : ({ HIC"江湖声望"NOR, "honor_point","点" }),
  "jjb"		: ({ HIR"竞技币"NOR, "SJ_PKC","个" }),
  "silk"	: ({ HIW"原丝"NOR, "collection/l1/raw silk","团" }),
  "ore"		: ({ CYN"原石"NOR, "collection/l1/ore","块" }),
  "nl"		: ({ HIG"最大内力"NOR, "max_neili","点" }),
  "jl"		: ({ HIG"最大精力"NOR, "max_jingli","点" }),
  "aexp"	: ({ HIW"攻击经验"NOR, "skill/attack/无","点" }),
  "dexp"	: ({ HIW"防御经验"NOR, "skill/defense/无","点" }),
  "dhd"	  : ({ RED"大还丹"NOR, "collection/medicine/dahuan dan","颗" }),  
  "yhb"	  : ({ HIW"永恒币"NOR, "SJ_TC","个" }),  
  "imb"   : ({ HIC"打造机会"NOR,"imbue_reward","次"})
]);

static mapping Job_List = ([
	"华山": "惩恶扬善",
	"颂摩崖": "抗敌颂摩崖"
]);

object reward_item(mixed ob,object me) {
  if(stringp(ob)) ob = new(ob);
  else if (functionp(ob)) ob=evaluate(ob,me);
  if(!objectp(ob)) return 0;
  ob->set("owner",me->query("id"));
  if(ob->move(me)) return ob;
  return 0;
}

varargs void event_reward(object me,mapping reward,mapping task,int flag){
  string msg="";
  mixed str;
  int i;
  
  if(!me) return;
  
  if(mapp(reward)) {
    if(str=reward["item"]){
      object ob;
      if(arrayp(str)) {
        foreach(string item in str){         
          if(ob=reward_item(item,me)) {
            msg += HIW"你获得了一"+ob->query("unit")+ob->name()+"。\n"NOR;
          }
        }
      }
      else if(ob=reward_item(str,me))
      msg += HIW"你获得了一"+ob->query("unit")+ob->name()+"。\n"NOR;
    }    
    str = keys(reward);
    foreach (string item in str){
      if(!Reward_List[item]) continue;
      if(Reward_List[item][1] == "cash") {
	      object cash=new("/clone/money/thousand-cash");
				cash->set_amount(reward[item]);
				cash->move(me);
        msg += HIW"你获得了"+chinese_number(reward[item])+Reward_List[item][2]+Reward_List[item][0]+"。\n"NOR;
      }
      else if(Reward_List[item][1] == "gold") {
        call_other(me, "add", "balance" , reward[item]*10000);
        msg += HIW"你获得了"+chinese_number(reward[item])+Reward_List[item][2]+Reward_List[item][0]+"。\n"NOR;
      }         
      else if(Reward_List[item][1] == "combat_exp") {
        //call_other(me, "add", "combat_exp", reward[item]);
        if(task["cid"]) {
        	i=add_exp_combat(me,reward[item],task["giver"],task["cid"],0,0,flag);
        	me->set("job_name",Job_List[task["cid"]]);
        }
        else i=add_exp_combat(me,reward[item]);
        msg += HIW"你获得了"+chinese_number(i)+Reward_List[item][2]+Reward_List[item][0]+"。\n"NOR;
      }
      else {
        if(item == "jl") call_other(me, "add", "eff_jingli" , reward[item]);
        call_other(me, "add", Reward_List[item][1], reward[item]);
        msg += HIW"你获得了"+chinese_number(reward[item])+Reward_List[item][2]+Reward_List[item][0]+"。\n"NOR;
      }
    }
  // tell_object(me,msg);    
    if(IN_MAIN) {
    	tell_object(me,msg); 
    	msg = sprintf("%s %s  %O\n",me->query("id"),task["id"],reward);    
	    msg = replace_string(msg,"([ /* sizeof() ==","");    
	    msg = replace_string(msg,"])","");
	    msg = replace_string(msg,"\n","");
	    log_file("/job/task_reward",msg+"\n");
    }
    else CP_D->set_reward(me,reward,task);    
  }  
}


string *query_reward_type(){return keys(Reward_List)+({"item"});}

void clean_hb_event(object me,int type){
  if(!type) {
    me->delete(T_HB);
    me->delete_temp(T_HB);
  }
  else {
    me->delete(T_HB);
    me->delete_temp(T_HB);
  }
}

void heart_beat_event(object me){
  mapping list;
  int t = time();
  
  list = me->query(T_HB);
  if(mapp(list) && sizeof(list)) {
    foreach(string item in keys(list)) {
      if(list[item][0] < t ) {
        if(sizeof(list[item])==6) {
          object task = load_object(list[item][2]);
          task->event_prog(list[item][4],me);
        }
        else 
        call_other(list[item][2],list[item][3],me);
      } 
      
      if(arrayp(list[item]) && list[item][1] < t 
        && list[item][5] != HB_PROC) map_delete(list,item);
    }
  }  
  list = me->query_temp(T_HB);
  if(mapp(list) && sizeof(list)) {
    foreach(string item in keys(list)) {
      if(list[item][0] < t ) {
        if(sizeof(list[item])==6) {
          object task = load_object(list[item][2]);
          task->event_prog(list[item][4],me);
        }
        else 
        call_other(list[item][2],list[item][3],me);
      }
      if(arrayp(list[item]) && list[item][1] < t 
      && list[item][5] != HB_PROC ) map_delete(list,item);
    }
  }  
} 



object find_task(string id) {
  object task;  
  
  task = find_object(TASK_DIR(id));  
  
  if(objectp(task) && task->is_on()) 
  return task;  
 
  return 0; 
}


void init_tasks() {
  mixed task,tasks = get_dir(TASK_DIR(""));
  int i;
  mapping tt,events;
  string npc,place,id;
  object task_ob;

  NPC_Event = ([]);
  Place_Event = ([]);

  for(i=0;i<sizeof(tasks);i++){
    if(catch(task=load_object(TASK_DIR(tasks[i])))) continue;
    if(!objectp(task)
    || !mapp(tt=task->query_task())
    || !tt["on"]
    || !mapp(events=task->query_events())
    ) continue;
    id = tt["start"];
    if((npc=events[id]["npc"])) {
      if(arrayp(NPC_Event[npc])) {
        NPC_Event[npc] += ({ ({tasks[i],id,events[id]["type"]}) });
      }
      else {
        NPC_Event[npc] = ({ ({tasks[i],id,events[id]["type"]}) });
      }
    }
    if(place=events[id]["place"]) {
      if(arrayp(Place_Event[place])) {
        Place_Event[place] += ({ ({tasks[i],id,events[id]["type"]}) });
      }
      else {
        Place_Event[place] = ({ ({tasks[i],id,events[id]["type"]}) });
      }
    }
  }  
}

mapping query_place_list() {return Place_Event;}
mapping query_npc_list() {return NPC_Event;}

mixed user_npc_event(object me,object ob) {
  mixed list;
  if(mapp(me->query_temp(T_NPC)))
  list =  me->query_temp(T_NPC)[base_name(ob)];

  if(mapp(me->query(T_NPC))
    && arrayp(me->query(T_NPC)[base_name(ob)])
  ){
    if(arrayp(list)) list += me->query(T_NPC)[base_name(ob)];
    else list = me->query(T_NPC)[base_name(ob)];
  }
  return list;
}

int action_event(object me,string verb,string arg){
  mapping cmds;
  mixed task,events;
  int i;

  if(mapp(me->query_temp(T_CMD))) 
  cmds = me->query_temp(T_CMD);
  
  if(mapp(me->query(T_CMD))) {
    if(mapp(cmds)) cmds += me->query(T_CMD);
    else cmds = me->query(T_CMD);
  }
  
  if(!mapp(cmds)) return 0;

  if(events=cmds[verb]) {
    for(i=0;i<sizeof(events);i++) {
      if(!objectp(task=find_object(events[i][0]))) continue;
      if(task->event_prog(events[i][1],me,arg))
      return 1;
    }
  }
  return 0;
}

mixed* user_place_event(object me,object ob){
  mixed *events;
  mapping list = me->query(T_PLC);

  if(mapp(list))
  events = list[base_name(ob)];

  list = me->query_temp(T_PLC);

  if(mapp(list)) {
    if(arrayp(list[base_name(ob)]))
    events += list[base_name(ob)];
    else
    events = list[base_name(ob)];
  }
  
  return  events;
}


void place_event(object me,object ob) {
  int i,flag;
  mixed task,events;

  if(!objectp(ob)) return;
  if(events=Place_Event[base_name(ob)]) {
    for(i=0;i<sizeof(events);i++) {
      if(!(events[i][2] & TYP_GO)) continue;
      if(!objectp(task=find_object(TASK_DIR(events[i][0])))) continue;
      if(task->event_prog(events[i][1],me,ob))
      return;
    }
  }

  if(events=user_place_event(me,ob)) {
    for(i=0;i<sizeof(events);i++) {
      if(!(events[i][2] & TYP_GO)) continue;
      if(!objectp(task=find_object(events[i][0]))) continue;

      if(task->event_prog(events[i][1],me,ob))
      return;

    }
  }
  return;
}

void ef_event(object me,object ob){
  mixed task,events;
  int i;

  if(events=user_npc_event(me,ob)){
    for(i=0;i<sizeof(events);i++) {
      if(!(events[i][2] & TYP_EF)) continue;
      if(!objectp(task=find_object(TASK_DIR(events[i][0])))) continue;
      task->event_prog(events[i][1],me,ob);
    }
  }

}

int talk_event(object me,object ob) {
  int i,flag;
  mixed task,events;
  
  if(events=NPC_Event[base_name(ob)]) {
    for(i=0;i<sizeof(events);i++) {
      if(!(events[i][2] & TYP_TALK)) continue;
      if(!objectp(task=find_object(TASK_DIR(events[i][0])))) continue;
     // flag = task->check_status(me,events[i][1]);
     // if( flag == STAT_ACT || flag == STAT_PROG) {
        if(task->event_prog(events[i][1],me,ob))
        return 1;
    //  }            
    }    
  }
  
  if(events=user_npc_event(me,ob)) {
    for(i=0;i<sizeof(events);i++) {
     // tell_object(me,events[i][0]+"\n");
      if(!(events[i][2] & TYP_TALK)) continue;
      if(!objectp(task=find_object(events[i][0]))) continue;
     // flag = task->check_status(me,events[i][1]);
     // if( flag == STAT_ACT || flag == STAT_PROG){
        if(task->event_prog(events[i][1],me,ob))
        return 1;
     // }
    }    
  }  
  message_vision(CYN "$N说道：哈哈，今天天气可真是好啊。\n" NOR, ob);
  return 1;
} 

string npc_event_hint(object me,object ob) {
  int i,flag;
  mixed task,events;
  string *flist = ({"  ",HIG"◎"NOR, WHT"◎"NOR});
  
  if(!userp(me)) return "  ";
  if(!ob->is_character()) return "  ";
  
  if(events=NPC_Event[base_name(ob)]) {
    for(i=0;i<sizeof(events);i++) {
      if(!objectp(task=find_object(TASK_DIR(events[i][0])))) continue;     
      if(flag=task->npc_hint_flag(me,events[i][1])) return flist[flag];
    }    
  }  
  if(events=user_npc_event(me,ob)) {
    for(i=0;i<sizeof(events);i++) {
      if(!objectp(task=find_object(events[i][0]))) continue;
      if(flag=task->npc_hint_flag(me,events[i][1])) return flist[flag];
    }    
  }
  
  return "  ";
}

void create()
{
  seteuid(getuid());
  if( !restore() && !mapp(stuffs) )
  stuffs = ([]);
  st = keys(stuffs);
  init_tasks();
}

string query_save_file()
{
  return STUFFS;
}

mapping query_stuffs()
{
  return stuffs;
}

int set_stuff(string name, mapping etc)
{
  if (!undefinedp(stuffs[name]))
  return 0;
  stuffs[name] = etc;
  save();
  return 1;
}

int delete_stuff(string name)
{
  if (undefinedp(stuffs[name]))
  return 0;
  map_delete(stuffs, name);
  save();
  return 1;
}

string query_all_stuffs()
{
  string str = "";
  int i;
  i = sizeof(st);
  while(i--)
  str += sprintf(" %-30s  奖励：%d\n", st[i]+"("+stuffs[st[i]]["id"]+")", stuffs[st[i]]["reward"]);
  return str;
}

static int my_exp, quest_time;
static string last_id;

mapping get_quest(object me, int shen, string type, string family, string gender)
{
  object target, *living;
  int i,j;

  if(!family || !stringp(family))
  family = me->query("family/family_name");

  if(type == "找"){
    if( !stuffs || sizeof(stuffs) < 1) return 0;
    i = random(sizeof(st));
    j = stuffs[st[i]]["reward"];
    return ([
      "id":	   stuffs[st[i]]["id"],
      "name":	 st[i],
      "time":	 TIME_TICK +(j*50*60),
      "exp_reward":  j,
      "pot_reward":  j/5 + random(j/10),
      "type":	 type,
      "family":family,
    ]);
  }

  if (!shen || !intp(shen))  shen = 0;
  shen = SGN(shen);

  if(!type || !stringp(type)) type = "杀";

  if(gender !="女" && gender !="男" && gender !="无性")
  gender = "无";

  my_exp = me->query("combat_exp");
  last_id = me->query_temp("quest/last_id");
  if ((quest_time = uptime()) < 180)
  quest_time = 180;
  living = filter_array(livings(), "checkx", this_object(), me, family, type, shen, gender);

  if (!living || sizeof(living) <= 0)
  return 0;

  i = random(sizeof(living));
  target = living[i];

  if(!objectp(target) || !stringp(target->query("id")))
  return 0;       // no target at all or cant get target's id.

  i = quest_reward(me->query("combat_exp"), target->query("combat_exp")); // get the rewards.

  if (type == "请")
  i += random(i/10);

  target->set_temp("last_quest_time", quest_time);
  return ([
    "id":		(string)target->query("id"),
    "name":		(string)target->name(1),
    "time":		TIME_TICK+(i*30*60),
    "exp_reward":	i,
    "pot_reward":	i/5 + random(i/10),
    "type":		type,
    "family":	family,
    "place":	get_place(base_name(environment(target)))+environment(target)->query("short"),
  ]);
}

static string *no_kill_list = ({"mu ren","jin ren","yin ren","zhong shentong","zhu cong","hou junji","shi qingshan"});

private int checkx(object ob, object me, string fam, string type, int shen, string gender)
{
  object room;
  string room_name;

  reset_eval_cost();
  if (type == "送")
  my_exp = 0;

  if (!objectp(ob)
  || userp(ob)
  || ob->query("chat_msg")		// add by Yuj@SJ to void random_move NPC 2000-08-30
  || ob->query("env/invisibility")
  || ob->query("no_quest")
  || ob->query("job_npc")
  || ob->query_temp("last_quest_time") > quest_time - 180	// 间隔三分钟 YUJ@SJ 2001-11-01
  || member_array(ob->query("id"), no_kill_list) != -1  // special npcs, like zhong shentong, attacker, biaoshi etc.
  || ob->query("id") == last_id
  || ob->query("race") !="人类"
  || ob->query("family/family_name") == fam
  || ob->query("have_master")
  || strsrch(base_name(ob), "/d/") != 0
  || !objectp(room=environment(ob))
  || room->query("no_fight")
  || room->query("quest")
  || strsrch(room_name = base_name(room),"/d/") != 0
  || ob->query("startroom") != room_name	// improve by Yuj@SJ to void job NPC 2001-11-25
  || strsrch(room_name, "/d/group") == 0
  || strsrch(room_name, "/d/wizard/") == 0
  || strsrch(room_name, "/d/wuguan/") == 0
  || strsrch(room_name, "/d/death/") == 0
  || ob->query("combat_exp") > my_exp/2*3
  || ob->query("combat_exp") < my_exp/3*2
  || function_exists("prevent_learn",ob)
  )
  return 0;

  if (intp(shen) && (ob->query("shen") > 0 && shen > 0
  || ob->query("shen") < 0 && shen < 0))
  return 0;
  if(gender != "无" && ob->query("gender") != gender) return 0;

  switch(type){
    case "请": if(ob->query("attitude")=="aggressive") return 0;
    case "送": if(strsrch(room_name,"/d/dali") == 0) return 0;
    default : return 1;
  }
}

private int quest_reward(int exp, int texp)     // need a better form here.
{
  int i = 5, j = 2;

  if( exp < texp/3*2 ) return 0;

  i *= (texp * 100 / exp);

  return i/j;
}

int is_target(object corpse, object me)
{
  mapping quest;

  if ( !mapp(quest = me->query_temp("quest"))) return 0; // didnt get the job

  if ( quest["time"] < TIME_TICK) return -3; // time up.

  if ( quest["type"] != "杀") return -1;  // job type is not killing

  if ( corpse->query("id") != "corpse"
  && corpse->query("id") != "shouji") return 0;// it's not a corpse or head.

  if ( corpse->query("victim_id") != quest["id"]
  || corpse->query("victim_name") != quest["name"]
  || corpse->query("kill_by") != me)
  return -2;      // killed wrong person.

  if ( corpse->query("victim_user")) return -4;   // killed a player.
  else return 1;
}

string get_time(int i)
{
  return CHINESE_D->chinese_date(i);
}

// Modified by mxzhao 2004/05/04
string get_regions(string str)
{
  string arg = "";
  string *place = explode(str, "/");

  switch (place[0])
  {
    case "area":
    arg = area_names[place[1]] + region_names[place[2]];
    break;
    case "cmds":
    if (!(arg = sub_region_names[place[2]]))
    {
      arg = region_names[place[1]];
    }
    break;
    case "d":
    if (place[1] == "group" || place[1] == "zhiye")
    {
      object room;

      if(!(room = find_object(str)))
      {
        room = load_object(str);
      }

      if (room)
      {
        if (room->query("outdoors"))
        {
          arg = room->query("outdoors");
        }
        else if (room->query("indoors"))
        {
          arg = room->query("indoors");
        }
      }
    }
    else
    {
      if (!(arg = sub_region_names[place[2]]))
      {
        arg = region_names[place[1]];
      }
    }
    break;
    default:
    break;
  }

  return arg?strip(arg):"";
}


void give_reward(object me, mapping quest, int shen, int extra,mixed giver,string job)
{
  int exp=0, pot;
  if(!me || !mapp(quest)) return;

  if (intp(quest["exp_reward"]))
  exp = quest["exp_reward"];
  else
  return;
  if (intp(quest["pot_reward"]))
  pot = quest["pot_reward"];

  if(extra){
    exp += extra;
    pot += extra;
  }
  if ( exp > 350 ) exp = 330 + random(20);

  exp = me->add_exp_combat(exp,giver,job);

  me->add("potential", pot);
  if(me->query("potential") > me->query("max_pot"))
  me->set("potential", me->query("max_pot"));
  tell_object(me, HIY"你觉得脑中豁然开朗，增加了"+chinese_number(pot)+"点潜能和"+chinese_number(exp)+"点经验！\n"NOR);
  if(shen)
  me->add("shen", shen * quest["exp_reward"] * 3);
  me->delete_temp("quest");
  me->set_temp("quest/last_id", quest["id"]);
  me->set_temp("quest/exp_reward", exp);
  me->set_temp("quest/pot_reward", pot);
}


varargs int add_exp_combat(object me,int iexp,mixed giver,string reason,int no_job,int gift_flag,int silent)
{
  int gift_mul,oexp,party_mul,rate, vip_lvl;
  int k,l,m,n;
  string fam;

  if(!me || !userp(me)) return 0;

  silent = !IN_MAIN;
  
  fam = me->query("family/family_name");

  if(reason) gift_mul = JOB_GIFT_MUL[reason];
  if(!gift_mul) gift_mul = 100;

  vip_lvl = me->query("registered");
  if(vip_lvl==17) vip_lvl = 12; 
  
  if(fam) party_mul = PARTY_JOB_MUL[fam];
  if(me->query("sec_party_id")) party_mul = 110;
  if(!party_mul) party_mul = 100;
  oexp = iexp*gift_mul*(party_mul+(vip_lvl-2)*3+(vip_lvl>12?(vip_lvl-12)*2:0))/10000;
  oexp = oexp*SITE_MUL/100;


  if( reason!="丐帮" && reason!="嵩山"
  && me->query_temp("quest") && sizeof(me->query_temp("quest")) ){
    oexp = oexp /20;
    me->delete_temp("quest");
    if(!silent) tell_object(me,"你觉得自己最近心神不宁，对武学的研究已经进入了举步维艰的程度，似乎就是江湖传说的『"+HIG+"身名障"+NOR+"』。\n");
  }

  if(me->query("SJ_BUG")){
    oexp = oexp * 5/100;
    if(!silent) tell_object(me,"你觉得自己最近心神不宁，对武学的研究已经进入了举步维艰的程度，似乎就是江湖传说的『"+HIG+"知见障"+NOR+"』。\n");
    if(!silent) write(HIR+"提示:你因使用BUG获利，请速到扬州城隍庙偿还(payback)。\n");
  }

  oexp = oexp * me->query("eff_online_rate") / 100;
  
  if(!no_job)
  {
    me->add("job_time/"+reason,1);
    GIFT_D->check_count(me,giver,reason,gift_flag);
  }

  rate = 100;

  if (me->query_condition("db_exp")>0){
    if(!silent) tell_object(me,HBGRN+HIG+"你感觉到精英之技发挥特殊作用，你额外的得到了"+chinese_number(oexp)+"点经验值。\n"+NOR);
    oexp *= 2;
    rate*=2;
  }
  if (me->query_condition("qt_exp")>0){
    if(!silent) tell_object(me,HBGRN+HIW+"你的洛迦之心发挥了作用，你额外的得到了"+chinese_number(oexp)+"点经验值。\n"+NOR);
    oexp = oexp*2;
    rate = rate*2;
  }
  else 	if(me->query_condition("tp_exp")>0){
    if(!silent) tell_object(me,HBGRN+HIW+"你的明悟之心发挥了作用，你额外的得到了"+chinese_number(oexp/2)+"点经验值。\n"+NOR);
    oexp = oexp*3/2;
    rate = rate*3/2;
  }

  if (k=me->query("quest/jiuyin1/lvl")){
    m = oexp*k/50;
    if(!silent) tell_object(me,HBGRN+HIW+"你印证九阴真经所载武学心有所得，额外的得到了"+chinese_number(m)+"点经验值。\n"+NOR);
    oexp += m;
    rate += rate*k/50;
  }

  if(m=GIFT_D->query("exp")){
    if(!silent) tell_object(me,BLINK+HIW+"现在是本站经验加倍活动期，你额外得到了"+chinese_number(m*oexp)+"点经验值。\n"+NOR);
    oexp = oexp*(1+m);
    rate = rate*(1+m);
  }

  if(reason!="颂摩崖"
    &&(  ctime(time())[11..12]=="21"
      || ctime(time())[11..12]=="20"
    )
  )
  oexp = oexp*6/5;

  if((rate=sizeof(me->query("reparty")))>1) {
  	k = oexp;
  	oexp = oexp*(99+rate)/100;
  	if(!silent) tell_object(me,ansi_color(054)+"你印证各派武学心有所得，额外得到了"+chinese_number(oexp-k)+"点经验值。\n"+NOR);
  }
  
  
  me->add("week_rate/this_week_exp",oexp);
  me->add("combat_exp", oexp);
  me->set("jobfinish/"+reason,time());
  me->set("job_name",Job_List[reason]);

  if(me->query("exp_log")){
    dbquery("REPLACE explog SET "
      "id='" + me->query("id") + "',"
      "reason='" + reason + "',"
      "party='" + fam + "',"
      "oexp=" + oexp + ","
      "iexp=" + iexp + ","
      "rate=" + rate + ","
      "timestamp=" + time() + ","
      "week=" + time()/(86400*7) + ","
      "utime=" + (me->query_temp("jobstart/"+reason)?(time()-me->query_temp("jobstart/"+reason)):0) + ","
      "ftime='" + c_date(time()) + "'"
    );
  }

  return oexp;
}