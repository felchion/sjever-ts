inherit NPC;
#include <ansi.h>
#include <function.h>

mapping default_prop = ([
  "dbase" :([
    "shen": -15000,
    "attitude": "killer",    
    "combat_exp": 9999999,
    "max_qi":9999,
    "eff_qi":9999,
    "qi":9999,
    "max_jing":9999,
    "eff_jing":9999,
    "jing":9999,
    "max_jingli":9999,
    "eff_jingli":9999,
    "jingli":9999,
    "neili":9999,
    "jiali": 99,
    "max_neili":9999,
    "score": 10000
  ]),
]);

string dest_desc;
mapping cmds;

string* f = ({"","","","",""});
string task;

int do_cmds(string arg){
  string v = query_verb();
  if(mapp(cmds)) {
    if( stringp(cmds[v]) ) 
    return call_other(task,cmds[v],this_object(),arg);
    else if(cmds[v]){
      CHANNEL_D->do_channel( this_object(),"err",
      sprintf("错误函数入口：命令 %s to %O!\n",v,cmds[v]) );
      return 0;
    }
  }    
  return 0;
} 

void dest()
{
  if (!living(this_object())) {
    call_out("dest", 20);
    return;
  }
  if(stringp(dest_desc)) message_vision(dest_desc,this_object());
  destruct(this_object());
}

void create(mapping prop){

   set_name( "禽兽", ({ "animal" }));
   set("race","野兽");
   set("limbs",({ "头部","身体","尾巴" }));
   set("verbs", ({ "bite" }));  
   set("age", 3+random(10));
  
  if(!mapp(prop)) prop = default_prop;

  if(mapp(prop["dbase"])){
    foreach(string item in keys(prop["dbase"])){
      set(item,prop["dbase"][item]);
    }
  }
  
  if(mapp(prop["function"])) {
    f[0] = prop["function"]["init"];
    f[1] = prop["function"]["kill_ob"];
    f[2] = prop["function"]["die"];
    f[3] = prop["function"]["unconcious"];
    f[4] = prop["function"]["heart_beat"];
  }
  
  if(arrayp(prop["call"])) {
    foreach(mixed item in prop["call"]){
      call_other(this_object(),item);
    }
  }
  
  if(prop["dest_time"]) call_out("dest",prop["dest_time"]);
  
  cmds=prop["cmds"];
  dest_desc = prop["dest_desc"];
  task = base_name(previous_object());

  setup();
}


void init()
{
  object ob = this_player();
  object me = this_object();

  ::init();
  if(mapp(cmds)) add_action("do_cmds","");
  if(stringp(f[0])) call_other(task,f[0],me,ob);
}

void kill_ob(object ob)
{
  object me = this_object();

  if(stringp(f[1]) && call_other(task,f[1],me,ob)) return;

  ::kill_ob(ob);
}

void die()
{
  object me, ob;
  me = this_object();
  ob = me->query_temp("last_damage_from");
  
  if(stringp(f[2]) && call_other(task,f[2],me,ob)) return;

  ::die();
}

void unconcious()
{
  object me,ob;

  me = this_object();
  ob = me->query_temp("last_damage_from");
  
  if(stringp(f[3]) && call_other(task,f[3],me,ob)) return;

  ::unconcious();
}
 

void heart_beat() {
  if(stringp(f[4])) call_other(task,f[4],this_object());
  ::heart_beat();
}
