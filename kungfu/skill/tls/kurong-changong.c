#include <ansi.h>
#include <combat.h>

inherit FORCE;

#include "../force.h"

int valid_enable(string usage) { return usage == "force" ;}

void skill_improved(object me)
{
  int skill = me->query_skill("kurong-changong", 1);
  if( skill >= 200 && !me->query("kr_add")){
    me->set("kr_add", 1);
    tell_object(me, GRN"\n忽然心中灵光一闪，不由得轻轻吟道：\n"+
                         "「一切有为法 如梦幻泡影」\n"+
                         "「如露亦如电 应做如是观」\n"+
                         "你的禅功修为到了一个新的境界！\n"NOR);
    me->add("max_neili", skill+random(skill));
  }
}

int valid_learn(object me)
{
  int i, l;
  int t = 1, j;
  mapping myfam = (mapping)me->query("family");
  for (j = 1; j < i / 10; j++) t *= 2;

  i = (int)me->query_skill("kurong-changong", 1);
  l = (int)me->query_skill("buddhism", 1);

  if (i > 120 && i <180 && l < i - 10)
    return notify_fail("枯荣禅功需要深厚的佛学造诣，你的佛学不够！\n");

  if (!me->query_skill("kurong-changong", 1))
    if (me->query_skill("qiantian-yiyang",1) < 140)
      return notify_fail("你的乾天一阳功火候还不够，无法领会枯荣禅功。\n");

  if ( me->query_skill("qiantian-yiyang", 1)
  && me->query_skill("kurong-changong", 1) > me->query_skill("qiantian-yiyang", 1))
    return notify_fail("你不散掉乾天一阳功，如何能修习枯荣禅功。\n");

  if ( me->query("gender") != "男性" )
    return notify_fail("你非童男之体，不能练习枯荣禅功内功。\n");

  if (me->query("class") != "bonze" || me->query("menggu"))
    return notify_fail("你非我佛弟子，不可修习枯荣禅功。\n");

  if (me->query_skill("yiyang-zhi",1) < 140)
    return notify_fail("枯荣禅功需要一阳指神功作为后盾。\n");

  if ((int)me->query_skill("kurong-changong", 1) > me->query_skill("force", 1) + 10
  && me->query_skill("kurong-changong", 1) >= 200 )
    return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

  if ( !mapp(myfam) || myfam["master_name"] != "枯荣长老")
    return notify_fail("你辈分不够，不可以修习枯荣禅功。\n");
  return 1;
}

int practice_skill(object me)
{
  if((int)me->query_skill("kurong-changong", 1) >= 200 ) {
    if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200) {
      me->improve_skill("kurong-changong", me->query_int());
      me->add("neili", -100);
      me->add("potential", -1*(1+random(3)));
      return 1;
    }
    else return notify_fail("你现在的修为不足以提高枯荣禅功了。\n");
  }
  else return notify_fail("枯荣禅功不可随便练习。\n");
}

string exert_function_file(string func)
{
  return __DIR__"tls/kurong-changong/" + func;
}

mapping exercise_msg(object me)
{
  return ([
    "status_msg" : HIG + me->name()+"面带凝涩之气，双目低垂内视。" NOR,
    "start_my_msg" : "你盘膝坐下，垂目合什，默运枯荣禅功，只觉冷热两股真气开始在体内缓缓游动。\n",
    "start_other_msg" : me->name()+"盘膝坐下，垂目合什，周身似乎笼罩在一层青气之中。\n",
    "halt_msg" : "$N双掌一分，屈掌握拳，两股真气迅速交汇消融。\n",
    "end_my_msg" : "你真气在体内运行了一个周天，冷热真气收于丹田，慢慢抬起了眼睛。\n",
    "end_other_msg" : "只见"+me->name()+"慢慢睁开眼睛，周身笼罩的青气逐渐隐去。\n"
  ]);
}

int ob_hit(object ob, object me, int damage)
{
  string msg;
  int ap, dp, j, neili, neili1;
  object weapon;

  if( damage < 100) return 0;

  neili = me->query("neili");
  neili1 = ob->query("neili");

  if(me->query_temp("kurong-changong")) {
    me->delete_temp("kurong-changong");
    return 0;
  }

  if( me->query_temp("fanzhen")) {
    me->delete_temp("fanzhen");
    return 0;
  }

  if (me->query_skill("parry") < 300
  || me->query_skill_mapped("parry") != "kurong-changong"
  || me->query_skill_mapped("force") != "kurong-changong"
  || me->query("combat_exp") < ob->query("combat_exp")/2
  || neili < 400 ) 
    return 0;

  weapon = ob->query_temp("weapon");

  if(objectp(weapon))
    msg = weapon->query("skill_type");
  else {
    mapping prepare = ob->query_skill_prepare();
    if (!sizeof(prepare)) msg = "unarmed";
    else msg = keys(prepare)[0];
  }

  ap = COMBAT_D->skill_power(ob, msg, SKILL_USAGE_ATTACK) / 1000;
  if ( weapon ) ap *= 2;
  if ( ob->query("neili") > me->query("neili") * 2 ) ap += ap/2;

  dp = COMBAT_D->skill_power(me, "parry", SKILL_USAGE_DEFENSE) / 1000;
  if ( me->is_busy() ) dp /= 2;
  if ( me->query("neili") > ob->query("neili") * 2 ) dp *= 2;
  else if ( me->query("neili") > ob->query("neili") / 2 * 3 ) dp += dp/2;

  if ( random(10) < 3 ) me->set_temp("fanzhen", 1);

  ap = ABS(ap);
  dp = ABS(dp);

  if ( me->query_temp("pf_ku"))
  dp += ap/2 + random(ap/2);

  if ( wizardp(me))
  tell_object(me, sprintf("ap: %d, dp: %d\n", ap, dp));

  if( random(dp) > ap && random(2)) {
    msg = GRN "$N默运枯荣禅功的「枯」字大法，半边身子的隐隐透出青气，内力全部集中在$N的这半边身上。\n" NOR;
    tell_object(ob, HIR "你只见对手突然身子一侧，将半边身子迎了上来！\n" NOR);
    msg += "$n一招打在$N的身上，只觉似乎打在一块朽木上。\n";
    if(neili >= neili1*2 + random(damage)) {
      msg += "结果$n的攻击被$N的枯荣禅功化掉了！\n",
      j = -damage;
    }
    else if( neili > neili1 + random(damage) ) {
      msg += "结果$n的攻击被$N化掉了一半！\n",
      j = -damage/3*2;
    }
    else {
      msg += "结果$N只化掉了$n的一小半力道！\n",
      j = -damage/3;
    }
    message_vision(msg, me, ob);
    return j;
  }
}
