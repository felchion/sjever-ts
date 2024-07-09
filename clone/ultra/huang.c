// huang 黄裳

#include <ansi.h>

inherit NPC;
inherit F_MASTER;


mixed ask_opinion();
mixed ask_skill();

#define ULTRA_OPINION   "宗师"
#define OPINION_TYPE    HIY "拳脚功夫" NOR 
#define MY_OPINION      "unarmed"

string  *my_opinions = ({ "unarmed", "finger",
                          "strike", "cuff", "hand", "claw" });

// 该头文件必须在这里嵌入
#include "ultra.h"

void create()
{
        set_name("黄裳", ({ "huang shang", "huang" }) );
        set("title", HIY "拳脚大宗师" NOR);
        set("gender", "男性");
        set("age", 92);
        set("long", "此人气度闲雅，看不出武功高低。\n");
        set("attitude", "peaceful");
        set("str", 32);
        set("int", 39);
        set("con", 37);
        set("dex", 32);

        set("max_qi", 20000);
        set("max_jing", 20000);
        set("neili", 30000);
        set("max_neili", 30000);
        set("jiali", 100);

        set("combat_exp", 50000000);

        set("inquiry", ([
                "九阴真经" : "想不到当年我随手写的一本武学心得竟成为危害江湖的不祥之物。",
                "评价"     : (: ask_opinion :),
                "opinion"  : (: ask_opinion :),

        ]));

        set_skill("unarmed", 800);
        set_skill("finger", 800);
        set_skill("claw", 800);
        set_skill("strike", 800);
        set_skill("hand", 800);
        set_skill("cuff", 800);
        set_skill("parry", 800);
        set_skill("dodge", 800);
        set_skill("force", 800);
        set_skill("daode-jing",600);
        set_skill("jiuyin-zhengong", 800);
        set_skill("jiuyin-baiguzhua",800);
        set_skill("jiuyin-shenfa",800);
        set_skill("jiuyin-shenzhang",800);
        //set_skill("martial-cognize", 500);
        set_skill("literate", 600);

        map_skill("parry", "jiuyin-baiguzhua");
        map_skill("force", "jiuyin-zhengong");
        map_skill("dodge", "jiuyin-shenfa");
        map_skill("claw",  "jiuyin-baiguzhua");
        map_skill("strike",  "jiuyin-shenzhang");
        prepare_skill("claw", "jiuyin-baiguzhua");

        set("opinion/ultra",  1);
        set("opinion/master", 1);

        set("rank_info/self", "某家");

        setup();
  
}

void init()
{
        object me;

        if (! interactive(me = this_player()))
                return;

        me->delete_temp("want_opinion");

}

mixed ask_opinion()
{
        object me;
        object ob;

        me = this_player();

        if (me->query("combat_exp") < 200000)
                return "呵呵，你这点武功还谈什么评价？";

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
                return "哈哈，你拳脚方面的修为已经称得上是宗师了！咱们要不要过过招？";

        if (is_fighting(me))
                return "哈哈，废什么话，打赢了我，你自己给自己评价吧！";

        if (me->is_fighting())
                return "你等会儿吧！待我赶走他们。";

        if (objectp(ob = query_temp("opinion")))
        {
                if (ob == me)
                {
                        command("say 你着什么急？不要烦我。");
                        return 1;
                }

                message_vision(CYN "$N" CYN "正在沉思，没有回答$n"
                               CYN "，只是摆了摆手。\n" NOR,
                               this_object(), me);
                return 1;
        }

        switch(me->query_temp("want_opinion/huang"))
        {
        case 0:
                command("look " + me->query("id"));
                command("say 好吧，你空手攻我一招，我看看你的水平如何。");
                break;
        case 1:
                command("say 不是说让你来一招我看看吗？");
                break;
        case 2:
                message_vision(CYN "$N" CYN "皱眉对$n" CYN
                               "道：“我看你这人是够罗嗦的。”\n" NOR,
                               this_object(), me);
                break;
        case 3:
                command("say 给我闭上你的嘴！");
                return 1;
        }

        me->add_temp("want_opinion/huang", 1);
        return 1;
}

int calc_level(object me, string skill)
{
        int lvl;
        string sp;
        mapping action;

        lvl = me->query_skill(skill, 1) / 2;
        sp = me->query_skill_mapped(skill);
        if (! sp) return lvl;

        lvl += me->query_skill(sp, 1);
        action = SKILL_D(sp)->query_action(me, 0);
        if (mapp(action))
                lvl += (action["force"] + action["attack"] +
                        action["parry"] + action["dodge"] - 150) / 15;
        return lvl;
}

void do_congration(object me)
{
        command("chat 恭喜这位" + RANK_D->query_respect(me) + "，不过武"
                "道永无止境，还望继续努力啊！");
}

int accept_hit(object me)
{
        if (me->query_temp("want_hit/huang") >= 2)
        {
                command("heng");
                command("say 事不过三，你要找死那就去死吧！");
                kill_ob(me);
                return 1;
        }

        command("shake");
        command("say 这位" + RANK_D->query_respect(me) + "，"
                "练武之人切忌贸然挑衅。");
        me->add_temp("want_hit/huang", 1);
        return 0;
}

int accept_fight(object me)
{
        object weapon;

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
        {
                message_vision(CYN "$N" CYN "大喜，对$n" CYN
                               "道：“很好，近年我还没有和宗"
                               "师高手过过招，来吧！”\n" NOR,
                               this_object(), me);
                return 1;
        }

        if (! me->query_temp("want_opinion/huang"))
        {
                command("say 平白无故动什么手？");
                return 0;
        }

        if (objectp(weapon = me->query_temp("weapon")))
        {
                command("say 把你手中的" + weapon->name() + NOR +
                        CYN "给我放下！" NOR);
                return 0;
        }

        if (me->query("qi") * 100 / me->query("max_qi") < 80||
            me->query("jing") * 100 / me->query("max_jing") < 80)
        {
                command("say 你现在状态不佳，等休息好了再说吧！");
                return 0;
        }

        command("nod");
        COMBAT_D->do_attack(me, this_object(), 0);
        COMBAT_D->do_attack(this_object(), me, 0);

        if (me->query("qi") <= 0)
        {
                message_vision(CYN "\n$N" CYN "摇摇头对$n" CYN
                               "道：“你现在的武功还差得远，连"
                               "我一招都接不住！”\n" NOR,
                               this_object(), me);
                return notify_fail("看来要好好的修炼功夫了。\n");
        }

        call_out("check_opinion", 2, me);
        set_temp("opinion", me);
        me->delete_temp("want_opinion");
        message_vision(CYN "\n$N" CYN "点点头道：“好了，你先等"
                       "会儿，待我想想。”\n" NOR,
                       this_object(), me);
        return notify_fail("看来不能心急，还得等一会儿。\n");
}

int accept_kill(object me)
{
        if (me->query("combat_exp") < 500000)
        {
                message_vision(CYN "$N" CYN "大怒道：“无名小辈捣"
                               "什么乱？给我滚！”说罢一掌把$n" CYN
                               "劈倒在地。\n" NOR, this_object(), me);
                me->unconcious();
                return -1;
        }
        command("sigh");
        command("say 很好！近年还真没有遇到什么敢和我较量的高手！");
        return 1;
}


void unconcious()
{
        message("vision", "\n"HIR + name() + "脸色一变，吐了一口鲜"
                "血，身形一闪，消逝不见。\n\n" NOR, environment());
        command("chat 罢！罢！罢！老夫去也！");
        destruct(this_object());
}


void die()
{
        message("vision", "\n"HIR + name() + "大叫一声，狂吐了一口鲜"
                "血，身形一闪，就此不见。\n\n" NOR, environment());
        command("chat 老夫去也！");
        destruct(this_object());
}
