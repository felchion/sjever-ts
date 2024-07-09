// aliasd.c
string TimeDate(int t)
{
        string ts = ctime(t);
        string tm;
       // int tt;
        tm = ts[11..18];
        ts = ts[20..23]+"年"+ts[4..6]+"月"+ts[8..9]+"日";
        
        ts = replace_string(ts, " 1", "01");
        ts = replace_string(ts, " 2", "02");
        ts = replace_string(ts, " 3", "03");
        ts = replace_string(ts, " 4", "04");
        ts = replace_string(ts, " 5", "05");
        ts = replace_string(ts, " 6", "06");
        ts = replace_string(ts, " 7", "07");
        ts = replace_string(ts, " 8", "08");
        ts = replace_string(ts, " 9", "09");
        ts = replace_string(ts, "Jan", "01");
        ts = replace_string(ts, "Feb", "02");
        ts = replace_string(ts, "Mar", "03");
        ts = replace_string(ts, "Apr", "04");
        ts = replace_string(ts, "May", "05");
        ts = replace_string(ts, "Jun", "06");
        ts = replace_string(ts, "Jul", "07");
        ts = replace_string(ts, "Aug", "08");
        ts = replace_string(ts, "Sep", "09");
        ts = replace_string(ts, "Oct", "10");
        ts = replace_string(ts, "Nov", "11");
        ts = replace_string(ts, "Dec", "12");
        
        sscanf(tm,"%d:%*d:%*d",t);
        if(t<12) ts +="正午前";
        if(t>=12) ts +="正午后";
                        
        return ts;
}

mapping global_alias = ([
	"bai":		"apprentice",
	"bei":		"prepare",
	"dating":	"check",
	"dazuo":	"exercise",
	"du":		"study",
	"fangqi":	"abandon",
	"i":		"inventory",
	"jia":		"marry",
	"jiali":	"enforce",
	"jifa":		"enable",
	"kaichu":	"expell",
	"l":		"look",
	"lian":		"practice",
	"qiu":		"propose",
	"quan":		"persuade",
	"sha":		"kill",
	"shou":		"recruit",
	"tk":		"teamkill",
	"tt":		"team talk",
	"tt*":		"team talk*",
	"tuna":		"respirate",
	"xue":		"learn",
	"yun":		"exert",
	"zisha":	"suicide",
	"chanrang":	"abdicate",
	"tanhe":	"delate",
	"fenshua":	"stucco",
	"n":		"go north",
	"e":		"go east",
	"w":		"go west",
	"s":		"go south",
	"nu":		"go northup",
	"eu":		"go eastup",
	"wu":		"go westup",
	"su":		"go southup",
	"nd":		"go northdown",
	"ed":		"go eastdown",
	"wd":		"go westdown",
	"sd":		"go southdown",
	"ne":		"go northeast",
	"se":		"go southeast",
	"nw":		"go northwest",
	"sw":		"go southwest",
	"u":		"go up",
	"d":		"go down",
	"r":		"go enter",
	"run":	"go away"
]);

string *trust_obj_list = ({
"/d/xiangyang/hanshui1",
"/d/xiangyang/hanshui2",
"/d/wudang/chating",
"/d/xiangyang/npc/banker",
"/d/changle/obj/buliao",
"/d/xiangyang/zahuopu",
"/d/changle/obj/buliao",
"/d/huashan/jitan",
"/d/zhiye/obj/yao",
"/d/zhiye/npc/yaodian-zhanggui",
"/d/dali/npc/qian",
"/d/dali/npc/shen",
"/d/city/npc/huang",
"/d/xiangyang/lzz/npc/xue",
"/d/chengdu/npc/banker",
"/d/hz/npc/dplaoban",
"/d/hz/npc/qzlaoban",
"/clone/medicine/renshen-guo",
"/clone/medicine/neili/puti-zi",
"/d/hmy/ryping",
"/d/hmy/yading",
"/d/fuzhou/npc/zhang",
"/d/dali/job/dali_letter",
"/d/dali/zahuopu",
"/d/lanzhou/road2",
"/d/lanzhou/road3",
"/d/huanghe/road2",
"/d/huanghe/road3",
"/d/city/jiangnan",
"/d/city/jiangbei",
"/kungfu/class/wudang/robber",
"/d/xiangyang/npc/cuicui",
"/d/suzhou/npc/laoban2",
"/d/shaolin/dmyuan",
"/d/shaolin/dmyuan2",
"/d/shaolin/guangchang",
"/d/shaolin/smdian",
"/d/city/npc/qian",
"/d/changan/npc/qianyankai",
"/d/fuzhou/npc/banker",
"/d/mr/yanziwu/shuixie",
"/d/mr/shiqiao",
"/kungfu/class/gaibang/qigai",
"/kungfu/class/gaibang/qnqigai",
"/d/dali/dalisouth/jiangnan",
"/d/dali/dalisouth/jiangbei",
"/d/changan/npc/huang",
"/d/changan/npc/qianyankai"
});


static mapping no_send_list = (["who":1,"mudlist":1,"chat":1,"rumor":1,"party":1,"sj":1,"finger":1,"tell":1,"mm":1,"reply":1,]); 
static mapping no_lower_list = (["checkall":1,"sqlfmt":1,"purge":1,"cvsfmt":1,"hj":1,"gift":1,"mm":1,"cchat":1,"chat":1,"rumor":1,"party":1,"sj":1,"wiz":1,"cc":1,"alias":1,"set":1,"unset":1,"nick":1,"describe":1,"color":1,"tt":1,"call":1]);
static mapping msg_list = (["mm":1,"reply":1,"tell":1]);

string process_global_alias(string arg, object ob)
{
	string *word;
	mixed *cmds;
	int i;
	object tob;

	if (!ob)
		ob = this_player();
	if (ob && userp(ob) && !wizardp(ob) && ob->is_ghost())
		return "";

	for (i=0; i<strlen(arg); i++)
		if (arg[i] != ' ') {
			arg = arg[i..<1];
			break;
		}
	if (arg[0]=='\'')
		arg = "say " + arg[1..<1];

	word = explode(lower_case(arg), " ");	
	if (sizeof(word)==0) return "";
	

	
	if(objectp(tob=ob->query_temp("telnet_ob"))) {
		if(!no_send_list[word[0]]){
			tob->send_to_remote(arg);
	 		return "";
		}
  }
  
	if(!IN_MAIN && member_array(word[0],({"market","quit","transfer"}))!=-1 //&& !wizardp(ob)
	) {
		tell_object(ob,"副本进行中，无法执行此操作。\n");
		return "";
	}
	
	if (ob->query_temp("last_msg")!=arg)
		if(msg_list[word[0]]){
	 		log_file("player/private",sprintf("%-8s:%s",ob->query("id"),arg));	  
			ob->set_temp("last_msg",arg);
			ob->delete("last_msg",arg);
	}
	if (no_lower_list[word[0]]
	|| (sizeof(word)>2 && word[0]=="team" && word[1]=="talk")) {
		word = explode(arg, " ");
		word[0] = explode(lower_case(arg), " ")[0];
		if (word[0] == "cc" && sizeof(word)>2)
			word[1]=lower_case(word[1]);
		arg = implode(word," ");
	}	
	else 
		arg = lower_case(arg);
		
	if( sizeof(word) && !undefinedp(global_alias[word[0]]) ) {
		word[0] = global_alias[word[0]];
		arg = implode(word, " ");
	}			

	ob->set_temp("last_cmds",arg);
	
	i = strlen(arg);
	if (ob && userp(ob) && !wizardp(ob))
		while (i--)
			if (arg[i] < ' ' && arg[i] != 27 && arg[i] != 10) arg[i] = ' ';

	return arg; 
}
