// 扫雷游戏
/*
	by snowman@SJ 20/11/2000
	
	Copyright 1997-1999 ShuJian MUD Wizard Group.
   	All rights reserved.
*/

// fixed some little bugs.. snowman@SJ

#include <ansi.h>
#define CLR ESC+"[2J"	

inherit ROOM;
inherit F_SAVE;

static int x = 15, y = 20, bomb = 40;
static string *number = ({ "■",CYN"１"NOR,HIG"２"NOR,HIR"３"NOR,HIC"４"NOR,RED"５"NOR,HIM"６"NOR,MAG"７"NOR,BLU"８"NOR,WHT"★"NOR,"□",HIY"⊙"NOR });
static mapping game;
static mapping map;
static int mine = 0, real_mine = 0, t = 0;
static string player, table = "现在桌上什么也没有。\n";
static string rules = "      
       	
“扫雷”游戏的目标是迅速找到雷区中的所有地雷，而不许踩到地雷。
要揭开某个方块，请使用"CYN" mine dig x?y? "NOR"打开它。若是所揭方块下有雷，您便输了这一局。 
如果方块上出现数字，它代表在它周围的八个方块中共有多少颗地雷。 
如果您认为某个方块下埋着地雷，请用"CYN" mine at x?y?"NOR" 确定它，找完所有地雷就赢。 

目前的省却值是 "+bomb+" 枚地雷， X轴为"+x+"，Y轴为"+y+"。
系统自动记录完成时间前十名的玩家及所用时间(list)。

游戏指令：
	"HIC"mine start"NOR"	：	开始游戏。
	"HIC"mine cancel"NOR"	：	结束目前的游戏。
	"HIC"mine dig x?y?"NOR"	：	打开位于第 ? 个X轴和第 ? 个Y轴的方块。
	"HIC"mine at x?y?"NOR"	：	确定位于第 ? 个X轴和第 ? 个Y轴的方块是地雷。
	"HIC"mine re x?y?"NOR"	：	取消此方块的地雷确定。
	"HIC"mine finish"NOR"	：	如果你确定你已经找出了所有地雷，用这个命令。
	
此游戏基于 Microsoft (R) 扫雷

\n";

mapping *top10;
string look_map();
string look_table();
void clean_table();

void create()
{
	seteuid(getuid());
	restore();
	delete("no_death");
	if (!sizeof(top10)) top10 = ({ ([ "id": "beginner", "time": 3600 ]) });
	
        set("short", YEL"扫雷游戏室"NOR);
        set("long", @LONG
这里是书剑里的扫雷天地，可以玩扫雷(Minesweep）游戏或观看，正中的
桌子上是游戏主表格(table)，旁边的牌子(paizi)上写有游戏规则和玩法，一
旁是成绩表(list)。
LONG    );

        set("no_fight", 1);
        set("no_get_from", 1); 
        set("no_sleep_room", 1); 
        set("item_desc", ([             
        	"list" : (: look_map :),  
        	"table" : (: look_table :),        
             	"paizi" : rules,
        ]));
        set("exits", ([
        	"south" : __DIR__"chmiao",
           	//"up" : SP_A("wizard/guest_room"),
        ]));

        set("incity",1);
	setup(); 
}

void init()
{
	object me = this_player();
	if (!userp(me)) return;
	add_action("do_set", "mineset");
	add_action("main_mine", "mine");	                          
}

string look_table(){ return table; }
       
string look_map()
{
	int i;
	string tmp = HIW"最佳成绩表：\n"NOR;
	for (i = 0; i < sizeof(top10); i++){
		if( i > 9 ){
			top10[i] = 0;
			continue;
		}
		tmp += sprintf(" %-2s，%-8s "HIY"→"NOR" %s\n", (i+1)+"", capitalize(top10[i]["id"]), CHINESE_D->chinese_time(top10[i]["time"]));
	}
	top10 -= ({ 0 });
	return tmp;
}         
     
int do_set(string arg)
{
	int i, j, k;
	if( !wizardp(this_player()) )
		return notify_fail("指令：mine <dig|at|re x?y?> | start | cancel | ? 。\n");
	if( sscanf(arg, "%d x%d y%d", k, i, j) == 3 ){
		if( k < 10 || i < 10 || j < 10 )
			return notify_fail("set <bomb> x<数目> y<数目>： 各数字最少为 10。\n");
		if( k > 99 )
			return notify_fail("set <bomb> x<数目> y<数目>： 地雷数最多为 99。\n");
		if( i > 25 || j > 35 )
			return notify_fail("set <bomb> x<数目> y<数目>： x最多为 25，y最多为 35。\n");
		bomb = k;
		x = i;
		y = j;
		clean_table();
		return notify_fail("地雷数："+ bomb +"，X 轴："+ x +"，Y 轴："+ y +"。\n");
	}
	else return notify_fail("格式：mineset <bomb> x<轴目> y<轴目>。\n");
}


int remove()
{
	save();
	return 1;
}

string query_save_file()
{
	return "/log/mine_rank";
}

int clean_all()
{
	top10 = ({ ([ "id": "beginner", "time": 3600 ]) });
	return remove();
}

void clean_table()
{
	int i;
	game = mine = real_mine = t = 0;
	player = 0;
	table = "现在桌上什么也没有。\n";
	map = allocate_mapping(x);
	for(i = 0; i < x; i++ ){
		map[i] = allocate_mapping(y);
	}
	tell_room(this_object(), HIW"\n游戏可以重新开始了！\n"NOR);	
}
mapping make_game(int mines)
{
	mapping m = allocate_mapping(x);
	int k, i, j;
	
	for(i = 0; i < x; i++ ){
		m[i] = allocate_mapping(y);
	}

	i = x;
	j = y;
	for(k = 0;k < mines; k++){
		i = random(x);
		j = random(y);
		if( m[i][j] == 9 ){
			k--;
			continue;
		}
		else	m[i][j] = 9;
	}
	
	for(i = 0; i < x; i++){
		for(j = 0; j < y; j++){
			k = m[i][j];
			if( k != 9 ){
				if( j > 0 && m[i][j-1] == 9 ) k++;
				if( j < y-1 && m[i][j+1] == 9 ) k++;
				if( i > 0 && m[i-1][j] == 9 ) k++;
				if( i < x-1 && m[i+1][j] == 9 ) k++;
				if( i-1 >= 0 ){
					if( j-1 >= 0 && m[i-1][j-1] == 9 ) k++;
					if( j+1 < y && m[i-1][j+1] == 9 ) k++;
				}
				if( i+1 < x ){
					if( j-1 >= 0 && m[i+1][j-1] == 9 ) k++;
					if( j+1 < y && m[i+1][j+1] == 9 ) k++;
				}
			}
			m[i][j] = k;
		}
	}
	return m;
}
string drew_map(int k)
{
	int i, j;
	string str = YEL"现在是 "+ player +" 在扫雷。\n\n   "NOR;
	
	for(i = 0; i < y; i++)
		str += sprintf("%-2s", (i+1)+"");
	str += " ←Y轴\n 1,";
	for(i = 0; i < x; i++){
		for(j = 0; j < y; j++){
			if( k )
				str += number[game[i][j]];
			else 	str += number[map[i][j]];
		}
		if( i < x-1 )
			str += "\n"+sprintf("%3s", (i+2)+",");
		else str += "\n↑\t地雷总数："+ bomb +"，目前已探明数："+ mine +"。\nX轴\n";
	}
	table = str;
	return str;
}

int block_check(int i, int j)
{
	int k, l = 0;
	if( i-1 >= 0 ){
		if( (k = game[i-1][j]) < 9 && map[i-1][j] < 9 ) map[i-1][j] = ((k == 0)?10:k);
		else l += 1;
	}
	if( i+1 < x ){
		if( (k = game[i+1][j]) < 9 && map[i+1][j] < 9 ) map[i+1][j] = ((k == 0)?10:k);
		else l += 2;
	}
	if( (j-1) >= 0 ){
		if( (k = game[i][j-1]) < 9 ){
			if( map[i][j-1] < 9 )
		 		map[i][j-1] = ((k == 0)?10:k);
		}
		else l += 3;
		if( i-1 >= 0 )
			if( (k = game[i-1][j-1]) < 9 && map[i-1][j-1] < 9 && l % 4 != 0 ) map[i-1][j-1] = ((k == 0)?10:k);
		if( i+1 < x )
			if( (k = game[i+1][j-1]) < 9 && map[i+1][j-1] < 9 && l % 5 != 0 ) map[i+1][j-1] = ((k == 0)?10:k);
	}
	if( (j+1) < y ){
		if( (k = game[i][j+1]) < 9 ){ 
			if( map[i][j+1] < 9 )
				map[i][j+1] = ((k == 0)?10:k);
		}
		else l += 5;
		if( i-1 >= 0 )
			if( (k = game[i-1][j+1]) < 9 && map[i-1][j+1] < 9 && l % 6 != 0 ) map[i-1][j+1] = ((k == 0)?10:k);
		if( i+1 < x )
			if( (k = game[i+1][j+1]) < 9 && map[i+1][j+1] < 9 && l % 7 != 0 ) map[i+1][j+1] = ((k == 0)?10:k);
	}
}
int main_mine(string arg)
{
	int i, j, k;
	object ob, me;
	string tmp = YEL"\n扫雷游戏："NOR;
	
	if( !arg ){
		if( game )
			 return notify_fail(table);
		else return notify_fail("指令：mine <dig|at|re x?y?> | start | cancel | finish 。\n");
	}
	me = this_player();
	if( !player ) player = me->query("id");
	
	else if( player != me->query("id") ){
		ob = LOGIN_D->find_body(player);
		if( objectp(ob) && environment(ob) == this_object() && ob->query_idle(ob) < 180 ) 
			return notify_fail("现在正由"+ob->name()+"在玩呢，你先等等。\n");
		else {
			clean_table();
			player = me->query("id");
		}
	}	
	
	// mine cancel
	if( arg == "cancel" ){
		clean_table();
		tmp += "：游戏已经清理干净。\n";	
	}
	else if( arg == "start" ){
		if( game )
			return notify_fail("游戏已经开始了，用 mine <dig|at|re x?y?> 游戏或 mine cancel 清理。\n");
		clean_table();
		player = me->query("id");
		game = make_game(bomb);
		t = time();
		if( wizardp(me) && me->query("env/mine") )
			tmp += drew_map(1);
		else 	tmp += drew_map(0);
	}
	
	// mine finish
	else if( arg == "finish" ){
		if( !game )
			return notify_fail("游戏还没开始呢，用 mine start 开始。\n");
		if( mine < bomb || real_mine < bomb || real_mine < mine )
			return notify_fail("你还没有完全探测完呢，继续吧！\n");
		tmp += drew_map(0);
		tmp += "探明的正确数："+ real_mine +"，";
		t = time() - t;
		tmp += player+"成功地完成了这次任务！，总共使用了"+ CHINESE_D->chinese_time(t) +"。\n";
		k = sizeof(top10);
		for (i = 0; i < k; i++){
			if( i > 9 ) break;
			if( t < top10[i]["time"] ){
				top10 = top10[0..i-1] + ({ ([ "id": me->query("id"), "time":t, ]) }) + top10[i..k];
				me->add("balance", (10-i) * 500000 );  // base reward is 50 gold.
				tmp += "\n此次成绩为目前排名第" + chinese_number(i+1) + "名！奖励"+MONEY_D->money_str((10-i) * 500000)+"存款！\n";
				break;
			}
		}
		clean_table();
	}
	
	// mine dig xxxx
	else if( sscanf(arg, "dig x%dy%d", i, j) == 2 ){
		if( !game )
			return notify_fail("游戏还没开始呢，用 mine start 开始。\n");
		if( i > x || i < 1 ) 
			return notify_fail("X值太大或太小了。\n");
		if( j > y || j < 1 ) 
			return notify_fail("Y值太大或太小了。\n");
		tmp += "选择在 X"+i+"，Y"+j+" 的地方挖掘。\n";
		i--;
		j--;
		if( i < 0 ) i = 0;
		if( j < 0 ) j = 0;
		if( map[i][j] > 10 ) 
			return notify_fail("此地不可点，除非你先取消标志。\n");
		k = game[i][j];
		if( k < 9 ){
			block_check(i, j);
			if( k == 0 )
				k = 10;
		}
		map[i][j] = k;
		tmp += drew_map(0);
		if( map[i][j] == 9 ){
			t = time() - t;
			tmp += HIR"进行了"+ CHINESE_D->chinese_time(t) +"后踩上了地雷！游戏结束！\n"NOR;
			tmp += "扫雷的正确数："+ real_mine +"。\n";
			clean_table();
		}
	}
	
	// mine at xxxx
	else if( sscanf(arg, "at x%dy%d", i, j) == 2 ){
		if( !game )
			return notify_fail("游戏还没开始呢，用 mine start 开始。\n");
		if( i > x || i < 1 ) 
			return notify_fail("X值太大或太小了。\n");
		if( j > y || j < 1 ) 
			return notify_fail("Y值太大或太小了。\n");
		tmp += "选择在 X"+i+"，Y"+j+" 的地方作标志。\n";
		i--;
		j--;
		if( i < 0 ) i = 0;
		if( j < 0 ) j = 0;
		if( map[i][j] == 11 )
			return notify_fail("此地已经探过并做好标志了，要取消请用 mine re x?y?。\n");
		map[i][j] = 11;
		mine++;
		if( game[i][j] == 9 )
			real_mine++;
		tmp += drew_map(0);
	}
	
	// mine re xxxx
	else if( sscanf(arg, "re x%dy%d", i, j) == 2 ){
		if( !game )
			return notify_fail("游戏还没开始呢，用 mine start 开始。\n");
		if( i > x || i < 1 ) 
			return notify_fail("X值太大或太小了。\n");
		if( j > y || j < 1 ) 
			return notify_fail("Y值太大或太小了。\n");
		tmp += "选择在 X"+i+"，Y"+j+" 的地方取消标志。\n";
		i--;
		j--;
		if( i < 0 ) i = 0;
		if( j < 0 ) j = 0;
		if( map[i][j] != 11 )
			return notify_fail("差点地并无雷标，无法取消。\n");
		map[i][j] = 0;
		mine--;
		if( game[i][j] == 9 )
			real_mine--;
		tmp += drew_map(0);
	}
	else return notify_fail("指令格式：mine <dig|at|re x?y?> | start | cancel | finish 。\n");
	tell_room(this_object(), tmp+"\n");
	return 1;
}

int valid_leave(object me, string dir)
{
  	if( me->query("id") == player )
               return notify_fail("你正在游戏！如要离开，请先打 mine cancel \n");

      	return ::valid_leave(me, dir);
} 
