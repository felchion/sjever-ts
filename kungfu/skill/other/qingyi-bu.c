// qingyi-bu.c 娥嵋青衣步
// Created by snowman@SJ 06/03/2001

inherit SKILL;
 
string *dodge_msg = ({
  	"只见$n双足一点，身子飞起，眼见无法避开的攻击，竟然给$n闪了开去。\n",
  	"$n身形一扭，竟从$N招式之间溜了过去，这便是当年瑛姑的「泥鳅功」。\n",
  	"$n向一旁飘然纵出，轻轻着地。\n"
  	"但是$n身形飘忽，轻轻一纵，早已避开。\n",
  	"$n身随意转，倏地往一旁挪开了三尺，避过了这一招。\n",
  	"$N但见眼前灰影一闪，$n以快捷无伦的身法已经飘了开去。\n",
  	"却见$n足不点地，往旁跃开数尺，躲了开去。\n",
  	"$n身形微晃，有惊无险地避开了$N这一招。\n"
});

int valid_enable(string usage) 
{ 
    	return usage == "dodge"; 
}

int valid_learn(object me) { return 1; }

string query_dodge_msg(string limb)
{
        return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
        if( (int)me->query("jingli") < 30 )
            	return notify_fail("你的体力太差了，不能练娥嵋青衣步。\n");
        me->receive_damage("jingli", 20);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"qingyi-bu/" + action;
}

