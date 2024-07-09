// map.c
// Created by mxzhao 2003/11/28

/*********************************************
// 地图示例
<area>
<id>changle</id>
<name>长乐帮</name>
<byname>长乐,cl</byname>
<map>
─────────────────────────────────────────────────────────

                                      卧室  甬道─石室
                                       |     |
                               睡房   小厅─花园─八角亭
                                |      |
         ┏━━━━━━━┓  西走廊─走廊─东走廊─东走廊
         ┃【扬州】大驿道┃    |      |      |       |
         ┗━━━━━━━┛   柴房   大厅   小屋    厨房
                    \                  |
                  大驿道              大门
                      \                |
                     青石路─────青石路

─────────────────────────────────────────────────────────
</map>
<npc place=大门>邱山风</npc>
<npc place=走廊>陈冲之</npc>
<npc place=小厅>贝海石,米横野</npc>
<npc place=石室>展飞</npc>
</area>
*********************************************/
inherit F_SAVE;
inherit F_CLEAN_UP;

#include <ansi.h>
#include <mudlib.h>
#include <get_place.h>



#define TAG_AREA			"area"
#define TAG_ID				"id"
#define TAG_NAME			"name"
#define TAG_BYNAME			"byname"
#define TAG_MAP				"map"
#define TAG_NPC				"npc"
#define KEY_PLACE			"place"

#define NEXT				"next"
#define MAP_FILE			DATA_DIR + "map"

#define MAP_TITLE(name)		HIY "【" + name + "地图】" NOR  "其中"HBRED+HIY"■■■"NOR"表示你所在房间位置，" + HBBLU + HIW +"■■■"NOR"表示目标Npc位置"
#define NPC_TITLE(name)		HIY "【" + name + "主要NPC如下】" NOR "\n"

#define MAP_LINE			repeat_string("━", 40) + "\n" 

#define THIS_ROOM_COLOR		HBRED + HIY
#define TARGET_ROOM_COLOR	HBBLU + HIW

#define LEGAL_COPYRIGHT		"\n	Copyright 1997-2003 ShuJian MUD Wizard Group.\n"

mapping maps;

int help(object me);

string query_save_file()
{
	return MAP_FILE;
}

private string color_filter(string content)
{
	if ( !content )
	{
		return "";
	}
	
	{
		mapping color_map = ([
			// Foreground Colors
			"$BLK$": BLK,		// Black
			"$RED$": RED, 		// Red 
			"$GRN$": GRN, 		// Green
			"$YEL$": YEL, 		// Yellow
			"$BLU$": BLU, 		// Blue  
			"$MAG$": MAG, 		// Magenta
			"$CYN$": CYN, 		// Cyan 
			"$WHT$": WHT, 		// White
			
			// Hi Intensity Foreground Colors
			"$HIR$": HIR, 		// Red
			"$HIG$": HIG, 		// Green
			"$HIY$": HIY, 		// Yellow
			"$HIB$": HIB, 		// Blue
			"$HIM$": HIM, 		// Magenta
			"$HIC$": HIC, 		// Cyan 
			"$HIW$": HIW, 		// White
			
			// Background Colors  
			"$BBLK$": BBLK, 	// Black
			"$BRED$": BRED, 	// Red
			"$BGRN$": BGRN, 	// Green
			"$BYEL$": BYEL, 	// Yellow
			"$BBLU$": BBLU, 	// Blue
			"$BMAG$": BMAG, 	// Magenta
			"$BCYN$": BCYN, 	// Cyan
			
			// Hi Intensity Background Colors  
			"$HBRED$": HBRED, 	// Red
			"$HBGRN$": HBGRN, 	// Green
			"$HBYEL$": HBYEL, 	// Yellow
			"$HBBLU$": HBBLU, 	// Blue
			"$HBMAG$": HBMAG, 	// Magenta
			"$HBCYN$": HBCYN, 	// Cyan
			
			// Puts everything back to normal
			"$NOR$": NOR, 		
			
			// Additional  
			"$U$": U, 			// Initialize underscore mode
			"$BLINK$": BLINK, 	// Initialize blink mode
			"$REV$": REV, 		// Turns reverse video mode on
			"$HIREV$": HIREV, 	// Hi intensity reverse video 
			"$BOLD$": BOLD, 	// Turn on bold mode
			]);
		
		foreach ( string color in keys(color_map) )
		{
			content = replace_string(content, color, color_map[color]);
		}
	}

	return content;
}

private int load_map()
{
	if ( !mapp(maps) )
	{
		restore();
	}
	
	if( !mapp(maps) || sizeof(maps) == 0 )
	{
		maps = ([]);
	}

	return 0;
}

// 清除字符串内特定字符
private string trim_string(string str)
{
	string *chs = ({" ", "\t", "\n"});

	foreach ( string ch in chs )
	{
		str = replace_string(str, ch, "");
	}

	return str;
}

// 从字符串中取得指定内容
private varargs mapping get_string(string str, string tag, string key)
{
	mapping result = ([]);
	string *substrs = ({ "", "", "" });

	if ( !str || sizeof(str) == 0 
		|| !tag || sizeof(tag) == 0 )
	{
		return 0;
	}

	if ( sscanf(str, "%*s<" + tag + "%s>%s</" + tag + ">%s", 
		substrs[0], substrs[1], substrs[2] ) != 4 )
	{
		return 0;
	}

	if ( !key || sizeof(key) == 0 )
	{
		if ( sizeof(trim_string(substrs[0])) != 0 )
		{
			return 0;
		}

		result[tag] = substrs[1];
	}
	else
	{
		foreach ( string kv in explode(substrs[0], " ") )
		{
			string value;
			if (sscanf(trim_string(kv), key + "%s=%s", kv, value) == 2)
			{
				if ( sizeof(trim_string(kv)) != 0 )
				{
					return 0;
				}

				result[tag] = ([ value:substrs[1] ]);

				break;
			}
		}

		if ( undefinedp(result[tag]) )
		{
			return 0;
		}
	}

	result[NEXT] = substrs[2];

	return result;
}

// 建立地图文件
int build_maps(object me, string arg)
{
	mapping temp;
	string str;
	int count = 0;

	if ( !arg )
	{
		return help(me);
	}

	if ( !wizardp(me) || wiz_level(me) < 4 )
	{
		return help(me);
	}
	
	str = read_file(resolve_path(me->query("cwd"), arg));
	
	if ( !str )
	{
		write("没有这个文件。\n");
		return 1;
	}
	
	while ( temp=get_string(str, TAG_AREA) )
	{
		mapping map = ([]);
		mapping temp1;
		mapping temp2;
		
		str = temp[TAG_AREA];
		
		if ( temp1 = get_string(str, TAG_NAME) )
		{
			map[TAG_NAME] = temp1[TAG_NAME];
		}
		
		if ( temp1 = get_string(str, TAG_BYNAME) )
		{
			map[TAG_BYNAME] = explode(trim_string(temp1[TAG_BYNAME]), ",");
		}
		
		if ( temp1 = get_string(str, TAG_MAP) )
		{
			map[TAG_MAP] = temp1[TAG_MAP];
		}
		else
		{
			continue;
		}
		
		map[TAG_NPC] = ([]);

		while ( temp2 = get_string(str, TAG_NPC, KEY_PLACE) )
		{
			string place = keys(temp2[TAG_NPC])[0];

			if ( !place )
			{
				continue;
			}

			if ( undefinedp(map[TAG_NPC][place]) )
			{
				map[TAG_NPC][place] = ({});
			}

			map[TAG_NPC][place] += explode(trim_string(temp2[TAG_NPC][place]), ",");
			
			str = temp2[NEXT];
		}
		
		maps[get_string(temp[TAG_AREA], TAG_ID)[TAG_ID]] = map;

		str = temp[NEXT];

		count++;
	}

	save();

	write(sprintf("%d幅地图创建成功。\n", count));
	
	return 1;
}

// 目前提供的地图列表
int do_map_list(object me)
{
	string str = "";
	int i = 0;

	load_map();

	if ( !mapp(maps) || sizeof(maps) == 0 )
	{
		write(CHINESE_MUD_NAME + "目前没有提供任何地图。\n");
		return 1;
	}

	foreach ( string map in sort_array(keys(maps), 0) )
	{
		i++;

		str += sprintf("%-20s%s", maps[map][TAG_NAME] + "(" + map + ")", (i%4==0)?"\n":"");
	}

	write (CHINESE_MUD_NAME + "目前提供的地图如下：\n" + str + "\n");

	return 1;
}

// 指定区域的NPC列表
int do_npc_list(object me, string area)
{
	string str = "";

	load_map();

	if ( !mapp(maps) || sizeof(maps) == 0 )
	{
		write(CHINESE_MUD_NAME + "目前没有提供任何地图。\n");
		return 1;
	}

	foreach ( string place in sort_array(keys(maps[area][TAG_NPC]), 0) )
	{
		str += sprintf("  %-12s：%-s\n", place, 
			implode(maps[area][TAG_NPC][place], "、"));
	}
	
	if ( sizeof(str) > 0 )
	{
		str = NPC_TITLE(maps[area][TAG_NAME]) + MAP_LINE + str + MAP_LINE;
		
		if (me->query("env/fastread") && sizeof(str) <= 8192)
		{
			write(str + "\n");
		}
		else
		{
			me->start_more(str + "\n");
		}
	}
	
	return 1;
}

void create()
{
	seteuid(getuid());

	load_map();
}

int main(object me, string arg)
{
	string area;
	string target;
	string file;
	string myarea;
	string str;
	string *paths;

	int npclist = 0;

	if ( me != this_player(1))
	{
		return 0;
	}

	if( !arg || arg=="" )
	{		
		return help(me);
	}
	else if ( arg == "-l" )
	{
		return do_map_list(me);
	}
	else if ( arg == "-t" )
	{
		area = "here";
		npclist = 1;
	}
	else if ( sscanf(arg, "%s -t %s", area, target)==2 )
	{
	}
	else if ( sscanf(arg, "%s -t", area)==1 )
	{
		npclist = 1;
	}
	else if ( sscanf(arg, "-t %s", target)==1 )
	{
	}
	else if ( sscanf(arg, "-update %s", file)==1 )
	{
		return build_maps(me, file);
	}
	else
	{
		if ( !area ) 
		{
			area = arg;
		}
	}
	
	if ( !area )
	{
		area = "here";
	}
	
	paths = explode(base_name(environment(me)), "/");

	if (sizeof(paths) > 1)
	{
		myarea = paths[sizeof(paths)-2];
	}

	if ( area == "here" )
	{
		area = myarea;
	}
	
	if ( undefinedp(maps[area]) )
	{
		foreach ( string map in keys(maps) )
		{
			if ( maps[map][TAG_NAME] == area 
				|| member_array(area, maps[map][TAG_BYNAME]) >=0 )
			{
				area = map;
				break;
			}
		}
		
		if ( undefinedp(maps[area]) )
		{
			return notify_fail(CHINESE_MUD_NAME + "目前没有提供此区域的地图。\n");
		}
	}
	
	if (npclist)
	{
		return do_npc_list(me, area);
	}
	
	if ( undefinedp(maps[area][TAG_MAP]) )
	{
		return notify_fail(CHINESE_MUD_NAME + "此区域的地图异常。\n");
	}
	
	str = maps[area][TAG_MAP];
	
	if ( area == myarea )
	{
		string roomname = strip(environment(me)->query("short"));
		str = replace_string(str, roomname, THIS_ROOM_COLOR + roomname + NOR);
	}
	
	if ( target && sizeof(target) > 0)
	{
		foreach ( string place in keys(maps[area][TAG_NPC]) )
		{
			if ( member_array(target, maps[area][TAG_NPC][place]) >=0 )
			{
				str = replace_string(str, place, TARGET_ROOM_COLOR + place + NOR);
				break;
			}
		}

		str = replace_string(str, target, TARGET_ROOM_COLOR + target + NOR);
	}
	
	str = MAP_TITLE(maps[area][TAG_NAME]) + str;
	
	if (me->query("env/fastread") && sizeof(str) <= 8192)
	{
		write(str + "\n");
	}
	else
	{
		me->start_more(str + "\n");
	}
	
	return 1;
}

int help(object me)
{
	string help = @HELP
指令说明：
	这个指令可以让你查看所在地域的地图和NPC情况。

指令格式：map [地区名| here] [-t [目标名]> | -l]

	map                     显示当前所在区域的地图
	map -l                  显示所有地图列表
	map [here] -t           显示当前所在区域的NPC列表
	map [here] -t 目标名    高亮显示当前区域内此目标房间
	map 地区名              显示指定区域的地图
	map 地区名 -t           显示指定区域的NPC列表
	map 地区名 -t 目标名    高亮显示指定区域内此目标房间

	地区名可用 map -l 查看，目标名任意

HELP;

	help += "\t" THIS_ROOM_COLOR "当前所在房间" NOR "\t" TARGET_ROOM_COLOR "目标房间" NOR "\n\n";
	if ( wizardp(me) )
	{
		help += @HELP
	map -update 文件名      用文件里的地图信息更新地图
HELP;
	}

	help += LEGAL_COPYRIGHT;

	write(help);

    return 1;
}
