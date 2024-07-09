// vote chblk
#include <vote.h>
#include <ansi.h>

inherit F_CONDITION;

int clear_vote(object me);

int vote(object me, object victim)
{
  	int reason; // vote for what?
  	int vv;       // valid voter numbers
  	int vc;       // vote count;
  	int df;
  	string *juror, my_id, v_name;

	if (me->query("vote/abuse") > 50) {
		me->set("vote/deprived", 1);
		me->apply_condition("vote_suspension", 240);
		me->delete("vote/abuse");
		return notify_fail(HIR"你因为胡乱投票，投票权被剥夺了！\n" NOR);
	}

  	if (me == victim){
		if (random(2)) me->add("vote/abuse", 10);
		return notify_fail("你不是开玩笑吧？当心被剥夺表决权！\n");
  	}

  	if ((reason = (int)victim->query("vote/reason"))>0 && (reason!=V_CHBLK))
		return notify_fail("要把当前的表决完成以后才可以提新的动议。\n");

  	if( victim->query("chblk_on") )
  		return notify_fail(victim->name(1)+"的频道已经是关闭的了。\n");

  	if (reason <= 0)
		victim->set("vote/reason", (int)V_CHBLK);

  	my_id = me->query("id");

  	// dont allow me to vote twice for the same issue
  	juror = victim->query("vote/juror");

  	if( !pointerp(juror) )
  		victim->set("vote/juror", ({ my_id }) );
  	else if( member_array(my_id, juror) == -1 )
  		victim->set("vote/juror", juror+({ my_id }) );
  	else {
		me->add("vote/abuse", 10);
		return notify_fail("一人一票！滥用表决权是要受惩罚的！\n");
  	}

  	vv = (int) ("/cmds/std/vote")->valid_voters(me)/10;
  	vc = victim->add("vote/count", 1);

  	df = vv - vc;
  	if (vv < 4) df = 4 - vc;
	v_name = victim->name(1)+"("+capitalize(victim->query("id"))+")";
  	if (df>0){
		shout( HIW "【表决】"+me->name(1)+"投票关闭"+v_name+
			"的交谈频道，还差"+chinese_number(df)+"票。\n" NOR);
		write( HIW "【表决】"+me->name(1)+"投票关闭" +v_name
			+"的交谈频道，还差"+chinese_number(df)+"票。\n" NOR);
		victim->query("vote/deprived")? me->add("vote/abuse", 5) : me->add("vote/abuse", 10);	// add by Lklv 防止胡乱投票
		victim->apply_condition("vote_clear", 10);
  	}
  	else {
		shout( HIW "【表决】"+me->name(1)+"投票关闭" +v_name+
			"的交谈频道。"+v_name+"的交谈频道被关闭了！\n" NOR);
		write( HIW "【表决】"+me->name(1)+"投票关闭" +v_name
			+"的交谈频道。"+v_name+"的交谈频道被关闭了！\n" NOR);

		victim->apply_condition("vote_clear", -10);
		victim->apply_condition("vote_chblk", 50); // 十分钟多一点
		victim->set("chblk_on", 1);
		victim->delete("chblk_rumor");
		victim->delete("chblk_chat");
		victim->delete("vote/reason");
		victim->delete("vote/juror");
		victim->delete("vote/count");
  	}

  	return 1;
}
