/*把所有东西都进入数据库供玩家查询
ROOM
filename short long npc cmds code
NPC
filename name id skills inv room
 
*/

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object);   
void QueryNpcInfo(string path,int flag);
void QueryRoomInfo(string);
void QueryItemInfo(string);
string LoadNpc(object);
string QuerySkill(object);
string QueryObj(object);
string QueryAction(object,object);
string QueryEquipProp(object,mapping);

string status_color(int current, int max);
string conv(int i)
{
	if (i < 1000) return ""+i;
	return conv(i/1000)+sprintf(",%03d", i%1000);
}
int main(object me,string arg)
{
    
    string path,var;
    string *filename;

    if( !arg || sscanf(arg,"%s %s",path,var) != 2){
        return help(me);
    }
    if(file_size(path = resolve_path(me->query("cwd"),path)) != -2 )
        return notify_fail("没有这个目录！\n");
    seteuid(getuid());
    if(path[sizeof(path)-1] != '/')
        path += "/";
    switch(var)
    {
        case "-npc":
        case "-NPC":
            QueryNpcInfo(path,1);
            break;
        case "-room":
        case "-ROOM":
            QueryRoomInfo(path);
            break;
        case "-item":
        case "-ITEM":
            QueryItemInfo(path);
            break;
        default:
            write("参数不对，请重新输入！\n");
            break;
    }
    return 1;
}
void QueryNpcInfo(string path,int flag)
{
    mapping my;
    string filename;
    string *file;
    object obj,me=this_player();
    int num=0;
    int lmt, i;
    if(flag){
    filename=path+"文档归类之NPC篇";
    
    if(file_size(filename)>0)
        rm(filename);
    file=get_dir(path);

    foreach(string tmp in file){
        if (file_size(tmp) == -2)
            continue;
        write(sprintf("  整理（%-15s）中...",tmp));
        reset_eval_cost();
        obj=new(path + tmp);
        if (!objectp(obj) ){
            write(" ...失败！\n");
            continue;
        }
        if(!obj->is_character() || !obj->query("name")){
            write("NOT NPC！\n");
            continue;
        }
        num ++;
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
    write_file(filename,"文件序号："+num+"\n",0);
    write_file(filename,"文件位置："+base_name(obj)+".c\n",0);
    write_file(filename,"文件名称："+tmp+"\n",0);
    write_file(filename,"中文名字："+obj->query("name")+"\n",0);
    write_file(filename,"英文ＩＤ: " + sprintf("%s",implode(obj->parse_command_id_list(), ","))+"\n",0);
    write_file(filename,QuerySkill(obj),0);
    write_file(filename,QueryObj(obj),0);
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	my = obj->query_entire_dbase();

	//if (userp(obj)){
		lmt = my["combat_exp"];
		for (i = to_int(pow(10.0 * lmt, 0.3333333));to_int(i*i/10.0*i)<=lmt;i++);
		my["max_pot"] = i+100;
	//}

	if (!my["max_jing"]) my["max_jing"] = 1;
	if (!my["max_qi"]) my["max_qi"] = 1;
	write_file(filename,sprintf("·精血·%s%5d / %5d %s(%3d%%)" NOR "  ·精力·%s%5d / %5d%s(%d)\n" NOR,
		status_color(my["jing"], my["eff_jing"]+obj->query_temp("apply/jing")), my["jing"], my["eff_jing"]+obj->query_temp("apply/jing"),
		status_color(my["eff_jing"]+obj->query_temp("apply/jing"), my["max_jing"]), ((my["eff_jing"]+obj->query_temp("apply/jing")) * 100 / my["max_jing"]),
		status_color(my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli")), my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli"),
		status_color(my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli")), my["eff_jingli"]-my["max_jingli"]+(obj->query_skill("force")-obj->query_temp("apply/force"))*8+obj->query_temp("apply/jingli")));
	write_file(filename,sprintf("·气血·%s%5d / %5d %s(%3d%%)" NOR "  ·内力·%s%5d / %5d(+%d)\n" NOR,
		status_color(my["qi"], my["eff_qi"]+obj->query_temp("apply/qi")), my["qi"], my["eff_qi"]+obj->query_temp("apply/qi"),
		status_color(my["eff_qi"]+obj->query_temp("apply/qi"), my["max_qi"]), ((my["eff_qi"]+obj->query_temp("apply/qi")) * 100 / my["max_qi"]),
		status_color(my["neili"], my["max_neili"]+obj->query_temp("apply/neili")), my["neili"], my["max_neili"]+obj->query_temp("apply/neili"),
		my["jiali"] ));

	write_file(filename,sprintf("·%s气·%s %-13s   " NOR " ·内力上限·"HIC"%5d / %5d\n" NOR,
		my["shen"] < 0?"戾":"正",
		my["shen"] < 0?HIR:HIC,
		conv((my["shen"] < 0?-1:1) * my["shen"]),
		(obj->query_skill("force")-obj->query_temp("apply/force"))*8 + my["combat_exp"]/1000 + obj->query_temp("apply/neili"),
		(obj->query_skill("force")-obj->query_temp("apply/force"))*10 + my["age"]*20 + my["combat_exp"]/1000 + obj->query_temp("apply/neili")));

	i = obj->max_food_capacity();
	if (!i) i = 1;
	write_file(filename,sprintf("·食物·%s%7.2f%%         " NOR "     ·潜能·"HIY" %4d / %4d\n" NOR,
		status_color(my["food"], i),
		my["food"]*100.0/i, my["potential"], my["max_pot"] ));
	i = obj->max_water_capacity();
	if (!i) i = 1;
	write_file(filename,sprintf("·饮水·%s%7.2f%%         " NOR "     ·经验·"HIM" %s\n"NOR,
		status_color(my["water"], i),
		my["water"]*100.0/i,
		conv(obj->query("id") == "zhong shentong"?my["combat_exp"]/3*2:my["combat_exp"])));
        destruct(obj);
        write(" ...成功。 \n");
    }
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
    write_file(filename,"    本目录下总共有"+num+"个NPC文件！\n",0);
    write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
    write("      共整理（"+chinese_number(num)+"）个NPC文件   \n\n");
       me->start_more(read_file(filename,1));
    }
    else{
    	obj=new(path);
        if (!objectp(obj) ){
            write(" ...失败！\n");
            return;
        }
        if(!obj->is_character() || !obj->query("name")){
            write("NOT NPC！\n");
            return;
        }
        num ++;
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
    write_file(filename,"文件序号："+num+"\n",0);
    write_file(filename,"文件位置："+base_name(obj)+".c\n",0);
    write_file(filename,"文件名称："+path+"\n",0);
    write_file(filename,"中文名字："+obj->query("name")+"\n",0);
    write_file(filename,"英文ＩＤ: " + sprintf("%s",implode(obj->parse_command_id_list(), ","))+"\n",0);
    write_file(filename,QuerySkill(obj),0);
    write_file(filename,QueryObj(obj),0);
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	my = obj->query_entire_dbase();

	//if (userp(obj)){
		lmt = my["combat_exp"];
		for (i = to_int(pow(10.0 * lmt, 0.3333333));to_int(i*i/10.0*i)<=lmt;i++);
		my["max_pot"] = i+100;
	//}

	if (!my["max_jing"]) my["max_jing"] = 1;
	if (!my["max_qi"]) my["max_qi"] = 1;
	write_file(filename,sprintf("·精血·%s%5d / %5d %s(%3d%%)" NOR "  ·精力·%s%5d / %5d%s(%d)\n" NOR,
		status_color(my["jing"], my["eff_jing"]+obj->query_temp("apply/jing")), my["jing"], my["eff_jing"]+obj->query_temp("apply/jing"),
		status_color(my["eff_jing"]+obj->query_temp("apply/jing"), my["max_jing"]), ((my["eff_jing"]+obj->query_temp("apply/jing")) * 100 / my["max_jing"]),
		status_color(my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli")), my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli"),
		status_color(my["jingli"], my["eff_jingli"]+obj->query_temp("apply/jingli")), my["eff_jingli"]-my["max_jingli"]+(obj->query_skill("force")-obj->query_temp("apply/force"))*8+obj->query_temp("apply/jingli")));
	write_file(filename,sprintf("·气血·%s%5d / %5d %s(%3d%%)" NOR "  ·内力·%s%5d / %5d(+%d)\n" NOR,
		status_color(my["qi"], my["eff_qi"]+obj->query_temp("apply/qi")), my["qi"], my["eff_qi"]+obj->query_temp("apply/qi"),
		status_color(my["eff_qi"]+obj->query_temp("apply/qi"), my["max_qi"]), ((my["eff_qi"]+obj->query_temp("apply/qi")) * 100 / my["max_qi"]),
		status_color(my["neili"], my["max_neili"]+obj->query_temp("apply/neili")), my["neili"], my["max_neili"]+obj->query_temp("apply/neili"),
		my["jiali"] ));

	write_file(filename,sprintf("·%s气·%s %-13s   " NOR " ·内力上限·"HIC"%5d / %5d\n" NOR,
		my["shen"] < 0?"戾":"正",
		my["shen"] < 0?HIR:HIC,
		conv((my["shen"] < 0?-1:1) * my["shen"]),
		(obj->query_skill("force")-obj->query_temp("apply/force"))*8 + my["combat_exp"]/1000 + obj->query_temp("apply/neili"),
		(obj->query_skill("force")-obj->query_temp("apply/force"))*10 + my["age"]*20 + my["combat_exp"]/1000 + obj->query_temp("apply/neili")));

	i = obj->max_food_capacity();
	if (!i) i = 1;
	write_file(filename,sprintf("·食物·%s%7.2f%%         " NOR "     ·潜能·"HIY" %4d / %4d\n" NOR,
		status_color(my["food"], i),
		my["food"]*100.0/i, my["potential"], my["max_pot"] ));
	i = obj->max_water_capacity();
	if (!i) i = 1;
	write_file(filename,sprintf("·饮水·%s%7.2f%%         " NOR "     ·经验·"HIM" %s\n"NOR,
		status_color(my["water"], i),
		my["water"]*100.0/i,
		conv(obj->query("id") == "zhong shentong"?my["combat_exp"]/3*2:my["combat_exp"])));
        destruct(obj);
        write(" ...成功。 \n");

    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
    write_file(filename,"    本目录下总共有"+num+"个NPC文件！\n",0);
    write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
    write("      共整理（"+chinese_number(num)+"）个NPC文件   \n\n");
       me->start_more(read_file(filename,1));
    }
    
}
void QueryRoomInfo(string path)
{
    string filename;
    string *file;
    object obj;
    object finder=new("/clone/npc/trashcan");
       object me=this_player();
    int num=0;
    filename=path+"文档归类之ROOM篇";
    
    if(file_size(filename)>0)
        rm(filename);
    file=get_dir(path);
    foreach(string tmp in file){
        if (file_size(path+tmp) == -2)
            continue;
        write(sprintf("  整理（%-15s）中...",tmp));
        reset_eval_cost();
        obj=load_object(path+tmp);
        if (!objectp(obj) ){
            write(" ...失败！\n");
            continue;
        }
        if( obj->is_character() || !stringp(obj->query("short")) ){
            write("NOT ROOM\n");
            continue;
        }
        num ++;
        write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
        write_file(filename,"房间序号："+num+"\n",0);
        write_file(filename,"房间名称："+obj->query("short")+"\n",0);
        write_file(filename,"文件位置："+base_name(obj)+".c\n",0);
        write_file(filename,"房间描述：\n"+obj->query("long")+"\n",0);
        write_file(filename,LoadNpc(obj),0);
        write_file(filename,QueryAction(obj,finder),0);
        
        write(" ...成功\n");
    }
    write_file(filename,"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n",0);
    write_file(filename,"    本目录下总共有"+num+"个房间！\n",0);
    write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
    write("      共整理（"+chinese_number(num)+"）个房间文件   \n\n");
       me->start_more(read_file(filename,1));
    destruct(finder);
}
void QueryItemInfo(string path)
{
    string filename;
    string *file;
    object obj;
    mapping prop;
       object me=this_player();    
    int num=0;
    int flag=0;
    filename=path+"文档归类之ITEM篇";
    
    if(file_size(filename)>0)
        rm(filename);
    file=get_dir(path);
    foreach(string tmp in file){
        if (file_size(path+tmp) == -2)
            continue;
        write(sprintf("  整理（%-15s）中...",tmp));
        reset_eval_cost();
        obj=new(path + tmp);
        if (!objectp(obj) ){
            write(" ...失败！\n");
            continue;
        }
        if( obj->is_character() || !stringp(obj->query("name")) ){
            write("NOT ITEM\n");
            continue;
        }
        num ++;
        write_file(filename,"─────────────────────────\n",0);
        write_file(filename,"物品序号："+num+"\n",0);
        write_file(filename,"物品名称："+obj->query("name")+"("+
                    implode(obj->parse_command_id_list(),",")+")\n",0);
        write_file(filename,"物品价格："+MONEY_D->price_str((int)obj->query("value"))+"\n",0);
/*
        if(mapp(prop=obj->query("weapon_prop")))
            QueryEquipProp(obj,prop);
        else if(mapp(prop=obj->query("armor_prop")))
            QueryEquipProp(obj,prop);
*/
        destruct(obj);
        write(" ...成功。 \n");
    }
    write_file(filename,"==================================================\n",0);
    write_file(filename,"    本目录下总共有"+num+"个物品！\n",0);
    write("=========================================\n");
    write("      共整理（"+chinese_number(num)+"）个物品文件   \n\n");
       me->start_more(read_file(filename,1));
}
string LoadNpc(object ob)
{

    string *p;
    mapping npc;
    string msg="",*str,name;
    int i=0,count;
    object tmpnpc;

    if (stringp(ob->query("short")))
        name=ob->query("short");
    else name=ob->query("name");

    npc = ob->query("objects");
    count = sizeof(npc);
    if ( count == 0) 
    {
    return (name+"没有任何NPC或者物品。\n");
    }
        
    p = sort_array(keys(npc), 1);

    msg += name +"场景有以下NPC或物品：\n";

    for (i=0; i < count; i++){
    	tmpnpc = new(p[i]);   	
    	msg += sprintf("%s%s\n",npc[p[i]]>1?(chinese_number(npc[p[i]])+"位"):"",tmpnpc->short(1));
    }
    return msg;
}

string QueryAction(object ob,object finder)
{
    mixed *cmds;
    string msg="",*str,name;
    int count=0;

    if (stringp(ob->query("short")))
        name=ob->query("short");
    else name=ob->query("name");

    finder->move(ob);
    cmds = finder->query_commands();
    if ( sizeof(cmds) == 1) return("");
    msg += name +"提供如下指令：\n";
    foreach(mixed cmd in cmds){
        if( cmd[2]==finder ) continue;
        msg+=sprintf("%-10s   ", cmd[0]);
        count++;
        if(count%4==0)
            msg+="\n";
    }
    msg+="\n";
    return msg;
}
string QuerySkill(object ob)
{
    mapping m_skills,m_enable;
    string *sname;
    int i, jibie, lingwu, jilei,xiuwei, opt_all;    
    string msg="";

    m_skills = ob->query_skills();
    if ( !sizeof(m_skills) )
        return(ob->query("name")+"没有学过任何技能。\n");
    msg += ob->name() +"目前所学过的技能：\n";
    m_enable = ob->query_skill_map();
    if ( !mapp(m_enable) ) m_enable = ([]);
    sname=keys(m_skills);
    msg=msg+"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n";
    for ( i = 0; i < sizeof( m_skills ); i++ ){
        msg=msg+sprintf( "%s%-12s%-28s%8d\n",
            m_enable[ sname[i] ] ? "□" : "  ",
            to_chinese(sname[i]),  "(" + sname[i] + ")",
            m_skills[sname[i]]);        
        }
    return msg;
}
string QueryObj(object ob)
{
    	object *inv;
	string *key, unit;
	mixed first;
	int i, w;
	string msg = "┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n";
	
    	inv = filter_array(all_inventory(ob), "visible", ob);
	if (!sizeof(inv)) {
		msg +=  ob->name() + "身上没有携带任何东西。\n";
		return msg;
	}

	msg += sprintf("%s身上带着%s件东西(负重 %:2f%%)：\n",
		ob->name(),
		chinese_number(sizeof(inv)),
		ob->query_encumbrance() * 100.0 / ob->query_max_encumbrance()
	);

	first = filter_array(inv,
		(: $1->query("base_unit") && !$1->query("equipped") && !$1->query("embedded") :)
	);
	first += filter_array(inv,
		(: $1->query("equipped") :)
	);
	first += filter_array(inv,
		(: $1->query("embedded") :)
	);

	foreach(object item in first) {
		unit = sprintf("%s",
			(item->query("equipped")? HIC"□"NOR :item->query("embedded")? HIR"☆"NOR: "  ")+
			item->short()
		);

		msg += unit + "\n";
	}

	inv -= first;
	i = sizeof(inv);


	first = ([]);
	foreach(object item in inv) {
		unit = base_name(item) + item->name();
		if( !mapp(first[unit]) ) {
			first[unit] = ([
				"total" : 1,
				"unit" : item->query("unit"),
				"short" : item->short(),
			]);
			//if ( wiz ) first[unit]["weight"] = item->weight();
		} else first[unit]["total"]++;
	}

	key = keys(first);

	foreach(string item in key) {
		unit = sprintf("  %s", chinese_number(first[item]["total"]) + first[item]["unit"] + first[item]["short"]);
		msg += unit + "\n";
	}
	//write(str);
	//if ( wiz ) write("总重量：" + ob->query_encumbrance() + "\n");
    return msg;
}

string QueryEquipProp(object equip,mixed props)
{
    string type,msg="";
    int *special_props,i;
    int count=0;
    string *prop;

    if(stringp(type=equip->query("skill_type")))
        msg+=sprintf("兵器类型：%-10s     攻 击 力：%d",type,(int)equip->query("weapon_porp/damage"));
    else if(stringp(type=equip->query("skill_type")))
        msg+=sprintf("防具类型：%-10s     防 御 力：%d",type,(int)equip->query("armor_porp/armor"));
    if ( !sizeof(props)) return("");
    msg += "          "+equip->name()+"的其他功效：\n";
    prop=keys(props);
    for (i=0;i<sizeof(props);i++){
// foreach(string prop in props){
        if( prop[i]=="armor"||prop[i]=="damage")
            continue;
        msg+=sprintf("%-12d  ", props[prop[i]]);
        if(i%4==0)
            msg+="\n";
    }
    return msg;
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
int help(object me)
{
    write(@HELP
指令格式：doc 路径名 -种类名 
目前提供的种类有：room、npc、item
例：
    doc /d/city/npc -npc
有玩家在线时最好不使用此指令。
HELP);
    return 1;
}