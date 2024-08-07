// Npc: /kungfu/class/shaolin/xuan-ku.c
// Date: YZC 96/01/19

inherit NPC;
inherit F_MASTER;

string ask_me();

void create()
{
        object ob;
        set_name("玄苦大师", ({
                "xuanku dashi",
                "xuanku",
                "dashi",
        }));
        set("long",
                "他是一位白须白眉的老僧，身穿一袭银丝棕黄袈裟。他身材瘦高，\n"
                "脸上满布皱纹，手臂处青筋绽露，似乎久经风霜。\n"
        );


        set("nickname", "罗汉堂首座");
        set("gender", "男性");
        set("attitude", "friendly");
        set("class", "bonze");

        set("age", 70);
        set("shen_type", 1);
        set("str", 30);
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("max_qi", 1800);
        set("max_jing", 1000);
        set("neili", 2000);
        set("max_neili", 2000);
        set("jiali", 150);
        set("combat_exp", 1000000);
        set("score", 500000);

        set_skill("force", 150);
        set_skill("yijin-jing", 150);
        set_skill("dodge", 150);
        set_skill("shaolin-shenfa", 150);
        set_skill("parry", 150);
        set_skill("cuff", 150);
        set_skill("club", 150);
        set_skill("blade", 150);
        set_skill("sword", 150);
        set_skill("luohan-quan", 150);
        set_skill("weituo-chu", 150);
        set_skill("xiuluo-dao", 150);
        set_skill("fumo-jian", 150);
        set_skill("buddhism", 150);
        set_skill("literate", 150);
set_skill("zui-gun",150);
        map_skill("force", "yijin-jing");
		map_skill("dodge", "shaolin-shenfa");
		map_skill("parry", "luohan-quan");
        map_skill("cuff", "luohan-quan");
        map_skill("club", "weituo-chu");
        map_skill("blade", "xiuluo-dao");
        map_skill("sword", "fumo-jian");

        prepare_skill("cuff", "luohan-quan");

        create_family("少林派", 36, "弟子");

        set("inquiry", ([
                "十八罗汉阵"       : (: ask_me :),
                "罗汉大阵"         : (: ask_me :)
        ]));

        setup();
	if (clonep()) {
                ob = unew(BINGQI_D("blade/jinnao"));
		if (ob) {
			destruct(ob);
                        ob = unew(BINGQI_D("blade/jinnao2"));
		}
              if (!ob) ob = unew(BINGQI_D("blade"));
		ob->move(this_object());
		ob->wield();
		carry_object("/d/shaolin/obj/xuan-cloth")->wear();
	}
}

#include "/kungfu/class/shaolin/xuan-ku.h"
