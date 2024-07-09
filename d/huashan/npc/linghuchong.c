// NPC: /d/huashan/npc/linghuchong.c
// Date: Look 99/03/25
// Modified by Numa@JH 2000 For 黑木崖Job
// Lklv Modify at 2001.10.18

#include <ansi.h>
inherit NPC;
#include "skill1.h"

string ask_chongling();
string ask_ylj1();
string ask_ylj2();
string ask_ylj3();
int ask_ryy();
int ask_heimu();
int ask_follow();

void create()
{
        set_name("令狐冲", ({ "linghu chong", "linghu", "chong" }));
        set("nickname", "大师兄");
        set("long",
"令弧冲身材较瘦，长的浓眉阔眼，气宇暄昂，他在\n"
"同门中排行老大，是华山派年轻一代中的顶尖好手。\n"
"如果掌门人岳不群不在，就由他执掌日常事物。\n");
        set("gender", "男性");
        set("age", 28);
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 30);
        set("int", 40);
        set("con", 30);
        set("dex", 30);
        set("unique", 1);

        set("max_qi", 3000);
        set("max_jing", 1800);
        set("neili", 4000);
        set("max_neili", 4000);
        set("eff_jingli", 1800);

        set("jiali", 60);
        set("combat_exp", 800000);
        set("score", 8000);
        set("startroom","/d/huashan/siguoya");

        set_skill("cuff", 120);
        set_skill("force", 120);
        set_skill("dodge", 120);
        set_skill("parry", 120);
        set_skill("sword", 120);
        set_skill("dugu-jiujian", 120);
        set_skill("zixia-gong", 120);
        set_skill("poyu-quan", 120);
        set_skill("huashan-jianfa", 120);
        set_skill("huashan-shenfa",120);
        set_skill("zhengqi-jue", 120);
        set_skill("chongling-jian", 120);

        map_skill("force", "zixia-gong");
        map_skill("cuff", "poyu-quan");
        map_skill("dodge", "huashan-shenfa");
        map_skill("parry", "huashan-jianfa");
        map_skill("sword", "huashan-jianfa");

        prepare_skill("cuff", "poyu-quan");

        create_family("华山派", 14, "弟子");
        set("chat_msg_combat", ({
                (: random_move :)
        }) );
        set("chat_chance", 15);
        set("chat_msg", ({
                (: random_move :)
        }) );

        set("inquiry", ([
                "风清扬" : "唉，已经很久没有见到他老人家了。你问这干嘛？\n",
                "岳灵珊" : "你找小师妹有什么事啊？\n",
                "仪琳" : "遇见尼姑，逢赌必输\n",
                "紫霞秘籍" : "那是我们华山的内功秘籍，我没有看过。\n",
                "宝剑" : (: ask_ylj3 :),
                "墨纹玄铁" : (: ask_ylj2 :),
                "冲灵剑法" : (: ask_chongling :),
                "遇见尼姑，逢赌必输" : (: ask_ylj1 :),
// add here for ask
                "任盈盈" : (: ask_ryy :),
                "黑木令" : (: ask_heimu :),
                "follow" : (: ask_follow :),
        ]));
        setup();
        carry_object(BINGQI_D("sword/sword"))->wield();
        carry_object(ARMOR_D("armor/cloth"))->wear();
}

string ask_chongling()
{
        mapping fam;
        object me;
        me = this_player();
        fam = me->query("family");
	if (!fam || fam["family_name"] !="华山派")
		return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？\n";
	if (!me->query("hs/getchongling"))
		return "冲灵剑法？那是我和小师妹在朝阳峰舞剑时所创啊。";
	if ((int)me->query_skill("chongling-jian", 1) > 60 )
		return "冲灵剑法贵在独创，你还是自己领悟去吧。\n";
	if ((int)me->query("jingli", 1) < 100 )
		return "你现在身体状况太差了，你无法领悟冲灵剑法的绝妙之处。\n";
	if ((int)me->query("jing", 1) < 100 )
		return "你现在身体状况太差了，你无法领悟冲灵剑法的绝妙之处。\n";
	me->improve_skill("chongling-jian", 20);
	me->receive_damage("jingli",30);
	me->receive_damage("jing", 30);
	me->start_busy(3);
	tell_object(me, HIC"你不由自主跟着令狐冲比划着，对剑法的造诣又深了一层。\n"NOR);
	return  "好吧，既然你问起我冲灵剑法，我就告诉你一次。\n" ;
}

string ask_ylj1()
{
        object me;
        me = this_player();
        command("haha "+me->query("id"));
        me->set_temp("ylj/step1",1);
        return "兄台也知道这件事？这是当初兄弟我为救恒山派的仪琳师\n
妹，向采花大盗田伯光开的玩笑，事后恒山的定静师伯不但没怪我，还送我了\n
一块世间已绝迹的墨纹玄铁，要我用它铸成一把绝世宝剑，行侠处恶，笑傲江\n
湖.....\n";
}

string ask_ylj2()
{
        object me;
        me = this_player();
        if( !me->query_temp("ylj/step1") )
                return "这可是个天大的秘密。\n";
        me->set_temp("ylj/step2",1);
        me->delete_temp("ylj/step1");
        return "说来这墨纹玄铁也是一个宝物，我一直找不到铸剑名师，\n
不想浪费了这块玄铁...。\n";
}

string ask_ylj3()
{
        object me, obj;
        me = this_player();
        if (!me->query_temp("ylj/step2") )
                return "这可是个天大的秘密。\n";
        me->delete_temp("ylj/step2");
        command("look "+me->query("id"));
        if ((int)me->query("shen") > 100000 ){
                obj = unew(__DIR__"obj/xuantie");
                if( clonep(obj) ) {
                        obj->move(me);
                        me->set_temp("ylj/step3",1);
                        return "看兄台一身正气，这块玄铁就送与兄台，望兄台能找到铸\n
剑名师，铸成宝剑，行侠江湖，也不枉费定静师伯的一翻苦心.....\n";
                }
        }
        return "这可是个天大的秘密。\n";
}

int ask_ryy()
{
        object ob = this_player();

        if (ob->query_temp("hostage/job_1") != 2) {
                command("say 我很久没见到她了！");
                command("sigh");
                return 1;
        }
        ob->set_temp("hostage/ask_ling",1);
        command("ah " + ob->query("id"));
        command("say 你见到她了吗？她现在怎么样了？");
        command("say 她的黑木令还在我这里，我正想还给她呢。");
        return 1;
}


int ask_heimu()
{
        object ob = this_player();
        object me = this_object();

        if (!ob->query_temp("hostage/ask_ling")){
                command("say "+RANK_D->query_respect(ob)+"，这种事不是你该打听的！");
                return 1;
        }

        command("say 想要黑木令？嘿嘿，来咱俩比试比试，赢了我就给你！");
        ob->set_temp("hostage/pass_l",1);
        call_out("do_clone",1,me,ob);
        return 1;
}

int accept_fight(object ob)
{
        object me = this_object();

        if ((int)ob->query_temp("hostage/passed_l")){
                command("say 咱们不是比试过了吗？");
                return 0;
        }
        if (me->query("eff_qi") < me->query("max_qi")
        || me->query("qi") < me->query("eff_qi")
        || me->query("neili") < me->query("max_neili")){
        	me->reincarnate();
                message_vision(CYN"$N对$n说：我还没有休息好，你等一会儿吧。\n"NOR, ob, me);
                return 0;
        }
        if (ob->query_temp("hostage/pass_l")){
                message_vision(CYN"$N对$n说：好吧，那我就同你切搓几招吧，点到为止。\n"NOR, ob, me);
                remove_call_out("checking");
                call_out("checking", 1, me, ob);
                ob->delete_temp("hostage/pass_l");
                return 1;
        }
        else return 0;
}

int checking(object me, object ob)
{
        int my_max_qi,his_max_qi;
        my_max_qi  = (int)me->query("max_qi");
        his_max_qi = (int)ob->query("max_qi");

        if ( !present(ob, environment()) )
                return 1;
        if (( (int)me->query("qi")*100 / my_max_qi) <= 50 ){
                message_vision(CYN"$N对$n不住地赞叹："+RANK_D->query_respect(ob)+"果然好功夫，你就先带我去任盈盈那里吧。\n"NOR, me, ob);
                ob->set_temp("hostage/passed_l", 1);
                if (me->query("block_msg/all")>0) {
                        me->revive();
                }
                me->reincarnate();
                me->set_leader(ob);
                me->delete("chat_chance");
                me->set("location",1);
                me->set_temp("no_return",1);
                message_vision("$N决定跟随$n一起行动。\n"NOR, me, ob);
                return 1;
        }
        if (( (int)ob->query("qi")*100 / his_max_qi) < 50 ){
                message_vision(CYN"$N看了看$n，说道：你的功夫稀松平常，怎么带我去找任盈盈？\n"NOR, me,ob);
                message_vision("$N急急忙忙离开了。\n",me);
                ob->set_temp("hostage/passno_l", 1);
                me->move(__DIR__"siguoya");
                message_vision("$N急急忙忙走了过来。\n", me);
                return 1;
        }
        if (me->is_fighting()) {
                call_out("checking",1, me, ob);
                return 1;
        }
        return 1;
}

int ask_follow()
{
        object ob = this_object();
        object me = this_player();

        if (!me->query_temp("hostage/passed_l"))
                return 0;
        message_vision(CYN"$n哈哈大笑，对着$N说道：那就走吧！\n"NOR,me,ob);
        ob->set_leader(me);
        message_vision("$n决定跟随$N一起行动。\n"NOR,me,ob);
        ob->delete("chat_chance");
        return 1;
}
