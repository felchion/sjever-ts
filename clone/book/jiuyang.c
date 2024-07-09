// jiuyang-zhenjing.c
// Modified by jpei
// Modify By River@SJ 99.06

inherit ITEM;
#include <ansi.h>
void create()
{
        set_name(HIW"九阳真经"NOR, ({"jiuyang zhenjing", "jiuyang", "zhenjing", "book"}));
        set_weight(600);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "册");
                set("long", "这是一套四册薄薄的经书。\n");
                set("value", 8000);
                set("unique", 1);
                set("material", "paper");
                set("no_drop", "这么珍贵的武林秘籍，你可不能随意丢弃！\n");
                set("no_get", "这样东西是那人的宝贝，连睡觉都要当枕头垫，你无法拿取！\n");
                set("treasure", 1);
        }
}

void init()
{
        add_action("do_du", "read");
}

int do_du(string arg)
{
        object me = this_player();
        object where = environment(me);
        int jylevel, neili_lost;

        if(!id(arg)) return 0;

        if(me->query("gender") =="无性")
                return notify_fail("你无根无性，阴阳不调，难以领会高深的九阳神功。\n");
                
        if(me->is_busy()) 
                return notify_fail("你现在正忙着呢。\n");
                
        if(query_temp("jiuyang") != me->query("id"))
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");
                
        if(me->is_fighting())
                return notify_fail("你无法在战斗中专心下来研读新知！\n");
                
        if(where->query("outdoors") != "昆仑翠谷" )
                return notify_fail("你只能在昆仑翠谷中才能静下心来研读九阳真经。\n");
                
        if(!me->query("jiuyang/baiyuan"))
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");
                
        if(!me->query_skill("literate", 1)) 
                return notify_fail("你是个文盲，先学点文化(literate)吧。\n");        
                
        if(me->query("jing") < 25)
                return notify_fail("你现在过于疲倦，无法专心下来研读新知。\n");
                
        if(me->query_skill("force",1) < 100) 
                return notify_fail("你的内功基础没有打好，不能修练九阳神功。\n");
                
        if(me->query_int() < 40)
                return notify_fail("由于你的悟性不够，无法研习九阳神功。\n");

        if(this_object()->query("clone_by_wizard") && !wizardp(me)) 
                return notify_fail("你从哪里偷来的？可不能偷读哦！\n");

        jylevel = me->query_skill("jiuyang-shengong", 1);
        if (jylevel < 30)
                neili_lost = jylevel / 2;
        else if (jylevel < 70)
                neili_lost = jylevel* 2 / 3;
        else if (jylevel < 140)
                neili_lost = jylevel* 3 / 4;
        else
                neili_lost = jylevel;
        if( neili_lost > 150) neili_lost = 150;
        if ( jylevel >= (me->query_skill("force", 1) + 30))
                return notify_fail("由于你的内功的火侯不够，不能再获得更高一层的提高。\n");
        if ( jylevel >= (me->query_skill("shenghuo-shengong", 1)+ 10) && jylevel < 220)
                return notify_fail("由于你的圣火神功火候不够，不能获得更高一层的提高。\n");
        if (me->query("neili") < neili_lost) 
                return notify_fail("你内力不够，无法钻研这么高深的武功。\n");
        if (me->query("combat_exp") < jylevel * jylevel * jylevel / 10) 
                return notify_fail("你的实战经验不足，再怎么读也没用。\n");
        if (jylevel >= 140 && me->query("neili") < jylevel * 3 )
                return notify_fail("这最后一册书上的内容过于深奥，光靠研读而不身体力行是无法再获得进步的。\n");
        me->receive_damage("jing", 20);
        me->receive_damage("neili", neili_lost);
        if (!jylevel) {
                write(HIW"\n你翻开第一册经书，发现上面尽是弯弯曲曲的梵文，但每一行之间却以蝇头小楷写满了中国文字。\n"NOR);
                write(HIW"你定一定神，从头细看，发现文中所记似是关于练功运气的诀窍。\n"NOR);
                write(HIW"你开始研读第一册，上面讲述的是运气法门。\n\n"NOR);
        }
        if (jylevel >= 140)
                write("你按照书中所讲打坐一阵，再对照着研读「九阳真经」最后一册，这才有点心得。\n");
        else
                write("你研读「九阳真经」，颇有心得。\n");
        me->improve_skill("jiuyang-shengong", me->query_skill("literate", 1));
        if (!random(9)) message("vision", me->name() + "拿着一本经书正在钻研。\n", environment(me), ({me}));
        if (me->query_skill("jiuyang-shengong", 1) > jylevel) {
                if (jylevel == 29) {
                        write(HIW"\n你读完了第一册，觉得这册上所讲的入门基础很是易学。\n"NOR);
                        write(HIW"你开始研读第二册，上面讲述的是练气法门。\n\n"NOR);
                }
                else if (jylevel == 69) {
                        write(HIW"\n你读完了第二册，虽然这册上所讲的练气法门有些难度，但是并没有太深奥的地方。\n"NOR);
                        write(HIW"你开始研读第三册，上面讲述的是运气诀要。\n\n"NOR);
                }
                else if (jylevel == 139) {
                        write(HIW"\n你读完了第三册，这册上所讲的运气诀要颇为艰深，你费了好大的劲才完全理解了。\n"NOR);
                        write(HIW"你开始研读第四册，上面讲述的已经是十分艰深的内功精要，读起来相当困难。\n\n"NOR);
                }
                else if (jylevel == 219) {
                        write(HIW"\n你终于读完了第四册，揭过最后一页之后，心中又是欢喜，又微微感到怅惘。\n"NOR);
                        write(HIW"屈指算来，数年的光阴匆匆而过，至今方始功行圆满。\n\n"NOR);
                }
        }
        if( !me->query("jiuyang/read")){
		me->set("jiuyang/read", 1);
		log_file("quest/jiuyang",sprintf("%8s%-10s成功得到并开始研读九阳神功，此时等级是：%s。\n",
			me->name(1),"("+capitalize(me->query("id"))+")", chinese_number(jylevel)), me);
        }
        return 1;
}
