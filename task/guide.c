//测试
inherit TASK;
#include <task.h>
#include <mudlib.h>

int task_on() {return 1;}

mapping task = ([
    "name"      : "玩家指引之一",
    "on"        : 1,
    "id"        : __FILE__[6..<3],
    "start"     : "start"
]);

mapping events = ([
  "start" : ([
    "name"   : "进入游戏",
    "type"   : TYP_GO,
    "event"  : "start",
    "place"  : "/d/wuguan/dayuan", 
    "next"   : "go"
  ]),   
  "go" : ([
    "name"   : "学习移动",
    "place"  : "/d/wuguan/menlang",
    "type"   : TYP_GO,
    "event"  : "do_go",
    "hint"   : "使用"HIY"map"NOR"命令查看地图，并用指令移动到"HIG"门廊"NOR"。",
    "reward" : (["gold":10]),
    "next"   : "talk"
  ]),
  "talk" : ([
    "name"   : "学习交谈",
    "type"   : TYP_TALK,
    "event"  : "do_talk",
    "npc"    : "/d/wuguan/npc/dizi4",
    "hint"   : "使用"HIY"talk sun"NOR"命令和孙均交谈。",
    "reward" : (["exp":1000]),
    "next"   : "out"
  ]),
  "out" : ([
    "rst"    : (["combat_exp<3000":"你的战斗经验还不够。"]), 
    "name"   : "离开武馆",
    "npc"    : "/d/wuguan/npc/dizi4",
    "event"  : "do_out",
    "type"   : TYP_TALK,
    "hint"   : "通过帮大师兄工作累积三千战斗经验，然后和"HIG"孙均"NOR"交谈。",
    "reward" : (["pot":50000]),
    "next"   : "party"
  ]),
  "party" : ([
    "name"   : "拜师学艺",
    "npc"    : "/d/wuguan/npc/dizi1",
    "hint"   : "使用命令"HIY"help party"NOR"了解游戏里的各大门派,选择一个门派加入。",
    "type"   : TYP_HB,
    "reward"   : ([
      "nl"  : 500,
      "jl" : 500
    ]),
    "hb_type" : HB_PROC,
    "event"  : "hb_check_party",
  ])
]);

void create() {
  set_events(events);
  set_task(task);
  setup();
}

int start(object me,object ob){
  string msg = HIR"【系统提示】"HIY"您成功登录游戏，您的论坛用户组已更新。\n";


  if(!me->query("combat_exp")) {
    msg = HIR"【系统提示】"HIY"您成功登录游戏，您的论坛用户组已更新。\n";
    dbquery("UPDATE members set extcredits1 = 1 WHERE username='"
      + me->query("id")+"@"+INTERMUD_MUD_NAME+"'"
    );
  } else msg ="";
  msg +=HIR"【系统提示】"HIW"欢迎你来到书剑世界，请先来了解一下一些基本的指令：在书剑\n"+
  "里移动的命令是"HIY"e、w、s、n、u、d"HIW"，分别代表东西南北上下六个基本方向，以\n"+
  "及"HIY"ne、wd、su、enter、out"HIW"等组合和特殊命令，它们分别代表东北、西下、南\n"+
  "上等方向以及进入和外出等。好了，现在你可以去找孙均，他会告诉你更多的\n"+
  "常识，他现在应该在门廊一带，往南走两步就到了。\n"NOR+
  HIR"【系统提示】"HIW"使用命令"HIY"tlist"HIW"可以查看你现在进行的任务。\n";

  tell_object(me,msg);
  return STAT_PROG+STAT_END;
}

int do_go(object me,object ob){
  string msg =  HIR"【系统提示】"NOR"很好，你成功的到达指定地点，现在试一下和孙均交谈("HIY"talk sun"NOR")吧。\n"; 
  tell_object(me,msg);  
  return STAT_PROG+STAT_END;
}

int do_talk(object me,object ob){
  tell_object(me, CYN+ob->name()+"说道：「最近武馆很忙，我鲁坤师兄正缺人手，你可以到他那找点工作来做。」\n"NOR);
  tell_object(me, CYN+ob->name()+"说道：「工作可以获得战斗经验，有三千战斗经验就可以出去闯荡江湖了。」\n"NOR);
  tell_object(me, CYN+ob->name()+"说道：「要离馆的时候记得过来和我道个别啊。」\n"NOR);
  tell_object(me,HIW"孙均指点了你一些江湖经验。\n"NOR);
  return STAT_PROG+STAT_END;
}

int do_out(object me,object ob){
  if(me->query("enter_wuguan")){
    tell_object(me, CYN+ob->name()+"说道：「这么快就攒够经验了，不容易啊，现在可以去找"HIG"冬暖阁"CYN"向馆主申请离馆("HIY"ask wang about 离馆"CYN")了。」\n"NOR);
    tell_object(me, CYN+ob->name()+"说道：「馆主同意之后，你再来回来找我吧。」\n"NOR);    
    return STAT_CNT;
  }
  else {
    tell_object(me, CYN+ob->name()+"说道：「如果你已准备妥当，就再来和我说一下("HIY"ask sun about 离馆"CYN")吧。」\n"NOR);
    tell_object(me, CYN+ob->name()+"说道：「离馆之后，记得找一个名门大派拜师啊。」\n"NOR);    
    tell_object(me,HIW"孙均激发你的一些学习潜能。\n");
    return STAT_PROG+STAT_END;
  }
}

int hb_check_party(object me) {
  mapping fam;
  string msg;
  if(!mapp(fam=me->query("family")))
  return STAT_CNT;
  
  msg =  HIR"【系统提示】"NOR"恭喜你成为"+fam["family_name"]+"的一员，现在开始你可以在书剑世界里尽情享受江湖生活、快意恩仇了。\n";
  msg += HIR"【系统提示】"NOR"玩家指引之一结束了，祝你游戏愉快。\n";
  tell_object(me,msg);  
  return STAT_FIN+STAT_END;
}
  
