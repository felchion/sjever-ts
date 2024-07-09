//                标准描述长度示例                                   |

#include <ansi.h>
inherit ITEM;

int errs( string msg )
{
     if( !msg ) return 0;
    write( msg );
    return 1;
}

/*
人皮宝衣
燕尾戳魂针
玄阴寒玉匣
紫凤钗
极乐之星
绿玉竹符
晶瓶美人璧
如玉金币令
清心镜
穴道铜人
*/

#define B1		"人皮宝衣"		// 人皮宝衣  	- YEL, HIY
#define B2		"燕尾戳魂针"		// 燕尾戳魂针  	- MAG, HIM
#define B3		"玄阴寒玉匣"		// 玄阴寒玉匣 	- YEL, HIY
#define B4		"紫凤钗"		// 紫凤钗 	- CYN, HIC
#define B5		"极乐之星"		// 极乐之星  	- WHT, HIW
#define B6		"绿玉竹符"		// 绿玉竹符  	- WHT, HIW
#define B7		"晶瓶美人璧"		// 晶瓶美人璧  	- HIM, HIM
#define B8		"如玉金币令"		// 如玉金币令  	- CYN, HIC
#define B9		"清心镜"		// 清心镜  	- RED, HIR
#define B10		"穴道铜人"		// 穴道铜人  	- GRN, HIG

string *clr = ({RED"",GRN"",YEL"",BLU"",MAG"",CYN"",WHT"",HIR"",HIG"",HIY"",HIB"",HIM"",HIC"",HIW""});

string game_now = "等待移动命令......";
string att_now = "not_ok";
int target_hp, target_damage;
string pic_me, pic_target;


string status_color(int current, int max);
void new_game_set( int amount, string mark );
void map(object me);
void check_hp(object me);
object get_me();
string get_msg( int x, int y, string type );
void xunbaotu(object me);
void fight_start( string who, object me );
void be_attack(object me);
void target_check_hp(object me);


int finded_baowu( int which, string name )
{
	object me = get_me();
	int score;

	if( !me ) return 0;

	add("be_finded", 1);
	set("find_b"+which,1);
	set("b"+ which, HIW + query("b"+ which) );
	map(me);
	tell_object(me,HIC"【南海探宝】  恭喜你，你的船只找到了 "+clr[random(sizeof(clr))] + name + HIC" ！\n"NOR);

	if( query("be_finded") == 10 )
	{
		me->delete_temp("hanghai_gameing");
		game_now = "游戏结束";
		score = query("score");

		score += query("att_hp") * 500;
		score += query("att_pg") * 500;
		score += query("att_rs") * 500;
		score += query("att_hj") * 500;
		score += query("att_fd") * 500;

		score += query("hp") * 1000;

		if( score < 1 ) score = 1;


		tell_object(me,HIY"
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             【南海探宝】

  你找齐了十件宝物！！南海探宝胜利结束了。
  你的最终得分是 "+score+" 分(仅供参考)。

≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
\n"NOR);
        	/*if (!dbquery("update Users set U_Credit = U_Credit + " + 100 + " where U_Username = \"" + me->query("id")+ "\""))
        	{
        		message_vision("$N成功地完成了 【南海探宝】 游戏，积分增加失败，请与管理员联系。！\n", me);
			log_file("job/SEA", sprintf("%s(%s) 完成【南海探宝】游戏，积分增加失败。\n",me->query("name"), getuid(me) ) );
        	}
        	
        	else
        	{
			message_vision("$N成功地完成了 【南海探宝】 游戏！\n", me);
			log_file("job/SEA", sprintf("%s(%s) 完成【南海探宝】游戏，奖励一百点书剑积分。\n",me->query("name"), getuid(me) ) );
		}*/
		message_vision("$N成功地完成了 【南海探宝】 游戏！\n", me);		
		me->move("/d/foshan/dukou_tanbao");
		remove_call_out("del_me");
		call_out("del_me", 1);
	}

	return 1;
}


// 获得“me”
object get_me()
{
	object me;
	if( !query("owner_id") ) return 0;

	me = find_player( query("owner_id") );
	if( !me ) return 0;
	return me;
}

// 获得攻击力段
int damage( int type )  // 6-11
{
	if( !type || type<1 || type>5 ) type = 1;

	switch( type )
	{
		case 1:	return ( 6+random(6) );
		case 2:	return ( 12+random(6) );
		case 3:	return ( 18+random(6) );
		case 4:	return ( 24+random(6) );
		case 5:	return ( 30+random(6) );
	}

	return (6+random(6));
}

void del_me()
{
	destruct( this_object() );
}

void create()
{
	set_name(HIY"航海经纬仪"NOR, ({"jingwei yi","yi"}));
	set_weight(1);
	if (clonep())
		set_default_object(__FILE__);
	else
	{
		set("long", "【南海探宝】必备之物，大海航行不可缺少，一排篆头小字写着他的作用。
查询现在的游戏状态，输入： game check
任何时候结束游戏，输入：   game over
\n");
		set("unit", "具");
		set("value", 0);
	}

	set("no_get", 1);
	set("no_drop",1);
	set("no_steal",1);
	set("no_give",1);
	setup();
}


void init()
{
	add_action("do_game", "game");
	add_action("do_go", "go");
	add_action("do_liaowang", "liaowang");
	add_action("do_attack", "atk");
	add_action("do_attack", "attack");
	add_action("do_look", "l");
	add_action("do_look", "look");
}


void new_game(object me)
{
	if( !me ) return del_me();

	set("owner_id", getuid(me));
	set("owner_name", me->query("name"));

// ******************************************
	new_game_set( 8, "岛屿" );

// ******************************************
	new_game_set( 4, "望远镜" );

// ******************************************
	new_game_set( 4, "报警探索仪" );

// ******************************************
	new_game_set( 4, "寻宝图" );

// ******************************************
//	new_game_set( 2, "旋涡" );
// 不要旋涡的设置！

// ******************************************
	new_game_set( 2, "暗礁" );

// ******************************************
	new_game_set( 3, "补给品" );

// ******************************************
	new_game_set( 6, "海盗船" );

// ******************************************
	new_game_set( 6, "倭寇船" );

// ******************************************
	new_game_set( 6, "天鹰教船" );

// ******************************************
	new_game_set( 6, "潜鲸帮船" );

// ******************************************
	new_game_set( 6, "海砂帮船" );


// 宝物部分
// ******************************************
	new_game_set( 1, B1);
	new_game_set( 1, B2);
	new_game_set( 1, B3);
	new_game_set( 1, B4);
	new_game_set( 1, B5);
	new_game_set( 1, B6);
	new_game_set( 1, B7);
	new_game_set( 1, B8);
	new_game_set( 1, B9);
	new_game_set( 1, B10);

	new_game_set( 1, "当前船只位置");

	set("att_rb", 999999);
	set("att_hp", 1);
	set("att_pg", 1);
	set("att_rs", 1);
	set("att_hj", 1);
	set("att_fd", 1);
	set("hp", 100);

	
	tell_object(me,">>>>>>>  各类武器及船只补给完毕。\n");
	me->set_temp("hanghai_gameing", "waiting_go");
	tell_object(me,"\n============ 设置成功！正在进入游戏…… ============\n");


	set("b1", HIY"人皮宝衣"NOR);
	set("b2", HIM"燕尾戳魂针"NOR);
	set("b3", YEL"玄阴寒玉匣"NOR);
	set("b4", CYN"紫凤钗"NOR);
	set("b5", WHT"极乐之星"NOR);
	set("b6", WHT"绿玉竹符"NOR);
	set("b7", RED"晶瓶美人璧"NOR);
	set("b8", CYN"如玉金币令"NOR);
	set("b9", RED"清心镜"NOR);
	set("b10",GRN"穴道铜人"NOR);
	
	call_out("game_start", 1,me);
}

void new_game_set( int amount, string mark )
{
	int x, y;
// 这里的 x, y ， x是竖数第几横，y是横数第几个。

	while( amount-- )
	{
		x = 1 + random(10);
		y = 1 + random(16);

		if( query( "aaa(" +x+ "," +y+ ")" ) )
		{
			amount ++;
			continue;
		}

		set( "aaa(" +x+ "," +y+ ")" , mark );
		if( mark == "当前船只位置" )
		{
			set("now_x", x);
			set("now_y", y);
		}
		if( mark == "岛屿" )
			set("island("+x+","+y+")", "new" );

	}
	if (wizardp(this_player()))
		write(">>>>>>>  " + mark + " 设置完毕。\n");
}

void game_start(object me)
{
	//object me = get_me();
	if( !me ) return del_me();

	me->set_temp("hanghai_gameing", "ok");
	message_vision("$N进入了 南海探宝 游戏！\n", me);

	tell_object(me,"
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
你决定进入南海探宝！探宝的目标为寻得10件江湖许久不见的宝物！
分别是：
人皮宝衣、燕尾戳魂针、玄阴寒玉匣、紫凤钗、极乐之星
绿玉竹符、晶瓶美人璧、如玉金币令、清心镜、穴道铜人
移动指令与 MUD 内相同。祝南海探宝成功！
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈n\n");

	map(me);
}


////////////////////////////////////
/////////  玩家操作函数  ///////////
////////////////////////////////////
int do_look()
{
        object me = this_player(), who;
	who = get_me();

	if( !who || me != who ) return 0;
	
	if( !me->query_temp("hanghai_gameing") )
		return 0;
	map(me);
	return 1;
	
}
int do_game(string arg)
{
	object me = this_player(), who;
	who = get_me();

	if( !who || me != who ) return 0;

	if( !arg || ( arg!= "check" && arg != "ck" && arg != "over") )
		return 0;

	if( !me->query_temp("hanghai_gameing") )
		return 0;

	if( arg == "check" || arg == "ck" )
	{
		map(me);
		return 1;
	}

	if( arg == "over" )
	{
		me->delete_temp("hanghai_gameing");
		message_vision("$N中途放弃，退出了 南海探宝 游戏。\n", me);
		tell_object(me,HIR"\n  ≈≈≈≈≈≈已退出南海探宝，祝一切愉快！≈≈≈≈≈≈\n\n"NOR);
		remove_call_out("del_me");
		call_out("del_me", 1);
		me->move("/d/foshan/dukou_tanbao");
		return 1;
	}
	return 1;
}

int do_go(string arg)
{
	object me = this_player(), who;
	int x, y, old_x, old_y;

	string *dirs = ({
"north", "south", "west", "east", "northwest", "northeast", "southwest", "southeast", });

	string here, msg;

	who = get_me();
	if( !who || who != me ) return 0;
	if( member_array( arg , dirs ) == -1 ) return 0;

	if( me->query_temp("hanghai_gameing") != "ok" )
		return errs("现在不许移动！正在： "+ game_now + " 。\n");

	x = query("now_x");
	y = query("now_y");

	old_x = x;
	old_y = y;

	switch( arg )
	{
		case "north": x--; break;
		case "south": x++; break;
		case "west" : y--; break;
		case "east" : y++; break;
		case "northwest" : x--; y--; break;
		case "northeast" : x--; y++; break;
		case "southwest" : x++; y--; break;
		case "southeast" : x++; y++; break;
	}

	if( x<1 || y<1 || x>10 || y>16 )
		return errs("这个方向已经无法前进了！\n");

	set("be_finded(" +old_x+ "," +old_y+ ")", 1); 


	if( query( "island(" +old_x+ "," +old_y+ ")" ) == "new"
	|| query( "island(" +old_x+ "," +old_y+ ")" ) == "old" )
	{
		set("island(" +old_x+ "," +old_y+ ")", "old");
		set("aaa("+old_x+","+old_y+")", "旧的岛屿");
	}

	if( !query( "island(" +old_x+ "," +old_y+ ")" ) )
		delete("aaa(" +old_x+ "," +old_y+ ")" );

	here = query( "aaa(" +x+ "," +y+ ")" );

	set("aaa(" +x+ "," +y+ ")", "当前船只位置");
	set("now_x", x);
	set("now_y", y);

// 各类判断！！！

	if( here == "岛屿" )
	{
		add("att_hp", 1);
		add("att_pg", 1);
		add("att_rs", 1);
		add("att_hj", 1);
		add("att_fd", 1);
		add("hp", 20+random(11));
		if( query("hp") > 100 ) set("hp", 100);
		map(me);
		tell_object(me,HIG"【南海探宝】发现岛屿！得到了弹药补给和船只修理。\n"NOR);
		return 1;
	}

	if( here == "补给品" )
	{
		add("hp", 20+random(11));
		if( query("hp") > 100 ) set("hp", 100);
		map(me);
		tell_object(me,HIC"【南海探宝】发现修补材料！船只得到了修理。\n"NOR);
		return 1;
	}

	if( here == "暗礁" )
	{
		add("hp", - (30+random(11)));
		map(me);
		tell_object(me,HIR"【南海探宝】糟糕！撞上了暗礁！船受到了严重的损坏。\n"NOR);
		check_hp(me);
		return 1;
	}

	if( here == B1 )
		return finded_baowu( 1, B1 );
	if( here == B2 )
		return finded_baowu( 2, B2 );
	if( here == B3 )
		return finded_baowu( 3, B3 );
	if( here == B4 )
		return finded_baowu( 4, B4 );
	if( here == B5 )
		return finded_baowu( 5, B5 );
	if( here == B6 )
		return finded_baowu( 6, B6 );
	if( here == B7 )
		return finded_baowu( 7, B7 );
	if( here == B8 )
		return finded_baowu( 8, B8 );
	if( here == B9 )
		return finded_baowu( 9, B9 );
	if( here == B10 )
		return finded_baowu( 10, B10 );


	if( here == "望远镜" )
	{
		game_now = "了望中 （指令： liaowang x y）";
		map(me);
		tell_object(me,HIM"【南海探宝】发现了望远镜！输入 (liaowang x y) 指令查看该点周边海域的情形！ \n"NOR);
		me->set_temp("hanghai_gameing", "wangyuanjing");
		return 1;
	}

	if( here == "报警探索仪" )
	{
		map(me);
		tell_object(me,HIM"【南海探宝】发现了报警探索仪！\n"NOR);


		msg = HIM"\n报警仪探索中……自动探索 "+x+","+y+" 区域的情况。\n\n";

		msg += get_msg( x-1,y-1, "bjy" );
		msg += get_msg( x-1,y,   "bjy" );
		msg += get_msg( x-1,y+1, "bjy" );
		msg += get_msg( x,  y-1, "bjy" );
		msg += 	get_msg( x,  y,   "bjy" );
		msg += 	get_msg( x,  y+1, "bjy" );
		msg += 	get_msg( x+1,y-1, "bjy" );
		msg += 	get_msg( x+1,y,   "bjy" );
		msg += 	get_msg( x+1,y+1, "bjy" );

		msg += "\n探索危险区域报警完毕。\n\n"NOR;
		tell_object(me, msg );
		return 1;
	}

	if( here == "寻宝图" )
	{
		map(me);
		tell_object(me,HIC"【南海探宝】发现了寻宝图！\n"NOR);
		xunbaotu(me);
		return 1;
	}


	if( here == "海盗船" || here == "潜鲸帮船" || here == "天鹰教船"
	 || here == "倭寇船" || here == "海砂帮船" )
	{
		game_now = "与 "+ here+ " 战斗中！ 等待敌人攻击";
		me->set_temp("hanghai_gameing", "fighting");
		att_now = "not_ok";
		map(me);
		tell_object(me,HIR"【南海探宝】糟糕！遭遇了 "+ here+ " ！进入紧急战斗状态！！！\n"NOR);
		set("who_name", here);
		remove_call_out("fight_start");
		call_out("fight_start", 1, here,me);
		return 1;
	}


	map(me);
	if (random(2))
		tell_object(me,HIW"【南海探宝】漫漫航程，什么也没有发现……\n"NOR);
	else
		tell_object(me,HIW"【南海探宝】你扬帆前行，但是什么也没有发现……\n"NOR);
	return 1;
}

void map(object me)
{
	string the_map = HIC"  ⊕南海探宝航行图⊕  "HIW"\n  剩余生命值："+status_color(query("hp"),100) + query("hp") + HIW + " 点  已发现宝物数："+ query("be_finded")+NOR"\n\n        ", temp_map, temp2;
	int x, y, temp = 0;

	for( x=1; x<11; x++ )
	{
		for( y=1; y<17; y++ )
		{
			if( temp == 16 )
			{
				temp = 0;
				the_map += "\n        ";
			}

			temp2 = "("+x+","+y+")";

			if( query( "be_finded"+temp2) )
				temp_map = HIC"□"NOR;
			else if( query("weixian"+temp2) )
				temp_map = HIR"■"NOR;
			else temp_map = HIC"■"NOR;
			if( query( "aaa"+temp2 ) == "岛屿" )
				temp_map = HIG"▲"NOR;
			if( query( "aaa"+temp2 ) == "旧的岛屿" )
				temp_map = HIG"△"NOR;
			if( query( "aaa"+temp2 ) == "当前船只位置" )
				temp_map = BLINK+HIY"≌"NOR;

			the_map += temp_map;
			temp++;
		}
	}

	the_map += "\n\n";

//	for( temp = 1;temp<11;temp++)
//	{
//		set("b"+temp, HIW+query("b"+temp) );
//	}
	the_map  += "  "HIC"■"NOR" 未知海域 "HIG"▲"NOR" 岛屿  "HIG"△"NOR" 空岛 "HIR"■ "NOR"危险海域  \n  "HIC"□"NOR" 已知海域 "BLINK+HIY"≌ "NOR"当前船只位置\n";
	the_map  += HIC"  ⊕宝物列表⊕ "NOR"×"HIC" 未找到 "HIY"√"HIC" 已找到\n"NOR;
	the_map += sprintf( "  %s%s  %s%s  %s%s  %s%s  %s%s\n  %s%s  %s%s  %s%s  %s%s  %s%s\n\n",
		query("b1") ,query("find_b1")?HIY"√"NOR:"×", 
		query("b2") ,query("find_b2")?HIY"√"NOR:"×", 
		query("b3"), query("find_b3")?HIY"√"NOR:"×", 
		query("b4"), query("find_b4")?HIY"√"NOR:"×", 
		query("b5"), query("find_b5")?HIY"√"NOR:"×", 
		query("b6"), query("find_b6")?HIY"√"NOR:"×", 
		query("b7"), query("find_b7")?HIY"√"NOR:"×", 
		query("b8"), query("find_b8")?HIY"√"NOR:"×", 
		query("b9"), query("find_b9")?HIY"√"NOR:"×", 
		query("b10"),query("find_b10")?HIY"√"NOR:"×" ); 	

	the_map += sprintf(HIW"  船舱武器情况：\n  肉搏之刃(rb)：∞  火炮(hp)：%d  拍竿(pg)：  %d
  漫天飞刀(fd)：%d   火箭(hj)：%d  燃烧弹(rs)：%d\n"NOR,
		query("att_hp"), query("att_pg"), 
		query("att_fd"), query("att_hj"), query("att_rs") );




	tell_object(me, HIY"≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈\n"NOR +the_map +
HIY"≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈\n"NOR"当前坐标：("+ HIG + query("now_x") + "," + query("now_y") + NOR + ")  \n当前状态：现在，"+ game_now +" \n");

}

void check_hp(object me)
{
	//object me = get_me();
	int x, y, temp = 0;
	string the_map, temp_map, temp2;

	if( !me ) call_out("del_me", 1);

	if( query("hp") > 0 ) return;

	the_map = HIR"\n【南海探宝】  -- 失败结果公布(共寻得"+query("be_finded")+"件宝物)\n\n        "NOR;

	for( x=1; x<11; x++ )
	{
		for( y=1; y<17; y++ )
		{
			if( temp == 16 )
			{
				temp = 0;
				the_map += "\n        ";
			}

			temp2 = query("aaa("+x+","+y+")");
			temp_map = HIC"□"NOR;

			if( temp2 == "海盗船" )
				temp_map = HIR"盗"NOR;
			if( temp2 == "倭寇船" )
				temp_map = HIR"倭"NOR;
			if( temp2 == "天鹰教船" )
				temp_map = HIR"鹰"NOR;
			if( temp2 == "潜鲸帮船" )
				temp_map = HIR"潜"NOR;
			if( temp2 == "海砂帮船" )
				temp_map = HIR"砂"NOR;
			if( temp2 == "暗礁" )
				temp_map = HIW"礁"NOR;


			if( temp2 == "岛屿" )
				temp_map = HIG"▲"NOR;
			if( temp2 == "旧的岛屿" )
				temp_map = HIG"△"NOR;
			if( temp2 == "当前船只位置" )
				temp_map = BLINK+HIB"≌"NOR;

			if( temp2 == B1 || temp2 == B2 || temp2 == B3
			 || temp2 == B4 || temp2 == B5 || temp2 == B6
			 || temp2 == B7 || temp2 == B8 || temp2 == B9 || temp2 == B10 )
				temp_map = HIY"宝"NOR;

			if( temp2 == "望远镜" )
				temp_map = HIM"望"NOR;
			if( temp2 == "报警探索仪" )
				temp_map = HIM"警"NOR;
			if( temp2 == "补给品" )
				temp_map = HIM"补"NOR;
			if( temp2 == "寻宝图" )
				temp_map = HIM"图"NOR;

			the_map += temp_map;
			temp++;
		}
	}

	the_map += "\n\n";

	tell_object(me, the_map );
	me->delete_temp("hanghai_gameing");
	game_now = "已经灭亡！";


	message_vision("$N游戏失败，退出了 【南海探宝】 游戏。\n", me);
	remove_call_out("game_over_over");
	call_out("game_over_over", 1, me);
	return;
}

void game_over_over(object me)
{
	tell_object(me,HIR"
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             【南海探宝】

      你的船只损坏特别严重，再也无法支持，
          终于沉入了荒洋大海之中！
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
\n"NOR);
	remove_call_out("del_me");
	call_out("del_me", 1);
	return;
}

int do_liaowang( string arg )
{
	object me = this_player(), who;
	int x, y;
	string msg;

	who = get_me();
	if( !who || who != me ) return 0;

	if( me->query_temp("hanghai_gameing") != "wangyuanjing" )
		return 0;

	if( !arg || sscanf( arg, "%d %d", x, y ) != 2 )
		return errs("请输入正确坐标！x -> 竖着数第几横  y -> 横着数第几个\n");
	if( x<1 || x>10 || y<1 || y>16 )
		return errs("请输入正确坐标！x -> 竖着数第几横  y -> 横着数第几个\n");


	msg = HIM"了望中……你决定查看 "+x+","+y+" 区域的情况。\n\n";

	msg += get_msg( x-1,y-1, "wyj" );
	msg += get_msg( x-1,y,   "wyj" );
	msg += get_msg( x-1,y+1, "wyj" );
	msg += get_msg( x,  y-1, "wyj" );
	msg += 	get_msg( x,  y,   "wyj" );
	msg += 	get_msg( x,  y+1, "wyj" );
	msg += 	get_msg( x+1,y-1, "wyj" );
	msg += 	get_msg( x+1,y,   "wyj" );
	msg += 	get_msg( x+1,y+1, "wyj" );

	msg += "\n查看完毕。\n\n"NOR;

	tell_object(me, msg );
	me->set_temp("hanghai_gameing", "ok");
	game_now = "等待移动命令......";
	return 1;
}

string get_msg( int x, int y, string type )
{
	string msg;

	if( x<1 || x>10 || y<1 || y>16 ) return "";

	msg = query("aaa(" +x+ "," +y+ ")" );

	if( !msg )
	{
		if( type == "wyj" ) return "("+x+","+y+") -----------\n";
		return "";
	}

	if( msg == "暗礁" || msg == "海盗船" || msg == "倭寇船"
	 || msg == "天鹰教船" || msg == "潜鲸帮船" || msg == "海砂帮船" )
	{
		set("weixian("+x+","+y+")", 1);
		if( type == "wyj" ) return "("+x+","+y+") " + msg + "\n";
		return "("+x+","+y+") 有危险潜伏！\n";
	}

	if( type == "wyj" ) return "("+x+","+y+") " + msg + "\n";
	else return "";
}

void xunbaotu(object me)
{
	int x, y;
	string msg;

	for( x = 1; x<11; x++ )
	{
		for( y=1;y<17;y++)
		{
			msg = query("aaa(" +x+ "," +y+ ")" );
			if( msg == B1 || msg == B2 || msg == B3 || msg == B4
			 || msg == B5 || msg == B6 || msg == B7 || msg == B8
			 || msg == B9 || msg == B10 )
			{
				tell_object(me, HIM"("+x+","+y+")    该处藏有宝藏！\n"NOR);
				return;
			}
		}
	}
}

void fight_start( string who,object me )
{
	switch( who )
	{
		case "海盗船": pic_target = "≌";target_damage=2; break;
		case "倭寇船": pic_target = "∝"; target_damage=4; break;
		case "天鹰教船": pic_target = "※"; target_damage=6; break;
		case "潜鲸帮船": pic_target = "∮"; target_damage=8; break;
		case "海砂帮船": pic_target = "≌"; target_damage=10; break;
	}

	target_hp = 50+ random(31);
	pic_me = "≌";

	remove_call_out("be_attack");
	call_out("be_attack", 1, me);
}

void be_attack(object me)
{
	int damage;
	string pic1, pic2;

	damage = target_damage + random(3);
	add("hp", -damage);

	pic1 = HIR + pic_target + " " + query("who_name") + "("+ target_hp + ")"NOR;
	pic2 = sprintf("    "HIG"(%|4d)航海者 "HIR"¤"NOR, query("hp"), pic_me);

	tell_object(me, sprintf("
                     _ _ _ _ _ _ _ _ _ _       ATTACK!
            "HIR"-%-2d"NOR"    /                     `"HIY"¤"NOR"
%s                        %s
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈\n\n", damage, pic2,  pic1));

	tell_object(me,HIY"【南海探宝】船只受到了来自 "+clr[random(sizeof(clr))] + query("who_name") + HIY + " 的攻击，生命值减少："+ HIR + chinese_number(damage) + HIY + "点，剩余：" + HIG + chinese_number(query("hp")) + HIY + "点。\n"NOR);
	check_hp(me);
	call_out("can_att", 1, me);
}

void can_att(object me)
{
	if( query("hp") < 1 ) return;

	tell_object(me,HIY"【南海探宝】轮到你攻击了！(atk 方式)\n"NOR);
	tell_object(me, sprintf(HIG"⊕船舱武器情况⊕\n肉搏之刃(rb)：无限  火炮(hp)：%d  拍竿(pg)：%d\n漫天飞刀(fd)：%d     火箭(hj)：%d  燃烧弹(rs)：%d\n"NOR,
		query("att_hp"), query("att_pg"), query("att_fd"), query("att_hj"), query("att_rs") ) );
	game_now = "与 "+ query("who_name")+ " 战斗中!等待攻击指令";
	att_now = "ok";
}

int do_attack(string arg)
{
	object me = this_player(), who;
	string target_name = query("who_name"), pic1, pic2, temp;
	int damage;

	who = get_me();
	if( !who || who!=me ) return 0;

	if( att_now != "ok" ) return errs("现在并不是你攻击的时候！\n");
	if( !arg || ( arg != "rb" && arg != "hp" && arg != "pg" && arg != "rs"
	 && arg != "hj" && arg != "fd" ) )
		return errs("你选择的武器不对！\n");

	if( query("att_"+ arg ) < 1 ) return errs("这种武器已经没有了！\n");
	add( "att_"+ arg , -1 );

	switch( arg )
	{
		case "rb":  temp = "肉搏之刃"; damage = damage(1); break;
		case "hp":
			temp = "火炮";
			if( target_name == "海盗船" ) damage = damage(5);
			if( target_name == "倭寇船" ) damage = damage(4);
			if( target_name == "天鹰教船" ) damage = damage(3);
			if( target_name == "潜鲸帮船" ) damage = damage(2);
			if( target_name == "海砂帮船" ) damage = damage(1);
			break;
		case "pg":
			temp = "拍竿";
			if( target_name == "海盗船" ) damage = damage(4);
			if( target_name == "倭寇船" ) damage = damage(1);
			if( target_name == "天鹰教船" ) damage = damage(5);
			if( target_name == "潜鲸帮船" ) damage = damage(3);
			if( target_name == "海砂帮船" ) damage = damage(2);
			break;
		case "rs":
			temp = "燃烧弹";
			if( target_name == "海盗船" ) damage = damage(2);
			if( target_name == "倭寇船" ) damage = damage(3);
			if( target_name == "天鹰教船" ) damage = damage(4);
			if( target_name == "潜鲸帮船" ) damage = damage(1);
			if( target_name == "海砂帮船" ) damage = damage(5);
			break;
		case "hj":
			temp = "火箭";
			if( target_name == "海盗船" ) damage = damage(1);
			if( target_name == "倭寇船" ) damage = damage(5);
			if( target_name == "天鹰教船" ) damage = damage(2);
			if( target_name == "潜鲸帮船" ) damage = damage(4);
			if( target_name == "海砂帮船" ) damage = damage(3);
			break;
		case "fd":
			temp = "漫天飞刀";
			if( target_name == "海盗船" ) damage = damage(3);
			if( target_name == "倭寇船" ) damage = damage(2);
			if( target_name == "天鹰教船" ) damage = damage(1);
			if( target_name == "潜鲸帮船" ) damage = damage(5);
			if( target_name == "海砂帮船" ) damage = damage(4);
			break;
	}

	target_hp -= damage;
	add("score", damage);

	pic1 = HIR"¤" + " " + query("who_name") + "("+ target_hp + ")"NOR;
	pic2 = sprintf("    "HIG"(%|4d)航海者 %s"NOR, query("hp"), pic_me);


	tell_object(me, sprintf("
       %|8s！    _ _ _ _ _ _ _ _ _ _
                   "HIY"¤"NOR"                     \\    "HIR"-%d"NOR"
%s                        %s
≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈\n\n", temp,damage, pic2,  pic1));

	tell_object(me,HIY"【南海探宝】你的船只使用 "+ temp + " 对 "+query("who_name")+ " 进行攻击！损害对方生命值"+ chinese_number(damage) + "点。\n"NOR);

	att_now = "not_ok";
	remove_call_out("target_check_hp");
	call_out("target_check_hp", 1, me);

	return 1;
}

void target_check_hp(object me)
{
	//object me = get_me();
	if( !me )
	{
		remove_call_out("del_me");
		call_out("del_me", 1);
		return;
	}

	if( target_hp < 1 )
	{
		game_now = "等待移动命令......";
		add("hp", 1+(random(5)) );
		if( query("hp") > 100 ) set("hp", 100);
		map(me);
		tell_object(me,HIG"\n【南海探宝】打败了 "+query("who_name")+ " ！船只受到了些许修复。继续出发吧！\n\n"NOR);
		me->set_temp("hanghai_gameing", "ok");
		add("score", 100);
		return;
	}

	be_attack(me);
}

string status_color(int current, int max)
{
	int percent;

	if( max ) percent = current * 100 / max;
	else percent = 0;
	if( percent > 100 ) return HIC;
	if( percent >= 90 ) return HIG;
	if( percent >= 60 ) return HIY;
	if( percent >= 30 ) return YEL;
	if( percent >= 10 ) return HIR;
	return RED;
}