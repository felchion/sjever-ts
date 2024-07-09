// suanming.c
#include <ansi.h>
inherit NPC;
string ask_where();

void create()
{
	set_name("李半仙", ({ "li banxian", "li", "banxian", "xiansheng" }) );
	set("nickname", "算命先生" );
	set("gender", "男性" );
        set("age", 92);
	set("long", "这位算命先生坐在那里，一副神秘莫测的样子。\n");
	set("combat_exp", 10000);
	set("attitude", "friendly");
        set("no_get", 1);
	set("inquiry", ([
		"name" : "在下姓李，因为算命算得准，所以大家都尊称我为李半仙。",
		"rumors" : "要想知道你今生的祸福荣辱，那就问我来算命吧。",
		"行踪": (: ask_where :),
		"where": (: ask_where :),
		"算命" : "我的价钱最公道，不多不少，只要五十两银子。"
	]) );
	set("chat_chance", 10);
	set("chat_msg", ({
		"李半仙吆喝道：铁嘴神算，价钱公道，相资五十两白银。\n",
		(: random_move :),
		"李半仙对你说道：客倌请留步，卜个吉凶如何？\n",
	}) );
	setup();
	carry_object("/clone/misc/cloth")->wear();
}

string ask_where()
{
    object me;
    string name;
    mapping quest;
    me=this_player();
    
    if ( !mapp(quest = me->query_temp("quest")))
        return "你这人真是的，没事问别人的行踪干嘛？";
        
    if (!me->query_temp("suanming_paid"))
        return "什么？你说什么？我听不见呢。\n";
        
    if (!stringp(name = me->query_temp("quest/name")))
        return "这，你要问谁啊？\n";
    
    if (!stringp(name = me->query_temp("quest/place")))
        return "唉哟，这人可不知道在哪呢。\n";

    me->delete_temp("suanming_paid");        
    return me->query_temp("quest/name")+CYN"是吧？听说有人看见他曾经出现在"+name+CYN"一带。\n"NOR;
}
