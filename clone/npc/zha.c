// jinyong.c Yu Jue 98/7/28

#include <ansi.h>
#include <job_mul.h>
#include <get_place.h>

inherit NPC;

int move_other();
int check(object ob);

string *no_kill_list = ({
	 "mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", 
	 "huo du","he shiwo"
});

string ask_age()
{
	object me = this_player();
    	object *ob = filter_array(objects(), (: userp($1) && !wiz_level($1) :));
	int no;

	ob = filter_array(ob,
		(: $2->query("age")*12+$2->query("month")
			< $1->query("age")*12+$1->query("month") :), me);
	no = sizeof(ob) + 1;
	return me->name()+RANK_D->query_respect(me)+"的年龄在目前江湖人士中排名第"
		+chinese_number(no)+"。";
}

string ask_beauty()
{
	object me = this_player();
    	object *ob = filter_array(objects(), (: userp($1) && !wiz_level($1) :));
	int no;

	ob = filter_array(ob,
		(: $2->query("per")	< $1->query("per") :), me);
	no = sizeof(ob) + 1;
	return me->name()+RANK_D->query_respect(me)+"的容貌在目前江湖人士中排名第"
		+chinese_number(no)+"。";
}

int ask_exp()
{
	object me = this_player();
    	object *ob = filter_array(objects(), (: userp($1) && !wiz_level($1) :));
	int no;

	ob = filter_array(ob,
		(: $2->query("combat_exp")	< $1->query("combat_exp") :), me);
	no = sizeof(ob) + 1;

	command("say "+me->name()+RANK_D->query_respect(me)+"的经验在目前江湖人士中排名第"
		+chinese_number(no)+"。");
	message_vision(CYN "$N指着$n赞叹道：“"+me->name()+"是武林第"
		+chinese_number(no)+"高手！”\n" NOR, this_object(), me);
	return 1;
}

void create()
{
        set_name("查老学士", ({ "zha xueshi", "zha","xueshi" }));
        set("title", WHT"飞雪连天射白鹿 笑书神侠倚碧鸳"NOR);
	set("gender","男性");	
	set("age",60);
    set("per", 25);
    set("dex", 30);
	set("long", @LONG
一个胖乎乎的老学究，一双深邃的眼睛笑咪咪的，却似乎蕴藏了无穷的智慧。
LONG);
	set("combat_exp", 10000000);
	set("attitude", "friendly");
	set("chat_chance", 200);
	set("chat_msg", ({      
        CYN"查老先生喃喃自语：我的手稿哪里去了呢？\n"NOR,
	(: random_move :),
        CYN"查老先生喃喃自语：谁能把我的手稿找到，我一定不会亏待他。\n"NOR,
        (: random_move :),
        (: random_move :),
        (: random_move :),
        (: random_move :),
        }) );
	set_skill("dodge",250);
	set_skill("parry",250);
	set_skill("unarmed",250);
	setup();
	
	carry_object(ARMOR_D("ruanxue2"))->wear();
	carry_object(ARMOR_D("changshan"))->wear();
	
	set("no_quest",1);//added by Ciwei@SJ
	
	if(clonep())
	{
		remove_call_out("move_other");
		call_out("move_other", 100);
	}
}

int accept_object(object me, object ob)
{
        int pot, exp;
        
	if( !me || environment(me) != environment() ) {return 0;}
	if ( !objectp(ob) ) {return 0; }
	if ( !present(ob, me) ) return notify_fail("你没有这件东西。");

	if ((string)base_name(ob) == "/clone/gift/zhuanji" && ob->query("gift_target") == getuid(me)){
		command("ah");
		message_vision("$N又惊又喜：我只当我的" + ob->query("name") + "被贼人盗走再难复得，谁想" + RANK_D->query_respect(me) + "竟帮我夺了回来。\n",this_object());
		command("look " + me->query("id"));
		message_vision("$N赞叹道：" + RANK_D->query_respect(me) + "如此英雄，日后定也能成为武林传奇人物。\n",this_object());
		remove_call_out("destroying");
		call_out("destroying", 1, this_object(), ob);
		exp = 400 + random(50) ;
		exp = me->add_exp_combat(exp,this_object(),"传记",1);
		pot = exp / 4;
		me->add("potential",pot);
                //if(me->query("potential") > me->query("max_pot"))
                	//me->set("potential", me->query("max_pot"));
		log_file("job/zha",sprintf("%8s%-10s 得exp:%d,pot:%d",me->name(),"("+me->query("id")+")",exp,pot));
		tell_object(me,HIW"你被奖励了："+chinese_number(exp)+"点经验和"+chinese_number(pot)+"点潜能。\n"NOR);		
		return 1;
	}
	else if ((string)base_name(ob) == "/clone/gift/zhuanji" ){
		command("ah");
		message_vision("$N又惊又喜：我只当我的" + ob->query("name") + "被贼人盗走再难复得，谁想" + RANK_D->query_respect(me) + "竟帮我找了回来。\n",this_object());
		command("look " + me->query("id"));
		message_vision("$N赞叹道：" + RANK_D->query_respect(me) + "如此英雄，日后定也能成为武林传奇人物。\n",this_object());
		remove_call_out("destroying");
		call_out("destroying", 1, this_object(), ob);
		exp = 300 + random(50) ;
		exp = me->add_exp_combat(exp,this_object(),"传记",1);
		pot = exp / 4;
		me->add("potential",pot);
                if(me->query("potential") > me->query("max_pot"))
                	me->set("potential", me->query("max_pot"));
		log_file("job/zha",sprintf("%8s%-10s 找exp:%d,pot:%d",me->name(),"("+me->query("id")+")",exp,pot));
		tell_object(me,HIW"你被奖励了："+chinese_number(exp)+"点经验和"+chinese_number(pot)+"点潜能。\n"NOR);
		
		return 1;
	}
	else {
		command("say 这个就想来拿奖励？");
		command("knock "+ me->query("id"));
		return 0;
	}
}

void destroying(object me, object obj)
{
	if(!obj) return;
	destruct(obj);
}

int move_other()
{
        object target, objwhere, *living;
        string str;
        int i,j;
        object ob = this_object(); 
              
        living = livings();
	for (i=0;i < sizeof(living);i++){
		j = random(sizeof(living));
		if (!j || j == sizeof(living) -1) continue;
		if(check(living[j]) ){
			target = living[j];
			objwhere = environment(target);
			str = objwhere->query("short");
			if (userp(living[j])) continue; // add by LinuX
			if (!sizeof(objwhere->query("exits"))
			|| str == "武僧堂"
			|| str == "僧舍"
			|| str == "休息室"
			|| str == "女休息室"
			|| str == "男休息室"
			|| str == "龙池"
			|| str == "凤池"
			|| str == HIR"心禅堂"NOR
			|| str == "金刚伏魔圈" ) continue;
			if ( !random(30) && get_place(base_name(environment(target))) != "神龙岛" ) continue;
			if( get_place(base_name(environment(target))) != "" ) break;
		}
	}

	if( !target )
		return 0;

   	tell_room(environment(ob),"一晃眼,"+ob->name()+"不知道去哪里了。\n");
	ob->move(objwhere);
        tell_room(environment(ob),ob->name()+"突然不知道从哪里钻出来，出现在你面前。\n");
        
        remove_call_out("move_other");
	call_out("move_other", 100);
	
        return 1;
}

int check(object ob)
{
	object room;
	string room_name;

	if(!objectp(ob)
	 || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	 || ob->query("env/invisibility")
	 || ob->query("no_quest")
	 || member_array(ob->query("id"), no_kill_list) != -1
	 || userp(ob)
	 || ob->query("attitude")=="aggressive"
	 || ob->query("have_master")
	 || !objectp(room=environment(ob))
	 || room->query("no_fight")
	 || room->query("outdoors") == "昆仑翠谷"
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/group/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/wudang/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0)
	 return 0;
	return 1;
}
