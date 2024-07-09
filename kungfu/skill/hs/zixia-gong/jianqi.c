// jianqi.c 紫霞剑气, 附带化学辅助效果,by server 2007/7/26

#include <ansi.h>

inherit F_CLEAN_UP;

string exert_name() {return HIM"紫霞剑气"NOR;}

void remove_effect(object me,object weapon);

int exert(object me, object target)
{
    object weapon = me->query_temp("weapon");
    string s_name,t_name,s_long,t_long;

        if (!me->is_fighting())
                return notify_fail("你只能在战斗中使用用紫霞剑气。\n");

        if(!weapon || weapon->query("skill_type") != "sword")
                return notify_fail("你手中无剑，如何使用紫霞剑气?\n");

  //      if(userp(me) && me->query("quest/huashan")!="气宗")
  //              return notify_fail("紫霞神功乃华山气宗绝学，你非华山气宗弟子，哪里偷学来的紫霞神功？\n");

        if((int)me->query_skill("zixia-gong",1)<150)
                return notify_fail("你的紫霞神功火候还尚浅，无法使用「紫霞剑气」。\n");

        if((int)me->query_skill("force",1)<150)
                return notify_fail("你的基本内功火候还尚浅，无法使用「紫霞剑气」。\n");

        if( (int)me->query("neili") < 1200  )
                return notify_fail("你的内力不够。\n");

        if( weapon->query("zxg/jianqi") ) 
                return notify_fail("这把武器已经无法再承受紫霞剑气的威力了。\n");

        weapon->set("zxg/jianqi", 1);
        message_vision(HIM"$N暗运一口真气,脸上紫气大盛,竟将紫霞神功内劲附于手中"+weapon->query("name")+HIM"上,刹那间,"+
            weapon->query("name")+HIM+"顿时被一股剑气所笼罩\n" NOR, me);

        s_name = weapon->query("name");
        s_long = weapon->query("long");
        t_name = strip(s_name);
        t_long = strip(s_long);

        weapon->set("name",MAG+t_name+NOR);
        weapon->set("long",MAG+t_long+NOR);
        weapon->set("s_long",s_long);
        weapon->set("s_name",s_name);
/*        weapon->set("s_rigidity",(int)weapon->query("rigidity"));
        weapon->set("s_sharpness",(int)weapon->query("sharpness"));
        weapon->add("rigidity",10);
        weapon->add("sharpness",10);
        if((int)me->query("rigidity")>12) weapon->set("rigidity",12);
        if((int)me->query("sharpness")>12) weapon->set("sharpness",12);
*/
        call_out("remove_effect", 1, me, weapon);
        call_out("zixia_jianqi",1,me,target,weapon);
        me->start_exert(2+random(2), "紫霞剑气");

        return 1;
}

void remove_effect(object me,object weapon)
{
       if(!objectp(weapon)) return;

       if (!weapon->query("zxg/jianqi")) return;

       if (weapon->query("broken")) return;

       if (!me){
           weapon->set("name",weapon->query("s_name"));
           weapon->set("long",weapon->query("s_long"));
//           weapon->set("rigidity",(int)weapon->query("s_rigidity"));
//           weapon->set("sharpness",(int)weapon->query("s_sharpness"));
           weapon->delete("zxg/jianqi");
           return;
       }

       if (me->query_temp("weapon") != weapon
         ||!me->is_fighting()
         ||!living(me)
         ||me->query_skill_mapped("force") != "zixia-gong"
         ||me->query("neili")<200 ){
             message_vision(weapon->query("name")+HIM+"周身剑气渐渐退去,剑身又恢复了平常!\n"NOR,me);
             weapon->set("name",weapon->query("s_name"));
             weapon->set("long",weapon->query("s_long"));
//           weapon->set("rigidity",(int)weapon->query("s_rigidity"));
//           weapon->set("sharpness",(int)weapon->query("s_sharpness"));
             weapon->delete("zxg/jianqi");
             return;
         }

         call_out("remove_effect", 1, me, weapon);
}

void zixia_jianqi(object me,object target,object weapon)
{
    int ap, dp;
    if(!weapon) return;
    if(!me) return;
    if(!target) return;
    if(!weapon->query("zxg/jianqi")) return;
    ap = me->query("neili");
    dp = target->query("neili");

    if( random(ap + dp) > dp*6/5  && random((int) me->query_skill("zixia-gong",1)) > 150 ) {
                message_vision(HIM"$N突然微微一凝神,脸上紫气大盛,一股剑气从"+weapon->query("name")+HIM+"上激射而出,正中$n要穴,$n只觉自己内息再也无法凝聚!\n"NOR,me,target);
                if(!target->query_condition("no_exert")) target->add_condition("no_exert",1+random(2));
                if(!target->is_busy() ) target->add_busy(1);
                me->add("neili", -100);
                call_out("zixia_jianqi",2,me,target,weapon);
    }
    else{
         message_vision(MAG"$N脸上紫气若隐若现,手中"+weapon->query("name")+MAG"亦竟闪出阵阵紫光!\n"NOR,me,target);
         call_out("zixia_jianqi",2,me,target,weapon);
    }
}
int help(object me)
{
    write(YEL"\n紫霞神功「"HIM"紫霞剑气"YEL"」："NOR"\n");
write(@HELP
   "华山九功，第一紫霞"，紫霞神功乃华山气宗最高绝学，这招紫霞
   剑气诀窍在于将浑厚的紫霞内劲附于剑身，逼出道道剑气伤敌，厉
   害非常！
   指令：exert jianqi

   要求：紫霞神功 150 级，
         基本内功 150 级，
         当前内力 1200 以上，
         使用剑类武器。

HELP
   );
   return 1;
}
