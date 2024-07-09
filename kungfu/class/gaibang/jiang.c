// jiang.c 蒋舵主
// update by looklove 2000,7,3
// Modify By Looklove 2000/10/21
// Modified by looklove 2001/5/24 for xlz quest

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

#include "begger.h";

void create()
{
        set_name("蒋舵主", ({"jiang duozhu", "jiang" }));
        set("title", YEL"丐帮大义分舵舵主"NOR);
        set("gb/bags",8);
        set("gender", "男性");
        set("age", 47);
        set("long", "他是八袋舵主，掌管大义分舵。\n");
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 25);
        set("int", 20);
        set("con", 25);
        set("dex", 25);
        set("unique", 1);

        set("qi", 1200);
        set("max_qi", 1200);
        set("jing", 800);
        set("max_jing", 800);
        set("eff_jingli", 940);
        set("neili", 1400);
        set("max_neili", 1400);
        set("jiali", 100);

        set("combat_exp", 320000);

        set_skill("force", 100); // 基本内功
        set_skill("huntian-qigong", 100); // 混天气功
        set_skill("strike", 100); // 基本拳脚
        set_skill("blade",90);
        set_skill("liuhe-daofa",90);
        set_skill("dodge", 90); // 基本躲闪
        set_skill("xiaoyaoyou", 90); // 千里行乞
        set_skill("parry", 90); // 基本招架
        set_skill("xianglong-zhang",90);
        map_skill("parry","liuhe-daofa");
        map_skill("strike","xianglong-zhang");
        map_skill("blade","liuhe-daofa");
        map_skill("force", "huntian-qigong");
        map_skill("dodge", "xiaoyaoyou");

        create_family("丐帮", 19, "八袋弟子");
        set("chat_chance", 1);
        set("chat_msg", ({
                "蒋舵主叹道:真怀念以前和乔帮主一起饮酒的日子啊。\n",
        }) );
        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔帮主" : "不管怎样，他永远是我的乔帮主。\n",
                "喝酒" : "你也喜欢喝酒，那太好了！\n",
                "小岛" : "在东门外，海港东南方向，可以游过去。\n",
                "萧峰" : "不管怎样，他永远是我的乔帮主。\n",
        ]));
        setup();
        carry_object(__DIR__"obj/gb_budai8")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("blade"))->wield();
}

void attempt_apprentice(object ob)
{
        if( ob->query("shen") < 1000 ) return;
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        command("recruit " + ob->query("id"));
        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大义分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大义分舵");
        ob->set("class","beggar");
}
