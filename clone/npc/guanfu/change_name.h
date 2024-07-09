// change_name.h  for change name npc ps: zhaochengzhi.c
// Lklv change to thisfile for zhao

// Modified by mxzhao 2004/02/18
// Modified by lInuX 2004/04/01
#include <database.h>
#include <ansi.h>

void init()
{
	add_action("change_name", "zhuce");
	add_action("change_title", "change");
}

string delansi(string str)
{
   int is_ansi,ansi_len,i;
   //string ruslt;
   is_ansi=0;
   ansi_len=0;
   
   for (i=0;i<strlen(str);i++){     
     if (str[i]==27)
    	 is_ansi=1;
     if (str[i]==109 && is_ansi==1){ 
     	 str[i]=37;
     	 is_ansi=0;
     }
     if (is_ansi==1){
     	 ansi_len++;
       str[i]=37;
     }     
   }
	 return replace_string(str,"%","");	
}

private int change_name(string arg)
{
	string new_name, name, f_mname, f_class;
	int i, f_gen;
	object me = this_player();

	if (!me->query_temp("name_modify_paid") && !wizardp(me)) {
		command("say 朝廷明文规定，平民不得随意改名换姓，你想坐牢是不是？");
		return 1;
	}

	if ( !arg ) {
		command("say 你到底要改什么名字啊？");
		return 1;
	}

	name = me->query("name");
	new_name = arg;
	f_gen = me->query("family/generation");
	f_class = me->query("class");
	f_mname = me->query("family/master_name");

	if (me->query("registered") < 4)	{
		switch(me->query("family/family_name")){
			case "桃花岛":
				if (f_gen == 2) {
					if (strlen(arg) != 3 || arg[2..2] != "风")
						return notify_fail("你的名字必须是三个字，而且以风结尾。\n");
				}
				break;
			case "峨嵋派":
			case "天龙寺":
			case "少林派":
				if (f_class == "bonze" || f_class == "huanxi") {	// 给敌人一个花钱去掉 huanxi 的机会
					if (strlen(arg) != 4)
						return notify_fail("你的名字必须是两个字。\n");
					switch (f_gen) {
						case 40:
							if (arg[0..0] == "清")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 39:
							if (arg[0..0] == "道")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 38:
							if (arg[0..0] == "慧")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 37:
							if (arg[0..0] == "澄")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 36:
							if (arg[0..0] == "玄")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 35:
							if (arg[0..0] == "渡")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 14:
							if (arg[0..0] == "了")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 13:
							if (arg[0..0] == "本")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 5:
							if (arg[0..0] == "文")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 4:
							if (arg[0..0] == "静")
								break;
							return notify_fail("辈分不对，重来！\n");
						case 3:
							if (arg[0..0] == "灭")
								break;
							return notify_fail("辈分不对，重来！\n");
						default:
							if (name[0..0] != arg[0..0])
								return notify_fail("你的法号必须是两个字，而且第一个字不能变。\n");
					}
					if (f_class == "huanxi")
						me->set("class", "bonze");
				} else if (f_mname == "段正明")
					if (strlen(arg) > 3 || arg[0..0] != "段")
						return notify_fail("你的姓名必须是两或三个字，而且第一个字必须为段。\n");
				break;
			case "星宿派":
				if (f_gen == 2) {
					if (strlen(arg) != 3 || arg[2..2] != "子")
						return notify_fail("你的名字必须是三个字，而且以子结尾。\n");
				}
				break;
			case "姑苏慕容":
				if (f_gen == 2) {
					if (strlen(arg) < 3 || arg[0..1] != "慕容")
						return notify_fail("你的名字必须至少三个字，而且以慕容开头。\n");
				}
		}
  }
	i = strlen(new_name);

	if (i < 2 || i > 4 ) {
		command("say "+new_name+"？");
		command("say 你的中文名字必须是 2 到 4 个中文字才行。");
		return 1;
	}


	if( !is_chinese(new_name) ) {
		command("say 你得用「中文」取名字，本官我可不认识西洋字。");
		return 1;
	}

	if (!wiz_level(me->query("id"))
	&& (!"/adm/daemons/named"->valid_name(new_name) ) ){
		command("say 你这种名字会造成其他人的困扰，想个其它的。");
		return 1;
	}

// Modified by mxzhao 2004/02/18
	{
		mixed ret = dbquery("select U_Name from Users where U_Name='"+new_name+"'");
		if (!ret)
		{
			return notify_fail("暂时不能修改姓名。\n");
		}

		if (sizeof(ret) > 0)
		{
			return notify_fail("已经有其他玩家使用了这个姓名。\n");
		}

		if (!dbquery("update Users set "
			"U_Name='"+new_name+"' "
			"where U_Username='"+me->query("id")+"' limit 1"))
		{
			return notify_fail("数据库更新失败。\n");
		}
//		if (!BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "UPDATE members SET cname = 
//		'"+new_name+"' WHERE username = '"+
//		me->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"' limit 1"))
//		{
//			return notify_fail("WEB数据库更新失败。\n");
//		}
	}
	
/*
	{
		mixed mysql = db_connect("localhost","mud","root");

		if (!intp(mysql))
			return notify_fail("暂时不能修改姓名。\n");
		if (db_exec(mysql, "select U_Name from " + INTERMUD_MUD_NAME + "_Users where U_Name='"+new_name+"'")) {
			db_close(mysql);
			return notify_fail("已经有其他玩家使用了这个姓名。\n");
		}
		db_exec(mysql, "update " + INTERMUD_MUD_NAME + "_Users set "
			"U_Name='"+new_name+"' "
			"where U_Username='"+me->query("id")+"' limit 1"
		);
		db_close(mysql);
	}
*/
// End

	me->set("name", new_name);
	me->add("name_modify", 1);
	command("chat 从今以后，"+name+"("+getuid(me)+") 改姓名为：" + new_name + "。");
	log_file("name_modify",
		sprintf("%-18s 第 %d 次改名为：%s\n",
			name+"("+getuid(me)+")", me->query("name_modify"), new_name
		), me
	);
	me->delete_temp("name_modify_paid");
	start_busy(2);
	if (new_name == name)
		command("say 你可真是有钱找我开心来着，改了个一模一样的名字。");
	command("say 你的户籍手续已经办理完毕，你可以走了。");
	return 1;
}

string ask_me()
{
	object me = this_player();
	string mname = (string)me->query("name");

	if(me->query_temp("name_modify_paid")){
		command("nod");
		return "好吧，你只要注册( zhuce <姓名> )你的新名字就行了。\n";
	}

	if(mname == "名字待定"){
		me->set_temp("name_modify_paid", 1);
		return "此民无名无姓，本官同意改名，你要注册( zhuce <姓名> )什么名字？";
	}

	if( this_object()->is_busy())
		return "我正忙着改写户册呢，你等等。";

	if(me->query_temp("name_wait_modify")){
	       command("hmm");
	       return "不是告诉了你，这事不那么好办吗？\n";
	}

	if (!wiz_level(me->query("id"))
	&& (!"/adm/daemons/named"->valid_name(mname) )) {
		command("pat");
		me->set_temp("name_modify_paid", 1);
		return "我已经得到通知，不收你的改名费用，你要换个( zhuce <姓名> )什么名字？";
	}
	command("consider");
	command("say "+me->query("name", 1)+"你有名有姓而要修改户籍和姓名，本官觉得这事很难啊......\n");
	me->set_temp("name_wait_modify", 1);
	if(me->query("class") == "bonze")
		command("say 对了，和尚，尼姑可改不了辈分称号。\n\t比如你叫 本一，就必须 zhuce 以本字开头的名字。");
	if( me->query("registered") > 2 )
		return "如果你坚持要改，鉴于你是贵宾玩家，上报和入册费用是"+chinese_number(100*(me->query("name_modify")+1))+"两黄金，交到张帐房那里。\n";
	else
		return "如果你坚持要改，上报和入册费用是"+chinese_number(200*(me->query("name_modify")+1))+"两黄金，交到张帐房那里。\n";

}	

int change_title(string arg)
{
	string new_name, name,str;
	int i;
	object me = this_player();

	if (!me->query_temp("title_modify_paid")) {
		command("say 朝廷明文规定，平民不得随意改名换姓，你想坐牢是不是？");
		return 1;
	}

	if ( !arg ) {
		command("say 你到底要改什么称号啊？");
		return 1;
	}
 
  arg = replace_string(arg, "$red$", RED);
  arg = replace_string(arg, "$grn$", GRN);
  arg = replace_string(arg, "$yel$", YEL);
  arg = replace_string(arg, "$blu$", BLU);
  arg = replace_string(arg, "$mag$", MAG); 
  arg = replace_string(arg, "$cyn$", CYN);
  arg = replace_string(arg, "$wht$", WHT);
  arg = replace_string(arg, "$hir$", HIR);
  arg = replace_string(arg, "$hig$", HIG);
  arg = replace_string(arg, "$hiy$", HIY);
  arg = replace_string(arg, "$hib$", HIB);
  arg = replace_string(arg, "$him$", HIM);
  arg = replace_string(arg, "$hic$", HIC);
  arg = replace_string(arg, "$hiw$", HIW);
  arg += NOR;
        
	name = me->query("name");
	new_name = arg;


  str = delansi(arg); 
	i = strlen(str);

	if (i < 4 || i > 10 ) {
		command("say "+str+"？");
		command("say 你的称号必须是 4 到 10 个中文字才行。");
		return 1;
	}


	if(  !is_chinese(str) ) {
		command("say 你得用「中文」取称号，本官我可不认识西洋字。");
		return 1;
	}

	if (!wiz_level(me->query("id"))
	&& !"/adm/daemons/named"->valid_name(str)) {
		command("say 你这种称号会造成其他人的困扰，想个其它的。");
		return 1;
	}

	me->set("title", new_name);
	me->add("title_modify", 1);
	command("chat 从今以后，"+name+"("+getuid(me)+") 改称号为：" + new_name + "。");
	log_file("title_modify",
		sprintf("%-18s 第 %d 次改名为：%s\n",
			name+"("+getuid(me)+")", me->query("title_modify"), new_name
		), me
	);
	me->delete_temp("title_modify_paid");
	start_busy(2);
	command("say 你的修改手续已经办理完毕，你可以走了。");
	return 1;
}

string ask_me1()
{
	object me = this_player();

	if(me->query("registered")<4){
		return "你想干什么。\n";
	}
	
	if(me->query_temp("title_modify_paid")){
		command("nod");
		return "好吧，你只要改变( change <称号> )你的新称号就行了。\n";
	}


	if( this_object()->is_busy())
		return "我正忙着改写户册呢，你等等。";

	if(me->query_temp("title_wait_modify")){
	       command("hmm");
	       return "不是告诉了你，这事不那么好办吗？\n";
	}

	command("consider");
	command("say "+me->query("name", 1)+"你有门有派而要修改称号，本官觉得这事很难啊......\n");
	me->set_temp("title_wait_modify", 1);
	return "如果你坚持要改，上报和入册费用是"+chinese_number(200*(me->query("title_modify")+1))+"两黄金，交到张帐房那里。\n";
}