// /u/snowman/npc/oyf1.c 欧阳锋
// by snowman 12.feb.98

#include <ansi.h>
inherit NPC;
void create()
{
       set_name("欧阳锋", ({ "ouyang feng", "ouyang", "feng" }) );
       set("gender", "男性" );
       set("title", BLU "老毒物" NOR);
       create_family("白陀山庄", 1, "庄主");
       set("shen_type", -1);
       set("age", 70);
       set("per", 20);
       set("long","你走过来一看，他满腮须髯，根根如戟，一张脸犹如刺□相似，\n"+
		"正是你分别已久的义父欧阳锋。\n" );
       set_temp("hide_level", 45+random(6));
       set("inquiry", ([
                "欧阳锋" : "欧阳锋、欧阳锋，听起来这名字挺熟的。。。\n",
                "洪七公" : "洪七公，这名字也熟。\n",
                "蛤蟆功" : "蛤蟆功和九阴真经乃是当世两大奇功，余下的都不足为虑。\n",
       	]));
       	setup();       
       	carry_object(ARMOR_D("cloth"))->wear();
       	add_money("silver", 10);
}


void kill_ob(object me)
{
	command("pat " + me->query("id"));
        command("say 儿啊，怎么找为父来给你喂招了？");
        me->remove_all_enemy();
        this_object()->remove_all_enemy();
}

void init()
{
        object ob = this_player();   
        if (!stringp(query("startroom")) ) return;
             
        add_action("do_lian","yanlian");
        if(ob->query("oyf_son") 
        && ob->query("quest/hama/pass") 
        && ob->query("oyf/hamagong") < 2) 
                call_out("do_angry", 5, ob);          
        else{
             	command("look " + ob->query("id"));
             	command("say 喂，你是哪儿来的" + RANK_D->query_rude(ob) + "？走开，走开。");
             	command("kick " + ob->query("id"));
             	message_vision("$N唉呀一声，已经被欧阳锋飞脚踢了出去。\n",ob);
             	ob->move("/d/gumu/shulin2");
             	message_vision(ob->name() + "被人一脚从树林里给踢了出来。\n",ob);
             	ob->receive_wound("qi", 50, "被欧阳锋一脚踢");
             	return;
        }
}

void do_angry(object ob)
{
        if( !ob ) return;
        if( environment(ob) != environment() ) return;
        command("angry " + ob->query("id"));
        command("say 儿啊，你跑哪儿去了？为父我找得你好苦！");  
        call_out("do_back", 180);     
        call_out("ask_test", 10, ob);               
}

void ask_test(object me)
{
	if( !me ) return;
        if( environment(me) != environment() ) return;
        command("hug " + me->query("id"));
        command("say 看起来你的武功倒练得不错，再好也没有了。你演练(yanlian)蛤蟆功给我瞧瞧。");       
        me->set_temp("oyf_test", 1);         
}

int do_lian()
{
      	object me = this_player();             
      	if( !me->query_temp("oyf_test") ) return 0;
      
      	tell_object(me, HIC "你此时听他一说，欣然照办。\n" NOR );      
      	message_vision("$N当下蹲下身子，练起蛤蟆功来，只见$N使得花团锦簇，甚是好看。\n",me);
      	me->delete_temp("oyf_test");  
      	call_out("oyf_laugh", 6, me);                 
      	return 1; 
}

void oyf_laugh(object me)
{
	if( !me ) return;
        if( environment(me) != environment() ) return;
        
        command("haha " + me->query("id"));
        command("say 好看！好看！就是不对劲，中看不中用。");
        command("say 我把其中诀窍尽数传了你罢！");
        tell_object(me, "\n欧阳锋当下指手划脚、滔滔不绝的说了起来。。。\n");      
        call_out("oyf_teach", 10, me);       
}

void oyf_teach(object me)
{
      	int i,lvl=1;
	mapping skills;
	string *sk;
	
	if( !me ) return;
        if( environment(me) != environment() ) return;
      	command("say 我用九阴真经将这蛤蟆功逆转运行后发现威力更大，");
      	command("say 你以后用我传你这法子运功(yun nizhuan)，就算义父我也不一定能打得过你。");
      	tell_object(me, HIC "\n欧阳锋给你讲解了逆运蛤蟆功的要诀。。。\n" NOR );  
      	me->add("max_neili", 100);  
      	tell_object(me, "你的内功修为提高了！\n");                
      	me->set("oyf/hamagong", 2);   
        skills = me->query_skills();
	if (mapp(skills)) {
		sk = keys(skills);
		for (i=0;i<sizeof(sk);i++)
			skills[sk[i]]+=lvl;
        }
	me->add("combat_exp",me->query("combat_exp")/(50-lvl)+lvl);
	me->add("shen", me->query("shen")/(10-lvl));	
      	tell_object(me, "你的功夫提高了！\n");
	log_file("quest/hamagong", sprintf("%8s%-10s学会nizhuan，并得到奖励。\n",
		me->name(1), "("+me->query("id")+")" ), me);
	call_out("do_back", 15);    
}

void do_back(object me)
{                 
      	message_vision(HIY"\n只见$N突然双目呆瞪，一动也不动，嘴里不住念道：「九阴真经，九阴真经。」\n\n"NOR, this_object());
      	call_out("do_leave", 4);             
}

void do_leave()
{
      	message_vision("$N突然一个□斗，倒转了身子，以头撑地，大叫：「我是谁？我是谁？欧阳锋到那□去了？」\n", this_object());
      	message_vision("接着$N双掌乱舞，身子急转，以手行路，其快如风的冲出树林消失了。\n", this_object());            
      	destruct(this_object());      
}
