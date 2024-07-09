// study.c
// LinuX@SJ 2004/02/28

#include <ansi.h>
#include <job_mul.h>
#include <skill.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
        object where = environment(me);
        object ob;
        string zhao_name, str;
        mapping skill;
        int my_skill, cost, num;
        int improve;

        if (where->query("pigging"))
                return notify_fail("你还是专心拱猪吧！\n");
        if (where->query("no_fight"))
                return notify_fail("这里不能读书！\n");
        if (me->is_busy())
                return notify_fail("你现在正忙着呢。\n");

        if (where->query("sleep_room"))
                return notify_fail("卧室不能读书，会影响别人休息。\n");

        if( me->is_fighting() )
                return notify_fail("你无法在战斗中专心下来研读新知！\n");

        if(!arg)
                return notify_fail("你要读什么？\n");

        sscanf(arg, "%s %d", arg, num);
        
        if(!objectp(ob = present(arg, me)))
                return notify_fail("你要读什么？\n");
                
        if( !mapp(skill = ob->query("skill")) )
                return notify_fail("你无法从这样东西学到任何东西。\n");

        if( !me->query_skill("literate", 1) )
                return notify_fail("你是个文盲，先学点文化(literate)吧。\n");

        if (!random(5))
                message("vision", me->name() + "正专心地研读" + ob->name()
                + "。\n", environment(me), me);

        if( (int)me->query("combat_exp") < skill["exp_required"])
                return notify_fail("你的实战经验不足，再怎么读也没用。\n");
               
        if( me->query_int() < skill["difficulty"])
              return notify_fail("以你目前的领悟能力，还没有办法学这个技能。\n");

        notify_fail("以你目前的能力，还没有办法学这个技能。\n");
        if( !SKILL_D(skill["name"])->valid_learn(me) ) return 0;

        cost = skill["jing_cost"] + skill["jing_cost"] 
                * (skill["difficulty"] - (int)me->query("int"))/10;
        if (cost < 5) cost = 5; // minimum cost
				if (num > me->query("jing")/cost) num = me->query("jing")/cost;
       
        if( num ){
        	if( num < 1) 
        	         return notify_fail("你要读几次啊？\n");
        	else cost *= num;
        }
        
        if( me->query_skill(skill["name"], 1) < skill["min_skill"] )
                return notify_fail("你研读了一会儿，但是发现上面所说的对你而言都太深了，没有学到任何东西。\n");
        if( me->query_skill(skill["name"], 1) > skill["max_skill"] )
                return notify_fail("你研读了一会儿，但是发现上面所说的对你而言都太浅了，没有学到任何东西。\n");

        if( !me->query_skill(skill["name"], 1) ) me->set_skill(skill["name"], 0);

        my_skill = me->query_skill(skill["name"], 1);

        if( (int)me->query("jing") < cost ) {
        	if( num )
                        return notify_fail("你现在的精力不够研读那么多次。\n");
                else return notify_fail("你现在过于疲倦，无法专心下来研读新知。\n");
        }
        
        if( (string)SKILL_D(skill["name"])->type()=="martial"
           && my_skill * my_skill * my_skill / 10 > (int)me->query("combat_exp") ) 
                write("也许是缺乏实战经验，你对"+ob->name()+"上面所说的东西总是无法领会。\n");
        else {
        	if( num ) str = "你反复研读着有关";
        	else str = "你研读着有关";
          if(zhao_name = SKILL_D(skill["name"])->query_skill_name(my_skill))
              write(str+to_chinese(skill["name"])+"的技巧，对「"+zhao_name+"」这一招似乎有些心得。\n");
          else 
          	write(str+to_chinese(skill["name"])+"的技巧，似乎有点心得。\n");
                
      if (me->query("registered") > 2 && me->query_temp("ggs/started") && me->query("ggs/started") ){
			if (!random(5))
			write("你参照自己在"HIG"鬼谷算术"NOR"上的修为，运神通数，觉得自己阅读起来事半功倍。\n");
			if (!random(10))
			tell_room(environment(me), me->name() + "正在参照"HIG"鬼谷算术"NOR"上的修为，运神通数，似乎在阅读什么东西。\n", ({ me }));
		}
		
    improve = to_int((me->query_skill("literate", 1)/5+1) * (num?num:1) * me->query("int") / 15.0);
		/*奖励控制开始*/
		if (me->query("registered")>2) improve = improve * STUDY_MUL * VIP_STUDY_MUL /10000;
		else improve = improve * STUDY_MUL /100;
		/*奖励控制结束*/               
    if (me->query("registered") >2 && me->query_temp("ggs/started") && me->query("ggs/started") )
    		me->improve_skill(skill["name"], improve * 2);
    else
    		me->improve_skill(skill["name"], improve);
    if( num ) me->start_busy(1);
	}
	
	if (cost < 1) cost = 1;
  me->receive_damage("jing", cost );

  return 1;
}

int help(object me)
{
   write( @HELP
指令格式: study|du <物品名称> [次数]

这个指令使你可以通过阅读秘笈或其他物品自学某些技巧,
但前提是: 你不能是个『文盲』。

see also : learn
HELP
   );
   return 1;
}
