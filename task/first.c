//测试
inherit "/inherit/misc/task";
#include <task.h>


int task_on() {return 1;}


mixed query(string arg){
  if(arg=="name") return task["name"];
  if(arg=="id") return __FILE__;
}


mapping npc_list = ([
  

]);

mapping task = ([
    "name"      : "拜山",
    "cid"       : "武馆",
    "on"        : 1,
    "id"        : "wuguan",
    "start"     : "start",
    "hardness"  : 0,    
    "offline"   : 0,
   // "RPTL"      : 1
]);


string *plist = ({"hs","kl","ss","em","thd","wd","sld","tls","mj","mr","xx","sl","dls","tz","gb","ts"});

mapping events = ([
  "start" : ([
    "name"   : "闲聊八卦",
    "npc"    : "/d/wuguan/npc/menwei",
    "type"   : TYP_TALK,
    "event"  : "step1",
    "next" : "hs",
    "reward"   : ([
      "gold"   : 50,
    ]),
  ]),
  "hs" : ([
    "name"      : "华山派拜山",
    "npc" : "/d/huashan/npc/ningzhongze",
    "place" : "/d/huashan/zhengqi",
    "type"   : TYP_TALK,
    "reward"   : ([
      "exp"   : 200000,
    ]),
    "word"     : "我就和$R交流些战斗心得吧。\n",
    "event"  : "do_greet",
    "next" : "kl"
  ]),
  "kl" : ([
    "name"      : "昆仑派拜山",
    "npc" : "/kungfu/class/kunlun/xihuazi",
    "place" : "/d/kunlun/qianting",
    "type"   : TYP_TALK,
    "reward"   : ([
      "pot"   : 50000,
    ]),
    "word"     : "没有潜能就无法学习新的技能。\n"  ,
    "event"  : "do_greet",
    "next" : "ss"
  ]),
  "ss" : ([
    "name"      : "嵩山派拜山",
    "npc" : "/kungfu/class/songshan/ding",
    "place" : "/d/songshan/chanyuan",
    "type"   : TYP_TALK,
    "reward"   : ([
      "jl"   : 1000,
    ]),
    "word"     : "祝这位$R，龙马精神，精力充沛。\n" ,
    "event"  : "do_greet",
    "next" : "em"
  ]),
  "em" : ([
    "name"      : "峨嵋派拜山",
    "npc" : "/kungfu/class/emei/jing-xian",
    "place" : "/d/emei/qingyinge",
    "type"   : TYP_TALK,
    "reward"   : ([
      "nl"   : 2000,
    ]),
    "word"     : "内力修为是武学的根本。\n" ,
    "event"  : "do_greet",
    "next" : "thd"
  ]),
  "thd" : ([
    "name"      : "桃花岛拜山",
    "npc" : "/d/thd/guiyun/npc/lugy",
    "place" : "/d/thd/guiyun/qianting",
    "type"   : TYP_TALK,
    "reward"   : ([
      "gold"   : 10000,
    ]),
    "word"     : "行走江湖，没钱可是寸步难行，本庄就资助$R一些黄金。\n",
    "event"  : "do_greet",
    "next" : "wd"
  ]),
  "wd" : ([
    "name"      : "武当派拜山",
    "npc"    : "/kungfu/class/wudang/guxu",
    "place"  : "/d/wudang/fuzhen",
    "type"   : TYP_TALK,
    "reward" : ([
      "yhb"  : 100,
    ]),
    "word"   : "永恒币是这个世界最宝贵的东西，可以在蝶梦楼换到很多奇怪道具。\n",
    "event"  : "do_greet",
    "next" : "sld"
  ]),
  "sld" : ([
    "npc" : "/d/sld/npc/lgx",
    "place" : "/d/sld/lgxroom",
    "type"   : TYP_TALK,
    "name"      : "神龙教拜山",
    "reward"   : ([
      "aexp"   : 500000,
    ]),
    "word"   : "不断精研武技才是武者本色啊。\n" ,
    "event"  : "do_greet",
    "next" : "tls"
  ]),
  "tls" : ([
    "name"      : "天龙寺拜山",
    "npc" : "/d/tls/npc/liao-huo",
    "place" : "/d/tls/road",
    "type"   : TYP_TALK,
    "reward"   : ([
      "jjb"   : 500,
    ]),
    "word"     : "狭路相逢勇者胜，多去蝶梦楼会有好处的，有些竞技币就送给$R。\n",
    "event"  : "do_greet",
    "next" : "mj"
  ]),
  "mj" : ([
    "name"      : "明教拜山",
    "npc" : "/kungfu/class/mingjiao/yin2",
    "place" : "/d/mingjiao/qianting",
    "type"   : TYP_TALK,
    "reward"   : ([
      "jtp"   : 300,
    ]),
    "word"     : "这些精铁就送给$R了，希望有日能用得上。\n",
    "event"  : "do_greet",
    "next" : "mr"
  ]),
  "mr" : ([
    "name"      : "慕容派拜山",
    "npc" : "/d/mr/npc/bao",
    "place" : "/d/mr/xiaoshe",
    "type"   : TYP_TALK,
    "reward"   : ([
      "sw"   : 1000,
    ]),
    "word"     : "人活一张皮，没有声望可是什么事都做不成啊，我会多多传扬$R的声名。\n",
    "event"  : "do_greet",
    "next" : "xx"
  ]),
  "xx" : ([
    "name"      : "星宿派拜山",
    "npc" : "/d/xingxiu/npc/feitian",
    "place" : "/d/xingxiu/xx1",
    "type"   : TYP_TALK,
    "reward"   : ([
      "ore"   : 200,
    ]),
    "word"     : "这些石头就送给$R了，据说扬州某处可以用这些石头换到一些稀有的宝石。\n",
    "event"  : "do_greet",
    "next" : "sl"
  ]),
  "sl" : ([
    "name"      : "少林寺拜山",
    "npc" : "/kungfu/class/shaolin/xu-tong",
    "place" : "/d/shaolin/ting",
    "type"   : TYP_TALK,
    "reward"   : ([
      "dhd"   : 5,
    ]),
    "word"     : "人在江湖飘哪能不挨刀，这些伤药就送给$R了。\n",
    "event"  : "do_greet",
    "next" : "dls"
  ]),
  "dls" : ([
    "name"  : "大轮寺拜山",
    "npc"   : "/d/xueshan/npc/heilinbofu",
    "place" : "/d/xueshan/guangchang",
    "type"  : TYP_TALK,
    "reward": ([
      "dexp" : 50000,
    ]),
    "word"  : "防守是为了更好的进攻，守御之道$R不可轻慢。\n",
    "event" : "do_greet",
    "next"  : "tz"
  ]),
  "tz" : ([
    "name"      : "铁掌帮拜山",
    "npc" : "/d/tiezhang/npc/mayi4",
    "place" : "/d/tiezhang/guangchang",
    "type"   : TYP_TALK,
    "reward"   : ([
      "tb"   : 100,
    ]),
    "word"     : "通宝是这个世界最不可思议的东西，可以在扬州当铺换到很多奇怪道具。\n",
    "event"  : "do_greet",
    "next" : "gb"
  ]),
  "gb" : ([
    "name"      : "丐帮拜山",
    "npc" : "/kungfu/class/gaibang/lu",
    "place" : "/d/gb/pomiao",
    "type"   : TYP_TALK,
    "reward"   : ([
      "silk"   : 600,
    ]),
    "word"     : "这些原丝就送给$R了，希望有日能用得上。\n",
    "event"  : "do_greet",
    "next" : "ts"
  ]),
  "ts" : ([
    "name"      : "泰山派拜山",
    "npc" : "/d/taishan/npc/tiansong",
    "place"  : "/d/taishan/ertian",
    "type"   : TYP_TALK,
    "reward" : ([
      "item" : "/clone/gift/phoenix",
    ]),
    "word"   : "这羽毛是老道当年机缘所得，据说是来自异界，有逆天改命的威能，希望$R能善用。\n",
    "event"  : "do_greet",
  ]),
]);


void create() {
  seteuid(geteuid(this_player()));
  set_events(events);
  set_task(task);
  setup();
}

varargs int step1(mixed arg,object ob,object me){
	
	if(userp(arg)) {
	  string msg=HIG+ob->name()+"说道：不知道这位"+RANK_D->query_respect(arg)+"是否有空和我聊聊天？["HIY"Y|n"HIG"]："NOR;
	  event_choice(msg,ob,arg);
	  return STAT_CNT;
	}
	else if(arg[0]=='y' || arg[0]=='Y') {
    string msg = "你说道：好啊，我们就聊个五毛钱的天吧。\n";
    msg += ob->name()+"说道：哈哈哈，今天天气真的很好啊。\n";
    msg += "你说道，是啊，真的很好啊。\n";
    msg += ob->name()+"说道：...\n";
    msg += "你说道：...\n";
    msg += ob->name()+"说道：？？？\n";
    msg += "你说道：？？？\n";    
    tell_object(me,CYN+msg+NOR);
    ob->force_me("joke");
    msg = ob->name()+"说道：闯荡江湖，混个脸熟很重要，你可去先去各大门派拜拜山。\n";
    msg += ob->name()+"就先从华山派开始吧，这些黄金就给你当盘缠了。\n";  	
    pre_set_hint(me,plist[0],"到"+HIY+events[plist[0]]["place"]->query("short")+NOR"找"HIG+events[plist[0]]["npc"]->query("name")+NOR"拜山。");
    tell_object(me,CYN+msg+NOR);
  	return STAT_PROG+STAT_END;
	}
	else {
	  tell_object(me,CYN+ob->name()+"说道：那真是太遗憾，希望"+RANK_D->query_respect(me)+"有空时，能陪"+RANK_D->query_self(ob)+"聊聊。\n");
	  return STAT_ACT;
	}
}



int do_greet(object me,object ob){
  int pno;
  object env = environment(me);
  string msg;

  pno = check_flag(me,"party");

  me->force_me("bow "+ob->query("id"));
  me->force_me("visit");

  message_vision("$N大声说道："+me->query("title")+NOR+me->query("name")+NOR+"特来宝地拜山。\n",me);
  msg = CYN+ob->name()+"说道：这位" +RANK_D->query_respect(me)+"真是太客气了。\n";
  msg += CYN+ob->name()+"说道：哈哈，都是武林同道，自当相互帮衬。\n";
  msg += CYN+ob->name()+"又说道：" +replace_string(events[plist[pno]]["word"],"$R",RANK_D->query_respect(me));

  pno++;
  if(pno==sizeof(plist)){
    tell_object(me,msg);
    tell_object(me,HIR"你已访遍各大门派，可以继续安心修习武学了。\n"NOR);
    return STAT_END+STAT_FIN;
  }  
  else {
    msg += CYN+ob->name()+"想了一想又说：" +RANK_D->query_respect(me)+"若是有空，可继续到"+events[plist[pno]]["name"]+"。\n";
    pre_set_hint(me,plist[pno],"到"+HIY+events[plist[pno]]["place"]->query("short")+NOR"找"HIG+events[plist[pno]]["npc"]->query("name")+NOR"拜山。");
    set_flag(me,"party",pno);
    tell_object(me,msg);
    if(wizardp(me)) me->move(events[plist[pno]]["place"]);
    return STAT_PROG+STAT_END;
  }
}