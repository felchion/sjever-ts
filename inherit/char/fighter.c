//标准战士文件 from Campsun
//target 为 player's id 必须

inherit NPC;
#include <skills_pfm.h>
#include <ansi.h>
#include <r_name.h>

string *str_menpai = ({ "wd","hs","ss","gm","gb","tz","dls","mr","sld","em","mj","thd","tls","sl","kl"});

varargs void setparty(int p,int lv,int exp,int l)
{
  object me = this_object();
  exp = lv/10*lv*lv;

  if (p>=7)
  {
    str_menpai=str_menpai-({"9y","hmg","pxj","jsj","xx"});
    if(!random(20)) str_menpai=str_menpai+({"9y"});
    if(!random(25)) str_menpai=str_menpai+({"hmg"});
    if(!random(15)) str_menpai=str_menpai+({"pxj"});
    if(!random(15)) str_menpai=str_menpai+({"jsj"});
    if(!random(5)) str_menpai=str_menpai+({"xx"});
  }

  me->set("jiali",30*p);
  me->set("hardness",p);
  me->set("extralife",l);
  if (p>10) p = 10; 
  if (p<1) p=1;
  
  switch( p ){
    case 1:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),0,1,10);
    set_skills_level(lv - 10);
    me->set("combat_exp",exp);
    break;
    case 2:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),0,1,20);
    set_skills_level(lv - 5);
    me->set("combat_exp",exp);
    break;
    case 3:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(2),1,30);
    set_skills_level(lv - random(5));
    me->set("combat_exp",exp + random(50000));
    break;
    case 4:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(2),1,40);
    set_skills_level(lv);
    me->set("combat_exp",exp );
    break;
    case 5:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(3),1,50);
    set_skills_level(lv + random(10));
    me->set("combat_exp",exp );
    break;
    case 6:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(3),1,60);
    set_skills_level(lv + 10 + random(5));
    me->set("combat_exp",exp );
    break;
    case 7:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(3),1,70);
    set_skills_level(lv + 15 + random(5));
    me->set("combat_exp",exp );
    break;
    case 8:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(3),1,80);
    set_skills_level(lv + 20 + random(5));
    me->set("combat_exp",exp );
    break;
    case 9:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(4),1,90);
    set_skills_level(lv + 25 + random(5));
    me->set("combat_exp",exp );
    break;
    case 10:
    copy_menpai(({str_menpai[random(sizeof(str_menpai))]}),random(4),1,100);
    set_skills_level(lv + 30 + random(5));
    me->set("combat_exp",exp);
    break;
  }
  if (me->query("family/family_name") == "星宿派")
   	me->set("env/xx_poison","内敛");

  get_npc_name();
  set("long", me->query("long")+"此人看上去师承"+HIY+me->query("family/family_name")+NOR+"，擅长使用"+HIC+to_chinese(me->query("f_w_skill"))+NOR+"伤敌！\n" );
}

int checking(object me, object ob)
{
  if(!me || !ob) return 0;
  if((!me->query("qi") >= me->query("max_qi")/2) && (me->query_temp("cure") < 2)){
    me->add("qi",me->query("max_qi")/4);
    me->add_temp("cure",1);
    tell_room(environment(me), me->query("name")+"深深的吸了口气，脸色顿时好了许多。\n", ({me}));
  }
  if(!me->query_temp("weapon") && !me->is_busy() && !me->is_perform())
  {
    me->map_skill("parry",me->query("nw_parry"));
    me->set("chat_msg_combat", query("u_f_pfm") + query("u_pfm"));                                 //出空手pfm
  }
  //else
  if(ob->is_fighting(me)){
    // remove_call_out("checking");
    // call_out("checking", 1, me, ob);
    return 1;
  }
  else if(!me->is_killing(ob->query("id"))){
    ob->kill_ob(me);
    //remove_call_out("checking");
    //call_out("checking", 1, me, ob);
    return 1;
  }
  return 1;
}

void kill_ob(object ob)
{
  object obj,pp;
  obj = this_object();

  if(!obj->query_temp("target"))
  {
    ::kill_ob(ob);
    return;
  }
  pp = find_player(obj->query_temp("target"));
  if(!ob) return;
  if((obj->query_temp("target") != ob->query("id") && !query("can_guard_ob")  )
  && !ob->query("env/invisibility") && objectp(pp) ){
    tell_room(environment(obj), obj->query("name")+"嚷嚷道：走远点，老子正忙着呢。！\n", ({obj}));
    obj->remove_enemy(ob);
    ob->remove_killer(obj);
    return;
  }
  //  ::kill_ob(ob);
  ob->kill_ob(obj);
}

int do_kill(object ob)
{
  object me;
  int lv,exp,b_exp,p,l;

  if(!ob) return 0;

  me = this_object();
  lv = (int)ob->query("max_pot");
  lv = lv-100;
  exp = ob->query("combat_exp");
  b_exp = 300000;
  p = query("hardness");
  l = query("extralife");             //add by campsun

  set("max_qi",ob->query("max_qi") + ob->query("max_qi") * p /10 + ob->query("max_qi")*l /3 - ob->query_temp("adjust/qi"));
  set("max_jing", ob->query("max_jing") + ob->query("max_jing") * p /10 + ob->query("max_jing")*l /3 - ob->query_temp("adjust/jing"));

  copy_state();

  set("max_neili",(query_skill("force")-query_temp("apply/force"))*12 + exp/1000 + query_temp("apply/neili"));
  set("max_jingli",query_skill("force")*12);

  set("neili",query("max_neili")*2);
  set("jingli",query("max_jingli")*2);
  set("eff_jingli",query("max_jingli"));
  set("qi",query("max_qi"));
  set("eff_qi",query("max_qi"));
  set("jing",query("max_jing"));
  set("eff_jing",query("max_jing"));

  if ( !random (100) && exp > 1000000)
 	 me->set("double_attack", 1);

  me->delete("chat_msg");
  ob->kill_ob(me);
}
