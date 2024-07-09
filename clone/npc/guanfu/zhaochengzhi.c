// /d/city/npc/cheng.c
// this is made by beyond
// update 1997.7.4
// updated by snowman@SJ 13/02/2000
// for change_name()
// change to named for banname list by Lklv 2001.12.6

inherit NPC;

#include <ansi.h>
#include <guanfu.h>
#include "change_name.h"
#include "gtitle.h"


void create()
{
	set_name("赵城之", ({ "zhao chengzhi", "zhao", "chengzhi" }));
	set("title", "从四品知府");
	set("gender", "男性");
	set("age", 43);
	set("str", 20);
	set("dex", 20);
	set("per", 20);
	set("long", "他就是赵城之，扬州现任知府。百姓如有困难可找他解决。(改名换姓....等)\n");
	set("combat_exp", 30000);

	set("no_get", 1);
	set("no_bark", 1);

	set("shen_type", 0);
	set("attitude", "heroism");

	set_skill("unarmed", 50);
	set_skill("force", 50);
	set_skill("sword", 50);
	set_skill("dodge", 50);
	set_skill("parry", 50);
	set_temp("apply/attack", 50);
	set_temp("apply/defense", 50);
	set_temp("apply/armor", 50);
	set_temp("apply/damage", 15);

	set("neili", 500);
	set("max_neili", 500);
	set("jiali", 10);

	set("inquiry", ([
		"change_name" : (: ask_me :),
		"改名换姓" : (: ask_me :),
		"改名" : (: ask_me :),
		"改名字" : (: ask_me :),
		"为官": (: get_title :),
		"报效朝廷": (: get_title :),
		"称号" : (: ask_me1 :),
		"title" : (: ask_me1 :),
	]));
	setup();
	carry_object(ARMOR_D("cloth"))->wear();
}

void kill_ob(object ob)
{
	command("say 杀人啦！救命啊！");
	ob->add_condition("killer", 15);
	::kill_ob(ob);
}

void arrest_him(object me)
{
	int i;
	object *inv = all_inventory(me);

	me->clear_condition();
	me->apply_condition("city_jail", 200+(me->query("PKS") * 4) );
	me->set("startroom", JAIL);
	for( i=0; i < sizeof(inv); i++ ) {
		if( inv[i]->query_autoload() ) continue;
		if( inv[i]->is_character() ){
			inv[i]->move(environment(me));
			continue;
		}
		if( inv[i]->query("equipped") )
			inv[i]->unequip();
		destruct(inv[i]);
	}
	me->move(JAIL);
	message("vision", "只听“乒”地一声，一个昏沉沉的家伙被扔了进来。\n", environment(me), me);
}

int accept_object(object me, object obj)
{
	string id, name;
	object wenshu;
	mapping target;

	if (is_busy()) {
		command("say 没看见本官正忙着吗？一边等着！");
		me->add_busy(1);
		return 0;
	}

	start_busy(2);

	if (member_array(me->query("id"), GF_WANTED->query_wanted_list() ) != -1) {
		command("say 好哇"+me->query("id")+"，本府正通缉你呢，竟敢来自投落网！");
		command("say 来人啊！给我把这"+RANK_D->query_rude(me)+"打进死牢！！");
		if (userp(me)) {
			tell_object(me, "你一惊，正要有所行动，只听一声：“在我张召重面前还敢耍滑头？倒！”你后脑一震，就晕了过去！\n");
			me->unconcious();
			arrest_him(me);
			log_file("job/gfbug", sprintf("%-18s 自投落网，Obj id：%s\n",
				me->name()+"("+me->query("id")+")", obj->query("id")), me);
		}
		message_vision("\n只听一阵风响，几名高大的捕头已经点倒了$N，拖入内去了。\n",me);
		if (!userp(me)) destruct(me);
		return 0;
	}

	if( !objectp(wenshu = present("tiebu wenshu", me)) ){
		command("say 咦？你平白无故来干什么？本府怎么知道你不是歹人一伙？");
		return 0;
	}

	if( !mapp(target = wenshu->query("target")) ){
		command("say 你拿个什么烂文书来滥竽充数？");
		return 0;
	}

	if (!stringp(id = obj->query("victim_id")))
		id = obj->query("id");

	if (!stringp(name = obj->query("victim_name")))
		name = obj->query("name");

	if (target["id"] != id || target["name"] != name) {
		command("say 要你去拿通缉的犯人，你带个"+obj->query("name")+"回来？好好看看铁捕文书！");
		return 0;
	}

	message_vision("$N给$n一"+obj->query("unit")+obj->name(1)+"。\n", me, this_object());

	if ( obj->is_character() && !obj->is_corpse() ){
		command("say 好！"+me->query("name")+"竟然把"+obj->query("name")+"给抓回来了，真是位大大的英雄！");
		command("say 来人啊！给我把这"+RANK_D->query_rude(obj)+"打进死牢！！");

		message_vision("\n只见几名衙役过来，拖起$N，入内去了。\n",obj);
		command("say 本府代表千万民众多谢这位"+RANK_D->query_respect(me)+"了");
		GF_WANTED->do_give_reward(id, target, me, 0);
		
		GIFT_D->check_count(me,this_object(),"官府");
		
		if( userp(obj) )
			arrest_him(obj);
		else 	destruct(obj);
		return notify_fail("");
	}

	if( obj->query("kill_by") != me ){
		command("say 哦，"+obj->name()+"！多谢这位"+RANK_D->query_respect(me)+"了，你下去吧。");
		command("say 下一个！");
		log_file("job/gfbug", sprintf("%-18s 通缉任务，Obj：%s，Killed by %s\n",
			me->name()+"("+me->query("id")+")", obj->query("name"),
			(stringp(obj->query("kill_by"))?obj->query("kill_by"):"Unknow") ), me);
	}
	else {
		command("thumb "+me->query("id"));
		command("say "+RANK_D->query_respect(me)+"铲除此人，真是大快人心啊！");
		command("say 本府代表千万民众多谢这位"+RANK_D->query_respect(me)+"了");
		GF_WANTED->do_give_reward(id, target, me, 1);
	}
	destruct(obj);
	return notify_fail("");
}
