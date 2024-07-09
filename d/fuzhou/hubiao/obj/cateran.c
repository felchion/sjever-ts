// cateran.c
// Modify By River@SJ
// Validate By YUJ@SJ 2002-01-17

#include <ansi.h>
inherit NPC;
int do_copy(object ob);

string *title = ({
	"天河帮", "长鲸岛", "神农帮", "巨鲸帮", "海沙派", "青竹帮",
	"龙游帮", "金龙帮", "鄱阳帮", "恶虎沟", "千柳庄"
});

void create()
{
	string weapon = "null" , menpai;
	mapping name;
	int i,j;

	i = random(4);	
	j = random(40);
	name = RNAME_D->get_random_name(i);	
	
	set("title", HIW+title[random(sizeof(title))]+" "+NOR+(i?"":"女")+"帮众");
	set_name(name["name"], name["id"] + ({ "jie fei" }));
	
	set("gender", i?"男性":"女性");
	set("age", 27+random(30));
	
	
	set("long","这个拦路的劫匪是"+query("title")+"，满脸伤疤，一脸凶狠而霸道的样子。\n");

	set("attitude", "peaceful");

	set("combat_exp", 5000000);

	set("hubiao_jiefei", 1);

	set("max_qi", 20000);
	set("max_jing", 20000);
	set("max_neili", 5000);
	set("max_neili", 20000);
	set("neili", 5000);
	set("neili", 20000);
	set_temp("apply/pfm_damage", -20);
	set_temp("apply/pfm_force", -20);
	set_temp("apply/pfm_attack", -10);

	set("eff_jing", 20000);
	set("eff_jingli", 20000);
	set("jingli", 20000);
	set("max_jingli", 20000);

	set("no_ansuan",1);
	set("no_bark", 1);
	set("no_quest", 1);
	set("location", 1);

	set_skill("strike", 500);
	set_skill("cuff", 500);
	set_skill("hand", 500);
	set_skill("dodge", 500);
	set_skill("sword", 500);
	set_skill("force", 500);
	set_skill("parry", 500);
	set_skill("blade", 500);
	set_skill("hammer", 500);
	set_skill("stick", 500);
	set_skill("club", 500);
	set_skill("staff", 500);
	set_skill("literate", query("int") * 10);
	set_skill("whip", 500);
	set_skill("dagger", 500);
	set("location",1);
	/*
	set("chat_chance", 100);
	set("chat_msg", ({
	(: check :),
	}));
	*/
	set("no_get", 1);

	switch( random(30) ) {
		case 0:  // wd1
		set_skill("yinyun-ziqi", 500);
		set_skill("tiyunzong", 500);
		set_skill("taiji-jian", 500);
		set_skill("taiji-quan", 500);
		map_skill("force", "yinyun-ziqi");
		map_skill("dodge", "tiyunzong");
		map_skill("sword", "taiji-jian");
		map_skill("parry", "taiji-quan");
		map_skill("cuff", "taiji-quan");
		prepare_skill("cuff", "taiji-quan");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "taiji" :),
			(: perform_action, "cuff.ji" :),
		}));
		menpai = "武当";
		break;
		case 1:  // wd2
		set_skill("yinyun-ziqi", 500);
		set_skill("tiyunzong", 500);
		set_skill("taiji-jian", 500);
		set_skill("taiji-quan", 500);
		map_skill("force", "yinyun-ziqi");
		map_skill("dodge", "tiyunzong");
		map_skill("sword", "taiji-jian");
		map_skill("parry", "taiji-jian");
		map_skill("cuff", "taiji-quan");
		prepare_skill("cuff", "taiji-quan");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "sword.sanhuan" :),
		}));
		weapon = "sword";
		menpai = "武当";
		break;
		case 2:  // mr
		case 3:  // mr
		case 4:  // mr
		set_skill("shenyuan-gong", 500);
		set_skill("douzhuan-xingyi", 500);
		set_skill("yanling-shenfa", 500);
		set_skill("murong-jianfa", 500);
		set_skill("canhe-zhi", 500);
		map_skill("force", "shenyuan-gong");
		map_skill("dodge", "yanling-shenfa");
		map_skill("sword", "murong-jianfa");
		map_skill("parry", "canhe-zhi");
		map_skill("finger", "canhe-zhi");
		prepare_skill("finger", "canhe-zhi");
		prepare_skill("finger", "canhe-zhi");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "shenyuan" :),
			(: perform_action, "finger.ci" :),
		}));
		menpai = "姑苏慕容";
		break;
		case 5:  // hs1
		set_skill("zixia-gong", 500);
		set_skill("huashan-shenfa", 500);
		set_skill("huashan-jianfa", 500);
		set_skill("hunyuan-zhang", 500);
		map_skill("force", "zixia-gong");
		map_skill("dodge", "huashan-shenfa");
		map_skill("sword", "huashan-jianfa");
		map_skill("parry", "huashan-jianfa");
		map_skill("strike", "hunyuan-zhang");
		prepare_skill("strike", "hunyuan-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "zixia" :),
			(: perform_action, "sword.fengyi" :),
		}));
		weapon = "sword";
		menpai = "华山";
		break;
		case 6:  // mr2
		set_skill("shenyuan-gong", 500);
		set_skill("douzhuan-xingyi", 500);
		set_skill("yanling-shenfa", 500);
		set_skill("murong-jianfa", 500);
		set_skill("canhe-zhi", 500);
		map_skill("force", "shenyuan-gong");
		map_skill("dodge", "yanling-shenfa");
		map_skill("sword", "murong-jianfa");
		map_skill("parry", "douzhuan-xingyi");
		map_skill("finger", "canhe-zhi");
		prepare_skill("finger", "canhe-zhi");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "shenyuan" :),
			(: perform_action, "sword.lianhuan" :),
			(: perform_action, "parry.bishen" :),
		}));
		weapon = "sword";
		menpai = "姑苏慕容";
		break;
		case 7:  // gb1
		set_skill("huntian-qigong", 500);
		set_skill("bangjue", 500);
		set_skill("xiaoyaoyou", 500);
		set_skill("dagou-bang", 500);
		set_skill("xianglong-zhang", 500);
		map_skill("force", "huntian-qigong");
		map_skill("dodge", "xiaoyaoyou");
		map_skill("stick", "dagou-bang");
		map_skill("parry", "dagou-bang");
		map_skill("strike", "xianglong-zhang");
		prepare_skill("strike", "xianglong-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "huntian" :),
			(: perform_action, "stick.pi" :),
			(: perform_action, "stick.chan" :),
		}));
		weapon = "stick";
		menpai = "丐帮";
		break;
		case 8:  // gb2
		set_skill("huntian-qigong", 500);
		set_skill("bangjue", 500);
		set_skill("xiaoyaoyou", 500);
		set_skill("dagou-bang", 500);
		set_skill("xianglong-zhang", 500);
		map_skill("force", "huntian-qigong");
		map_skill("dodge", "xiaoyaoyou");
		map_skill("stick", "dagou-bang");
		map_skill("parry", "xianglong-zhang");
		map_skill("strike", "xianglong-zhang");
		prepare_skill("strike", "xianglong-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "strike.paiyun" :),
		}));
		menpai = "丐帮";
		break;
		case 9:  // thd1
		set_skill("bihai-chaosheng", 500);
		set_skill("qimen-bagua", 500);
		set_skill("suibo-zhuliu", 500);
		set_skill("yuxiao-jian", 500);
		set_skill("tanzhi-shentong", 500);
		set_skill("lanhua-shou", 500);
		set_skill("luoying-zhang", 500);
		set_skill("xuanfeng-tui", 500);
		map_skill("force", "bihai-chaosheng");
		map_skill("hand", "lanhua-shou");
		map_skill("leg", "xuanfeng-tui");
		map_skill("strike", "luoying-zhang");
		map_skill("dodge", "suibo-zhuliu");
		map_skill("sword", "yuxiao-jian");
		map_skill("parry", "yuxiao-jian");
		map_skill("finger", "tanzhi-shentong");
		prepare_skill("finger", "tanzhi-shentong");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "sword.feiying" :),
			(: perform_action, "sword.qimen" :),
		}));
		set("thd/perform",1023);
		weapon = "xiao";
		menpai = "桃花岛";
		break;
		case 10: // thd2
		set_skill("bihai-chaosheng", 500);
		set_skill("qimen-bagua", 500);
		set_skill("suibo-zhuliu", 500);
		set_skill("yuxiao-jian", 500);
		set_skill("throwing", 500);
		set_skill("tanzhi-shentong", 500);
		map_skill("force", "bihai-chaosheng");
		map_skill("dodge", "suibo-zhuliu");
		map_skill("sword", "yuxiao-jian");
		map_skill("parry", "tanzhi-shentong");
		map_skill("finger", "tanzhi-shentong");
		map_skill("throwing", "tanzhi-shentong");
		prepare_skill("finger", "tanzhi-shentong");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "throwing.huayu" :),
		}));
		set("thd/perform",1023);
		weapon = "shizi";
		menpai = "桃花岛";
		break;
		case 11: // mr1
		set_skill("shenyuan-gong", 500);
		set_skill("douzhuan-xingyi", 500);
		set_skill("yanling-shenfa", 500);
		set_skill("murong-daofa", 500);
		set_skill("canhe-zhi", 500);
		set_skill("xingyi-zhang", 500);
		map_skill("strike", "xingyi-zhang");
		map_skill("force", "shenyuan-gong");
		map_skill("dodge", "yanling-shenfa");
		map_skill("blade", "murong-daofa");
		map_skill("parry", "murong-daofa");
		map_skill("finger", "canhe-zhi");
		prepare_skill("finger", "canhe-zhi");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "shenyuan" :),
			(: perform_action, "blade.daoying" :),
		}));
		weapon = "blade";
		menpai = "姑苏慕容";
		break;
		case 12: // gumu1
		set_skill("yunu-xinjing", 500);
		set_skill("yunu-shenfa", 500);
		set_skill("xuantie-jianfa", 500);
		set_skill("anran-zhang", 500);
		map_skill("force", "yunu-xinjing");
		map_skill("dodge", "yunu-shenfa");
		map_skill("sword", "xuantie-jianfa");
		map_skill("parry", "xuantie-jianfa");
		map_skill("strike", "anran-zhang");
		prepare_skill("strike", "anran-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "sword.wujian" :),
			(: perform_action, "sword.haichao" :),
		}));
		weapon = "sword";
		menpai = "古墓";
		if (random(2)) set("env/玄铁剑法","海潮");
		else set("env/玄铁剑法","汹涌");
		break;
		case 13: // emei1
		set_skill("linji-zhuang", 500);
		set_skill("anying-fuxiang", 500);
		set_skill("huifeng-jian", 500);
		set_skill("sixiang-zhang", 500);
		map_skill("force", "linji-zhuang");
		map_skill("dodge", "anying-fuxiang");
		map_skill("sword", "huifeng-jian");
		map_skill("parry", "huifeng-jian");
		map_skill("strike", "sixiang-zhang");
		prepare_skill("strike", "sixiang-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "wuwo" :),
			(: perform_action, "sword.mie" :),
			(: perform_action, "sword.jue" :),
		}));
		weapon = "sword";
		menpai = "峨嵋";
		break;
		case 14: // emei2
		set_skill("linji-zhuang", 500);
		set_skill("anying-fuxiang", 500);
		set_skill("yanxing-daofa", 500);
		set_skill("sixiang-zhang", 500);
		map_skill("force", "linji-zhuang");
		map_skill("dodge", "anying-fuxiang");
		map_skill("blade", "yanxing-daofa");
		map_skill("parry", "yanxing-daofa");
		map_skill("strike", "sixiang-zhang");
		prepare_skill("strike", "sixiang-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "wuwo" :),
			(: perform_action, "blade.feiyan" :),
		}));
		weapon = "blade";
		menpai = "峨嵋";
		break;
		case 15: // mj1
		set_skill("shenghuo-shengong", 500);
		set_skill("qiankun-danuoyi", 500);
		set_skill("piaoyi-shenfa", 500);
		set_skill("shenghuo-lingfa", 500);
		set_skill("hanbing-mianzhang", 500);
		map_skill("force", "shenghuo-shengong");
		map_skill("dodge", "piaoyi-shenfa");
		map_skill("dagger", "shenghuo-lingfa");
		map_skill("parry", "qiankun-danuoyi");
		map_skill("cuff", "shenghuo-lingfa");
		prepare_skill("cuff", "shenghuo-lingfa");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "yinfeng" :),
			(: perform_action, "dagger.tisha" :),
		}));
		weapon = "tieling";
		menpai = "明教";
		break;
		case 16: // gm2
		set_skill("yinsuo-jinling", 500);
		set_skill("yunu-jianfa", 500);
		set_skill("yunu-shenfa", 500);
		set_skill("meinu-quanfa", 500);
		set_skill("yunu-xinjing", 500);
		map_skill("force", "yunu-xinjing");
		map_skill("dodge", "yunu-shenfa");
		map_skill("sword", "yunu-jianfa");
		map_skill("whip", "yinsuo-jinling");
		map_skill("parry", "meinu-quanfa");
		map_skill("cuff", "meinu-quanfa");
		prepare_skill("cuff", "meinu-quanfa");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "cuff.baimei" :),
		}));
		menpai = "古墓派";
		break;
		case 17: // dali1
		set_skill("qiantian-yiyang", 500);
		set_skill("tianlong-xiang", 500);
		set_skill("duanjia-jianfa", 500);
		set_skill("qingyan-zhang", 500);
		map_skill("force", "qiantian-yiyang");
		map_skill("dodge", "tianlong-xiang");
		map_skill("sword", "duanjia-jianfa");
		map_skill("parry", "duanjia-jianfa");
		map_skill("strike", "qingyan-zhang");
		prepare_skill("strike", "qingyan-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "sword.fenglei" :),
		}));
		weapon = "sword";
		menpai = "西南大理";
		break;
		case 18: // tls
		case 19: // gumu2
		set_skill("yinsuo-jinling", 500);
		set_skill("yunu-jianfa", 500);
		set_skill("yunu-shenfa", 500);
		set_skill("meinu-quanfa", 500);
		set_skill("yunu-xinjing", 500);
		map_skill("force", "yunu-xinjing");
		map_skill("dodge", "yunu-shenfa");
		map_skill("sword", "yunu-jianfa");
		map_skill("whip", "yinsuo-jinling");
		map_skill("parry", "yinsuo-jinling");
		map_skill("cuff", "meinu-quanfa");
		prepare_skill("cuff", "meinu-quanfa");
		set("yueyin", 1);
		set("gmsanwu", 1);
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "whip.yueyin" :),
			(: perform_action, "whip.sanwu" :),
		}));
		weapon = "whip";
		menpai = "古墓派";
		break;
		case 20: //gumu3
		set_skill("anran-zhang", 500);
		set_skill("yunu-xinjing", 500);
		set_skill("yunu-shenfa", 500);
		map_skill("force", "yunu-xinjing");
		map_skill("dodge", "yunu-shenfa");
		map_skill("parry", "anran-zhang");
		map_skill("strike", "anran-zhang");
		prepare_skill("strike", "anran-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "strike.jishi" :),
		}));
		menpai = "古墓派";
		break;
		case 21: // tls
		set_skill("kurong-changong", 500);
		set_skill("tianlong-xiang", 500);
		set_skill("pangen-fu", 500);
		set_skill("axe", 500);
		map_skill("force", "kurong-changong");
		map_skill("dodge", "tianlong-xiang");
		map_skill("axe", "pangen-fu");
		map_skill("parry", "pangen-fu");
		map_skill("finger", "yiyang-zhi");
		prepare_skill("finger","yiyang-zhi");
		set("chat_chance_combat", 26);
		set("chat_msg_combat", ({
			(: perform_action, "axe.jingtian" :),
		}));
		weapon = "axe";
		menpai = "大理天龙寺";
		break;
		case 22: // kl2
		case 23: // kl3
		case 24: // kl4
		set_skill("xuantian-wuji", 500);
		set_skill("taxue-wuhen", 500);
		set_skill("xunlei-jian", 500);
		set_skill("kunlun-zhang", 500);
		map_skill("force", "xuantian-wuji");
		map_skill("dodge", "taxue-wuhen");
		map_skill("parry", "xunlei-jian");
		map_skill("sword", "xunlei-jian");
		prepare_skill("strike", "kunlun-zhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "sword.xunlei" :),
		}));
		menpai = "昆仑派";
		weapon = "sword";
		break;
		case 25: // tls3
		set_skill("kurong-changong", 500);
		set_skill("tianlong-xiang", 500);
		set_skill("yiyang-zhi", 500);
		map_skill("force", "kurong-changong");
		map_skill("dodge", "tianlong-xiang");
		map_skill("parry", "kurong-changong");
		map_skill("finger", "yiyang-zhi");
		prepare_skill("finger", "yiyang-zhi");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: perform_action, "finger.sanmai" :),
			(: perform_action, "finger.sandie" :),
		}));
		menpai = "大理天龙寺";
		break;
		case 26: // tz1
		set_skill("guiyuan-tunafa", 500);
		set_skill("shuishangpiao", 500);
		set_skill("tiezhang-zhangfa", 500);
		map_skill("force", "guiyuan-tuanfa");
		map_skill("dodge", "shuishangpiao");
		map_skill("parry", "tiezhang-zhangfa");
		map_skill("strike", "tiezhang-zhangfa");
		prepare_skill("strike", "tiezhang-zhangfa");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "juli" :),
			(: perform_action, "dodge.piao" :),
			(: perform_action, "strike.heyi" :),
			(: perform_action, "strike.duoming" :),
			(: perform_action, "strike.heisha" :),
		}));
		menpai = "铁掌派";
		break;
		case 27: // sld
		case 28: // sld1
		set_skill("dulong-dafa", 500);
		set_skill("youlong-shenfa", 500);
		set_skill("huagu-mianzhang", 500);
		map_skill("force", "dulong-dafa");
		map_skill("dodge", "youlong-shenfa");
		map_skill("parry", "huagu-mianzhang");
		map_skill("strike", "huagu-mianzhang");
		prepare_skill("strike", "huagu-mianzhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "wudi" :),
			(: perform_action, "strike.bujue" :),
		}));
		menpai = "神龙教";
		break;
		case 29: // sld2
		set_skill("dulong-dafa", 500);
		set_skill("youlong-shenfa", 500);
		set_skill("huagu-mianzhang", 500);
		set_skill("tenglong-bifa", 500);
		map_skill("dagger", "tenglong-bifa");
		map_skill("force", "dulong-dafa");
		map_skill("dodge", "youlong-shenfa");
		map_skill("parry", "tenglong-bifa");
		map_skill("strike", "huagu-mianzhang");
		prepare_skill("strike", "huagu-mianzhang");
		set("chat_chance_combat", 15);
		set("chat_msg_combat", ({
			(: exert_function, "wudi" :),
			(: perform_action, "dagger.xiaolian" :),
			(: perform_action, "dagger.guifei" :),
			(: perform_action, "dagger.diqing" :),
		}));
		menpai = "神龙教";
		weapon = "dagger";
		break;
		default: // others      BUG!!!
		set("long","武功设置错误，BUG！！！");
	}
	set("weapon" , weapon);
	setup();
	if( weapon != "null" ) carry_object(BINGQI_D(weapon))->wield();
	carry_object(ARMOR_D("yexing-yi"))->wear();
	call_out("dest", 300);
		
	
}

void open_cart()
{
	object cart,me=this_object();
	if (objectp(cart=me->query_temp("target"))){
		command("chat 嘿嘿，今天又捞了一票。");
		tell_room(environment(cart),HIR"帮主推着镖车走了。\n"NOR);
		cart->fail();
	}
	me->dest();
}

int do_copy(object ob)
{
	mapping skills,name;
	string *sk;
	object me,cart;
	int lv,exp,i;

	me = this_object();
	lv = ob->query("max_pot") -100;
	
	exp = ob->query("combat_exp");

	me->set("combat_exp", exp);
	me->set("max_qi", ob->query("max_qi")*3/2);
	me->set("eff_qi", me->query("max_qi"));
	me->set("qi", me->query("max_qi"));
	me->set("max_jing", ob->query("max_jing"));
	me->set("eff_jing", ob->query("max_jing"));
	me->set("jing", ob->query("max_jing"));
	me->set("max_neili", ob->query("max_neili"));
	me->set("neili", ob->query("neili"));
	if( me->query("neili") < me->query("max_neili"))
		me->set("neili", me->query("max_neili"));
	me->set("eff_jingli", ob->query("eff_jingli"));
	me->set("jingli", ob->query("jingli"));
	if( me->query("jingli") < me->query("eff_jingli"))
		me->set("jingli", me->query("eff_jingli"));
		
	me->set("jiali", me->query_skill("force")/5);
	
	//匪帮帮主出现	
	cart = me->query_temp("target");
	
	if(cart->query("cateran")>5 
	&& cart->query("cateran")<16
	&& cart->query("boss")<2
	&& !random(15)) {
		set("is_head",1);
		
		i = random(4);
		name = RNAME_D->get_random_name(i);	
		set("title", HIW+title[random(sizeof(title))]+HIR+" 帮主"+NOR);		
		set_name(name["name"], name["id"] + ({ "bang zhu" }));
		set("long","这个拦路的劫匪是"+query("title")+"，满脸伤疤，一脸凶狠而霸道的样子。\n");		
		
		set("chat_chance_combat", 100);
		lv = lv + 5 + random(20);	
//		if (!random())
//			carry_object("/clone/misc/yushi");
		set("con",20+random(5));
		set("int",20+random(5));
		set("str",20+random(5));
		set("dex",20+random(5));		
		if (!random(10))
			set("double_attack",1);
			
		if (!random(3)) {
			set_temp("apply/pfm_parry", 60);
			set_temp("apply/pfm_dodge", 60);
			set_temp("apply/pfm_attack",90);
			set_temp("apply/pfm_damage",100);	
			set_temp("apply/pfm_force", 100);	
			set_temp("apply/damage",100);
		}
		else {
			set_temp("apply/damage",200);
			set_temp("apply/pfm_parry", 90);
			set_temp("apply/pfm_dodge", 90);
			set_temp("apply/pfm_damage",150);	
			set_temp("apply/pfm_force", 150);	
			set_temp("apply/pfm_attack",120);
		}		
		me->set("combat_exp", lv/10*lv*lv);
		me->set("max_qi", ob->query("max_qi")*5/2);
		me->set("eff_qi", me->query("max_qi"));
		me->set("qi", me->query("max_qi"));
		me->set("max_jing", ob->query("max_jing")*5/2);
		me->set("eff_jing", ob->query("max_jing")*5/2);
		me->set("jing", ob->query("max_jing")*5/2);
		me->set("max_neili", ob->query("max_neili")*5/2);
		me->set("neili", me->query("max_neili")*2);
		me->set("eff_jingli", ob->query("eff_jingli")*5/2);
		me->set("jingli", me->query("eff_jingli")*2);
		me->set("jiali", 100+me->query_skill("force")/5);	
	
		remove_call_out("dest");
		call_out("open_cart",900);
	}	
	else {
		me->set("combat_exp", lv/10*lv*lv);
		me->set("max_qi", ob->query("max_qi")*3/2);
		me->set("eff_qi", me->query("max_qi"));
		me->set("qi", me->query("max_qi"));
		me->set("max_jing", ob->query("max_jing"));
		me->set("eff_jing", ob->query("max_jing"));
		me->set("jing", ob->query("max_jing"));
		me->set("max_neili", ob->query("max_neili"));
		me->set("neili", ob->query("neili"));
		if( me->query("neili") < me->query("max_neili"))
			me->set("neili", me->query("max_neili"));
		me->set("eff_jingli", ob->query("eff_jingli"));
		me->set("jingli", ob->query("jingli"));
		if( me->query("jingli") < me->query("eff_jingli"))
			me->set("jingli", me->query("eff_jingli"));
			
		me->set("jiali", me->query_skill("force")/5);
	}
	skills = me->query_skills();
	if (mapp(skills)) {
		sk = keys(skills);
		for (i=0;i<sizeof(sk);i++)
		skills[sk[i]] = lv;
	}
}

void init()
{
	object cart,me=this_player();

	::init();
	cart = query_temp("target");
	if (cart && me->query_temp("protecting" ) == cart ) {
		if (!query("is_head")) {
			kill_ob(me);
			me->fight_ob(this_object());
		}
		else {			
			if (!query("chat_msg")) {
				command("chat* rob");
				set("chat_msg",1);
			}
		}
	}
}

void kill_ob(object ob)
{
	object me = this_object();

	if(!objectp(me->query_temp("target"))
	|| (me->query_temp("target") != ob->query_temp("protecting"))){
		me->remove_enemy(ob);
		ob->remove_killer(me);
		return;
	}
	::kill_ob(ob);
}

void fight_ob(object ob)
{
	if (!objectp(query_temp("target"))
	|| ob->query_temp("protecting") != query_temp("target")) {
		command("say 走远点，老子正忙着呢。");
		ob->remove_killer(this_object());
	}
	else ::fight_ob(ob);
}

void dest()
{
	object ob = this_object();

	if (environment(ob)) message_vision("$N匆匆忙忙地离开了。\n",ob);
	destruct(ob);
}
