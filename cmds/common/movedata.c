// movedata.c
// By River@SJ 2002.4.5
// YUJ@SJ 2002-04-08

inherit F_CLEAN_UP;

#define NID_PATH "/u/river/data/"
#define ID_PATH "/data/"
#define ODB "/clone/user/olduser"

int check_legal_id(string id)
{
	int i;

	i = strlen(id);
	if( i < 3 || i > 8 ) {
		write("对不起，新ID必须是 3 到 8 个英文字母。\n");
		return 0;
	}
	while(i--)
	if( id[i] < 'a' || id[i] > 'z' ) {
		write("对不起，新ID只能用小写英文字母。\n");
		return 0;
	}
	return 1;
}

int check_legal_email(string email)
{
	if (sscanf(email, "%*s@%*s.%*s") == 3) return 1;
	write("email 格式不对。\n");
	return 0;
}

mapping olddb, newdb;

varargs void dup(string key)
{
	newdb[key] = olddb[key];
}

int main(object me, string arg)
{
	string id, nid, email, status, msg, oldemail, passwd;
	mapping job;
	int i, j, exp, q, days;

	seteuid(ROOT_UID);

	if (!arg) return notify_fail("指令格式：movedata <ID> [new ID] <email>  <days>\n");

	if (sscanf(arg, "%*s %*s %*s %*s %*s") == 5)
		return notify_fail("指令格式：movedata <ID> [new ID] <email> <days>\n");
	if (sscanf(arg, "%s %s %s %d", id, nid, email, days) == 4)
		q = 0;
	else if (sscanf(arg, "%s %s %d", id, email, days) == 3) {
		nid = id;
		q = 1;
	} else return notify_fail("指令格式：movedata <ID> [new ID] <email> <days>\n");

	if (!check_legal_id(nid))
		return notify_fail("指令格式：movedata <ID> [new ID] <email> <days>\n");

	if (!check_legal_email(email))
		return notify_fail("指令格式：movedata <ID> [new ID] <email> <days>\n");

	if (days < 30) {
		write("有效天数不正确。\n");
		return notify_fail("指令格式：movedata <ID> [new ID] <email> <days>\n");
	}

	if (!ODB->restore(NID_PATH +"login/"+ id[0..0] +"/"+ id + SAVE_EXTENSION))
		return notify_fail(capitalize(id) +" 登录档案不存在。\n");

	if (!ODB->restore(NID_PATH +"user/"+ id[0..0] +"/"+ id + SAVE_EXTENSION))
		return notify_fail(capitalize(id) +" 数据档案不存在。\n");

	if (file_size(ID_PATH +"login/"+ nid[0..0] +"/"+nid + SAVE_EXTENSION) != -1)
		return notify_fail(capitalize(nid) + " 这个用户已经存在了。\n");

	olddb = ODB->query_dbase();
	newdb = ([ ]);

	// hp 部分, max_jing max_qi eff_jingli 由系统自动计算
	dup("jing");
	dup("eff_jing");

	dup("jingli");
	dup("max_jingli");

	dup("qi");
	dup("eff_qi");

	dup("neili");
	dup("max_neili");
	if ( olddb["max_neili"] > 8500 )
		newdb["max_neili"] = 8500;

	dup("shen");

	dup("food");
	dup("potential");
	dup("water");
	dup("combat_exp");

	// score 部分
	if ( olddb["class"] && olddb["class"] != "eunuch")
		dup("class");
	dup("title");
	dup("str");
	dup("con");
	dup("name");
	newdb["id"] = nid;
//	dup("id");
	dup("dex");
	dup("int");

	dup("nickname");
	dup("mud_age");
	dup("birthday");

	dup("gender");
	if (newdb["gender"] != "女性")
		newdb["gender"] = "男性";

	newdb["mud_time"] = olddb["mud_age"];
	newdb["startroom"] = "/d/city/kedian/bonus_room";

	dup("PKS");
	dup("MKS");
	dup("BKS");
	dup("GKS");
	dup("death_count");
	dup("death_times");
	dup("balance");
	if (olddb["balance"] > 10000000)
		newdb["balance"] = 10000000;

	if (mapp(olddb["family"]))
		newdb["family"] = copy(olddb["family"]);

	dup("per");
	dup("kar");
	dup("pur");

	newdb["oldsj"] = 1;
	newdb["name"] = "名字待定";

	if ( olddb["luohan_winner"] )
		newdb["luohan_winner"] = 1;

	if ( olddb["muren_winner"] )
		newdb["muren_winner"] = 1;

	if ( olddb["tls"] )
		newdb["tls"] = 1;

	if ( olddb["xie_qishang"] )
		newdb["xie_qishang"] = 1;
	
	if ( olddb["gb_bags"] )
		newdb["gb_bags"] = 1;

// Job time
	newdb["job_time"] = ([ ]);

	if (olddb["dali_job"])
		newdb["job_time"]["大理"] = olddb["dali_job"];
	if (olddb["gb_job"])
		newdb["job_time"]["丐帮"] = olddb["gb_job"];
	if (olddb["gb_job2"])
		newdb["job_time"]["报效国家"] = olddb["gb_job2"];
	if (olddb["gf_job"])
		newdb["job_time"]["官府"] = olddb["gf_job"];
	if (olddb["hs_job"])
		newdb["job_time"]["华山"] = olddb["hs_job"];
	if (olddb["mingjiao_job"])
		newdb["job_time"]["明教"] = olddb["mingjiao_job"];
	if (olddb["mr_job"])
		newdb["job_time"]["慕容"] = olddb["mr_job"];
	if (olddb["sl_job"])
		newdb["job_time"]["少林"] = olddb["sl_job"];
	if (olddb["snake_job"])
		newdb["job_time"]["抓蛇"] = olddb["snake_job"];
	if (olddb["songshan_points"])
		newdb["job_time"]["嵩山"] = olddb["songshan_points"];
	if (olddb["thd_job"])
		newdb["job_time"]["桃花岛"] = olddb["thd_job"];
	if (olddb["wd_job"])
		newdb["job_time"]["武当"] = olddb["wd_job"];
	if (olddb["xs_job"])
		newdb["job_time"]["雪山"] = olddb["xs_job"];
	if (olddb["xx_job4"])
		newdb["job_time"]["星宿熬膏"] = olddb["xx_job4"];
	if (olddb["xx_job5"])
		newdb["job_time"]["星宿叛徒"] = olddb["xx_job5"];
	if (olddb["xx_points"])
		newdb["job_time"]["星宿"] = olddb["xx_points"];
	if (olddb["zhao_job"])
		newdb["job_time"]["赵敏"] = olddb["zhao_job"];

// 经验和任务次数，年龄等转换
	if ((exp = newdb["combat_exp"]) > 3000000) {
		float scale = 3000000.0 / exp;

		newdb["combat_exp"] = 3000000;
		newdb["mud_age"] = to_int(newdb["mud_age"] * scale);
		newdb["PKS"] = to_int(newdb["PKS"] * scale);
		newdb["BKS"] = to_int(newdb["BKS"] * scale);
		newdb["GKS"] = to_int(newdb["GKS"] * scale);
		newdb["MKS"] = to_int(newdb["MKS"] * scale);
		newdb["death_count"] = to_int(newdb["death_count"] * scale);
		newdb["death_times"] = to_int(newdb["death_times"] * scale);
		if (job = newdb["job_time"]) {
			string *jobs = keys(job);
			i = sizeof(jobs);
			for (j = 0; j < i; j ++)
				job[jobs[j]] = to_int(job[jobs[j]] * scale);
		}
	}

	if ( newdb["mud_age"] > 7840000 )
		newdb["mud_age"] = 7840000;

	ODB->set_dbase(newdb);

	status = read_file(NID_PATH +"login/"+ id[0..0] +"/"+ id + SAVE_EXTENSION);

	sscanf(status, "%*s\"email\":\"%s\",", oldemail);
	status = replace_string(status, "\"email\":\""+oldemail+"\",", "\"email\":\""+email+"\",");

	sscanf(status, "%*s\"password\":\"%s\",", passwd);
	if (q)
		msg = "玩家 "+ capitalize(id) +" 已经更新完毕。\n";
	else {
		status = replace_string(status, "\"id\":\""+id+"\",", "\"id\":\""+nid+"\",");
		msg = "玩家 "+ capitalize(id) +" 更新完毕，更换 ID 为 "+ capitalize(nid) +" 。\n";
	}

	if ( ! write_file(NID_PATH +"login/"+ id[0..0] +"/"+ id + SAVE_EXTENSION, status, 1))
		return notify_fail("文件 "+ NID_PATH +"login/"+ id[0..0] +"/"+ id + SAVE_EXTENSION +" 存储失败。\n");

	if (!cp(NID_PATH +"login/" + id[0..0] +"/"+ id + SAVE_EXTENSION, ID_PATH +"login/"+ nid[0..0] +"/"+nid + SAVE_EXTENSION)
	|| !ODB->save(ID_PATH +"user/" + nid[0..0] +"/"+nid + SAVE_EXTENSION))
		return notify_fail("存储文件失败。\n");		// 写数据失败

	write(msg);
	i = db_connect("localhost","mud","root");
	if (!intp(i)) return notify_fail("连接数据库失败："+i+"。\n");
	j = db_exec(i,"replace into Users set "
		"U_Username='"+nid+"',"
		"U_Name='"+newdb["name"]+"',"
		"U_Password='"+passwd+"',"
		"U_Email='"+email+"',"
		"U_Site='BX',"
		"Mud_Site='BX',"
		"U_Online='N',"
		"Alivedays="+save_variable(days)
	);
	db_close(i);
	if (!intp(j)) return notify_fail("数据库操作失败："+j+"\n");
	write(sprintf("%s -> %s, %s, %d\n", id, nid, email, days));
// log file
	log_file("static/MOVEDATA",
		sprintf("%8s%-10s 将 %-8s 更改为 %-8s, %s, %d\n",
			me->query("name"), "("+ geteuid(me)+")", id, nid, email, days), me);
	return 1;
}

int help()
{
	write(@HELP

指令格式：movedata <ID> [New ID] <email> <days>

此指令用来更改玩家数据，如果 New ID 空缺，就不换 ID。

HELP
	);
	return 1;
}
                     
