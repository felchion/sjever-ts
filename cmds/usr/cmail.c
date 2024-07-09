// cmail.c by stal@sj 2003.10.24

#include <ansi.h>
#include <login.h>

#define LOG_FILE       "/data/cmail"
#define OLD_DATA	   "/log/"

inherit ITEM;
inherit F_SAVE;

mapping *notes_csite;
mapping *notes_cmail;

int help(object);

void list_csite_simple(object me);
void list_csite_simple2(object me);
void list_cmail_simple(object me);
void list_cmail_simple2(object me);
void search_csite(object me,int i);
void search_cmail(object me,int i);
void cancel();
string str_result(mapping note);
string query_save_file();
int index_csite(string user);
int index_cmail(string user);


void create()
{
	seteuid(getuid());
	restore();
}

int main(object me, string arg)
{
	string s,user;
	int num;

	if (!arg || arg=="") 
                return help(me);

	if (arg=="-site") {
		list_csite_simple(me);
		return 1;
	}
	
	if (arg=="-site2") {
		list_csite_simple2(me);
		return 1;
	}	
	if (arg=="-mail") {
		list_cmail_simple(me);
		return 1;
	}
	
	if (arg=="-mail2") {
		list_cmail_simple2(me);
		return 1;
	}
	
	if (arg=="-a") {		
    	if ((int)me->query_temp("cmail/failed")>3) {
    		write (HIR"\n您的尝试已失败多次，出于安全考虑，您被暂时禁止使用本功能。\n"NOR);
    		cancel();
    		return 1;
    	}
		
					
		write(HIG+"\n注意：该命令主要针对那些因为种种原因遗忘密码和注册信箱的玩家,
如果你企图通过这种方式获得他人帐号密码,那么所得的惩罚将是非常严重，\n"
HIR"请担保者注意，一旦查出你在做伪证，你将受到至少减少40％的经验惩罚。\n\n"NOR);
		
		write("欢迎使用本向导，请按您的需要进行选择：\n");
		write("1.申请将老站ID转至本站\n");
		write("2.申请更改注册邮箱\n");
		write("3.为其他玩家更改注册邮箱做担保（你至少需要 5M 的经验值）\n");
		write("4.对其他玩家更改注册邮箱提出置疑\n");
		write("0.退出\n");
		write("请选择(0～4)：");
		
		input_to("input_start",1,me);
		
	
		return 1;
	}

	if (sscanf(arg,"-site -d %d",num)==1) {
		if ( wiz_level(me)<5 )
			return notify_fail("只有assist才可以删除转站申请记录。 \n");
		if( !arrayp(notes_csite) || num < 1 || num > sizeof(notes_csite) )
			return notify_fail("没有这个编号的内容。\n");
		num--;
		user = notes_csite[num]["id_old"] + "@" + notes_csite[num]["site_old"];		
		log_file( "static/CMAIL", sprintf("%s %s(%s)把关于%s的转站申请记录删除。\n", 
			ctime(time()), me->query("name"), me->query("id"), user));
		
		write("您将要删除"+ (num+1) + "号内容，内容如下：\n");
		search_csite(me,num);
		notes_csite = notes_csite[0..num-1] + notes_csite[num+1..<1];
		write("确定要删除，请输入“y”：");
		input_to("confirm",1);
		return 1;
		
	}

	if (sscanf(arg,"-site -r %d %s",num,s)==2) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以填写处理结果。 \n");
		if( !arrayp(notes_csite) || num < 1 || num > sizeof(notes_csite) )
			return notify_fail("没有这个编号的内容。\n");
			
		if (strlen(s)< 4 || strlen(s)>44)
			return notify_fail("请简明扼要地说明处理结果，字数必须在2-20之间。 \n");
			
		num--;
		user = notes_csite[num]["id_old"] + "@" + notes_csite[num]["site_old"];		
		
		log_file( "static/CMAIL", sprintf("%s %s(%s)对关于%s的转站申请做了答复：%s。\n", 
			ctime(time()), me->query("name"), me->query("id"), user,s));
			
		notes_csite[num]["result"]=s+"(by "+capitalize(me->query("id"))+")";
		
		write("您将要答复"+ (num+1) + "号申请：\n");
		search_csite(me,num);
		write("您的答复是：\n");
		write(HIR+s+"\n"NOR);
		write("确认，请输入“y”：");
		input_to("confirm",1);
		return 1;
		
	}
	
	if (sscanf(arg,"-mail -d %d",num)==1) {
		if ( wiz_level(me)<5 )
			return notify_fail("只有assist才可以删除邮箱更改记录。 \n");
		if( !arrayp(notes_cmail) || num < 1 || num > sizeof(notes_cmail) )
			return notify_fail("没有这个编号的内容。\n");
		num--;
		user = notes_cmail[num]["player"];		
		log_file( "static/CMAIL", sprintf("%s %s(%s)把关于%s的邮箱更改记录删除。\n", 
			ctime(time()), me->query("name"), me->query("id"), user));

		write("您将要删除"+ (num+1) + "号内容，内容如下：\n");
		search_cmail(me,num);
		notes_cmail = notes_cmail[0..num-1] + notes_cmail[num+1..<1];
		write("确定要删除，请输入“y”：");
		input_to("confirm",1);
		return 1;
		
	}
	
	if (sscanf(arg,"-mail -r %d %s",num,s)==2) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以填写处理结果。 \n");
		if( !arrayp(notes_cmail) || num < 1 || num > sizeof(notes_cmail) )
			return notify_fail("没有这个编号的内容。\n");
			
		if (strlen(s)< 4 || strlen(s)>44)
			return notify_fail("请简明扼要地说明处理结果，字数必须在2-20之间。 \n");
			
		num--;
		user = notes_cmail[num]["player"];		
		
		log_file( "static/CMAIL", sprintf("%s %s(%s)对关于%s的邮箱更改申请做了答复：%s。\n", 
			ctime(time()), me->query("name"), me->query("id"), user,s));
			
		notes_cmail[num]["result"]= s + "(by " + me->query("id") +")" ;
		
		write("您将要答复"+ (num+1) + "号申请：\n");
		search_cmail(me,num);
		write("您的答复是：\n");
		write(HIR+s+"\n"NOR);
		write("确认，请输入“y”：");
		input_to("confirm",1);
		return 1;
		
	}
		
	if (sscanf(arg, "-site %d",num)==1) {
		search_csite(me,num-1);
		return 1;
	}
	
	if (sscanf(arg, "-site %s",user)==1) {
		search_csite(me,index_csite(user));
		return 1;
	}
	
	if (sscanf(arg, "-mail %d",num)==1) {
		search_cmail(me,num-1);
		return 1;
	}
	
	if (sscanf(arg, "-mail %s",user)==1) {
		search_cmail(me,index_cmail(user));
		return 1;
	}
	
	if (sscanf(arg,"-%s",s)>0) 
		return notify_fail("指令格式错误，请参考help cmail\n");
	
	
}

void input_start(string arg,object me)
{
	switch(arg) {
		case "1":
			write ("您开始填写转站申请单。\n");
			write (HIG"\n此命令用于将合站后尚未转站的ID转至本站。您必须准确填写ID的注册\n"NOR);
			write (HIG"邮箱，巫师核实后将为您转移数据。如果您老站ID的注册邮箱已经不能\n"NOR);
			write (HIG"使用,请在转站成功后再次使用本向导申请更改注册邮箱。\n"NOR);
			write ("\n请输入老站名称(sd，sy，cq，hb)：");
			input_to("input_site",1,me);
			return;
		case "2":
			write ("您开始填写更改邮箱申请单。\n");
			write (HIG"\n申请范围仅限于密码遗失的ID 。(注意：如果ID被盗，属玩家保管不善，\n"NOR);
			write (HIG"责任自负。)在完成申请后，请找四位IP地址各不相同的5M以上玩家为您 \n"NOR);
			write (HIG"做担保。巫师核实所有信息属实后，将为您更改注册邮箱。您所填写的邮\n"NOR);
			write (HIG"箱，只有您自己和巫师可以看到。\n"NOR);
			write ("\n请输入需要更改邮箱的ID：");
			input_to("input_user",1,me);
			return;
		case "3":
			write ("您开始填写担保证明书。\n");
			write (HIR"\n请确保您所担保的对象的可靠性。否则，您可能会因为做伪证而受到至少减少40%经验值的惩罚。\n"NOR);
			write ("\n请输入需要更改邮箱的ID：");
			input_to("input_prove",1,me);
			return;
		case "4":
			write ("您开始填写置疑说明书。\n");
			write (HIG"\n如果您认为某项更改邮箱的申请有问题，请举报。除了巫师，任何人都看不到\n"NOR);
			write (HIG"您的举报内容。\n"NOR);
			write ("\n请输入您觉得有疑问的申请对象ID：");
			input_to("input_doubt_id",1,me);
			return;
		case "0":
			cancel();
			return;
		default:
			write ("没有这个选项，请重新选择：");
			input_to("input_start",1,me);
			return;
	}
	
}

void input_doubt_id(string arg,object me)
{
	int i=0;
	string doubt,ip;
	mapping note;
	if (strlen(arg)<3) {
		cancel();
		return;
	}

	if (index_cmail(arg)<0) {
		write ("没有人对这个ID提出更改邮箱的申请。\n");
		cancel();
		return;
	}
	
	note = notes_cmail[index_cmail(arg)];
	if (note["result"]!="applying" && note["result"]!="waiting") {
		write("巫师已经对这个申请做了答复，请用sos说明您的置疑。\n");
		cancel();
		return;
   	}
	
	doubt = "doubt"+i;
	ip = "ip_doubt"+i;
	while (note[doubt]) {
		i++;
		doubt = "doubt"+i;
		ip = "ip_doubt"+i;
	}
	
	note[doubt]=me->query("id");
	note[ip]=query_ip_number(me);
	
	write ("请用4～40个字简明扼要地说明您的怀疑：\n");
	input_to("input_doubt",1,note,i);
	return;
}

void input_doubt(string arg,mapping note,int i)
{
	string content = "content" +i;
	
	if (strlen(arg)< 8 || strlen(arg)>44) {
		write("字数必须在4-20之间,请重新输入： \n");
		input_to("input_doubt",1,note,i);
		return;
	}
		
	note[content] = arg;
	
	write ("您对关于"HIR+capitalize(note["user"])+NOR"的邮箱更改申请有下述疑问：\n");
	write (HIR+arg+"\n"NOR);
	write ("\n确认上述信息无误，请输入“y”：");
	input_to("confirm",1);
	return;
}

void input_prove(string arg,object me)
{
	mapping note;
	int i;
	string prove,ip;
	if (strlen(arg)<3) {
		cancel();
		return;
	}
	if (me->query("combat_exp") < 5000000) {
		write ("你的经验值少于5000000，不能进行担保。\n");
		cancel();
		return;
	}
		
	
	if (index_cmail(arg)<0) {
		write ("没有人对这个ID提出更改邮箱的申请。\n");
		cancel();
		return;
	}
	
	note = notes_cmail[index_cmail(arg)];
	if (note["result"]!="applying") {
		write("这个ID的担保人数已经足够了。\n");
		cancel();
		return;
	}
	
	if (wiz_level(me)<6 && note["ip_proposer"]==query_ip_number(me)) {
		write("您跟申请人在一起，不能担保。\n");
		cancel();
		return;
	}		
		
	for (i=1;i<=4;i++) {
		prove = "prove"+i;
		ip = "ip_prove"+i;
		if (!note[prove]) {
			note[prove]=me->query("id");
			note[ip]=query_ip_number(me);
			break;
		} else if (wiz_level(me)<6 && note[prove]==me->query("id")) {
			write( "您已经为这个ID做过担保了。\n");
			cancel();
			return;
		} else if (wiz_level(me)<6 && note[ip]==query_ip_number(me)) {
			write( "IP地址重复，不能做担保。\n");
			cancel();
			return;
		}
	}
	if (i==4) {
		note["result"]="waiting";
		note["time"]=time();
	}
	
	write ("您决定为"HIR+capitalize(arg)+NOR"更改邮箱做担保。\n");
	write (HIR"注意，一旦查出你在做伪证或者企图使用担保获取他人帐号，你将受到至少减少40％的经验惩罚。\n"NOR);
	write("确认上述信息正确，请输入“y”（按回车放弃担保）：");	
	input_to("confirm",1);
	return;
}

void confirm(string arg)
{
	if (arg!="y") {
		cancel();
		return;
	}

    save();
    write(HIW"您的操作已经成功。\n"NOR);
    return;	
	
}

void input_user(string arg,object me)
{
	mapping note =allocate_mapping(8);

	object *online_list,ob;	
	int i;
	
	if (strlen(arg)<3) {
		cancel();
		return;
	}
	
	if (arg==note["proposer"]) {
		write("给自己挂失？别瞎捣腾！\n");
		cancel();
		return;
	}
	if ("/cmds/usr/blacklist"->is_black(arg))
        {
                write(arg + " 是黑名单中的人物，不能申请更改信箱,请等待巫师处理结果。\n");
                return;
        }
    if (index_cmail(arg)>=0) {
    	if (notes_cmail[index_cmail(arg)]["result"]=="waiting" || notes_cmail[index_cmail(arg)]["result"]=="applying") {
    		write("\n已经有人对这个ID提出了更改邮箱的申请，巫师答复前不能重复申请。\n");
    		cancel();
    		return;
    	}
	}
	
	online_list = filter_array(children(USER_OB), (: userp($1) && environment($1) :));
	i = sizeof(online_list)-1;
	while(i>=0) {
		if (online_list[i]->query("id")==arg)
			break;
		else
			i--;
	}
	if (i>=0) {
		write("这个人正在线上呢，您没弄错吧？\n");
		cancel();
		return;
	}
    ob = LOGIN_D->find_body(arg);
    if (!ob) {
		if( file_size(DATA_DIR + "login/" + arg[0..0] + "/" + arg + SAVE_EXTENSION)<0 ) {
            write("没有这位使用者。\n");
            cancel();
            return;
        }
		ob = new(LOGIN_OB);
		ob->set("id",arg);
		if( !ob->restore() ) {
			destruct(ob);
			write("没有这个玩家。\n");
			cancel();
			return;
		}
	}
	
    if (time()-ob->query("last_on")<1296000) {
    	destruct(ob);
    	write("这个人在十五天之内登录过，不能申请更换邮箱。\n");
    	cancel();
    	return;
    }
    if( wiz_level(ob->query("id"))){
         destruct(ob);
         write("葛哈葛哈，你想葛哈？！！\n");
         cancel();
         return;                       
    }
	note["proposer"] = me->query("id");
	note["time"] = time();
	note["ip_proposer"] = query_ip_number(me);
	note["user"] = arg;
	note["result"] = "applying";
    write("请输入新的注册邮箱：");
    input_to("input_mbox_new",1,note);
    return;
}

void input_mbox_new(string arg,mapping note)
{
	string s;
    if (strlen(arg)<3) {
    	cancel();
    	return;
    }
    	
    if (sscanf(arg,"%s@%s.%s",s,s,s)<3) {
    	write("\n邮箱格式错误，重新输入或按回车键放弃：");
    	input_to("input_mbox_new",1,note);
    	return;
    }
    
    note["mbox"] = arg;
	write ("您决定申请将"HIR+capitalize(note["user"])+NOR"的注册邮箱更改为："HIR+arg+NOR"\n");
	write("确认上述信息正确，请输入“y”：");	
    input_to("confirm_apply",1,note);
    return;	
}		

void confirm_apply(string arg,mapping note)
{
	if (arg!="y") {
		cancel();
		return;
	}
		
    if( !pointerp(notes_cmail) || !sizeof(notes_cmail) )
	    notes_cmail = ({ note });
    else
        notes_cmail += ({ note });
    
    save();
    write(HIW"您的申请已经发出。\n"NOR);
    return;	
}

void cancel()
{
	write(HIW"您结束了本次操作。\n"NOR);
	restore();
	return;
}
void input_site(string arg,object me)
{
	mapping note =allocate_mapping(8);

	if (strlen(arg)<2) {
		cancel();
		return;
	}
	
    if ( arg!="hb" && arg!="sd" && arg!="sy" && arg!="cq") {
            write("没有这个站点，重新输入或按回车键放弃：");
            input_to("input_site",1,me);
            return;
    }

	note["proposer"] = me->query("id");
	note["time"] = time();
	note["ip_proposer"] = query_ip_number(me);   
    note["site_old"] = arg;
    write("请输入用户ID：");
    input_to("input_oid",1,note,me);
    return;		
}

void input_oid(string arg,mapping note,object me)
{
	string file,site,str;
	if (strlen(arg)<3) {
		cancel();
		return;
	}
	
	site = note["site_old"];	
	file  = OLD_DATA + site + "/data/login/"+ arg[0..0] +"/"+ arg + SAVE_EXTENSION;	
    if (file_size(file) <= 0) {
        write("\n"+capitalize(arg) +"@"+site+"不存在，重新输入或按回车键放弃：");
        input_to("input_oid",1,note);
        return;
    }
    
    if (index_csite(arg)>=0)
    	if (notes_csite[index_csite(arg)]["result"]=="waiting") {
    		write("\n已经有人对这个ID提出了转站申请，巫师答复前不能重复申请。\n");
    		restore();
    		return;
    	}
    
    str = read_file(file,2);
    
        
    if (sscanf(str,"%*semail%*s:%s,%*s",str) != 4) {
    	write ("\n这个ID的数据中没有注册邮箱信息，请用"HIY"sos"NOR"向巫师求助。\n");
    	cancel();
    	return;
    }   	
    str = replace_string(str,"\"","");
    note["id_old"] = arg;
    write("\n请输入"+capitalize(arg)+"@"+site+"的注册邮箱：");
    input_to("input_mbox",1,note,str);
    return;
}

void input_mbox(string arg,mapping note,string mbox)
{
	string s;
	object me = this_player();
    if (strlen(arg)<3) {
    	cancel();
    	return;
    }
    	
    if (sscanf(arg,"%s@%s.%s",s,s,s)<3) {
    	write("\n邮箱格式错误，重新输入或按回车键放弃：");
    	input_to("input_mbox",1,note);
    	return;
    }

    if (arg!=mbox) {
   	
   	me->add_temp("cmail/failed",1);
    	if ((int)me->query_temp("cmail/failed")>3) {
    		write (HIR"\n您的尝试已失败多次，出于安全考虑，您被暂时禁止使用本功能。\n"
    			"注意，您的行为已经被系统记录!\n"NOR);
			log_file( "static/CMAIL", sprintf("%s %s(%s)对%s的注册邮箱多次尝试均已失败。\n", 
				ctime(time()), me->query("name"), me->query("id"),note["id_old"]+"@"+note["site_old"]));
    		cancel();
    		return;
    	}
    	write ("\n您所输入的邮箱，不是这个ID的注册邮箱。\n请重新输入：");
    	input_to("input_mbox",1,note,mbox,me);
    	return;
    }
    
    note["mbox"] = arg;
    write("\n请输入新ID：");
    input_to("input_nid",1,note);
    return;
}

void input_nid(string arg,mapping note)
{
	string file;
	if (strlen(arg)<3) {		
		cancel();
		return;
	}
		
	file  = "/data/login/"+ arg[0..0] +"/"+ arg + SAVE_EXTENSION;			
    if (file_size(file) > 0) {
        write("\n这个ID已经被其他玩家使用了，重新输入或按回车键放弃：");
        input_to("input_nid",1,note);
        return;
    }
    
    note["id_new"] = arg;
    note["result"] = "waiting";
    write("\n您填写的申请单内容如下：\n老用户名：" + note["id_old"]);
    write("\n来自站点：" + note["site_old"]);
    write("\n注册邮箱：" + note["mbox"]);
    write("\n新用户名：" + note["id_new"]);
    write("\n确认上述信息无误，请输入“y”：");
    input_to("confirm_csite",1,note);
    return;
}

void confirm_csite(string arg,mapping note)
{
	if (arg!="y") {
		cancel();
		return;
	}
		
    if( !pointerp(notes_csite) || !sizeof(notes_csite) )
	    notes_csite = ({ note });
    else
        notes_csite += ({ note });
    
    save();
    write(HIW"您的申请已经发出。\n"NOR);
    return;
}
		
void list_csite_simple(object me)
{
	int i;
	string str;

	i = sizeof(notes_csite)-1;
	if( i < 0 ) {
		write("目前没有转站申请记录。\n");
		return;
	} 
	str="转站申请记录：\n";
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	while (i>=0) {
		str += sprintf("编号%3d   时间 %s     ID  %-13s   状态  %s\n",
			i+1,
			ctime(notes_csite[i]["time"])[4..15],
			capitalize(notes_csite[i]["id_old"]) + "@" + notes_csite[i]["site_old"],			
			str_result(notes_csite[i])
			);
		
		i--;
		
	}
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	me->start_more( str );	
}

void list_csite_simple2(object me)
{
	int i;
	string str;

	i = sizeof(notes_csite)-1;
	if( i < 0 ) {
		write("目前没有转站申请记录。\n");
		return;
	} 
	str="转站申请即将要处理记录：\n";
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	while (i>=0) {
		if(notes_csite[i]["result"]=="waiting")
		{
			str += sprintf("编号%3d   时间 %s     ID  %-13s   状态  %s\n",
			i+1,
			ctime(notes_csite[i]["time"])[4..15],
			capitalize(notes_csite[i]["id_old"]) + "@" + notes_csite[i]["site_old"],			
			str_result(notes_csite[i])
			);
		}
		i--;
		
	}
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	me->start_more( str );	
}

void list_cmail_simple(object me)
{
	int i;
	string str;

	i = sizeof(notes_cmail)-1;
	if( i < 0 ) write("目前没有任何邮箱更改申请记录。\n");
	else {
		str="邮箱更改申请记录：\n";
		str += HIY + "≡——————————————————————————————————————≡\n" + NOR;
		while (i>=0) {
			str += sprintf("  编号%3d   时间 %s   更换信箱ID %-9s   状态  %s\n",
				i+1,
				ctime(notes_cmail[i]["time"])[4..15],
				capitalize(notes_cmail[i]["user"]),			
				str_result(notes_cmail[i])
				);			
			i--;
		}
		str += HIY + "≡——————————————————————————————————————≡\n" + NOR;
	}
	me->start_more( str );	
}

void list_cmail_simple2(object me)
{
	int i;
	string str;
	string str2 = "";

	i = sizeof(notes_cmail)-1;
	if( i < 0 ) write("目前没有玩家申请信箱修改。\n");
	else {
		str="  当前有下列玩家信箱修改申请，如果有疑问请和巫师联系或者直接用(cmail -a)\n  指令启动向导的置疑系统(质疑内容只有巫师可以见)：\n";
		str += HIY + "≡——————————————————————————————————————≡\n" + NOR;
		while (i>=0) {
			if(notes_cmail[i]["result"]=="waiting")
			{
			str += sprintf("  编号%3d   时间 %s   更换信箱ID %-9s   状态  %s\n",
				i+1,
				ctime(notes_cmail[i]["time"])[4..15],
				capitalize(notes_cmail[i]["user"]),			
				str_result(notes_cmail[i])
				);
			}
			if(notes_cmail[i]["result"]=="applying")
			{
			str2 += sprintf("  编号%3d   时间 %s   更换信箱ID %-9s   状态  %s\n",
				i+1,
				ctime(notes_cmail[i]["time"])[4..15],
				capitalize(notes_cmail[i]["user"]),			
				str_result(notes_cmail[i])
				);
			}
			
			i--;
		}
		str = str + str2;
		str += HIY + "≡——————————————————————————————————————≡\n" + NOR;
		str += HIR"  注:如果申请成功三天内还无人质疑，游戏管理员会按照申请人要求进行操作\n"NOR;
		
	}
	write( str );	
}

string str_doubt(object me,mapping note)
{
	string str="";
	string doubt,ip,content;
	int i=0;
	
	if (!wizardp(me)) return "";
	
	doubt = "doubt"+i;
	ip = "ip_doubt"+i;
	content = "content"+i;
	while (note[doubt]) {
		str += note[doubt];
		str += "(" + note[ip] + ")";
		str += "：" + note[content] + "\n";		
		
		i++;
		doubt = "doubt"+i;
		ip = "ip_doubt"+i;
		content = "content"+i;
	}
	
	if (strlen(str)<1) {
		str = "【目前没有玩家对这个申请提出置疑】\n";
	} else {
		str = "【下列玩家对这个申请提出置疑】\n" + str;
	}
	return str;
}

string str_result(mapping note)
{
	if (!note["result"]) return "错误的申请信息";
	if (note["result"]=="applying") return HBBLU+HIR"担保玩家未达要求"NOR;
	if (note["result"]=="waiting") return HBRED+HIY"正在等待巫师处理"NOR;
	//return note["result"];
	return HBCYN+HIG"巫师已经处理完毕"NOR;
}

string str_prove(object me,mapping note)
{
	string str="";
	string prove,ip;
	int i;
	for (i=1;i<=4;i++) {
		prove = "prove"+i;
		ip = "ip_prove"+i;
		if (!note[prove]) return str;
		str += " "+note[prove];
		if (wizardp(me)) str += "("+note[ip]+")";
	}
	return str;
}
	
int index_csite(string user)
{
	int i;
	if (!pointerp(notes_csite) || !sizeof(notes_csite)) return -1;
	i = sizeof(notes_csite)-1;
	while(i>=0) {
		if (notes_csite[i]["id_old"]==user) return i;
		i--;
	}
	return i;
}	
	
int index_cmail(string user)
{
	int i;
	if (!pointerp(notes_cmail) || !sizeof(notes_cmail)) return -1;
	i = sizeof(notes_cmail)-1;
	while(i>=0) {
		if (notes_cmail[i]["user"]==user) return i;
		i--;
	}
	return i;
}	


void search_csite(object me,int i)
{
	
	string str;
	if( !arrayp(notes_csite) || i < 0 || i > (sizeof(notes_csite) - 1) )
	{
		write("没有关于这项转站申请记录。\n");
		return;
	}

	str="转站申请记录：\n";
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	str += sprintf("编号 %d 时间 %s\n【老ID】%s\n【新ID】%s%s \n【申请人】%s \n【状  态】%s\n",
		i+1,
		ctime(notes_csite[i]["time"])[4..15],
		capitalize(notes_csite[i]["id_old"])+"@"+notes_csite[i]["site_old"],
		notes_csite[i]["id_new"],
		(wizardp(me)||me->query("id")==notes_csite[i]["proposer"])?"\n【注册邮箱】"+notes_csite[i]["mbox"]:"",
		notes_csite[i]["proposer"]+(wizardp(me)?"("+notes_csite[i]["ip_proposer"]+")":""),
		//str_result(notes_csite[i])
		notes_csite[i]["result"]
		);
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;	
	
	me->start_more( str );	
}

void search_cmail(object me,int i)
{
	
	string str;
	if( !arrayp(notes_cmail) || i < 0 || i > (sizeof(notes_cmail)-1) )
	{
		write("没有关于这个玩家的邮箱更改申请记录。\n");
		return;
	}

	str="邮箱更改申请记录：\n";
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;
	str += sprintf("编号 %d 时间 %s\n【玩家ID】%s\n%s【申请人】%s \n【担保人】%s \n【状  态】%s\n%s",
				i+1,
				ctime(notes_cmail[i]["time"])[4..15],
				capitalize(notes_cmail[i]["user"]),
				(wizardp(me)||me->query("id")==notes_cmail[i]["proposer"])?"【新邮箱】"+notes_cmail[i]["mbox"]+"\n":"",
				notes_cmail[i]["proposer"]+(wizardp(me)?"("+notes_cmail[i]["ip_proposer"]+")":""),
				str_prove(me,notes_cmail[i]),
				//str_result(notes_cmail[i]),
				notes_cmail[i]["result"],
				str_doubt(me,notes_cmail[i])
			);
	str += HIY + "≡————————————————————————————————————≡\n" + NOR;	
	
	me->start_more( str );	
}

string query_save_file()
{
	return LOG_FILE ;
}

int help(object me)
{
	if (!wiz_level(me) )
        write(@HELP
指令格式 :
cmail -a                        启动操作向导
cmail -mail [id|数字]           查看玩家转站的申请记录
cmail -site [id|数字]           查看玩家更改邮箱的申请记录
cmail -mail2                    查看玩家申请成功等待巫师处理的信箱申请记录
cmail -site2                    查看玩家申请成功等待巫师处理的转站申请记录

HELP);
    else 
        write(@HELP
指令格式 :
cmail -a                              启动操作向导
cmail -mail [id|数字]                 查看玩家转站的申请记录
cmail -mail -d <数字>                 删除指定编号的邮箱更改申请 (assist可用)
cmail -mail -r <数字> <处理结果>      填写指定编号申请的处理结果 (arch可用)
cmail -site [id|数字]                 查看玩家更改邮箱的申请记录
cmail -site -d <数字>                 删除指定编号的转站更改申请 (assist可用)
cmail -site -r <数字> <处理结果>      填写指定编号申请的处理结果 (arch可用)
cmail -mail2                          查看玩家申请成功等待巫师处理的信箱申请记录
cmail -site2                          查看玩家申请成功等待巫师处理的转站申请记录

巫师操作被记录在/log/csite中。

HELP);
	return 1;
}
