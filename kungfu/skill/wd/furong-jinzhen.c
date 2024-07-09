// furong-jinzhen 芙蓉金针
// By lius 99/07

inherit SKILL;
#include <ansi.h>

string  *msg = ({
"只见$N衣袖一挥，一式「无孔不入」，黄光一闪而没，$w直向$n的空挡中射了进去。",
"$N突然状似痴狂，衣袍乱舞，挥舞间又时不时发出一枚$w，令$n防不胜防，竟是一招[无法无天]。",
"$N大喝一声，右手一扬，小小一枚$w竟带着呼啸激射而出，左手却无声无息对着$n的$l弹出一$w，正是一招[前呼后拥]。",
"$n突见$N双手交叉挥出,手型怪异,两枚$w倏左倏右倏前倏后的直奔要害而来,却是一招[两小无猜]。",
"$N微一转身，双手连挥，使出一招[满天星雨]，$w象雨点般撒向$n面门。",
"$N飞身而起，一式「芙蓉花开」，数枚$w成品字型飞出，宛若芙蓉花开,漫天花絮射向$n周身各大要穴。"});

int valid_enable(string usage)
{
        return usage == "throwing" || usage == "parry";
}

mapping query_action(object me, object weapon)
{
      int i, j, p;      
      i = random(me->query_skill("throwing"))+50;
      if(i<120) i = 120;
      j = random(me->query_skill("furong-jinzhen"))+40;
      if(j<120) j = 120;
      p = ((int)me->query_skill("force",1))/3 + me->query_con();
      if(p>100) p=100;
      if(me->query_temp("zhen/powerup") >= 1){
        me->add_temp("zhen/powerup", -1);               
        return ([
                "action":WHT"$N突然身型一拔,向后跃出丈许，去势尤疾，人在空中又连续踢出数腿，随即整个身体
就象陀螺般盘旋飙升，待又升的丈许，突然从$N身上发出无数金光，如降世活佛，
光芒万丈。无数金针带着呖啸，把$n笼罩在一片金色海洋之中。\n"NOR,
                "damage":(i+j)/2+random((int)me->query_con())+60,
                "damage_type": "刺伤",
                "dodge": random(10)+40,
                "force": random(120)+(i+j)/2+p,
		   "post_action":
                  (: call_other, WEAPON_D, "throw_weapon" :)
                ]);
}

	if(random(5) > 3 && me->query_skill("furong-jinzhen", 1) > 200 && random(me->query_dex()) > 26 ) 
              me->add_temp("zhen/powerup", 1);

      return ([
                "action":msg[random(sizeof(msg))],
                "damage":(random(3)+1)*60,
                "damage_type":random(2)?"刺伤":"割伤",
                "dodge":random(30),
                "force":random(150)+100,
                "post_action":
                  (: call_other, WEAPON_D, "throw_weapon" :)
                ]);

      
}

int practice_skill(object me)
{
       object weapon;
       if (!objectp(weapon = me->query_temp("weapon"))
       || (string)weapon->query("skill_type") != "throwing")
               return notify_fail("你使用的武器不对。\n");
       if ((int)me->query("jingli") < 50)
               return notify_fail("你的体力不够练习芙蓉金针。\n");
       me->receive_damage("jingli", 30);
       me->add("neili", -10);
       return 1;
}

int valid_learn(object me)
{
	mapping fam  = me->query("family");
	if (fam["family_name"] != "武当派" || fam["generation"] != 16 )
          return notify_fail("此类功夫若非投缘，恐怕是不会轻易传授的吧。\n");
	if ((int)me->query("max_neili") < 500)
          return notify_fail("你的内力不够。\n");
       if ((int)me->query_skill("yinyun-ziqi", 1) < 80)
          return notify_fail("你的氤氲紫气修为太浅，无法参习此技。\n");
          return 1;
}
string perform_action_file(string action)
{
       return __DIR__"wd/furong-jinzhen/" + action;
}

