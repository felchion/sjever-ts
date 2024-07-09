// wudi.c 
// by emnil   98.8.12

#include <ansi.h>

inherit F_CLEAN_UP;

string exert_name() {return HIR"无敌大法"NOR;}

void remove_effect(object me);
void remove_effect2(object me);
int exert(object me, object target)
{
        int skill;
        string s;

        if( target != me )
                return notify_fail("你只能用毒龙大法来提升自己的战斗力。\n");

        if((int)me->query_skill("dulong-dafa",1) < 100  )
                return notify_fail("你的毒龙大法功力不够，无法施用「无敌」大法。\n");

        if((int)me->query("max_neili") < 500 )
                return notify_fail("你的内力不够，无法使用「无敌」大法。\n");
 
        if((int)me->query("eff_qi") < me->query("max_qi")/2 )
                return notify_fail("你现在受伤过重，不能使用「无敌」大法。\n");

        if( me->query_temp("sld/shougong"))
                return notify_fail("你刚刚收功完毕，还是歇会再使用「无敌」大法吧。\n");

        if( me->query_temp("sld/wudi") && environment(me)->query("no_death"))
                return notify_fail("这里你无法使用无敌。\n");

        if((int)me->query_temp("sld/wudi2"))
                return notify_fail("你已经发挥了你的最大潜力。\n");

        if((int)me->query("eff_qi") > me->query("max_qi"))
                return notify_fail("你现在无法使用「无敌」大法。\n");

        if((int)me->query("neili") < 800  )
                return notify_fail("你的真气不够，无法使用「无敌」大法。\n");

        skill = me->query_skill("dulong-dafa", 1);
        if( me->query_temp("sld/wudi") && me->query("eff_qi") < me->query("max_qi")){

                s=RED"\n$N发疯般的叫道：“洪教主神通护佑，众弟子勇气百倍，以一当百，以百当万。\n"+
                "洪教主神目如电，烛照四方。我弟子杀敌护教，洪教主亲加提拔，升任圣职。\n"+ 
                "我教弟子护教而死，同升天堂！”，突然喷出一口鲜血，似乎已经忘记了伤痛！\n\n"NOR;

                me->delete_temp("sld/wudi");
                me->add("max_neili", -1); // 一点max neili作为代价得到高攻击力
                me->set("eff_qi",me->query("max_qi"));
                me->set("qi",me->query("eff_qi"));
                me->set("jingli",me->query("eff_jingli"));
                me->set("neili",me->query("max_neili"));
                me->add_temp("apply/attack", skill/2);
                me->set_temp("sld/wudi2", skill/2);
                me->start_call_out( (: call_other, __FILE__, "remove_effect2", me :), skill/3);
                me->start_perform(1,"「无敌」");
                me->start_exert(1,"「无敌」");
                message_vision(s, me);
                return 1;
        }
        if( !me->query_temp("sld/wudi2") && !me->query_temp("sld/wudi")){
                me->add("neili", - 500);
                s=HIR"\n$N大声叫道：“洪教主神通广大，我教战无不胜，攻无不克，无坚不摧，无敌不破，\n"+
                "敌人望风披靡，逃之夭夭！”，双眼突然冒出一丝红光，似乎突然增加了功力！\n\n"NOR;

                if(! me->is_fighting()){
                        me->set_temp("sld/wudiqi",skill*2); 
                        me->add("max_qi", skill*2);
                        me->add("eff_qi", skill*2);
                        me->add("qi", skill*2);
                }
                me->add_temp("apply/attack", skill/4);
                me->set_temp("sld/wudi", skill/4);
                me->start_call_out( (: call_other, __FILE__, "remove_effect", me:), skill/2);
                me->start_perform(1,"「无敌」");
                me->start_exert(1,"「无敌」");
                message_vision(s, me);
                return 1;
        }
        return notify_fail("你现在无法使用「无敌」大法。\n");
}

void remove_effect(object me )
{
        int i;
        
        if(!objectp(me)) return;
        if(!intp(i = me->query_temp("sld/wudi")) ) return; 
        
        me->add("max_qi",- me->query_temp("sld/wudiqi"));
        me->add_temp("apply/attack", -i);
        me->delete_temp("sld/wudiqi");
        me->delete_temp("sld/wudi");
        message_vision(HIR"\n$N好象虚脱了一样，突然垮了下来！\n\n"NOR,me);
        return;
}

void remove_effect2(object me)
{
        int amount;
        
        if(!objectp(me)) return;
        if(!intp(amount = me->query_temp("sld/wudi2")) ) return;
        
        if(me->is_ghost()){
                me->delete_temp("sld/wudi");
                me->add_temp("apply/attack", -amount/2*3);  
                me->add("max_qi",- me->query_temp("sld/wudiqi"));
                return;
        }
        message_vision(RED"\n$N心神一松，再也支持不主！\n"NOR,me);                 
        
        me->add_temp("apply/attack", - amount/2*3);
        me->add("max_qi",- me->query_temp("sld/wudiqi"));
        if(me->query("eff_qi")>me->query("max_qi")) 
                me->set("eff_qi",me->query("max_qi"));
        if(me->query("qi")>me->query("eff_qi")) 
                me->set("qi",me->query("eff_qi"));
        me->delete_temp("sld");
        me->unconcious();
}
