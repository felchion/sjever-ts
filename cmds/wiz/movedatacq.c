// movedata.c
// By River@SJ 2002.4.5
// YUJ@SJ 2002-04-08
// Ciwei@SJ string SITE_NAME = "LN" 转换其他站点时 请修改此变量 重新编辑使用

#include <ansi.h>

inherit F_CLEAN_UP;

#define NID_PATH "/log/cq/newdata/"
#define ID_PATH "/log/cq/data/"
#define ODB "/clone/user/olduser"
//#define SITE_NAME "LN"

string SITE_NAME = "CQ";

mapping olddb, newdb;

static object os_user;
static int super_debug;

int help(object me);

varargs void dup(string key)
{
	newdb[key] = olddb[key];
}

int savedata(string id)
{
	string nid;
	string status;
	string email;
	string passwd;
	string* jobs;
	
	mapping job;	
	mixed ret;

	int i,j,exp;

	seteuid(ROOT_UID);

	nid = id;

	ODB->restore(ID_PATH + "user/" + id[0..0] + "/" + id);
	olddb = ODB->query_dbase();
	newdb = ([ ]);
	//应该分开ln cq都执行的 ln执行的 两部分
	//任务标记转换 -> 公用部分 ^_^
	// Job time
	//一些无job_time的转换
	if(olddb["job_time"]) newdb["job_time"] = copy(olddb["job_time"]);
	else newdb["job_time"] =([]);
	if (olddb["dali_job"])
		newdb["job_time"]["送信"] += olddb["dali_job"];
	if (olddb["gb_job"])
		newdb["job_time"]["丐帮"] += olddb["gb_job"];
	if (olddb["gb_job2"])
		newdb["job_time"]["报效国家"] += olddb["gb_job2"];
	if (olddb["gf_job"])
		newdb["job_time"]["官府"] += olddb["gf_job"];
	if (olddb["hs_job"])
		newdb["job_time"]["华山"] += olddb["hs_job"];
	if (olddb["mingjiao_job"])
		newdb["job_time"]["明教"] += olddb["mingjiao_job"];
	if (olddb["mr_job"])
		newdb["job_time"]["慕容"] += olddb["mr_job"];
	if (olddb["sl_job"])
		newdb["job_time"]["少林救援"] += olddb["sl_job"];
	if (olddb["snake_job"])
		newdb["job_time"]["抓蛇"] += olddb["snake_job"];
	if (olddb["songshan_points"])
		newdb["job_time"]["嵩山"] += olddb["songshan_points"];
	if (olddb["thd_job"])
		newdb["job_time"]["桃花岛"] += olddb["thd_job"];
	if (olddb["wd_job"])
		newdb["job_time"]["武当"] += olddb["wd_job"];
	//if (olddb["xs_job"])
		//newdb["job_time"]["雪山"] = olddb["xs_job"];ln cq code 都已经不存在xs_job 这个变量
	if (olddb["xx_job4"])
		newdb["job_time"]["星宿熬药"] += olddb["xx_job4"];
	if (olddb["xx_job5"])
		newdb["job_time"]["星宿叛徒"] += olddb["xx_job5"];
	if (olddb["xx_points"])
		newdb["job_time"]["星宿"] += olddb["xx_points"];
	if (olddb["zhao_job"])
		newdb["job_time"]["赵敏"] += olddb["zhao_job"];
	//一些错误job_name的修正
	if (job = newdb["job_time"]) {
			jobs = keys(job);
			for(i=0;i<sizeof(jobs);i++)
			{
				if(jobs[i]=="桃花岛守墓"){
					job["桃花岛"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="慕容偷学"){
					job["慕容"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="wd_job"){
					job["武当"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="丐帮吴"){
					job["丐帮"] = job[jobs[i]];
					map_delete(job,jobs[i]);
				}
			}
	}
	//因为ln转来的有cut exp 的方面所以job_last_gift就不复制
	if(SITE_NAME =="CQ"){
	if (mapp(olddb["job_last_gift"]))
		newdb["job_last_gift"] = copy(olddb["job_last_gift"]);
	if (job = newdb["job_last_gift"]) {
			jobs = keys(job);
			for(i=0;i<sizeof(jobs);i++)
			{
				if(jobs[i]=="桃花岛守墓"){
					job["桃花岛"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="慕容偷学"){
					job["慕容"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="wd_job"){
					job["武当"] = job[jobs[i]];
					map_delete(job,jobs[i]);
					continue;
				}
				if(jobs[i]=="丐帮吴"){
					job["丐帮"] = job[jobs[i]];
					map_delete(job,jobs[i]);
				}
			}
	}			
	}			
	//job部分结束
	// hp 部分, max_jing max_qi eff_jingli 由系统自动计算 公用部分
	dup("jing");
	dup("eff_jing");

	dup("jingli");
	dup("max_jingli");

	dup("qi");
	dup("eff_qi");

	dup("neili");
	dup("max_neili");
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
		
	dup("PKS");
	dup("MKS");
	dup("BKS");
	dup("GKS");
	dup("death_count");
	dup("death_times");
	dup("balance");
	
	dup("per");
	dup("kar");
	dup("pur");

	newdb["oldsj"] = 1;
	newdb["from_site"] = SITE_NAME;
	dup("name");
	newdb["mud_time"] = olddb["mud_age"];
	//newdb["name"] = "名字待定";
	//about party还有很多哦~~~
	//还有许多标记
	//分开两类 一类是 标记玩家是否受到过奖励 为了避免玩家重复得到奖励 这类标记一定得copy
	//         一类是 只复制cq ln不复制 包括quest 标记   
	
	//for all
	if(arrayp(olddb["channels"]))
		newdb["channels"] = copy(olddb["channels"]);
	
	if (mapp(olddb["marry"]))
		newdb["marry"] = copy(olddb["marry"]);
	if ( olddb["enter_wuguan"] )
		newdb["enter_wuguan"] = 1;
	if ( olddb["luohan_winner"] )
		newdb["luohan_winner"] = 1;
	if ( olddb["muren_winner"] )
		newdb["muren_winner"] = 1;
	if ( olddb["tls"] )
		newdb["tls"] = 1;
	if ( olddb["xie_qishang"] )
		newdb["xie_qishang"] = 1;	
	if ( olddb["gb_bags"] )
		newdb["gb_bags"] = copy(olddb["gb_bags"]);	
	if ( olddb["gb_pass"] )
		newdb["gb_pass"] = 1;
	if (mapp(olddb["family"]))
		newdb["family"] = copy(olddb["family"]);
	if (stringp(olddb["last_death"]))
		newdb["last_death"] = copy(olddb["last_death"]);
	if (stringp(olddb["last_killer"]))
		newdb["last_killer"] = copy(olddb["last_killer"]);
	if (olddb["gf_gives"])
		newdb["gf_gives"] = copy(olddb["gf_gives"]);
	//ss
	if (mapp(olddb["ss"]))
		newdb["ss"] = copy(olddb["ss"]);
	//gb
	if (mapp(olddb["gb"]))
		newdb["gb"] = copy(olddb["gb"]);
	if (mapp(olddb["htqg"]))
		newdb["htqg"] = copy(olddb["htqg"]);		
	///wd
	if (olddb["wudang_given"])
		newdb["wudang_given"] = copy(olddb["wudang_given"]);
	if (olddb["taiji_zsf"])
		newdb["taiji_zsf"] = copy(olddb["taiji_zsf"]);
	if (olddb["taiji_quest"])
		newdb["taiji_quest"] = copy(olddb["taiji_quest"]);
	//tls
	if (olddb["dudan_given"])
		newdb["dudan_given"] = copy(olddb["dudan_given"]);
	if (olddb["fenglu"])
		newdb["fenglu"] = copy(olddb["fenglu"]);
	if (olddb["kr_add"])
		newdb["kr_add"] = copy(olddb["kr_add"]);	
	//sl
	if (mapp(olddb["yjj"]))
		newdb["yjj"] = copy(olddb["yjj"]);
	//mj
	if (mapp(olddb["mjling"]))
		newdb["mjling"] = copy(olddb["mjling"]);
	//hs
	//if (olddb["fengset"])
		//newdb["fengset"] = copy(olddb["fengset"]);
	if (mapp(olddb["hs"]))
		newdb["hs"] = copy(olddb["hs"]);
	//gm
	if (olddb["gmpass"])
		newdb["gmpass"] = copy(olddb["gmpass"]);
	//mr	
	//em
	if (olddb["emsujia"])
		newdb["emsujia"] = copy(olddb["emsujia"]);
	//dls
	if (olddb["join_age"])
		newdb["join_age"] = copy(olddb["join_age"]);
	if (olddb["ryl_add"])
		newdb["ryl_add"] = copy(olddb["ryl_add"]);
	if(SITE_NAME =="CQ"){
	//just fo cq
	//gm	
	if (olddb["gmsanwu"])
		newdb["gmsanwu"] = copy(olddb["gmsanwu"]);
	if (olddb["ygpass"])
		newdb["ygpass"] = copy(olddb["ygpass"]);
	//quest	
	if (olddb["hanshui_food"])
		newdb["hanshui_food"] = copy(olddb["hanshui_food"]);
	if (olddb["menggu"])
		newdb["menggu"] = copy(olddb["menggu"]);
	//tls
	if (olddb["liumai_pass"])
		newdb["liumai_pass"] = copy(olddb["liumai_pass"]);
	if (mapp(olddb["liumai-shenjian"]))
		newdb["liumai-shenjian"] = copy(olddb["liumai-shenjian"]);
	if (stringp(olddb["qinwang"]))
		newdb["qinwang"] = copy(olddb["qinwang"]);
	//sl
	if (olddb["fenwo"])
		newdb["fenwo"] = copy(olddb["fenwo"]);
	if (olddb["pass_xin"])
		newdb["pass_xin"] = copy(olddb["pass_xin"]);
	//if (mapp(olddb["wxz"]))
		//newdb["wxz"] = copy(olddb["wxz"]);
	//thd
	if (mapp(olddb["thd"]))
		newdb["thd"] = copy(olddb["thd"]);				
	//else
	//if (mapp(olddb["forging"]))
		//newdb["forging"] = copy(olddb["forging"]);
	if (arrayp(olddb["friend_user"]))
		newdb["friend_user"] = copy(olddb["friend_user"]);
	if (mapp(olddb["env"]))
		newdb["env"] = copy(olddb["env"]);
	if (stringp(olddb["color"]))
		newdb["color"] = copy(olddb["color"]);	
	if (olddb["color_time"])
		newdb["color_time"] = copy(olddb["color_time"]);	
	if (olddb["name_modify"])
		newdb["name_modify"] = copy(olddb["name_modify"]);
	if (olddb["rules"])
		newdb["rules"] = copy(olddb["rules"]);
	}
	/*	
	if (mapp(olddb[""]))
		newdb[""] = copy(olddb[""]);
	*/			
	//LN并站的一些限制
	if(SITE_NAME =="LN"){
	if (olddb["color_time"])
		newdb["color_time"] = copy(olddb["color_time"])/2;
	if (olddb["name_modify"])
		newdb["name_modify"] = copy(olddb["name_modify"])/2;
	if (olddb["rules"])
		newdb["rules"] = copy(olddb["rules"])/2;
	//减半	
	if ( newdb["mud_age"] > 7840000 )
		newdb["mud_age"] = 7840000;
	if ( olddb["max_neili"] > 8500 )
		newdb["max_neili"] = 8500;
	if (olddb["balance"] > 10000000)
		newdb["balance"] = 10000000;
// 经验和任务次数，年龄等转换 just for ln
	//再加上literate
	ODB->set_skill("literate",(ODB->query_skill("literate")*2)/3 );
	newdb["combat_exp"] = copy(newdb["combat_exp"] / 5);
	newdb["mud_age"] = copy(newdb["mud_age"]/ 5 );
	newdb["PKS"] = copy(newdb["PKS"] / 5 );
	newdb["BKS"] = copy(newdb["BKS"] / 5);
	newdb["GKS"] = copy(newdb["GKS"] / 5);
	newdb["MKS"] = copy(newdb["MKS"] / 5);
	newdb["death_count"] = copy(newdb["death_count"] / 5);
	newdb["death_times"] = copy(newdb["death_times"] / 5);
	if (job = newdb["job_time"]) {
		jobs = keys(job);
		i = sizeof(jobs);
		for (j = 0; j < i; j ++){
				job[jobs[j]] = copy(job[jobs[j]] / 5);
				if(job[jobs[j]]==0) map_delete(job,jobs[j]);
			}
	}
		
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
			jobs = keys(job);
			i = sizeof(jobs);
			for (j = 0; j < i; j ++){
					job[jobs[j]] = to_int(job[jobs[j]] * scale);
					if(job[jobs[j]]==0) map_delete(job,jobs[j]);
				}
		}
	}
	}
	//about skill and master change
	ODB->delete_skill("jingmai-xue");
	ODB->delete_skill("lingbo-weibu");
	ODB->delete_skill("jiuyin-zhengong");
	ODB->delete_skill("jiuyin-shenfa");
	ODB->delete_skill("jiuyin-shenzhua");
	ODB->delete_skill("jiuyin-baiguzhua");
	ODB->delete_skill("jiuyin-shenzhang");
	ODB->delete_skill("cuixin-zhang");
	ODB->delete_skill("dafumo-quan");
	ODB->delete_skill("xuanyin-jian");
	ODB->delete_skill("xuanming-shenzhang");
	ODB->delete_skill("yinlong-bian");
	ODB->delete_skill("daode-jing");
	if ( ODB->query_skill("jiuyang-shengong") > ODB->query_skill("shenghuo-shengong"))
		ODB->set_skill("shenghuo-shengong", ODB->query_skill("jiuyang-shengong"));
	ODB->delete_skill("jiuyang-shengong");
	ODB->delete_skill("hamagong");
	ODB->delete_skill("jinshe-jianfa");
	ODB->delete_skill("jinshe-zhangfa");
	if ( ODB->query_skill("dugu-jiujian") > ODB->query_skill("huashan-jianfa"))
		ODB->set_skill("huashan-jianfa", ODB->query_skill("dugu-jiujian"));
	ODB->delete_skill("dugu-jiujian");
	ODB->delete_skill("hamabu");
	ODB->delete_skill("baituo-shezhang");
	ODB->delete_skill("liumai-shenjian");
	ODB->delete_skill("kongming-quan");
	ODB->delete_skill("wuxiang-zhi");
	ODB->delete_skill("lanhua-shou");
	ODB->delete_skill("pixie-jian");
	ODB->delete_skill("hunyuan-zhang");
	ODB->delete_skill("qianzhu-wandushou");
	ODB->delete_skill("xiantian-gong");
	
	//ODB->set_skill("duanzao", ODB->query_skill("forging"));
	ODB->delete_skill("forging");
	
	if(mapp(newdb["family"]) && newdb["family"]["master_name"]){
	switch(newdb["family"]["master_name"])
	{
		case "张无忌":		newdb["title"] = HIR"明教教主亲传弟子"NOR;	break;	// 明教
		case "无名老僧":	newdb["title"] = "少林派第三十五代弟子";		break;	// 少林派
		case "杨过":
		case "小龙女":		newdb["title"] = HIW"古墓派神雕侠侣传人"NOR;	break;	// 古墓派
		case "张三丰":		newdb["title"] = "武当派第二代弟子";		break;	// 武当派
		case "金轮法王":	newdb["title"] = HIY"大轮寺金轮法王座下弟子"NOR;	break;
		case "鸠摩智":		newdb["title"] = HIY"大轮寺掌门座下弟子"NOR;	break;
		case "血刀老祖":	newdb["title"] = HIR"大轮寺血刀老祖亲传弟子"NOR;	break;	// 大轮寺
		case "洪安通":		newdb["title"] = "神龙教第二代弟子";		break;	// 神龙教
		case "丁春秋":		newdb["title"] = "星宿派第二代弟子";		break;	// 星宿派
		case "慕容博":		newdb["title"] = "姑苏慕容第二代弟子";		break;	// 姑苏慕容
		case "上官剑南":	newdb["title"] = HIM"铁掌帮上官帮主弟子"NOR;	break;	// 铁掌帮
		//156: qinwang            : "承北王" 
		case "枯荣长老":	newdb["title"] = "天龙寺第十三代弟子";
					if(stringp(newdb["qinwang"]))
						newdb["title"] = sprintf(HIW"大理国%s"NOR,newdb["qinwang"]);
					break;	// 天龙寺
		case "风陵师太":
		case "孤鸿子":		newdb["title"] = "峨嵋派第三代弟子";		break;	// 峨嵋派
		case "左冷禅":		newdb["title"] = HIC"五岳盟主亲传弟子"NOR;	break;	// 嵩山派
		case "黄药师":		newdb["title"] = "桃花岛第二代弟子";		break;	// 桃花岛
		case "穆人清":		newdb["title"] = "华山派第十三代弟子";		break;
		case "风清扬":		newdb["title"] = "华山派第十三代弟子";
					newdb["family"]["master_name"]    =  "穆人清";
					newdb["family"]["master_id"]      =  "mu renqing";
					break;
		case "岳不群":
		case "宁中则":		newdb["title"] = "华山派第十四代弟子";		break;	// 华山派
		case "洪七公":
					newdb["family"]["master_name"]    =  "梁长老";
					newdb["family"]["master_id"]      =  "liang zhanglao";
					newdb["family"]["generation"]     =  19;
					if(!mapp(newdb["gb"])) newdb["gb"] = ([]);
					newdb["gb"]["fenduo"]             =  "总舵";
					newdb["class"]                    =  "beggar";
					if(!newdb["gb"]["bags"] || newdb["gb"]["bags"]<1 ) newdb["gb"]["bags"] = 1;
					newdb["title"]                    = sprintf("丐帮总舵%s袋弟子",
                								chinese_number(newdb["gb"]["bags"]));
                			break;
		}
		//default :		title = "none";				break;			
	}
	//考虑中
	if(olddb["registered"]>1) newdb["registered"]= 2;
	//if(SITE_NAME =="LN") newdb["startroom"] = "/d/city/kedian/bonus_room";
	//else 
	//newdb["startroom"] = "/d/city/kedian";
	dup("startroom");
			
	ODB->set_dbase(newdb);
	
	status = read_file(ID_PATH +"login/"+ id[0..0] +"/"+ id + SAVE_EXTENSION);

	sscanf(status, "%*s\"email\":\"%s\",", email);

	sscanf(status, "%*s\"password\":\"%s\",", passwd);
	ret = dbquery("SELECT Alivedays FROM "+SITE_NAME+"_Users "
	//ret = dbquery("SELECT Alivedays FROM Users "
		"WHERE U_Username=" + save_variable(nid));
	if (!ret) {
		log_file("static/MOVEDATA",sprintf("%-8s数据库操作失败 for Serch Users\n",nid) );
		tell_object(os_user,sprintf("%-8s数据库操作失败 for Serch Users\n",nid));
		return 0;
	}
	if(sizeof(ret)>0)
	{
		log_file("static/MOVEDATA", sprintf("%-8s 来自 %-4s 因为数据表 %s_Users 已经存在%-8s资料，转换失败", 
			nid, SITE_NAME,SITE_NAME,nid));
		tell_object(os_user,capitalize(id) +" 更新失败。\n");
		return 1;
	}
	//确认Users里没有数据才save 不会发生 Users里么  Data有的事吧？fear
	
	if ( !ODB->save(NID_PATH +"user/" + nid[0..0] +"/"+nid + SAVE_EXTENSION))
		{
		log_file("static/MOVEDATA",sprintf("%-8s数据库操作失败 for Data's User\n",nid));
		tell_object(os_user,sprintf("%-8s数据库操作失败 for Data's User\n",nid));
		return 0;
	}
	write_file(NID_PATH +"login/" + nid[0..0] +"/"+nid + SAVE_EXTENSION,status, 1);
		
	ret = dbquery("SELECT alivedays,question,answer,mail "
			"FROM "+lower_case(SITE_NAME)+"_qa_users WHERE id=" + save_variable(nid));
	if(ret && sizeof(ret)==1){
		
	if(ret[0][0]>0)
		{
		string site_name2 = lower_case(SITE_NAME);
		if(site_name2=="ln") site_name2 = sprintf("ln%d",2);
		dbquery("INSERT INTO mudvips "
			"(username,regtimelimit,updatetime,regemail) "
			"VALUES (\""+nid+"@"+site_name2+"\",\""+(time()+ret[0][0]*24*60*60)+"\",\""+time()+"\",\""+ret[0][3]+"\")"
			);
		//regtimelimit alivedays*24*60*60+time() 
		//updatetime time()
		//regtimelimit  updatetime 
		//
		}	
	if(super_debug){message("wizard:linux","replace into "+SITE_NAME+"_Users set "	
			"U_Username=\""+nid+"\","
			"U_Name=\""+newdb["name"]+"\","
			"U_Password=\""+passwd+"\","
			"U_Email=\""+email+"\","		
			"U_Site=\"" + SITE_NAME + "\","		
			"Mud_Site=\"" + SITE_NAME + "\","
			"U_Registered=\"" + newdb["birthday"] + "\","
			"U_Extra=\"" + newdb["gender"] + "\","
			"U_Online=\"N\","		
			"Question=\""+ret[0][1]+"\","
			"Answer=\""+ret[0][2]+"\"\n",users());
			
			ret = dbquery("replace into "+SITE_NAME+"_Users set "	
	//ret = dbquery("replace into Users set "
		"U_Username=\""+nid+"\","
		"U_Name=\""+newdb["name"]+"\","
		"U_Password=\""+passwd+"\","
		"U_Email=\""+email+"\","		
		"U_Site=\"" + SITE_NAME + "\","		
		"Mud_Site=\"" + SITE_NAME + "\","
		"U_Registered=\"" + newdb["birthday"] + "\","
		"U_Extra=\"" + newdb["gender"] + "\","
		"U_Online=\"N\","
		//"Alivedays='"+ret[0][0]+"',"
		"Question=\""+ret[0][1]+"\","
		"Answer=\""+ret[0][2]+"\""
		);
		
			
		}
	else ret = dbquery("replace into "+SITE_NAME+"_Users set "	
	//ret = dbquery("replace into Users set "
		"U_Username='"+nid+"',"
		"U_Name='"+newdb["name"]+"',"
		"U_Password='"+passwd+"',"
		"U_Email='"+email+"',"		
		"U_Site='" + SITE_NAME + "',"		
		"Mud_Site='" + SITE_NAME + "',"
		"U_Registered='" + newdb["birthday"] + "',"
		"U_Extra='" + newdb["gender"] + "',"
		"U_Online='N',"
		//"Alivedays='"+ret[0][0]+"',"
		"Question='"+ret[0][1]+"',"
		"Answer='"+ret[0][2]+"'"
		);	
	}
	else {
	if(super_debug){
		message("wizard:linux","replace into "+SITE_NAME+"_Users set "	
	//ret = dbquery("replace into Users set "
		"U_Username=\""+nid+"\","
		"U_Name=\""+newdb["name"]+"\","
		"U_Password=\""+passwd+"\","
		"U_Email=\""+email+"\","		
		"U_Site=\"" + SITE_NAME + "\","		
		"Mud_Site=\"" + SITE_NAME + "\","
		"U_Registered=\"" + newdb["birthday"] + "\","
		"U_Extra=\"" + newdb["gender"] + "\","
		"U_Online=\"N\","
		"Alivedays=\"0\"",users());
		
		ret = dbquery("replace into "+SITE_NAME+"_Users set "	
	//ret = dbquery("replace into Users set "
		"U_Username=\""+nid+"\","
		"U_Name=\""+newdb["name"]+"\","
		"U_Password=\""+passwd+"\","
		"U_Email=\""+email+"\","		
		"U_Site=\"" + SITE_NAME + "\","		
		"Mud_Site=\"" + SITE_NAME + "\","
		"U_Registered=\"" + newdb["birthday"] + "\","
		"U_Extra=\"" + newdb["gender"] + "\","
		"U_Online=\"N\","
		"Alivedays=\"0\"");
		
		
	}	
	else ret = dbquery("replace into "+SITE_NAME+"_Users set "	
	//ret = dbquery("replace into Users set "
		"U_Username='"+nid+"',"
		"U_Name='"+newdb["name"]+"',"
		"U_Password='"+passwd+"',"
		"U_Email='"+email+"',"		
		"U_Site='" + SITE_NAME + "',"		
		"Mud_Site='" + SITE_NAME + "',"
		"U_Registered='" + newdb["birthday"] + "',"
		"U_Extra='" + newdb["gender"] + "',"
		"U_Online='N',"
		"Alivedays='0'");
	}
	if (!ret) {
		log_file("static/MOVEDATA",sprintf("%-8s数据库操作失败 for Users\n",nid));
		tell_object(os_user,sprintf("%-8s数据库操作失败 for Users\n",nid));
		return 0;
	}
			
	status = replace_string(status, "", "");
	status = replace_string(status, "\\", "");
	status = replace_string(status, "\"", "");
	status = replace_string(status, "'", "");
	ret = dbquery("REPLACE "+SITE_NAME+"_Data SET "	
	//ret = dbquery("REPLACE Data SET "
		"U_ID=" + save_variable(sprintf(DATA_DIR "login/%c/%s%s", nid[0], nid,SAVE_EXTENSION)) + ","
		"U_Time=" + save_variable(time()) + ","
		"U_Data=" + save_variable(status));	
	if (!ret) {
		log_file("static/MOVEDATA",sprintf("%-8s数据库操作失败 for Data's Login\n",nid));
		tell_object(os_user,sprintf("%-8s数据库操作失败 for Data's Login\n",nid));
		return 0;
	}
	
	tell_object(os_user,capitalize(id) +" 更新完毕。\n");
	return 1;
}

int iso(string str)
{
	if(!str) return 0;
	return sscanf(str+"$","%s.o$", str)==1;
}

int main(object me, string arg)
{
	string uid;
	string dir;
	string *files;

	int count = 0;
	int dix = 0;
	int j = 1;
	
	super_debug=0;	
	os_user = me;	
	remove_call_out("main");
	if (!arg)
	{
		return help(me);
	}
	if(sscanf(arg,"-a %s",uid)==1) {
		super_debug=1;
		savedata(uid);
		//tell_object(os_user,
		tell_object(os_user,"处理"+uid+"完毕。\n");
		return 1;
	}
	sscanf(arg,"%s %d %d",arg,dix,j);
	dir = ID_PATH + "login/" + arg + "/";
	files = get_dir(dir);
	files = filter_array(files,(:iso:));	
	tell_object(os_user,dir + " 处理开始。\n");	
	if(!dix) dix = sizeof(files)-1;
	else dix += j-2;
	if(dix<0) return notify_fail("超上限了/\n");
	if(dix>sizeof(files)-1) dix= sizeof(files)-1;
	for (j-=1; j<=dix; j++)
	{
		if (sscanf(files[j]+"$", "%s.o$", uid)==1 )
		{
			savedata(uid);
			count++;
		}
	}
	if(dix==sizeof(files)-1) tell_object(os_user,dir + " 处理结束。\n");
	else{
		tell_object(os_user,sprintf("%s 处理尚未结束。\n从 %d 开始到最后还有 %d 记录。\n",dir,dix+2,sizeof(files)-dix-1));
		if(os_user->query("env/auto")==1){
			tell_object(os_user,"\n请稍候，响应处理中。。。。");
			remove_call_out("main");
			call_out("main",5,me,sprintf("%s %d %d",arg,250,dix+2) );
		}
	}
	tell_object(os_user,"\n共处理了 " + count + " 位用户。\n");
	return 1;
}

int help(object me)
{
	write(@HELP

指令格式：movedata2 <a|b|c|...> [<numa> <numb>]
	numa 表示一次处理多少数据,
	numb 表示从哪里开始处理。
	
	例如：movedata2 a 100 1
	说明：从第一个记录开始处理100个数据
		然后继续movedata2 a 100 101....
		
	movedata2 -a [id]
	单独处理某一ID。	
	set auto 1 将会自动处理请求。
HELP
	);
	return 1;
}