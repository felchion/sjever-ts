// Modified by Numa 19991018
// Fixed Tianqi-san too many 's problem
// Modified by Darken@SJ     slogan numa :)

#include "nadir.c"
inherit NPC;

// string ask_yao1();
string ask_yao2(); 
string ask_san();
string ask_dan();
object ob = this_object();
void create()
{
        set_name("程瑶迦", ({"cheng yaojia", "cheng", "yaojia"}) );
        set("long", "她端端正正的坐着，脸上晕红，嘴角含笑，却不知心中在想些甚么。\n");
        set("gender", "女性" );
        set("title", "少庄主夫人");
        set("age", 24);
        set("attitude", "peaceful");
        set("str",25);
        set("int",25);
        set("con",27);
        set("dex",26);
        set("per",28);
        set("max_qi", 1000);
        set("max_jing", 450);
        set("neili", 800);
        set("max_neili", 800);
        set("eff_jingli",500);
        set("jiali", 30);
        set("combat_exp", 300000);
        set("unique", 1);

        set_skill("force", 100);
        set_skill("xiantian-gong", 90);
        set_skill("dodge", 90);
        set_skill("suibo-zhuliu", 90);
        set_skill("parry", 90);
        set_skill("strike", 100);
        set_skill("luoying-zhang", 90);
        set_skill("leg", 110);
        set_skill("xuanfeng-tui", 110);
        set_skill("sword", 90);
        set_skill("quanzhen-jianfa", 80);
        set_skill("literate", 100);
        set_skill("qimen-bagua", 60);

        map_skill("force", "xiantian-gong");
        map_skill("dodge", "suibo-zhuliu");
        map_skill("strike", "luoying-zhang");
        map_skill("leg", "xuanfeng-tui");
        map_skill("parry", "luoying-zhang");
        map_skill("sword", "quanzhen-jianfa");
        prepare_skill("strike", "luoying-zhang");

        set("shen_type", 1);
        set("shen", 200);
        set("class", "fighter");
        set("yao", 10);   
        set("san", 5);
        set("dan", 10);

        set("inquiry", ([
//                "六壬集气丸": (: ask_yao2 :),
                "田七鲨胆散": (: ask_san :),
                "无常丹": (: ask_dan :),
        ]) );

        setup();
        carry_object(__THD__"obj/white_cloth.c")->wear();
}

string ask_yao1()
{
        object ob = this_player();

        if (ob->query("family/family_name") != "桃花岛")
                return RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？";

        if (ob->query_condition("jingli_drug") > 0 )
                return RANK_D->query_respect(ob) + "你不是刚吃过药吗，怎么又来要了？灵药多吃有害无宜，过段时间再来吧。";

        if (ob->query("max_jingli" ) < 200)
                return RANK_D->query_respect(ob) + "功力不够，灵药多吃有害无宜，过段时间再来吧。";

        if (present("sancai dan", ob))
                return RANK_D->query_respect(ob) + "你现在身上不是有颗药丸吗，怎么又来要了？真是贪得无厌！";

        if (query("yao") < 1) return "对不起，药已经发完了，新的还未炼出。";
   
        new(__GUI__"obj/sancai-dan")->move(ob);

        add("yao", -1);

        message_vision("$N获得一颗三才聚精丹。\n", ob);
        return "好吧，此药练之不易，对练武之人大有好处，你要小心收藏好了。";
}

string ask_yao2()
{
        object ob = this_player();

        if (ob->query("family/family_name") != "桃花岛")
                return RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？";

        if (ob->query_condition("medicine") > 0 )
                return RANK_D->query_respect(ob) + "你不是刚吃过药吗，怎么又来要了？灵药多吃有害无宜，过段时间再来吧。";

        if (this_player()->query("max_neili") < 400)
                return RANK_D->query_respect(ob) + "功力不够，灵药多吃有害无宜，过段时间再来吧。";

        if (present("liuren wan", ob))
                return RANK_D->query_respect(ob) + "你现在身上不是有颗药丸吗，怎么又来要了？真是贪得无厌！";

        if (query("yao") < 1) return "对不起，药已经发完了，新的还未炼出。";

        if(ob->query("max_neili")>= (ob->query_skill("force")* 10 + ob->query("con")* ob->query("age") +ob->query("combat_exp",1)/1000))
                     return RANK_D->query_respect(ob) + "此药已经难以在帮助你提高内力修为了！";
        new("/clone/medicine/liuren-wan")->move(ob);

        add("yao", -1);

        message_vision("$N获得一颗六壬集气丸。\n", ob);
        return "好吧，此药练之不易，对练武之人大有好处，你要小心收藏好了。";
}

string ask_san()
{
        object ob = this_player(),obj;

        if (ob->query("family/family_name") != "桃花岛")
                return RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？";

        if (ob->query("combat_exp") < 500000)
                return RANK_D->query_respect(ob) + "功夫太浅，吃了不会有什么帮助的。";

        if (this_player()->query("max_neili") < 1000)
                return RANK_D->query_respect(ob) + "功力不够，灵药多吃有害无宜，过段时间再来吧。";

        if (present("tianqi san", ob))
                return RANK_D->query_respect(ob) + "你现在身上不是有吗，怎么又来要了？真是贪得无厌！";
        if (ob->query_temp("thd/get_tianqi")>0)
                return RANK_D->query_respect(ob) + "你刚才要过，怎么又来要了？真是贪得无厌！";

        if (query("san") < 1) return "对不起，药已经发完了，新的还未炼出。";

        obj = unew(__GUI__"obj/tianqi-san");
        if(!clonep(obj))
                return "对不起，药已经发完了，新的还未炼出。";

        obj->move(ob);
        ob->set_temp("thd/get_tianqi",1);

        add("san", -1);

        message_vision("$N获得一包田七鲨胆散。\n", ob);
        return "好吧，此药练之不易，你要小心收藏好了。";
}

string ask_dan()
{
        object ob = this_player();

        if (ob->query("family/family_name") != "桃花岛")
                return RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？";

        if (ob->query("combat_exp") < 500000)
                return RANK_D->query_respect(ob) + "功夫太浅，吃了不会有什么帮助的。";

        if (this_player()->query("max_neili") < 400)
                return RANK_D->query_respect(ob) + "功力不够，灵药多吃有害无宜，过段时间再来吧。";

        if (present("wuchang dan", ob))
                return RANK_D->query_respect(ob) + "你现在身上不是有颗药丸吗，怎么又来要了？真是贪得无厌！";

        if (query("dan") < 1) return "对不起，药已经发完了，新的还未炼出。";

        new("/d/thd/guiyun/obj/wuchang-dan")->move(ob);

        add("dan", -1);

        message_vision("$N获得一颗无常丹。\n", ob);
        return "好吧，此药练之不易，你要小心收藏好了。";
}


