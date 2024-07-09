// mao.c
// Creator : Numa@Sj 00-5-28 12:02

#include <ansi.h>
inherit NPC;

string do_give_book();
string do_lingwu_dmd();

void create()
{
        set_name("茅十八", ({ "mao shiba", "mao" }));
        set("gender", "男性");
        set("age", 32);
        set("long", "一个虬髯大汉，头上包了白布。\n");
        set("attitude", "peaceful");
        set("shen_type", 0);
        set("str", 30);
        set("int", 20);
        set("con", 30);
        set("dex", 26);
        set("startroom", "/d/city/dongxiangfang");

        set("max_qi", 1000);
        set("max_jing", 1000);
        set("neili", 1000);
        set("max_neili", 1000);
        set("combat_exp", 200000);
        set("inquiry", ([
                "name" : "我姓茅，茅草的茅，不是毛虫之虫，排行第十八。茅十八便是我了。",
                "rumors" : "咱们都是大汉百姓，花花江山却给鞑子占了，总有一日，要把鞑子杀得干干净净！",
                "秘籍" : (: do_give_book :),
                "五虎断门刀" : (: do_lingwu_dmd :),
        ]));

        set_skill("force", 100);
        set_skill("huntian-qigong",100);
        set_skill("dodge", 120);
        set_skill("xiaoyaoyou",120);
        set_skill("blade",120);
        set_skill("wuhu-duanmendao",120);
        set_skill("parry", 100);
        set_skill("taizu-quan", 100);
        set_skill("cuff",100);
        map_skill("force", "huntian-qigong");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("parry", "wuhu-duanmendao");
        map_skill("blade", "wuhu-duanmendao");
        map_skill("cuff", "taizu-quan");
        prepare_skill("cuff", "taizu-quan");
        setup();

        carry_object(__DIR__"obj/cloth")->wear();
        carry_object(__DIR__"obj/blade")->wield();
}

string do_give_book()
{

        object me = this_player();
        object ob;

        if (!me->query("quest/whdmd/pass"))
                return "秘籍？什么秘籍？不知道。";
        if (present("wuhuduanmendao miji",me))
                return "耶？你身上不是有一本吗？";
        ob = new("/d/city/obj/whdmd-book");
        if (!ob)
                return "秘籍我给弄丢了，你还是告诉巫师解决吧！";
        ob->move(me);
        ob->set_temp("mao18/book",me->query("id"));
        message_vision("茅十八递给$N一本秘籍。\n", me, );
        return "这本秘籍你拿去好好研读，将我们这门功夫好好发扬光大。";
}

string do_lingwu_dmd()
{
        object me = this_player();
        int i;

        if (!me->query("quest/whdmd/pass"))
                return "五虎断门刀可是我的绝学，你想干什么？";
        if (me->query("mao18/pass2"))
                return "我的全部所学都已经教给你了呀！";
        i = me->query_skill("wuhu-duanmendao",1);
        if (i != 150)
                return "我不去好好研读秘籍，跑到这里来干什么？";
        me->set("mao18/pass2",1);
        tell_object(me, CYN"茅十八对你说道：我们这门五虎断门刀绝学乃是一无名老者所创，其招式辛辣，堪称一代奇功。\n我现在就把这手「猛虎下山」传授与你。\n"NOR);
        tell_object(me, HIY"你听了茅十八的话，心中豁然开朗，五虎断门刀的造诣更深了一层！\n"NOR);
        tell_room(environment(me), "茅十八在"+me->name()+"耳边嘀嘀咕咕的说了些话。\n", ({me}));
        return "你回去加紧修炼吧！";
}
