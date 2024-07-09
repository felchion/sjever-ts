/*
新版web版runner
Linux
*/

inherit NPC;

#include <ansi.h>
#include <mudlib.h>
#include <database.h>

#define URLMSG(x) HIY"请浏览"+HIW+U+BBS_ADDR+"/runner.php?id="+x+NOR+HIY+"把看到的数字和字母的组合用answer回答。\n"NOR

object ob;
int timer = 0;

int is_corpse(){ return 1; }
void move_to_court(object ob);
void test_robot(object obj,int reward);
void question();
string makepasswd();

void create()
{
	set_name("银翼杀手", ({ "blade runner", "runner" }));

	set("gender", "机器");
	set("age", 20);
	set("no_quest", 1);
	set("no_drop", 1);
	set("no_get", 1);
	set_temp("apply/long",
		({ "这是一个专门追缉机器人的机器探员。\n"
		"如果利用机器自动回答问题，档案将被无条件删除。\n"})
	);
	set_skill("dodge", 1000);
	set("max_qi", 10000);
	set("qi", 10000);
	set("max_jing", 10000);
	set("jing", 10000);

	set("combat_exp", 5000000);

	set("inquiry", ([
		"name": "你还有这闲功夫啊？专心回答问题吧！",
		"rumors": "听说有机器人混入本MUD，嘿嘿，我可是火眼金睛呀。",
	]) );
	set("attitude", "friendly");
	set("env/invisibility", 1);

	setup();
	set_weight(0);
}

void init()
{
	if (environment() == ob) {
		question();
		add_action("do_answer", ({ "answer" }));

		add_action("wait",
			({
			"beep", "chat", "chat*",
			"irc", "irc*","mm",
			"party", "party*", "quit",
			"reply", "rumor", "rumor*",
			"group","group*",
			"sr","sr*",
			"say", "sj",  "tell", "to", "whisper",
			})
		);
	}
}

int wait()
{
	//return 0;
	write("对不起，在银翼杀手判断期间,你不能做这个动作,请先回答问题。\n");
	return 1;
}

void kill_ob(object ob)
{
	ob->remove_killer(this_object());
	remove_killer(ob);
	message_vision("$N沮丧地发现银翼杀手根本不理会作战请求。\n", ob);
}

void unconcious()
{
	reincarnate();
	set("eff_qi", query("max_qi"));
	set("qi", query("max_qi"));
	set("eff_jing", query("max_jing"));
	set("jing", query("max_jing"));
	set("jingli", query("eff_jingli"));
	say("银翼杀手狞笑着说：我是无敌不死版！\n");
	command("hehe");
}

void die()
{
	unconcious();
}

void give_reward(object ob)
{
	mapping skills, skls = ([]), skls2 = ([]);
	int sn,sn2, lvl = 0;
	string skill;
	string* quest_skill = ({ "jiuyin-zhengong","hamagong", "kuihua-xinfa" });
	
	if (ob->query("registered") < 3) return;
	switch (ob->add_temp("robot_check_pass", 1)) {
		case 1:
			skills = ob->query_skills();
			if (!(sn = sizeof(skills))) break;
			sn2 = sn;
			foreach (skill, int val in skills) {
				if ( SKILL_D(skill)->type() == "martial"
				|| (sn2>5 && SKILL_D(skill)->type2()=="worker") ) {
					skls2 += ([ skill : val ]);
					lvl += val;
				} else sn--;
			}
			if (!sn) break;
			lvl /= sn;
			foreach (skill, int val in skls2) {
				if (val >= 0) {
					skls += ([ skill : val ]);
				}
			}
			
			sn = random(sizeof(skls));
			skill = keys(skls)[sn];			
			
			while( member_array(skill,quest_skill)!= -1 || SKILL_D(skill)->type2() == "worker" )
			{
				sn = random(sizeof(skls));
				skill = keys(skls)[sn];
			}
						
			lvl = values(skls)[sn];
			lvl *= lvl/2;
			lvl = lvl/2 + random(lvl/2);
			
			if( SKILL_D(skill)->type2() == "worker" ) {
				if( values(skls)[sn] > 300 )
					lvl = lvl / 3;
				else if( values(skls)[sn] > 200 )
					lvl = lvl / 2;
			}
			
			write("你获得了"+chinese_number(lvl)+"点「"+to_chinese(skill)+"」的奖励。\n");
			ob->improve_skill(skill, lvl, 1);
			log_file("static/ROBOT",
				sprintf("%s(%s)通过检查，奖励 %s %d。\n",
					ob->query("name"), ob->query("id"), skill, lvl
				), ob
			);
			break;
		case 2:
			lvl = 500 + random(500);
			write("你获得了"+chinese_number(lvl)+"点经验的奖励。\n");
			ob->add("combat_exp", lvl);
			log_file("static/ROBOT",
				sprintf("%s(%s)通过检查，奖励 exp %d。\n",
					ob->query("name"), ob->query("id"), lvl
				), ob
			);
		default:
			ob->delete_temp("robot_check_pass");
	}
}

void move_to_court(object ob)
{
	object me=this_object();
	ob->delete_temp("robot_check_pass");
	ob->apply_condition("robot", 15);
	
	ob->delete_temp("robot_test");	
	ob->set("iamrobot",1);
	ob->add("iamrobottime",1);
	if (ob->is_vip())
	if (me->query("reward")) ob->set_temp("robot_prize",1);
	message_vision("系统判定$n为机器人，你随时可以用"HIY"robot"NOR"命令重新召回银翼杀手把自己的robot状态取消掉！\n", this_object(), ob);	
}

void test1()
{
	object me = this_object();

	if (ob) {
		timer -= 10;
		if (timer > 0) {
			message("channel", "银翼杀手说道：你还有" + chinese_number(timer) + "秒钟的考虑时间。\n", ob);
			me->move(ob, 1);
			call_out("test1", 10);
		} else {
			if (!interactive(ob) || query_idle(ob) >= 170) {

				//"/cmds/usr/quit"->main(ob);
				move_to_court(ob);
				if (me) destruct(me);
			} else {
				// 防止player故意晕倒等bug. snowman@SJ 12/06/2000.
				//if (!living(ob)) ob->revive(1);
				message_vision(
					query("name")+"叹了口气道：$N一点反应都没有，又是一个机器人嫌犯！\n", ob
				);
				move_to_court(ob);
				destruct(me);
			}
		}
	}
	else destruct(me);
}

/*
void test_player(string player)
{
	object ob = find_player(player);

	if (ob) test_robot(ob);
}
*/
int aflag;
int num = 2;
string ans="";
string ans1;

string query_id()
{
	string id;
	
	if(!ob) return 0;
	
	id = ob->query("id");
	if(strsrch(id,"@")==-1) id += "@"+lower_case(INTERMUD_MUD_NAME);
	
	return id;
}

void question()
{
	message("channel", URLMSG(query_id())+"\n", ob);
	aflag = 1;
}

int do_answer(string arg)
{
	object me = this_object();
	int len = strlen(ans);

	if (arg) {
		int i = strlen(arg);

		while (i--)
			if (arg[i] == ' ')
				arg = arg[0..i-1] + arg[i+1..<1];
	} else
		arg = "";
	if (strlen(arg) != len)
		return notify_fail("答案的长度不对，再仔细看看。\n");
	if (aflag) {
		aflag = 0;
		if (ans1 == arg) {
			command("wiz "+ob->query("id")+" 极可能使用了自动应答机器人，请检查！");
			return notify_fail("答案不对，再仔细看看。\n");
		}
		ans1 = arg;
	}
	if (arg==ans) {
		message_vision("$N点了点头，说道：OK，$n继续忙吧。\n", me, ob);
		ob->delete("iamrobot");
		if(ob->query_temp("robot_try"))
		ob->delete_temp("robot_try");
		//删除robot test标记
		ob->delete_temp("robot_test");
		if (me->query("reward")) give_reward(ob);
		message_vision("$N突然消失不见了。\n", me);
	} else {
		if(query("failed") > 10)
		{
			message_vision("$N叹了口气道：连续10次都回答错误，你肯定是机器人！\n", me);
			move_to_court(ob);
			destruct(this_object());
			return 1;			
		}
		if (num--){
			add("failed",1);
			return notify_fail("答案不对，再仔细看看。\n");
		}
		
		message_vision("$N叹了口气道："+arg+"？"+ans+"才对，这么简单都答不出？肯定是机器人！\n", me);
		move_to_court(ob);
	}
	destruct(this_object());
	return 1;
}

void test_robot(object obj,int reward)
{	
	object me = this_object();
	string prize;
	//int temp;

	ob = obj;
	
	if (reward) 
		prize="有奖励.";
	else
		prize="无奖励.";
	if (environment(ob) && !ob->is_fighting() && interactive(ob) ) {
		//len = 4 + random(2);
		//目前暂定4位验证码
		ans = makepasswd();
		
		/*temp = random(1) + 4;
		while (temp--) {
			ans += "0";
			ans[<1] += random(10);
		}*/
		// reward 1 的话，表示该runner会给奖励
		if (reward)
		{
			me->set("reward",1);
		}
		message("wizard:linux",sprintf(HIY"%-8s 的机器人检查答案是 %s, %s\n"NOR,ob->query("id"),ans,prize),users());
		timer = 180;		
		if(!BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,"UPDATE members SET runner_code = '"+ans+"',mud_ip='"+query_ip_number(ob)+"' WHERE username = '"+query_id()+"' limit 1",this_object(),"bbs_ok",1))
			CHANNEL_D->do_channel( me, "sys", ob->name(1)+"("+capitalize(getuid(ob))+")的问题答案被论坛拒绝接受。");
	} else destruct(me);
}

void bbs_ok(mixed ret)
{
	if(!clonep()) return;
	
	if(!ob)
	{
		destruct(this_object());
		return;
	}
	message("channel", HIM"银翼杀手略带歉意地说：“机器人稽查，例行公事，请回答一个小问题。”\n", ob);
	message("channel", "如果你正在机器人练功，你可以完全不理会，这对你游戏没有任何影响。\n", ob);
	message("channel", "不过如果你正在电脑前，请按照银翼杀手所提示的进行操作，那样你获得的奖励将会比较丰厚。\n", ob);
	this_object()->set("target",ob->query("id"));
	//当前判断标记，表示机器人在测试
	ob->set_temp("robot_test",1);
	call_out("test1", 10);
	move(ob, 1);
}

string seed = "123456789";

string makepasswd()
{
	int i = 4, j;
	string id = "";

	while (i--) {
		j = random(strlen(seed));
		id += seed[j..j];
	}
	return id;
}

public string query_version(object ob)
{
	if(ob==find_object(BBS_D)) return "1.0";
}