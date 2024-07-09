// gcreate.c  开帮
// by augx@sj 4/11/2002

// Modified by mxzhao 2004/02/18

#include <ansi.h>
inherit F_CLEAN_UP;
#include <mudlib.h>
#include <group.h>

static string * valid_group_name = ({ "帮","会","门","派","楼","山","教","岛","宫","盟","谷","馆","观","院","堡","园","店",
				      "坊","阁","堂","舵","洞","寺","源","庙","庄","族","团", "府", "寨", "组","城" });				      
static string * invalid_group_name = ({ "丐帮","武当派","少林派" });
static string * invalid_group_id = ({ "obj","npc","entry" });

int help(object me)
{
	write(@HELP
指令格式 :
gcreate [帮派中文名(二至六个汉字)] [帮派英文表示(3到8个英文字母)]
        [首脑的称谓一] [首脑的称谓二] [首脑的称谓三] [帮众的称谓]

这个指令让你开设自己的帮派。帮派中文名和首领的称谓中可以带有色彩字串，
具体加颜色的方式和nick命令相同，请参见help nick。
帮派中文名的最后一个字，必须是下列文字之一：
    帮、会、门、派、楼、山、教、盟、岛、宫、谷、馆、庄、团、府、店、
    观、院、堡、园、坊、阁、堂、舵、洞、寺、源、庙、族、组、寨、城。
帮派名称、英文表示、首领称谓中请勿使用不当(政治性、淫秽)，或可能引起混
淆(金庸小说中已有的门派)的文字。否则将删除此帮派，并根据情节轻重给予相
应处罚。首领的称谓就是指“帮主”，“副帮主”......

例如：gcreate $YEL$斧头帮 futou 帮主 副帮主 堂主 帮众
      gcreate $MAG$风雨$GRN$楼 fengyu $MAG$楼主 副楼主 香主 弟子

HELP
	);
	return 1;
}

int main(object me, string arg)
{
	string gname,gid,gname2,title1,title2,title3,title4;
	int i;
//	mixed mysql;

	if(!arg || (sscanf(arg,"%s %s %s %s %s %s",gname,gid,title1,title2,title3,title4)<6) )
		return help(me);
		
	if( me->query("registered") < 3 )
		return notify_fail("抱歉，当前只允许贵宾玩家才能创建帮派，如何加入贵宾请 "HIG"help vip"NOR" 进行浏览。\n");

	if( me->query("combat_exp") < EXP_REQUIRE )
		return notify_fail("你的资历太浅，恐怕难以组织起自己的帮派。\n");
	if( me->query("group/class") == 1 )
		return notify_fail("你已经组织了自己的帮派。\n");
	if( me->query("group/id") )
		return notify_fail("你已经加入了别的帮派。\n");

	gname = GROUP_D->color_string(gname);
	title1 = GROUP_D->color_string(title1);
	title2 = GROUP_D->color_string(title2);
	title3 = GROUP_D->color_string(title3);
	title4 = GROUP_D->color_string(title4);

	// group name check
	gname2 = strip(gname);
	if( (strlen(gname2)%2)==1 || strlen(gname2)<4 || strlen(gname2)>12 || strlen(gid)<4 || strlen(gid)>8 )
		return notify_fail("帮派中文名或英文标识长度不符合要求，请参见help group。\n");

	for(i=0;i<sizeof(valid_group_name);i++)
		if( gname2[<2..<1] == valid_group_name[i] ) break;
	if( i >= sizeof(valid_group_name) )
		return notify_fail("帮派中文名最后一个字不符合要求，参见help group。\n");

	for(i=0;i<sizeof(invalid_group_name);i++)
		if( gname2 == invalid_group_name[i] ) break;
	if( i < sizeof(invalid_group_name) )
		return notify_fail("帮派名称不符合要求，参见help group。\n");

	// group id check
	for(i=0;i<sizeof(invalid_group_id);i++)
		if( gid == invalid_group_id[i] ) break;
	if( i < sizeof(invalid_group_id) )
		return notify_fail("帮派英文标识不符合要求，参见help group。\n");

// Modified by mxzhao 2004/02/18
	{
		mixed ret = dbquery("select id from mud_group where id='" 
			+ gid + "' or name='" + gname + "'");

		if (!ret)
		{
			write("数据库连接失败。\n");
			return 0;
		}

		if (sizeof(ret) > 0)
		{
			return notify_fail("帮派中文名或英文标识和现有帮派重复！\n");		
		}

		ret = dbquery("insert into mud_group set "
			"name='" + gname + "',"
			"id='" + gid + "',"
			"master='" + me->name() 
			+ "(" + me->query("id") + ")" + "',"
			"members=" + "1" + ","
			"score=" + "1000,"
			"power=" + GROUP_D->query_member_power(me) + ","
			"site='" + INTERMUD_MUD_NAME + "'");
		
		if (!ret)
		{
			write("数据库连接失败。\n");
			return 0;
		}
	}
/*
	mysql = db_connect("localhost","mud","root");
	if( !intp(mysql) ) return notify_fail("无法连接数据库，请稍后再试。\n");
	if( db_exec(mysql,"select id from " + INTERMUD_MUD_NAME + "_mud_group where id='"+gid+"' or name='"+gname+"'") > 0 ) {
		db_close(mysql);
		return notify_fail("帮派中文名或英文标识和现有帮派重复！\n");
	}
	db_exec(mysql,"insert into mud_group set "+
		"name='"	+gname					+"',"+
		"id='"		+gid					+"',"+
		"master='"	+me->name()+"("+me->query("id")+")"	+"',"+
		"members="	+"1"					+","+
		"score="	+"1000"					+","+
		"power="	+GROUP_D->query_member_power(me)	+","+
		"site='"	+INTERMUD_MUD_NAME 			+"'"
	);
	db_close(mysql);
*/
// End

	me->set("group/id",gid);
	me->set("group/class",1);
	me->save();

	mkdir("/data/npc/group/"+gid);
	GROUP_D->create_group(me,gid,gname,title1,title2,title3,title4);

	tell_object(me,sprintf("你建立了自己的帮派：%s(%s)。\n",gname,gid));
	CHANNEL_D->do_channel(me,"chat",HIY+RANK_D->query_self(me)+"今日开创"+gname+HIY+"("+gid+")，希望各位江湖朋友多多捧场。"NOR);
	log_file("group/group",sprintf("create :%-12s:%-8s:%-8i:%s(%s)",gname,gid,me->query("combat_exp"),me->name(),me->query("id")),me);

	return 1;
}
