// logind.c

#include <ansi.h>
#include <command.h>
#include <login.h>
#include <mudlib.h>

inherit F_DBASE;

int wiz_lock_level = WIZ_LOCK_LEVEL;
int ppls = 0;
int maxusers = MAX_USERS;
string welcome_msg;

private void get_id(string, object);
private void confirm_id(string, object);
object make_body(object);
private void init_new_player(object);
varargs void enter_world(object, object, int);
varargs void reconnect(object, object, int);
object find_body(string);
int check_legal_id(string);
int check_legal_name(object, string);
private void get_email(string, object, mapping);

private void random_gift(mapping my, int select)
{
	int i = 10, j;
	int tmpstr, tmpint, tmpcon, tmpdex;

	tmpstr = tmpint = tmpcon = tmpdex = 10;

	switch (select) {
		case 1: tmpstr = i = my["str"]; break;
		case 2: tmpint = i = my["int"]; break;
		case 3: tmpcon = i = my["con"]; break;
		case 4: tmpdex = i = my["dex"];
	}
	i = 50 - i;
	while (i--) {
		j = random(4);
		if (j == select - 1) i++;
		else switch (j) {
			case 0: if (tmpstr < 30) tmpstr++;
				else i++;
				break;
			case 1: if (tmpint < 30) tmpint++;
				else i++;
				break;
			case 2: if (tmpcon < 30) tmpcon++;
				else i++;
				break;
			case 3: if (tmpdex < 30) tmpdex++;
				else i++;
		}
	}
	my["str"] = tmpstr;
	my["int"] = tmpint;
	my["con"] = tmpcon;
	my["dex"] = tmpdex;
	// next 3 are randomly selected
	my["kar"] = 10 + random(21);
	my["per"] = 10 + random(21);
	my["pur"] = 10 + random(21);
}

void create()
{
	seteuid(getuid());
	set("channel_id", "连线精灵");
	set("md5", crypt("ShuJian", "$1") != crypt("ShuJian", "$1$"));
	if (file_size("/log/ppls") > 0)
		sscanf(read_file("/log/ppls"), "%d", ppls);
	if (file_size(WELCOME) > 0)
		welcome_msg = read_file(WELCOME);
	else welcome_msg = sprintf("\n%76|s\n\n", "欢迎光临"HIW"书剑"NOR"网络游戏");
	if (file_size("/adm/etc/maxusers") > 0)
		sscanf("\n"+read_file("/adm/etc/maxusers"), "%*s\n" INTERMUD_MUD_NAME " %d\n", maxusers);
	welcome_msg += "本站目前人数上限：" HIY + chinese_number(maxusers) + NOR "\n"
		+ "本站密码编码方式："HIY+(query("md5")?
		"MD5 支持任意长度的密码":"DES 支持 8 个字符长度的密码")+NOR+"\n";
}

//void site_check(object user){
//	string uid;
//	object ob;
//	if(!objectp(user)) return;
//	
//	if(user->query_condition("manyou")) return;
//	
//	uid = user->query("id");
//	
//	if("tj"==MUD_SITE_ID && TOP_D->query_pk_rank(uid) > 200 
//	&& (TOP_D->query_rank(uid) > 249 || TOP_D->query_rank(uid)==-1) 
//	&& TOP_D->query_pk_rank(uid) != -1 ) {				
//		message_vision("一个黑影突然出现在$N面前，说道：“"+user->name(1) +
//			"！这个站点不是你现在可以玩的地方，请退出游戏吧！”\n", user);
//		CHANNEL_D->do_channel(this_object(), "rumor", user->name(1) + "被系统踢出了天玑站了。");
//		tell_object(user,HIR "你的帐号只能在TJ2站(tj2.sjever.net:7777)登录，你被系统踢出了天玑站了。\n"NOR);
//		user->save();
//		if(objectp(ob=user->query_temp("link_ob"))) {
//			ob->save();
//			destruct(ob);
//		}
//		destruct(user);
//		dbquery("UPDATE Users SET U_Online='N' WHERE U_Username="+save_variable(uid));
//		return;
//	}
//	if("tj2"==MUD_SITE_ID && TOP_D->query_pk_rank(uid) < 200 && TOP_D->query_pk_rank(uid) != -1) {
//		message_vision("一个黑影突然出现在$N面前，说道：“"+user->name(1) +
//			"！这个站点不是你现在可以玩的地方，请退出游戏吧！”\n", user);
//		CHANNEL_D->do_channel(this_object(), "rumor", user->name(1) + "被系统踢出了天玑二站了。");
//		tell_object(user,HIR "你的帐号只能在TJ站(tj.sjever.net:5555)登录，你被系统踢出了天玑二站了。\n"NOR);
//		user->save();
//		if(objectp(ob=user->query_temp("link_ob"))) {
//			ob->save();
//			destruct(ob);
//		}
//		destruct(user);
//		dbquery("UPDATE Users SET U_Online='N' WHERE U_Username="+save_variable(uid));
//		return;
//	}
//}
	
int remove()
{
	write_file("/log/ppls", sprintf("%d", ppls), 1);
	return 1;
}

void logon(object ob)
{
	object *usr;
	int login_cnt = 0;
	string ip = query_ip_number(ob);

	if(!IN_MAIN){
  	input_to("get_id", ob);  	
  	return;
  }
  
	if (++ppls % 100 == 0)
		remove();
// 防止有恶意的玩家用 KMUD 等工具捣乱
	usr = filter_array(children(LOGIN_OB), (: interactive :));
	foreach (object item in usr)
		login_cnt += query_ip_number(item) == ip;
	if (login_cnt > 3) {
		destruct(ob);
		return;
	}
// Add by Yu Jue 1997.8.6
	write("\nBest view with 800 * 600\nAre you using BIG5 font(Y/N)? ");
	input_to("gb_big5", ob);
}

void gb_big5(string arg, object ob)
{
	if (lower_case(arg)[0] == 'y')
		ob->set_temp("big5", 1);
	if (BAN_D->is_banned(query_ip_number(ob))) {
		write("对不起，您的地址在" + MUD_NAME + "不受欢迎。\n");
		destruct(ob);
		return;
	}
	write(sprintf("\n%76|s\n%76|s\n%s", MUD_NAME, "- " __VERSION__ " -", welcome_msg));
	write("本站总共访问人次：" HIY + chinese_number(ppls) + NOR "\n");
	UPTIME_CMD->main(1);
	"/cmds/usr/mudlist"->main(ob, "all");

	write("您的英文名字(ID)是：");
	input_to((: get_id :), ob);
}

varargs void enter_cp(object ob, object user, int silent)
{
	string startroom;
	user->set_temp("link_ob", ob);
	ob->set_temp("body_ob", user);
	user->set_temp("big5", ob->query_temp("big5"));
	user->set_temp("client_type", ob->query_temp("client_type"));
	
	exec(user, ob);
	user->setup();
	user->set_temp("db_quit", 1);
	user->set_temp("db_login", 1);
	ob->set_temp("db_quit", 1);
	ob->set_temp("db_login", 1);
	ob->restore();
	UPDATE_D->get_cloth(user);
	
	startroom=user->query("cp_startroom");
	user->delete("cp_startroom");
	user->move(startroom);
}
private void get_in(object ob)
{
	object user;
	string uid;
	uid = ob->query("id");
	user = find_body(uid);
	if (user) {
		if (!interactive(user)) {
			reconnect(ob, user);
			return;
		}
		call_out("confirm_relogin",0,"Y", ob, user);
		return;
	}
	if (objectp(user = make_body(ob))) {
		user->restore();
		enter_cp(ob, user);
		return;
	}
	destruct(user);	
	write("进入副本失败。\n");
	destruct(ob);
}
private void get_id(string arg, object ob)
{
	int i,mysql;
	object ppl, *usr;
	string ip = query_ip_number(ob);
	mixed rows;

	if(!IN_MAIN){
		ob->set("id", arg);
		if (ob->restore()) {
			get_in(ob);			
		}
		return;
	}
	
		
	arg = lower_case(arg);
	if (!check_legal_id(arg)) {
		write("您的英文名字(ID)是：");
		input_to((: get_id :), ob);
		return;
	}


	if ("/cmds/usr/blacklist"->is_black(arg)) {
		write("很遗憾，你是黑名单中的人物，请等待处理结果。\n");
		destruct(ob);
		return;
	}

	if (ob->set("id", arg) != arg) {
		write("Failed setting user name.\n");
		destruct(ob);
		return;
	}

	ppl = find_body(arg);
	if (!ppl && !wiz_level(arg)) {
		usr = filter_array(users(), (: environment :));
		if (sizeof(usr) >= maxusers) {
		// Only allow reconnect an player in MUD when MAX_USERS exceeded.
			write("对不起，目前" + MUD_NAME + "的玩家已经太多了，请待会再来。\n");
			destruct(ob);
			return;
		}
	}

	// add by snowman@SJ 16/02/2000, wiz_lock后新建人物也不能login.
	if (wiz_level(arg) < wiz_lock_level) {
		write("对不起，" + MUD_NAME + "目前限制巫师等级 "+wiz_lock_level+" 以上的人才能连线。\n");
		destruct(ob);
		return;
	}

	if (arg == "guest") {
		// If guest, let them create the character.
		ob->set("password", "SJGUEST");
		write("您的中文名字：");
		input_to("get_name", ob);
		return;
	}

	if (file_size(ob->query_save_file() + __SAVE_EXTENSION__) >= 0) {
		if (ob->restore()) {
			write("英文 ID 已经存在，请输入密码或者直接回车重新选择 ID。\n");
			write("请您输入这个人物的识别密码(passwd)：");
			input_to("get_passwd", 1, ob);
			return;
		}
		write("您的人物档案出了一些问题，请利用 guest 登录通知巫师处理。\n");
		destruct(ob);
		return;
	}

	// 防止多个玩家用同一 id
	usr = filter_array(children(LOGIN_OB), (: interactive :));
	i = sizeof(usr);
	while (i--)
		if (usr[i] != ob && usr[i]->query("id") == arg)
			break;
	if (i >= 0) {
		ob->delete("id");
		write("有其他玩家也在使用这个 ID 创造新人物，请重新输入。\n");
		write("您的英文名字(ID)是：");
		input_to((: get_id :), ob);
		return;
	}

	write("使用 " + arg + " 这个名字将会在此创造一个新的人物，您确定吗(y/N)？");
	input_to("confirm_id", ob);
}

private void get_passwd(string pass, object ob)
{
	string my_pass, new_pass;
	object user;
	int mysql;
	mixed rows;
	string ip_number = query_ip_number(ob);

	if (query_temp("step2/"+ip_number) >= 3) {
		write("系统检测到这个地址有人多次输入错误的密码，暂时禁止登录。\n");
		destruct(ob);
		return;
	}

	if (query_temp("step1/"+ip_number) >= 3) {
		if (add_temp("step2/"+ip_number, 1)==1)	// YUJ@SJ 2001-12-11
			call_out("delete_temp", 600, "step2/"+ip_number);
		write("系统检测到这个地址有人多次输入错误的密码，暂时禁止登录。\n");
		destruct(ob);
		return;
	}

	write("\n");
	if (pass == "") {
		ob->delete("id");
		write("您的英文名字(ID)是：");
		input_to((: get_id :), ob);
		return;
	}

	my_pass = ob->query("password");
	new_pass = ob->query("newpassword");

	switch (ob->query("registered")) {
		case 1:
			if (crypt(pass, new_pass) == new_pass) {
				ob->set("registered", 2);
				ob->delete("newpassword");
				ob->save();
				log_file("maillist", ob->query("email") +" "+ ob->query("id") +" "+ ctime(time())[4..18] +"\n");
				write(BLINK HIY "\n登记成功，你可以继续使用原来的密码游戏！\n" NOR);
				break;
			}
		default:
			if (crypt(pass, my_pass) != my_pass ) {
				write("\n您所输入的密码并不正确！请检查是否输入出错。\n");
				if (add_temp("step1/"+ip_number, 1)==1)	// YUJ@SJ 2001-12-11
					call_out("delete_temp", 30, "step1/"+ip_number);
				log_file( "USAGE", sprintf("%s 来自 %15s 的某人企图使用：%16s 登录 %s\n",ctime(time())[4..18], ip_number, pass, capitalize(""+ob->query("id")) ));
				ob->add("login_fail", 1);
				ob->save();
				destruct(ob);
				return;
			}
			ob->set("password", crypt(pass, "$1$ShuJian"));
	}

	// Check if we are already playing.
	user = find_body(ob->query("id"));
	if (user) {
		if (user->query("registered") == 1) {
			message_vision("$N离开这个房间。\n", user);
			if (user->query_temp("link_ob")) destruct(user->query_temp("link_ob"));
			destruct(user);
		}
	}
	if (user) {
		if (!interactive(user)) {
			log_file( "USAGE", sprintf("%-19s被 %-16s重连(%s)\n",
				user->query("name")+"("+capitalize(""+user->query("id"))+")",
				query_ip_number(ob), ctime(time())[4..18]));
			reconnect(ob, user);
			return;
		}
		write("人物目前正在游戏当中，您可以取而代之，确定请打 Y ，否则请打 N 。确定吗(y/N)？");
		input_to("confirm_relogin", ob, user);
		return;
	}

	if (time() - ob->query("last_on") < 30 && !wiz_level(ob->query("id"))) {
		write("你可能刚刚退出或者自杀，不能马上登录。\n");
		destruct(ob);
		return;
	}
	if (objectp(user = make_body(ob))) {
		if (user->restore()) {
			
		if(!wiz_level(ob->query("id")) && user->query("online") && user->query("online")!=MUD_SITE_ID) {
			write(HIY "您的帐号已在"HIR+upper_case(user->query("online"))+HIY"登录，请先退出。\n"NOR);
			destruct(user);
			destruct(ob);
			return;
		}	
			enter_world(ob, user);
			return;
		}
		destruct(user);
	}
	write("请您重新创造这个人物。\n");
	confirm_id("y", ob);
}

private void confirm_relogin(string yn, object ob, object user)
{
	object old_link;

	if (!user) {
		write("您要取代的人物已经退出游戏，请重新登录。\n");
		destruct(ob);
		return;
	}
	if (lower_case(yn)[0] != 'y') {
		write("好吧，欢迎下次再来" + MUD_NAME + "。\n");
		destruct(ob);
		return;
	}
	tell_object(user, "\n有人从别处(" + query_ip_name(ob)+ ")连线取代你所控制的人物。\n\n");
	log_file( "USAGE", sprintf("%-19s被 %-16s代替(%s)\n",
		user->query("name")+"("+capitalize(""+user->query("id"))+")",
		query_ip_number(ob), ctime(time())[4..18]));

	// Kick out the old player.
	old_link = user->query_temp("link_ob");
	if (old_link) {
		if (interactive(user)) {
			old_link->set("last_on", time());
			old_link->set("last_from", query_ip_name(user)+"("+query_ip_number(user)+")");
			old_link->save();
			ob->restore();
			exec(old_link, user);
		}
		destruct(old_link);
	}
	reconnect(ob, user);
}

private void confirm_id(string yn, object ob)
{
	if ( yn=="" ) {
		write("使用这个名字将会创造一个新的人物，您确定吗(y/n)？");
		input_to("confirm_id", ob);
		return;
	}

	if (lower_case(yn)[0] != 'y') {
		ob->delete("id");
		write("请重新输入您的英文名字：");
		input_to((: get_id :), ob);
		return;
	}
	write("密码的长度至少要五个字符，并且必须包含数字和英文大写字母。\n请设定您的密码：");
	input_to("new_password", 1, ob);
}

private void new_password(string pass, object ob)
{
	int i = strlen(pass), j = 0 ,k = 0;

	write("\n");

	if (i < 5) {
		write("为了保证安全，密码的长度至少要五个字符。\n");
		write("请重设您的密码：");
		input_to("new_password", 1, ob);
		return;
	}

	if (!query("md5") && i > 8) {
		write("因为是 DES 编码，密码的长度最多只能八个字符。\n");
		write("请重设您的密码：");
		input_to("new_password", 1, ob);
		return;
	}

	while (i--) {
		if (pass[i]>='0' && pass[i]<='9') j++;
		if (pass[i]>='A' && pass[i]<='Z') k++;
	}

	if (j < 1 || k < 1) {
		write("对不起，您的密码必须包含数字和英文大写字母。\n");
		write("请重设您的密码：");
		input_to("new_password", 1, ob);
		return;
	}

	if (strsrch(lower_case(pass), ob->query("id")) >= 0) {
		write("对不起，您的密码太简单。\n");
		write("请重设您的密码：");
		input_to("new_password", 1, ob);
		return;
	}

	ob->set("password", crypt(pass, "$1$ShuJian"));
	write("\n请再输入一次您的密码，以确认您没记错：");
	input_to("confirm_password", 1, ob);
}

private void confirm_password(string pass, object ob)
{
	string old_pass = ob->query("password");

	write("\n");
	if (crypt(pass, old_pass) != old_pass) {
		write("您两次输入的密码不一样，请重新设定密码：");
		input_to("new_password", 1, ob);
		return;
	}
	write(
"\n"
"请输入您的高姓大名，由于这个名字代表你的人物，而且以后不能随意更改，\n"
"务必慎重择名（不雅观的姓名将被删除）。\n"
HIY "警告：不要使用金庸小说中的人物姓名和常用称呼。\n"
"      不要使用游戏中已经存在的人物姓名和常用称呼。\n" NOR
"\n"
);
	write("您的中文名字：");
	input_to("get_name", ob);
}

private string display_attr(int gift)
{
	if (gift > 24) return HIY + gift + NOR;
	if (gift < 16) return CYN + gift + NOR;
	return "" + gift;
}

private void get_name(string arg, object ob)
{
	if (!check_legal_name(ob, arg)) {
		write("您的中文名字：");
		input_to((: get_name :), ob);
		return;
	}
	ob->set("name", arg);

	write("\n\n"
		"一个人物的天赋对于他或她所修习的武艺息息相关。\n"
		+ MUD_NAME + "中的人物具有以下四项天赋：\n\n"
		" 1. 膂力：影响伤害力、攻击能力、饭量、负荷量和气的增量。\n"
		" 2. 悟性：影响学习速度、防御能力和精的增量。\n"
		" 3. 根骨：影响体力恢复速度、受伤承受能力和其它三项的增量。\n"
		" 4. 身法：影响躲避能力和精力的增量。\n\n"
		"您可以自己指定其中一项的值，或者输入 0 由系统随机产生。\n"
		"请输入您的选择(0-4)："
	);
	input_to("select_gift", ob);
}

private void get_gift(string yn, object ob, mapping my, int select)
{
	if (lower_case(yn)[0] != 'y') {
		random_gift(my, select);
		write(sprintf("\n膂力[%s]，悟性[%s]，根骨[%s]，身法[%s]\n您同意这一组天赋吗？",
			display_attr(my["str"]),
			display_attr(my["int"]),
			display_attr(my["con"]),
			display_attr(my["dex"]))
		);
		input_to((: get_gift :), ob, my, select);
		return;
	}
	get_email("guest", ob, my);
}

private void select_gift(string yn, object ob)
{
	int i;

	if (!sscanf(yn, "%d", i) || i < 0 || i > 4) {
		write("\n输入错误，请重新选择：");
		input_to("select_gift", ob);
		return;
	}
	if (i) {
		write("\n请输入您想要的数值(10-30)：");
		input_to("set_gift", ob, ([]), i);
	} else get_gift(" ", ob, ([]), i);
}

private void set_gift(string yn, object ob, mapping my, int select)
{
	int i;

	if (!sscanf(yn, "%d", i) || i < 10 || i > 30) {
		write("\n数值错误，请重新输入：");
		input_to("set_gift", ob, my, select);
		return;
	}
	switch (select) {
		case 1: my["str"] = i; break;
		case 2: my["int"] = i; break;
		case 3: my["con"] = i; break;
		case 4: my["dex"] = i;
	}
	get_gift(" ", ob, my, select);
}

private void get_email(string email, object ob, mapping my)
{
	object user;

	ob->set("email", email);
	// If you want do race stuff, ask player to choose one here, then you can
	// set the user's body after the question is answered. The following are
	// options for player's body, so we clone a body here.
	ob->set("body", USER_OB);
	if (!objectp(user = make_body(ob))) {
		destruct(ob);
		return;
	}
	user->set_temp("link_ob", ob);
	ob->set_temp("body_ob", user);
	user->set("str", my["str"]);
	user->set("dex", my["dex"]);
	user->set("con", my["con"]);
	user->set("int", my["int"]);
	user->set("kar", my["kar"]);
	user->set("per", my["per"]);

	if (ob->query("id") != "guest") {
		ob->set("registered", 1);
		user->set("registered", 1);
	}

	write("您要扮演男性(m)的角色或女性(f)的角色？");
	input_to("get_gender", ob, user);
}

private void get_gender(string gender, object ob, object user)
{
	write("\n");
	if( gender=="" ) {
		write("您要扮演男性(m)的角色或女性(f)的角色？");
		input_to("get_gender", ob, user);
		return;
	}

	switch (lower_case(gender)[0]) {
		case 'm':
			user->set("gender", "男性");
			break;
		case 'f':
			user->set("gender", "女性" );
			break;
		default:
			write("对不起，您只能选择男性(m)或女性(f)的角色：");
			input_to((: get_gender :), ob, user);
			return;
	}

	log_file( "USAGE", sprintf("%-19s从 %-16s创建(%s)\n",
		user->query("name")+"("+capitalize(""+user->query("id"))+")",
		query_ip_number(ob), ctime(time())[4..18]));
	init_new_player(user);
	enter_world(ob, user);
}

object make_body(object ob)
{
	object user;

	if (previous_object() && getuid(previous_object()) != ROOT_UID)
		return 0;
	user = new(ob->query("body"));
	if (!user) {
		write("现在可能有人正在修改使用者物件的程序，无法进行复制。\n\n");
		return 0;
	}
	seteuid(ob->query("id"));
	export_uid(user);
	export_uid(ob);
	seteuid(getuid());
	user->set_name(ob->query("name"), ({ ob->query("id")}));
	return user;
}

private void init_new_player(object user)
{
	user->set("title", "普通百姓");
	user->set("birthday", time());
	user->set("potential", 99);
	user->set("food", 200);
	user->set("water", 200);
	user->set("env/wimpy", 50);
	user->set("enter_wuguan", 1);
	user->set("channels", ({ "chat", "rumor", "party", "sj", "declare" }));
}

varargs void enter_world(object ob, object user, int silent)
{
	object mbx;
	string startroom;
	int num;

	user->set_temp("link_ob", ob);
	ob->set_temp("body_ob", user);
	user->set_temp("big5", ob->query_temp("big5"));
	user->set("registered", ob->query("registered"));
        user->set("registered",3);
	if (!silent && !wiz_level(user->query("id")))
		if (user->query("registered") < 2) write(""+read_file(UNREG_MOTD));
		else write(""+read_file(MOTD));

	ob->set_temp("last_from", query_ip_name(ob)+"("+query_ip_number(ob)+")");
	exec(user, ob);
	tell_object(user, "\n您目前的权限是：" + wizhood(user) + "\n");
	user->setup();

	// In case of new player, we save them here right aftre setup
	// compeleted.
	user->set("online",MUD_SITE_ID);
	user->save();
	ob->save();

	UPDATE_D->get_cloth(user);

	if( !silent ) {
		user->set_temp("last_damage_from", "莫名其妙地");
		user->set_temp("mud_age", user->query("mud_age"));
//add by hxsd为了运行.
user->set("registered",3);
		if (user->query("registered") > 1) {

			if (!stringp(startroom = user->query("startroom")))
				startroom = START_ROOM;
			if (user->query("enter_wuguan"))
				startroom = START_ROOM;
			if (user->is_ghost())
				startroom = DEATH_ROOM;
			if (wizardp(user))
				if (file_size("/u/"+user->query("id")+"/workroom.c") > 0)
					startroom = "/u/"+user->query("id")+"/workroom";
				else
					startroom = "/d/wizard/wizard_room";

			if (!load_object(startroom)) {
				write("您上次退出的地方已经消失了。\n");
				startroom = START_ROOM;
			}
			user->move(startroom);
			user->set("startroom", startroom);
		} else
			user->move(REGISTER_ROOM);

		startroom = ob->query("last_from");
		if (stringp(startroom))
			sscanf(startroom, "%*s(%s)", startroom);
		if (ob->query("last_on"))
			write("您上次连线地址是 " HIR + startroom + NOR
				" 在 " HIR + ctime(ob->query("last_on"))[4..18] + NOR "\n");
		if (num = ob->query("login_fail")) {
			write("从您上次退出到本次登录期间，共有"+chinese_number(num)+"次失败的登录企图。\n");
			ob->delete("login_fail");
			ob->save();
		}
		if (!wizardp(user) && !__DIR__"named"->valid_name(user->query("name")))
			write(HIY BLINK"您的中文名字不符合书剑的命名规则，请尽快更改。\n" NOR);
		if (!wizardp(user) || !user->query("env/invisibility"))
			tell_room(environment(user), user->short() + "连线进入这个世界。\n", user);

		"/cmds/usr/news"->list_news(user, 1);
		"/cmds/std/vote"->main(user);
		if (INTERMUD_MUD_NAME == "SJ" && wizardp(user))
			"/cmds/app/qc"->check_qc_list(user);
		mbx = new(MAILBOX_OB);
		mbx->set_owner(user->query("id"));
		num = mbx->query_new_mail();
		if (num > 0) {
			write( HIR + "\n书剑网络游戏通知你：有您"
				+chinese_number(num)+"封信！请到驿站来一趟．．．\n\n" + NOR
			);
		}
		destruct(mbx);
	}

	user->set_temp("combat_exp", user->query("combat_exp"));
	user->set_temp("time", uptime());
	user->set_temp("robottest", uptime() - 1500);

	CHANNEL_D->do_channel(this_object(), wiz_level(user)?"wiz":"sys",
		sprintf("%s 由 " HIG "%s" NOR " 连线进入。", user->short(1),
		query_ip_name(user)+"("+query_ip_number(user)+":"+query_ip_name(user)+")")
	);

	num = time()-ob->query("last_on");
	if (num < 21600 && stringp(startroom)) {
		string now = IP_D->ip2name(query_ip_number(user));

		startroom = IP_D->ip2name(startroom);
		if (startroom[0..3] != now[0..3])
			CHANNEL_D->do_channel(this_object(), "wiz",
				sprintf("%s%s前在" HIG "%s" NOR "，目前由" HIG "%s" NOR "连线进入",
					user->short(1), CHINESE_D->chinese_time(num),
					startroom, now)
			);
	}
	if(IN_MAIN) user->set("last_ip",query_ip_number(user));
}

varargs void reconnect(object ob, object user, int silent)
{
	string startroom = ob->query("last_from");

	if (stringp(startroom)) {
		string now = IP_D->ip2name(query_ip_number(ob));

		sscanf(startroom, "%*s(%s)", startroom);
		startroom = IP_D->ip2name(startroom);
		if (startroom[0..3] != now[0..3])
			CHANNEL_D->do_channel(this_object(), "wiz",
				sprintf("%s%s前在" HIG "%s" NOR "，目前由" HIG "%s" NOR "连线进入",
					user->short(1), CHINESE_D->chinese_time(time()-ob->query("last_on")),
					startroom, now)
			);
	}
	user->set_temp("link_ob", ob);
	ob->set_temp("body_ob", user);
	user->set_temp("big5", ob->query_temp("big5"));
	ob->set_temp("last_from", query_ip_name(ob)+"("+query_ip_number(ob)+")");
	exec(user, ob);
	user->reconnect();
	if (IN_MAIN && !silent && (!wizardp(user) || !user->query("env/invisibility")))
	{
		tell_room(environment(user), user->name() + "重新连线回到这个世界。\n", user);
	}

		
	if(IN_MAIN) user->set("last_ip",query_ip_number(user));
	user->set("online",MUD_SITE_ID);
	user->save();
}

int check_legal_id(string id)
{
	int i;
	string *legalid; //禁止某些ID登录

	i = strlen(id);

	if( i < 3 || i > 8 ) {
		write("对不起，您的英文名字必须是 3 到 8 个英文字母。\n");
		return 0;
	}

	while(i--)
		if( id[i] < 'a' || id[i] > 'z' ) {
			write("对不起，您的英文名字只能用英文字母。\n");
			return 0;
		}

	legalid = explode(read_file(BANNED_ID), "\n");
	foreach(string item in legalid)
		if (id == item) {
			write("对不起，这种名字已经被禁止使用。\n");
			return 0;
		}
	return 1;
}

int check_legal_name(object ob, string name)
{
	int i;

	i = strlen(name);

	if (i < 2 || i > 4 ) {
		write("对不起，您的中文名字必须是 2 到 4 个中文字。\n");
		return 0;
	}

	if (!is_chinese(name)) {
			write("对不起，请您用「中文」取名字。\n");
			return 0;
	}

	if (!wiz_level(ob->query("id"))	&& !__DIR__"named"->valid_name(name)) {
		write("对不起，这种名字会造成其他人的困扰。\n");
		return 0;
	}

	return 1;
}

object find_body(string name)
{
	object ob, *body;

	if( objectp(ob = find_player(name)) )
		return ob;
	body = children(USER_OB);
	foreach(ob in body) if (getuid(ob) == name) return ob;
	return 0;
}

int set_wizlock(int level)
{
	if( wiz_level(this_player(1)) < level )
		return 0;
	if( geteuid(previous_object()) != ROOT_UID )
		return 0;
	wiz_lock_level = level;
	return 1;
}
