#include <ansi.h>
#include <task.h>

void create()
{
	seteuid(getuid());
}

int main(object me, string arg){
  object ob,task;
  mapping tlist,elist;
  string msg="";
  int count;

  if (!userp(me)) return 0;
  seteuid(getuid(me));

  if (!arg) ob = me;
  else if(wizardp(me)) {
    ob = LOGIN_D->find_body(arg);
    if (!ob) return notify_fail("你要察看谁的任务状态？\n");    
  }
  else ob = me;
  
  if(mapp(tlist=ob->query(T_E_LST))) {
    string *tids = keys(tlist);
    foreach(string tid in tids){
      if(mapp(elist=tlist[tid])) {
        string *eids = keys(elist);
        foreach(string eid in eids){
          if(intp(elist[eid])) {
            if((elist[eid]&STAT_ACT || elist[eid]&STAT_CNT) 
              && objectp(task=TASK_D->find_task(tid))
              && !task->no_hint()  
            ){
              string hint = task->query_event_hint(ob,eid);
              msg += sprintf("%s  %s\n",task->query_event_name(eid),hint?hint:"无");
              count++;
            }
          }
        }
      }     
    }
  }
  if(mapp(tlist=ob->query_temp(T_E_LST))) {
    string *tids = keys(tlist);
    foreach(string tid in tids){
      if(mapp(elist=tlist[tid])) {
        string *eids = keys(elist);
        foreach(string eid in eids){
          if(intp(elist[eid])) {
            if((elist[eid]&STAT_ACT || elist[eid]&STAT_CNT) 
              && objectp(task=TASK_D->find_task(tid))
              && !task->no_hint()
            ){
              string hint = task->query_event_hint(ob,eid);
              msg += sprintf("%s  %s\n",task->query_event_name(eid),hint?hint:"无");
              count++;
            }
          }
        }
      }     
    }
  }
  
  if(count) {
    msg += "\n目前共有"+count+"个任务正在进行中。\n";
    tell_object(me,msg);
    return 1;
  }
  
  return notify_fail("你当前没有进行中的任务。\n");  
}


int help(object me)
{
	if (!wiz_level(me) )
	write(@HELP

HELP);
	return 1;
}
