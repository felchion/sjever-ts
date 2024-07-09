// mudlist.c Yuj@SJ 1999-6-2

inherit F_DBASE;

#include <ansi.h>
#include <net/daemons.h>
#include <net/macros.h>
#include <net/config.h>
void create()
{
	seteuid(getuid());
}

int main(object me, string arg)
{
	mapping mud_list;
	mixed *muds;
	string output,port;
	int loop, size, count=0, total, n=0,i,j;

	if (!find_object(DNS_MASTER)) {
		write("网路精灵并没有被载入。\n");
		return 1;
	}

	//      Obtain mapping containing mud data
	mud_list = (mapping)DNS_MASTER->query_muds();

	if(!mud_list){ 
		write(MUD_NAME + "目前并没有跟网路上其他 Mud 取得联系。\n");
		return 1;
	}

	//      Get list of all mud names within name server
	muds = keys( mud_list ) - ({ "DEFAULT" });

	//      Place mudlist into alphabetical format
	muds = sort_array(muds, 1);

	//      If mudname provided, search server's database for a match
	//      and display its cooresponding address
	if (arg)
		arg = upper_case(arg);
	else
		arg = "ALL";
		
	if (arg && arg != "ALL") {
		arg = htonn(arg);
		if(!mapp( mud_list[arg] )) {
			write(MUD_NAME + "并没有和这个 Mud 取得联系。\n");
			return 1;
		}
		if (wizardp(me) && !me->query("env/debug"))
			write(sprintf("有关 %s 的资讯：\n%O\n", arg, mud_list[arg]));
		else
			write(sprintf("\n有关 %s 的资料：\n"
				"──────────────────\n"
				"中文名称：  %s\n"
				"语言：      %s\n"
				"版本：      %s\n"
				"MudOS：     %s\n"
				"Mudlib：    %s\n"
				"主机：      %s\n"
				"端口：      %s\n"
				"状态：      %s\n"
				"在线玩家：  %s\n"
				"运行时间：  %s\n"
				"──────────────────\n\n",
				mud_list[arg]["NAME"],
				undefinedp(mud_list[arg]["MUDNAME"])?"不详":mud_list[arg]["MUDNAME"],
				undefinedp(mud_list[arg]["ENCODING"])?"不详":(mud_list[arg]["ENCODING"]=="GB"?"简体中文":"繁体中文"),
				undefinedp(mud_list[arg]["VERSION"])?"不详":mud_list[arg]["VERSION"],
				undefinedp(mud_list[arg]["DRIVER"])?"不详":mud_list[arg]["DRIVER"],
				undefinedp(mud_list[arg]["MUDLIB"])?"不详":mud_list[arg]["MUDLIB"],
        (NAT_MAP[mud_list[arg]["HOSTADDRESS"]])?NAT_MAP[mud_list[arg]["HOSTADDRESS"]]:mud_list[arg]["HOSTADDRESS"],
				mud_list[arg]["PORT"],
				mud_list[arg]["_dns_no_contact"]<1?"连线":"断开",
				undefinedp(mud_list[arg]["USERS"])?"不详":mud_list[arg]["USERS"],
				undefinedp(mud_list[arg]["TIME"])?"不详":CHINESE_D->chinese_time(atoi(mud_list[arg]["TIME"])/600*600)
			));
		return 1;
	}

	output = "\n   中  文  名  称            运  行  时  间              网 络 地 址      端口  人数\n";
	output += repeat_string("─", 42) + "\n";
	//      Loop through mud list and store one by one
	for(loop = 0, size = sizeof(muds); loop<size; loop++) {
		if (!arg && mud_list[muds[loop]]["VERSION"] != MUDLIB_VERSION)
			continue;

		if(!wizardp(me) && mud_list[muds[loop]]["NAME"]=="cp") continue;
		if (arg) {
			if (arg !="ALL" && mud_list[muds[loop]]["MUDLIB"] != MUDLIB_NAME )
				continue;
		}

		if (mud_list[muds[loop]]["_dns_no_contact"] > 0) {
			if( !me || !wizardp(me) || (wizardp(me) && me->query("env/debug")) )
				continue;
		}

		if (mud_list[muds[loop]]["TIME"] != query(muds[loop])) {
			set(muds[loop], mud_list[muds[loop]]["TIME"]);
			set(muds[loop]+".Time", time());
		}

		mud_list[muds[loop]]["HOSTADDRESS"]= mud_list[muds[loop]]["HOSTADDRESS"];
		  		  		
		if (htonn(upper_case(muds[loop])) == htonn(upper_case(MUD_SITE_ID)))
			output +=  BRED+HIY;
 
		if(muds[loop]=="ts2")
		output += sprintf("%-20s%-5s   ",
			(undefinedp(mud_list[muds[loop]]["MUDNAME"])?"":mud_list[muds[loop]]["MUDNAME"]),
			"(" + upper_case(muds[loop]) +")",
		);
		else 
		output += sprintf("%-19s%-6s   ",
			(undefinedp(mud_list[muds[loop]]["MUDNAME"])?"":mud_list[muds[loop]]["MUDNAME"]),
			"(" + upper_case(muds[loop]) +")",
		);
		if (time() > query(muds[loop]+".Time") + 130)
			output += "<失去联系"+CHINESE_D->chinese_time((time()-query(muds[loop]+".Time"))/60*60)+">";
		else {
			total = atoi(mud_list[muds[loop]]["TIME"]);
			if(total>86400*7) total = total/3600*3600;
			else total = total/60*60;
			output += sprintf("%-25s", CHINESE_D->chinese_time(total));

		}
    
    i=sizeof(NAT_MAP[muds[loop]]);
    //write((upper_case(mud_list[muds[loop]]+"\n");
    if(i>1) 
  		output += sprintf("   %-15s  %4s  ","","");  	
    else if(i==1){
  		output += sprintf("   %-15s  %4s  ",
  			NAT_MAP[muds[loop]][0][0..<6],
  			NAT_MAP[muds[loop]][0][<4..<1]
  		);
    	//0?NAT_MAP[mud_list[muds[loop]]["HOSTADDRESS"]]:  	 
  	}
    else output += sprintf("   %-15s  %4s  ",
    	mud_list[muds[loop]]["HOSTADDRESS"],
			mud_list[muds[loop]]["PORT"]
		);
		
		
		if(!undefinedp(mud_list[muds[loop]]["USERS"])
		&& time() <= query(muds[loop]+".Time") + 130 ) {
				output+=sprintf("%4s", mud_list[muds[loop]]["USERS"] );
				count += atoi(mud_list[muds[loop]]["USERS"]);
		} else output+=" ----";		
		
		
		output +=NOR"\n";
		if(i>1) {
			for(j=0;j<i;j++) {				
				if (htonn(upper_case(muds[loop])) == htonn(upper_case(MUD_SITE_ID))
				&& sprintf("%d",query_ip_port(this_player())) ==NAT_MAP[muds[loop]][j][<4..<1])
				output +=  HBCYN+HIW;
			
				output += sprintf("%-20s%-6s",
					(undefinedp(mud_list[muds[loop]]["MUDNAME"])?"":mud_list[muds[loop]]["MUDNAME"]),
					"线路" + chinese_number(j+1),
				);
				output += sprintf("%-28s   %-15s  %4s  ",					
					"",
					NAT_MAP[muds[loop]][j][0..<6],
  				NAT_MAP[muds[loop]][j][<4..<1],
				);
				port = NAT_MAP[muds[loop]][j][<4..<1];
				if ( htonn(upper_case(muds[loop])) == htonn(upper_case(MUD_SITE_ID)) )
					output+= sprintf("%4d\n",sizeof(filter_array(users(),(: sprintf("%d",query_ip_port($1)) == $(port) :))));
				else output+="\n";			
				output += NOR;	
			}
		}		
		n++;
	}
	//      Display dumped mudlist output through user's more pager
	output += repeat_string("─", 42) + "\n";
	output += "「书剑·永恒」共有" + chinese_number(count) + "位玩家在" + chinese_number(n) + "处站点连线中。\n";
	write(output);

	return 1;
}

int help()
{
	write("
指令格式：mudlist		列出连线中的"MUD_NAME"站点。
	  mudlist <MudName>	列出指定 Mud 的信息。
	  mudlist all		列出目前所有连线中的 Mud。
"	);
	return 1;
}
