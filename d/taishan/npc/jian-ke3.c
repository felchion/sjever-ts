// jian-ke.c

#include <ansi.h>

inherit NPC;

void create()
{
	set_name("剑客", ({ "jian ke","ke" }) );
	set("gender", "男性");
	set("age", 45);
	set("long",
		"这是位中年武人，肩背长剑，长长的剑穗随风飘扬，看来似乎身怀绝艺。\n");
        set("combat_exp", 25000);
        set("shen_type", -1);
	set("attitude", "heroism");

	set("apply/attack",  30);
	set("apply/defense", 30);

        set_skill("sword", 70);
        set_skill("unarmed", 60);
        set_skill("parry", 60);
        set_skill("dodge", 60);

	setup();

        carry_object("/clone/weapon/changjian")->wield();
        carry_object("/clone/misc/cloth")->wear();
	add_money("silver", 20);
}

void init()
{
	object room, ob;
	string mengzhu;
	
	if(!( room = find_object("/d/taishan/fengchan")) )
	room = load_object("/d/taishan/fengchan");

	if( objectp(ob = present("mengzhu", room)) ) 
	mengzhu = ob->query("winner");

	if( this_player()->query("id") == mengzhu ) {
		message_vision(HIY "剑客向$N躬身为礼，说道：盟主您老人家好！\n" NOR, 
		this_player());
	}	
}
