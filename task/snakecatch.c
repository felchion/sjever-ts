inherit TASK;
#include <task.h>

int task_on() {return 1;}

mapping task = ([
    "name"      : "丐帮捉蛇",
    "cid"       : "捉蛇",
    "on"        : 1,
    "id"        : __FILE__[6..<3],
    "start"     : "start",
    "hardness"  : 0,    
    "RPTL"      : 1,    
    "limit_day" : 1,
]);

mapping events = ([
  "start" : ([
    "name"   : "询问工作",
    "npc"    : "/kungfu/class/gaibang/liang",
    "type"   : TYP_TALK,
    "event"  : "start",    
    "next"   : "catch",
  ]),
  "catch" : ([
    "prev"   : "start",
    "name"   : "捉蛇",
    "type"   : TYP_ACTION,
    "event"  : "do_catch",
    "hint"   : "到"HIY"苏州南郊"NOR"野外找蛇("HIG"find snake"NOR")。",
    "cmds"   : ({"find"}), 
    "next"   : "finish"
  ]),
  "finish" : ([
    "pre"    : "catch",
    "name"   : "领取奖励",
    "npc"    : "/kungfu/class/gaibang/liang",
    "hint"   : "回到"HIY"杏子林"NOR"和"HIG"梁长老"NOR"交谈。", 
    "type"   : TYP_TALK,
    "reward" : ([
     "gold"   : 1000, 
    ]),
    "event"  : "reward"
  ]),
  "finish2" : ([
    "name"   : "领取奖励",
    "npc"    : "/kungfu/class/gaibang/liang",
    "hint"   : "继续和"HIG"梁长老"NOR"交谈。", 
    "type"   : TYP_TALK,
    "reward" : ([
      "gold"   : 200000, 
    ]),
    "event"  : "reward2"
  ]),
  "poison"  : ([
    "name"   : "蛇毒",
    "type"   : TYP_HB,
    "event"  : "hb_poison"
  ])
]);


void create() {
  set_events(events);
  set_task(task);
  setup();
}

int hb_poison(object me){
  if(check_flag(me,"poison_dur")<3) {
    set_flag(me,"poison_dur");
    return STAT_CNT;
  }
  del_flag(me,"poison_dur");
  me->receive_wound("jing" , me->query("max_jing")/10);
  me->receive_damage("jing", me->query("max_jing")/10);
  me->set_temp("last_damage_from", "蛇毒发作");
  if (living(me))
  message_vision(BLU "$N两眼发直，身子瑟瑟抖了起来！\n" NOR, me);
  else
  message_vision(BLU "$N痛苦地呻咛了一声！\n" NOR, me);
  me->add_busy(random(2));
  return STAT_CNT;
}

int look_lou(object ob,string arg) {
  object me = this_player();
  string str;
  if(arg=="lou" || arg=="zhu lou") {
    str = check_flag(me,"s_list");
    if(!str) tell_object(me,"一个竹子编制的篓子，里面什么也没有。\n");
    else tell_object(me,"一个竹子编制的篓子，里面有：\n" + str);    
    return 1;
  }  
  return 0;
}

int catch_snake(object ob,string arg) {
  object me = this_player();
  int h = ob->query("hardness");  
  if(arg=="snake") {
    if(me->is_busy()) {
      tell_object(me,YEL"你正忙着呢。\n"NOR);
      return 1;
    }
    if (check_flag(ob,"founder")==me){
      string str = check_flag(me,"s_list");
      if(h<10 && !random(h)) {
        if(!str) str = "";
        set_flag(me,"snake");
        set_flag(me,"s_list",str+"  "+ob->name()+"\n");        
        message_vision("$N一个眼疾手快，一把抓住$n的七寸，收进了竹篓里。\n",me,ob);
        if(bossp(ob)) {
          set_flag(me,"snake",7);
          set_flag(me,"got_boss");
        }
        destruct(ob);
        me->add_busy(1);
        return 1;
      }
      else{
        message_vision("$N一把抓向$N的七寸，却差了几分，被$n一口咬住了虎口。\n",me,ob);
        if(QSKILL_D->quest_passed(me,"bdbq")) {
          if(h>10) {
            ob->add("hardness",-1);
            message_vision("$N一个吃痛，慌忙甩手将$n扔在地上。\n",me,ob);
            return 1;
          }          
          message_vision("幸而$N已是百毒不侵，忍着疼痛随手将$n收进了竹篓里。\n",me,ob);
          if(!str) str = "";
          set_flag(me,"snake");
          set_flag(me,"s_list",str+"  "+ob->name()+"\n");        
          message_vision("$N一个眼疾手快，一把抓住$n的七寸，收进了竹篓里。\n",me,ob);
          if(bossp(ob)) {
            set_flag(me,"snake",7);
            set_flag(me,"got_boss");
          }
          destruct(ob);
          me->add_busy(1);
          return 1;
        }
        else {
          message_vision("$N一个吃痛，慌忙甩手将$n扔在地上。\n",me,ob);  
          if(h>10) {
            if(random(2)) ob->add("hardness",-1);
            set_hb_event(me,"poison",1);
          }
          else {
            ob->kill_ob(me);                  
            set_hb_event(me,"poison",1+h/2);           
          } 
          me->add_busy(1);
          tell_object(me,BLU"你好象中毒了。\n"NOR);
          return 1;
        }
      }
    }
    message_vision("$N一把抓向$N的七寸，却差了几分，被$n一口咬住了虎口。\n",me,ob);
    message_vision("$N一个吃痛，慌忙甩手将$n扔在地上。\n",me,ob);
    if(!QSKILL_D->quest_passed(me,"bdbq")) {
      set_hb_event(me,"poison",h*10);
      tell_object(me,BLU"你好象中毒了。\n"NOR);
    }
    me->add_busy(1);
    return 1;   
  }
  return 0;
}

void _init(object me,object ob){
  if (check_flag(me,"founder")==ob) {
    message_vision("突然$n发现草丛中有一条$N。\n",me,ob);
    me->set_leader(ob);
    ob->start_busy(2+random(3)); 
  }
}

int _kill_ob(object me,object ob){
  if (check_flag(me,"founder")!=ob){
    me->remove_killer(ob);
    ob->remove_killer(me);
    message_vision("$N发现$n根本不理$P！\n",ob,me);
    return 1;
  }
  return 0;
}

int _die(object me,object ob){
  if(me->query("hardness")>10) {
    me->reincarnate();
    return 1;
  }
  if(check_flag(me,"founder")==ob)
  {

    string str = check_flag(ob,"s_list");
    if(!str) str = "";
    set_flag(ob,"snake");
    set_flag(ob,"s_list",str+"  "+me->name()+"\n");
    message_vision ("\n$N缩成一团，不动了。\n$n伸手抓起$N放进竹篓中。\n",me,ob);
    if(bossp(me)) {
      set_flag(ob,"snake",7);
      set_flag(ob,"got_boss");
    }
    destruct(me);
    return 1;
  }
  return 0;
}

int _unconcious(object me,object ob){
  if(me->query("hardness")>10) {
    me->reincarnate();
    return 1;
  }
  if( check_flag(me,"founder")==ob)
  {
    string str = check_flag(ob,"s_list");
    if(!str) str = "";
    set_flag(ob,"snake");
    set_flag(ob,"s_list",str+"  "+me->name()+"\n");
    message_vision ("\n$N缩成一团，不动了。\n$n伸手抓起$N放进竹篓中。\n",me,ob);
    if(bossp(me)) {
      set_flag(ob,"snake",7);
      set_flag(ob,"got_boss");
    }
    destruct(me);
    return 1;
  }
  return 0;
}

void _heart_beat(object me) {
  object ob = check_flag(me,"founder");
  int h=me->query("hardness");
  
  if(objectp(ob) && me->is_fighting(ob)){
    if(!random(40-h)) {
      message_vision(HIR"$n一个不留神，被$N一口咬在脚脖子上。\n"NOR,me,ob);
      if(!QSKILL_D->quest_passed(ob,"bdbq")) {
        tell_object(ob,BLU"你好象中毒了。\n"NOR);
        set_hb_event(ob,"poison",1+random(5));
      }
      ob->add_busy(random(2));
    }
  }
}

object new_snake(object ob) {
  int number,k_qi,k_neili,k_jing,k_jingli,p;
  string name,title;
  mapping snake;  
  
  switch (random(11))
  {
    case 0: name=HIG"小青蛇"NOR;p=1;title="无毒的";break;
    case 1: name=YEL"菜花蛇"NOR;p=1;title="无毒的";break;
    case 2: name=BLU"眼镜蛇"NOR;p=8;title=MAG"剧毒的"NOR;break;
    case 3: name=HIW"白花蛇"NOR;p=5;title=HIM"有毒的"NOR;break;
    case 4: name=HIC"五步蛇"NOR;p=6;title=MAG"剧毒的"NOR;break;
    case 5: name=WHT"银环蛇"NOR;p=3;title=HIM"有毒的"NOR;break;
    case 6: name=HIY"响尾蛇"NOR;p=2;title=HIM"有毒的"NOR;break;
    case 7: name=CYN"四脚蛇"NOR;p=1;title="无毒的";break;
    case 8: name=HIY"金环蛇"NOR;p=4;title=HIM"有毒的"NOR;break;
    case 9: name=HIR"赤练蛇"NOR;p=2;title=HIM"有毒的"NOR;break;
    case 10: name=HIB"野鸡脖子"NOR;p=7;title=MAG"剧毒的"NOR;break;
  }

  if(!random(800)) {
    name = ansi_color(237) + "黑曼巴" + NOR;
    p = 30;
    title = HIR"最扁的蝙蝠蛇"NOR;
  }
  
  k_qi = (int)ob->query("max_qi");
  k_jing = (int)ob->query("max_jing");
  k_jingli = (int)ob->query("eff_jingli");
  k_neili = (int)ob->query("max_neili");
  
  snake = ([
    "dbase" : ([
      "title" : title,
      "chat_chance": 10,
      "long" : "这是一条"+name+"，你可试着捉住它("HIY"catch snake"NOR")。\n",
      "chat_msg":({
        "一条"+name+"在草丛中盘成一团。\n",
        name+"爬过来，在你的腿边爬来爬去。\n",
        name+"向着你，慢慢爬了过来。\n",
      }),
      "max_qi":k_qi*3/2 +random(100),
      "eff_qi":k_qi*3/2 +random(100),
      "qi":k_qi*3/2 +random(100),
      "max_jing":k_jing,
      "eff_jing":k_jing,
      "eff_jingli":k_jingli,
      "max_jingli":k_jingli,
      "jingli":k_jingli,
      "jing":k_jing,
      "max_neili":k_neili,
      "neili":k_neili*2-1,
      "jiali":500,
      "hardness" :p
    ]),
    "call" : ({
      ({ "set_name",name,({"snake"}) }),
    }),
    "function" : ([
      "init"        : "_init",
      "kill_ob"     : "_kill_ob",
      "die"         : "_die" ,
      "unconcious"  : "_unconcious",
      "heart_beat"  : "_heart_beat",
    ]),
    "cmds" : ([
      "catch" : "catch_snake"
    ]),
    "dest_time" : 300+p*10,
    "dest_desc" : HIW"$N扭了扭尾巴，不知怎么就不见了。\n"NOR,
  ]);
  return new(NPC_M,snake);  
}

varargs int start(mixed arg,object ob,object me){
  if(userp(arg)) {
    string msg=HIG+ob->name()+"说道：不知道这位"+RANK_D->query_respect(arg)+"可否帮我一个小忙。["HIY"Y|n"HIG"]："NOR;
    event_choice(msg,ob,arg);
    return STAT_CNT;
  }
  else if(arg[0]=='y' || arg[0]=='Y') {
    object obj;
    mapping obj_data = ([
      "dbase" : ([
        "unit": "个",                    
        "long": "一个竹子编制的篓子。\n",          
        "material": "bamboo",           
        "owner":me->query("id"),
      ]),
      "cmds" : ([
        "look" : "look_lou"
      ]),
      "call" :({
        ({ "set_name",YEL"竹篓"NOR,({ "zhu lou", "lou"}) })
      })    
    ]);
    
    obj = new(K_ITM,obj_data);
    if(objectp(obj)) {
      set_flag(me,"tool",obj);
      obj->move(me);
      tell_object(me, CYN+ob->name()+"说道：「我帮要布蛇阵需要些蛇，烦请"+RANK_D->query_respect(me)+"去捉一些吧。」\n"NOR);
      tell_object(me, CYN+ob->name()+"说道：「据说苏州城南郊有大量毒蛇出没。」\n"NOR);
      message_vision("$N给$n一个"YEL"竹篓"NOR"。\n",ob,me);
    }
    else {
      tell_object(me, CYN+ob->name()+"说道：「我的竹篓发完了，你还是晚点再来吧。」\n"NOR);
      return STAT_ACT;
    }
    return STAT_PROG+STAT_END;
  }
  else {
    tell_object(me,CYN+ob->name()+"说道：那真是太遗憾，希望"+RANK_D->query_respect(me)+"有空时，能为"+RANK_D->query_self(ob)+"分忧。\n");
    return STAT_ACT;
  }
}

int do_catch(object me,string arg)
{
  string *place = ({
    "/d/suzhou/caixiangjing",
    "/d/suzhou/shiyuan",
    "/d/suzhou/mubei",
    "/d/suzhou/yingxiaoting",
    "/d/suzhou/lingyanshan",
    "/d/suzhou/tianpingshan",
    "/d/suzhou/baiyunquan",
    "/d/suzhou/shanpo",
    "/d/suzhou/shihu"
  });
  object wp=me->query_temp("weapon"),plc = environment(me);
  string env = base_name(plc);
  int i;

  if(me->is_busy()) {
    tell_object(me,"你正忙着呢。\n");
     return STAT_CNT;
  }
  if(arg!="snake") {
    tell_object(me,"你要找什么？\n");
    return STAT_CNT;
  }

  if(!objectp(check_flag(me,"tool"))) {
    tell_object(me, "你正准备捉蛇却发现竹篓不见了。\n"NOR);
    return STAT_FAIL+STAT_FIN;
  }	
  
  if(present("snake",environment(me))){
    tell_object(me,"这里不是已经有蛇了吗，快点捉起来吧。\n");
    return STAT_CNT;
  }
  
  if(!wp || !objectp(wp)) {
    tell_object(me,"你空手在地上翻来翻去。\n");
    return STAT_CNT;
  }

  if(member_array(env,place) == -1) {
    tell_object(me,"你在地上翻来翻去，结果什么都没发现。\n");
    return STAT_CNT;
  }

  message_vision("$N操起"+wp->name()+"在草丛、石堆里翻来翻去。\n",me);

  if(check_flag(me,env[10..]+"_found")) {
    message_vision("$N忙活了半天，结果什么都没发现。\n",me);
    tell_object(me,"看来这里已经没有蛇了，还是到别外找找吧。\n");
    return STAT_CNT;
  }

  if(check_flag(me,"snake")>5){
    message_vision("$N忙活了半天，结果什么都没发现。\n",me);
   // pre_set_hint(me,"finish","回到"HIY"杏子林"NOR"和"HIG"梁长老"NOR"交谈。");
    tell_object(me,HIY"看来今天这附近的蛇都已经抓光了，还是回去交差吧。\n"NOR);
    return STAT_END+STAT_PROG;
  }

  if(check_flag(me,env[10..]) > 5 + random(5)) {
    object snake;
        
    snake=new_snake(me);
    if(objectp(snake)) {
      set_flag(snake,"founder",me);
      if(snake->query("hardness")>10){
        snake->set("is_head",1);
        CHANNEL_D->do_channel(this_object(),"rumor","据说"+HIY+plc->query("short")+HIY"出现了一只"+snake->query("title")+HIY"。\n"NOR);
      }
      snake->move(plc);
      set_flag(me,env[10..]+"_found");
    }
    return STAT_CNT;
  }

  message_vision("$N忙活了半天，结果什么都没发现。\n",me);
  me->start_busy(1);
  set_flag(me,env[10..]);
  return STAT_CNT;
}


int reward(object me,object ob){
  int obj=check_flag(me,"tool"); 
  
  if(!objectp(obj)) {
    tell_object(me, CYN+ob->name()+"说道：「咦，你的竹篓呢！」\n"NOR);
    return STAT_FAIL+STAT_FIN;
  }
	
  if(!check_flag(me,"got_boss")) {
    message_vision("$N给$n一个"YEL"竹篓"NOR"。\n",me,ob);
	  destruct(obj);	
	  tell_object(me, CYN+ob->name()+"说道：「好，"+RANK_D->query_respect(ob)+"，做得好，这是给你的奖赏！」\n"NOR);	
	  return STAT_END+STAT_FIN;
  }
  else {
    tell_object(me, CYN+ob->name()+"说道：「"+RANK_D->query_respect(ob)+"，居然能捕获如此神物！」\n"NOR);	
	  pre_set_next(me,"finish2");
	  return STAT_PASS+STAT_PROG;
  }
}


int reward2(object me,object ob){
  int obj=check_flag(me,"tool"); 
  
  if(!objectp(obj)) {
    tell_object(me, CYN+ob->name()+"说道：「咦，你的竹篓呢！」\n"NOR);
    return STAT_FAIL+STAT_FIN;
  }
	
	message_vision("$N给$n一个"YEL"竹篓"NOR"。\n",me,ob);
	destruct(obj);	
	tell_object(me, CYN+ob->name()+"激动地说道：「好！好！好！"+RANK_D->query_respect(ob)+"真是本帮的大福星啊！这些黄金不成敬意，敬请笑纳！」\n"NOR);
	
	return STAT_END+STAT_FIN;
}

