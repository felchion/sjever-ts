//dulong-dafa.c

inherit FORCE;

int get_bouns(int damage,int t,int level,int flag);
int valid_enable(string usage) { return usage == "force"; }

#include "/kungfu/skill/force.h"
#include <ansi.h>
/*

*/
int force_effect(string skill)
{	
	//对抗效果是从-3 -> 3，分别是受伤最重到受伤很轻，唯一一个例外，是jiuyin-zhengong，吸收
	//当然，本身有吸收效果
	//不同内功性质的内功有相生相克的效果，dulong-dafa属于阴寒内功，
	//阳性的内功对抗这个的效果就好，但是阴性内功就无法对抗，还要附加伤害
	//没有列出来的内功，算是中性内功，无法区分了。:(
        switch (skill)
        {
                case "jiuyang-shengong" : return 4;
                case "yijin-jing"       : return 3;
                case "huntian-qigong"   : return 3;
                case "qiantian-yiyang"  : return 3;
                case "yinyun-ziqi"      : return 2;
                case "zixia-gong"       : return 2;
                case "shenghuo-shengong": return 2;
                case "longxiang-boruo"  : return 2;
                case "kurong-changong"  : return 1;
                case "linji-zhuang"     : return 1;
                  case "shenzhao-jing"     : return -1;
                case "shenyuan-gong"    : return -1;
                case "guiyuan-tunafa"   : return -2;
                case "dulong-dafa"      : return -2;
                case "huagong-dafa"     : return -2;
                case "hamagong"         : return -2;
                case "yunu-xinjing"     : return -3;
                case "hanbing-zhenqi"   : return -3;
                case "bihai-chaosheng"  : return -3;
                case "jiuyin-zhengong"  : return -4;
                default: return 1;
        }
}

int valid_learn(object me)
{
        int nlskill=me->query_skill("shenlong-yaoli",1);

        if(nlskill<(me->query_skill("dulong-dafa",1)-100)
         && me->query_skill("dulong-dafa",1 ) < 301)
           return notify_fail("你对神龙药理学的领悟还不够。\n");

        if ((int)me->query_skill("dulong-dafa", 1) > me->query_skill("force", 1) + 10
          && me->query_skill("dulong-dafa", 1) >= 200 )
               return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");
        return valid_public(me);
}

int practice_skill(object me)
{
       int lvl = me->query_skill("dulong-dafa",1);
       if(lvl >= 200 )
       {
           if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
           {
                   me->improve_skill("dulong-dafa", (int)me->query_int());
                   me->add("neili", -100); 
                   me->add("potential", -1*(1+random(3)));
                   return 1;
           }
           else return notify_fail("你现在的修为不足以提高毒龙大法了。\n");   
       }
          return notify_fail("毒龙大法只能用学(learn)方法的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	string * weapon;
	object me=this_player();
	int hua;
    
	hua=strsrch(func,"hua");
	weapon=explode(func,"hua");
	if(hua!=-1 && sizeof(weapon)>0)
		me->set_temp("wea_type",weapon[0]);
	else if(me->query_temp("wea_type",1))
		me->delete_temp("wea_type");
	if(hua!=-1)
		return SKILL_D("sld/dulong-dafa/hua");
	else
		return SKILL_D("sld/dulong-dafa/") + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : BLU + me->name()+"正坐在地上盘膝打坐，周身被一团黑气包裹着。" NOR,
		"start_my_msg" : "你盘膝坐下，双手合十置于头顶，潜运内力，一团黑气渐渐将你包围了起来，双眼冒出一丝绿光。\n",
		"start_other_msg" : me->name()+"盘膝而坐，周身被一团黑气包裹着，眼中绿光四射，不可逼视。\n",
		"halt_msg" : "$N双眼缓缓闭合，片刻猛地睁开，两道绿光急射而出。\n",
		"end_my_msg" : "你分开双手，黑气慢慢沉下，眼中的绿光也渐渐暗淡下来。\n",
		"end_other_msg" : me->name()+"分开双手，黑气慢慢沉下，眼中的绿光也渐渐暗淡下来。\n"
	]);
}

string* msgs =
({
	RED"$N发疯般的叫道：“洪教主神通护佑，众弟子勇气百倍，以一当百，以百当万。”\n",
	HIR"$N大声叫道：“洪教主神目如电，烛照四方，我教弟子护教而死，同升天堂！”\n",
	HIC"$N大声叫道：“我弟子杀敌护教，洪教主亲加提拔，升任圣职。”\n",	
	RED"$N发疯般的叫道：“洪教主神通广大，我教战无不胜，攻无不克，无坚不摧，无敌不破，敌人望风披靡，逃之夭夭！”\n",
});

int ob_hit(object ob, object me, int damage)
{
	object she;
	
	if(!environment(me)) return 0;
	if(me->query_temp("sld/wudi") && me->query_skill("dulong-dafa",1)>250 )
	{
		int t = me->query_temp("combat_time");
		message("wizard",sprintf("测试信息：攻击回合%d\n",t),environment(me));
		if( random(t)>10 || (t>20 && !random(4) ) )
		{
			message_vision(msgs[random(sizeof(msgs))]+"$n一惊手上的招数就缓了下来。\n"NOR,me,ob);
			return -get_bouns(damage,t,me->query_skill("dulong-dafa",1),0);
		}
	}
	if( !(int)me->query_temp("sld/huti") || !objectp(she=me->query_temp("sld/snake")) ) return 0;
	if( (me->query("qi")-damage) >= 100 ) return 0;
	
	message_vision(HIY"$N"HIY"眼见就要毙命，从$N"HIY"的身上却突然飞出一条金蛇，抵挡了这致命的一击！\n" NOR, me);
	she->die();
	me->delete_temp("sld/huti");
	return -damage;
}

int hit_ob(object me,object victim,int damage)
{
	if(!me) return 0;
	if(!environment(me)) return 0;
	if(me->query_temp("sld/wudi") && me->query_skill("dulong-dafa",1)>250 )
	{
		int t = me->query_temp("combat_time");
		message("wizard",sprintf("测试信息：攻击回合%d\n",t),environment(me));
		if( random(t)>10 || (t>20 && !random(4) ) )
		{
			message_vision(msgs[random(sizeof(msgs))]+"$N只觉得越战越强，威力百倍。\n"NOR,me);
			return get_bouns(damage,t,me->query_skill("dulong-dafa",1),1);
		}
	}
	return 0;
}

int get_bouns(int damage,int t,int level,int flag)
{
	int dm;
	dm= (damage*100*(t+level/20))/(t+level/20+10);
	if(flag && level>350) dm += dm/2;
	return dm/100;
}
