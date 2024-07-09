// NPC: /d/wizard/npc/shitong.c
// Date: Look 99/10/25

inherit NPC;

string ask_gonglao();

void create()
{
        set_name("狱卒", ({ "yu zu", "yuzu" }));
        set("long", "他是官府监狱的狱卒，一脸威严的表情。\n");
        set("age", 47);
        set("combat_exp", 20000);
        set("int", 10);
        set("con", 50);
        set("dex", 50);
        set("str", 50);
        set("no_quest", 1);

        set("attitude", "peaceful");
        set("shen_type", 1);
        set("max_qi", 9000);
        set("qi", 9000);
        set("max_jing", 300);
        set("jing", 300);
        set("eff_jingli", 300);
        set("jingli", 300);
        set("max_neili", 300);
        set("neili", 300);
        
        set("inquiry", ([
              "释放" : (: ask_gonglao :),                        
        ]));

        setup();
        carry_object(ARMOR_D("armor/cloth"))->wear();
}

string ask_gonglao()
{
        object me=this_player();
        return "想离开这里？，"+RANK_D->query_respect(me)+"花二两银子问我买一件衣服就可以了！";
}

int accept_object(object who, object ob)
{
	object cloth;

	if (ob->query("money_id") && ob->value() >= 200) 
	{
		command("nod");
		command("say 既然你有悔过之心，那我就给你一件衣服，你穿好赶紧就走吧。");
		cloth = new("/clone/armor/cloth");
		cloth->move(who);
		who->move("/d/city/dilao2");
		who->delete_temp("nude");
		return 1;
		
		
	}
	return 0;
}

