inherit ITEM;
#include <ansi.h>

mapping cmds;
string task;
string _init;
string dest_desc;


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

  set_name ("关键道具", ({"key item"}));
  set("long","剧情关键道具。\n");
  set("unit","个");
  set_weight(100);
  set("no_get",1);
  set("no_get_from",1);
  set("no_give", "这样东西不能离开你。\n");
  set("no_get", "这样东西不能离开那儿。\n");
  setup();

  if(mapp(prop)) {
    if(mapp(prop["dbase"])){
      foreach(string item in keys(prop["dbase"])){
        set(item,prop["dbase"][item]);
      }
    }

    if(mapp(prop["function"])) {
      _init = prop["function"]["init"];
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
  }
  setup();
}

void init()
{
  object ob = this_player();
  object me = this_object();

  if(mapp(cmds)) add_action("do_cmds","");
  if(stringp(_init)) call_other(task,_init,me,ob);
}
