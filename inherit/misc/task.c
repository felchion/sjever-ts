#define VCOND "/v/obj/cond"
#include <task.h>

mapping events;
mapping task;
mapping cond;

void set_task(mapping m) {task=m;}
void set_events(mapping m) {events=m;}

mapping query_task(){return task;}
mapping query_events(){return events;}
mapping query_event(string eid) {return events[eid];}


string task_cond(string cid){return cid+"@"+task["id"];}
object get_cond(string cid) {
	if(mapp(cond) && arrayp(cond[cid])) {
		if(sizeof(cond[cid])!=3 || !stringp(cond[cid][0])
		||!function_exists(cid+"_cond",this_object())) {
			CHANNEL_D->do_channel( this_object(),"err",
        sprintf("虚拟cond设置错误：任务%s 事件%s!\n",task["id"],cid));
      return 0;
		}
		return new(VCOND,cid,cond[cid]);
	}	
}

mixed query(string arg){
  if(!mapp(task)) return 0;
  return task["arg"];
}

int event_rst(object,string);

//任务状态相关函数
string query_task_name() {return task["name"];}

int set_status(object me,int status){
  if(status == STAT_OFF) return me->delete(TID);
  else return me->set(TID,status);
}

int query_status(object me) {return me->query(TID);}

int is_on() {return task["on"];}

int no_hint() {return task["no_hint"]; }

varargs int save_type(string eid) {
  if(task["online"]) return 0;
  if(eid && events[eid]["online"]) return 0;
  return task["offline"] || !task["RPTL"] || task["limit_day"];
}
//------------------------------------


//事件状态相关函数
string query_event_name(string eid){
  return HIW+task["name"]+"·"+events[eid]["name"]+NOR;
}

int set_event_status(object me,string eid,int status) {
  if(!mapp(events[eid])) return -1;
  if(save_type(eid)) {
    if(status == STAT_OFF) {
      me->delete(E_LST+eid);
      return 0;
    }
    return me->set(E_LST+eid,status);
  }
  else {
    if(status == STAT_OFF) {
      me->delete_temp(E_LST+eid);
      return 0;
    }
    return me->set_temp(E_LST+eid,status);
  }
}

int query_event_status(object me,string eid) {
  if(!mapp(events[eid])) return STAT_OFF;
  
  if(save_type(eid)){
    if( me->query(E_LST+eid) ) 
    return me->query(E_LST+eid);  
  }
  else {
    if( me->query_temp(E_LST+eid) ) 
    return me->query_temp(E_LST+eid);  
  }
  return STAT_ACT;  
}
//------------------------------------


//任务专用标志变量相关函数
varargs mixed set_flag(object me,string flag,mixed value){  
  if(!value) {
      if(save_type()) return me->add(E_LST+"Flag/"+flag,1);
      return me->add_temp(E_LST+"Flag/"+flag,1);
  }
  if(save_type()) return me->set(E_LST+"Flag/"+flag,value);
  return me->set_temp(E_LST+"Flag/"+flag,value);
}

varargs mixed check_flag(object me,string flag){
  if(save_type()) return me->query(E_LST+"Flag/"+flag);
  return me->query_temp(E_LST+"Flag/"+flag);
}

varargs int del_flag(object me,string flag){
  if(!flag) flag="";  
  if(flag == "CLR_ALL") {
     me->delete(E_LST);
     me->delete_temp(E_LST);
     return 1;
  }  
  if(save_type()) return me->delete(E_LST+"Flag/"+flag);  
  return me->delete_temp(E_LST+"Flag/"+flag);
}
//------------------------------------


//任务提示相关函数
int npc_hint_flag(object me,string eid) {
  int flag = query_status(me);

  if(no_hint()) return 0;

  if(flag & STAT_FIN+STAT_FAIL
    && (!task["limit_day"]
      || me->check_day_flag(task["id"]) >= task["limit_day"]
    )
  )  return 0;

  flag = query_event_status(me,eid);
  if(flag & (STAT_END+STAT_FAIL+STAT_PEND) ) return 0;
  if( flag & STAT_ACT ) {
    if(!event_rst(me,eid))
    return 1;
    else 
    return 2;
  }
}

void pre_set_hint(object me,string next,string hint) {
  if(save_type(next) ) me->set(E_LST+"P_Hint/"+next,hint);
  else me->set_temp(E_LST+"P_Hint/"+next,hint);
}

void set_event_hint(object me,string eid){
  int hint = events[eid]["hint"];
  if(save_type(eid)){
    if(me->query(E_LST+"P_Hint/"+eid)){
      hint = me->query(E_LST+"P_Hint/"+eid);
      me->delete(E_LST+"P_Hint/"+eid);
    }
    me->set(E_LST+"Hint/"+eid,hint);
  }
  else {
    if(me->query_temp(E_LST+"P_Hint/"+eid)){
      hint = me->query_temp(E_LST+"P_Hint/"+eid);
      me->delete_temp(E_LST+"P_Hint/"+eid);
    }
    me->set_temp(E_LST+"Hint/"+eid,hint);
  }
}

string query_event_hint(object me,string eid) {
  if(save_type(eid))
  return me->query(E_LST+"Hint/"+eid);
 
  return me->query_temp(E_LST+"Hint/"+eid);
}
//------------------------------------


//事件入口设置相关函数
varargs void set_hb_event(object me,string eid,int lasting,int delay) {
  mapping list,event;
  string fid = base_name(this_object())[6..]+"_"+eid;    
  int start = delay + time();
  
  if(!mapp(event=events[eid])) {
    tell_object(me,HIR"任务事件["+eid+"]设置错误，请报告巫师。\n"NOR);
    return;
  }  
  if(event["online"]) {
    list = me->query_temp(T_HB);
    if(!list) list = me->set_temp(T_HB,([]));
  }
  else {
    list = me->query(T_HB);
    if(!list) list = me->set(T_HB,([]));
  }
  
  set_event_hint(me,eid);
  
  if(!arrayp(list[fid])) {
    //心跳事件描述数组结构 ({开始时间,结束时间,事件文件,事件函数,类型}) 
    list[fid] = ({start,start+lasting,base_name(this_object()),event["event"],eid,event["hb_type"]});
  }
  else{
    if(!lasting) {
      list[fid][0] = start;
      list[fid][1] = start;
    }
    else list[fid][1] = list[fid][1]+lasting;    
  }  
}

void set_user_event(object me,string eid) {
  mapping event,list,plc,cmds;
  string hint;
   
  if(!mapp(event=events[eid])) {
    tell_object(me,HIR"任务事件["+eid+"]设置错误，请报告巫师。\n"NOR);
    return;
  } 
  
  if(event["type"]==TYP_HB) {
    set_hb_event(me,eid);
    return;
  }
  
  if(save_type(eid)) {
    list = me->query(T_NPC);
    plc  = me->query(T_PLC);
    cmds = me->query(T_CMD);
    if(!cmds) cmds = me->set(T_CMD,([]));
    if(!list) list = me->set(T_NPC,([]));  
    if(!plc)  plc  = me->set(T_PLC,([]));
  }
  else {
    list = me->query_temp(T_NPC);
    plc  = me->query_temp(T_PLC);
    cmds = me->query_temp(T_CMD);
    if(!cmds) cmds = me->set_temp(T_CMD,([]));
    if(!list) list = me->set_temp(T_NPC,([]));  
    if(!plc)  plc  = me->set_temp(T_PLC,([]));
  }  
 
  set_event_hint(me,eid);  
  if(event["npc"] && event["type"]&(TYP_TALK+TYP_EF)) {
    if(arrayp(list[event["npc"]]))
    list[event["npc"]] += ({ ({base_name(this_object()),eid,event["type"]}) });
    else
    list[event["npc"]] = ({ ({base_name(this_object()),eid,event["type"]}) });
  }
  else if(event["place"] && event["type"]&TYP_GO) {
    if(arrayp(list[event["place"]]))
    plc[event["place"]] += ({ ({base_name(this_object()),eid,event["type"]}) });
    else
    plc[event["place"]] = ({ ({base_name(this_object()),eid,event["type"]}) });
  }
  else if(event["type"]&TYP_ACTION) {    
    foreach(string item in event["cmds"]) {
      if(!arrayp(cmds[item]))
      cmds[item] = ({ ({base_name(this_object()),eid}) });
      else 
      cmds[item] += ({ ({base_name(this_object()),eid}) });
    }
  }
}

void delete_user_event(object me,string eid){
  mapping event,list;
  mixed elist;

  if(!mapp(event=events[eid])) return;

  if(event["type"] == TYP_HB) {
    if(event["online"]) list = me->query_temp(T_HB);
    else list = me->query(T_HB);
    map_delete(list,base_name(this_object())[6..]+"_"+eid);
    return;
  }
  if(event["type"] & (TYP_TALK+TYP_EF)) {
    if(save_type(eid)) list = me->query(T_NPC);
    else list = me->query_temp(T_NPC);
    if(!mapp(list)) return;
    if(sizeof(elist=list[event["npc"]])>0)
    list[event["npc"]] = filter_array( elist, (: 
      $1[1]!=$(eid) || base_name(this_object())!=$1[0] || $1[2]!=$(event["type"])
    :));
    if(!sizeof(list[event["npc"]])) map_delete(list,event["npc"]);
    return;
  }
  if(event["type"] == TYP_GO) {
    if(save_type(eid)) list = me->query(T_PLC);
    else list = me->query_temp(T_PLC);
    if(sizeof(elist=list[event["place"]])>0)
    list[event["place"]] = filter_array( elist, (: $1[1]!=$(eid) || base_name(this_object())!=$1[0] :));
    if(!sizeof(list[event["place"]])) map_delete(list,event["place"]);
    return;
  }
  if(event["type"] & TYP_ACTION) {
    if(save_type(eid)) list = me->query(T_CMD);
    else list = me->query_temp(T_CMD);
    foreach(string item in event["cmds"]) {
      if(sizeof(elist=list[item])>0)
      list[item] = filter_array( elist, (: $1[1]!=$(eid) || base_name(this_object())!=$1[0] :));
      if(!sizeof(list[item])) map_delete(list,item);
    }
    return;
  }
}
//------------------------------------


//事件进行相关函数
string event_rst(object me,string eid) {
  string prop,*list;
  mapping rst;
  mixed value;

  rst=events[eid]["rst"];
  
  if(functionp(rst)) return evaluate(rst,me);
  
  if(!mapp(rst)) return 0;
  
  list = keys(rst);
  
  foreach(string item in list){    
    if(sscanf(item,"%s>%d",prop,value)==2) {
      if(me->query(prop)>value) return rst[item];      
    }
    else if(sscanf(item,"%s<%d",prop,value)==2) {
      if(me->query(prop)<value) return rst[item];      
    }
    else if(sscanf(item,"%s=%d",prop,value)==2) {
      if(me->query(prop)==value) return rst[item];      
    }
    else if(sscanf(item,"%s!=%d",prop,value)==2) {
      if(me->query(prop)!=value) return rst[item];      
    }
    else if(sscanf(item,"%s=%s",prop,value)==2) {
      if(me->query(prop)==value) return rst[item];      
    }
    else if(sscanf(item,"%s!=%s",prop,value)==2) {
      if(me->query(prop)!=value) return rst[item];      
    }
  }  
  return 0;
}

void event_end(object me,string eid){
  mapping reward = events[eid]["reward"];
  if(mapp(reward)) TASK_D->event_reward(me,reward,task);
}

void pre_set_next(object me,string next) {
  if(!mapp(events[next])) 
  tell_object(me,HIR"Next event id error!!!\n"NOR);
  me->set_temp(E_LST+"P_Next",next);
}

varargs int event_prog(string eid,mixed me,mixed ob,string recall){  
  mixed rst;
  int flag,status;  
  
  if(stringp(recall)) {
    flag = call_other(this_object(),recall,eid,ob,me); 
    foreach(string item in keys(events)) 
    if (events[item]["event"] == recall) eid = item;
  }
  else if(events[eid]["type"] == TYP_HB)
    flag = call_other(this_object(),events[eid]["event"],me,ob);
  else {
    status = query_event_status(me,eid);
    if(query_status(me) & STAT_FIN){      
      if(!task["RPTL"]) {        
        tell_object(me,ob->name()+"说道：你的【"+query_task_name()+"】任务已经完成了。\n");
        return 0;
      }     
    }
    if(task["limit_day"] && me->check_day_flag(task["id"]) >= task["limit_day"] ) {
       tell_object(me,ob->name()+"说道：你今天已无法继续【"+query_task_name()+"】任务了。\n");
       return 0;
    }
    if(status & STAT_PEND){
      tell_object(me,STAT_PEND+ob->name()+"说道：你不是已经领了【"+query_event_name(eid)+"】任务吗？还不快去做。\n");
      return 0;
    }
    if(status & STAT_FAIL){
      tell_object(me,ob->name()+"说道：你的【"+query_event_name(eid)+"】任务已经失败了。\n");
      return 0;
    }
    if(status & STAT_END){
      //tell_object(me,ob->name()+"说道：你的【"+query_event_name(eid)+"】任务已经完成了。\n");
      return 0;
    }
    if(status == STAT_OFF){
      tell_object(me,"现在【"+query_event_name(eid)+"】任务暂时没有开放。\n");
      return 0;
    }
    if(stringp(rst=event_rst(me,eid))){
      tell_object(me,ob->name()+"说道："+rst+"\n");
      return 0;
    }
    flag = call_other(this_object(),events[eid]["event"],me,ob);   
  }  

  if(!flag) return 0;
  
  set_event_status(me,eid,flag);  
  if(flag & STAT_PROG){
    mixed next;    
    
    if(next=me->query_temp(E_LST+"P_Next")) {
       me->delete_temp(E_LST+"P_Next");
    }
    else if(functionp(next=events[eid]["next"]))
    next = evaluate(next,me);    
    
    if(stringp(next)) {
      set_event_status(me,next,STAT_ACT);
      set_user_event(me,next);    
    } 
    else if(arrayp(next)) {
      foreach(string item in next) {        
        set_event_status(me,item,STAT_ACT);
        set_user_event(me,item); 
      }           
    }
  }

  if(flag & STAT_FAIL){
    delete_user_event(me,eid); 
    del_flag(me,"CLR_ALL");
    set_status(me,STAT_FAIL);
    return 1;
  }   
  if(flag & STAT_PASS){
    delete_user_event(me,eid); 
    set_event_status(me,eid,STAT_END);
  }
  if(flag & STAT_END){
    delete_user_event(me,eid); 
    set_event_status(me,eid,STAT_END);    
    event_end(me,eid);
  }    
  if(flag & STAT_FIN){
    delete_user_event(me,eid); 
    del_flag(me,"CLR_ALL");
    set_status(me,STAT_FIN);
    if(task["limit_day"]) me->set_day_flag(task["id"]);
    if(task["RPTL"]) me->add("Task/times/"+task["id"],1);
  }
  
  return 1;
}

void event_choice(string msg,mixed ob,object me){
  tell_object(me,msg);
  input_to("event_prog",me,ob,call_stack(2)[1]);
}
//------------------------------------

//任务对象初始化函数
void setup(){  
  foreach(string item in keys(events)){
    if(!stringp(events[item]["event"])) {
      CHANNEL_D->do_channel( this_object(),"err",
        sprintf("%s event miss!",item)
      );
      continue;
    }
    if(!function_exists(events[item]["event"],this_object())){
      CHANNEL_D->do_channel( this_object(),"err",
        sprintf("%s function miss!",item)
      );
      continue;
    }
    if(mapp(events[item]["reward"])) {
      string *list = TASK_D->query_reward_type();
      foreach(string type in keys(events[item]["reward"])){
        if(member_array(type,list)==-1)
          CHANNEL_D->do_channel( this_object(),"err",
            sprintf("%s undefined reward type '%s'!",item,type)
          );
      }
    }
  }  
}
//------------------------------------