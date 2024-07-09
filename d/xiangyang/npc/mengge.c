#include <ansi.h>
inherit NPC;
inherit F_UNIQUE;
inherit F_SAVE;
int ask_job();
void create()
{
    set_name("蒙哥", ({ "meng ge", "meng", "ge" }));
    set("title","蒙古元帅");
    set("gender", "男性");
    set("long", "这是此次负责进攻襄阳的蒙古元帅。\n");
    set("age", 25);
    set("str", 30);
    set("int",20);
    set("con",30);
    set("dex",20);
    set("combat_exp", 300000);
    set("attitude", "friendly");
    set("max_qi",2000);
    set("max_jing",1000);
    set("neili",2000);
    set("max_neili",2000);
    set("qi",2000);
    set("jing",1000);
    set("jingli",1000);
    set("score", 100);
    set_skill("force", 100);
    set_skill("dodge", 100);
    set_skill("parry", 100);
        set_skill("yijin-jing", 100);
        set_skill("shaolin-shenfa", 100);
        set_skill("blade", 100);
        set_skill("xiuluo-dao",100);
        set_skill("buddhism", 100);
        set_skill("literate", 100);

        map_skill("force", "yijin-jing");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("parry", "xiuluo-dao");
        map_skill("blade", "xiuluo-dao");

        set("sljob",3);
        set("xxjob",4);
        set("xsjob",2);
        set("gjob",1);
        set("job",5);

        set("inquiry", ([
//          "援手": (: ask_job :),

        ]) );

        setup();
        carry_object(BINGQI_D("blade"))->wield();
        carry_object(ARMOR_D("armor"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

int ask_job()
{
        object me = this_player();

        if (me->query("combat_exp")<800000)
        {
                 command("say 就凭你这样的功夫能做什么？！");
                 command("sneer");
                 return 1;
        }
        if (me->query_temp("xy/mgjob") ||
            me->query_temp("xy/xxjob") ||
            me->query_temp("xy/xsjob") ||
            me->query_temp("xy/sljob"))
        {
                command("say 还不快去做！");
                command("push " + me->query("id"));
                return 1;
        }
        if (me->query_condition("xyjob") > 0 || me->query_condition("xy_job") > 0)
        {
                command("say 你到一边去等候吩咐吧，我要准备攻城了。");
                 return 1;
        }
        if ((me->query("family/family_name") == "神龙教")&&(query("sljob")>1))
        {
                 command("say 洪老头的弟子？你们的暗器不错吧？");
                 command("say 这些石灰粉拿去，整治一下东面那些人，想办法破阵。");
                 new("/d/xiangyang/obj/shihui")->move(me);
                 new("/d/xiangyang/obj/mask")->move(me);
                 me->apply_condition("xy_job",15);
                 message_vision(HIY"蒙哥给$N一包石灰粉和一个面罩。\n"NOR,me);
                 me->set_temp("xy/sljob",1);
                 add("sljob",-1);
                 return 1;
        } else if ((me->query("family/family_name") == "星宿派")&&(query("xxjob")>1))
        {
                 command("say 星宿老怪的徒弟？听说你们的毒技勉强过得去。");
                 command("say 这些蒙汗药拿去，对付西门那些小辈吧。");
                 new("/d/xiangyang/obj/mask")->move(me);
                 new("/d/xiangyang/obj/menghan")->move(me);
                 message_vision(HIY"蒙哥给$N一包蒙汗药和一个面罩。\n"NOR,me);
                 me->apply_condition("xy_job",15);
                 me->set_temp("xy/xxjob",1);
                 add("xxjob",-1);
                 return 1;
        } else if ((me->query("family/family_name") == "大轮寺")&&(query("xsjob")>1))
        {
                 command("say 嘿嘿，贵派血刀老祖的药粉灵验得很啊。");
                 command("say 你就拿些去，把郭靖的女娃子抓一个回来吧。");
                 new("/d/xiangyang/obj/mask")->move(me);
                 new("/d/xiangyang/obj/hehuan")->move(me);
                 message_vision(HIY"蒙哥给$N一包阴阳合和散和一个面罩。\n"NOR,me);
                 me->apply_condition("xy_job",15);
                 me->set_temp("xy/xsjob",1);
                 add("xsjob", -1);
                 return 1;
        } else if (query("job")>1)
        {
                 command("say 你们自负是名门正派，想必不会做下流勾当了。");
                 command("say 那么你就帮我去杀些城中的守城弟子吧！");
                 new("/d/xiangyang/obj/mask")->move(me);
                 message_vision(HIY"蒙哥给$N一个面罩。\n"NOR,me);
                 me->apply_condition("xy_job",15);
                 me->set_temp("xy/mgjob",1);
                 add("job",-1);
                 return 1;
        }
        command("say 我们蒙古军纵横天下，才不用帮忙。");
        command("kick "+me->query("id"));
        return 1;
}


int accept_object(object me, object ob)
{
        int exp, pot, busy;
        if (!me->query_temp("xy/mgjob") &&
            !me->query_temp("xy/sljob") &&
            !me->query_temp("xy/xsjob") &&
            !me->query_temp("xy/xxjob") )
              return notify_fail("你凑什么热闹？\n");
        busy =  30 - (int)me->query_condition("xy_job");
        exp = random((me->query("xy/job")+1)/10)
               + random((int)me->query("age",1))
               + random(100) + 200
               - busy*busy/2;
        if (exp<250) exp = 200+random(100);
        if (exp>400) exp = 350+random(100);
        pot = exp/3 + random(exp/4);

        message_vision(HIR"$N觉得一股邪念自心中升起。\n"NOR,me);
        switch(ob->query("name")) {
              case "武敦儒的首级":
              case "武修文的首级":
              case "耶律燕的首级":
              case "完颜萍的首级":
              if (me->query("family/family_name") == "星宿派") {
                     me->add("xy/job",1);
                     command("say 好，你做的很好！");
                     command("haha");
                     destruct(ob);
                     me->delete_temp("xy");
                     me->add("combat_exp",exp);
                     me->add("potential", pot);
                     if (me->query("potential")>me->query("max_pot"))
                     me->set("potential", me->query("max_pot"));
                     me->add("shen", -(exp*10));
      message_vision(HIW"$N获得了"+CHINESE_D->chinese_number(exp)+"点经验，"
      +CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR, me);
      write_file("/log/job/xyjob2",sprintf("%s %s(%s)%d次突袭西门得到经验%d，潜能%d。\n",
ctime(time())[4..19],me->name(1),me->query("id"),(int)me->query("xy/job"),exp,pot));
                     return 1; }
             else { command("say 你偷了个头就想要奖励啊？");
                     me->delete_temp("xy");
                       return 1;}
              break;

              case "程英的首级":
              case "冯默风的首级":
              case "武天风的首级":
              if (me->query("family/family_name") == "神龙教") {
                     me->add("xy/job",1);
                     command("say 好，你做的很好！");
                     command("haha");
                     destruct(ob);
                     me->delete_temp("xy");
                     me->add("combat_exp",exp);
                     me->add("potential", pot);
                     me->add("shen", -(exp*10));
                     if (me->query("potential")>me->query("max_pot"))
                         me->set("potential", me->query("max_pot"));
      message_vision(HIW"$N获得了"+CHINESE_D->chinese_number(exp)+"点经验，"
      +CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR, me);
      write_file("/log/job/xyjob2",sprintf("%s %s(%s)%d次突袭东门得到经验%d，潜能%d。\n",
ctime(time())[4..19],me->name(1),me->query("id"),(int)me->query("xy/job"),exp,pot));
                     return 1; }
             else { command("say 你偷了个头就想要奖励啊？");
                     me->delete_temp("xy");
                       return 1;}
              break;

              case "郭靖的首级":
                     if (!me->query_temp("xy/gjob"))
                     { command("抢功也不是这么个抢法的。");
                       command("这次就不给奖励了。");
                     me->delete_temp("xy/job");
                       return 1; }
                     me->add("xy/job",1);
                     command("say 好，你做的很好！");
                     command("haha");
                     destruct(ob);
                     me->delete_temp("xy");
                     me->add("combat_exp",exp*3/2);
                     me->add("potential", pot*3/2);
                     if (me->query("potential")>me->query("max_pot"))
                         me->set("potential", me->query("max_pot"));
                     if (me->query("family/family_name") != "桃花岛") {
                             me->add("shen", -(exp*10)); }
      message_vision(HIW"$N获得了"+CHINESE_D->chinese_number(exp*3/2)+"点经验，"
      +CHINESE_D->chinese_number(pot*3/2)+"点潜能的奖励。\n"NOR, me);
      write_file("/log/job/xyjob2",sprintf("%s %s(%s)%d次暗杀郭靖得到经验%d，潜能%d。\n",
ctime(time())[4..19],me->name(1),me->query("id"),(int)me->query("xy/job"),exp*3/2,pot));
                     return 1;
              break;

              case "丐帮弟子的首级":
              case "峨嵋弟子的首级":
              case "耶律齐的首级":
                     me->add("xy/job",1);
                     command("say 好，你做的很好！");
                     command("haha");
                     destruct(ob);
                     me->delete_temp("xy");
                     me->add("combat_exp",exp);
                     me->add("potential", pot);
                     if (me->query("potential")>me->query("max_pot"))
                         me->set("potential", me->query("max_pot"));
                     if (me->query("family/family_name") != "桃花岛") {
                             me->add("shen", -(exp*10)); }
      message_vision(HIW"$N获得了"+CHINESE_D->chinese_number(exp)+"点经验，"
      +CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR, me);
      write_file("/log/job/xyjob2",sprintf("%s %s(%s)%d次突袭城内得到经验%d，潜能%d。\n",
ctime(time())[4..19],me->name(1),me->query("id"),(int)me->query("xy/job"),exp,pot));
                     return 1;
              break;

              case "郭襄":
              case "郭芙":
              if (me->query("family/family_name") == "大轮寺") {
                     me->add("xy/job",1);
                     command("say 好，你做的很好！");
                     command("haha");
                     destruct(ob);
                     me->delete_temp("xy");
                     me->add("combat_exp",exp);
                     me->add("potential", pot);
                     me->add("shen", -(exp*10));
                     if (me->query("potential")>me->query("max_pot"))
                         me->set("potential", me->query("max_pot"));
      message_vision(HIW"$N获得了"+CHINESE_D->chinese_number(exp)+"点经验，"
      +CHINESE_D->chinese_number(pot)+"点潜能的奖励。\n"NOR, me);
      write_file("/log/job/xyjob2",sprintf("%s %s(%s)%d次俘虏人质得到经验%d，潜能%d。\n",
ctime(time())[4..19],me->name(1),me->query("id"),(int)me->query("xy/job"),exp,pot));
                     return 1; }
             else { command("say 又没叫你去抓，这次就当作送礼啦。");
                       command("hehe");
                     me->delete_temp("xy");
                       return 1;}
              break;
              }
}

