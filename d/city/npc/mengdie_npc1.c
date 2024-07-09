//add by tangfeng
//2004
inherit FIGHTER;
inherit F_UNIQUE;
#include <ansi.h>
#include <skills_pfm.h>
void do_copy(object ob);
#include "/d/city/diemenglou_party.h";

//2005.3 added by tangfeng
int yun();
int pfm();

string *valid_types =
({
	"claw",
	"cuff",
	"finger",
	"hand",
	"leg",
	"strike",
});

void create()
{
	mapping name;
	int sex = random(3);
	name = RNAME_D->get_random_name(random(3));
	set_name(name["name"], name["id"]);
	set_name(query("name"), ({ query("id"),"mengdie npc"}));
	set("gender", (sex?"男性":"女性"));
	set("long", "神秘人，你尽力地去去看清，却还是一片模糊。\n");
	set("age", 25+random(100));
	set("attitude", "peaceful");
	set("rank_info/rank","神秘人");
	set("rank_info/self_rude","神秘人");
	set("rank_info/self","小生");
	set("rank_info/rude","神秘人");
	set("rank_info/respect","神秘人");
	set("rank_info/color","HIW");

	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("per", 30);
	set("max_qi", 500);
	set("max_jing", 500);
	set("eff_jingli",400);
	set("neili", 100);
	set("max_neili", 100);
	set("jiali", 20);
	set("combat_exp", 8000+random(500));
//一些武功绝技被设定因为quest取消，特意增加该变量
//目前设定不允许使用的是tanzhi-shentong的pfm qiankun
    set("quest_cancel",1);
	set_skill("force", 70);
	set_skill("dodge", 80);
	set_skill("unarmed", 70);
	set_skill("parry", 80);
	set_skill("sword", 80);
	set_skill("cuff", 80);
	set("inquiry", ([
		"name" : random(2)?"什么叫名字？":"呵呵，人生如梦，名字早就忘记了。",
		"名字" : random(2)?"什么叫名字？":"呵呵，人生如梦，名字早就忘记了。",
		"拜师" : "嗯....，小生注定一声漂泊，还是算了吧。",
    ]));

	setup();
	carry_object(ARMOR_D("shoes"))->wear();
	carry_object(ARMOR_D("changshan"))->wear();
}
init()
{
	int i,p,k;
    mapping pmap,my_party;
	object ob,me;
	::init();
	ob=this_object();
	if(!ob->query("kill_object")) return;
    me =ob->query("kill_object");
	if(!me) return;
	if(query("set_ok")) return;
	set("set_ok",1);
	pmap = me->query_skill_prepare();

	if (!pmap)
	{
		i=sizeof(valid_types);
		p=0;
		while (i--)
		{
			if (!me->query_skill(valid_types[i], 1)) continue;
			if (me->query_skill_mapped(valid_types[i]))continue;
			if(p>=2) continue;
			p=p+1;
			me->prepare_skill(valid_types[i], me->query_skill_mapped(valid_types[i]));
		}
	}
	if(ob->query("party_id"))
	{
		k=ob->query("party_id");
		my_party = get_party_int(k);
		if(my_party["shen"]=="邪")
		{
			if(random(2)) command("nomatch "+me->query("id"));
			else command("u&me");
			message_vision(HIC"$N对$n哼了一声道：老子正是"+query("title")+query("name")+HIC"，特来会会。\n" NOR, ob,me);
		}
		else if(my_party["shen"]=="正")
		{
			if(random(2)) command("sorry2");
			else command("welcome "+ob->query("id"));
			message_vision(HIC"$N对$n点了点头道：在下正是"+query("title")+query("name")+HIC"，还请阁下手下留情。\n" NOR, ob,me);
		}
		else
		{
			if(random(2)) command("sneer "+me->query("id"));
			else command("grpfight "+ob->query("id"));
			message_vision(HIC"$N瞥了$n一眼道：在下正是"+query("title")+query("name")+HIC"，还望多多包涵。\n" NOR, ob,me);
		}
		if(my_party["party"]=="pxj" && objectp(ob->query_temp("weapon"))) 
		{
			ob->set_temp("double_attack",1);//pxj 兵器hubo
			ob->add_temp("apply/attack", ob->query_skill("force")/4);
			ob->add_temp("apply/damage", ob->query_skill("force")/4);	
		}
		if(my_party["party"]=="jsj") 
		{
			ob->set("double_attack",1);//jsj 空手hubo
			ob->add_temp("apply/attack", ob->query_skill("force")/5);
			ob->add_temp("apply/damage", ob->query_skill("force")/5);	
			ob->add_temp("apply/parry", ob->query_skill("force")/4);
		}
		if(!objectp(ob->query_temp("weapon")) && random(3))//空手增加伤害和防御
		{
			ob->add_temp("apply/damage", ob->query_skill("force")/6);
			ob->add_temp("apply/parry", ob->query_skill("force")/6);
		}
		if(!random(5)) //随机增加攻击
		{
			if(my_party["shen"]=="邪")
				command("killair "+me->query("id"));
			else if(my_party["shen"]=="正")
				command("buddha");
			else
				command("piggy");
			ob->add_temp("apply/attack", random(ob->query_skill("force")/5));
			ob->add_temp("apply/damage", random(ob->query_skill("force")/5));
		}
	}
    else
	{
		if(random(2)) command("noshame "+me->query("id"));
		else command("grpfight "+ob->query("id"));
		message_vision(HIC"$N瞥了$n一眼道：在下正是"+query("title")+query("name")+HIC"，正好我们较量较量。\n" NOR, ob,me);
	}
	remove_call_out("dest");
	call_out("dest",360);
}
void die()
{
	object ob,me;
	object *obs;
	int i;
	ob=this_object();

	if(ob->query("kill_object") && ob->query("quest/level"))
	{
        me=ob->query("kill_object");
	}
	if(me && present(me,environment(ob)))
	{
		i=ob->query("quest/level");
		i=i*i;
		me->add_temp("quest/special/score",i);
		me->add("quest/special/score",i);
		tell_object(me,HIR"\n\n你成功的击退"+ob->short(1)+HIR"，获得奖励分数"+i+"。*_* \n"NOR);
		log_file("quest/SPECIAL", sprintf("%s(%s)一楼比武得分：%d。经验：%d。\n", me->name(1),me->query("id"),i,me->query("combat_exp")) );
	}
	obs = deep_inventory(ob);       
	obs = filter_array(obs,(:$1->query("imbued"):));
	i = sizeof(obs);
	while(i--)
	{
		obs[i]->move(environment(ob));
		message_vision(YEL"$N丢下一"+obs[i]->query("unit")+obs[i]->name()+NOR+YEL"。\n"NOR,ob);
	}
	command("why");
	if(!random(3))	command("sue");
	else if(!random(3)) command("kill1");
	else if(!random(3)) command("nomatch");
	else if(random(2)) command("poem");
	else command("killair");
	if(random(2)) command("wait "+me->query("id"));
	message_vision(HIY"\n突然一阵风散，$N原来站着的地方竟然一点痕迹也没有留下。\n"HIM"刚才的一切如梦幻般，全然消逝在风中。\n"NOR,ob);
	destruct(this_object());
}
void unconcious()
{
  die();
}
void dest()
{
	object ob,me;
	object *obs;
	int i;
	ob=this_object();
	obs = deep_inventory(ob);       
	obs = filter_array(obs,(:$1->query("imbued"):));
	i = sizeof(obs);
	if(ob->query("kill_object") && ob->query("quest/level"))
	{
		me=ob->query("kill_object");
	}
	while(i--)
	{
		if(me) 
		{
			tell_object(me,HIY"突然你觉得行囊里似乎多了点什么，低头一看竟然是刚才失去的那一"+obs[i]->query("unit")+obs[i]->name()+HIY"。\n"NOR);
			obs[i]->move(me);
		}
		else 
		{
			message_vision(YEL"$N丢下一"+obs[i]->query("unit")+obs[i]->name()+NOR+YEL"。\n"NOR,ob);
			obs[i]->move(environment(ob));
		}
	}
	message_vision(HIR"\n突然一阵风散，$N原来站着的地方竟然一点痕迹也没有留下。"HIM"刚才的一切如梦幻般，全然消逝在风中。\n"NOR,this_object());
	destruct(this_object());
}
void do_copy(object ob)
{
        object *inv;
        mapping hp_status, skill_status, map_status, prepare_status;
        string *sname, *mname, *pname;
        int i;
		//string* u_pfm=({});		
        set_name(ob->name(), ({ ob->query("id")}) );
        set("title", ob->query("title") );
        set("long", ob->query("long") );
        set("gender", ob->query("gender") );
        set("per", ob->query("per") );
        set("age", ob->query("age") );
		set("family/family_name",ob->query("family/family_name"));
		set("family/master",ob->query("family/master"));
        set_temp("apply/damage",  30 + random(100) );
        set_temp("apply/attack",  30 + random(100) );
        set_temp("apply/armor",   30 + random(100) );

        if( mapp(skill_status = ob->query_skills()) ) {
                skill_status = ob->query_skills();
                sname = keys(skill_status);

                for(i=0; i<sizeof(skill_status); i++) {
                        set_skill(sname[i], skill_status[sname[i]]);
                }
        }

        if( mapp(map_status = ob->query_skill_map()) ) {
                mname  = keys(map_status);

                for(i=0; i<sizeof(map_status); i++) {
                        map_skill(mname[i], map_status[mname[i]]);
                }
        }

        if( mapp(prepare_status = ob->query_skill_prepare()) ) {
                pname = keys(prepare_status);

                for(i=0; i<sizeof(prepare_status); i++) {
                        prepare_skill(pname[i], prepare_status[pname[i]]);
                }
        }

        inv = all_inventory(ob);
        i = sizeof(inv);
        while (i--) {
                if( inv[i]->query("weapon_prop/damage") > 100
                || inv[i]->query("unique")
                || inv[i]->query("imbued")
                || inv[i]->query("treasure")
                || inv[i]->query("armor_prop/armor") > 100 ) continue;
				inv[i]->set("copy_object",1);
                if( inv[i]->query("weapon_prop") && inv[i]->query("equipped") ) {
                        carry_object(base_name(inv[i]))->wield();
                        set("weapon", base_name(inv[i]));
                }
                if( inv[i]->query("armor_prop") && inv[i]->query("equipped") ) {
                        carry_object(base_name(inv[i]))->wear();
                        set("armor", base_name(inv[i]));
                }
        }

        hp_status = ob->query_entire_dbase();

        set("str", hp_status["str"]);
        set("int", hp_status["int"]);
        set("con", hp_status["con"]);
        set("dex", hp_status["dex"]);

        set("max_qi",    hp_status["max_qi"]);
        set("eff_qi",    hp_status["max_qi"]);
        set("qi",        hp_status["max_qi"]);
        set("max_jing",  hp_status["max_jing"]);
        set("eff_jing",  hp_status["max_jing"]);
        set("jing",      hp_status["max_jing"]);
        set("max_neili", hp_status["max_neili"]);
        set("eff_jingli", hp_status["max_jingli"]);
        set("max_jingli", hp_status["max_jingli"]);
        set("jingli",     hp_status["max_jingli"]*2);
        set("neili",     hp_status["max_neili"]*2);
        set("jiali",     hp_status["jiali"]*2);
        if( query("jiali") > 200 ) set("jiali", 200 );
        set("combat_exp",hp_status["combat_exp"]);
		set("chat_chance_combat", 100);
		set("chat_msg_combat", ({ 
			(: yun :),
		    (: pfm :),
		}));   
}
int sort_skill(string file)
{
	int i;
	if ( file[<2..<1] != ".c" ) return 0;
	if ( !sscanf(file, "%*s.c") )  return 0;
	if ( sscanf(file, "%*s_noshow.c") )  return 0;
	i = strlen(file);
	while(i--){
		if( file[i] == '.' ) continue;
		if( file[i] < 'a' || file[i] > 'z' )
			return 0;
	}

	return 1;
}
int yun()
{
	string arg, force, exert, *file;
	object me;
	int j = 0;
	me = this_object();
	if( me->is_busy() ) return 0;
	if(me->is_exert()|| !(force = me->query_skill_mapped("force")) || file_size(SKILL_D(force)+".c") < 1 )
	{
	   pfm();
	   return 1;
	}
	if( stringp(exert = SKILL_D(force)->exert_function_file("")) )
	{
		file = get_dir(exert);
		if( sizeof(file) > 0 )
		{
			file = filter_array(file, (: sort_skill :) );
			j = sizeof(file);
		}
	}
	if( j && j > 0 ) {
		sscanf(file[random(j)], "%s.c", arg);
		if( arg == "roar" || arg == "play" || arg == "shougong" || arg == "duwu" || 
		    arg == "huagong" || arg == "huajing" || arg == "guiyuan" ) return 0;
		command("exert "+arg);
	}
	return 1;
}
int pfm()
{
	mapping pmap;
	string arg, skill, pfm, *file;
	object me, weapon;
	int j = 0;

	me = this_object();
	if( me->is_busy() ) return 0;
	if( me->is_perform() ) return 0;

	if( me->query_skill("douzhuan-xingyi", 1) >= 120 && random(2) ) {
		command("enable parry douzhuan-xingyi");
		if( random(2) ) command("perform parry.bishen");
		else command("perform parry.xingyi");
	} else if( me->query_skill("qiankun-danuoyi", 1) >= 210 && random(2) ) {
		command("enable parry qiankun-danuoyi");
		if( random(2) ) command("perform parry.yin");
		else if( random(2) ) command("perform parry.xu");
		else command("perform parry.hua");
	}

	if( (skill = me->query_skill_mapped("dodge")) && file_size(SKILL_D(skill)+".c") >=1) 
	{
		if( stringp(pfm = SKILL_D(skill)->perform_action_file("")) ) {
			file = get_dir(pfm);
			if( sizeof(file) > 0 ) {
				file = filter_array(file, (: sort_skill :) );
				j = sizeof(file);
			}
		}
		if( j && j > 0 ) {
			sscanf(file[random(j)], "%s.c", arg);
			if( arg != "baibian" ) command("perform dodge."+arg);
		}
	}

	weapon = me->query_temp("weapon");
	if( objectp(weapon) ) {
		if( !(skill = me->query_skill_mapped(weapon->query("skill_type"))) ) return 0;
		if( file_size(SKILL_D(skill)+".c") < 1 ) return 0;
		if( stringp(pfm = SKILL_D(skill)->perform_action_file("")) ) {
			file = get_dir(pfm);
			if( sizeof(file) > 0 ) {
				file = filter_array(file, (: sort_skill :) );
				j = sizeof(file);
			}
		}
		if( j && j > 0 ) {
			sscanf(file[random(j)], "%s.c", arg);
			if( arg == "tongshou" || arg == "fentian" || arg == "jue" ) return 0;
			command("enable parry "+skill);
			command("perform "+weapon->query("skill_type")+"."+arg);
		}
	}
	else {
		pmap = me->query_skill_prepare();
		if( !pmap ) pmap = ([]);
		if( !sizeof(pmap) ) {
			command("prepare strike");
			command("prepare cuff");
			command("prepare hand");
			command("prepare leg");
			command("prepare finger");
			command("prepare claw");
			return 0;
		}
		file = keys(pmap);
		foreach (string item in file) {
			if( !(skill = me->query_skill_mapped(item)) ) return 0;
			if( file_size(SKILL_D(skill)+".c") < 1 ) return 0;
			if( stringp(pfm = SKILL_D(skill)->perform_action_file("")) ) {
				file = get_dir(pfm);
				if( sizeof(file) > 0 ) {
					file = filter_array(file, (: sort_skill :) );
					j = sizeof(file);
				}
			}
			if( j && j > 0 ) {
				sscanf(file[random(j)], "%s.c", arg);
				if( arg == "juehu" ||  arg == "sanyin" || arg == "xuanming" || arg == "huagu" ) return 0;
				command("enable parry "+skill);
				command("perform "+item+"."+arg);
			}
		}
	}
	return 1;
}

