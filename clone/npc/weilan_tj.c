/* tiejiang.c 
  Imbue npc 
	Created by snowman@SJ 21/04/2001

   Copyright 1997-2004 ShuJian MUD Wizard Group.
   All rights reserved.
*/

#include <ansi.h>
inherit NPC;
string ask_money();
string ask_imbue();
#define USER_WEAPOND     "/adm/daemons/user_weapond"


void back_player(object obj)
{
	object me;
	if(!objectp(me = query_temp("dazao_target")))
	{
		destruct(obj);
		command("say 这倒奇了，叫我打造，人却又不见了。");
		command("consider");
		delete_temp("dazao_target");
		call_out("ddd",1);
		return;
	}
	if(!present(me,environment()))
	{
		obj->move(me);
		message_vision("一个兵器铺的伙计冲到$N面前，给了$N一"+obj->query("unit")+obj->name()+"，随即说道：这是老板交代给你的。\n",me);
		//tell_object(me,
		//"一个兵器铺的伙计冲到你面前，给了你一"+obj->query("unit")+obj->name()+"，随即说道：这是老板交代给你的。\n"
		//);
		delete_temp("dazao_target");
		call_out("ddd",1);
		return;
	}
	obj->move(me);
	message_vision("韦兰给了$N一"+obj->query("unit")+obj->name()+"。\n",me);
	command("say 等得辛苦了吧，呵呵。");
	
	call_out("ddd",1);
}

void ddd()
{
	object npc;
	command("say 韦爵爷迟会还要我打造兵器。");
	command("wave");
	delete_temp("dazao_target");
	
	npc=present("zhujian shi",environment(this_object()));
	if(npc) npc->delete_temp("dazao_target");
	destruct(this_object());
}


void create()
{
        set_name("韦兰铁匠", ({ "weilan tiejiang", "weilan", "tiejiang" }));
        set("nickname", HIY"巧夺天工"NOR);
        set("gender", "男性");
        set("long", "
这名铁匠便是为查理大帝打造那把传奇式的剑“乔依乌斯”的半人半神！
你可以向他打听他的事迹(name)。\n\n");
        set("age", 41);
        set("attitude", "heroism");
        set("unique", 1);
        set("combat_exp", 100000);
        set("no_quest", 1);
        set("shen", 3000);
        set("str", 1000);
        set("int", 1000);
        set("con", 1000);
        set("dex", 1000);
        set("jiali", 200);
        set("max_qi", 900000);
        set("max_jing", 900000);
        set("eff_jingli", 900000);
        set("max_neili", 900000);
        /*
        set("inquiry", ([
                "dazao": (: ask_imbue() :),
                "money": (: ask_money :),
                "name": "我现在负责帮助玩家打造(dazao)武器，能打出最好的来！机率吗.....嘿嘿。

要求： 一把"HIC"随意"CYN"的武器(令和暗器不行)。
理论上最高威力可以达到倚天剑屠龙刀的两倍，还有很多附加属性。

限制：  每人经验500K时可以打造，以后每一百万经验增加一次打造机会，好好把握哦！

\n",
        ]) );;
        */
        set_skill("unarmed", 60);
        set_skill("parry", 60);
        set_skill("dodge", 60);
        
                
        set_temp("apply/dodge", 700);
        set_temp("apply/parry", 700);
        set_temp("apply/attack", 700);
        
        setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object("/d/zhiye/obj/tiechui")->wield();
}

void init()
{
	::init();
	if(query("come")) return;
	
	set_skill("duanzao", 260+random(80) );
		
	set("come",1);
	call_out("iamhere",0);
	//add_action("do_imbue","dazao");
}

void iamhere()
{
	command("wink zhujian shi");
}

int go_dazao(object lu,object me,string cmds)
{
	set_temp("dazao_target",me);
	return lu->duanzao_it(this_object(),cmds);
}

void die()
{
	message_vision("$N深深吸了几口气，脸色看起来好多了。\n",this_object());
	return;
}


string ask_money()
{
	return "我现在已经赚了" + MONEY_D->money_str(this_object()->query("balance")) + "！";
}

string ask_imbue()
{
	object wp, me = this_player();
	mapping im;
	int i = me->query("get_imbue_weapon");
	
	if( i && i > time() )
		return "还没打造好呢，你再等等吧！\n";
	if( i && i <= time() ){
		me->delete("get_imbue_weapon");
		if( !mapp(im = me->query("imbue_weapon")) )
			return USER_WEAPOND->ask_imbue();		
		wp = USER_WEAPOND->make_weapon(me, im["type"], im["i"] , im["mar"]);
		me->delete("imbue_weapon");
		if( wp )
        		wp->move(me);
        	else return "武器生成出错，请通知snowman! \n";
        	
        	message_vision("$N将打造好的"+wp->name()+"交给$n。\n\n", this_object(), me);
        	write(HIW"详细使用请看 uweapon 命令。\n"NOR);
        	return "别弄丢了！\n";
        }
        
	return USER_WEAPOND->ask_imbue();
}

int do_imbue(string arg)
{
        object wp, me;
        string type, mar;
        int i;
        
        if( !living(this_object()) ) return 0;
        
        if( !arg )
                return notify_fail("格式： dazao <你身上要打造的武器>\n");
        
        me = this_player();
        
        if( (int)me->query("get_imbue_weapon") )
        	return notify_fail("你先把打造的武器取了再说吧(ask weilan about dazao)。\n");        
        if( wizardp(me) || SECURITY_D->get_status(me)!="(player)" )
                return notify_fail("巫师没有打造的权利！\n");

        if( !objectp(wp = present(arg, me)) )
                return notify_fail("你身上没有这样东西。\n");
        
        if( !wp->query("weapon_prop") )
                return notify_fail("这里目前只能打造武器。\n");
        
        if( me->is_busy() || me->is_fighting() )
                return notify_fail("你先忙完再说吧。\n");
                
        if( wp->query("equipped") )
                return notify_fail("要打造，你先放下武器吧。\n");
        if( wp->query("imbued") || strsrch(file_name(wp),USER_WEAPOND->query_uwname())==0  )
                return notify_fail("你这武器已经打造过了。\n");
                
        if( !stringp(type = wp->query("skill_type")) )
                return notify_fail("这里只能打造各式武器。\n");
               
        if( this_object()->is_busy() )
                return notify_fail("我正忙着呢，等等。\n");
                
	if( wp->is_corpse() ){
		me->start_busy(5);
		message_vision("$N被吓得晕了过去！\n", this_object());
		this_object()->unconcious();
		return notify_fail("糟糕！\n");
	}

        switch(type){
                case "sword": 
                case "axe":
                case "blade":
                case "brush":
                case "club": 
                case "dagger":
                case "fork": 
                case "hammer": 
                case "hook":
                case "spear": 
                case "staff": 
                case "stick":
                case "whip": break;
                default:
                        return notify_fail("这里不能打造这种武器。\n");
        }
        
        if( wp->id("xiao") ) type = "xiao";
        
        message_vision("$N拿出"+wp->name()+"想要$n帮忙打造。\n\n",me,  this_object());
	this_object()->start_busy(2);
	
	if( !me->query("imbue_reward")){
		if( me->query("combat_exp") < 500000 )
                	return notify_fail("依您目前的水平，我看是用不了什么好武器的，你以后再来吧。\n");
        
        	if( me->query("combat_exp")/1000000 < (int)me->query("user_weapon_imbued") ){
                	return notify_fail("依您目前的经验来说，这好武器还是以后再来打吧。\n");        	
        	}
        	me->add("user_weapon_imbued", 1);
        }
        else me->add("imbue_reward",-1);
        
        if(!me->query("imbue_reward")) me->delete("imbue_reward");
	
	me->start_busy(2);
        mar = wp->query("material");
        mar = stringp(mar)?mar:"steel";
        
        destruct(wp);
        
        if( me->query("combat_exp") > 4000000 && !random(me->query("kar"))
        && !random(me->query("per")) && !random(me->query("pur")) )
        	i = 4;        	
        else if( me->query("combat_exp") > 3000000 
        && random(me->query("kar")) > 20 && random(20) > 15 )
        	i = 3;
        else if(  me->query("combat_exp") > 1000000
        && random(me->query("kar")) > 10 && random(15) > 11 )
        	i = 2;
        else if(  random(me->query("kar")) > 5 )
        	i = 1;
        else i = 0;
        
        me->set("imbue_weapon/type", type);
        me->set("imbue_weapon/mar", mar);
        me->set("imbue_weapon/i", i);
                 	
 	if( !wizardp(me) ){
 		me->set("get_imbue_weapon", time() + 18000);
        	message_vision("$N点点头道：你两个半时辰后来取吧。\n\n", this_object());
	}
	else {
		message_vision("$N点点头道：打造好了。\n\n", this_object());
		me->set("get_imbue_weapon", 10000);
	}
        return 1;
}
