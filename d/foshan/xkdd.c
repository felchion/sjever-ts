
#define JOB_DIR "/cmds/leitai/dzd/"

#define SRM JOB_DIR"boat.c"
#define DEATHS_ROOMO JOB_DIR"matou.c"
/*船只文件*/
#define GAME_TIME 1200
//600
/*游戏时间*/
#define NEXT_TIME 1200

/*回合时间*/
#define IN_TIME 420
//#define IN_TIME 120
//420
//300 7*60 420
/*从第一个人开始ask到开始的时间*/
#define EXP_LIMIT 3500
/*最高可以获得的EXP*/
#define FORBID_ML 2
/*是否允许相同IP参与Job
 *此处修订为允许同IP参加
 *的玩家数量 0 表示禁止
 */

//为了中秋节 
#define BOOK    "/clone/gift/book"      //书
#define MIC     "/clone/gift/danwan"    //药
#define MON     "/clone/gift/money"     //钱
#define JADE    "/d/zhiye/obj/othermaterial" //玉
#define DAZAO   "/clone/gift/imbue"     //打造机会

inherit F_DBASE;

#include <ansi.h>
#include <mudlib.h>
//#include <job_mul.h>


static int debug = 0;   //测试状态
static int max_users = 40;//最多玩家数量
static int min_users = 7;
//static int min_users = 2;
//7
static int closed = 0;//关闭状态
static object author;//
static string* deads = ({"冰蚕毒发作","七心海棠毒发作","醉仙灵芙毒发","蜂毒发作",});
   ///add condition dead
static string game_name = "大智岛";
//int is_ghost() { return ghost; }

int one_live;
string stating;
mapping* user_list;
int start_time;
int over_time;
int total_exp;

int get_num(mixed me);
void demon_tell(int i,object me,string arg);
void job_over();

void create()
{
	seteuid(ROOT_UID);
	set("no_clean_up",1);
	set("channel_id",game_name+"控制精灵");
	if(!user_list) user_list = ({});
}

string query_boat()
{
	return SRM;
}

int change_debug()
{
	if(debug==1){
		debug = 0;
	}
	else debug = 1;
	return debug;	
}

int query_exp()
{
	if(!sizeof(user_list)) return 0;
	return total_exp/sizeof(user_list);
}

int query_mode()
{
	return debug;
}

string query_name()
{
	return game_name;
}

void init_start()
{
	stating = "ready";
	//user_list = allocate_mapping(max_users);;	
	user_list = ({});
	start_time = time()+IN_TIME;
	total_exp = 0;
	one_live = 0;
	author = find_player("ciwei");
	remove_call_out("check");
	call_out("check",2);
}

void debug_msg(string arg)
{
	arg += "\n"NOR;
	if(debug && author) tell_object(author,arg);
}

string query_stat()
{
	if(undefinedp(stating) || stating=="waitby"  ) init_start();
	return stating;
}

int add_player(object me,int flag)
{
	int i;
	mapping db;
	
	if(sizeof(user_list)){
		int same_ip = 0;
		for(i=0;i<sizeof(user_list);i++){
			if(getuid(me)==user_list[i]["id"]) break;
			if(query_ip_number(me)==user_list[i]["ip"]) same_ip++;
			if(same_ip>FORBID_ML) break;
		}
		if(i!=sizeof(user_list)) return -1;
	}
	if(query_stat()=="ingame" || query_stat()=="overbusy") return -3;
	if(time() > start_time-5) return -3;//提前不让ask 因为ask也来不及了
	if(sizeof(user_list)>=max_users) return -2;
	if(closed) return 0;
	if(flag){
		db = (["id":           getuid(me),
			"object":      me,
			"asktime":     time(),
			"start_time":  start_time,
			"score":       0,
			"kar_bonus":   2*MAX(MAX(me->query_skill("zhizao",1),me->query_skill("duanzao",1)),me->query_kar()*10),
			"exp":         me->query("combat_exp"),
			"ready":       0,
			"over":        0,
			"alive":       1,
			"ip":          query_ip_number(me),
		]);///etc......
		debug_msg(sprintf("%O",db));
		if (!sizeof(user_list)){
                		user_list = ({ db });
                		CHANNEL_D->do_channel(this_object(),"rumor",HIY+me->name()+"好象想去"+game_name+"探险。");
                	}
        	else
                	user_list += ({ db });
                total_exp += db["exp"];
                me->delete_temp("job_xkd");
		me->set_temp("job_xkd/asked",1);
		me->apply_condition("job_busy",20);//防止去ask别的job grin
	}
	return start_time;
}

int is_player(object me)
{
	if(add_player(me,0)==-1) return 1;
	return 0;
}

void get_item(object me)
{
	object* items = deep_inventory(me);
	if(!sizeof(items)) return;
        for(int i=0;i<sizeof(items);i++)
        	if( items[i]->query("dzd_obj") ) destruct(items[i]);
}

int clear_data(mapping db)
{
	if(!objectp(db["object"])) return 0;
	if(!db["ready"]){
		object ob;
		if(objectp(ob = find_player(db["id"]))){
			ob->delete_temp("job_xkd");
			get_item(ob);
			tell_object(ob,"你错过了去"+game_name+"的时间！\n");
			ob->apply_condition("job_busy",1);//防止去ask别的job grin
		}
		return 0;
	}
	return 1;
}

void player_ready(object me)
{
	int i;	
	i = get_num(me);
	if(undefinedp(user_list[i])) return;
	user_list[i]["ready"] = 1;
	//some msg
}

void give_item(object me)
{
	object item;
	string* items = ({
		JOB_DIR"npc/obj/baoyi",
		JOB_DIR"npc/obj/bishou",
		JOB_DIR"npc/obj/hssy",
		JOB_DIR"npc/obj/hulu",
		JOB_DIR"npc/obj/huqin",
		JOB_DIR"npc/obj/pimao",
		JOB_DIR"npc/obj/qxht",
		JOB_DIR"npc/obj/xsycw",
		JOB_DIR"npc/obj/yufeng",
		JOB_DIR"npc/obj/yuwang",
		JOB_DIR"npc/obj/ziweijian",
		JOB_DIR"npc/obj/qimen",
		JOB_DIR"npc/obj/zhujingbch",
		JOB_DIR"npc/obj/mujian",
		JOB_DIR"npc/obj/mianmo",
		JOB_DIR"npc/obj/dujing",
	});
	item = new(items[random(sizeof(items))]);
	
	if(clonep(item)) {
		item->set("dzd_obj",1);
		item->move(me);
		tell_object(me,HIY"艄公在你身旁悄悄的走过，你觉得行囊里多了什么东西。\n"NOR);
	}
}

void reward()
{
	int exp,pot,shen,i;
	string str;
	string live_name = "ereesdfsdfd";
	//object gift;	

	for(i =0;i<sizeof(user_list);i++){
		//int sss;
		int t;
		if(!user_list[i]["object"]) continue;
		str = HIW"你此番经此大难，";
		exp = 300+user_list[i]["score"]+random(user_list[i]["score"]/3);
		///至少等待5分钟 所以最多加500 exp
		//10m 即有500 exp
		//	
		
											
		if(user_list[i]["score"]==0) exp += (  user_list[i]["over_time"]  -start_time)/6;//没杀过任何人 给时间奖励
		if(user_list[i]["dead"]) {exp = exp *2 ; exp = exp/3;str+="虽然不能全身而退，";}
		if(!one_live) {exp = exp *2 ; exp = exp/3;str += "未能探得"+CHINESE_MUD_NAME+game_name+"秘密";}
		
		t = (user_list[i]["object"]->query("combat_exp") * 100 )/2000000;
		//t = t*100;
		if(t<=100) t = 100;
		if(t>500) t = 500;
		exp += t;
		if(one_live && user_list[i]["alive"] ) {exp = exp * 2;str+="得胜而归，";live_name = user_list[i]["object"]->query("name"); }//独胜不受limit控制
				
		if(exp>EXP_LIMIT) exp = EXP_LIMIT-random(30);
		pot = exp /3;
		//奖励控制
		shen = exp;
		if(user_list[i]["object"]->query("family") &&  user_list[i]["object"]->query("family/family_name")=="桃花岛") shen = 0;
		log_file("job/dazhidao",
		sprintf("%-8s%-8s 完成大智岛任务 获得奖励：exp:%-4d pot:%-4d 当前exp:%d",user_list[i]["object"]->query("name"),user_list[i]["object"]->query("id"),exp,pot,user_list[i]["object"]->query("combat_exp") ),user_list[i]["object"]  );
		user_list[i]["object"]->add("combat_exp",exp);
		user_list[i]["object"]->add("potential",pot);
		user_list[i]["object"]->add("shen",shen);		
		if(user_list[i]["object"]->query("potential")> user_list[i]["object"]->query("max_pot")) user_list[i]["object"]->set("potential",user_list[i]["object"]->query("max_pot"));
		str +="发现自己不但丝毫无损，而且武功大进。\n"NOR; 
		str += sprintf(HIC"这次%s你获得%s点经验，%s点潜能",game_name,chinese_number(exp), chinese_number(pot) );
		if(shen) str += sprintf("，%s点正神的奖励。\n"NOR,chinese_number(shen));
		else str += "的奖励。\n"NOR;

		/*
		大智岛上增加随机奖励。
        
		每个带月饼的玩家都可能额外获得，不论战绩，完全随机的，没有月饼的玩家，没有额外奖励。
		每次JOB的1/2玩家随机获得如下奖励中的一项：
		1、200 GOLD存款。
		2、500 EXP。	
		3、一块玉（1-4级随机随机）。
		4、一本技能残篇。
		5、韦兰铁锤。
		
        
		另外：活动期间，每次获得大智岛最后生存者头衔的ID，可以获得一块高级4级玉（密玉/香凝玉）(1/50)。
		但是 4 6 奖励最多获得三次
		*/
		/*if(user_list[i]["object"]->query_temp("quest/gift/mid-autumn/yuebing")
			&& random(2))//只有一半人有奖励
		{
			//如果胜利者有机会获得一次高级宝玉
			if(one_live && user_list[i]["alive"] && !user_list[i]["object"]->query("quest/gift/mid-autumn/winner") 
				&&!random(150))//这个概率下才能获得
			{
				level=3;
				if(!random(2)) level+=1;//4级的可能性很大
				if(!random(20)) level+=1;//玩家前面已经一个random(50)了，这里还行的话，真的可以中彩了
				gift = new(JADE);
				gift->set_level(level);
				user_list[i]["object"]->add("quest/gift/mid-autumn/winner",1);
				if(user_list[i]["object"]->query("registered")<3)
					log_file("quest/yuebing", sprintf("非VIP %s(%s)获胜因第%d个月饼获得%d级宝玉%s。胜利者终极宝物。\n", 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 level,
												 gift->query("name")) );
				else
					log_file("quest/yuebing", sprintf("VIP %s(%s)获胜因第%d个月饼获得%d级宝玉%s。胜利者终极宝物。\n", 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 level,
												 gift->query("name")) );
			}
			//大多数的可能获得这三种奖励，另外特殊礼物必须小于两次
			else if(user_list[i]["object"]->query("quest/gift/mid-autumn/special")>=2|| random(150))
			{
				sss=random(3);
				if(random(60))
				{
					switch(sss)
					{
					case 0: 
						gift = new (BOOK);
						break;
					case 1: 
						gift = new (MIC);
						if(!random(8)) gift->set("secret_obj",1);
						gift->set("bonus","combat_exp");
						break;
					case 2:         
						gift = new (MON);
						break;
					default:return;
					}
				}
				else
				{
					gift = new(JADE);
					gift->set_level(1+random(2));
				}
				gift->set("gift_target",user_list[i]["object"]->query("id"));
				gift->set("owner",user_list[i]["object"]->query("id"));
				if(user_list[i]["object"]->query("registered")<3)
					log_file("quest/yuebing", sprintf("非VIP %s(%s)因第%d个月饼获得%s。其中宝物获得%d次。\n", 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 gift->query("name"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/special")) );
				else
					log_file("quest/yuebing", sprintf("VIP %s(%s)因第%d个月饼获得%s。其中宝物获得%d次。\n", 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 gift->query("name"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/special")) );
			}
			else
			{
				level=1;
				sss = random(2);//最后的倚天屠龙技能残篇没有加入
				if(!random(5)) level+=1;
				if(!random(7)) level+=1;
				if(!random(9)) level+=1;//玉的级别1-4
				switch(sss)
				{
			    case 0:
					gift = new(DAZAO);
					break;
				case 1:
					gift = new(JADE);
					gift->set_level(level);
					break;
				default:return;              			
				}
				user_list[i]["object"]->add("quest/gift/mid-autumn/special",1);
				if(user_list[i]["object"]->query("registered")<3)
					log_file("quest/yuebing", sprintf(HIR"非VIP %s(%s)因第%d个月饼第%d次获得宝物%s。\n"NOR, 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/special"),
												 gift->query("name")) );
				else
					log_file("quest/yuebing", sprintf(HIR"VIP %s(%s)因第%d个月饼第%d次获得宝物%s。\n"NOR, 
				                                 user_list[i]["object"]->name(1),
												 user_list[i]["object"]->query("id"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/yuebing"),
												 user_list[i]["object"]->query("quest/gift/mid-autumn/special"),
												 gift->query("name")) );
			}
			gift->set("gift_target",user_list[i]["object"]->query("id"));
			gift->set("owner",user_list[i]["object"]->query("id"));
			gift->move(user_list[i]["object"]);
			str += "你还意外地获得"+gift->query("name")+"的奖励。\n"NOR;
		}

		user_list[i]["object"]->delete_temp("quest/gift/mid-autumn/yuebing");//全部去掉标志
*/
		demon_tell(2+random(2),user_list[i]["object"],str );
	}
	str = "大智岛冒险结束了";
	if(live_name != "ereesdfsdfd" && one_live) str += "，最后生存者："+live_name+"。";
	else str += "。";
	CHANNEL_D->do_channel(this_object(),"rumor",HIY+str);
}

void quit_player(object me,int flag)
{
	int i;
			
	i = get_num(me);
	if(undefinedp(user_list[i])) return;
	if(user_list[i]["over"] == 1) return;
	if(user_list[i]["gift"] == 1){
		me->add_temp("apply/attack",-user_list[i]["kar_bonus"]);
		me->add_temp("apply/armor",-user_list[i]["kar_bonus"]);
	}	
	if(user_list[i]["gift"] == -1){
		me->add_temp("apply/dodge",user_list[i]["kar_bonus"]);
		me->add_temp("apply/parry",user_list[i]["kar_bonus"]);
	}	
	me->clear_conditions_by_type("poison");//clear poison
	me->clear_conditions_by_type("hurt");//clear poison
	me->clear_condition("killer");//clear killer
	me->remove_relay_listener("die");//注销die监听器
	me->remove_all_killer();
	me->delete_temp("other_kill");
	me->delete_temp("kill_other");//delete flag sign
	me->delete_temp("dzd_quest");
	///dzd_quest some temp about obj room
	//me->apply_condition("job_busy",5+random(5));
	me->clear_condition("job_busy");
	me->set("job_name",game_name);
	if(flag) me->apply_condition("job_busy",10+random(10));	
	if(flag){
		object* obs;
		user_list[i]["dead"] = 1;
		user_list[i]["alive"] = 0;		
		obs = filter_array(all_inventory(me),(:$1->query("dzd_obj"):));
		if(sizeof(obs)){
			for(int j=0;j<sizeof(obs);j++)
				obs[j]->move( environment(me) );
			tell_room(environment(me),HIG"你似乎看到从"+me->name()+"身上失落了什么东西。\n"NOR,({me}));
		}
		if (environment(me)) environment(me)->die(me);
		me->move(DEATHS_ROOMO);
		tell_room(environment(me),me->name()+"被人从岛里扔了出来。\n"NOR,({me}));
		me->set_temp("job_xkd/dead",1);
		tell_object(me,"你被击败了。\n");
	}
	//me->reincarnate();
        me->set("eff_jing", me->query("max_jing") + me->query_temp("apply/jing"));
        me->set("eff_qi", me->query("max_qi") + me->query_temp("apply/qi"));
        me->set("jingli", me->query("eff_jingli") + me->query_temp("apply/jingli"));
        me->set("jing", me->query("eff_jing"));
        me->set("qi", me->query("eff_qi"));
        
	get_item(me);//防止遗祸江湖
	if(!flag) me->add("job_time/"+game_name,1);
	//and gift 
	//this is a big quest -> about shenzhao-jing need close the wg shenzhao-jing quest
	user_list[i]["over"] = 1;
	user_list[i]["over_time"] = time();
	//if(flag) demon_tell(2+random(2),me,"你虽然满腹委屈，却也只得知趣得离了"+game_name+"。\n" );
	//else demon_tell(2+random(2),me,"哼!"+game_name+"又如何我还不是照样来去自如。\n" );
	
	//me->delete_temp("job_xkd"); delete this var in room
	//give msg
}
//HIM"突然一阵烟雾出现，把你的身体淹没，等你能看清四周情况时，你发现已经到达了一个陌生的码头。\n"NOR
void gogogo()
{
	int texp;
	//这里给exp偏低的player 加上kar_bonus 嘿嘿 够淫荡吧？
	stating = "ingame";
	user_list = filter_array(user_list, (:clear_data:),);
	if(!sizeof(user_list)){
		job_over();
		return;
	}
	texp = total_exp/sizeof(user_list);
	for(int i=0;i<sizeof(user_list);i++){
		int rd;
		if(file_name(environment(user_list[i]["object"]))+".c"!=SRM){
			//这里把没有来参加的给..
			continue;
		}
		user_list[i]["object"]->register_relay_listener("die");//注册die监听器 这里防止玩家损失 另外再Log
		user_list[i]["object"]->set_temp("dzd_quest",([]) );
		debug_msg(sprintf("%O",user_list[i]["object"]->query_extra_listener() ) );
		rd = user_list[i]["exp"]>texp?10:90;
		if(random(100)<=rd){
			user_list[i]["gift"] = 1;
			user_list[i]["object"]->add_temp("apply/attack",user_list[i]["kar_bonus"]);
			user_list[i]["object"]->add_temp("apply/armor",user_list[i]["kar_bonus"]);
			debug_msg( sprintf("%-10s增加attack armor 各 %d",user_list[i]["object"]->name(),user_list[i]["kar_bonus"] ));
			demon_tell(2+random(3),user_list[i]["object"],HIW"你发现天气变幻玄机莫测，觉得自己的武功大有长进，不禁暗暗惊奇！\n"NOR);
			//你发现大智岛玄机莫测，觉得自己的武功顿受压抑，不禁暗暗提高警惕！
		}
		else if(random(100)>rd)
		{
			user_list[i]["gift"] = -1;
			user_list[i]["object"]->add_temp("apply/dodge",-user_list[i]["kar_bonus"]);
			user_list[i]["object"]->add_temp("apply/parry",-user_list[i]["kar_bonus"]);
			debug_msg( sprintf("%-10s减少dodge parry 各 %d",user_list[i]["object"]->name(),user_list[i]["kar_bonus"] ));
			demon_tell(2+random(3),user_list[i]["object"],HIR"你发现天气变幻玄机莫测，觉得自己的武功顿受压抑，不禁暗暗提高警惕！\n"NOR);
		}
				
		//提高加的幅度
		
		//about no_pk
		if(user_list[i]["object"]->query("no_pk")){
			user_list[i]["object"]->set("dzd_no_pk",user_list[i]["object"]->query("no_pk"));
			user_list[i]["object"]->delete("no_pk");
		}
				
		give_item(user_list[i]["object"]);
		user_list[i]["object"]->set("job_name",game_name);
		tell_object(user_list[i]["object"],CYN"远处传来一阵巨响，你感觉到整艘船剧烈的震动，急忙运功跟响声抵抗。\n"NOR);		
		user_list[i]["object"]->set_temp("last_damage_from","被巨响震伤心肺");
		user_list[i]["object"]->receive_damage("qi",random(300));
		user_list[i]["object"]->receive_wound("qi",random(200));
		user_list[i]["object"]->receive_damage("neili",random(1000));								
	}
	//give msg
}

void start_war()
{
	stating = "ingame";
	user_list = filter_array(user_list, (:clear_data:),);
	tell_room(SRM,"艄公站在众人面前摘下帽子，竟就是那个中年男子，原来他也上了船。\n");
	tell_room(SRM,"中年男子朗声说道：本欲带诸位去"+game_name+"寻宝何乃天有不测风云，刚才那阵巨响恐是岛上传来，估计那岛再过"+CHINESE_D->chinese_time(GAME_TIME)+"就要沉没了。\n");
	tell_room(SRM,"中年男子接着说到：这只船转眼就也要沉没，大家自谋生路吧。\n");
	tell_room(SRM,"正说着中年男子已经纵身跃入海中。\n");
	SRM->move_to_iland();
	over_time = time() + GAME_TIME;
	remove_call_out("check");
	call_out("check",2);
}

//dest all rooms
void job_over()
{
	mapping* lives;
	mixed *d;
	int i,fl;
	object f;
	
	stating = "overbusy";
	//dest
	user_list = filter_array(user_list, (:clear_data:),);
	lives = filter_array(user_list, (:$1["alive"]:) );
	if(sizeof(lives)) {
	for(i =0;i<sizeof(lives);i++)
		quit_player(lives[i]["object"],0);
	}
	
        d = get_dir(JOB_DIR,-1);
        for (i=0;i<sizeof(d);i++){
        	if (d[i][1] == -2) continue;
        	fl = strlen(d[i][0]);
        	if (d[i][1] <= 0
        	|| d[i][0][(fl-2)..(fl-1)] != ".c") continue;
    	        if (!(f = find_object(JOB_DIR+d[i][0]))) continue;
    	        destruct(f);
        }
        remove_call_out("init_start_re2");
        remove_call_out("init_start_re");
	call_out("init_start_re2",NEXT_TIME-30);
	call_out("init_start_re",NEXT_TIME);
}

void init_start_re2()
{
	CHANNEL_D->do_channel(this_object(),"rumor",HIY+"听说"+game_name+"即将重新浮出海面了。");
}

void init_start_re()
{
	stating="waitby";
}

int sort_rank(mapping rank1, mapping rank2)
{
	reset_eval_cost(); // added by snowman@SJ
	return rank2["score"]-rank1["score"];
}

string query_score(object me,int list)
{
	int t;
	string msg = HIW+CHINESE_MUD_NAME+game_name;
	if(stating == "ready"){
		t = start_time - time();
		//CHINESE_D->chinese_time(t)
		if(t>0) msg += sprintf("任务还有%s就要正式开始了。\n"NOR,CHINESE_D->chinese_time(t));
		else msg += "任务已经开始了。\n"NOR;
	}
	else if(stating == "ingame")
	{
		mapping* lives = filter_array(user_list, (:clear_data:) );
		mapping* deads = filter_array(lives, (:!$1["alive"]:) );
		//lives = filter_array(lives, (:clear_data:) );
		
		lives = filter_array(lives, (:$1["alive"]:) );
		t = over_time - time();
		if(!sizeof(lives) || t<=0) msg += "即将沉没。\n"NOR;
		else msg += sprintf("还剩 %s 人，岛再过%s后就要沉没了。\n"NOR,chinese_number(sizeof(lives)),  CHINESE_D->chinese_time(t) );
		if(list)
		{
			lives = sort_array(lives, (: sort_rank :));
			deads = sort_array(deads, (: sort_rank :));
			msg += "\n具体参与玩家资料如下：\n";
        		msg +="╭━━━━━━━━━━━━━━━━┳━━━━━━━━┳━━╮\n";
        		msg +="┃           "HIW"玩    家"NOR"             ┃ "HIW"大 智 岛 积 分"NOR" ┃"HIW"状态"NOR"┃\n";
        		//msg +="┃           "HIW"玩    家"NOR"             ┃ "HIW"大 智 岛 积 分"NOR" ┃\n";
        		msg +="┣━━━━━━━━━━━━━━━━╋━━━━━━━━╋━━┫\n";
        		if(sizeof(lives)) for(int i = 0;i<sizeof(lives);i++){
        			if(!lives[i]["object"]) continue;
        			msg += sprintf("┃%|32s┃%-16s┃%|4s┃\n",HIW+lives[i]["object"]->query("name")+"("+capitalize(lives[i]["id"])+")"NOR,HIW+chinese_number(lives[i]["score"])+NOR, (lives[i]["alive"]?HIG"活动":HIR"阵亡")+NOR  );
        		}
        		if(sizeof(deads)) for(int i = 0;i<sizeof(deads);i++){
        			if(!deads[i]["object"]) continue;
        			msg += sprintf("┃%|32s┃%-16s┃%|4s┃\n",HIW+deads[i]["object"]->query("name")+"("+capitalize(deads[i]["id"])+")"NOR,HIW+chinese_number(deads[i]["score"])+NOR, (deads[i]["alive"]?HIG"活动":HIR"阵亡")+NOR  );
        		}
        		msg +="╰━━━━━━━━━━━━━━━━┻━━━━━━━━┻━━╯\n";
		}
	}
	else if(stating == "overbusy") msg +="已经结束。\n"NOR;
	else if(stating == "waitby") msg +="正在准备重新开始。\n"NOR;
	return msg;
}

void check()
{
	//分两两个阶段
	//start init call以后开始的等待时间	
	remove_call_out("check");
		
	if(stating == "ready"){
		if(time() < start_time){
			call_out("check",4);
			return;
		}
		user_list = filter_array(user_list, (:clear_data:),);
		if(sizeof(user_list)<min_users) {
			CHANNEL_D->do_channel(this_object(),"rumor",HIY+"由于人数不足，暂时取消了去大智岛的计划！");						
			job_over();
			return;
		}
		SRM->run_sea();
		call_out("gogogo",10+random(10));
		call_out("start_war",23+random(5));
		return;
	}
		
	if(stating == "ingame")
	{
		mapping* lives = filter_array(user_list, (:clear_data:) );
		lives = filter_array(lives, (:$1["alive"]:) );
		//lives = filter_array(lives, (:clear_data:) );		
		if(time() < over_time && sizeof(lives)>1 ){
			call_out("check",4);
			return;
		}
		one_live = sizeof(lives)==1;
		job_over();
		reward();
		return;
	}	
}

void demon_tell(int i,object me,string arg)
{
	call_out("demon_tell2",i,me,arg);
}
void demon_tell2(object me,string arg)
{
	tell_object(me,arg);
}

int relay_listener(object me,string type)
{
	///死亡立刻 退出游戏给奖励
	mixed hehe;
	object mul;
	int flag = 1;
	string str;
	int i;
	int bonus;
	if(type=="die"){
		//死亡原因列表此列表内 score 加倍		
		//奖励根据存活时间
		if(stringp(hehe= me->query_temp("last_damage_from")) && member_array(hehe,deads)!=-1) flag = 2;
		//给下毒的人部分奖励 ^_^
		//打晕他的人						
		if(objectp(mul= me->query_temp("faint_by")) && userp(mul) ) {
			i = get_num(mul);
			bonus = 300*(me->query("combat_exp")/1000);
			bonus = bonus/(mul->query("combat_exp")/1000);			
			if(i!=max_users+10 && !undefinedp(user_list[i])) {
				debug_msg(sprintf("%s 得到%d score",mul->query("name"), bonus*flag ));
				user_list[i]["score"] += bonus*flag;
				flag = 1;
			}
		}
		//打死他的人
		if(objectp(hehe= me->query_temp("last_damage_from"))  && mul!=(object)hehe && userp(hehe) ) {
			i = get_num(hehe);
			bonus = 300*(me->query("combat_exp")/1000);
			bonus = bonus/(hehe->query("combat_exp")/1000);
			//
			if(i!=max_users+10 && !undefinedp(user_list[i])) {
				debug_msg(sprintf("%s 得到%d score",hehe->query("name"), bonus*flag ));
				user_list[i]["score"] += bonus*flag;
				flag = 1;
			}
		}
		/*		
		
		if(objectp(hehe= me->query_temp("last_damage_from"))  && userp(hehe) && (i = get_num(hehe))!=max_users+10 )
			if(!undefinedp(user_list[i])) {user_list[i]["score"] += 150*flag;flag = 1;}
		*/				
		if(stringp(hehe)){
			str = me->query("name")+hehe;
			if(hehe[<1..<1] != "。") str += "死了。";
		}
		if(objectp(hehe)) str = sprintf("%s被%s逐出%s了。",me->query("name"),hehe->name(),game_name);
		if(objectp(hehe) && objectp(mul) && (object)hehe!=mul ) str= sprintf("%s被%s和%s联合逐出%s了。",me->query("name"),hehe->name(),mul->name(),game_name);
		CHANNEL_D->do_channel(this_object(),"rumor",str);
		quit_player(me,1);
		//me->remove_relay_listener(type);
		// remove this listener
	}
	return 1;
}

int get_num(mixed me)
{
	int i;
	string id;
	if(!sizeof(user_list)) return max_users+10;
	
	if(objectp(me)) id = getuid(me);
	else id = (string)me;
	
	for(i=0;i<sizeof(user_list);i++) if(id==user_list[i]["id"]) break;
		
	if(i==sizeof(user_list)) return max_users+10;
	return i;
}