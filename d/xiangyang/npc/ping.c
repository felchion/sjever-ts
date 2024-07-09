#include <ansi.h>
inherit NPC;
inherit F_VENDOR;
int ask_quest(object);
int ask_me();

void create()
{
        set_name("平一指", ({"ping yizhi", "ping"}));
        set("long",
                "他就是江湖上闻名的杀人名医平一指。\n"
                "杀一人，救一人，不做赔本生意。\n");
        set("title",HIR"杀人名医"NOR);
        set("rank_info/rude", "死庸医");
        set("attitude","friendly");
        set("no_bark",1);
        set("str", 20);
        set("dex", 20);
        set("con", 20);
        set("int", 20);
        set("per", 20);
        set("gender", "男性");
        set("age", 47);
        set("shen_type", 0);

        set("unique", 1);
        set("no_get","平一指对你而言太重了。\n");
        set("vendor_goods", ({
                (["name": MEDICINE_D("huiyang"), "number":3]),
                (["name": MEDICINE_D("jinchuang"), "number": 5]),
                (["name": MEDICINE_D("jingmai/jingmai-book3"), "number": 1]),
        }));
        set("combat_exp", 1000000);
        set("max_qi", 5000);
        set("max_jing", 5000);
        set("eff_jingli",5000);
        set("max_jingli",5000);
        set("eff_jing",5000);
        set("neili", 5000);
        set("max_neili", 5000);
        set("jiali",50);

        set_skill("force", 200);
        set_skill("bihai-chaosheng", 200);
        set_skill("dodge", 200);
        set_skill("suibo-zhuliu", 200);
        set_skill("parry", 200);
        set_skill("finger", 300);
        set_skill("tanzhi-shentong", 300);
        set_skill("qimen-bagua", 200);

        map_skill("force", "bihai-chaosheng");
        map_skill("dodge", "suibo-zhuliu");
        map_skill("finger", "tanzhi-shentong");
        map_skill("parry", "tanzhi-shentong");

        prepare_skill("finger", "tanzhi-shentong");

        set("inquiry", ([
                "经脉学": "怎么？是来求教的么？",
                "求教": (: ask_quest :),
                "杀人": (: ask_quest :),
                "疗伤" : (:ask_me:),
        ]));

        set("waiting",0);

        setup();

        carry_object(ARMOR_D("changshan"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}


void init()
{
        object ob;
        ::init();
        if( interactive(ob = this_player()) && visible(ob) && !is_fighting() ) {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
        add_action("do_buy", "buy");
        add_action("do_list", "list");
}

void greeting(object ob)
{
       if(!ob || environment(ob) != environment()) return;
       if((ob->query("eff_qi")*100 / ob->query("max_qi") < 5)
          && (ob->query_skill("jingmai-xue",1)<170)
          && (ob->query_skill("jingmai-xue",1)>=160)) {
                command("ah");
                command("say 这下真是糟糕，你的伤势实在太严重了。");
                command("say 我如果随便治疗反而会伤到你的经脉。");
                command("consider");
                command("say 你的经脉学已经有相当根底了，我这本秘藏的脉经你拿去研究研究，想办法自疗吧！");
                message_vision(HIY"$N给了$n一本王叔和脉经。\n"NOR, this_object(), ob);
                new("/clone/medicine/jingmai/jingmai-book7")->move(ob);
        }
}

int ask_quest(object me)
{
        int i;
        object *list;
        object where, target;
        object ob2 = load_object("/clone/npc/zhong-shentong");
        if (this_player()) me = this_player();
        if (!me) return 0;
        if (me->query_temp("count")>10) {
                command("say 现在我想不出什么人好杀，你等等再问吧。");
                me->delete_temp("count");
        return 1;}
        if (me->query_temp("ping_quest")) {
                command("say 不杀了？那就算了，反正我也还没给你什么。");
                delete("waiting");
                return 1;}
        if (query("waiting")) {
                command("say 你等等吧，我在等人呢。");
                return 1;}
        if (me->query_skill("jingmai-xue",1)>=170) {
                command("say 要帮我杀人？先杀了人再救你，这生意不错。");
                return 1;}
        if (me->query_skill("jingmai-xue",1)>=160 && me->query_skill("jingmai-xue",1)<170) {
                command("say 你的经脉学已经不错了，如果不是什么紧急情况，也够用了。");
                return 1;}
        if (me->query("combat_exp")< 850000 ) {
                command("say 你经验太低，学了也没用。");
                return 1;}
        if (me->query_skill("jingmai-xue",1)<150) {
                command("say 你经脉学技巧太低，看不懂的。");
                return 1;}
        if (me->query_int() < 40) {
                command("say 你悟性太差，看不懂的。");
                return 1;}
        list = filter_array(users(), (: $1->query("combat_exp")>=1000000 :));
        target = list[random(sizeof(list))];
        i = target->query("combat_exp")/5;
        where = environment(target);
        if ((!environment(target)) ||
        (wizardp(target) || target->query("env/invisibility")) ||
        (!interactive(target)) ||
        (target == me) ||
        (ob2->query("winner") == me->query("id")) ||
        (target->query("combat_exp")<me->query("combat_exp")-i) ||
        (target->query("combat_exp")>me->query("combat_exp")+i) ||
        (query_idle(target) > 2000)) {
                me->add_temp("count",1);
                call_out("ask_quest", 1, me);
                return 1; } else {
                if (me->query_skill("jingmai-xue",1)<170) {
                command("say 要我指导你？不过，我救一人前要杀一人，你听说过吧？");
                command("say 要是我教了你，你拿去救人，我就亏本了。"); }
                command("say 你去帮我杀了"+HIR+target->query("name")+HIW+"("+target->query("id")+")"+CYN"吧。");
                command("say 你去"+HIG+where->query("short")+CYN+"找找看。");
                command("sing3");
                target->set_temp("no_zhong",1);
                set("target",target->query("name"));
                set("targetid",target->query("id"));
                me->set_temp("ping_quest",1);
                set("waiting",1);
        return 1;
        }
}

int accept_object(object me, object ob)
{
        if (!me->query_temp("ping_quest")) {
                command("say 给我这个做什么？");
                return 0; }
        if (ob->query("name")==query("target")+"的首级"
        && ob->query("victim_id")==query("targetid")
        && ob->query("material")=="meat") {
                command("nod");
        if (ob->query_skill("jingmai-xue",1)>=170) {
                command("say 好，我不会亏待你的。");
                ob->add("ping/kill",1);
        } else {
                command("say 好，那么我就把这本书传给你了，好好研究吧。");
                message_vision(HIY"平一指给了$N一本黄帝内经。\n"NOR, me);
                new("/clone/medicine/jingmai/jingmai-book6")->move(me);}
                set("waiting",0);
                return 1; } else {
                command("say 给我这个做什么？");
                set("waiting",0);
       return 0; }

}

int ask_me()
{
        object ob;
        ob=this_player();

        if (ob->query("ping/kill")<1) {
            command("grin1 ping");
            command("say 我可不随便救人，要我疗伤先帮我杀个人再说。");
            return 1;}
        if ((int)ob->query("eff_qi") == (int)ob->query("max_qi"))
        {
                command("?"+ob->query("id"));
                command("say 你根本没受伤，想捣乱？\n");
                command("knock "+ob->query("id"));
                return 1;
        }
        else
        {
                message_vision("平一指手指微动，连续点过$N身上十八处大穴！$N感觉舒服多了。\n", ob);
                remove_call_out("recover");
                call_out("recover",2,ob);
                ob->add("ping/kill",-1);
                return 1;
        }
}

int recover(object ob)
{
        ob->set("eff_qi", (int)ob->query("max_qi"));
        ob->set("eff_jing", (int)ob->query("max_jing"));
        message_vision("一柱香的工夫过去了，你觉得伤势已经基本痊愈了。\n",ob);
        return 1;
}
