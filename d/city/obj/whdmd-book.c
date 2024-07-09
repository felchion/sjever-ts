// whdmd-boook.c
// Created By Numa@SJ 2000

#include <ansi.h>

inherit ITEM;

void create()
{
        set_name(HIM"五虎断门刀秘籍"NOR, ({ "wuhuduanmendao miji","miji","book" }));
        set_weight(200);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "本");
                set("long","这是一本由五虎断门刀的创始人手书的秘籍，书页泛黄，看来年代已经久远。\n");
                set("value", 500000);
                set("no_get","这是那人的宝贝，你如何拿的走？\n");
                set("no_drop","这么珍贵的东西，你敢乱丢？\n");
                set("treasure",1);
                set("unique",1);
                set("material", "paper");
        }
}

void init()
{
        add_action("do_du", "read");
        add_action("do_po", ({"po","pour"}));
}

int do_du(string arg)
{
        object me = this_player();
        int neili, lev, lim;
        
        if(!id(arg)) return 0;

        if(me->query("gender") =="无性")
                return notify_fail("你乃一个宦官，又如何练的了这绝学？\n");
        if(me->is_fighting())
                return notify_fail("你无法在战斗中专心下来研读新知！\n");
        if(me->is_busy()) 
                return notify_fail("你现在正忙着呢。\n");
        if(query_temp("mao18/book") != me->query("id"))
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");
        if(!me->query("quest/whdmd/pass"))
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");
        if(this_object()->query("clone_by_wizard") && !wizardp(me)) 
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");
        if(!me->query_skill("literate", 1)) 
                return notify_fail("你是个文盲，先学点文化(literate)吧。\n");        
        if(me->query("jing") < 20)
                return notify_fail("你现在过于疲倦，无法专心下来研读新知。\n");
        lev = me->query_skill("wuhu-duanmendao", 1);
        lim = me->query_skill("blade", 1);
        if (lev <150)
                neili = 20;
        else
                neili = 20*(lev/100);
        if (me->query("neili") < neili) 
                return notify_fail("你内力不够，无法钻研这么高深的武功。\n");
        if ( lev >= (me->query_skill("blade", 1) + 10))
                return notify_fail("由于你的基本刀法火侯不够，无法再获得进一步的提高。\n");

        if (!lev)
                write(HIG"\n你看了看秘籍，封皮上赫然写道："HIR"\n\n\t\t\t五虎断门刀\n\t\t\t\t\t无名氏手书\n\n"HIG"你翻开秘籍，发现泛黄的书页上记载着密密麻麻的蝇头小字，每页都有不少练功插图。\n"NOR);
        else if (lev > 149 && !me->query("mao18/pass2")) {
                        write(HIR"\n你感觉自己对于五虎断门刀还有部分未能完全参透，看来是要找个师傅指导一下了。\n"NOR);
                        return 1;
        }
        else if (lev > 199 && !me->query("mao18/pass3")) {
                        write(HIR"\n你将整本秘籍研读至此，前面所有招式精髓都已掌握通透，只差最后一式无法研透。\n而书中最后一页豁然写道："HIY"\n\n\t\t此招既出，万夫莫敌。\n\t\t\t\t留待有缘人。\n"NOR);
                        return 1;
        }
        else
                write("你正在研读「五虎断门刀」秘籍，似乎有些心得。\n");
        me->receive_damage("jing", 20);
        me->receive_damage("neili", neili);
        me->improve_skill("wuhu-duanmendao", me->query_skill("literate", 1)/2 + me->query_skill("blade", 1)/2);

        if (!random(5))
                message("vision", me->name() + "拿着一本秘籍正在苦苦钻研。\n", environment(me), ({me}));
        return 1;
}

int do_po(string arg)
{
        object me = this_player();
        object ob;


        if (!arg)
                return notify_fail("你要泼什么？\n");
        if (sscanf(arg,"from %s",arg) != 1)
                return notify_fail("你要从什么里泼？\n");
        if (!(ob = present(arg,me)))
                return notify_fail("你身上没有这样东西。\n");
        if (me->is_busy() || me->is_fighting()) return notify_fail("你正忙着呢！\n");
        if ((int)me->query_skill("wuhu-duanmendao",1) != 200)
                return notify_fail("你没事泼水玩啥？\n");
        if (me->query("mao18/pass3"))
                return notify_fail("你干吗破坏这么好的一本秘籍？\n");
        if (!ob->query("liquid"))
                return notify_fail("这不是容器。\n");
        if (ob->query("liquid/type") != "water")
                return notify_fail("这不是清水，泼到书上岂不难看？\n");
        if (ob->query("liquid/remaining") < 1)
                return notify_fail(ob->name() +"里已经没水了。\n");
        message_vision("$N将"+ob->name()+NOR"向着手中的秘籍直泼过去，书立刻湿透了。\n", me);
        ob->set("liquid/remaining",0);
        if (random(30) >= 15) {
                tell_object(me, HIY"你突然发现书页的最后面渐渐显现出几排蝇头小字来。\n你仔细端详，发现原来这就是五虎断门刀最后一式的详细练功方法，顿时欣喜若狂。\n"NOR);
                me->set("mao18/pass3",1);
        }
        else
                write("你并没发现手中的秘籍有什么变化。\n");
        return 1;
}
