// bing.c 官兵

inherit NPC;
#include "guanbing.h"

void create()
{
        set_name("官兵", ({ "guan bing", "bing", "guanbing" }));
        set("age", 22+random(35));
        set("gender", "男性");
	set("shen_type", 1);
        set("attitude", "peaceful");
        set("long", "虽然官兵的武艺不能和武林人士相比，可是他们讲究的是人多力量大。\n");

        set("age", 16 + random(40));
        set("str", 15 + random(5));
        set("dex", 15 + random(5));
        set("con", 15 + random(5));
        set("int", 15 + random(5));
	set("combat_exp", 15000+random(20000));

        set_skill("unarmed", 40);
        set_skill("dodge", 40);
        set_skill("parry", 40);
        set_skill("blade", 40);
        set_skill("force", 40);
        set_temp("apply/attack", 40);
        set_temp("apply/defense", 40);
        set_temp("apply/damage", 20);
        set_temp("apply/armor", 40);

        set("chat_chance_combat", 2);
        set("chat_msg_combat", ({
                "官兵喝道：大胆刁民，竟敢造反不成？\n",
                "官兵喝道：跑得了和尚跑不了庙，你还是快快束手就擒！\n",
        }));
	set("inquiry", ([
		"朝廷":"朝廷的事情也是你打听的嘛？\n",
	]));
        setup();
        carry_object(BINGQI_D("blade"))->wield();
        carry_object(ARMOR_D("junfu"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}
