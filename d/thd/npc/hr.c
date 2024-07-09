// /u/jpei/thd/npc/huangrong.c
// Modified by Numa 19990914

#include <ansi.h>
inherit NPC;
#include "nadir.c"
#include "thdjob.h"

int ask_husband();
int ask_job_s();
int ask_job();

void create()
{
        set_name("黄蓉", ({ "huang rong", "huang", "rong" }));
        set("long", 
                "她方当韶龄，不过十五六岁年纪，肌肤胜雪，娇美无比，容色绝丽，不可逼视。\n"
                "她长发披肩，全身白衣，头发上束了一条金带，灿然生光。一身装束犹如仙女一般。\n");
        set("gender", "女性");
        set("rank_info/rude", "小妖女");
        set("age", 16);
        set("attitude","peaceful");
        set("str", 16);
        set("dex", 26);
        set("con", 18);
        set("int", 30);
        set("per", 50);
        set("shen_type", 0);

        set("jiali",50);
        set_skill("force", 70);
        set_skill("bihai-chaosheng", 65);
        set_skill("dodge", 90);
        set_skill("xiaoyaoyou", 80);
        set_skill("parry", 80);
        set_skill("strike", 85);
        set_skill("luoying-zhang", 80);
        set_skill("hand", 90);
        set_skill("lanhua-shou", 85);
        set_skill("finger", 60);
        set_skill("tanzhi-shentong", 50);
        set_skill("sword", 80);
        set_skill("yuxiao-jian", 70);
        set_skill("literate",220);
        set_skill("qimen-bagua", 200);

        map_skill("force", "bihai-chaosheng");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("strike", "luoying-zhang");
        map_skill("hand", "lanhua-shou");
        map_skill("finger", "tanzhi-shentong");
        map_skill("parry", "yuxiao-jian");
        map_skill("sword", "yuxiao-jian");
        prepare_skill("hand", "lanhua-shou");

        set("combat_exp", 450000);

        set("max_qi", 1200);
        set("max_jing", 600);
        set("neili", 650);
        set("max_neili", 650);

        set("inquiry", ([
                "name" : "在下黄蓉，是桃花岛主的女儿。",
                "rumors" : "我爹爹在这里布了一个八卦阵，必须精通八卦的卦象才能通过。",
                "here" : "这里就是桃花岛了。寒雨那个小坏蛋在这里加了无数的ｑｕｅｓｔ，看你能不能找全了。",
                "郭靖" : "靖哥哥虽然有点傻乎乎的，对我却是真心的。",
                "黄蓉" : "你找我有什么事吗？",
                "黄药师" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
                "爹爹" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
                "东邪" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
                "周伯通" : "是那个被关在山洞里的怪人吗？有一次我穿过八卦阵去玩，和他聊了半天呢。",
                "八卦阵" : "是爹爹按八卦的卦象排的，不同的方向表示爻的变化与否，每三爻为一卦，八卦按顺序都走对就过阵了。",
                "爻" : "这也不知道？我没法帮你了。",
                "丈夫" : (: ask_husband :),
                "夫君" : (: ask_husband :),
                "寒雨": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
                "jpei": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
// here add thd-job "ask huang" by Numa 19990911
                "师母": (: ask_job_s :),
                "守墓": (: ask_job :),  
        ]) );
        
//here set do job's number
        set("thd/havejob",1);        

        setup();

        carry_object(__THD__"obj/gold_ribbon.c")->wear();
        carry_object("/d/city/obj/necklace.c")->wear();
        carry_object(__THD__"obj/white_cloth.c")->wear();
        carry_object(__THD__"obj/gold_belt.c")->wear();
        carry_object("/d/city/obj/goldring.c")->wear();
}

int ask_husband()
{
        object nvxu, user;
        object ob = this_player();
        string id;
        int user_load = 0;

        if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
                nvxu = load_object("/clone/npc/huang-son-in-law");
        id = nvxu->query("winner");
        if (id == ob->query("id")) {
                if (nvxu->query("death_count") < ob->query("death_count"))
                        say("黄蓉低下头，弄着衣角，什么话也不说。");
                else
                        message_vision(CYN"黄蓉含情脉脉地看着$N，眼光娇羞无限。\n"NOR, ob);
        }
        else {
                if (!(user = find_player(id))) {
                        user = new(LOADUSER_OB, id);
                        if (!user->restore()) {
                                say("黄蓉低下头，弄着衣角，什么话也不说。");
                                destruct(user);
                                return 1;
                        }
                        user_load = 1;
                }
                if (nvxu->query("death_count") < user->query("death_count"))
                        say("黄蓉低下头，弄着衣角，什么话也不说。");
                else
                        command("say 我的丈夫就是" + nvxu->query("name") + "！");
                if (user_load) destruct(user);
        }
        return 1;
}