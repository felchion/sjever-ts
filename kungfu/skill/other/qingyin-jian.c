// 娥女清音剑法
// Created by snowman@SJ 28/02/2001
// By Spiderii 修改效果！
#include <ansi.h>
#include <combat.h>
inherit SKILL;

mapping *action = ({
([      "action" : "$N一式「轻罗小扇」，手齐鼻尖，轻轻一颤，剑尖嗡嗡连响，自右至左、又自左至右的连晃九下，快得异乎寻常，但每一晃却又都清清楚楚",
        "force" : 100,
        "dodge" : 20,
        "parry" : 5,
        "damage" : 25,
        "lvl" : 0,
        "skill_name" : "轻罗小扇",
        "damage_type" : "刺伤"
]),
([      "action" : "$N身随步行，一式巧妙的「黑沼灵狐」，两三下便转到了$n的身前，在$p大惊之下，一点剑光已经点出",
        "force" : 130,
        "dodge" : 15,
        "parry" : 15,
        "damage" : 30,
        "lvl" : 20,
        "skill_name" : "黑沼灵狐",
        "damage_type" : "刺伤"
]),
([      "action" : "$N使出「万紫千红」，嗤的一声，$w当胸直刺过去，剑尖不住颤动，使$n瞧不定剑尖到底攻向何处",
        "force" : 180,
        "dodge" : 15,
        "parry" : 15,
        "damage" : 40,
        "lvl" : 40,
        "skill_name" : "万紫千红",
        "damage_type" : "刺伤"
]),
([      "action" : "$N将$w回转，自下而上倒刺，却是全真派剑法中一「天绅倒悬」，此乃娥嵋祖师所改进的剑招，威力已大超以前",
        "force" : 210,
        "dodge" : 10,
        "parry" : 15,
        "damage" : 50,
        "lvl" : 60,
        "skill_name" : "天绅倒悬",
        "damage_type" : "刺伤"
]),
([      "action" : "$N回身拧腰，$w随身而走，从上至下一式「飞瀑流泉」，霞光洒出，宛如瀑水直落，划向$n的要穴",
        "force" : 240,
        "parry" : 15,
        "dodge" : 10,
        "damage" : 60,
        "lvl" : 80,
        "skill_name" : "飞瀑流泉",
        "damage_type" : "刺伤"
]),
([      "action" : "$N将$w幌了几幌，使的竟是“打狗棒法”中的一招「恶犬拦路」，乃属“封”字诀，但见白光闪动，剑锋来势神妙无方",
        "force" : 280,
        "dodge" : 10,
        "parry" : 20,
        "damage" : 75,
        "lvl" : 100,
        "skill_name" : "恶犬拦路",
        "damage_type" : "刺伤"
]),
([      "action" : "$N$w轻扬，飘身而进，姿态飘飘若仙，剑锋向$n的下盘连点数点，却是娥嵋祖师从古墓小龙女处学来的一招玉女剑法「小园艺菊」",
        "force" : 310,
        "dodge" : 5,
        "parry" : 15,
        "damage" : 90,
        "lvl" : 120,
        "skill_name" : "小园艺菊",
        "damage_type" : "刺伤"
]),
([      "action" : "$N长剑招陡变，东趋西走，连削四剑八式！这招叫作「四通八达」，乃是当年神雕大侠杨过所创，由娥嵋祖师改为了剑法",
        "force" : 340,
        "dodge" : 10,
        "parry" : 15,
        "damage" : 120,
        "lvl" : 150,
        "skill_name" : "四通八达",
        "damage_type" : "刺伤"
]),
([      "action" : "$N倒转剑柄，以剑作为手指，使一招「一阳指」，迳点$n手腕上“腕骨”、“阳谷”、“养老”三穴！这一指点三穴的手法，正是一阳指功夫的精要所在",
        "force" : 360,
        "dodge" : 10,
        "parry" : 5,
        "damage" : 130,
        "lvl" : 150,
        "skill_name" : "一指点三穴",
        "damage_type" : "内伤"
]),
([      "action" : "$N$w斜走，使一招「顺水推舟」，斜着切向$n的手臂空隙，这招精密细腻，实乃要将$p逼入绝路",
        "force" : 300,
        "dodge" : 15,
        "parry" : 15,
        "damage" : 100,
        "lvl" : 150,
        "skill_name" : "顺水推舟",
        "damage_type" : "刺伤"
]),
([      "action" : "$N看也不看$n的招式，但见$w寒芒吞吐，电闪星飞，一招「铁锁横江」推送而上，此招凌厉之极，青霜到处已是避无可避",
        "force" : 400,
        "dodge" : 10,
        "parry" : 15,
        "damage" : 160,
        "lvl" : 150,
        "skill_name" : "铁锁横江",
        "damage_type" : "刺伤"
]),
([      "action" : "$N当下一招「月落西山」，直刺$n小腹，此招简单而精细，已达返璞归真！娥嵋派剑术高明之处，果然名不虚传",
        "force" : 390,
        "dodge" : 20,
        "parry" : 15,
        "damage" : 180,
        "lvl" : 150,
        "skill_name" : "月落西山",
        "damage_type" : "刺伤"
]),
([      "action" : "$N身形一晃，抢上数丈，反手刷刷刷三剑，一剑比一剑快，一剑比一剑狠，乃是娥嵋派嫡传的「金顶九式」",
        "force" : 400,
        "dodge" : 10,
        "parry" : 25,
        "damage" : 210,
        "lvl" : 150,
        "skill_name" : "金顶九式",
        "damage_type" : "刺伤"
]),
});


int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }
int valid_learn(object me)
{
        if ((int)me->query("max_neili") < 100)
                return notify_fail("你的内力不够。\n");
                
        if ((int)me->query_skill("sancheng-gong", 1) < 20)
                return notify_fail("你的三乘功法火候太浅。\n");
        if ((int)me->query_skill("parry", 1) < 10)
                return notify_fail("你的基本招架火候太浅。\n");
        if ( me->query("class") != "bonze" && me->query_skill("sancheng-gong", 1) < 100 )
                return notify_fail("清音剑法不传俗家新弟子。\n");
        return 1;
}


mapping query_action(object me,object target)
{
        int i, level;
        level   = (int) me->query_skill("qingyin-jian",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}
int practice_skill(object me)
{
        object weapon;

        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");

        if ((int)me->query("jingli") < 40)
                return notify_fail("你的体力不够练清音剑法。\n");
        me->receive_damage("jingli", 30);
        if ((int)me->query("neili") < 20)
                return notify_fail("你的内力不够练清音剑法。\n");
        me->receive_damage("neili", 5);
        return 1;
}
int ob_hit(object ob, object me, int damage)
{
        object weapon, weapon1;
        string msg, *limbs, limb;
        int ap, dp, skill, j, i;

        if( ob->query("env/invisibility") ) return 0;
        if( !userp(me) && random(2) ) return 0;

        weapon1 = me->query_temp("weapon");

        if( me->query_skill("qingyin-jian", 1) < 150
         || me->query_skill_mapped("sword") != "qingyin-jian"
         || me->query_skill_mapped("parry") != "qingyin-jian"   // 招架必须是qingyin-jian
         || !weapon1
         || weapon1->query("skill_type") != "sword" ) return 0;

        if( random(me->query_skill("parry")) > ob->query_skill("force")/3
         || random(me->query_con()) > ob->query_str()/3 )
                me->add_temp("hfj/"+ob->query("id"), 1);        // 添加反击参数

//      原来后发先至的判断，去除了反复判断
        if( me->query("combat_exp") < ob->query("combat_exp")/2 || me->is_busy() || damage < 100 ) return 0;

        skill = me->query_skill("qingyin-jian", 1);
        weapon = ob->query_temp("weapon");

        if( objectp(weapon) )
                msg = weapon->query("skill_type");
        else {
                mapping prepare = ob->query_skill_prepare();
                if( !sizeof(prepare) ) msg = "unarmed";
                else msg = keys(prepare)[0];
        }

        if ( me->query("neili") < ob->query("neili")/5 ) return 0;

        ap = COMBAT_D->skill_power(ob, msg, SKILL_USAGE_ATTACK) / 1000;
        if ( weapon ) ap *= 2;
        if ( ob->query("neili") > me->query("neili") * 2 ) ap += ap/2;

        dp = COMBAT_D->skill_power(me, "parry", SKILL_USAGE_DEFENSE) / 1000;
        if ( me->query("neili") > ob->query("neili") * 2 ) dp *= 2;  
        else if ( me->query("neili") > ob->query("neili") / 2 * 3 ) dp += dp/2;

        ap = ABS(ap);

        dp = ABS(dp);

        if ( random(me->query("dex")) > ob->query("dex")/2) dp += random(dp/2);
        if ( skill > 350 ) dp += random(dp);

        if ( wizardp(me) ) tell_object(me, sprintf("ap: %d, dp: %d\n", ap ,dp ));

        if ( random(dp + ap) > ap && !random(3) ) {     // 概率降低点
                limbs = ob->query("limbs");
                limb = limbs[random(sizeof(limbs))];
                msg = HIW+"只见$N后发先至，";
                msg += query_action(me, weapon1)["action"];
                msg = replace_string( msg, "$l", limb);
                msg = replace_string( msg, "$w", weapon1->name()+HIW) + "！\n"NOR;
                if( userp(ob) && !userp(me) && damage > 1500 )
                        damage = 1500 + (damage-1500)/10;       // 修改了伤害的控制
//      伤害不能全部来自于对手
//              ob->receive_damage("qi", damage/3*2 + ob->query("jiali")*2, me);
//              ob->receive_wound("qi", damage/2, me);
                j = damage/2 + me->query_skill("sword") + me->query("jiali");
                if( wizardp(me) ) tell_object(me, "你的后发先至伤害为："+j+"。\n"NOR);
                ob->receive_damage("qi", j, me);
                ob->receive_wound("qi", j/2, me);
                msg += COMBAT_D->damage_msg(j, "刺伤");
                msg = replace_string( msg, "$l", limb);
                msg = replace_string( msg, "$w", weapon1->name());
                message_vision(msg, me, ob);
                COMBAT_D->report_status(ob, ob->query("qi") >= 0 || ob->query("eff_qi")+ob->query_temp("apply/qi") < 0);
                return -6000;
        }
        
//      峨嵋剑诀的效果 “后发制人”
        else if( me->query_temp("hfj/"+ob->query("id")) > ( 1 + random(4) ) 
         && ( random(me->query_skill("parry")) > ob->query_skill("force")/2
         || random(me->query_con()) > ob->query_str()/2 ) ) {
                me->delete_temp("hfj/"+ob->query("id"));
                msg = HIW"$N摸透了$n的武功套路，"+weapon1->name()+HIW"微侧，便即抢攻，竟不挡格$n的来招！！"NOR;
                message_vision(msg, me, ob);
                i = ob->query("qi");
                me->set_temp("jianjue", 1);
                me->add_temp("apply/attack",  me->query_skill("qingyin-jian", 1)/4);
                COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(3)?1:3);
                me->add_temp("apply/attack", -me->query_skill("qingyin-jian", 1)/4);
                me->delete_temp("jianjue");
                if( i > ob->query("qi") ) return -damage/2;
                else return -damage;
        }
        return 0;
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
//      增加峨嵋剑诀，辅助攻击，概率很低
        object weapon; 
        int i;
        weapon = me->query_temp("weapon");
        i = me->query_skill("qingyin-jian", 1);

        if( !userp(me) && random(2) ) return 0;

        if( !weapon || weapon->query("skill_type") != "sword" ) return 0;
        if( i < 300 ) return 0;
        if( !me->query("emei/jianjue") && i < 400 ) return 0;
        if( me->query_temp("jianjue") ) return 0;
        if( me->query_temp("liaoyuan") ) return 0;
        if( damage_bonus < 100 ) return 0;
        if( !living(victim) ) return 0;

        if( i >= 300 && !random(3)
         && random(me->query_con()) > victim->query_con()/3
         && random(me->query_skill("sword")) > victim->query_skill("parry")/3 ) {
                if( me->query_skill_prepared("hand") == "jieshou-jiushi"
                 && me->query_skill_mapped("hand") == "jieshou-jiushi"
                 && me->query_skill("jieshou-jiushi", 1) >= 200 && random(2) ) {
                        message_vision(HIW"$N默念峨嵋剑诀，“"CYN"先以心使身，从人不从己，由己则滞，从人则活。"HIW"”右手猛的运剑回转，左手截手九式随劲而起！"NOR, me);
                        weapon->unequip();
                        me->add_temp("apply/attack",  me->query_skill("jieshou-jiushi", 1)/4);
                        COMBAT_D->do_attack(me, victim, 0, 1);
                        me->add_temp("apply/attack", -me->query_skill("jieshou-jiushi", 1)/4);
                        weapon->wield();
                } else {
                        message_vision(HIW"$N默念峨嵋剑诀，“"HIY"从身能从心，由己仍从人。前进后退，处处恰合。"HIW"”紧接着"+weapon->name()+HIW"猛的回转再次刺出！"NOR, me);
                        me->set_temp("jianjue", 1);
                        me->add_temp("apply/attack",  me->query_skill("qingyin-jian", 1)/6);
                        COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), random(4)?1:3);
                        me->add_temp("apply/attack", -me->query_skill("qingyin-jian", 1)/6);
                        me->delete_temp("jianjue");
                }
        }
        else if( i >= 400 && random(2)
         && random(me->query("combat_exp")) > victim->query("combat_exp")/3
         && random(me->query_skill("sword")) > victim->query_skill("parry")/3
         && random(me->query_con()) > victim->query_str()/3
         && !victim->query_temp("lost_attack") ) {
                message_vision(HIW"$N默念峨嵋剑诀，“"HBRED+HIW"前后左右，全无定向，后发制人，先发制于人。"NOR+HIW"”顿时剑气纵横，将$n攻个措手不及！\n"NOR, me, victim);
                victim->add_temp("lost_attack", 1+random(3));
                return random(me->query_skill("sword"))/3;
        }
        return 0;
}


string perform_action_file(string action)
{        
        return __DIR__"qingyin-jian/" + action;
}

int help(object me)
{
        write(HIG"\n娥女清音剑法"NOR"\n");
        write(@HELP
        清音阁地处峨眉山上山下山的中枢，与龙门洞素称“水胜双绝”。面对清
        音阁展开的是一幅青绿山水画卷，浓渌重彩，精工点染。高处，玲珑精巧
        的楼阁居高临下。中部，是丹檐红楼的接御、中心二亭、亭两侧各有一石
        桥、分跨在黑白二水之上，形如双翼，故名双飞桥。近景，则为汇合于牛
        心亭下的黑白二水。此地地灵山青，当年郭襄女侠便在此建舍居住数年，
        终领悟出了傲视武林的娥嵋剑法！
        
        要求：  
                基本招架等级 10 以上；
                三乘功法等级 20 以上；
                最大内力 100 以上；
                俗家弟子只有在通过了掌门的考验后才能修习。
HELP
        );
        return 1;
}

