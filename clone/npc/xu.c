// /u/snowman/npc/xu.c
// by snowman 12.feb.98
#include <ansi.h>
inherit NPC;

string jiuyinup();
string jiuyin();
string hama();
void create()
{
       set_name("徐霞客", ({ "xu xiake", "xu", "xiake" }) );       
       set("gender", "男性" );
       set("shen_type", 1);
       set("age", 35);
       set("str", 15);
       set("con", 15);
       set("int", 27);
       set("dex", 20);
       set("env/wimpy", 110);
       set("long", "这位就是游遍千山万水，踏尽大江南北的奇士徐霞客。\n" );
       set("combat_exp", 2500);
       set("attitude", "friendly");
       set("max_qi", 200);
       set("max_jing", 200);
       set_skill("unarmed", 10);
       set_skill("dodge", 10);
       set_skill("parry", 10);
       set("inquiry", ([
                "徐霞客游记" : "这是我走遍千山万水而写成的游记。\n",
                "旅游" : "你也想出去看看？好吧，给我一两黄金我就带你走走。\n",
                "黑风双煞" : (: jiuyin() :),
                "蛤蟆功" : (: hama() :),
                "周伯通" : (: jiuyinup() :),
       ]));
       set("chat_chance_combat", 99);
       set("chat_msg_combat", ({
                (: random_move :),
	}));
       set("chat_chance", 30);
       set("chat_msg", ({
 (: random_move :),
 "徐霞客摇头晃脑地讲道：你看这大江南北好地方可真不少！\n",
 "徐霞客道：想当年我走遍嵩山、西域、终南山、昆仑山和武当山。。。\n",
 (: random_move :),
 "徐霞客又说道：那南疆大理城和西疆伊犁城虽小，却和我们江南苏州、
              扬州一样繁华。\n",
 (: random_move :),
 "徐霞客露出恐惧的表情：去星宿海、光明顶和铁掌峰都会遇到凶徒阻拦。\n",
 "徐霞客拍了拍脑袋：上次去到东海，听人说海中有个桃花岛仙境，不知是真是假。\n",
 (: random_move :),
       }) );       
       setup();
       carry_object("d/city/obj/cloth")->wear();
        carry_object("/clone/book/xu-book");
}

void init()
{
       object me, ob;
       me = this_object();
       ob = this_player();
       ::init();
       if(ob->query_temp("money_paid")) 
               ob->delete_temp("money_paid");
        add_action("do_go","去");       
}

int do_go(string target)
{
       object ob1,me;       
        ob1=this_object();
        me=this_player(); 
        if (me->is_busy())
                return notify_fail("你正忙着呢。\n");

        message_vision(CYN"$N对徐霞客说道：请先生带晚辈去" + target +"。\n"NOR,me);
        if( !this_player()->query_temp("money_paid") ) 
               return notify_fail("徐霞客摇了摇头：我虽不是生意人，可没钱怎么走啊？\n");
        if( !target || target==" ") 
               return notify_fail("徐霞客皱了皱眉：那地方我可没去过。\n"); 

        if (target=="回疆") me->set_temp("go_hj", 1); 
        if (target=="星宿" || target=="星宿海") me->set_temp("go_xx", 1); 
        if (target=="伊犁" || target=="伊犁城") me->set_temp("go_yili", 1);
        if (target=="少林" || target=="少林寺" || 
            target=="嵩山少林寺" || target == "嵩山") me->set_temp("go_sl", 1); 
        if (target=="武当" || target=="武当山") me->set_temp("go_wd", 1); 
        if (target=="扬州" || target=="扬州城") me->set_temp("go_yz", 1);   
/*      
        if (target=="大理" || target=="大理城") me->set_temp("go_dali", 1);   
        if (target=="昆仑" || target=="昆仑山") me->set_temp("go_kl", 1);
        if (target=="铁掌帮" || target=="铁掌峰" ||
            target=="猴爪山") me->set_temp("go_tzb", 1);   
        if (target=="桃花岛" || target=="东海渔港") me->set_temp("go_thd", 1);
*/
        if (target=="明教" || target=="光明顶") me->set_temp("go_mj", 1);   
        if (target=="苏州" || target=="苏州城") me->set_temp("go_sz", 1);   
        if (target=="终南山" || target=="全真教") me->set_temp("go_zns", 1);   
        if (target=="杭州" || target=="杭州城") me->set_temp("go_hz", 1);        
        else return notify_fail("徐霞客皱了皱眉：那地方我可没去过。\n");        
        call_out("do_goto", 0, me);
        return 1;
}

void do_goto()
{
      object me, ob;
      me = this_player(); 
      ob = this_object(); 
      command("nod");
      command("say 那你就跟着我走吧。");
      message_vision("\n$n跟着$N开始了漫长的旅程。\n" NOR, ob, me);
      me->move("/d/wizard/room3"); 
      me->delete_temp("money_paid");   
      destruct(this_object());          
}

int accept_object(object who, object ob)
{       
       if (ob->query("money_id") && ob->value() >= 2000) {              
               message_vision("徐霞客对$N一笑：你要跟我去哪里呀？说吧。\n", who);
               tell_object(who, "请键入：去 地方中文名字\n");
               who->set_temp("money_paid",1);
               return 1;
               }
       else if (ob->query("money_id") && ob->value() < 2000) {
               tell_object(who, "徐霞客对着你摇了摇头。\n");
               return 1;
               }
       return 0;
}

string jiuyin() 
{
        object me = this_player();

        if(me->query("quest/chance")<1 
        || me->query("quest/jiuyin2/start") 
        || me->query("quest/jiuyin2/pass") ) 
        	return "黑风双煞？没听说过。";        
        else {
 	   	    me->add("quest/chance",-1);
    		  me->set("quest/jiuyin2/start",1);
   		    log_file("quest/jiuyin",sprintf("%8s%-18s开始解九阴真经下卷花费解迷机会一次。\n",
			          me->name(1), "("+capitalize(me->query("id"))+")"), me);
   		    return "好象他们正躲在青城沙漠。";
   		  }
 }
 
string hama() 
{
        object me = this_player();

        if(me->query("quest/chance")<1 
        || me->query("quest/hama/start") 
        || me->query("quest/hama/pass")
        || me->query("gender")!="男性" 
        || (me->query("age")-me->query("age_reyoung")) >= 18 )
        	return "蛤蟆功？没听说过。";        
        else {
 	   	    me->add("quest/chance",-1);
    		  me->set("quest/hama/start",1);
   		    
   		    log_file("quest/hamagong",sprintf("%8s%-18s开始解蛤蟆功花费解迷机会一次。\n",
			          me->name(1), "("+capitalize(me->query("id"))+")"), me);
   		    force_me("say 蛤蟆功，是什么东西？");
   		    return "那年我游行经过嘉兴城边看到一个怪人在树林里像只蛤蟆一样跳来跳去，还浑身冒白气。";
   		  }
 }
 
string jiuyinup() 
{
        object me = this_player();

        if(me->query("quest/chance")<1 
        || me->query("quest/jiuyin1/start") 
        || me->query("quest/jiuyin1/pass") )
        	return "周伯通是王重阳的师弟。";        
        else {
 	   	    me->add("quest/chance",-1);
    		  me->set("quest/jiuyin1/start",1);
   		    
   		    log_file("quest/jiuyin",sprintf("%8s%-18s开始解九阴真经上卷花费解迷机会一次。\n",
			          me->name(1), "("+capitalize(me->query("id"))+")"), me);
   		    return "老顽童好象让黄药师困在桃花岛上了。";
   		  }
 }